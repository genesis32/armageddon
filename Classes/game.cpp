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
#include <math.h>
#include <float.h>
#include "font.h"
#include "region.h"
#include "npc.h"
#include "game.h"
#include "transforms.h"

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
NPC     friendlyFleet[MAX_CHARACTERS_PER_FLEET];
NPC     enemyFleet[MAX_CHARACTERS_PER_FLEET];

GLuint  textures[MAX_TEXTURES];

static void RenderEntity(NPC &entity);

int GetRegionForPosition(const Point2D &pos)
{
	int row = (int)floor(((pos.GetY() + 90.0) / 180.0) * (float)NUM_REGION_ROWS);
	int col = (int)floor(((pos.GetX() + 180.0) / 360.0) * (float)NUM_REGION_COLS);
	
	int regionIdx = row * NUM_REGION_COLS + col;
	
	return regionIdx;
}

void InitFleets()
{

	NPC *inputFleet[2] = { friendlyFleet, enemyFleet }; 

	uint32_t affiliationCode = 0;
	for(int i=0; i < 2; i++)
	{
		float initialLon;
		if(inputFleet[i] == enemyFleet)
		{
			initialLon = 155.0;
			affiliationCode = NPC_AFFILIATION_FOE;
		}
		else
		{
			initialLon = -175.0;
			affiliationCode = NPC_AFFILIATION_FRIENDLY;
		}
		
		float startLat = -80.0; // evenly space for testing...
		Point2D  initialPoint;	
		for(int j=0; j < MAX_CHARACTERS_PER_FLEET; j++)
		{
			if(j == 0)
				inputFleet[i][j].SetStatus(NPC_BOMBER);
			
			initialPoint.SetX(initialLon);
			initialPoint.SetY(startLat);
			startLat += (180.0 / (float)MAX_CHARACTERS_PER_FLEET);
			
			// temp code here to init enemy movement....
			if(inputFleet[i] == enemyFleet)
			{
				Vector2D vec(-1.0, 0.0);
				inputFleet[i][j].SetDirection(vec);
			}
			
			
			inputFleet[i][j].SetStatus(NPC_ALIVE | affiliationCode);
			inputFleet[i][j].SetPosition(initialPoint);
		}
	}
}

void Init()
{
	srand(time(NULL));
	for(int i=0; i < MAX_TEXTURES; i++)
	{
		textures[i] = -1;
	}
	
	InitFleets();
	InitFleets();
	
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
		friendlyFleet[selectedEntityIndex].UnsetStatus(NPC_SELECTED);
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
		friendlyFleet[selectedEntityIndex].AddWayPoint(inputWaypoints[i].GetY(), inputWaypoints[i].GetX());
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
			friendlyFleet[i].GetPosition(loc);

			if(selectedLon <= loc.GetX()+ENTITY_WIDTH_RADIUS &&
			   selectedLon >= loc.GetX()-ENTITY_WIDTH_RADIUS &&
			   selectedLat <= loc.GetY()+ENTITY_HEIGHT_RADIUS &&
			   selectedLat >= loc.GetY()-ENTITY_HEIGHT_RADIUS) 
			{
				friendlyFleet[i].ClearWaypoints();
				friendlyFleet[i].SetStatus(NPC_SELECTED);
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
	for(int i=0; i < NUM_REGION_ROWS * NUM_REGION_COLS; i++)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

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

		if(region[i].GetStatus() & REGION_WARNING_INCOMING && frameCount < 30)
			glColor4f(1.0, 0.0, 0.0, 0.8);
		else if(region[i].GetStatus() & REGION_AFFILIATION_FRIENDLY)
			glColor4f(0.0, 0.0, 1.0, 0.3);
		else if(region[i].GetStatus() & REGION_AFFILIATION_FOE)
			glColor4f(1.0, 0.0, 0.0, 0.3);
		
		glVertexPointer(2, GL_FLOAT, 0, regionTriPts);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisable(GL_BLEND);
		
		// draw a dot for the main base..
		if(frameCount < 30)
		{
			glColor4f(0.9, 0.9, 0.9, 1.0);
			glPointSize(8.0);
			Point2D baseLocation = region[i].GetMainBaseLocation();
			GLfloat mainBasePt[] = { baseLocation.GetX(), baseLocation.GetY() };
			glVertexPointer(2, GL_FLOAT, 0, mainBasePt);
			glDrawArrays(GL_POINTS, 0, 1);
		}
	}

}

static void RenderEnemyCharacters()
{
	Point2D ptToRender;
	for(int i=0; i < MAX_CHARACTERS_PER_FLEET; i++)
	{
		if(enemyFleet[i].GetStatus() & NPC_ALIVE)
		{
			RenderEntity(enemyFleet[i]);
		}			
	}
}

