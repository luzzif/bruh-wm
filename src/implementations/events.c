#include <X11/Xlib.h>
#include <cairo/cairo-xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../includes/events.h"
#include "../includes/utils.h"
#include "../includes/clients.h"

#define TOOLBAR_FRAME_BORDER_SIZE 28
#define TOOLBAR_FRAME_BORDER_RADIUS 8
#define THIN_FRAME_BORDER_SIZE 1

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
                bruh_handle_button_release(display);
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
    unsigned int parent_width = width + THIN_FRAME_BORDER_SIZE *2; 
    unsigned int parent_height = height + THIN_FRAME_BORDER_SIZE + TOOLBAR_FRAME_BORDER_SIZE; 
    Window parent = XCreateSimpleWindow(
        display,
        root_window,
        x, y,
        parent_width, parent_height,
        0, 0, 0
    );
    XSelectInput(display, parent, ButtonPressMask | ExposureMask);
    XReparentWindow(
        display,
        child,
        parent,
        THIN_FRAME_BORDER_SIZE,
        TOOLBAR_FRAME_BORDER_SIZE
    );
    XMapWindow(display, parent);
    XMapWindow(display, child);
    bruh_client *new_client = (bruh_client *) malloc(sizeof(bruh_client));
    if(new_client == NULL) {
        printf("could not allocate new client: out of memory\n");
        exit(1);
    }
    bruh_frame *new_frame = new_client->frame; 
    new_frame = (bruh_frame *) malloc(sizeof(bruh_frame));
    if(new_frame == NULL) {
        printf("could not allocate new frame struct: out of memory\n");
        exit(1);
    }
    new_frame->window = parent;
    bruh_add_client(new_client);
}

void bruh_handle_button_press(Display *display, XButtonPressedEvent *event) {
    Window window = event->window;
    if(window == None) {
        return;
    }
    bruh_client *client = bruh_get_client_by_frame(window);
    client->frame->click_event = 
            (XButtonPressedEvent *) malloc(sizeof(XButtonPressedEvent));
    if(client->frame->click_event == NULL) {
        printf("could not allocate copied event: out of memory\n");
        exit(1);
    }
    memcpy(client->frame->click_event, event, sizeof(XButtonPressedEvent));
    switch(event->button) {
        case 1: {
            if(!client) {
                return;
            }
            XGrabPointer(
                display,
                window,
                False,
                PointerMotionMask | ButtonReleaseMask,
                GrabModeAsync,
                GrabModeAsync,
                None,
                None,
                CurrentTime
            );
            break;
        }
    }
}

void bruh_handle_button_release(Display *display) {
    XUngrabPointer(display, CurrentTime);
}


void bruh_handle_pointer_motion(Display *display, XMotionEvent *event) {
    Window window = event->window;
    bruh_client *client = bruh_get_client_by_frame(window);
    if(!client) {
        return;
    }
    bruh_frame *frame = client->frame;
    XMoveWindow(
        display,
        window,
        event->x_root - frame->click_event->x,
        event->y_root - frame->click_event->y
    );
}

// TODO: the window is fully redrawn every time... this needs to be changed
void bruh_handle_expose(Display *display, int default_screen, XExposeEvent *event) {
    Window window = event->window;
    bruh_client *client = bruh_get_client_by_frame(window);
    if(!client) {
        return;
    }
    unsigned int width;
    unsigned int height;
    bruh_get_window_dimensions(display, window, &width, &height);
    cairo_surface_t *cairo_surface = cairo_xlib_surface_create(
        display,
        window,
        XDefaultVisual(display, default_screen),
        width, height
    );
    cairo_t *cairo = cairo_create(cairo_surface);
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
    cairo_set_source_rgb(cairo, 1, 0, 0);
    cairo_paint(cairo);
    cairo_destroy(cairo);
    cairo_surface_destroy(cairo_surface);
}
