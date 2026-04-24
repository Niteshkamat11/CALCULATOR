#!/bin/bash
while true;
do
    make 
   
    ./nit-calculator &



    inotifywait -e modify include/ src/ ui/ 



    pkill -9 nit-calculator 


done




