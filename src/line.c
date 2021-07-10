#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <draw/line.h>
#include <draw/pixel.h>
#include <draw/color.h>
#include <draw/screen.h>

#define MIN(a, b) (((a) < (b) ? (a) : (b)))
#define ABS(a) ((a) < 0 ? -(a) : (a))

void draw_line_quick(int x1, int y1, int x2, int y2, DrawColor col) {
	int dx = ABS(x2 - x1);
	int sx = (x1 < x2) ? 1 : -1;
	int dy = ABS(y2 - y1);
	int sy = (y1 < y2) ? 1 : -1; 
	int err = ((dx > dy) ? dx : -dy)/2, e2;
	
	int bufindex = y1 * DRAW_SCREEN_W + x1;
	int lineadder = DRAW_SCREEN_W*sy;
	
	for(;;){
		draw_framebuffer[bufindex] = col;
		
		if(x1 == x2 && y1 == y2)
			break;
		
		e2 = err;
		
		if(e2 > -dx) {
			err -= dy;
			x1 += sx;
			bufindex += sx;
		}
		if(e2 < dy) {
			err += dx;
			y1 += sy;
			bufindex += lineadder;
		}
	}
}

DrawLineSet *draw_line_set_new(size_t lines, unsigned int thickness) {
	DrawLineSet *set = NULL;
	DrawLine *line = NULL;
	
	if(!(set = malloc(sizeof(DrawLineSet))))
		goto fail;
	
	if(!(line = malloc(sizeof(DrawLine)*lines)))
		goto fail;
	
	memset(line, 0, sizeof(DrawLine)*lines);
	set->line = line;
	set->lines = lines;
	set->thickness = thickness;
	
	return set;
	
	fail:
	free(set);
	free(line);
	
	return NULL;
}

void draw_line_set_move(DrawLineSet *set, size_t line, int x1, int y1, int x2, int y2) {
	if(!set)
		return;
	
	if(line >= set->lines)
		return;
	
	set->line[line].x1 = x1;
	set->line[line].y1 = y1;
	set->line[line].x2 = x2;
	set->line[line].y2 = y2;
	set->line[line].thickness = set->thickness;
}

void draw_line_set_get(DrawLineSet *set, size_t line, int *x1, int *y1, int *x2, int *y2) {
	if(!set)
		return;
	
	if(line >= set->lines)
		return;
	
	*x1 = set->line[line].x1;
	*y1 = set->line[line].y1;
	*x2 = set->line[line].x2;
	*y2 = set->line[line].y2;
}

void draw_line_set_draw(DrawLineSet *set, size_t lines) {
	size_t i;
	
	if(!set)
		return;
	
	lines = MIN(lines, set->lines);
	
	for(i = 0; i < lines; i++) {
		int x1, x2, y1, y2;
		
		x1 = set->line[i].x1;
		x2 = set->line[i].x2;
		y1 = set->line[i].y1;
		y2 = set->line[i].y2;
		
		int dx = ABS(x2 - x1);
		int sx = (x1 < x2) ? 1 : -1;
		int dy = ABS(y2 - y1);
		int sy = (y1 < y2) ? 1 : -1; 
		int err = ((dx > dy) ? dx : -dy)/2, e2;
		
		int off = y1*DRAW_SCREEN_W + x1;
		int off_y = (y1 < y2) ? DRAW_SCREEN_W : -DRAW_SCREEN_W;
		
		for(;;){
			draw_framebuffer[off] = draw_color;
			
			if(x1 == x2 && y1 == y2)
				break;
			
			e2 = err;
			
			if(e2 > -dx) {
				err -= dy;
				x1 += sx;
				off += sx;
			}
			if(e2 < dy) {
				err += dx;
				y1 += sy;
				off += off_y;
			}
		}
	}
}

void draw_line_set_free(DrawLineSet *set) {
	if(!set)
		return;
	free(set->line);
	free(set);
}