static void RenderEntity(NPC &entity)
{
	Point2D ptToRender;
	entity.GetPosition(ptToRender);
	
	float x = ptToRender.GetX();
	float y = ptToRender.GetY();
	
/*	
	GLfloat verts[] = {
		x-ENTITY_WIDTH_RADIUS, y-ENTITY_HEIGHT_RADIUS,   -0.5,
		x+ENTITY_WIDTH_RADIUS, y-ENTITY_HEIGHT_RADIUS,  -0.5,
		x-ENTITY_WIDTH_RADIUS, y+ENTITY_HEIGHT_RADIUS,  -0.5,
		x+ENTITY_WIDTH_RADIUS, y+ENTITY_HEIGHT_RADIUS, -0.5
	};
 */

	GLfloat verts[] = {
		-ENTITY_WIDTH_RADIUS, -ENTITY_HEIGHT_RADIUS,   -0.5,
		 ENTITY_WIDTH_RADIUS,  -ENTITY_HEIGHT_RADIUS,  -0.5,
		-ENTITY_WIDTH_RADIUS, ENTITY_HEIGHT_RADIUS,  -0.5,
		 ENTITY_WIDTH_RADIUS,  ENTITY_HEIGHT_RADIUS, -0.5
	};
	
	
	GLfloat min, max;
	if(entity.GetStatus() & NPC_BOMBER)
	{
		min = 0;
		max = 64.0/256*2.0;
	}
	else
	{
		min = 0.0;
		max = (64.0/256.0);
	}
	

	GLfloat textcoords[] = {
		min, max,
		max, max,
		min, min,
		max, min
	};
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textures[SPRITES_TEXTURE_ID]);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, verts);
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, textcoords);
	
	glPushMatrix();
	glTranslatef(x, y, 0);
	glRotatef(entity.GetCurrRotationAngle(), 0.0, 0.0, 1.0);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);	

	glPopMatrix();
	
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
}

static void RenderFriendlyCharacters()
{
	Point2D ptToRender;
	for(int i=0; i < MAX_CHARACTERS_PER_FLEET; i++)
	{
		if(friendlyFleet[i].GetStatus() & NPC_ALIVE)
		{
			friendlyFleet[i].GetPosition(ptToRender);
			
			float x = ptToRender.GetX();
			float y = ptToRender.GetY();
			
			// render his waypoints...
			std::list<Point2D> waypoints;
			friendlyFleet[i].GetWaypoints(waypoints);
			if(!waypoints.empty())
			{
				GLfloat *arrToRender = new GLfloat[(waypoints.size() + 1) * 2];
				int curr = 1;
				std::list<Point2D>::iterator itr = waypoints.begin();
				arrToRender[0] = x;
				arrToRender[1] = y;
				while(itr != waypoints.end())
				{
					arrToRender[2 * curr + 0] = itr->GetX();
					arrToRender[2 * curr + 1] = itr->GetY();
					curr++;
					itr++;
				}
				glColor4f(1.0, 1.0, 0.0, 0.0);
				glVertexPointer(2, GL_FLOAT, 0, arrToRender);
				glDrawArrays(GL_LINE_STRIP, 0, curr);
				delete[] arrToRender;
			}
			
			RenderEntity(friendlyFleet[i]);
					
		}
	}
}

static void ProcessRegionStatus(NPC *fleet)
{
	Point2D currShipPoint;	
	for(int i=0; i < MAX_CHARACTERS_PER_FLEET; i++)
	{
		fleet[i].GetPosition(currShipPoint);
		int regionIndex = GetRegionForPosition(currShipPoint);
		if(fleet[i].GetStatus() & NPC_BOMBER)
		{
			int lastIndex = fleet[i].GetCurrentRegionIndex();
			
			if(lastIndex > -1)
			{
				region[lastIndex].UnsetStatus(REGION_WARNING_INCOMING);
			}
			
			if(lastIndex != regionIndex)
			{
				fleet[i].SetCurrentRegionIndex(regionIndex);
			}
			
			region[regionIndex].CalculateStrike(fleet[i]);
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
	   
	ProcessRegionStatus(friendlyFleet);
	ProcessRegionStatus(enemyFleet);

	
	for(int i=0; i < MAX_CHARACTERS_PER_FLEET; i++)
	{
		friendlyFleet[i].Tick();
		enemyFleet[i].Tick();
	}
	
	RenderWorld();
	RenderRegions();

	RenderFriendlyCharacters();
	RenderEnemyCharacters();
}
