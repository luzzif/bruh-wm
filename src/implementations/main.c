#include <stdio.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include "../includes/events.h"

int main(int argc, char **argv) {
    Display *display = XOpenDisplay(NULL);
    if(display == NULL) {
        printf("could not initialize gdk\n");
        return 1;
    }
    Window root_window = XDefaultRootWindow(display);
    int default_screen = DefaultScreen(display);
    XSelectInput(display, root_window, SubstructureRedirectMask);
    XDefineCursor(display, root_window, XCreateFontCursor(display, 68));
    bruh_handle_events(display, default_screen);
}