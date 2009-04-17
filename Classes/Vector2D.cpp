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

void Vec2_Normalize(vec2d_t vec)
{
	float len = fast_sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
	vec[0] /= len;
	vec[1] /= len;
}

void Pt2_GetHeadingTo(vec2d_t outv, const pt2d_t frm, const pt2d_t to)
{
	outv[0] = to[0] - frm[0];
	outv[0] = to[1] - frm[1];
	
	Vec2_Normalize(outv);
}

bool Pt2_Equals(const pt2d_t pt1, const pt2d_t pt2)
{
	return pt2[0] == pt1[0] && pt2[1] == pt1[0]; 
}

float Vec2_DotProduct(const vec2d_t vec1, const vec2d_t vec2)
{
	return (vec1[0] * vec2[0] + vec1[1] * vec2[1]);
}

float Pt2_Distance(const pt2d_t pt1, const pt2d_t pt2)
{
	float xd = pt2[0] - pt1[0]; 
	float yd = pt2[1] - pt1[1];
	
	float distance = fast_sqrt(xd*xd + yd*yd);
	
	return distance;	
}



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


