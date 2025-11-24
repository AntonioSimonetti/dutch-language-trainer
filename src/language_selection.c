/*
 * Language Selection Screen - Implementation
 */

#include <pdcurses.h>
#include <string.h>
#include "../include/language_selection.h"
#include "../include/ui.h"
#include "../include/audio.h"

#define NUM_LANGUAGES 5

typedef struct {
    const char *code;        
    const char *name;        
    int available;           
} Language;

static Language languages[NUM_LANGUAGES] = {
    {"IT", "Italiano", 1},
    {"EN", "English", 0},
    {"ES", "Español", 0},
    {"FR", "Français", 0},
    {"DE", "Deutsch", 0}
};

void show_language_selection(char *language_out) {
    int selected = 0;
    int key;
    
    keypad(stdscr, TRUE);
    
    while (1) {
        clear();
        
        // Title
        draw_box(2, 1, COLS - 4, 3);
        attron(COLOR_PAIR(2) | A_BOLD);
        mvprintw(3, (COLS - 35) / 2, "BENVENUTO - WELKOM - WELCOME");
        attroff(COLOR_PAIR(2) | A_BOLD);
        
        // Main box
        draw_box(2, 5, COLS - 4, NUM_LANGUAGES + 6);
        
        // Instruction
        attron(COLOR_PAIR(2));
        mvprintw(7, (COLS - 30) / 2, "Seleziona la tua lingua madre:");
        attroff(COLOR_PAIR(2));
        
        // Language list
        for (int i = 0; i < NUM_LANGUAGES; i++) {
            int y = 9 + i;
            int x = (COLS - 40) / 2;
            
            // Cursor
            if (i == selected) {
                attron(COLOR_PAIR(1) | A_BOLD);
                mvprintw(y, x, ">");
                attroff(COLOR_PAIR(1) | A_BOLD);
            } else {
                mvprintw(y, x, " ");
            }
            
            // Language name - PRIORITÀ: selezionato > disponibile
            if (i == selected) {
                // Selected - always use cyan/bold
                attron(COLOR_PAIR(1) | A_BOLD);
                if (languages[i].available) {
                    mvprintw(y, x + 2, "%s (%s)", languages[i].name, languages[i].code);
                } else {
                    mvprintw(y, x + 2, "%s (%s)  [Coming Soon]", languages[i].name, languages[i].code);
                }
                attroff(COLOR_PAIR(1) | A_BOLD);
            } else {
                // Not selected
                if (languages[i].available) {
                    // Available but not selected - normal color
                    mvprintw(y, x + 2, "%s (%s)", languages[i].name, languages[i].code);
                } else {
                    // Not available and not selected - gray
                    attron(COLOR_PAIR(3));
                    mvprintw(y, x + 2, "%s (%s)  [Coming Soon]", languages[i].name, languages[i].code);
                    attroff(COLOR_PAIR(3));
                }
            }
        }
        
        // Footer hint
        attron(COLOR_PAIR(3));
        mvprintw(LINES - 2, (COLS - 40) / 2, "↑/↓: Naviga  ENTER: Conferma");
        attroff(COLOR_PAIR(3));
        
        refresh();
        
        // Input
        key = getch();
        
        switch (key) {
            case 60610:  // Freccia SU
            case KEY_UP:
                // NO beep per navigazione veloce
                selected = (selected - 1 + NUM_LANGUAGES) % NUM_LANGUAGES;
                break;
            
            case 60616:  // Freccia GIÙ
            case KEY_DOWN:
                // NO beep per navigazione veloce
                selected = (selected + 1) % NUM_LANGUAGES;
                break;
                
            case 10:  // ENTER
            case KEY_ENTER:
                if (languages[selected].available) {
                    // Language available - select it
                    play_success_beep();
                    strcpy(language_out, languages[selected].code);
                    return;
                } else {
                    // Coming soon - just beep error (no placeholder)
                    play_error_beep();
                }
                break;
                
            case 27:  // ESC
                // Can't exit from language selection (first run mandatory)
                play_error_beep();
                break;
        }
    }
}