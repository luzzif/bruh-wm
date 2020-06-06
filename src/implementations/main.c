#include <stdio.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include "../includes/events.h"
#include "../includes/cursor.h"

int main(int argc, char **argv) {
    Display *display = XOpenDisplay(NULL);
    if(display == NULL) {
        printf("could not initialize x server connection\n");
        return 1;
    }
    Window root_window = XDefaultRootWindow(display);
    XSelectInput(
        display,
        root_window,
        SubstructureRedirectMask | SubstructureNotifyMask
    );
    bruh_initialize_cursors(display);
    bruh_setup_standard_cursor(display, root_window);
    bruh_handle_events(display, DefaultScreen(display));
}