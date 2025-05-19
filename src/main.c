#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "panel_left.h"
#include "panel_right.h"
#include "utils.h"
#include <panel.h>

#define BUFFER_SIZE 1024

void initialize_ncurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
}

void cleanup_ncurses() {
    endwin();
}

int main() {
    char buffer[BUFFER_SIZE];
    FILE *fp;
    int ch;
    int rows, cols;

    initialize_ncurses();
    nodelay(stdscr, TRUE); // Make getch() non-blocking
    init_panel_left();
    init_panel_right();
    // Get terminal size
    getmaxyx(stdscr, rows, cols);

    // Calculate panel sizes
    int top_height = rows / 2;
    int bottom_height = rows - top_height;

    // Create windows for panels
    WINDOW *win_top = newwin(top_height, cols, 0, 0);
    WINDOW *win_bottom = newwin(bottom_height, cols, top_height, 0);

    // Create panels
    PANEL *left_panel = new_panel(win_top);
    PANEL *right_panel = new_panel(win_bottom);
    update_panels();
    doupdate();

    // Open the 'logs' command
    fp = popen("sudo tail -f /var/log/*.log /var/log/*/*.log", "r");
    if (fp == NULL) {
        perror("Failed to run logs command");
        cleanup_ncurses();
        return EXIT_FAILURE;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Update top panel with filtered logs
        update_left_panel(left_panel, buffer);

        // Update bottom panel for highlighting
        update_right_panel(right_panel, buffer);

        // Refresh panels
        update_panels();
        doupdate();

        // Check for user input
        ch = getch();
        if (ch == 'q') {
            break; // Exit on 'q'
        }
    }

    pclose(fp);
    cleanup_ncurses();
    printf("Exited. Press Enter to close.\n");
    getchar();
    return EXIT_SUCCESS;
}