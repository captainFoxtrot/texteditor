#include "defs.h"
#include "cursor.h"
#include "userInterface.h"
#include "file.h"

file_t mainFile;
cursor_t cursor;
window_t w;
enum editMode_t editMode;

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
        file_update_lineend_positions(&mainFile);
    }

    // Initialize ncurses
    initscr();
    signal(SIGINT, ctrlc_handler);
    start_color();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    // 1 Normal text - white on black
    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    // 2 Highlighted text - black on white
    init_pair(2, COLOR_BLACK, COLOR_WHITE);

    // 3 Error text - white on red
    init_pair(3, COLOR_WHITE, COLOR_RED);

    // Set normal text
    attron(COLOR_PAIR(1));

    window_init(&w);

    ////////////////////////////////////////////////////////////////////////////

    int ch;

    while(true){

        // Update display
        file_update_lineend_positions(&mainFile);
        window_updatescroll(&w, &mainFile, &cursor);
        window_display_titlebar(&w, &mainFile, &cursor, &editMode);
        window_display_file(&w, &mainFile, &cursor);
        cursor_update(&cursor, &w);
        refresh();

        // Get keystroke
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
                
                if(editMode == OVERWRITE) cursor_move(&cursor, &mainFile, KEY_RIGHT);
                refresh();

                file_deletechar(&mainFile, cursor_COORD2POS(&cursor, &mainFile));
                file_update_lineend_positions(&mainFile);
                cursor_move(&cursor, &mainFile, KEY_LEFT);
                break;

            /* F1 - Save */
            case KEY_F(1):
                file_writecontents(&mainFile);
                break;

            /* F2 - Save as */
            case KEY_F(2):
                break;

            /* F3 - Toggle insert/overwrite */
            case KEY_F(3):
                if(editMode == OVERWRITE) editMode = INSERT;
                else if(editMode == INSERT) editMode = OVERWRITE;
                break;

            /* F4 - Reload file */
            case KEY_F(4):
                file_readcontents(&mainFile);
                break;

            /* F12 - Test key */
            case KEY_F(12):;
                cursorpos_t temp;
                temp.col = 5; temp.line = 5;
                char* str = window_input(&w, temp, 10);
                break;

            /* Window resize */
            case KEY_RESIZE:
                window_init(&w);
                break;

            /* Insert/overwrite char */
            default:
                if(editMode == INSERT){
                    file_insertchar(&mainFile, cursor_COORD2POS(&cursor, &mainFile), ch);
                    file_update_lineend_positions(&mainFile);
                    cursor_move(&cursor, &mainFile, KEY_RIGHT);
                } else if(editMode == OVERWRITE){
                    file_overwritechar(&mainFile, cursor_COORD2POS(&cursor, &mainFile), ch);
                }
                break;
        }
    }

    ////////////////////////////////////////////////////////////////////////////

    ctrlc_handler(0);

    return 0;
}