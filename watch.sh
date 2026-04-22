#!/bin/bash
while true;
do
    make 
   
    ./nit-calculator &

    PID=$!

    inotifywait -e modify include/ src/ ui/ 

    kill PID

done




