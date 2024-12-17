/**
 * Derek Hessinger
 * Prof. Ying Li
 * CS 333
 * 11/19/24
 *
 * This program handles a floating point exception
 *
 * gcc -o task1b task1b.c
 * ./task1b
 */
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

// Signal handler for SIGFPE
void handle_sigfpe(int signal) {
    printf("Caught floating-point exception (SIGFPE). Signal code: %d\n", signal);
    exit(EXIT_FAILURE); // Exit the program after handling the signal
}

int main() {
    // Set up the SIGFPE handler
    if (signal(SIGFPE, handle_sigfpe) == SIG_ERR) {
        fprintf(stderr, "Error setting up signal handler for SIGFPE\n");
        return EXIT_FAILURE;
    }

    // Intentional integer division by zero
    printf("Attempting integer division by zero...\n");
    int numerator = 1;
    int denominator = 0;
    int result = 5 / 0; // This will raise SIGFPE

    // This line will not be executed if the exception is triggered
    printf("Result: %d\n", result);

    return EXIT_SUCCESS;
}
