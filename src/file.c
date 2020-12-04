#include "file.h"

bool file_open(file_t* f, char n[]){

    FILE* newfile;

    if((newfile = fopen(n, READWRITE))){

        // Success

        f->file = newfile;
        f->name = n;
        f->open = true;

        return true;

    } else return false;
}

bool file_close(file_t* f){
    if(fclose(f->file) == EOF) return false;
    else return true;
}

bool file_readcontents(file_t* f){

    char* newptr;
    size_t len = file_act_len(f);
    size_t allocatedSize = ALLOCATED_SIZE(len);

    if(!(
        // Allocate memory for the contents
        newptr = realloc(f->contents,
            // len rounded up to the next highest multiple of CHUNKSIZE
            allocatedSize
            // times the width of a char
            * sizeof(char)
        )
    )){
        // Failed? Return false
        return false;
    }

    // Clear the contents
    memset(newptr, 0, allocatedSize);

    // Try reading the contents
    if(fread(newptr, sizeof(char), len, f->file) < len){
        free(newptr);
        return false;
    }

    // Allocation successful
    f->contents = newptr;

    f->storedLen = len;

    f->allocatedChunks = REQUIRED_CHUNKS(len);

    return true;
}

bool file_writecontents(file_t* f){

    FILE* newfile;

    if(!(newfile = freopen(NULL, WRITEUPDATE, f->file))){
        return false;
    }

    f->file = newfile;

    if(fwrite(

        f->contents,
        sizeof(char),
        f->storedLen,
        f->file

    ) < f->storedLen){ return false; }

    if(!(newfile = freopen(NULL, READWRITE, f->file))){
        return false;
    }

    return true;
}

size_t file_act_len(file_t* f){
    size_t pos;
    fseek(f->file, 0, SEEK_END); // Seek the end of the file
    pos = ftell(f->file);        // Get current position
    rewind(f->file);             // Rewind to the file
    return pos;                  // Return the position
}

////////////////////////////////////////////////////////////////////////////////

bool file_overwritechar(file_t* f, int p, int c){

    // Return false if p is out of range
    if(p < 0 || p > f->storedLen) return false;

    // Overwrite the character
    (f->contents)[p] = (char) c;

    return true;
}

bool file_insertchar(file_t* f, int p, int c){

    // Return false if p is out of range
    if(p < 0 || p > f->storedLen) return false;

    // Extend the buffer if we need more memory
    if((f->storedLen) > (f->allocatedChunks * CHUNKSIZE) - 16){
        file_extend_contentsbuffer(f);
    }

    // Move chars right one space
    memmove(f->contents + p + 1, f->contents + p, f->storedLen - p);

    // Try overwriting the char
    if(!file_overwritechar(f, p, c)){

        // Failed?
        // Shift everything back
        memmove(f->contents + p, f->contents + p + 1, f->storedLen - p);

        return false;
    }

    // Increment the length of the buffer
    ++(f->storedLen);

    return true;
}

bool file_deletechar(file_t* f, int p){

    // Return false if p is out of range
    if(p < 0 || p > f->storedLen) return false;

    bool bufferShrinked = false;
    char originalChar;

    // Shrink the buffer if needed
    if(
        f->storedLen <= ((f->allocatedChunks - 1) * CHUNKSIZE * sizeof(char)) - 16
        && f->allocatedChunks > 1
    ){
        file_shrink_contentsbuffer(f);

        bufferShrinked = true;
    }

    originalChar = (f->contents)[p];

    // Move chars left one space
    memmove(f->contents + p - 1, f->contents + p, f->storedLen - p);

    // Try clearing the right-most char
    if(!file_overwritechar(f, f->storedLen - 1, 0)){

        // Failed?

        // Shift everything back
        memmove(f->contents + p, f->contents + p - 1, f->storedLen - p);

        // Re-write the char that was deleted
        (f->contents)[p] = originalChar;

        // Re-extend buffer if needed
        if(bufferShrinked) file_extend_contentsbuffer(f);

        return false;
    }

    // Decrement the stored length
    --(f->storedLen);

    return true;
}

bool file_update_lineend_positions(file_t* f){

    int* newptr;
    int currLine = 0;
    int currAllocatedChunks = 0;

    // Allocate a CHUNKSIZE to start
    if(!(newptr = malloc(CHUNKSIZE * sizeof(int)))){
        return false;
    } else ++currAllocatedChunks;

    // First line is always -1
    newptr[currLine] = -1;

    // For each character
    for(int i = 0; i <= f->storedLen; i++){

        // If character = newline or char after last character, then
        if(((f->contents)[i] == NEWLINE) || (i == f->storedLen)){

            // Increment current line
            ++currLine;

            // If we need more memory, then
            if(currLine >= (currAllocatedChunks * CHUNKSIZE)){
                // Try to allocate another CHUNKSIZE of memory
                if(!(newptr = realloc(newptr, (currAllocatedChunks + 1) * CHUNKSIZE * sizeof(int)))){
                    free(newptr);
                    return false;
                }
            }

            // Record the newline
            newptr[currLine] = i;
        }
    }

    if(!(newptr = realloc(newptr, (currLine + 1) * sizeof(int)))){
        free(newptr);
        return false;
    }

    f->lineEndPositions = newptr;
    f->lineCount = currLine;

    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Internal functions

bool file_extend_contentsbuffer(file_t* f){

    char* newptr;

    // Increment the number of allocated chunks
    ++(f->allocatedChunks);

    // Attempt to reallocate the memory
    if(!(newptr = realloc(f->contents,
        f->allocatedChunks * CHUNKSIZE
        * sizeof(char)
    ))){
        // Failed?

        // Decrement # allocated chunks
        --(f->allocatedChunks);

        return false;
    }

    // Store the new pointer to the contents
    f->contents = newptr;

    return true;
}

bool file_shrink_contentsbuffer(file_t* f){

    char* newptr;

    // Decrement the number of allocated chunks
    --(f->allocatedChunks);

    // Attempt to reallocate the memory
    if(!(newptr = realloc(f->contents,
        f->allocatedChunks * CHUNKSIZE
        * sizeof(char)
    ))){
        // Failed?

        // Decrement # allocated chunks
        ++(f->allocatedChunks);

        return false;
    }

    // Store the new pointer to the contents
    f->contents = newptr;

    return true;
}
