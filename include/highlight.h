#ifndef HIGHLIGHT_H
#define HIGHLIGHT_H

#include <ncurses.h>

// Function to initialize highlighting
void init_highlight();

char *get_grep_exclude_command(void);


// Function to add a highlight to the list
void add_highlight(const char *highlight);

// Function to remove a highlight from the list
void remove_highlight(const char *highlight);

// Function to get the current list of highlights
const char **get_highlights(int *count);

// Function to clear all highlights
void clear_highlights();

// Function to update the grep command with current highlights
void update_grep_command(char *grep_command, size_t size);

#endif // HIGHLIGHT_H