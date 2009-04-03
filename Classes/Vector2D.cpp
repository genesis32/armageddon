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

Point2D::Point2D(const Point2D &other)
{
	m_pos[0] = other.m_pos[0];
	m_pos[1] = other.m_pos[1];
}

bool Point2D::Equals(const Point2D &other)
{
	return other.m_pos[0] == m_pos[0] && other.m_pos[1] == m_pos[1];
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

void Vector2D::Normalize()
{
	float len = sqrt(m_vec[0] * m_vec[0] + m_vec[1] * m_vec[1]);
	m_vec[0] /= len;
	m_vec[1] /= len;
}

void Vector2D::SetXMagnitude(float x)
{
	m_vec[0] = x;
}

void Vector2D::SetYMagnitude(float y)
{
	m_vec[1] = y;
}

