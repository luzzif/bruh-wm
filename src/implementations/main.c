#include <stdio.h>
#include <gdk/gdk.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include "../includes/events.h"

int main(int argc, char **argv) {
    if(!gdk_init_check((gint *) &argc, (gchar ***) &argv)) {
        printf("could not initialize gdk\n");
        return 1;
    }
    GdkDisplay *display = gdk_display_get_default();
    if(!display) {
        printf("could not get display\n");
        return 1;
    }
    GdkWindow *root_window = gdk_get_default_root_window();
    gdk_window_set_events(root_window, GDK_SUBSTRUCTURE_MASK);
    bruh_handle_events();
}