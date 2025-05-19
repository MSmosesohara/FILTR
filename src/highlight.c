#include "highlight.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_HIGHLIGHTS 128
#define MAX_PATTERN_LEN 256

static char highlights[MAX_HIGHLIGHTS][MAX_PATTERN_LEN];
static int highlight_count = 0;

void init_highlight() {
    highlight_count = 0;
}

void add_highlight(const char *highlight) {
    if (highlight_count < MAX_HIGHLIGHTS && strlen(highlight) < MAX_PATTERN_LEN) {
        strncpy(highlights[highlight_count], highlight, MAX_PATTERN_LEN - 1);
        highlights[highlight_count][MAX_PATTERN_LEN - 1] = '\0';
        highlight_count++;
    }
}

void remove_highlight(const char *highlight) {
    for (int i = 0; i < highlight_count; ++i) {
        if (strcmp(highlights[i], highlight) == 0) {
            for (int j = i; j < highlight_count - 1; ++j) {
                strcpy(highlights[j], highlights[j + 1]);
            }
            highlight_count--;
            break;
        }
    }
}

const char **get_highlights(int *count) {
    *count = highlight_count;
    static const char *ptrs[MAX_HIGHLIGHTS];
    for (int i = 0; i < highlight_count; ++i) {
        ptrs[i] = highlights[i];
    }
    return ptrs;
}

void clear_highlights() {
    highlight_count = 0;
}

char *get_grep_exclude_command(void) {
    // Returns a malloc'd string, caller must free
    if (highlight_count == 0) {
        return strdup("");
    }
    size_t total = 0;
    for (int i = 0; i < highlight_count; ++i) {
        total += strlen(highlights[i]) + 10;
    }
    char *cmd = malloc(total + 1);
    if (!cmd) return NULL;
    cmd[0] = '\0';
    for (int i = 0; i < highlight_count; ++i) {
        strcat(cmd, "-v -e '");
        strcat(cmd, highlights[i]);
        strcat(cmd, "' ");
    }
    return cmd;
}

void update_grep_command(char *grep_command, size_t size) {
    char *exclude_cmd = get_grep_exclude_command();
    snprintf(grep_command, size, "sudo tail -f /var/log/*.log /var/log/*/*.log | grep %s", exclude_cmd);
    free(exclude_cmd);
}