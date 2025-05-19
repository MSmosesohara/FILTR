#ifndef PANEL_LEFT_H
#define PANEL_LEFT_H

#include <panel.h>

void init_panel_left(void);
PANEL* create_left_panel(void);
void update_left_panel(PANEL *panel, const char *buffer);

#endif