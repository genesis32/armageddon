#ifndef FONT_H
#define FONT_H

#include <OpenGLES/ES1/gl.h>

// Load the texture you want for the font. This font should be 8-bit
void LoadFontMap();

//   Set the current font size for the system
void SetFontSize(unsigned int);

// set the font color for the system
void SetFontColor(float, float, float);

// Draw a string (similiar to printf)
void DrawFontString(GLshort xpos, GLshort ypos, char *msg, ...);

#endif