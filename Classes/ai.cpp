/* 
		 *  ai.cpp
		 *  Armageddon
		 *
		 *  Created by ddm on 4/13/09.
		 *  Copyright 2009 __MyCompanyName__. All rights reserved.
		 *

		#include <deque>
		#include <float.h>
		#include "ai.h"
		#include "npc.h"

		#define DETECTOR_HALF_ANGLE (1.0 - (20.0/90.0))

		enum DecisionType 
		{
			HelpWingMan,
			AttackOpponent,
			CapBase,
			DestroyRegionBase
		};

		typedef struct group_s {
			DecisionType      currDecision;
			Point2D           dstLocation;
			std::deque<NPC *> entitiesInGroup;
		} group_t;

		extern NPC enemyFleet[MAX_CHARACTERS_PER_FLEET];
		extern NPC friendlyFleet[MAX_CHARACTERS_PER_FLEET];

		static group_t aiGroups[MAX_CHARACTERS_PER_FLEET];
		static float p2pDistance[MAX_CHARACTERS_PER_FLEET][MAX_CHARACTERS_PER_FLEET];

		// if the dot product is between 0 and 1 we know that the heading of the enemy ship
		// is facing the other ship.  Otherwise it is parallel or behind it.
		static float p2pDotProducts[MAX_CHARACTERS_PER_FLEET][MAX_CHARACTERS_PER_FLEET];

		static void ComputeParameters()
		{
			// calculate distances to each other player.
			for(int enemyIndex = 0; enemyIndex < MAX_CHARACTERS_PER_FLEET; enemyIndex++)
			{
				if(enemyFleet[enemyIndex].GetStatus() & NPC_ALIVE)
				{
					Point2D enemyPos;
					enemyFleet[enemyIndex].GetPosition(enemyPos);
					for(int friendlyIndex = 0; friendlyIndex < MAX_CHARACTERS_PER_FLEET; friendlyIndex++)
					{
						if(friendlyFleet[friendlyIndex].GetStatus() & NPC_ALIVE)
						{
							Point2D friendPos;
							friendlyFleet[friendlyIndex].GetPosition(friendPos);
							p2pDistance[enemyIndex][friendlyIndex] = enemyPos.Distance(friendPos);
							
							Vector2D offset = enemyPos.GetHeadingTo(friendPos);
							float dp = enemyFleet[enemyIndex].GetDirection().DotProduct(offset);
							p2pDotProducts[enemyIndex][friendlyIndex] = dp;
						}
					}
				}
			}	
		}

		void Think()
		{
			ComputeParameters();
			
			for(int enemyIndex = 0; enemyIndex < MAX_CHARACTERS_PER_FLEET; enemyIndex++)
			{
				if(enemyFleet[enemyIndex].GetStatus() & NPC_BOMBER)
					continue;
				
				if(enemyFleet[enemyIndex].GetStatus() & NPC_ALIVE)
				{
					int closestFriendIdx = -1;
					float closestVal = FLT_MAX;
					for(int friendlyIndex = 0; friendlyIndex < MAX_CHARACTERS_PER_FLEET; friendlyIndex++)
					{
						if(friendlyFleet[friendlyIndex].GetStatus() & NPC_ALIVE)
						{
							float dp = p2pDotProducts[enemyIndex][friendlyIndex]; 
							if(dp >= DETECTOR_HALF_ANGLE && dp <= 1.0)
							{
								float distance = p2pDistance[enemyIndex][friendlyIndex];
								if(distance < closestVal)
								{
									closestFriendIdx = friendlyIndex;
									closestVal = distance;
								}
							}
						}
					}
					if(closestFriendIdx > -1)
					{
						Point2D friendPos;
						friendlyFleet[closestFriendIdx].GetPosition(friendPos);
						enemyFleet[enemyIndex].MoveTowardsPoint(friendPos);	
					}
				}
			}
		}
		 */
