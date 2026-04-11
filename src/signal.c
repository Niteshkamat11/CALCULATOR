#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "signals.h"

//for all the digit 0-9
static void on_digit(GtkButton *btn, gpointer user_data) {
    calcstate  *state   = (calcstate *)user_data;
    const char *label   = gtk_button_get_label(btn);
    const char *current = gtk_editable_get_text(GTK_EDITABLE(state->display));
    char        buff[128];

    if (state->fresh_input) {
        gtk_editable_set_text(GTK_EDITABLE(state->display), label);
        state->fresh_input = 0;
    } else {
        snprintf(buff, sizeof(buff), "%s%s", current, label);
        gtk_editable_set_text(GTK_EDITABLE(state->display), buff);
    }
}
//for operator like +,-,/,%,*
static void on_operator(GtkButton *btn, gpointer user_data) {
    calcstate  *state   = (calcstate *)user_data;
    const char *current = gtk_editable_get_text(GTK_EDITABLE(state->display));
    const char *label   = gtk_button_get_label(btn);

    if (state->pending_op != 0 && !state->fresh_input) {
        int    err    = 0;
        double second = atof(current);
        state->result = calc_evaluate(state->result, second, state->pending_op, &err);

        char buff[128];
        snprintf(buff, sizeof(buff), "%.10g", state->result);
        gtk_editable_set_text(GTK_EDITABLE(state->display), buff);
   } else {
        state->result = atof(current);
    }

    state->pending_op = label[0];
    state->fresh_input = 1;
}
//for = 
static void on_equals(GtkButton *btn, gpointer user_data) {
    calcstate  *state   = (calcstate *)user_data;
    const char *current = gtk_editable_get_text(GTK_EDITABLE(state->display));
    double      second  = atof(current);
    int         err     = 0;

    double result = calc_evaluate(state->result, second, state->pending_op, &err);

    char buff[90];
    if (err) {
        gtk_editable_set_text(GTK_EDITABLE(state->display), "Error");
    } else {
        snprintf(buff, sizeof(buff), "%.10g", result);
        gtk_editable_set_text(GTK_EDITABLE(state->display), buff);
    }

    state->result      = result;
    state->pending_op  = 0;
    state->fresh_input = 1;
}

