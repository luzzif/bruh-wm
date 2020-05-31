#ifndef BRUH_EVENTS_H
#define BRUH_EVENTS_H

#include <X11/Xlib.h>

void bruh_handle_events(Display *display, int default_screen);
void bruh_handle_map(
    Display *display,
    int default_screen,
    XMapRequestEvent *event
);

#endif