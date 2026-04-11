#include <stdio.h>
#include <math.h>
#include <string.h>
#include "../include/calc.h"

double calc_evaluate(double a, double b, char op,int *err){
    *err = 0;
    if(op == '+') return a+b;
    if(op == '-') return a-b;
    if(op == '*') return a*b;
    if(op == '/'){
        if(b==0){
            *err = 1;
            return 0;
        } return a/b;
    }
    if(op == '%')return a* (b/100.0);
    return 0;
}

