#include "proc_reader.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    unsigned int sleep_time = 1; // Default sleep time of 1 second
    for (int i = 0 ; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == 's' && i + 1 < argc) {
            sleep_time = atoi(argv[++i]); // Get the sleep time from command line argument
            if (sleep_time <= 0) {
                perror("Invalid sleep time. Using default of 1 second.\n");
                sleep_time = 1; // Reset to default if invalid
            }

        }
    }
    while (1) {
        system("clear"); // Clear the terminal for better readability
        printf("| PID |      Command      | State|\n");
        printf("----------------------------------\n");
        if (printprocesses() != 0) {
            return 1; // Exit if there was an error printing process info
        }
        printf("----------------------------------\n");
        printf("Refreshing every %d seconds...\n", sleep_time);
        sleep(sleep_time); // Sleep for the specified duration before the next iteration
    }

    return 0;
}