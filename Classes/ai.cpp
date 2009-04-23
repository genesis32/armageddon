#include <float.h>
#include <stdlib.h>
#include "Vector2D.h"
#include "ai.h"
#include "entity.h"


#define GROUP_PATROL   1
#define GROUP_ENGAGING 2
#define GROUP_CAP      4

// entity of interest
typedef struct eofi_s
{
	entity_t *ent;
	float    dist;
} eofi_t;

typedef struct aigroup_s
{
	uint8_t flags;
	eofi_t eofi;
} aigroup_t;

static aigroup_t aigroup[MAX_CHARACTERS_PER_FLEET] = 
{ 
	{ NULL, 0, FLT_MAX },
	{ NULL, 0, FLT_MAX },
	{ NULL, 0, FLT_MAX },
	{ NULL, 0, FLT_MAX }
};

static void ComputeClosestEntites() 
{
	for(int i=0; i < num_enemies; i++)
	{
		int cegrpidx = enemies[i].groupidx;
		bool found_ent = false;
		for(int j=0; j < num_friendlies; j++)
		{
			float dist = Pt2_Distance(enemies[i].pos, friendlies[j].pos);
			if(dist <= RDR_SCHB_DIST && dist < aigroup[cegrpidx].eofi.dist)
			{
				aigroup[cegrpidx].eofi.dist = dist;
				aigroup[cegrpidx].eofi.ent = &friendlies[j];
				found_ent = true;
			}
		}
		
		if(!found_ent)
		{
		   aigroup[cegrpidx].eofi.dist = FLT_MAX;
		   aigroup[cegrpidx].eofi.ent  = NULL;
		}
	}
}

void MCP_Think(int frame)
{
	ComputeClosestEntites();
	
	for(int i=0; i < num_enemies; i++)
	{
		aigroup_t *mygroup = &aigroup[enemies[i].groupidx];
		
		entity_t *ent = mygroup->eofi.ent;
		// we have an entity in radar range that we are interested in..
		if(ent)
		{
			mygroup->flags |= GROUP_ENGAGING;
			if(mygroup->eofi.dist <= RDR_TRKB_DIST && !(mygroup->eofi.ent->flags & ENT_FLG_DYING))
			{
				if(Ent_Attack(&enemies[i], ent))
				{
					ent->flags |= ENT_FLG_DYING;
				}
			}
			else
			{
				Ent_MoveTowardsPoint(&enemies[i], mygroup->eofi.ent->pos);	
			}
		}
	}
}






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
