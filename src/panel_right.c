#include <ncurses.h>
#include <panel.h>
#include <stdlib.h>
#include <string.h>
#include "panel_right.h"
#include "highlight.h"

#define MAX_HIGHLIGHTS 100
#define MAX_LINE_LENGTH 256

#define MAX_LOG_LINES 100
#define LOG_LINE_LENGTH 256

static char *log_lines[MAX_LOG_LINES];
static int log_line_count = 0;


#define RIGHT_PANEL_HEIGHT 20
#define RIGHT_PANEL_WIDTH  40
#define RIGHT_PANEL_STARTY 0
#define RIGHT_PANEL_STARTX 41 // Place to the right of the left panel

static char highlights[MAX_HIGHLIGHTS][MAX_LINE_LENGTH];
static int highlight_count = 0;


void init_panel_right() {
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

void display_highlights(WINDOW *win) {
    werase(win);
    for (int i = 0; i < highlight_count; i++) {
        mvwprintw(win, i, 0, highlights[i]);
    }
    wrefresh(win);
}

void highlight_input(WINDOW *win) {
    char input[MAX_LINE_LENGTH];
    mvwprintw(win, highlight_count, 0, "Enter highlight (or 'done' to finish): ");
    wrefresh(win);
    wgetnstr(win, input, MAX_LINE_LENGTH - 1);

    if (strcmp(input, "done") == 0) {
        return;
    } else {
        add_highlight(input);
        highlight_input(win);
    }
}

PANEL* create_right_panel() {
    WINDOW *win = newwin(RIGHT_PANEL_HEIGHT, RIGHT_PANEL_WIDTH, RIGHT_PANEL_STARTY, RIGHT_PANEL_STARTX);
    PANEL *panel = new_panel(win);
    return panel;
}

void update_right_panel(PANEL *panel, const char *new_log) {
    WINDOW *win = panel_window(panel);
    if (!win) return;
    // Print line at the bottom, scroll up
    int maxy, maxx;
    getmaxyx(win, maxy, maxx);
    scroll(win);
    mvwprintw(win, maxy - 2, 0, "%.*s", maxx - 1, new_log);
    wrefresh(win);
}