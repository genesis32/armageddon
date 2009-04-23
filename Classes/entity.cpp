/*
 *  npc.cpp
 *  Armageddon
 *
 *  Created by ddm on 3/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdlib.h>
#include <string.h>
#include "entity.h"
#include "util.h"

entity_t enemies[MAX_CHARACTERS_PER_FLEET];
int num_enemies = 0;

entity_t friendlies[MAX_CHARACTERS_PER_FLEET];
int num_friendlies;

entity_t *Foe_New()
{
	if(num_enemies >= MAX_CHARACTERS_PER_FLEET)
		return NULL;

	memset( &enemies[ num_enemies ], 0, sizeof( enemies[0] ) );
	return &enemies[num_enemies++];
}

void Foe_Remove(entity_t *npc)
{
	memmove( npc, npc+1, (int)(&enemies[num_enemies]) - (int)(npc+1) );
	num_enemies--;
}

void Foe_Reset()
{
	memset(enemies, 0, sizeof(MAX_CHARACTERS_PER_FLEET));
}

entity_t *Fri_New()
{
	if(num_friendlies >= MAX_CHARACTERS_PER_FLEET)
		return NULL;
	
	memset( &friendlies[ num_friendlies ], 0, sizeof( friendlies[0] ) );
	return &friendlies[num_friendlies++];
}

void Fri_Remove(entity_t *npc)
{
	memmove( npc, npc+1, (int)(&friendlies[num_friendlies]) - (int)(npc+1) );
	num_friendlies--;
}

void Fri_Reset()
{
	memset(friendlies, 0, sizeof(MAX_CHARACTERS_PER_FLEET));
}

void Ent_Tick(entity_t *npc)
{
	if(npc->flags & ENT_FLG_STARTROUTE)
	{
		Ent_MoveTowardsPoint(npc, npc->waypoints[0]);
		
		npc->flags &= ~ENT_FLG_STARTROUTE;
		npc->flags |= ENT_FLG_ENROUTE;
	} 
	// if we are enroute and we have reached a waypoint go to the next waypoint
	else if(npc->flags & ENT_FLG_ENROUTE &&
			npc->waypoints[npc->currwaypointidx][0] <= npc->pos[0]+1.0f &&
			npc->waypoints[npc->currwaypointidx][0] >= npc->pos[0]-1.0f &&
			npc->waypoints[npc->currwaypointidx][1] <= npc->pos[1]+1.0f &&
			npc->waypoints[npc->currwaypointidx][1] >= npc->pos[1]-1.0f)
	{
		npc->currwaypointidx++;
		if(npc->currwaypointidx == npc->numwaypoints)
		{
			Ent_ClearWayPoints(npc);
		}
		else
		{
			Ent_MoveTowardsPoint(npc, npc->waypoints[npc->currwaypointidx]);
		}
	}
			
	npc->pos[0] = npc->pos[0] > 180.0  ? -180.0 : npc->pos[0];
	npc->pos[0] = npc->pos[0] < -180.0 ?  180.0 : npc->pos[0];

	npc->pos[1] = npc->pos[1] > 90.0  ? -90.0 : npc->pos[1];
	npc->pos[1] = npc->pos[1] < -90.0 ?  90.0 : npc->pos[1];
	
	npc->pos[0] += (npc->direction[0] * npc->speed);
	npc->pos[1] += (npc->direction[1] * npc->speed);
}

void Ent_SetHeading(entity_t *ent, vec2d_t dir)
{
	ent->direction[0] = dir[0];
	ent->direction[1] = dir[1];
	ent->rot_angle = fast_atan2(ent->direction[1], ent->direction[0]) * (180.0f / PI_FLOAT);		
}

void Ent_MoveTowardsPoint(entity_t *npc, const pt2d_t pt)
{
	npc->direction[0] = pt[0] - npc->pos[0];
	npc->direction[1] = pt[1] - npc->pos[1];
	
	Vec2_Normalize(npc->direction);
	npc->rot_angle = fast_atan2(npc->direction[1], npc->direction[0]) * (180.0f / PI_FLOAT);		
}

void Ent_AddWayPoint(entity_t *npc, float lat, float lon)
{
	if(npc->numwaypoints >= MAX_NUM_WAYPOINTS)
		return;
	
	if(npc->numwaypoints == 0)
		npc->flags |= ENT_FLG_STARTROUTE;
	
	npc->waypoints[npc->numwaypoints][0] = lon;
	npc->waypoints[npc->numwaypoints][1] = lat;
	
	npc->numwaypoints++;
}

void Ent_ClearWayPoints(entity_t *npc)
{
	memset(npc->waypoints, 0, sizeof(npc->waypoints));
	npc->numwaypoints = npc->currwaypointidx = 0;
	npc->flags &= ~(ENT_FLG_ENROUTE | ENT_FLG_STARTROUTE);
}

bool Ent_Attack(entity_t *p1, entity_t *p2)
{
	float dp = Vec2_DotProduct(p1->direction, p2->direction);
	// if it's head on the player automatally dies..
	if(dp <= -0.90 && dp >= -1.0)
	{
		return true;
	}
	return false;
}

