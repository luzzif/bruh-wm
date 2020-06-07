#include "../includes/ui.h"
#include <X11/Xutil.h>
#include "../includes/constants.h" 
#include "../includes/utils.h" 

void bruh_define_frame_structure(
    Display *display,
    cairo_t *cairo,
    int width, int height
) {
    cairo_move_to(cairo, TOOLBAR_FRAME_BORDER_RADIUS, 0);
    cairo_line_to(cairo, width - TOOLBAR_FRAME_BORDER_RADIUS, 0);
    cairo_arc(
        cairo,
        width - TOOLBAR_FRAME_BORDER_RADIUS, TOOLBAR_FRAME_BORDER_RADIUS,
        TOOLBAR_FRAME_BORDER_RADIUS,
        bruh_degrees_to_radians(270), 0
    );
    cairo_line_to(cairo, width, height);
    cairo_line_to(cairo, 0, height);
    cairo_line_to(cairo, 0, TOOLBAR_FRAME_BORDER_RADIUS);
    cairo_arc(
        cairo,
        TOOLBAR_FRAME_BORDER_RADIUS, TOOLBAR_FRAME_BORDER_RADIUS,
        TOOLBAR_FRAME_BORDER_RADIUS,
        bruh_degrees_to_radians(180), bruh_degrees_to_radians(270)
    );
    cairo_clip(cairo);
}

void bruh_draw_frame_portion(
    cairo_t *cairo,
    int x, int y,
    unsigned int width, unsigned int height
) {
    cairo_rectangle(cairo, x, y, width, height);
    cairo_set_source_rgb(cairo, 1, 0, 0);
    cairo_paint(cairo);
}