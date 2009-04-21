/*
 *  region.h
 *  Armageddon
 *
 *  Created by ddm on 4/3/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Vector2D.h"

#define NUM_REGION_ROWS 4
#define NUM_REGION_COLS 4
#define NUM_REGION_CELLS (NUM_REGION_ROWS * NUM_REGION_COLS)

#define REGION_AFFILIATION_FRIENDLY 1
#define REGION_AFFILIATION_FOE      2
#define REGION_WARNING_INCOMING     4
#define REGION_BASE_DESTROYED       8

class NPC;

typedef struct region_s
{
	uint32_t flags;
	pt2d_t   baseloc;
	pt2d_t   ll;
	pt2d_t   ur;
} region_t;


extern region_t regions[NUM_REGION_CELLS];
void   InitRegions();
