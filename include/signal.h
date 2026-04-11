#ifndef SIGNAL_H
#define SIGNAL_H

#include "calc.h"
#include <gtk/gtk.h>

typedef struct{
    GtkEntry *display;
    double result;
    char pending_op;
    int fresh_input;
}calcstate;

void signal_connect_all(GtkBuilder *builder, calcstate *state);

#endif
