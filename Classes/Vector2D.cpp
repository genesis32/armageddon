/*
 *  2DVector.cpp
 *  Armageddon
 *
 *  Created by ddm on 3/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "util.h"
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
	
	double distance = fast_sqrt(xd*xd + yd*yd);
	
	return distance;	
}

Vector2D Point2D::GetHeadingTo(const Point2D &offsetPoint)
{
	Vector2D out;
	
	out.SetXMagnitude(offsetPoint.GetX() - m_pos[0]);
	out.SetYMagnitude(offsetPoint.GetY() - m_pos[1]);

	out.Normalize();
	
	return out;
}


Vector2D::Vector2D(double x, double y)
{
	m_vec[0] = x;
	m_vec[1] = y;
}

Vector2D::Vector2D(const Vector2D &other)
{
	m_vec[0] = other.m_vec[0];
	m_vec[1] = other.m_vec[1];
}

void Vector2D::Normalize()
{
	float len = fast_sqrt(m_vec[0] * m_vec[0] + m_vec[1] * m_vec[1]);
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

float Vector2D::DotProduct(const Vector2D &other)
{
	return (m_vec[0] * other.m_vec[0] + m_vec[1] * other.m_vec[1]);
}


