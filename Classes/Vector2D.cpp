/*
 *  2DVector.cpp
 *  Armageddon
 *
 *  Created by ddm on 3/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <math.h>
#include "Vector2D.h"

Vector2D::Vector2D()
{
	
}

Point2D::Point2D(float x, float y)
{
	m_pos[0] = x;
	m_pos[1] = y;
}

void Point2D::Set(float x, float y)
{
	m_pos[0] = x;
	m_pos[1] = y;
}

void Point2D::SetX(float x)
{
	m_pos[0] = x;
}

void Point2D::SetY(float y)
{
	m_pos[1] = y;
}

float Point2D::Distance(const Point2D &otherPoint)
{
	float xd = otherPoint.m_pos[0] - m_pos[0];
	float yd = otherPoint.m_pos[1] - m_pos[1];
	
	double distance = sqrt(xd*xd + yd*yd);
	
	return distance;	
}


