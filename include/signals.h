#ifndef SIGNALS_H
#define SIGNALS_H

#include <gtk/gtk.h>
#include "calc.h"

typedef struct{
    GtkWidget *display;
    double result;
    char pending_op;
    int fresh_input;
}calcstate;

void signal_connect_all(GtkBuilder *builder, calcstate *state);

#endif
