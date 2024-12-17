/**
 * Derek Hessinger
 * Prof. Ying Li
 * CS 333
 * 11/19/24
 *
 * This program handles a segmentation fault
 *
 * gcc -o task1b task1b.c
 * ./task1b
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>

// signal handler
void sighandler(int signum) {
    printf("Error: accessing restricted memory!\n");
    exit(1);
}

int main() {
    // Set up signal handler
    signal(SIGSEGV, sighandler);

    int *ptr = NULL;
    // error here
    *ptr = 5;
    return 0;
}