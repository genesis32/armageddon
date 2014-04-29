/*
 *  sprite.h
 *  Armageddon
 *
 *  Created by ddm on 4/26/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SPRITE_H
#define SPRITE_H

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#define FRI_FIGHTER_NODMG 0
#define FRI_BOMBER_NODMG  1
#define FOE_FIGHTER_NODMG 2
#define FOE_BOMBER_NODMG  3

typedef struct sprite_s
{
	GLuint id;
	float llx, lly;
	float urx, ury;
	
} sprite_t;

//void GetSprite(sprite_t *, uint16_t spritenum);

#endif