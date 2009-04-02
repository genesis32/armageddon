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

#define FONT_TEXTURE_ID 0
#define MAP_TEXTURE_ID  1

#ifdef __cplusplus
extern "C" {
#endif 
	
void AddTexture(int textureId, GLuint glId);
void Init();
void GameTick();
void SetEntitySelection(float lat, float lon);
	
#ifdef __cplusplus
}
#endif

#endif

