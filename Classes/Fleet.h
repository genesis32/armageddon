/*
 *  Fleet.h
 *  Armageddon
 *
 *  Created by ddm on 3/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef FLEET_H
#define FLEET_H

class NPC;

class Fleet	
	{
	public:
		Fleet(int numEntities);
		
	private:
		NPC *m_npcs;
		
	};

#endif