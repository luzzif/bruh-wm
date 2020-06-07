#ifndef BRUH_UI_H
#define BRUH_UI_H

#include <X11/Xlib.h>
#include <cairo/cairo.h> 

void bruh_define_frame_structure(
    Display *display,
    cairo_t *cairo,
    int width, int height
);

void bruh_draw_frame_portion(
    cairo_t *cairo,
    int x, int y,
    unsigned int width, unsigned int height
);

#endif