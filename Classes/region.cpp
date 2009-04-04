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

Region::Region()
{
	
}

void Region::Initialize()
{
	int tmpVal = rand() % 256;
	if(tmpVal < 128)
		SetStatus(REGION_AFFILIATION_FRIENDLY);
	else
		SetStatus(REGION_AFFILIATION_FOE);	
}

void Region::SetStatus(uint32_t status)
{
	m_status |= status;
}

void Region::SetExtents(Point2D ll, Point2D ur)
{
	m_lowerLeft = ll;
	m_upperRight = ur;
}

void Region::GetExtents(Point2D &ll, Point2D &ur)
{
	ll = m_lowerLeft;
	ur = m_upperRight;
}


