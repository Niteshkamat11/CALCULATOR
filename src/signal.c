#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/signals.h"

//for all the digit 0-9
static void on_digit(GtkButton *btn, gpointer user_data) {

    engine_digit((calcstate *)user_data, gtk_button_get_label(btn));
 }

//for operator like +,-,/,%,*
static void on_operator(GtkButton *btn, gpointer user_data) {
    calcstate  *state   = (calcstate *)user_data;
    engine_operator(state , gtk_button_get_label(btn));

}
//for = 
static void on_equals(GtkButton *btn, gpointer user_data) {
    calcstate  *state   = (calcstate *)user_data;
    engine_equals(state);
 }
//for AC btn
static void on_clear(GtkButton *btn , gpointer user_data){
    calcstate *state = (calcstate *)user_data;
    engine_clear(state);
}
//for btn ⌫ 
static  void on_delete(GtkButton *btn , gpointer user_data){
    calcstate *state = (calcstate *)user_data;
    engine_delete(state);
}

//for bracket 
static void on_bracket(GtkButton *btn , gpointer user_data){
    calcstate *state = (calcstate *)user_data;
    engine_bracket(state );

}

//for keyboard input
static gboolean on_key_press(GtkEventControllerKey *controller,
                              guint keyval,
                              guint keycode,
                              GdkModifierType state,
                              gpointer user_data) {
    calcstate *s = (calcstate *)user_data;

    switch (keyval) {
        case GDK_KEY_0: case GDK_KEY_KP_0: engine_digit(s, "0"); return TRUE;
        case GDK_KEY_1: case GDK_KEY_KP_1: engine_digit(s, "1"); return TRUE;
        case GDK_KEY_2: case GDK_KEY_KP_2: engine_digit(s, "2"); return TRUE;
        case GDK_KEY_3: case GDK_KEY_KP_3: engine_digit(s, "3"); return TRUE;
        case GDK_KEY_4: case GDK_KEY_KP_4: engine_digit(s, "4"); return TRUE;
        case GDK_KEY_5: case GDK_KEY_KP_5: engine_digit(s, "5"); return TRUE;
        case GDK_KEY_6: case GDK_KEY_KP_6: engine_digit(s, "6"); return TRUE;
        case GDK_KEY_7: case GDK_KEY_KP_7: engine_digit(s, "7"); return TRUE;
        case GDK_KEY_8: case GDK_KEY_KP_8: engine_digit(s, "8"); return TRUE;
        case GDK_KEY_9: case GDK_KEY_KP_9: engine_digit(s, "9"); return TRUE;

        case GDK_KEY_period:
        case GDK_KEY_KP_Decimal:    engine_digit(s, ".");    return TRUE;

        case GDK_KEY_plus:
        case GDK_KEY_KP_Add:        engine_operator(s, "+"); return TRUE;

        case GDK_KEY_minus:
        case GDK_KEY_KP_Subtract:   engine_operator(s, "-"); return TRUE;

        case GDK_KEY_asterisk:
        case GDK_KEY_KP_Multiply:   engine_operator(s, "×"); return TRUE;

        case GDK_KEY_slash:
        case GDK_KEY_KP_Divide:     engine_operator(s, "÷"); return TRUE;

        case GDK_KEY_percent:       engine_operator(s, "%"); return TRUE;

        case GDK_KEY_Return:
        case GDK_KEY_KP_Enter:
        case GDK_KEY_equal:         engine_equals(s);        return TRUE;

        case GDK_KEY_BackSpace:     engine_delete(s);        return TRUE;
        case GDK_KEY_Escape:        engine_clear(s);         return TRUE;

        case GDK_KEY_parenleft:
        case GDK_KEY_parenright:    engine_bracket(s);       return TRUE;
    }
    return FALSE;
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

    
    const char *ops[] = {"btn_add", "btn_sub", "btn_mul", "btn_div", "btn_percentage"};
    for (int i = 0; i < 5; i++) {
        GObject *btn = gtk_builder_get_object(builder, ops[i]);
        if (btn) {
            g_signal_connect(btn, "clicked", G_CALLBACK(on_operator), state);
        }
    }

    //keyboard input 
    


    g_signal_connect(gtk_builder_get_object(builder, "btn_clear"), "clicked", G_CALLBACK(on_clear), state);
    g_signal_connect(gtk_builder_get_object(builder, "btn_delete"), "clicked", G_CALLBACK(on_delete), state);
    g_signal_connect(gtk_builder_get_object(builder, "btn_eq"), "clicked", G_CALLBACK(on_equals), state);
    g_signal_connect(gtk_builder_get_object(builder, "btn_bracket"), "clicked", G_CALLBACK(on_bracket), state);


    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    GtkEventControllerKey *key_ctrl = GTK_EVENT_CONTROLLER_KEY(gtk_event_controller_key_new());
    gtk_event_controller_set_propagation_phase(GTK_EVENT_CONTROLLER(key_ctrl), GTK_PHASE_CAPTURE);
    g_signal_connect(key_ctrl, "key-pressed", G_CALLBACK(on_key_press), state);
    gtk_widget_add_controller(window, GTK_EVENT_CONTROLLER(key_ctrl));
}

