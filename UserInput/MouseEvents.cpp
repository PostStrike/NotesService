#include "MouseEvents.h"

void window_clicked(GdkEventButton *event, gpointer data) {
    int x = static_cast<int>(event->x);
    int y = static_cast<int>(event->y);
}