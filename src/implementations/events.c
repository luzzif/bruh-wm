#include <X11/Xlib.h>
#include "../includes/events.h"
#include "../includes/utils.h"

#define FRAME_BORDER_SIZE 10

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
    Window parent = XCreateSimpleWindow(
        display,
        root_window,
        x, y,
        width + FRAME_BORDER_SIZE *2, height + FRAME_BORDER_SIZE * 2,
        0,
        0,
        XWhitePixel(display, default_screen)
    );
    XReparentWindow(
        display,
        child,
        parent,
        FRAME_BORDER_SIZE,
        FRAME_BORDER_SIZE
    );
    XMapWindow(display, parent);
    XMapWindow(display, child);
    XFlush(display);
}
