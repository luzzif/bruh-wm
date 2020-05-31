#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>
#include "../includes/events.h"
#include "../includes/utils.h"

#define TOOLBAR_FRAME_BORDER_SIZE 28
#define TOOLBAR_FRAME_BORDER_RADIUS 16
#define THIN_FRAME_BORDER_SIZE 1

void bruh_handle_events(Display *display, int default_screen) {
    while(1) {
        XEvent event;
        XNextEvent(display, &event);
        switch(event.type) {
            case MapRequest: {
                bruh_handle_map(
                    display,
                    default_screen,
                    (XMapRequestEvent *) &event
                );
                break;
            }
        }
    }
}

void bruh_handle_map(
        Display *display,
        int default_screen,
        XMapRequestEvent *event) {
    Window child = event->window;
    int x;
    int y;
    unsigned int width;
    unsigned int height;
    unsigned int border_width;
    unsigned int depth;
    Window root_window;
    bruh_get_geometry_checked(
        display,
        child,
        &root_window,
        &x, &y,
        &width, &height,
        &border_width,
        &depth
    );
    unsigned int parent_width = width + THIN_FRAME_BORDER_SIZE *2; 
    unsigned int parent_height = height + THIN_FRAME_BORDER_SIZE + TOOLBAR_FRAME_BORDER_SIZE; 
    Window parent = XCreateSimpleWindow(
        display,
        root_window,
        x, y,
        parent_width, parent_height,
        0,
        0,
        0
    );
    XReparentWindow(
        display,
        child,
        parent,
        THIN_FRAME_BORDER_SIZE,
        TOOLBAR_FRAME_BORDER_SIZE
    );
    XMapWindow(display, parent);
    XMapWindow(display, child);
    cairo_surface_t *cairo_surface = cairo_xlib_surface_create(
        display,
        parent,
        XDefaultVisual(display, default_screen),
        parent_width, parent_height
    );
    cairo_t *cairo = cairo_create(cairo_surface);
    cairo_move_to(cairo, TOOLBAR_FRAME_BORDER_RADIUS, 0);
    cairo_line_to(cairo, parent_width - TOOLBAR_FRAME_BORDER_RADIUS, 0);
    cairo_arc(
        cairo,
        parent_width - TOOLBAR_FRAME_BORDER_RADIUS, TOOLBAR_FRAME_BORDER_RADIUS,
        TOOLBAR_FRAME_BORDER_RADIUS,
        bruh_degrees_to_radiants(270), 0
    );
    cairo_line_to(cairo, parent_width, parent_height);
    cairo_line_to(cairo, 0, parent_height);
    cairo_line_to(cairo, 0, TOOLBAR_FRAME_BORDER_RADIUS);
    cairo_arc(
        cairo,
        TOOLBAR_FRAME_BORDER_RADIUS, TOOLBAR_FRAME_BORDER_RADIUS,
        TOOLBAR_FRAME_BORDER_RADIUS,
        bruh_degrees_to_radiants(180), bruh_degrees_to_radiants(270)
    );
    cairo_set_source_rgb(cairo, 1, 0, 0);
    cairo_fill(cairo);
    XFlush(display);
}
