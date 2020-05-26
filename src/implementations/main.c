#include <stdio.h>
#include <gtk-3.0/gdk/gdk.h>
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
    GdkSeat *seat = gdk_display_get_default_seat(display);
    GdkDevice *device = gdk_seat_get_pointer(seat);
    GdkWindow *root_window = gdk_get_default_root_window();
    gdk_window_set_device_events(root_window, device, GDK_BUTTON_PRESS_MASK);
    bruh_handle_events();
}