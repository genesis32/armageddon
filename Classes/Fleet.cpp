/*
 *  Fleet.cpp
 *  Armageddon
 *
 *  Created by ddm on 3/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "npc.h"
#include "Fleet.h"

Fleet::Fleet(int numEntities)
{
	m_npcs = new NPC[numEntities];
}

