/*
 *  2DVector.h
 *  Armageddon
 *
 *  Created by ddm on 3/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef VECTOR2D_H
#define VECTOR2D_H

class Point2D
	{
	public:
		Point2D() { m_pos[0] = m_pos[1] = 0.0f; }
		Point2D(float x, float y);
		
		void Set(float x, float y);
		
		void SetX(float x);
		void SetY(float y);
		
		float GetX() const { return m_pos[0]; }
		float GetY() const { return m_pos[1]; }
		
		float Distance(const Point2D &otherPoint);
		
	private:
		float m_pos[2];
	};

class Vector2D 
	{
	public:
		Vector2D() { m_vec[0] = m_vec[1] = 0.0f; }
		
		void SetXMagnitude(float x);
		void SetYMagnitude(float y); 
		
		float GetXMagnitude() const { return m_vec[0]; }
		float GetYMagnitude() const { return m_vec[1]; }
		
		void  Normalize();
		
	private:
		float m_vec[2];
	};

#endif