#ifndef BRUH_CLIENTS_H
#define BRUH_CLIENTS_H

#include <X11/Xlib.h>
#include <cairo/cairo.h>

typedef struct {
    Window window;
    cairo_surface_t *cairo_surface;
    cairo_t *cairo;
    XButtonPressedEvent *click_event;
} bruh_frame;

typedef struct {
    bruh_frame *frame;
    Window child;
} bruh_client;

typedef struct {
    bruh_client *data;
    void* next;
} bruh_node;

void bruh_add_client(bruh_client *new_client);
void bruh_remove_client(bruh_client *old_client);
bruh_client *bruh_get_client_by_frame(Window frame);
bruh_client *bruh_get_client_by_child(Window child);

#endif