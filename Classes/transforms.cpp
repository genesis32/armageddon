/*
 *  transforms.cpp
 *  Armageddon
 *
 *  Created by ddm on 4/1/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "transforms.h"

// lon goes 0 - 320
// lon goes from -180 to 180
float ScreenToLon(float x)
{
	return ((x / 480.0f) * 360.0f) - 180.0f;
}

// lat goes 0 - 480.0
float ScreenToLat(float y)
{
	return ((y / 320.0f) * 180.0f) - 90.0f;
}

float LonToScreen(float x)
{
	
}

float LatToScreen(float y)
{
	
}
