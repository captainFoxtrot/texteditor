#include "defs.h"
#include "cursor.h"
#include "userInterface.h"
#include "file.h"

file_t mainFile;
cursor_t cursor;
window_t w;

char* file_DEFAULTNAME = "Untitled";

void free_allocated(void){
    free(mainFile.contents);
    free(mainFile.lineEndPositions);
}

void ctrlc_handler(int signum){

    if(mainFile.open){
        file_close(&mainFile);
    }

    free_allocated();
    move(0, 0);
    erase();
    endwin();
    echo();
    exit(0);
}

// Entry point
int main(int argc, char** argv){

    if(argc > 1){
        if(file_open(&mainFile, argv[1])){
            
            file_readcontents(&mainFile);
            file_update_lineend_positions(&mainFile);

        } else {
            printf("Could not open \"%s\"\n", argv[1]);
            exit(0);
        }
    } else {
        mainFile.name = file_DEFAULTNAME;
        file_extend_contentsbuffer(&mainFile);
    }

    // Initializes ncurses
    initscr();
    signal(SIGINT, ctrlc_handler);
    start_color();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_WHITE, COLOR_RED);

    attron(COLOR_PAIR(1));

    window_init(&w);

    ////////////////////////////////////////////////////////////////////////////

    int ch;

    while(true){

        file_update_lineend_positions(&mainFile);
        window_updatescroll(&w, &cursor);
        window_display_titlebar(&w, &mainFile, &cursor);
        window_display_file(&w, &mainFile, &cursor);
        cursor_update(&cursor, &w);
        refresh();

        switch(ch = getch()){

            /* Arrow keys */
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
                cursor_move(&cursor, &mainFile, ch);
                break;

            /* Return - newline */
            case KEY_ENTER:
                file_insertchar(&mainFile, cursor_COORD2POS(&cursor, &mainFile), '\n');
                file_update_lineend_positions(&mainFile);
                cursor_move(&cursor, &mainFile, KEY_RIGHT);
                break;

            /* Backspace - delete char */
            case KEY_BACKSPACE:
                file_deletechar(&mainFile, cursor_COORD2POS(&cursor, &mainFile));
                file_update_lineend_positions(&mainFile);
                cursor_move(&cursor, &mainFile, KEY_LEFT);
                break;

            /* F1 - Save */
            case KEY_F(1):
                file_writecontents(&mainFile);
                break;//

            /* Insert char */
            default:
                file_insertchar(&mainFile, cursor_COORD2POS(&cursor, &mainFile), ch);
                file_update_lineend_positions(&mainFile);
                cursor_move(&cursor, &mainFile, KEY_RIGHT);
                break;
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    ctrlc_handler(0);

    return 0;
}