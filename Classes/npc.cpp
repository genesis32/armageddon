/*
 *  npc.cpp
 *  Armageddon
 *
 *  Created by ddm on 3/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <math.h>
#include "npc.h"

NPC::NPC()
{
	m_lastWayPoint = -1;
	m_currWayPoint =  0;
}

void NPC::SetDirection(const Vector2D &direction)
{
	m_currentDirection.SetXMagnitude(direction.GetXMagnitude());
	m_currentDirection.SetYMagnitude(direction.GetYMagnitude());
}

void NPC::Tick()
{
	if(m_status & NPC_ALIVE)
	{
		float offset = 0.0;
		
		if(m_currWayPoint >= m_numWayPoints)
			m_numWayPoints = 0;
		else if(m_lastWayPoint != m_currWayPoint)
		{
			Vector2D newDirVector;
			newDirVector.SetXMagnitude(m_waypoint[m_currWayPoint].GetX() - m_pos.GetX());
			newDirVector.SetYMagnitude(m_waypoint[m_currWayPoint].GetY() - m_pos.GetY());
			newDirVector.Normalize();
			
			m_currentDirection.SetXMagnitude(newDirVector.GetXMagnitude());	
			m_currentDirection.SetYMagnitude(newDirVector.GetYMagnitude());
			m_lastWayPoint = m_currWayPoint;
		}
			
		offset = m_currentDirection.GetXMagnitude() * m_speed;
		m_pos.SetX(m_pos.GetX() + offset);
	
		offset = m_currentDirection.GetYMagnitude() * m_speed;
		m_pos.SetY(m_pos.GetY() + offset);
		
		if(m_waypoint[m_currWayPoint].GetX() <= m_pos.GetX()+1.0 &&
		   m_waypoint[m_currWayPoint].GetX() >= m_pos.GetX()-1.0 &&
		   m_waypoint[m_currWayPoint].GetY() <= m_pos.GetY()+1.0 &&
		   m_waypoint[m_currWayPoint].GetY() >= m_pos.GetY()-1.0)
		{
			m_currWayPoint++;
		}	
			
		
	}
}

void NPC::SetSpeed(float speed)
{
	m_speed = speed;
}

void NPC::SetStatus(uint32_t status)
{
	m_status |= status;
}

void NPC::SetPosition(const Point2D &pos)
{
	m_pos.SetX(pos.GetX());
	m_pos.SetY(pos.GetY());
}

void NPC::GetPosition(Point2D &pos)
{
	pos.SetX(m_pos.GetX());
	pos.SetY(m_pos.GetY());
}

void NPC::AddWayPoint(float lat, float lon)
{
	if(m_numWayPoints >= MAX_NUM_WAYPOINTS)
		return;
	
	m_waypoint[m_numWayPoints].SetX(lon);
	m_waypoint[m_numWayPoints].SetY(lat);
	m_numWayPoints++;
}


