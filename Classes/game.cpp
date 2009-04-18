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
#include "entity.h"
#include "game.h"
#include "transforms.h"
#include "ai.h"

#define MAX_TEXTURES 16
#define NUM_INPUT_WAYPOINTS 128

static int frameCount = 0;
float   selectedLat, selectedLon;

// regions will be ROW major...
Region    region[NUM_REGION_ROWS * NUM_REGION_COLS];
pt2d_t    inputWaypoints[NUM_INPUT_WAYPOINTS];
int       numInputWaypoints = 0;
entity_t *selectedEntity = NULL;
bool      clearSelectedEntity = false;
GLuint  textures[MAX_TEXTURES];

static void RenderEntity(entity_t *entity);

int GetRegionForPosition(const Point2D &pos)
{
	int row = (int)floor(((pos.GetY() + 90.0) / 180.0) * (float)NUM_REGION_ROWS);
	int col = (int)floor(((pos.GetX() + 180.0) / 360.0) * (float)NUM_REGION_COLS);
	
	int regionIdx = row * NUM_REGION_COLS + col;
	
	return regionIdx;
}

void InitFriendlies()
{
	Fri_Reset();
	
	float initial_lon = -155.0;
	float start_lat   = -80.0;
	for(int i=0; i < MAX_CHARACTERS_PER_FLEET; i++)
	{
		entity_t *foe = Fri_New();
		assert(foe != NULL);
		
		foe->affiliation = ENT_AFFILIATION_FRIENDLY;
		if(i == 0)
		{
			foe->type = ENT_TYPE_BOMBER;
			foe->speed = 0.40; 
		}
		else
		{
			foe->type  = ENT_TYPE_FIGHTER;
			foe->speed = 0.50; 
		}
		
		foe->direction[0] = 1.0;
		
		foe->pos[0] = initial_lon;
		foe->pos[1] = start_lat;
		start_lat += (180.0 / 4.0);
	}
	
}

void InitFoes()
{
	Foe_Reset();

	float initial_lon = 155.0;
	float start_lat   = -80.0;
	for(int i=0; i < MAX_CHARACTERS_PER_FLEET; i++)
	{
		entity_t *foe = Foe_New();
		assert(foe != NULL);
		
		foe->affiliation = ENT_AFFILIATION_FOE;
		if(i == 0)
		{
			foe->type = ENT_TYPE_BOMBER;
			foe->speed = 0.40; 
		}
		else
		{
			foe->type  = ENT_TYPE_FIGHTER;
			foe->speed = 0.50; 
		}
		
		foe->direction[0] = -1.0;
		
		foe->pos[0] = initial_lon;
		foe->pos[1] = start_lat;
		start_lat += (180.0 / 4.0);
	}
}

void Init()
{
	srand(time(NULL));
	for(int i=0; i < MAX_TEXTURES; i++)
	{
		textures[i] = -1;
	}
	
	InitFriendlies();
	InitFoes();

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
		selectedEntity = NULL;
		clearSelectedEntity = false;
		numInputWaypoints = 0; // reset the number of pending waypoints
	}
}

void SetInputWayPoint(float lat, float lon)
{
	if(numInputWaypoints >= NUM_INPUT_WAYPOINTS)
		return;

	inputWaypoints[numInputWaypoints][0] = lon;
	inputWaypoints[numInputWaypoints][1] = lat;
	
	numInputWaypoints++;
}

static void ProcessInputWaypoints()
{
	if(selectedEntity != NULL)
	{
		for(int i=0; i < numInputWaypoints; i++)
		{
			Ent_AddWayPoint(selectedEntity, inputWaypoints[i][1], inputWaypoints[i][0]);
		}
		numInputWaypoints = 0;
	}
}

