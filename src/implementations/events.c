#include <gdk/gdk.h>
#include "../includes/events.h"

void bruh_handle_events() {
    while(1) {
        GdkEvent *event = gdk_event_get();
        if(!event) {
            continue;
        }
        GdkEventType event_type = gdk_event_get_event_type(event);
        if(event_type < 0) {
            continue;
        }
        switch(event_type) {
            case GDK_MAP: {
                printf("mapped\n");
                break;
            }
            case GDK_UNMAP: {
                printf("unmapped\n");
                break;
            }
        }
        gdk_event_free(event);
    }
}