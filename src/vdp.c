#include "vdp.h"

// Internal functions

void _write16bit(UINT16 w) {
	putch(w & 0xFF); // write LSB
	putch(w >> 8);	 // write MSB	
}

void _write32bit(UINT32 l) {
	UINT32 temp = l;
	
	putch(temp & 0xFF); // write LSB
	temp = temp >> 8;
	putch(temp & 0xFF);
	temp = temp >> 8;
	putch(temp & 0xFF);
	temp = temp >> 8;
	putch(temp & 0xFF);
	return;	
}

// Generic functions

void vdp_set_mode(unsigned char mode) {
    putch(22);
    putch(mode);
}

void vdp_get_mode(void) {
	putch(23);
	putch(0);
	putch(0x86);
}

void vdp_switch_buffers() {
	// VDU 23, 0, &C3
	putch(23);
	putch(0);
	putch(0xC3);
}

void vdp_set_palette_color(UINT8 index, UINT8 color, UINT8 r, UINT8 g, UINT8 b) {
	putch(0x13); // VDU palette
	putch(index);
	putch(color); // 255 - set R/G/B colors, or <80 color lookup table
	putch(r);
	putch(g);
	putch(b);
}

// Text functions
void vdp_cls() {
    putch(12);
}

void vdp_cursor_home() {
    putch(30);
}

void vdp_cursor_up() {
    putch(11);
}

void vdp_cursor_goto(unsigned char x, unsigned char y) {
    putch(31); // TAB
    putch(x);
    putch(y);
}

void vdp_set_fgcolor(unsigned char colorindex) {
	putch(17);
	putch(colorindex);	
}

void vdp_set_bgcolor(unsigned char colorindex) {
	putch(17);
	putch(colorindex | 0x80);	
}

// Graphics functions

void vdp_set_logical_coords(unsigned char n) {
	putch(23);	 	// vdu_sys
	putch(0);       // vdu_sys_video
	putch(0xC0);	// logical coords mode VDP_LOGICALCOORDS
	putch(n);       // mode (0 = off, >0 = on)
}

void vdp_clear_graphics() {
    putch(16);    
}

void vdp_set_graphics_viewport(unsigned int left, unsigned int bottom, unsigned int right, unsigned int top) {
	putch(24);		
	_write16bit(left);
	_write16bit(bottom);
	_write16bit(right);
	_write16bit(top);
}

void vdp_reset_viewports() {
	putch(26);
}

void vdp_set_plot_color(unsigned char colorindex) {
	putch(18);
    putch(1);
	putch(colorindex);
}

// internal function
void _vdp_plot(unsigned char mode, unsigned int x, unsigned int y) {
    putch(25);
    putch(mode);
    putch(x & 0xFF);
    putch(x >> 8);
    putch(y & 0xFF);
    putch(y >> 8);
}

void vdp_plot_moveto(unsigned int x, unsigned int y) {
	_vdp_plot(0x04, x, y);
}

void vdp_plot_lineto(unsigned int x, unsigned int y) {
	_vdp_plot(0x05, x, y);
}

void vdp_plot_point(unsigned int x, unsigned int y) {
	_vdp_plot(0x45, x, y);
}

void vdp_plot_triangle(unsigned int x, unsigned int y) {
	_vdp_plot(0x50, x, y);
}

void vdp_plot_circle_radius(unsigned int r) {
	_vdp_plot(0x91, r, 0);
}

void vdp_plot_circle_circumference(unsigned int x, unsigned int y) {
	_vdp_plot(0x95,x,y);
}

void vdp_plot_rectangle_filled(unsigned int x, unsigned int y) {
	_vdp_plot(0x61, x, y);
}

void vdp_set_plot_origin(unsigned int x, unsigned int y) {
    putch(29);
    putch(x & 0xF);
    putch(x >> 8);
    putch(y & 0xF);
    putch(y >> 8);
}

// Bitmap VDP functions
void vdp_bitmap_select(UINT8 id) {
	putch(23); // vdu_sys
	putch(27); // sprite command
	putch(0);  // select command
	putch(id); // bitmap_id
}

void vdp_bitmap_send_data_selected(UINT16 width, UINT16 height, UINT32 *data) {
	UINT16 n;
	
	putch(23); // vdu_sys
	putch(27); // sprite command
	putch(1);  // send data to selected bitmap
	
	_write16bit(width);
	_write16bit(height);
	
	for (n = 0; n < (width*height); n++) {
		_write32bit(data[n]);
	}
}

void vdp_bitmap_send_data(UINT8 id, UINT16 width, UINT16 height, UINT32 *data) {
	vdp_bitmap_select(id);
	vdp_bitmap_send_data_selected(width, height, data);
}

void vdp_bitmap_draw_selected(UINT16 x, UINT16 y) {
	putch(23); // vdu_sys
	putch(27); // sprite command
	putch(3);  // draw selected bitmap
	
	_write16bit(x);
	_write16bit(y);
}

void vdp_bitmap_draw(UINT8 id, UINT16 x, UINT16 y) {
	vdp_bitmap_select(id);
	vdp_bitmap_draw_selected(x,y);
}

void vdp_bitmap_create_solid_color_selected(UINT16 width, UINT16 height, UINT32 abgr) {
	putch(23); // vdu_sys
	putch(27); // sprite command
	putch(2);  // define in single color command
	
	_write16bit(width);
	_write16bit(height);
	_write32bit(abgr);
}