static void ProcessEntitySelection()
{
	if(selectedLat != FLT_MIN && selectedLon != FLT_MIN)
	{
		for(int i=0; i < num_friendlies; i++)
		{
			if(selectedLon <= friendlies[i].pos[0]+ENTITY_WIDTH_RADIUS &&
			   selectedLon >= friendlies[i].pos[0]-ENTITY_WIDTH_RADIUS &&
			   selectedLat <= friendlies[i].pos[1]+ENTITY_HEIGHT_RADIUS &&
			   selectedLat >= friendlies[i].pos[1]-ENTITY_HEIGHT_RADIUS) 
			{
				selectedEntity = &friendlies[i];
		 		Ent_ClearWayPoints(selectedEntity);
				break;
			}
		}
		selectedLat = selectedLon = FLT_MIN;

		/* for(int i=0; i < MAX_CHARACTERS_PER_FLEET; i++)
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
		 */		// we have a valid lat lon from a selection, see if we're over an entity and tag him as selected
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
	for(int i=0; i < num_enemies; i++)
	{
		RenderEntity(&enemies[i]);
	}
}

static void RenderEntity(entity_t *entity)
{
	float x = entity->pos[0];
	float y = entity->pos[1];
	
	GLfloat verts[] = {
		-ENTITY_WIDTH_RADIUS, -ENTITY_HEIGHT_RADIUS,   -0.5,
		 ENTITY_WIDTH_RADIUS,  -ENTITY_HEIGHT_RADIUS,  -0.5,
		-ENTITY_WIDTH_RADIUS, ENTITY_HEIGHT_RADIUS,  -0.5,
		 ENTITY_WIDTH_RADIUS,  ENTITY_HEIGHT_RADIUS, -0.5
	};
	
	
	GLfloat min, max;
	if(entity->type & ENT_TYPE_BOMBER)
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
	glRotatef(entity->rot_angle, 0.0, 0.0, 1.0);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);	

	glPopMatrix();
	
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
}

static void RenderFriendlyCharacters()
{
	for(int i=0; i < num_friendlies; i++)
	{
		
		GLfloat *arr = new GLfloat[(friendlies[i].numwaypoints + 1) * 2];
		arr[0] = friendlies[i].pos[0];
		arr[1] = friendlies[i].pos[1];		
		int curr = 1;
		for(int j=0; j < friendlies[i].numwaypoints; j++)
		{
			arr[2 * curr + 0] = friendlies[i].waypoints[j][0];
			arr[2 * curr + 1] = friendlies[i].waypoints[j][1];
			curr++;
		}
		
		glColor4f(1.0, 1.0, 0.0, 0.0);
		glVertexPointer(2, GL_FLOAT, 0, arr);
		glDrawArrays(GL_LINE_STRIP, 0, curr);
		
		delete[] arr;
		
		RenderEntity(&friendlies[i]);
	}
	
	/* Point2D ptToRender;
	for(int i=0; i < MAX_CHARACTERS_PER_FLEET; i++)
	{
		if(friendlyFleet[i].GetStatus() & NPC_ALIVE)
		{
			friendlyFleet[i].GetPosition(ptToRender);
			
			float x = ptToRender.GetX();
			float y = ptToRender.GetY();
			
			// render his waypoints...
			std::deque<Point2D> waypoints;
			friendlyFleet[i].GetWaypoints(waypoints);
			if(!waypoints.empty())
			{
				GLfloat *arrToRender = new GLfloat[(waypoints.size() + 1) * 2];
				int curr = 1;
				std::deque<Point2D>::iterator itr = waypoints.begin();
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
	 */}

static void ProcessRegionStatus(entity_t *enemy_fleet)
{
	Point2D currShipPoint;	
	for(int i=0; i < num_enemies; i++)
	{
			/* int regionIndex = GetRegionForPosition(currShipPoint);
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
	} */
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
	   
	ProcessRegionStatus(enemies);
	
	for(int i=0; i < num_enemies; i++)
	{
		Ent_Tick(&enemies[i]);
	}
	
	for(int i=0; i < num_friendlies; i++)
	{
		Ent_Tick(&friendlies[i]);
	}
	
	RenderWorld();
	RenderRegions();

	RenderFriendlyCharacters();
	RenderEnemyCharacters();
}
