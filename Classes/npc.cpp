/*
 *  npc.cpp
 *  Armageddon
 *
 *  Created by ddm on 3/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <math.h>
#include <float.h>
#include "npc.h"

NPC::NPC()
{
	m_currWaypoint = NULL;
	m_currRegionIndex = -1;
	
	SetSpeed(0.45); // default to fighter speed..
}

void NPC::ClearWaypoints()
{
	m_waypoints.clear();
	m_currWaypoint = NULL;
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
		
		if(!m_waypoints.empty() && m_currWaypoint == NULL)
		{
			m_currWaypoint = &m_waypoints.front();
			Vector2D newDirVector;
			newDirVector.SetXMagnitude(m_currWaypoint->GetX() - m_pos.GetX());
			newDirVector.SetYMagnitude(m_currWaypoint->GetY() - m_pos.GetY());
			newDirVector.Normalize();
			
			SetDirection(newDirVector);			
		}
			
		offset = m_currentDirection.GetXMagnitude() * m_speed;
		m_pos.SetX(m_pos.GetX() + offset);
	
		offset = m_currentDirection.GetYMagnitude() * m_speed;
		m_pos.SetY(m_pos.GetY() + offset);
		
		if(m_pos.GetX() > 180.0)
			m_pos.SetX(-180.0);
		else if(m_pos.GetX() < -180.0)
			m_pos.SetX(180.0);
		
		if(m_pos.GetY() > 90.0)
			m_pos.SetY(-90.0);
		else if(m_pos.GetY() < -90.0)
			m_pos.SetY(90.0);
		
		if(m_currWaypoint && 
		   m_currWaypoint->GetX() <= m_pos.GetX()+1.0 &&
		   m_currWaypoint->GetX() >= m_pos.GetX()-1.0 &&
		   m_currWaypoint->GetY() <= m_pos.GetY()+1.0 &&
		   m_currWaypoint->GetY() >= m_pos.GetY()-1.0 &&
		   !m_waypoints.empty())
		{
			m_currWaypoint = NULL;
			m_waypoints.pop_front();
		}	
	}
}

void NPC::SetSpeed(float speed)
{
	m_speed = speed;
}

void NPC::UnsetStatus(uint32_t status)
{
	m_status &= ~status;
}

void NPC::ToggleStatus(uint32_t status)
{
	m_status ^= status;
}

void NPC::SetStatus(uint32_t status)
{
	m_status |= status;
	if(m_status & NPC_BOMBER)
	{
		SetSpeed(0.50);
	}
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
	Point2D newWaypoint;
	
	newWaypoint.SetX(lon);
	newWaypoint.SetY(lat);

	m_waypoints.push_back(newWaypoint);
}


