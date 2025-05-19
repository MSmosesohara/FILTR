#ifndef UTILS_H
#define UTILS_H

#include <ncurses.h>

void init_ncurses();
void end_ncurses();
void read_logs(const char *command, void (*callback)(const char *));
void process_log_line(const char *line);

#endif // UTILS_H