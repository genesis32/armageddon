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

#define REGION_AFFILIATION_FRIENDLY 1
#define REGION_AFFILIATION_FOE      2

class Region
{
public:
	Region();
	
	void Initialize();
	
	void SetStatus(uint32_t status);
	uint32_t GetStatus() { return m_status; }
	
	void SetExtents(Point2D upperRight, Point2D lowerLeft);
	void GetExtents(Point2D &upperRight, Point2D &lowerLeft);
private:
	uint32_t m_status;
	Point2D  m_upperRight;
	Point2D  m_lowerLeft;
};

