#ifndef DRAW_SCREEN_H_
#define DRAW_SCREEN_H_

#include "color.h"

#define DRAW_SCREEN_W draw_screen_w
#define DRAW_SCREEN_H draw_screen_h

#define DRAW_SCREEN_HIGHRES_W 800
#define DRAW_SCREEN_HIGHRES_H 480
#define DRAW_SCREEN_LOWRES_W 400
#define DRAW_SCREEN_LOWRES_H 240

extern DrawColor *draw_framebuffer;
extern unsigned draw_screen_w;
extern unsigned draw_screen_h;

void draw_screen_clear();
void draw_screen_set_lowres();
void draw_screen_flip();

#endif
