#ifndef DEFS_H
#define DEFS_H

// Libraries
#include <memory.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <signal.h>
#include <math.h>

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

#define CHUNKSIZE 256

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef enum edit_type_t {
    EDIT_INSERT, EDIT_OVERWRITE, EDIT_DELETE
} edit_type_t;

typedef struct edit_t {
    edit_type_t edit;
    char character;
} edit_t;

#define EDIT_T_WIDTH sizeof(edit_t)
#define EDITSTACK_SP(e) (e->basePointer + e->numItems * EDIT_T_WIDTH)

enum editMode_t {
    INSERT, OVERWRITE
};

/* stack.c, stack.h */

typedef struct editstack_t {
    edit_t* basePointer;
    size_t numItems;
} editstack_t;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/* file.h, file.c */

typedef struct file_t {

    FILE* file;             // The actual file object
    bool open;              // Whether a file is open or not
    char* name;             // Name of the currently open or last open file
    char* contents;         // Contents of the file
    editstack_t edits;      // Edit history
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

    #define REQUIRED_CHUNKS(n) ((n / CHUNKSIZE) + 1)
    #define ALLOCATED_SIZE(n) (REQUIRED_CHUNKS(n) * CHUNKSIZE)

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
    int tabWidth;
} window_t;

#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Generic struct containing all of the info that the app and the various
// functions within need to run.
typedef struct app_info_t {

    file_t f;   // The currently open file as well as its contents and other metadata
    window_t w; // Data about the window such as scroll and size
    cursor_t c; // The position of the cursor 

} app_into_t;