#include "userInterface.h"

void window_init(window_t* w){
    w->width = COLS;
    w->height = LINES - 1;
    w->firstDisplayedLine = 0;
}

void window_display_titlebar(window_t* w, file_t* f, cursor_t* c){

    move(0, 0);

    attron(COLOR_PAIR(2));

    for(int i = 0; i < COLS; i++){
        printw(" ");
    }

    mvprintw(0, (COLS - strlen(f->name)) / 2, f->name);

    mvprintw(0, COLS * 3 / 4, "(%i, %i)", c->position.line, c->position.col);

    attron(COLOR_PAIR(1));
}

void window_display_file(window_t* w, file_t* f, cursor_t* c){
    attron(COLOR_PAIR(1));
    mvprintw(1, 0, f->contents + cursor_LINE2POS(w->firstDisplayedLine, f));
}

void window_updatescroll(window_t* w, cursor_t* cursor){

    #define currLine (cursor->position.line)

    // Check if the window needs to be scrolled down
    if(currLine < w->firstDisplayedLine){
        --(w->firstDisplayedLine);

    // ...or scrolled up?
    } else if(currLine > window_lastDisplayedLine(w)){
        ++(w->firstDisplayedLine);
    }

    #undef currLine
}
