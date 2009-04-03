/*
 *  npc.h
 *  Armageddon
 *
 *  Created by ddm on 3/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NPC_H
#define NPC_H

#include "Vector2D.h"

#define NPC_ALIVE    1
#define NPC_DAMAGED  2
#define NPC_SELECTED 4

#define MAX_NUM_WAYPOINTS 128

class NPC
{
public:
	NPC();
	
	void     Tick();

	void     SetDirection(const Vector2D &direction);
	
	void     SetSpeed(float speed);
	float    GetSpeed() const { return m_speed; }
	
	void     SetStatus(uint32_t status);
	uint32_t GetStatus() const { return m_status; }

	void SetPosition(const Point2D &pos);
	void GetPosition(Point2D &pos);
	
	void AddWayPoint(float lat, float lon);
	
private:
	uint32_t  m_status;
	Point2D   m_pos;
	Vector2D  m_currentDirection;
	float     m_speed;
	Point2D   m_waypoint[MAX_NUM_WAYPOINTS];
	int       m_lastWayPoint;
	int       m_currWayPoint;
    int       m_numWayPoints;
};

#endif

