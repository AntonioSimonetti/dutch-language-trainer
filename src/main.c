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
#include "../include/audio.h"
#include "../include/progress.h"
#include "../include/language_selection.h"
#include "../include/stage_selection.h"

int main() {
    Progress progress;

    // Initialize PDCurses, configure terminal and setup Colors for UI
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    curs_set(0);
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);

    // Load or initialize progress
    if (load_progress(&progress) == 0) {
        // First run - show language selection
        char language[3];
        show_language_selection(language);
        
        // Initialize progress with selected language
        init_progress(&progress, language);
        save_progress(&progress);
    }
    

    
    // Main loop

    while(1){
        int choice = show_main_menu();

        // Handle menu choice
        switch(choice){
            case 0: // Start
                play_success_beep();
                show_stage_selection(&progress);
                break;
            case 1: // Statistics
                play_error_beep();
                show_placeholder();
                break;
            case 2: // Settings
                play_success_beep();
                show_placeholder();
                break;
            case 3: // Exit
                // Exit from switch-case
                play_error_beep();
                break;
        }

        if(choice == 3){
            break; // Exit main loop
        }
    }

    endwin();
    return 0;
}