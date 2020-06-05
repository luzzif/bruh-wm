#ifndef BRUH_EVENTS_H
#define BRUH_EVENTS_H

#include <X11/Xlib.h>
#include "../includes/clients.h"

void bruh_handle_events(Display *display, int default_screen);
void bruh_handle_map(
    Display *display,
    int default_screen,
    XMapRequestEvent *event
);
void bruh_handle_button_press(Display *display, XButtonPressedEvent *event);
void bruh_handle_button_release(Display *display, XButtonReleasedEvent *event);
void bruh_handle_pointer_motion(Display *display, XMotionEvent *event);
void bruh_setup_clicked_client_cursor(
    Display *display,
    XButtonPressedEvent *event,
    bruh_client *clicked_frame
);
void bruh_handle_expose(
    Display *display,
    int default_screen,
    XExposeEvent *event
);
void bruh_handle_unmap(Display *display, XUnmapEvent *event);

#endif