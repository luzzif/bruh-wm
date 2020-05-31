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