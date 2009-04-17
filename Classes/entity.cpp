/*
 *  npc.cpp
 *  Armageddon
 *
 *  Created by ddm on 3/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "entity.h"
#include "util.h"

entity_t enemies[MAX_CHARACTERS_PER_FLEET];
int num_enemies = 0;

entity_t friendlies[MAX_CHARACTERS_PER_FLEET];
int num_friendlies;

entity_t *Foe_New()
{
	if(num_enemies >= MAX_CHARACTERS_PER_FLEET)
		return NULL;

	memset( &enemies[ num_enemies ], 0, sizeof( enemies[0] ) );
	return &enemies[num_enemies++];
}

void Foe_Remove(entity_t *npc)
{
	memmove( npc, npc+1, (int)(&enemies[num_enemies]) - (int)(npc+1) );
	num_enemies--;
}

void Foe_Reset()
{
	memset(enemies, 0, sizeof(MAX_CHARACTERS_PER_FLEET));
}

entity_t *Fri_New()
{
	if(num_friendlies >= MAX_CHARACTERS_PER_FLEET)
		return NULL;
	
	memset( &friendlies[ num_friendlies ], 0, sizeof( friendlies[0] ) );
	return &friendlies[num_friendlies++];
}

void Fri_Remove(entity_t *npc)
{
	memmove( npc, npc+1, (int)(&friendlies[num_friendlies]) - (int)(npc+1) );
	num_friendlies--;
}

void Fri_Reset()
{
	memset(friendlies, 0, sizeof(MAX_CHARACTERS_PER_FLEET));
}

void Ent_Tick(entity_t *npc)
{
	npc->pos[0] += npc->direction[0] * npc->speed;
	npc->pos[1] += npc->direction[1] * npc->speed;
	
	npc->pos[0] > 180.0  ? -180.0 : npc->pos[0];
	npc->pos[0] < -180.0 ?  180.0 : npc->pos[0];

	npc->pos[1] > 90.0  ? -90.0 : npc->pos[1];
	npc->pos[1] < -90.0 ?  90.0 : npc->pos[1];

	npc->rot_angle = fast_atan2(npc->direction[1], npc->direction[0]) * (180.0f / PI_FLOAT);	
}

void Ent_MoveTowardsPoint(entity_t *npc, const Point2D pt)
{
	
}

void Ent_AddWayPoint(entity_t *npc, float lat, float lon)
{
	if(npc->num_waypoints >= MAX_NUM_WAYPOINTS)
		return;
	
	int num_waypoints = npc->num_waypoints;
	
	npc->waypoints[num_waypoints][0] = lon;
	npc->waypoints[num_waypoints][1] = lat;
	
	num_waypoints++;
}

void Ent_ClearWayPoints(entity_t *npc)
{
	memset(npc->waypoints, 0, sizeof(npc->waypoints));
	npc->num_waypoints = 0;
}



/* NPC::NPC()
{
	m_currWaypoint = NULL;
	m_currRegionIndex = -1;
	
	SetSpeed(0.45); // default to fighter speed..
}

void NPC::ClearWaypoints()
{
	m_waypoints.clear();
	m_currWaypoint = NULL;
	m_currRotationAngle = 0.0f;
}

void NPC::SetDirection(const Vector2D &direction)
{
	m_currentDirection.SetXMagnitude(direction.GetXMagnitude());
	m_currentDirection.SetYMagnitude(direction.GetYMagnitude());
	
	m_currRotationAngle = fast_atan2(direction.GetYMagnitude(), direction.GetXMagnitude()) * (180.0 / PI_FLOAT);
}

float NPC::GetCurrRotationAngle()
{
	return m_currRotationAngle;
}

void NPC::MoveTowardsPoint(const Point2D &dstPoint)
{
	Vector2D newDirVector;
	newDirVector.SetXMagnitude(dstPoint.GetX() - m_pos.GetX());
	newDirVector.SetYMagnitude(dstPoint.GetY() - m_pos.GetY());
	newDirVector.Normalize();
	
	SetDirection(newDirVector);
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
		   m_currWaypoint->GetY() >= m_pos.GetY()-1.0)
		{
			m_currWaypoint = NULL;
			if(!m_waypoints.empty())
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
 */

