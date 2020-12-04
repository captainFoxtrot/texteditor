#include "defs.h"
#include "userInterface.h"

char* centerText(int width, char* text){

    if(strlen(text) > width) return NULL;

    // Allocate memory for string
    char* output = malloc(width * sizeof(char));

    // Fill string with spaces
    for(int i = 0; i < width; i++){
        output[i] = ' ';
    }

    // Write the original text, centered
    strcpy(
        output + ((width - strlen(text)) / 2),
        text
    );

    // Return the centered text
    return output;
}

void window_init(window_t* w){
    w->width = COLS;
    w->height = LINES - 1;
    w->firstDisplayedLine = 0;
}

void window_display_titlebar(window_t* w, file_t* f, cursor_t* c, enum editMode_t* e){

    attron(COLOR_PAIR(2));

    for(int i = 0; i < w->width; i++){
        mvprintw(0, i, " ");
    }

    // Print file name
    mvprintw(0, 0, centerText(
        w->width,
        f->name
    ));

    // Print edit mode
    mvprintw(0, 0, centerText(w->width / 2, *e == INSERT ? "INSERT" : "OVERWRITE"));

    // Print cursor coordinates
    mvprintw(0, w->width * 3/4,
        "Ln %i/%i, Col %i",
        c->position.line + 1,
        f->lineCount,
        c->position.col + 1
    );

    // Reset color
    attron(COLOR_PAIR(1));
}

void window_display_file(window_t* w, file_t* f, cursor_t* c){

    char* lineToPrint;

    attron(COLOR_PAIR(1));

    lineToPrint = malloc(w->width * sizeof(char) + 1);

    // Insert null terminator at the end
    lineToPrint[w->width] = 0;

    if(lineToPrint){

        for(int i = 0; i < w->height; i++){

            int act_line = i + w->firstDisplayedLine;

            // Set buffer with spaces
            memset(lineToPrint, ' ', w->width);

            if(act_line < f->lineCount){

                // Copy the contents of this line into the buffer
                memcpy(
                    // Destination
                    lineToPrint,

                    // Source
                    f->contents + cursor_LINE2POS(act_line, f),

                    // Number of bytes to copy
                    MIN(w->width, file_charsInLine(f, act_line))
                );
            }

            // Print the contents of the line
            mvprintw(i + 1, 0, lineToPrint);
        }

    } else mvprintw(1, 0, f->contents + cursor_LINE2POS(w->firstDisplayedLine, f));

    // For each line `i` that will be displayed on the screen
    for(int i = 0; i < w->height; i++){
        
        if(w->firstDisplayedLine + i >= f->lineCount){

            for(int j = 0; j < w->width; j++){
                attron(COLOR_PAIR(1));
                mvprintw(i, j, " ");
            }

        } else {
            // For each column `j` in line `i`
            if(file_charsInLine(f, w->firstDisplayedLine + i) > w->width){
                attron(COLOR_PAIR(2));
                mvprintw(i + 1, w->width - 1, ">");
                attron(COLOR_PAIR(1));
            }
        }


    }
}

void window_updatescroll(window_t* w, file_t* f, cursor_t* cursor){

    #define currLine (cursor->position.line)

    // Check if the window needs to be scrolled up
    if(
        currLine < w->firstDisplayedLine
        || window_lastDisplayedLine(w) >= f->lineCount
    ){
        --(w->firstDisplayedLine);

    // ...or scrolled down?
    } else if(currLine > window_lastDisplayedLine(w)){
        ++(w->firstDisplayedLine);
    }

    #undef currLine
}

char* window_input(window_t* w, cursorpos_t location, size_t width){

    char* buffer;

    if(!(buffer = calloc(CHUNKSIZE, sizeof(char)))) return NULL;
    
    int ch;
    int cursor = 0;
    int allocatedChunks = 0;
    int allocatedLen = 0;

    while(ch != KEY_ENTER){

        mvprintw(location.col, location.line, buffer);

        switch(ch = getch()){

            case KEY_LEFT:
                if(cursor > 0) --cursor;
                break;

            case KEY_RIGHT:
                if(cursor <= allocatedLen) ++cursor;
                break;

            case KEY_ENTER:;
                /*
                Intentionally empty to prevent from being interpreted as newline
                A "nop" function of some sort needs to be called here so that gcc
                doesn't optimize this away.
                */
                char* volatile dummy;
                break;

            case KEY_BACKSPACE:
                memmove(
                    buffer + cursor + 1,  // Destination
                    buffer + cursor,      // Source
                    allocatedLen - cursor // Num bytes to move
                );
                buffer[allocatedLen] = 0;
                --allocatedLen;
                break;

            default:
                memmove(
                    buffer + cursor,      // Destination
                    buffer + cursor + 1,  // Source
                    allocatedLen - cursor // Num bytes to move
                );
                buffer[cursor] = ch;
                ++allocatedLen;
                ++cursor;
                break;
        }

        // If more memory is required
        if(allocatedLen > allocatedChunks * CHUNKSIZE - 16){

            ++allocatedChunks;
            if(!(buffer = realloc(buffer, allocatedChunks * CHUNKSIZE))){
                return NULL;
            }

        } else if(
            allocatedLen <= allocatedChunks * CHUNKSIZE - 16
            && allocatedChunks > 1
        ){
            --allocatedChunks;
            if(!(buffer = realloc(buffer, allocatedChunks * CHUNKSIZE))){
                return NULL;
            }
        }
    }

    return buffer;
}

char* window_inputbox(window_t* w, char* prompt){

    

}