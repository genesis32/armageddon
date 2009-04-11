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
#include <deque>

#define ENTITY_WIDTH  24.0
#define ENTITY_HEIGHT 20.0
#define ENTITY_WIDTH_RADIUS  (ENTITY_WIDTH/2.0)
#define ENTITY_HEIGHT_RADIUS (ENTITY_HEIGHT/2.0)

#define NPC_AFFILIATION_FRIENDLY 1
#define NPC_AFFILIATION_FOE      2
#define NPC_ALIVE    4
#define NPC_DAMAGED  8
#define NPC_SELECTED 16
#define NPC_BOMBER   32 // on for bomber, off for fighter

#define MAX_NUM_WAYPOINTS 256

class NPC
{
public:
	NPC();
	
	void     Tick();

	void     SetDirection(const Vector2D &direction);
	
	void     SetSpeed(float speed);
	float    GetSpeed() const { return m_speed; }
	
	void     UnsetStatus(uint32_t status);
	void     SetStatus(uint32_t status);
	uint32_t GetStatus() const { return m_status; }
	void     ToggleStatus(uint32_t status);

	void SetPosition(const Point2D &pos);
	void GetPosition(Point2D &pos);
	
	void AddWayPoint(float lat, float lon);
	
	void GetWaypoints(std::deque<Point2D> &outPts) {  outPts = m_waypoints; }
	void ClearWaypoints();
	
	int  GetCurrentRegionIndex() { return m_currRegionIndex; }
	void SetCurrentRegionIndex(int regionIndex) { m_currRegionIndex = regionIndex; }
	
	float GetCurrRotationAngle();
	
	void MoveTowardsPoint(const Point2D &pt);
	
private:
	uint32_t  m_status;
	Point2D   m_pos;
	Vector2D  m_currentDirection;
	float     m_speed;
	std::deque<Point2D> m_waypoints;
	Point2D   *m_currWaypoint;
	int       m_currRegionIndex;
	float     m_currRotationAngle;
};

#endif

