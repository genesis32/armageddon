/*
 *  transforms.h
 *  Armageddon
 *
 *  Created by ddm on 4/1/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#ifdef __cplusplus
extern "C" {
#endif 

float ScreenToLon(float x);
float ScreenToLat(float y);

float LonToScreen(float x);
float LatToScreen(float y);	
	
#ifdef __cplusplus
}
#endif 
		

#endif