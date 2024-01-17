#include <agon/vdp_vdu.h>
#include "stdint.h"

#ifndef VDP_H
#define VDP_H

typedef uint8_t  UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;

// DEFAULT COLOR INDEXES
enum {
    BLACK = 0,
    DARK_RED,
    DARK_GREEN,
    DARK_YELLOW,
    DARK_BLUE,
    DARK_MAGENTA,
    DARK_CYAN,
    DARK_WHITE,
    BRIGHT_BLACK,
    BRIGHT_RED,
    BRIGHT_GREEN,
    BRIGHT_YELLOW,
    BRIGHT_BLUE,
    BRIGHT_MAGENTA,
    BRIGHT_CYAN,
    BRIGHT_WHITE
};

// VDP modes
#define VDP_MODE_640x480_16C       0
#define VDP_MODE_640x480_4C        1
#define VDP_MODE_640x480_4C_DB   129
#define VDP_MODE_640x480_2C        2
#define VDP_MODE_640x480_2C_DB   130
#define VDP_MODE_640x240_64C       3
#define VDP_MODE_640x240_16C       4
#define VDP_MODE_640x240_16C_DB  132
#define VDP_MODE_640x240_4C        5
#define VDP_MODE_640x240_4C_DB   133
#define VDP_MODE_640x240_2C        6
#define VDP_MODE_640x240_2C_DB   134
#define VDP_MODE_320x240_64C       8
#define VDP_MODE_320x240_64C_DB  136
#define VDP_MODE_320x240_16C       9
#define VDP_MODE_320x240_16C_DB  137
#define VDP_MODE_320x240_4C       10
#define VDP_MODE_320x240_4C_DB   138
#define VDP_MODE_320x240_2C       11
#define VDP_MODE_320x240_2C_DB   139
#define VDP_MODE_320x200_64C      12
#define VDP_MODE_320x200_64C_DB  140
#define VDP_MODE_320x200_16C      13
#define VDP_MODE_320x200_16C_DB  141
#define VDP_MODE_320x200_4C       14
#define VDP_MODE_320x200_4C_DB   142
#define VDP_MODE_320x200_2C       15
#define VDP_MODE_320x200_2C_DB   143
#define VDP_MODE_800x600_4C       16
#define VDP_MODE_800x600_2C       17
#define VDP_MODE_1024x768_2C      18
#define VDP_MODE_DEFAULT     1

void vdp_set_mode(unsigned char mode);
void vdp_get_mode(void);

// extent: 0 = current text window, 1 = entire screen
// direction: 0 = right, 1 = left, 2 = down, 3 = up
// speed: number of pixels to scroll
void vdp_scroll(unsigned char extent, unsigned char direction, unsigned char speed);

// Switch buffers (double-buffered mode)
void vdp_switch_buffers();

// Set palette color
void vdp_set_palette_color(UINT8 index, UINT8 color, UINT8 r, UINT8 g, UINT8 b);

// Text VDP functions
void  vdp_cls();
void  vdp_cursor_home();
void  vdp_cursor_up();
void  vdp_cursor_goto(unsigned char x, unsigned char y);
UINT8 vdp_cursor_get_xpos(void);
UINT8 vdp_cursor_get_ypos(void);
void  vdp_cursor_set_disable(void);
void  vdp_cursor_set_enable(void);
char  vdp_ascii_code_at(unsigned char x, unsigned char y);
void  vdp_set_paged_mode(bool mode);
void  vdp_set_fgcolor(unsigned char colorindex);
void  vdp_set_bgcolor(unsigned char colorindex);

// Graphic VDP functions
void vdp_set_logical_coords(unsigned char n);
void vdp_clear_graphics();
void vdp_set_graphics_viewport(unsigned int left, unsigned int bottom, unsigned int right, unsigned int top); 
void vdp_reset_viewports(); 
void vdp_set_plot_color(unsigned char colorindex);
void vdp_set_plot_origin(unsigned int x, unsigned int y);
void vdp_plot_moveto(unsigned int x, unsigned int y);
void vdp_plot_lineto(unsigned int x, unsigned int y);
void vdp_plot_point(unsigned int x, unsigned int y);
void vdp_plot_triangle(unsigned int x, unsigned int y);
void vdp_plot_circle_radius(unsigned int r);
void vdp_plot_circle_circumference(unsigned int x, unsigned int y);
void vdp_plot_rectangle_filled(unsigned int x, unsigned int y);

// Bitmap VDP functions
void vdp_bitmap_select(UINT8 id);
void vdp_bitmap_send_data_selected(UINT16 width, UINT16 height, UINT32 *data);
void vdp_bitmap_send_data(UINT8 id, UINT16 width, UINT16 height, UINT32 *data);
void vdp_bitmap_draw_selected(UINT16 x, UINT16 y);
void vdp_bitmap_draw(UINT8 id, UINT16 x, UINT16 y);
void vdp_bitmap_create_solid_color_selected(UINT16 width, UINT16 height, UINT32 abgr);
void vdp_bitmap_create_solid_color(UINT8 id, UINT16 width, UINT16 height, UINT32 abgr);

// Sprite VDP functions
void vdp_sprite_select(UINT8 id);
void vdp_sprite_clearFramesSelected(void);
void vdp_sprite_clearFrames(UINT8 bitmapid);
void vdp_sprite_add_frame_selected(UINT8 bitmapid);
void vdp_sprite_add_frame(UINT8 id, UINT8 bitmapid);
void vdp_sprite_next_frame_selected(void);
void vdp_sprite_next_frame(UINT8 id);
void vdp_sprite_previous_frame_selected(void);
void vdp_sprite_previous_frame(UINT8 id);
void vdp_sprite_set_frame_selected(UINT8 framenumber);
void vdp_sprite_set_frame(UINT8 id, UINT8 framenumber);
void vdp_sprite_show_selected(void);
void vdp_sprite_show(UINT8 id);
void vdp_sprite_hide_selected(void);
void vdp_sprite_hide(UINT8 id);
void vdp_sprite_moveto_selected(UINT16 x, UINT16 y);
void vdp_sprite_moveto(UINT8 id, UINT16 x, UINT16 y);
void vdp_sprite_move_by_selected(UINT16 x, UINT16 y);
void vdp_sprite_move_by(UINT8 id, UINT16 x, UINT16 y);
void vdp_sprite_activate_total(UINT8 number);
void vdp_sprite_refresh(void);

#endif
