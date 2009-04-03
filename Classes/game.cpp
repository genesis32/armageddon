/*
 *  game.cpp
 *  Armageddon
 *
 *  Created by ddm on 3/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#include <list>
#include <float.h>
#include "font.h"
#include "npc.h"
#include "game.h"

#define MAX_CHARACTERS_PER_FLEET 6
#define MAX_TEXTURES 16

static int frameCount = 0;
float  selectedLat, selectedLon;
NPC    enemyFleet[MAX_CHARACTERS_PER_FLEET];
GLuint textures[MAX_TEXTURES];

void Init()
{
	for(int i=0; i < MAX_TEXTURES; i++)
	{
		textures[i] = -1;
	}
	
	Point2D  initialPoint;
	
	float startLon = -165.0; // evenly space for testing...
	for(int i=0; i < MAX_CHARACTERS_PER_FLEET; i++)
	{
		initialPoint.SetX(startLon);
		initialPoint.SetY(0.0);
		startLon += 360.0 / 6.0;

		enemyFleet[i].SetStatus(NPC_ALIVE);
		enemyFleet[i].SetSpeed(0.25);
		enemyFleet[i].SetPosition(initialPoint);
	}
	selectedLat = selectedLon = FLT_MIN;
}

void AddTexture(int textureId, GLuint glId)
{
	textures[textureId] = glId;
}

void SetEntitySelection(float lat, float lon)
{
	selectedLat = lat;
	selectedLon = lon;
}

static void ProcessEntitySelection()
{
	if(selectedLat != FLT_MIN && selectedLon != FLT_MIN)
	{
		// we have a valid lat lon from a selection, see if we're over an entity and tag him as selected
		enemyFleet[0].SetStatus(NPC_SELECTED);
		selectedLat = selectedLon = FLT_MIN;
	}
}

static void RenderCharacters()
{
	Point2D ptToRender;
	for(int i=0; i < MAX_CHARACTERS_PER_FLEET; i++)
	{
		if(enemyFleet[i].GetStatus() & NPC_ALIVE)
		{
			enemyFleet[i].GetPosition(ptToRender);

			float x = ptToRender.GetX();
			float y = ptToRender.GetY();
			GLfloat verts[] = {
				x, y,   -0.5,
				x+10.0, y,  -0.5,
				x, y+10.0,  -0.5,
				x+10.0, y+10.0, -0.5
			};
			
			if(enemyFleet[i].GetStatus() & NPC_SELECTED)
			{
				if(frameCount < 30)
					glColor4f(0.0, 1.0, 1.0, 0.0);
				else
					glColor4f(1.0, 0.0, 0.0, 0.0);					
			}
			else
			{
				glColor4f(0.0, 1.0, 0.0, 0.0);
			}
				

			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0, verts);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			
			// now render his waypoints...
			std::list<Point2D> waypoints;
			enemyFleet[i].GetWaypoints(waypoints);
			if(!waypoints.empty())
			{
				GLfloat *arrToRender = new GLfloat[waypoints.size() * 3];
				int curr = 0;
				std::list<Point2D>::iterator itr = waypoints.begin();
				while(itr != waypoints.end())
				{
					arrToRender[2 * curr + 0] = itr->GetX();
					arrToRender[2 * curr + 1] = itr->GetY();
					arrToRender[2 * curr + 2] = -0.3;
					curr++;
					itr++;
				}
				glColor4f(0.0, 0.0, 1.0, 0.0);
				glPointSize(10.0);
				glVertexPointer(3, GL_FLOAT, 0, arrToRender);
				glDrawArrays(GL_POINTS, 0, curr);
			}
		}
	}
}

static void RenderWorld()
{
	const GLfloat squareVertices[] = {
        -180.0f, -90.0f, -0.5,
        180.0f,  -90.0f, -0.5,
        -180.0f,  90.0f, -0.5,
        180.0f,   90.0f, -0.5
    };
	
	const GLfloat squareTexCoords[] = {
		0.0, 0.625,
		0.9375, 0.625,
		0.0, 0.0,
		0.9375, 0.0
    };
	
	glEnable(GL_TEXTURE_2D);
		
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
	// this is what rotates the view when the oreintation changes.
	glRotatef(90.0, 0.0, 0.0, 1.0);
    glOrthof(-180.0f, 180.0f, -90.0, 90.0f, -1.0f, 1.0f);
	
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
	
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	
	glBindTexture(GL_TEXTURE_2D, textures[MAP_TEXTURE_ID]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	
    glVertexPointer(3, GL_FLOAT, 0, squareVertices);
    glEnableClientState(GL_VERTEX_ARRAY);
	
	glTexCoordPointer(2, GL_FLOAT, 0, squareTexCoords);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	glBindTexture(GL_TEXTURE_2D, textures[FONT_TEXTURE_ID]);
	
	SetFontSize(8);
	DrawFontString(-180, 78, "Armageddon");	
}



void GameTick()
{
	frameCount = frameCount > 60 ? 0 : frameCount + 1;
	
	ProcessEntitySelection();
	for(int i=0; i < MAX_CHARACTERS_PER_FLEET; i++)
	{
		enemyFleet[i].Tick();
	}
	
	RenderWorld();
	RenderCharacters();
}
