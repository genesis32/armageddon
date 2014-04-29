/*
 *  game.h
 *  Armageddon
 *
 *  Created by ddm on 3/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GAME_H
#define GAME_H

#ifdef __cplusplus
extern "C" {
#endif 
	
void AddTexture(int textureId, GLuint glId);
void Init();
void GameTick();
void SetEntitySelection(float lat, float lon);
void SetInputWayPoint(float lat, float lon);
void SetClearSelectedEntity();

	
#ifdef __cplusplus
}
#endif

#endif

