#ifndef CURSOR_H
#define CURSOR_H

#include "defs.h"

/* Moves the cursor in a given direction. */
void cursor_move(cursor_t* w, file_t* f, int dir);

/* Updates the on-screen cursor, but does not call refresh(). */
void cursor_update(cursor_t* c, window_t* w);

#endif