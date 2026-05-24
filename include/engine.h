#ifndef ENGINE_H
#define ENGINE_H

#include <gtk/gtk.h>
#include "calc.h"

typedef struct{
    GtkWidget *display;
    double result;
    char pending_op;
    int fresh_input;
    char current_number[64];
    int open_bracket;
    int fresh_equal;
}calcstate;

void engine_digit(calcstate *state, const char *label);
void engine_operator(calcstate *state, const char *label);
void engine_equals(calcstate *state);
void engine_clear(calcstate *state);
void engine_delete(calcstate *state);
void engine_bracket(calcstate *state);

#endif
