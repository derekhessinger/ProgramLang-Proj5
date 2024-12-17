/**
 * Derek Hessinger
 * Prof. Ying Li
 * CS 333
 * 11/19/24
 * 
 * This program enters a while loop and supports keyboard interrupts
 * 
 * gcc -o task1 task1.c
 * ./task1
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

// signal handler
void sighandler(int signum) {
   printf("Interuppted!");
   exit(1);
}

int main(){
    // create signal 
    signal(SIGINT, sighandler);
    //while loop
    while(1){
    }
    return(0);
}
