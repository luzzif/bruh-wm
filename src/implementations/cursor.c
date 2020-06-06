#ifndef BRUH_CURSOR_H
#define BRUH_CURSOR_H

#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <stdbool.h>
#include "../includes/clients.h"
#include "../includes/constants.h"

Cursor standard_cursor;
Cursor moving_cursor;
Cursor expand_top_cursor;
Cursor expand_top_right_cursor;
Cursor expand_top_left_cursor;
Cursor expand_right_cursor;
Cursor expand_left_cursor;
Cursor expand_bottom_right_cursor;
Cursor expand_bottom_left_cursor;
Cursor expand_bottom_cursor;

void bruh_initialize_cursors(Display *display) {
    standard_cursor = XCreateFontCursor(display, XC_arrow);
    moving_cursor = XCreateFontCursor(display, XC_hand1);
    expand_top_cursor = XCreateFontCursor(display, XC_top_side);
    expand_top_cursor = XCreateFontCursor(display, XC_top_side);
    expand_top_right_cursor = XCreateFontCursor(display, XC_top_right_corner);
    expand_top_left_cursor = XCreateFontCursor(display, XC_top_left_corner);
    expand_right_cursor = XCreateFontCursor(display, XC_right_side);
    expand_left_cursor = XCreateFontCursor(display, XC_left_side);
    expand_bottom_right_cursor = XCreateFontCursor(display, XC_bottom_right_corner);;
    expand_bottom_left_cursor = XCreateFontCursor(display, XC_bottom_left_corner);
    expand_bottom_cursor = XCreateFontCursor(display, XC_bottom_side);
}

void bruh_setup_standard_cursor(Display *display, Window root_window) {
    XDefineCursor(display, root_window, standard_cursor);
}

void bruh_setup_frame_button1_press_cursor(
    Display *display,
    XButtonPressedEvent *event,
    bruh_client *client
) {
    int x = event->x;
    int y = event->y;
    int client_width = client->width;
    if((y <= TOOLBAR_FRAME_BORDER_RADIUS &&
        x > TOOLBAR_FRAME_BORDER_RADIUS &&  
        x < client_width - TOOLBAR_FRAME_BORDER_RADIUS) ||
        (y > TOOLBAR_FRAME_BORDER_RADIUS &&
        y <= TOOLBAR_FRAME_BORDER_SIZE &&
        x > THIN_FRAME_BORDER_SIZE &&  
        x < client_width - THIN_FRAME_BORDER_SIZE)) {
        XDefineCursor(display, client->frame->window, moving_cursor);
    }
}

void bruh_setup_frame_pointer_motion_cursor(
    Display *display,
    XMotionEvent *event,
    bruh_client *client
) {
    int y = event->y;
    int x = event->x;
    bool top = false;
    bool left = false;
    bool right = false;
    bool bottom = false;
    bool in_upper_corner = false;
    if(y <= TOOLBAR_FRAME_BORDER_RADIUS) {
        in_upper_corner = x <= TOOLBAR_FRAME_BORDER_RADIUS || 
                x >= client->width - TOOLBAR_FRAME_BORDER_RADIUS;
        top = in_upper_corner || y <= THIN_FRAME_BORDER_SIZE / 2;
    } else if(y >= client->height - THIN_FRAME_BORDER_SIZE / 2) {
        bottom = true;
    }
    left = in_upper_corner ? 
            x <= TOOLBAR_FRAME_BORDER_RADIUS : 
            x <= THIN_FRAME_BORDER_SIZE / 2;
    right = in_upper_corner ? 
            x >= client->width - TOOLBAR_FRAME_BORDER_RADIUS : 
            x >= client->width - THIN_FRAME_BORDER_SIZE / 2;
    Cursor required_cursor;
    if(top && left) {
        required_cursor = expand_top_left_cursor;
    } else if(top && !left && !right) {
        required_cursor = expand_top_cursor;
    } else if(top && right) {
        required_cursor = expand_top_right_cursor;
    } else if(right && !top && !bottom) {
        required_cursor = expand_right_cursor;
    } else if(bottom && right) {
        required_cursor = expand_bottom_right_cursor;
    } else if(bottom && !right && !left) {
        required_cursor = expand_bottom_cursor; 
    } else if(bottom && left) {
        required_cursor = expand_bottom_left_cursor; 
    } else if(left && !bottom && !top) {
        required_cursor = expand_left_cursor; 
    } else {
        required_cursor = standard_cursor; 
    }
    XDefineCursor(display, client->frame->window, required_cursor);
}

#endif