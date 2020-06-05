#include <cairo/cairo-xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../includes/events.h"
#include "../includes/utils.h"

#define TOOLBAR_FRAME_BORDER_SIZE 28
#define TOOLBAR_FRAME_BORDER_RADIUS 8
#define THIN_FRAME_BORDER_SIZE 2

// TODO: consider predefinig the cursor variants and not creating them every time
void bruh_handle_events(Display *display, int default_screen) {
    while(1) {
        XEvent *event = (XEvent *) malloc(sizeof(XEvent));
        if(event == NULL) {
            printf("could not get next event: out of memory\n");
            exit(1);
        }
        XNextEvent(display, event);
        switch(event->type) {
            case MapRequest: {
                bruh_handle_map(
                    display,
                    default_screen,
                    (XMapRequestEvent *) event
                );
                break;
            }
            case ButtonPress: {
                bruh_handle_button_press(
                    display,
                    (XButtonPressedEvent *) event
                );
                break;
            }
            case ButtonRelease: {   
                bruh_handle_button_release(display, (XButtonReleasedEvent *) event);
                break;
            }
            case UnmapNotify: {
                bruh_handle_unmap(display, (XUnmapEvent *) event);
                break;
            }
            case MotionNotify: {
                bruh_handle_pointer_motion(display, (XMotionEvent *) event);
                break;
            }
            case Expose: {
                bruh_handle_expose(
                    display,
                    default_screen,
                    (XExposeEvent *) event
                );
                break;
            }
        }
        free(event);
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
    unsigned int parent_width = width + THIN_FRAME_BORDER_SIZE * 2; 
    unsigned int parent_height = height + THIN_FRAME_BORDER_SIZE + TOOLBAR_FRAME_BORDER_SIZE; 
    Window parent = XCreateSimpleWindow(
        display,
        root_window,
        x, y,
        parent_width, parent_height,
        0, 0, 0
    );
    XSelectInput(
        display,
        parent, ButtonPressMask | ExposureMask | PointerMotionMask | ButtonReleaseMask
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

    // clipping drawn window region: the actual drawing will be handled on expose.
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
        bruh_degrees_to_radians(270), 0
    );
    cairo_line_to(cairo, parent_width, parent_height);
    cairo_line_to(cairo, 0, parent_height);
    cairo_line_to(cairo, 0, TOOLBAR_FRAME_BORDER_RADIUS);
    cairo_arc(
        cairo,
        TOOLBAR_FRAME_BORDER_RADIUS, TOOLBAR_FRAME_BORDER_RADIUS,
        TOOLBAR_FRAME_BORDER_RADIUS,
        bruh_degrees_to_radians(180), bruh_degrees_to_radians(270)
    );
    cairo_clip(cairo);

    // adding client to state list
    bruh_client *new_client = (bruh_client *) malloc(sizeof(bruh_client));
    if(new_client == NULL) {
        printf("could not allocate new client: out of memory\n");
        exit(1);
    }
    new_client->child = child;
    new_client->width = parent_width;
    new_client->height = parent_height;
    new_client->frame = (bruh_frame *) malloc(sizeof(bruh_frame));
    if(new_client->frame == NULL) {
        printf("could not allocate new frame struct: out of memory\n");
        exit(1);
    }
    new_client->frame->window = parent;
    new_client->frame->cairo_surface = cairo_surface;
    new_client->frame->cairo = cairo;
    bruh_add_client(new_client);
}

void bruh_handle_button_press(Display *display, XButtonPressedEvent *event) {
    Window window = event->window;
    if(window == None) {
        return;
    }
    bruh_client *client = bruh_get_client_by_frame(window);
    if(!client) {
        return;
    }
    bruh_frame *frame = client->frame;
    frame->click_event = 
            (XButtonPressedEvent *) malloc(sizeof(XButtonPressedEvent));
    if(frame->click_event == NULL) {
        printf("could not allocate copied event: out of memory\n");
        exit(1);
    }
    memcpy(frame->click_event, event, sizeof(XButtonPressedEvent));
    bruh_setup_clicked_client_cursor(display, event, client);
}

void bruh_setup_clicked_client_cursor(
        Display *display,
        XButtonPressedEvent *event,
        bruh_client *client) {
    int x_position = event->x;
    int y_position = event->y;
    int client_width = client->width;
    // FIXME: this code is duplicated in the motion handler
    if((y_position <= TOOLBAR_FRAME_BORDER_RADIUS &&
        x_position > TOOLBAR_FRAME_BORDER_RADIUS &&  
        x_position < client_width - TOOLBAR_FRAME_BORDER_RADIUS) ||
        (y_position > TOOLBAR_FRAME_BORDER_RADIUS &&
        y_position <= TOOLBAR_FRAME_BORDER_SIZE &&
        x_position > THIN_FRAME_BORDER_SIZE &&  
        x_position < client_width - THIN_FRAME_BORDER_SIZE)) {
        XDefineCursor(display, client->frame->window, XCreateFontCursor(display, 60));
    }
}

void bruh_handle_button_release(Display *display, XButtonReleasedEvent *event) {
    bruh_client *client = bruh_get_client_by_frame(event->window);
    if(!client) {
        return;
    }
    // if a registered client's frame was clicked, reset any previously set click events
    client->frame->click_event = NULL;
    // reset any particular cursor
    XDefineCursor(display, client->frame->window, XCreateFontCursor(display, 68));
}


void bruh_handle_pointer_motion(Display *display, XMotionEvent *event) {
    Window window = event->window;
    bruh_client *client = bruh_get_client_by_frame(window);
    if(!client) {
        return;
    }
    bruh_frame *frame = client->frame;
    XButtonPressedEvent *click_event = frame->click_event;
    if(click_event) {
        // the user previously clicked to move the window around
        XMoveWindow(
            display,
            window,
            event->x_root - frame->click_event->x,
            event->y_root - frame->click_event->y
        );
    } else {
        // free motion, no previous click on the client's frame
        int y_position = event->y;
        int x_position = event->x;
        // FIXME: need to find a better solution for this mess
        if(y_position <= TOOLBAR_FRAME_BORDER_RADIUS) {
            Cursor cursor;
            if(x_position < TOOLBAR_FRAME_BORDER_RADIUS) {
                cursor = XCreateFontCursor(display, 134);
            } else if(x_position >= TOOLBAR_FRAME_BORDER_RADIUS && 
                x_position <=  client->width - TOOLBAR_FRAME_BORDER_RADIUS) {
                cursor = XCreateFontCursor(display, 138);
            } else if(x_position > client->width - TOOLBAR_FRAME_BORDER_RADIUS) {
                cursor = XCreateFontCursor(display, 136);
            } else {
                cursor = XCreateFontCursor(display, 68);
            }
            XDefineCursor(display, client->frame->window, cursor);
        } else if(y_position > TOOLBAR_FRAME_BORDER_RADIUS && 
            y_position < client->height - THIN_FRAME_BORDER_SIZE) {
            Cursor cursor;
            if(x_position < THIN_FRAME_BORDER_SIZE / 2) {
                cursor = XCreateFontCursor(display, 70);
            } else if(x_position >= client->width - THIN_FRAME_BORDER_SIZE / 2) {
                cursor = XCreateFontCursor(display, 96);
            } else {
                cursor = XCreateFontCursor(display, 68);
            }
            XDefineCursor(display, client->frame->window, cursor);
        } else if(y_position >= client->height - THIN_FRAME_BORDER_SIZE / 2) {
            Cursor cursor;
            if(x_position < THIN_FRAME_BORDER_SIZE / 2) {
                cursor = XCreateFontCursor(display, 12);
            } else if(x_position >= client->width - THIN_FRAME_BORDER_SIZE / 2) {
                cursor = XCreateFontCursor(display, 14);
            } else {
                cursor = XCreateFontCursor(display, 16);
            }
            XDefineCursor(display, client->frame->window, cursor);
        }
    }
}

void bruh_handle_expose(Display *display, int default_screen, XExposeEvent *event) {
    Window window = event->window;
    bruh_client *client = bruh_get_client_by_frame(window);
    if(!client) {
        return;
    }
    cairo_t *cairo = client->frame->cairo;
    cairo_rectangle(cairo, event->x, event->y, event->width, event->height);
    cairo_set_source_rgb(cairo, 1, 0, 0);
    cairo_paint(cairo);
}

void bruh_handle_unmap(Display *display, XUnmapEvent *event) {
    Window window = event->window;
    // TODO: consider implement an is_client_registered helper function
    bruh_client *client = bruh_get_client_by_child(window);
    if(!client) {
        return;
    }
    XDestroyWindow(display, client->frame->window);
    bruh_remove_client(client);
}