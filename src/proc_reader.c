#include "proc_reader.h"

#ifndef _DEFAULT_SOURCE
    #define _DEFAULT_SOURCE 
#endif

#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>


DIR *openprocfolder(){
    DIR *dir = opendir("/proc");
    if (dir == NULL) {
        perror("Failed to open /proc directory");
        closedir(dir);
        exit(1); // Exit if unable to open /proc directory
    }

    return dir;
}


//Raw process info reading function
//This function reads the process information from /proc/[pid]/stat file
//and fills the ProcessInfo_t structure with the data.
int readprocinfo(struct dirent *entry, ProcessInfo_t *pinfo) {

    //check if the entry is a directory and its name starts with a digit (indicating a process)
    if (entry->d_type == DT_DIR && entry->d_name[0] >= '0' && entry->d_name[0] <= '9') {
        char path[256 + 12]; // 256 for command + 12 for /proc/ + pid + /stat
        snprintf(path, sizeof(path), "/proc/%s/stat", entry->d_name);
        
        FILE *fp = fopen(path, "r");
        if (fp != NULL) {
            fscanf(fp, "%u %s %c %d %d %d %d %d %u %*s %*s %*s %*s %lu %lu %*s %*s %d %d %d %*s %lu %lu %ld",
                &pinfo->pid, pinfo->comm, &pinfo->state, &pinfo->ppid,&pinfo->pgrp,
                &pinfo->session, &pinfo->tty_nr, &pinfo->tpgid,&pinfo->flags,
                &pinfo->utime, &pinfo->stime, &pinfo->priority, &pinfo->nice, &pinfo->num_threads, &pinfo->starttime, &pinfo->vsize, &pinfo->rss
            );

            fclose(fp);

        }
        else {
            perror("Failed to open process stat file");
            return 1; // Error opening process stat file
        }
    }else {
        return 1; // Not a valid process directory
    }
    return 0; // Successfully read process info
}

void printprocesses_raw(DIR *dir) {
    ProcessInfo_t pinfo;
    struct dirent *entry;

    printf("| PID |      Command       | State | PPID | PGRP | SESS | TTY_NR | TPGID | FLAGS   | UTIME | STIME |PRIORITY| NICE |THREADS| START    |     VSIZE    | RSS    |\n");
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    while ( (entry = readdir(dir) ) != NULL) {
        if (readprocinfo(entry, &pinfo) != 0) {
            continue; // Skip if there was an error reading process info
        }
        printf("|%-5d| %-18s |  %c  | %-5d | %-5d | %-5d | %-5d | %-5d | %-5d | %-5lu | %-5lu | %-5d | %-5d | %-5d | %-8lu | %-12lu | %-6lu |\n",
               pinfo.pid, pinfo.comm, pinfo.state, pinfo.ppid,
               pinfo.pgrp, pinfo.session, pinfo.tty_nr, pinfo.tpgid, pinfo.flags, pinfo.utime,
               pinfo.stime, pinfo.priority, pinfo.nice, pinfo.num_threads, pinfo.starttime, pinfo.vsize, pinfo.rss
        );
    }
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

}

void printprocesses_basic(DIR *dir) {
    ProcessInfo_t pinfo;
    struct dirent *entry;
    while ( (entry = readdir(dir) ) != NULL) {
        if (readprocinfo(entry, &pinfo) != 0) {
            continue; // Skip if there was an error reading process info
        }
        printf("PID: %-5d| Command: %-18s | State:", pinfo.pid, pinfo.comm);
        switch (pinfo.state) {
            case 'R':
                printf(" (Running)\n");
                break;
            case 'S':
                printf(" (Sleeping)\n");
                break;
            case 'D':
                printf(" (Uninterruptible sleep)\n");
                break;
            case 'Z':
                printf(" (Zombie)\n");
                break;
            case 'T':
                printf(" (Stopped)\n");
                break;
            case 't':
                printf(" (Tracing stop)\n");
                break;
            case 'X':
                printf(" (Dead)\n");
                break;
            case 'x':
                printf(" (Dead)");
                break;
            case 'K':
                printf(" (Wakekill)\n");
                break;
            case 'W':
                printf(" (Waking)\n");
                break;
            case 'P':
                printf(" (Parked)\n");
                break;
            default:
                printf(" (Unknown state)\n");
        }

       
        printf("-------------------------------------------------------------------------------------------\n");
    }
}

