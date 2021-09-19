#include <stdint.h>
#include <draw/screen.h>
#include <draw/color.h>
#include <draw/pixel.h>

void draw_pixel(int x, int y) {
	draw_framebuffer[y * DRAW_SCREEN_W + x] = draw_color;
}

void draw_pixel_color(int x, int y, DrawColor col) {
	draw_framebuffer[y * DRAW_SCREEN_W + x] = col;
}
