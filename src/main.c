#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <panel.h>
#include "panel_left.h"
#include "panel_right.h"
#include "highlight.h"

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

    // Open the 'logs' command for the bottom panel (all logs)
    FILE *fp_all = popen("sudo tail -f /var/log/*.log /var/log/*/*.log", "r");
    if (fp_all == NULL) {
        perror("Failed to run logs command");
        cleanup_ncurses();
        return EXIT_FAILURE;
    }

    // For the top panel, we need to filter using grep with -v -e for each highlight
    char grep_command[BUFFER_SIZE * 4];
    char *exclude_cmd = get_grep_exclude_command();
    if (exclude_cmd && strlen(exclude_cmd) > 0) {
        snprintf(grep_command, sizeof(grep_command),
                 "sudo tail -f /var/log/*.log /var/log/*/*.log | grep %s", exclude_cmd);
    } else {
        snprintf(grep_command, sizeof(grep_command),
                 "sudo tail -f /var/log/*.log /var/log/*/*.log");
    }
    FILE *fp_filtered = popen(grep_command, "r");
    if (fp_filtered == NULL) {
        perror("Failed to run filtered logs command");
        pclose(fp_all);
        cleanup_ncurses();
        return EXIT_FAILURE;
    }
    free(exclude_cmd);

    // Buffers for the last line shown in the bottom panel
    char last_line[BUFFER_SIZE] = {0};

    while (1) {
        // Read from the bottom panel (all logs)
        if (fgets(buffer, sizeof(buffer), fp_all) != NULL) {
            strncpy(last_line, buffer, sizeof(last_line) - 1);
            last_line[sizeof(last_line) - 1] = '\0';
            update_right_panel(right_panel, buffer);
        }

        // Read from the top panel (filtered logs)
        if (fp_filtered && fgets(buffer, sizeof(buffer), fp_filtered) != NULL) {
            update_left_panel(left_panel, buffer);
        }

        // Refresh panels
        update_panels();
        doupdate();

        // Check for user input
        ch = getch();
        if (ch == 'q') {
            break; // Exit on 'q'
        } else if (ch == 'h') {
            // Highlight the last line shown in the bottom panel
            if (strlen(last_line) > 0) {
                // You may want to extract a pattern from last_line, here we use the whole line
                add_highlight(last_line);

                // Rebuild the filtered logs command
                if (fp_filtered) pclose(fp_filtered);
                exclude_cmd = get_grep_exclude_command();
                if (exclude_cmd && strlen(exclude_cmd) > 0) {
                    snprintf(grep_command, sizeof(grep_command),
                             "sudo tail -f /var/log/*.log /var/log/*/*.log | grep %s", exclude_cmd);
                } else {
                    snprintf(grep_command, sizeof(grep_command),
                             "sudo tail -f /var/log/*.log /var/log/*/*.log");
                }
                fp_filtered = popen(grep_command, "r");
                free(exclude_cmd);

                // Optionally clear the top panel here if you want
                // wclear(win_top); // Uncomment if needed
            }
        }

        // Sleep briefly to avoid busy loop
        usleep(10000);
    }

    if (fp_all) pclose(fp_all);
    if (fp_filtered) pclose(fp_filtered);
    cleanup_ncurses();
    printf("Exited. Press Enter to close.\n");
    getchar();
    return EXIT_SUCCESS;
}