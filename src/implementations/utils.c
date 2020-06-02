#include <math.h>
#include "../includes/utils.h"

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
) {
    Status status = XGetGeometry(
        display,
        window,
        root_window,
        x,
        y,
        width,
        height,
        border_width,
        depth
    );
    if(status == 0) {
        printf("could not get geometry for window\n");
        exit(1);
    } 
}

void bruh_get_window_dimensions(
    Display *display,
    Drawable window,
    unsigned int *width,
    unsigned int *height
) {
    int x;
    int y;
    unsigned int border_width;
    unsigned int depth;
    Window root_window;
    bruh_get_geometry_checked(
        display,
        window,
        &root_window,
        &x, &y,
        width, height,
        &border_width,
        &depth
    );
}

double bruh_degrees_to_radians(int degrees) {
    return degrees * (M_PI / 180.);
}
