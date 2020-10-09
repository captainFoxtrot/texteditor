#include "defs.h"

void cursor_update(cursor_t* c, window_t* w){
    move(c->position.line - w->firstDisplayedLine + 1, c->position.col);
}

void cursor_move(cursor_t* c, file_t* f, int dir){

    int temp;
    
    // The C preprocessor is awesome.
    #define col (c->position.col)
    #define line (c->position.line)
    #define __lineCount (f->lineCount)
    #define curr_linewidth (file_charsInLine(f, line))

    switch(dir){

        case KEY_UP:
            if(line > 0){
                --line;
                if(col >= (temp = curr_linewidth)){
                    col = temp;
                }
            }
            break;

        case KEY_DOWN:
            if(line < __lineCount - 1){
                ++line;
                temp = curr_linewidth;
                if(col >= temp){
                    mvprintw(0, 0, "test");
                    col = temp;
                }
            }
            break;

        case KEY_LEFT:
            if(col > 0){
                --col;
            } else if(line > 0){
                --line;
                col = curr_linewidth;
            }
            break;

        case KEY_RIGHT:
            if(col < curr_linewidth){
                ++col;
            } else if(line < __lineCount - 1){
                ++line;
                col = 0;
            }
            break;
    }

    #undef col
    #undef line
    #undef __lineCount
    #undef curr_numlines
}