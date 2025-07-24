#include "proc_reader.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    unsigned int sleep_time = 1; // Default sleep time of 1 second
    DATA_DISPLAY_MODE displayMode = DETAILED_DISPLAY_MODE; // Default display mode

    for (int i = 0 ; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == 's' && i + 1 < argc) {
            sleep_time = atoi(argv[++i]); // Get the sleep time from command line argument
            if (sleep_time <= 0) {
                perror("Invalid sleep time. Using default of 1 second.\n");
                sleep_time = 1; // Reset to default if invalid
            }

        }
        if (strcmp(argv[i], "-basic") == 0) {
            displayMode = BASIC_DISPLAY_MODE; // Set display mode to basic
        } else if (strcmp(argv[i], "-detailed") == 0) {
            displayMode = DETAILED_DISPLAY_MODE; // Set display mode to detailed
        } else if (strcmp(argv[i], "-raw") == 0) {
            displayMode = RAW_DISPLAY_MODE; // Set display mode to raw
        } else if (strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s [-s <seconds>] [-basic | -detailed | -raw]\n", argv[0]);
            return 0; // Exit after displaying help
        }else {
            printf("Usage: %s [-s <seconds>] [-basic | -detailed | -raw]\n", argv[0]);
            return 1; // Exit with error if invalid arguments
        }
    }

    while (1) {
        system("clear"); // Clear the terminal for better readability
        printProcesses(displayMode); // Call the function to print processes in the selected mode
        printf("Refreshing every %d seconds...\n", sleep_time);
        sleep(sleep_time); // Sleep for the specified duration before the next iteration
    }

    return 0;
}