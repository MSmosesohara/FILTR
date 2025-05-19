#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "panel_left.h"
#include "utils.h"
#include <panel.h>
#define MAX_LOG_LINES 100
#define LOG_LINE_LENGTH 256

static char *log_lines[MAX_LOG_LINES];
static int log_line_count = 0;

void init_panel_left() {
    for (int i = 0; i < MAX_LOG_LINES; i++) {
        log_lines[i] = malloc(LOG_LINE_LENGTH);
        if (!log_lines[i]) {
            // handle malloc failure
            for (int j = 0; j < i; j++) free(log_lines[j]);
            exit(1);
        }
        log_lines[i][0] = '\0';
    }
    log_line_count = 0;
}

void free_panel_left() {
    for (int i = 0; i < MAX_LOG_LINES; i++) {
        free(log_lines[i]);
    }
}


void display_panel_left(WINDOW *win) {
    werase(win);
    for (int i = 0; i < log_line_count; i++) {
        mvwprintw(win, i, 0, "%s", log_lines[i]);
    }
    wrefresh(win);
}

PANEL* create_left_panel() {
    WINDOW *win = newwin(20, 40, 0, 0); // Example size/position
    PANEL *panel = new_panel(win);
    return panel;
}

void update_left_panel(PANEL *panel, const char *new_log) {
    // Add the new log line to the buffer
    if (log_line_count < MAX_LOG_LINES) {
        strncpy(log_lines[log_line_count], new_log, LOG_LINE_LENGTH - 1);
        log_lines[log_line_count][LOG_LINE_LENGTH - 1] = '\0';
        log_line_count++;
    } else {
        // Shift up to make room for the new line
        for (int i = 1; i < MAX_LOG_LINES; i++) {
            strncpy(log_lines[i - 1], log_lines[i], LOG_LINE_LENGTH);
        }
        strncpy(log_lines[MAX_LOG_LINES - 1], new_log, LOG_LINE_LENGTH - 1);
        log_lines[MAX_LOG_LINES - 1][LOG_LINE_LENGTH - 1] = '\0';
    }

    // Draw the buffer in the panel's window
    WINDOW *win = panel_window(panel);
    werase(win);

    int max_y, max_x;
    getmaxyx(win, max_y, max_x);

    // Show only the last max_y lines
    int start = (log_line_count > max_y) ? (log_line_count - max_y) : 0;
    for (int i = 0; i < max_y && (start + i) < log_line_count; i++) {
        mvwprintw(win, i, 0, "%.*s", max_x - 1, log_lines[start + i]);
    }

    box(win, 0, 0); // Optional: draw a border
    wrefresh(win);
}