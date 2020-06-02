#include <stdio.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include "../includes/events.h"

int main(int argc, char **argv) {
    Display *display = XOpenDisplay(NULL);
    if(display == NULL) {
        printf("could not initialize x server connection\n");
        return 1;
    }
    Window root_window = XDefaultRootWindow(display);
    XDefineCursor(display, root_window, XCreateFontCursor(display, 68));
    XSelectInput(
        display,
        root_window,
        SubstructureRedirectMask | SubstructureNotifyMask
    );
    bruh_handle_events(display, DefaultScreen(display));
}