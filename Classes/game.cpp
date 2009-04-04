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
#include "region.h"
#include "npc.h"
#include "game.h"

#define MAX_CHARACTERS_PER_FLEET 6
#define MAX_TEXTURES 16
#define NUM_INPUT_WAYPOINTS 128

static int frameCount = 0;
float   selectedLat, selectedLon;

// regions will be ROW major...
Region  region[NUM_REGION_ROWS * NUM_REGION_COLS];


Point2D inputWaypoints[NUM_INPUT_WAYPOINTS];
int     numInputWaypoints = 0;
int     selectedEntityIndex = -1;
bool    clearSelectedEntity = false;
NPC     enemyFleet[MAX_CHARACTERS_PER_FLEET];
GLuint  textures[MAX_TEXTURES];

void Init()
{
	srand(time(NULL));
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
	
	Point2D calcUrPoint, calcLlPoint;
	float width  = 360.0 / (float)NUM_REGION_COLS;
	float height = 180.0 / (float)NUM_REGION_ROWS;
	
	for(int i=0; i < NUM_REGION_ROWS; i++)
	{
		for(int j=0; j < NUM_REGION_COLS; j++)
 		{
			calcLlPoint.SetX((j * width) - 180.0);
			calcLlPoint.SetY((i * height) - 90.0);
			
			calcUrPoint.SetX((j * width - 180.0) + width);
			calcUrPoint.SetY((i * height - 90.0) + height);
			
			region[i * NUM_REGION_ROWS + j].SetExtents(calcLlPoint, calcUrPoint);
			region[i * NUM_REGION_ROWS + j].Initialize();
		}
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

void SetClearSelectedEntity()
{
	clearSelectedEntity = true;

}

static void ProcessClearSelectedEntity()
{
	if(clearSelectedEntity)
	{
		selectedEntityIndex = -1;
		clearSelectedEntity = false;
		numInputWaypoints = 0; // reset the number of pending waypoints
	}
}

void SetInputWayPoint(float lat, float lon)
{
	if(numInputWaypoints >= NUM_INPUT_WAYPOINTS)
		return;

	inputWaypoints[numInputWaypoints].Set(lon, lat);
	numInputWaypoints++;
}

static void ProcessInputWaypoints()
{
	if(selectedEntityIndex != -1)
	{
	for(int i=0; i < numInputWaypoints; i++)
	{
		enemyFleet[selectedEntityIndex].AddWayPoint(inputWaypoints[i].GetY(), inputWaypoints[i].GetX());
	}
	numInputWaypoints = 0;
	}
}

static void ProcessEntitySelection()
{
	if(selectedLat != FLT_MIN && selectedLon != FLT_MIN)
	{
		Point2D loc;
		for(int i=0; i < MAX_CHARACTERS_PER_FLEET; i++)
		{
			enemyFleet[i].GetPosition(loc);
			if(loc.GetX() <= selectedLon+ENTITY_SIZE &&
			   loc.GetX() >= selectedLon-ENTITY_SIZE &&
			   loc.GetY() <= selectedLat+ENTITY_SIZE &&
			   loc.GetY() >= selectedLat-ENTITY_SIZE)
			{
				enemyFleet[i].ClearWaypoints();
				enemyFleet[i].SetStatus(NPC_SELECTED);
				selectedEntityIndex = i;
				break;	
			}
			
		}
		// we have a valid lat lon from a selection, see if we're over an entity and tag him as selected
		selectedLat = selectedLon = FLT_MIN;
	}
}

static void RenderRegions()
{
	Point2D ll, ur;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	for(int i=0; i < NUM_REGION_ROWS * NUM_REGION_COLS; i++)
	{
		GLfloat regionTriPts[2 * 4];
		region[i].GetExtents(ll, ur);
		regionTriPts[0] = ll.GetX();
		regionTriPts[1] = ll.GetY();
		regionTriPts[2] = ur.GetX();
		regionTriPts[3] = ll.GetY();
		regionTriPts[4] = ll.GetX();
		regionTriPts[5] = ur.GetY();
		regionTriPts[6] = ur.GetX();
		regionTriPts[7] = ur.GetY();

		if(region[i].GetStatus() & REGION_AFFILIATION_FRIENDLY)
			glColor4f(0.0, 0.0, 1.0, 0.3);
		else if(region[i].GetStatus() & REGION_AFFILIATION_FOE)
			glColor4f(1.0, 0.0, 0.0, 0.3);
			
		glVertexPointer(2, GL_FLOAT, 0, regionTriPts);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	glDisable(GL_BLEND);
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
				x+ENTITY_SIZE, y,  -0.5,
				x, y+ENTITY_SIZE,  -0.5,
				x+ENTITY_SIZE, y+ENTITY_SIZE, -0.5
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
				GLfloat *arrToRender = new GLfloat[waypoints.size() * 2];
				int curr = 0;
				std::list<Point2D>::iterator itr = waypoints.begin();
				while(itr != waypoints.end())
				{
					arrToRender[2 * curr + 0] = itr->GetX();
					arrToRender[2 * curr + 1] = itr->GetY();
					curr++;
					itr++;
				}
				glColor4f(0.0, 0.0, 1.0, 0.0);
				glVertexPointer(2, GL_FLOAT, 0, arrToRender);
				glDrawArrays(GL_LINE_STRIP, 0, curr);
				delete[] arrToRender;
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
	ProcessInputWaypoints();
	ProcessClearSelectedEntity();
	
	for(int i=0; i < MAX_CHARACTERS_PER_FLEET; i++)
	{
		enemyFleet[i].Tick();
	}
	
	RenderWorld();
	RenderCharacters();
	RenderRegions();
}
