/*
 *  region.cpp
 *  Armageddon
 *
 *  Created by ddm on 4/3/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#include <stdlib.h>
#include "region.h"

region_t regions[NUM_REGION_CELLS];

void InitRegions()
{
	for(int i=0; i < NUM_REGION_ROWS; i++)
	{
		for(int j=0; j < NUM_REGION_COLS; j++)
		{
			int idx = (NUM_REGION_COLS * i) + j;
			int tmp = rand() % 256;
			if(tmp < 128)
				regions[idx].flags |= REGION_AFFILIATION_FRIENDLY;
			else
				regions[idx].flags |= REGION_AFFILIATION_FOE;
			
			const float width  = 360.0 / (float)NUM_REGION_COLS;
			const float height = 180.0 / (float)NUM_REGION_ROWS;
			
			regions[idx].ll[0] = (j * width) - 180.0;
			regions[idx].ll[1] = (i * height) - 90.0;
			regions[idx].ur[0] = (j * width - 180.0) + width;
			regions[idx].ur[1] = (i * height - 90.0) + height;
		}
	}
}