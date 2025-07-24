typedef struct {
    //stat filex
    unsigned int pid;//1 <--positions in /proc/[pid]/stat
    char comm[256];//2
    char state;//3
    int ppid;//4
    int pgrp;//5
    int session;//6
    int tty_nr;//7
    int tpgid;//8
    unsigned int flags;//9
    unsigned long utime;//14
    unsigned long stime;//15
    int priority;//18
    int nice;//19
    int num_threads;//20
    unsigned long starttime;//22
    unsigned long vsize;//23
    long rss;//24
    //status file
    unsigned int uid; // Added UID field
} ProcessInfo_t;

typedef enum{
    BASIC_DISPLAY_MODE = 0,
    DETAILED_DISPLAY_MODE = 1,
    RAW_DISPLAY_MODE = 2
} DATA_DISPLAY_MODE;

void printProcesses(DATA_DISPLAY_MODE displayMode);