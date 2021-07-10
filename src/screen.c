#include <peripheral.h>
#include <draw/color.h>
#include "main.h"

#define MIN(a, b) (((a) < (b) ? (a) : (b)))
#define ABS(a) ((a) < 0 ? -(a) : (a))

RTFColor *draw_framebuffer = (void *) 0x00080000UL;
unsigned draw_screen_w = DRAW_SCREEN_HIGHRES_W;
unsigned draw_screen_h = DRAW_SCREEN_HIGHRES_H;

static uint32_t vgactrlreg;


DrawColor draw_backbuffer[DRAW_SCREEN_LOWRES_W*DRAW_SCREEN_LOWRES_H];


void draw_screen_clear() {
	int i;
	uint32_t *to = (uint32_t *) pixbuf;
	for(i = 0; i < draw_screen_w*draw_screen_h/4; i++)
		*(to++) = 0x0;

void draw_screen_set_lowres() {
	volatile uint32_t *vga_hw = (volatile uint32_t *) PERIPHERAL_VGA_BASE;
	vga_hw[0] = 0x3;
	vgactrlreg = 0x3;
	draw_screen_w = DRAW_SCREEN_LOWRES_W;
	draw_screen_h = DRAW_SCREEN_LOWES_H;
	
	//Move mouse out of the way
	vga_hw[10] = 800;
	vga_hw[11] = 480;

	//Lowres uses back buffer and must be flipped
	draw_framebuffer = draw_backbuffer;
}

void draw_screen_flip() {
	volatile uint32_t *vga_hw = (volatile uint32_t *) PERIPHERAL_VGA_BASE;
	DrawColor *real_framebuffer = (void *) 0x00080000UL;
	
	if(vgactrlreg == 0x3) {
		vgactrlreg = 0x7;
		real_framebuffer = (void *) 0x00080000UL;
	} else {
		vgactrlreg = 0x3;
		real_framebuffer = (void *) (0x00080000UL + 0x20000U);
	}
	vga_hw[0] = vgactrlreg;
	
	int i;
	uint32_t *from = (uint32_t *) draw_framebuffer;
	uint32_t *to = (uint32_t *) real_framebuffer;
	for(i = 0; i < DRAW_SCREEN_LOWRES_W*DRAW_SCREEN_LOWRES_H/4; i++)
		*(to++) = *(from++);
}
