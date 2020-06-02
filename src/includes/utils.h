#ifndef BRUH_UTILS_H
#define BRUH_UTILS_H

#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>

void bruh_get_geometry_checked(
    Display *display,
    Drawable window,
    Window *root_window,
    int *x,
    int *y,
    unsigned int *width,
    unsigned int *height,
    unsigned int *border_width, 
    unsigned int *depth
);
void bruh_get_window_dimensions(
    Display *display,
    Drawable window,
    unsigned int *width,
    unsigned int *height
);
double bruh_degrees_to_radians(int degrees);

#endif