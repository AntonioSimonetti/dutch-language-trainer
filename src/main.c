/*
 * Dutch Language Trainer - Main Menu
 * 
 * Terminal-based language learning application
 * Built with C99 and PDCurses for Windows
 */

#include <pdcurses.h>
#include <stdbool.h>
#include <string.h>

int main() {
    // Initialize PDCurses, configure terminal and setup Colors for UI
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    curs_set(0);
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);

    // Menu Configuration
    const char *menu_options[] = {"Start","Statistics","Settings","Exit"};
    int selected = 0;
    int num_options = sizeof(menu_options) / sizeof(menu_options[0]);

    const char *title = "=== DUTCH LANGUAGE TRAINER ===";
    bool running = true;

    // Main menu loop
    while(running){
        // Clear screen and redraw menu at every iteration
        clear();
        attron(COLOR_PAIR(1));

        // Center title horizontally
        int title_x = (COLS - strlen(title)) / 2;

        mvprintw(2, title_x, title);

        attroff(COLOR_PAIR(1));

        // Calculate centered position for menu options
        int max_len = 0;
        for(int i = 0; i < num_options;i++){
            int len = strlen(menu_options[i]);
            if(len > max_len) max_len = len;
        }

        int menu_options_x = (COLS - max_len -2) /2;
        
        for(int i=0; i < num_options; i++){

            if(i == selected){
                attron(COLOR_PAIR(2));
                mvprintw(6 + i, menu_options_x ,"► %s" , menu_options[i]);
                attroff(COLOR_PAIR(2));
            } else {
                mvprintw(6 + i, menu_options_x ,"  %s" , menu_options[i]);
            }
        }

        refresh();
        int key = getch(); // Wait for user input

        // Handle key presses
        if(key == 60616){
            selected++;

            if(selected >= num_options){
                selected = 0;
            }
        }

        if(key == 60610){
            selected--;

            if(selected < 0){
                selected = num_options -1;
            }
        }

        // Handle menu selection (Enter key)
        if(key == 10){
           switch(selected){
                case 0:
                    // TODO: Start
                    break;
                case 1:
                    // TODO: Statistics
                    break;
                case 2:
                    // TODO: Settings
                    break;
                case 3:
                    running = false;
                    break;
                default:
                    // Shouldn't happen.
                    clear();
                    mvprintw(LINES/2, 5, "ERROR: selected = %d (impossible!)", selected);
                    mvprintw(LINES/2 + 1, 5, "Press any key to exit...");
                    refresh();
                    getch();
                    running = false;
                    break;
           }
        }

    }

    endwin();
    return 0;
}