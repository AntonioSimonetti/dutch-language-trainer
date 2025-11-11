/*
 * Dutch Language Trainer - Main Menu
 * 
 * Terminal-based language learning application
 * Built with C99 and PDCurses for Windows
 */

#include <pdcurses.h>
#include <stdbool.h>
#include <string.h>
#include "../include/ui.h"
#include "../include/menu.h"

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

    
    // Main loop

    while(1){
        int choice = show_main_menu();

        // Handle menu choice
        switch(choice){
            case 0: // Start
                show_placeholder();
                break;
            case 1: // Statistics
                show_placeholder();
                break;
            case 2: // Settings
                show_placeholder();
                break;
            case 3: // Exit
                // Exit from switch-case
                break;
        }

        if(choice == 3){
            break; // Exit main loop
        }
    }

    endwin();
    return 0;
}