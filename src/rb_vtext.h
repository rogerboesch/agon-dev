//
//  rb_vtext.h
//  3d wireframe game engine
//
//  Vector font drawing
//
//  04-08-2021, created by Roger Boesch
//  Copyright © 2021 by Roger Boesch - use only with permission
//

#ifndef RB_VTEXT_H
#define RB_VTEXT_H

typedef unsigned char byte;

#ifdef __cplusplus
extern "C" {
#endif

void vtext_draw_string(int x, int y, char* str, float scale);
   
#ifdef __cplusplus
}
#endif

#endif
