#define _DEFAULT_SOURCE
#include <stdio.h>
#include <dirent.h>

typedef struct {
    int pid;
    char comm[256];
    char state;
} ProcessInfo_t;

int readprocinfo(ProcessInfo_t *pinfo, struct dirent *entry) {
    
    if (entry->d_type == DT_DIR && entry->d_name[0] >= '0' && entry->d_name[0] <= '9') {
        FILE *fp ;
        char path[256 + 12]; // 256 for command + 12 for /proc/ + pid + /stat
        snprintf(path, sizeof(path), "/proc/%s/stat", entry->d_name);
        fp = fopen(path, "r");
        if (fp != NULL) {
            fscanf(fp, "%d %s %c", &pinfo->pid, pinfo->comm, &pinfo->state);
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

int printprocesses() {
    DIR *dir = opendir("/proc");
    if (dir == NULL) {
        perror("Failed to open /proc directory");
        closedir(dir);
        return 1; // Error opening /proc directory
    }
    ProcessInfo_t pinfo;
    struct dirent *entry;
    while ( (entry = readdir(dir) ) != NULL) {
        if (readprocinfo(&pinfo, entry) != 0) {
            continue; // Skip if there was an error reading process info
        }
        printf("|%5d| %-18s | %c   |\n", pinfo.pid, pinfo.comm, pinfo.state);
        
    }

    closedir(dir);
    return 0;
}