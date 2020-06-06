#ifndef BRUH_CURSOR_H
#define BRUH_CURSOR_H

#include <X11/Xlib.h>
#include "../includes/clients.h"

void bruh_initialize_cursors(Display *display);
void bruh_setup_standard_cursor(Display *display, Window root_window);
void bruh_setup_frame_button1_press_cursor(
    Display *display,
    XButtonPressedEvent *event,
    bruh_client *client
);
void bruh_setup_frame_pointer_motion_cursor(
    Display *display,
    XMotionEvent *event,
    bruh_client *client
);

#endif