void vdp_bitmap_create_solid_color(UINT8 id, UINT16 width, UINT16 height, UINT32 abgr) {
	vdp_bitmap_select(id);
	vdp_bitmap_create_solid_color_selected(width, height, abgr);
}

// Sprite VDP functions

void vdp_sprite_select(UINT8 id) {
	putch(23); // vdu_sys
	putch(27); // sprite command
	putch(4);  // select sprite
	putch(id);
}

void vdp_sprite_clear_frames_selected(void) {
	putch(23); // vdu_sys
	putch(27); // sprite command
	putch(5);  // clear frames
}

void vdp_sprite_clear_frames(UINT8 bitmapid) {
	vdp_sprite_select(bitmapid);
	vdp_sprite_clear_frames_selected();
}

void vdp_sprite_add_frame_selected(UINT8 bitmapid) {
	putch(23); // vdu_sys
	putch(27); // sprite command
	putch(6);  // add frame
	putch(bitmapid);
}

void vdp_sprite_add_frame(UINT8 id, UINT8 bitmapid) {
	vdp_sprite_select(id);
	vdp_sprite_add_frame_selected(bitmapid);
}

void vdp_sprite_next_frame_selected(void) {
	putch(23); // vdu_sys
	putch(27); // sprite command
	putch(8);  // next frame
}

void vdp_sprite_next_frame(UINT8 id) {
	vdp_sprite_select(id);
	vdp_sprite_next_frame_selected();
}

void vdp_sprite_previous_frame_selected(void) {
	putch(23); // vdu_sys
	putch(27); // sprite command
	putch(9); // previous frame
}

void vdp_sprite_previous_frame(UINT8 id) {
	vdp_sprite_select(id);
	vdp_sprite_previous_frame_selected();
}

void vdp_sprite_set_frame_selected(UINT8 framenumber) {
	putch(23); // vdu_sys
	putch(27); // sprite command
	putch(10); // set current frame
	putch(framenumber);
}

void vdp_sprite_set_frame(UINT8 id, UINT8 framenumber) {
	vdp_sprite_select(id);
	vdp_sprite_set_frame_selected(framenumber);
}

void vdp_sprite_show_selected(void) {
	putch(23); // vdu_sys
	putch(27); // sprite command
	putch(11); // show sprite
}

void vdp_sprite_show(UINT8 id) {
	vdp_sprite_select(id);
	vdp_sprite_show_selected();
}

void vdp_sprite_hide_selected(void) {
	putch(23); // vdu_sys
	putch(27); // sprite command
	putch(12); // hide sprite
}

void vdp_sprite_hide(UINT8 id) {
	vdp_sprite_select(id);
	vdp_sprite_hide_selected();
}

void vdp_sprite_moveto_selected(UINT16 x, UINT16 y) {
	putch(23); // vdu_sys
	putch(27); // sprite command
	putch(13); // move to
	_write16bit(x);
	_write16bit(y);
}

void vdp_sprite_moveto(UINT8 id, UINT16 x, UINT16 y) {
	vdp_sprite_select(id);
	vdp_sprite_moveto_selected(x,y);
}

void vdp_sprite_moveby_selected(UINT16 x, UINT16 y) {
	putch(23); // vdu_sys
	putch(27); // sprite command
	putch(14); // move by
	_write16bit(x);
	_write16bit(y);
}

void vdp_sprite_moveby(UINT8 id, UINT16 x, UINT16 y) {
	vdp_sprite_select(id);
	vdp_sprite_moveby_selected(x,y);
}

void vdp_sprite_activate_total(UINT8 number) {
	putch(23); // vdu_sys
	putch(27); // sprite command
	putch(7);  // set number of sprites
	putch(number);
}

void vdp_sprite_refresh(void) {
	putch(23);	// vdu_sys
	putch(27);	// sprite command
	putch(15);	// refresh all sprites
}

UINT8 vdp_cursor_get_xpos(void) {
	unsigned int delay;
	
	putch(23);	    // VDP command
	putch(0);	    // VDP command
	putch(0x82);	// Request cursor position
	
	delay = 255;
	while (delay--);

	return (getsysvar_cursorX());
}

UINT8 vdp_cursor_get_ypos(void) {
	unsigned int delay;
	
	putch(23);	// VDP command
	putch(0);	// VDP command
	putch(0x82);	// Request cursor position
	
	delay = 255;
	while (delay--);

	return (getsysvar_cursorY());
}

char vdp_ascii_code_at(unsigned char x, unsigned char y) {
	unsigned int delay;
	
	putch(23);	// VDP command
	putch(0);	// VDP command
	putch(0x83);	// Request ascii code at position (x,y)
	putch(x);
	putch(0);
	putch(y);
	putch(0);
	
	delay = 64000;
	while(delay--);

	return (getsysvar_scrchar());
}

void  vdp_set_paged_mode(bool mode) {
	if(mode)
        putch(0x0E);
	else
        putch(0x0F);
}

void vdp_cursor_set_disable(void) {
	putch(23);
	putch(1);
	putch(0);
}

void vdp_cursor_set_enable(void) {
	putch(23);
	putch(1);
	putch(1);
}

void vdp_scroll(unsigned char extent, unsigned char direction, unsigned char speed) {
	putch(23);
	putch(7);	// scroll
	putch(extent);
	putch(direction);
	putch(speed);
}