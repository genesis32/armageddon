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
		Point2D() { }
		Point2D(float x, float y);
		
		void Set(float x, float y);
		
		void SetX(float x);
		void SetY(float y);
		
		float GetX() { return m_pos[0]; }
		float GetY() { return m_pos[1]; }
		
		float Distance(const Point2D &otherPoint);
		
	private:
		float m_pos[2];
	};

class Vector2D 
	{
	public:
		Vector2D();
		
	private:
		Point2D pt1;
		Point2D pt2;
	};

#endif