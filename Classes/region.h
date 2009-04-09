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
#define REGION_WARNING_INCOMING     4

class NPC;

class Region
{
public:
	Region();
	
	void Initialize();
	
	void UnsetStatus(uint32_t status);
	void SetStatus(uint32_t status);
	void ToggleStatus(uint32_t status);
	uint32_t GetStatus() { return m_status; }
	
	void SetExtents(Point2D upperRight, Point2D lowerLeft);
	void GetExtents(Point2D &upperRight, Point2D &lowerLeft);
	
	Point2D GetMainBaseLocation() { return m_mainBase; }
	
	void CalculateStrike(NPC &npc);
	
private:
	uint32_t m_status;
	Point2D  m_mainBase;
	Point2D  m_upperRight;
	Point2D  m_lowerLeft;
};

