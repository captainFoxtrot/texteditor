#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "defs.h"

#define window_lastDisplayedLine(w) ((w->firstDisplayedLine + w->height) - 1)

// Gets the number of characters that a positive integer converted to a string will take up.
#define widthLineNumAsInt(n) ((int)(log10(n) + 1))

#define centeredTextStart(s, n) ((n - strlen(s)) / 2)

/*
Returns `text` centered in a string of `width` spaces.
Returns `NULL` if `strlen(text) > width` or memory allocation failed.
*/
char* centerText(int width, char* text);

/* Initializes the window. */
void window_init(window_t* w);

/* Displays the title bar with appropriate highlighting. */
void window_display_titlebar(window_t* w, file_t* f, cursor_t* c, enum editMode_t* e);

/* Displays the contents of a file starting from the first line. */
void window_display_file(window_t* w, file_t* f, cursor_t* c);

/* */
void window_updatescroll(window_t* w, file_t* f, cursor_t* c);

/* */
void window_dispmsg(window_t* w, char* msg);

/* */
char* window_input(window_t* w, cursorpos_t location, size_t width);

/* */
char* window_inputbox(window_t* w, char* prompt);

#endif