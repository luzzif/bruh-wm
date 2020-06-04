#ifndef BRUH_EVENTS_H
#define BRUH_EVENTS_H

#include <X11/Xlib.h>

void bruh_handle_events(Display *display, int default_screen);
void bruh_handle_map(
    Display *display,
    int default_screen,
    XMapRequestEvent *event
);
void bruh_handle_button_press(Display *display, XButtonPressedEvent *event);
void bruh_handle_button_release(Display *display);
void bruh_handle_pointer_motion(Display *display, XMotionEvent *event);
void bruh_handle_expose(
    Display *display,
    int default_screen,
    XExposeEvent *event
);
void bruh_handle_unmap(Display *display, XUnmapEvent *event);

#endif