#ifndef BRUH_CLIENTS_H
#define BRUH_CLIENTS_H

#include <X11/Xlib.h>
#include <stdbool.h>

typedef struct {
    Window window;
    XButtonPressedEvent *click_event;
} bruh_frame;

typedef struct {
    bruh_frame *frame;
} bruh_client;

typedef struct {
    bruh_client *data;
    void* next;
} bruh_node;

void bruh_add_client(bruh_client *new_client);
bruh_client *bruh_get_client_by_frame(Window frame);

#endif