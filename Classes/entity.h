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

#define MAX_CHARACTERS_PER_FLEET 4
#define ENTITY_WIDTH  24.0
#define ENTITY_HEIGHT 20.0
#define ENTITY_WIDTH_RADIUS  (ENTITY_WIDTH/2.0)
#define ENTITY_HEIGHT_RADIUS (ENTITY_HEIGHT/2.0)

#define ENT_AFFILIATION_FRIENDLY 1
#define ENT_AFFILIATION_FOE      2

#define ENT_FLG_DAMAGED    1
#define ENT_FLG_DYING      2
#define ENT_FLG_SELECTED   4
#define ENT_FLG_ENROUTE    8
#define ENT_FLG_STARTROUTE 16

#define ENT_TYPE_FIGHTER  1 
#define ENT_TYPE_BOMBER   2 

#define ENT_RDR_ON     1
#define ENT_RDR_SEARCH 2
#define ENT_RDR_TRACK  4

#define RDR_SCHB_DIST  60.0
#define RDR_TRKB_DIST  30.0

#define MAX_NUM_WAYPOINTS 256

typedef struct entity_s
{
	uint8_t   type;
	uint8_t   affiliation;
	uint8_t   rdrstatus;
	uint8_t   flags;
	short     currwaypointidx;
	short     numwaypoints;
	float     speed;
	float     rot_angle;
	int       curregionidx;
	int       groupidx;
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
void   Ent_SetHeading(entity_t *, vec2d_t dir);
void   Ent_AddWayPoint(entity_t *, float lat, float lon);
void   Ent_ClearWayPoints(entity_t *);

bool   Ent_Attack(entity_t *, entity_t *);

#endif

