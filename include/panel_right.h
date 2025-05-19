#ifndef PANEL_RIGHT_H
#define PANEL_RIGHT_H

#include <panel.h>

void init_panel_right(void);
PANEL* create_right_panel(void);
void update_right_panel(PANEL *panel, const char *buffer);

#endif