void printprocesses_detailed(DIR *dir) {
    ProcessInfo_t pinfo;
    struct dirent *entry;
    printf("| PID |      Command       |   State   |   User   | Memory Usage | CPU Time |\n");
    printf("-----------------------------------------------------------------------------\n");

    int clock_ticks_per_second = sysconf(_SC_CLK_TCK); // Get the number of clock ticks per second

    while ( (entry = readdir(dir) ) != NULL) {
        if (readprocinfo(entry, &pinfo) != 0) {
            continue; // Skip if there was an error reading process info
        }
        printf("|%-5d| %-18s |", pinfo.pid, pinfo.comm);
        switch (pinfo.state) {
            case 'R':
                printf(" Running  ");
                break;
            case 'S':
                printf(" Sleeping ");
                break;
            case 'D':
                printf(" Uninterruptible sleep");
                break;
            case 'Z':
                printf(" Zombie");
                break;
            case 'T':
                printf(" Stopped");
                break;
            case 't':
                printf(" Tracing stop");
                break;
            case 'X':
                printf(" Dead");
                break;
            case 'x':
                printf(" Dead");
                break;
            case 'K':
                printf(" Wakekill");
                break;
            case 'W':
                printf(" Waking");
                break;
            case 'P':
                printf(" Parked");
                break;
            default:
                printf(" Unknown state");
        }

        

        FILE *fp;
        char path[256 + 12]; // 256 for command + 12 for /
        snprintf(path, sizeof(path), "/proc/%d/status", pinfo.pid);
        fp = fopen(path, "r");
        if (fp != NULL) {
            char line[256];
            while (fgets(line, sizeof(line), fp) != NULL) {
                if (strncmp(line, "Uid:", 4) == 0) {
                    sscanf(line, "Uid:\t%u", &pinfo.uid);
                    break;
                }
            }
            fclose(fp);
        }
        struct passwd *pw = getpwuid(pinfo.uid);
        if (pw != NULL) {
            printf(" | %-8s", pw->pw_name); // Print the username
        } else {
            printf(" | Unknown UID: %d", pinfo.uid); // If UID is not found
        }


        printf(" | ");
        if (pinfo.rss > 1024 * 1024 * 1024) {
            printf("   %-6.2f GB", pinfo.rss / (1024.0 * 1024.0 * 1024.0));
        } else if (pinfo.rss > 1024 * 1024) {
            printf("   %-6.2f MB", pinfo.rss / (1024.0 * 1024.0));
        } else if (pinfo.rss > 1024) {
            printf("   %-6.2f KB", pinfo.rss / 1024.0);
        } else {
            printf("   %-4lu B   ", pinfo.rss);
        }


        printf(" | %-6.2lf s |\n", (pinfo.utime + pinfo.stime) / (double)clock_ticks_per_second); // CPU time in seconds
    }   
        
    printf("-----------------------------------------------------------------------------\n");

}

//Main function to print processes based on the display mode
void printProcesses(DATA_DISPLAY_MODE displayMode){
    DIR *dir = openprocfolder();
    switch (displayMode) {
        case BASIC_DISPLAY_MODE:
            printprocesses_basic(dir);
            break;
        case DETAILED_DISPLAY_MODE:
            printprocesses_detailed(dir);
            break;
        case RAW_DISPLAY_MODE:
            printprocesses_raw(dir);
            break;
        default:
            printf("Unknown display mode.\n");
            break;
    }
    closedir(dir); // Close the directory after processing
} 