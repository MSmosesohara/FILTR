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
    WINDOW *win = panel_window(panel);
    if (!win) return;
    // Print line at the bottom, scroll up
    int maxy, maxx;
    getmaxyx(win, maxy, maxx);
    scroll(win);
    mvwprintw(win, maxy - 2, 0, "%.*s", maxx - 1, new_log);
    wrefresh(win);
}