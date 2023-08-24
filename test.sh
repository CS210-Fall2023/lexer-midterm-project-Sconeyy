#!/bin/bash
for file in files/*.cci
do
    ./lexer $file
    echo "${file}.ANS vs. ${file}.lexer"
    #diff ${file}.ANS  ${file}.lexer
    if [[ $(diff ${file}.ANS  ${file}.lexer) ]]; then #if output of diff command is not zero
        echo "Somthing went wrong"
        diff ${file}.ANS  ${file}.lexer
        echo
    else
       echo "You passed the test" 
       echo
    fi
done

for file in files/*.ccx
do
    ./lexer $file
    echo "${file}.ANS vs. ${file}.lexer"
    #diff ${file}.ANS  ${file}.lexer
    if [[ $(diff ${file}.ANS  ${file}.lexer) ]]; then #if output of diff command is not zero
        echo "Somthing went wrong"
        diff ${file}.ANS  ${file}.lexer
        echo
    else
       echo "You passed the test" 
       echo
    fi
done