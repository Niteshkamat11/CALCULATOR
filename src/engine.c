#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/engine.h"

void engine_digit(calcstate *state , const char *label){
    const char *current = gtk_editable_get_text(GTK_EDITABLE(state->display));
    char  buff[128];
    if((strcmp(label,".")==0) && strchr(state->current_number,'.') != NULL){
        return;
    } 
    if(state->fresh_equal){
        current = "";
        state->fresh_equal = 0;
    }
    if (state->fresh_input) {
        state->current_number[0]= '\0';
        state->fresh_input = 0;
        // if (strcmp(current, "0") == 0) {
        //     current = "";
        // }
    }
    strcat(state->current_number,label);
    snprintf(buff, sizeof(buff), "%s%s", current, label);
    gtk_editable_set_text(GTK_EDITABLE(state->display), buff);


}

void engine_operator(calcstate *state , const char *label){
    const char *current = gtk_editable_get_text(GTK_EDITABLE(state->display));
    char buff[128];

    double second = atof(state->current_number);
    if (state->pending_op != 0 ) {
        int    err    = 0;
        state->result = calc_evaluate(state->result, second, state->pending_op, &err);

   } else if(state->current_number[0] != '\0') {
        state->result = second;
    }
    snprintf(buff, sizeof(buff), "%s %s ", current,label);
    gtk_editable_set_text(GTK_EDITABLE(state->display), buff);

    if      (strcmp(label, "+") == 0) state->pending_op = '+';
    else if (strcmp(label, "-") == 0) state->pending_op = '-';
    else if (strcmp(label, "×") == 0) state->pending_op = '*';
    else if (strcmp(label, "÷") == 0) state->pending_op = '/';
    else if (strcmp(label, "%") == 0) state->pending_op = '%';

    state->current_number[0] = '\0';
    state->fresh_input = 1;
}

void engine_equals(calcstate *state) {

    if (state->pending_op == 0) return;

    double second  = atof(state->current_number);
    int err = 0;

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
    state->fresh_equal = 1;      
}
void engine_clear(calcstate *state){
    gtk_editable_set_text(GTK_EDITABLE(state->display), "");

    state->pending_op = 0;
    state->result = 0;
    state->fresh_input = 1;
    state->current_number[0] = '\0';
    state->open_bracket=0;

}
void engine_delete(calcstate *state){
    const char *current = gtk_editable_get_text(GTK_EDITABLE(state->display));
    int len  = strlen(current); 
    char buff[128]; // added here 
    if(len<=1){
        gtk_editable_set_text(GTK_EDITABLE(state->display), "");
        state->fresh_input = 1;
        state->current_number[0]='\0';
        state->pending_op=0;
        return;
    }
    int to_remove = 1;
    char last_char = current[len-1];
    if(current[len-1]==' ' && len>=3){
        to_remove = 3;
        state->pending_op = 0;
    }else if (last_char == '('){
        state->open_bracket--;
    }else if (last_char == ')'){
        state->open_bracket++;
    }else{
        int cur_num_len = strlen(state->current_number);
        if(cur_num_len > 0){
            state->current_number[cur_num_len -1] = '\0';
        }
    }
    int new_len = len - to_remove;
    if(new_len<0) new_len = 0;
    strncpy(buff, current,new_len);
    buff[new_len] = '\0';
    if (new_len == 0 || strcmp(buff, " ") == 0) {
        gtk_editable_set_text(GTK_EDITABLE(state->display), "0");
        state->current_number[0] = '\0';
    } else {
        gtk_editable_set_text(GTK_EDITABLE(state->display), buff);
    }

}

void engine_bracket(calcstate *state){
   const char *current = gtk_editable_get_text(GTK_EDITABLE(state->display));
    char buff[128];
    if(strcmp(current , "0")==0){
        current = "";
    }
    if(state->open_bracket==0) {
        snprintf(buff, sizeof(buff), "%s(", current);
        state->open_bracket++;
    }
    else{
        snprintf(buff, sizeof(buff), "%s)", current);
        state->open_bracket--;
    }
    gtk_editable_set_text(GTK_EDITABLE(state->display), buff);

}
