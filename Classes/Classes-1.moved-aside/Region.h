/*
 *  Region.h
 *  Armageddon
 *
 *  Created by ddm on 1/18/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef REGION_H
#define REGION_H

#include <vector>

class LatLon
{
public:
	float Latitude; 
	float Longitude;
	
	LatLon(float lat, float lon) : Latitude(lat), Longitude(lon)
	{
		
	}
	
};

typedef std::vector<LatLon> LatLonVector;

class Region
{
public:
	
	enum AFFILIATION { FRIENDLY, NEUTRAL, HOSTILE };
	Region(AFFILIATION startingAffiliation);
	
	void SetPoints(LatLonVector &points);
	void GetPoints(LatLonVector &outPts);
	
private:
	AFFILIATION _currAffiliation;
	uint8_t     _affiliationLevel;
	uint16_t    _numForces;
	
	LatLonVector _pts;
};

#endif

