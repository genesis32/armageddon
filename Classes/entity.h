/*
 *  npc.h
 *  Armageddon
 *
 *  Created by ddm on 3/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NPC_H
#define NPC_H


#include "Vector2D.h"
#include <deque>

#define MAX_CHARACTERS_PER_FLEET 4
#define ENTITY_WIDTH  24.0
#define ENTITY_HEIGHT 20.0
#define ENTITY_WIDTH_RADIUS  (ENTITY_WIDTH/2.0)
#define ENTITY_HEIGHT_RADIUS (ENTITY_HEIGHT/2.0)

#define NPC_AFFILIATION_FRIENDLY 1
#define NPC_AFFILIATION_FOE      2

#define NPC_FLG_DAMAGED  1
#define NPC_FLG_DYING    2
#define ENT_FLG_SELECTED 4

#define NPC_TYPE_FIGHTER  1 
#define NPC_TYPE_BOMBER   2 

#define MAX_NUM_WAYPOINTS 256

typedef struct entity_s
{
	uint8_t   type;
	uint8_t   affiliation;
	uint32_t  flags;
	float     speed;
	int       curregionidx;
	float     rot_angle;
	int       num_waypoints;
	pt2d_t   *curr_waypoint;
	pt2d_t    pos;
	vec2d_t   direction;
	pt2d_t    waypoints[MAX_NUM_WAYPOINTS];
} entity_t;

extern entity_t enemies[MAX_CHARACTERS_PER_FLEET];
extern int num_enemies;

extern entity_t friendlies[MAX_CHARACTERS_PER_FLEET];
extern int num_friendlies;

entity_t *Foe_New();
void   Foe_Remove(entity_t *);
void   Foe_Reset();

entity_t *Fri_New();
void   Fri_Remove(entity_t *);
void   Fri_Reset();

void   Ent_Tick(entity_t *);
void   Ent_MoveTowardsPoint(entity_t *, const pt2d_t pt);
void   Ent_AddWayPoint(entity_t *, float lat, float lon);
void   Ent_ClearWayPoints(entity_t *);

#endif

