#include "highlight.h"
#include <string.h>
#include <stdlib.h>

#define MAX_HIGHLIGHTS 100

static char *highlights[MAX_HIGHLIGHTS];
static int highlight_count = 0;

void add_highlight(const char *highlight) {
    if (highlight_count < MAX_HIGHLIGHTS) {
        highlights[highlight_count] = strdup(highlight);
        highlight_count++;
    }
}

void remove_highlight(const char *highlight) {
    for (int i = 0; i < highlight_count; i++) {
        if (strcmp(highlights[i], highlight) == 0) {
            free(highlights[i]);
            highlights[i] = highlights[highlight_count - 1];
            highlights[highlight_count - 1] = NULL;
            highlight_count--;
            break;
        }
    }
}

char *get_grep_exclude_command() {
    if (highlight_count == 0) {
        return NULL;
    }

    size_t command_length = 0;
    for (int i = 0; i < highlight_count; i++) {
        command_length += strlen(highlights[i]) + 4; // for -e and quotes
    }

    char *command = malloc(command_length + 1);
    if (!command) {
        return NULL;
    }

    command[0] = '\0';
    for (int i = 0; i < highlight_count; i++) {
        strcat(command, " -v -e '");
        strcat(command, highlights[i]);
        strcat(command, "'");
    }

    return command;
}

void clear_highlights() {
    for (int i = 0; i < highlight_count; i++) {
        free(highlights[i]);
        highlights[i] = NULL;
    }
    highlight_count = 0;
}