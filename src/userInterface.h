#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "defs.h"

#define window_lastDisplayedLine(w) (w->firstDisplayedLine + w->height) - 1

void window_init(window_t* w);

void window_display_titlebar(window_t* w, file_t* f, cursor_t* c);

void window_display_file(window_t* w, file_t* f, cursor_t* c);

void window_refresh(window_t* w, file_t* f);

void window_updatescroll(window_t* w, cursor_t* c);

#endif