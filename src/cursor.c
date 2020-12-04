#include "defs.h"

void cursor_update(cursor_t* c, window_t* w){
    move(c->position.line - w->firstDisplayedLine + 1, c->position.col);
}

void cursor_move(cursor_t* c, file_t* f, int dir){

    int temp;
    
    // The C preprocessor is awesome.
    #define cursor_move_col (c->position.col)
    #define line (c->position.line)
    #define __lineCount (f->lineCount)
    #define curr_linewidth (file_charsInLine(f, line))

    switch(dir){

        case KEY_UP:
            if(line > 0){
                --line;
                temp = curr_linewidth;
                if(cursor_move_col >= temp)
                    cursor_move_col = temp;
            }
            break;

        case KEY_DOWN:
            if(line < __lineCount - 1){
                ++line;
                temp = curr_linewidth;
                if(cursor_move_col >= temp) cursor_move_col = temp;
            }
            break;

        case KEY_LEFT:
            if(cursor_move_col > 0){
                --cursor_move_col;
            } else if(line > 0){
                --line;
                cursor_move_col = curr_linewidth;
            }
            break;

        case KEY_RIGHT:
            if(cursor_move_col < curr_linewidth){
                ++cursor_move_col;
            } else if(line < __lineCount - 1){
                ++line;
                cursor_move_col = 0;
            }
            break;
    }

    #undef col
    #undef line
    #undef __lineCount
}