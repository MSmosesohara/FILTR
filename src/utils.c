#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

#define BUFFER_SIZE 1024

void read_logs(const char *command, void (*callback)(const char *)) {
    static char buffer[BUFFER_SIZE];
    if (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        // process buffer or call callback
    }
    // No return statement needed
}

void process_logs(const char* command, char* output) {
    FILE* fp;
    char path[BUFFER_SIZE];

    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Failed to run command");
        return;
    }

    while (fgets(path, sizeof(path), fp) != NULL) {
        strcat(output, path);
    }

    pclose(fp);
}

void clear_output(char* output) {
    memset(output, 0, BUFFER_SIZE);
}