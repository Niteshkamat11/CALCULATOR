#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/signals.h"

//for all the digit 0-9
static void on_digit(GtkButton *btn, gpointer user_data) {
    calcstate  *state   = (calcstate *)user_data;
    const char *label   = gtk_button_get_label(btn);
    const char *current = gtk_editable_get_text(GTK_EDITABLE(state->display));
    char        buff[128];

    if (state->fresh_input) {
        state->current_number[0]= '\0';
        state->fresh_input = 0;

        if(strcmp(current, "0")==0){
            current = "";
        }
    } 
    strcat(state->current_number,label);
    snprintf(buff, sizeof(buff), "%s%s", current, label);
    gtk_editable_set_text(GTK_EDITABLE(state->display), buff);
    
}
//for operator like +,-,/,%,*
static void on_operator(GtkButton *btn, gpointer user_data) {
    calcstate  *state   = (calcstate *)user_data;
    const char *current = gtk_editable_get_text(GTK_EDITABLE(state->display));
    const char *label   = gtk_button_get_label(btn);
    char buff[128];

    double second = atof(state->current_number);
    if (state->pending_op != 0 ) {
        int    err    = 0;
        state->result = calc_evaluate(state->result, second, state->pending_op, &err);

   } else {
        state->result = second;
    }
    snprintf(buff, sizeof(buff), "%s %s ", current,label);
    gtk_editable_set_text(GTK_EDITABLE(state->display), buff);

    state->pending_op = label[0];
    state->current_number[0] = '\0';
    state->fresh_input = 1;
}
//for = 
static void on_equals(GtkButton *btn, gpointer user_data) {
    calcstate  *state   = (calcstate *)user_data;
    const char *current = gtk_editable_get_text(GTK_EDITABLE(state->display));
    double      second  = atof(state->current_number);
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
    state->current_number[0] = '\0';
    state->fresh_input = 1;
}

static void on_clear(GtkButton *btn , gpointer user_data){
    calcstate *state = (calcstate *)user_data;
    gtk_editable_set_text(GTK_EDITABLE(state->display), "0");

    state->pending_op = 0;
    state->result = 0;
    state->fresh_input = 1;
    state->current_number[0] = '\0';
}
static  void on_delete(GtkButton *btn , gpointer user_data){
    calcstate *state = (calcstate *)user_data;
    const char *current = gtk_editable_get_text(GTK_EDITABLE(state->display));
    int len  = strlen(current);
    if(len<=1){
        gtk_editable_set_text(GTK_EDITABLE(state->display), "0");
        state->fresh_input = 1;
    }else{
        char buff[128];
        strncpy(buff, current,len-1);
        buff[len-1] = '\0';
        gtk_editable_set_text(GTK_EDITABLE(state->display), buff);
    }
    
}
//connecting the signal with function above 
void signal_connect_all(GtkBuilder *builder, calcstate *state) {
    
    state->display = GTK_WIDGET(gtk_builder_get_object(builder, "display_entry"));

    
    const char *digits[] = {
        "btn_0", "btn_1", "btn_2", "btn_3", "btn_4", 
        "btn_5", "btn_6", "btn_7", "btn_8", "btn_9", "btn_dot"
    };
    for (int i = 0; i < 11; i++) {
        GObject *btn = gtk_builder_get_object(builder, digits[i]);
        if (btn) {
            g_signal_connect(btn, "clicked", G_CALLBACK(on_digit), state);
        }
    }

    
    const char *ops[] = {"btn_add", "btn_sub", "btn_mul", "btn_div", "btn_percentage", "btn_bracket"};
    for (int i = 0; i < 6; i++) {
        GObject *btn = gtk_builder_get_object(builder, ops[i]);
        if (btn) {
            g_signal_connect(btn, "clicked", G_CALLBACK(on_operator), state);
        }
    }

    g_signal_connect(gtk_builder_get_object(builder, "btn_clear"), "clicked", G_CALLBACK(on_clear), state);
    g_signal_connect(gtk_builder_get_object(builder, "btn_delete"), "clicked", G_CALLBACK(on_delete), state);
    g_signal_connect(gtk_builder_get_object(builder, "btn_eq"), "clicked", G_CALLBACK(on_equals), state);
}








