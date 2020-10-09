#ifndef DEFS_H
#define DEFS_H

// Libraries
#include <memory.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/* file.h, file.c */
typedef struct file_t {

    FILE* file;             // The actual file object
    bool open;              // Whether a file is open or not
    char* name;             // Name of the currently open or last open file
    char* contents;         // Contents of the file
    size_t storedLen;       // Length of `contents`
    size_t lineCount;       // Number of lines in `contents`
    size_t allocatedChunks; // Number of CHUNKSIZE's allocated for `contents`
    int* lineEndPositions;  // Where each line in `contents` ends, see docs
                            // below for file_update_lineend_positions()
    /*
    Clarifications:
    1 - When `file_readcontents` is called, the contents of the file is loaded
        into `contents`, but edits to `contents` can be made after that without
        affecting the actual file. Call `file_writecontents` to save your edits.
    2 - Do not change the value of any of the properties in the struct directly;
        use the functions below as needed (except for the ones under "Internal
        functions".) 
    */

   /* Get the number of characters in file `f`, line `n`. */
   #define file_charsInLine(f, n) ((f->lineEndPositions)[n + 1] - (f->lineEndPositions)[n] - 1)

} file_t;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/* cursor.h, cursor.c */

typedef struct cursorpos_t {
    int line;
    int col;
} cursorpos_t;

typedef struct cursor_t {
    cursorpos_t position;
} cursor_t;

#define cursor_LINE2POS(n, f) (((f)->lineEndPositions)[n] + 1)
#define cursor_COORD2POS(c, f) (((f)->lineEndPositions)[(c)->position.line] + (c)->position.col + 1)

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/* userInterface.h, userInterface.c */

typedef struct window_t {
    int width;
    int height;
    int firstDisplayedLine;
} window_t;

#endif