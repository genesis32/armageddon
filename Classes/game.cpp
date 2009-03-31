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

#include "font.h"
#include "npc.h"
#include "game.h"

#define MAX_CHARACTERS_PER_FLEET 6
#define MAX_TEXTURES 16

NPC    enemyFleet[MAX_CHARACTERS_PER_FLEET];

GLuint textures[MAX_TEXTURES];

void Init()
{
	for(int i=0; i < MAX_TEXTURES; i++)
	{
		textures[i] = -1;
	}
	
	Point2D  initialPoint;
	Vector2D initialDirection;
	for(int i=0; i < MAX_CHARACTERS_PER_FLEET; i++)
	{
		initialPoint.SetX(0.0);
		initialPoint.SetY(0.0);
		
		initialDirection.SetXMagnitude(-1.0);
		initialDirection.SetYMagnitude(-1.0);
		
		enemyFleet[i].SetStatus(NPC_ALIVE);
		enemyFleet[i].SetDirection(initialDirection);
		enemyFleet[i].SetSpeed(1.0);
		enemyFleet[i].SetPosition(initialPoint);
	}
	
}

void AddTexture(int textureId, GLuint glId)
{
	textures[textureId] = glId;
}

static void RenderCharacters()
{
	Point2D ptToRender;
	static float offset = 0.0f;
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
			
			glVertexPointer(3, GL_FLOAT, 0, verts);
			glEnableClientState(GL_VERTEX_ARRAY);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	}
	offset += 0.5;
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
	for(int i=0; i < MAX_CHARACTERS_PER_FLEET; i++)
	{
		enemyFleet[i].Tick();
	}
	
	RenderWorld();
	RenderCharacters();
}
