#ifndef FILE_H
#define FILE_H

#include "defs.h"

#define READWRITE "r+"
#define WRITEUPDATE "w+"

#define NEWLINE '\n'

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

/*
Open a file.
The caller is responsible for checking that `f->open == false` before usage.
Returns `true` if successful, otherwise `false`.
Modified properties: f->file, f->open, f->name
*/
bool file_open(file_t* f, char* n);

/*
Close a file.
The caller is responsible for checking that `f->open == true` before usage.
`f->name` will not be cleared. As such, it will retain the name of the
last-open file even after `file_close` is called.
Returns `true` if successful, otherwise `false`.
Modified properties: f->open
*/
bool file_close(file_t* f);

/*
Read the contents of the currently opened file into `f->contents` and update 
`f->storedLen`.
Returns `true` if successful, otherwise `false`.
Modified properties: f->contents, f->storedLen, f->allocatedChunks
*/
bool file_readcontents(file_t* f);

/*
Write `f->contents` into the currently opened file.
The caller is responsible for checking that `f->open == true` before usage.
Returns `true` if successful, otherwise `false`.
Modified properties: f->file
*/
bool file_writecontents(file_t* f);

/*
Reload a file and read the new contents.
The caller is responsible for checking that `f->open == true` before usage.
Returns `true` if successful, otherwise `false`.
Modified properties: f->contents, f->storedLen, f->allocatedChunks
*/
inline bool file_reloadcontents(file_t* f){
    // TODO
}

/*
Get the length of the currently-opened file. This has no correlation with
f->storedLen.
Modified properties: None
*/
size_t file_act_len(file_t* f);

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/*
Operations on the stored contents of the file
None of these affect the actual file until file_writecontents() is used to save
the contents of `f->contents` to the file.
*/

/*
Overwrite the character at `f->contents[p]`.
Returns `false` if p is out of range (of `f->contents`), otherwise `true`.
Modified properties: f->contents, f->storedLen
*/
bool file_overwritechar(file_t* f, int p, int c);

/*
Insert a character at `f->contents[p]`, moving all characters to the right of it
one space to the right.
Returns `false` if p is out of range (of `f->contents`), otherwise `true`.
Modified properties: f->contents, f->storedLen, f->allocatedChunks
*/
bool file_insertchar(file_t* f, int p, int c);

/*
Delete the character at `f->contents[p]`, moving all characters to the right of
it one space to the left.
Returns `false` if p is out of range (of `f->contents`), otherwise `true`.
Modified properties: f->contents, f->storedLen, f->allocatedChunks
*/
bool file_deletechar(file_t* f, int p);

/*
Get the positions in `f->contents[p]` at which all the lines end; this includes
the imaginary -1st line which would end at position -1, as well as the last line,
which ends at `f->storedLen`. The two aforementioned will always be the first
and last items of the output array, respectively.

e.g., for the following file:

Index  0  1  2  3  4  5  6  7  8  9 10 11 12
Char   H  e  l  l  o \n  W  o  r  l  d \n  !

`f->lineEndPositions` would be redirected to point to the following array:
{-1, 5, 11, 13}

`false` is returned if memory allocation failed, otherwise `true`.
*/
bool file_update_lineend_positions(file_t* f);

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// Internal functions
/* i.e. should only be called by other functions in this file; do not use, you
   are likely to cause a segfault. You have been warned. */

bool file_extend_contentsbuffer(file_t* f);

bool file_shrink_contentsbuffer(file_t* f);

#endif