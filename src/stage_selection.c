/*
 * Stage Selection Screen - Implementation
 */

#include <pdcurses.h>
#include <stdio.h>
#include <string.h>
#include "../include/stage_selection.h"
#include "../include/ui.h"
#include "../include/audio.h"
#include "../include/progress.h"

#define MAX_DISPLAY_STAGES 4
#define TOTAL_MENU_ITEMS (MAX_DISPLAY_STAGES + 2)  // Stages + Phrases + Review

typedef struct {
    int stage_number;        // 0 = not a stage (special mode)
    const char *title;
    int is_special;          // 1 = Phrases/Review, 0 = normal stage
    int special_type;        // 0 = phrases, 1 = review
} MenuItem;

static MenuItem menu_items[TOTAL_MENU_ITEMS] = {
    {1, "Stage 1: Introduzioni", 0, 0},
    {2, "Stage 2: Provenienza", 0, 0},
    {3, "Stage 3: Domande Base", 0, 0},
    {4, "Stage 4: Question Words", 0, 0},
    {0, "Modalita Frasi", 1, 0},
    {0, "Modalita Review", 1, 1}
};

// Forward declaration
static void run_stage_placeholder(Progress *p, int stage_num);

void show_stage_selection(Progress *p) {
    int selected = 0;
    int key;
    
    keypad(stdscr, TRUE);
    
    while (1) {
        clear();
        
        // Title
        draw_box(2, 1, COLS - 4, 3);
        attron(COLOR_PAIR(2) | A_BOLD);
        mvprintw(3, (COLS - 25) / 2, "SELEZIONE ESERCIZI");
        attroff(COLOR_PAIR(2) | A_BOLD);
        
        // Main box
        draw_box(2, 5, COLS - 4, TOTAL_MENU_ITEMS + 5);
        
        // Menu items
        int y_start = 7;
        
        for (int i = 0; i < TOTAL_MENU_ITEMS; i++) {
            int y = y_start + i;
            int x = (COLS - 50) / 2;
            
            // Separator before special modes
            if (i == MAX_DISPLAY_STAGES) {
                mvprintw(y, x, "------------------------------");
                y++;
                y_start++;
            }
            
            y = y_start + i;
            
            // Cursor
            if (i == selected) {
                attron(COLOR_PAIR(1) | A_BOLD);
                mvprintw(y, x, ">");
                attroff(COLOR_PAIR(1) | A_BOLD);
            } else {
                mvprintw(y, x, " ");
            }
            
            // Determine status
            int is_unlocked = 0;
            int is_completed = 0;
            
            if (menu_items[i].is_special) {
                // Special mode
                if (menu_items[i].special_type == 0) {
                    // Phrases mode
                    is_unlocked = p->phrases_unlocked;
                } else {
                    // Review mode
                    is_unlocked = p->review_unlocked;
                }
            } else {
                // Normal stage
                is_unlocked = is_stage_unlocked(p, menu_items[i].stage_number);
                is_completed = is_stage_completed(p, menu_items[i].stage_number);
            }
            
            // Draw item with status
            if (i == selected) {
                attron(COLOR_PAIR(1) | A_BOLD);
            }
            
            // Status icon
            if (is_completed) {
                mvprintw(y, x + 2, "[");
                attron(A_BOLD);
                addch(ACS_DIAMOND);  // ✓ symbol
                attroff(A_BOLD);
                addstr("]");
            } else if (is_unlocked) {
                mvprintw(y, x + 2, "[ ]");
            } else {
                mvprintw(y, x + 2, "[");
                addch(ACS_CKBOARD);  // Lock symbol
                addstr("]");
            }
            
            // Title
            mvprintw(y, x + 6, "%s", menu_items[i].title);
            
            // Lock indicator if not unlocked
            if (!is_unlocked) {
                mvprintw(y, x + 6 + strlen(menu_items[i].title) + 2, "BLOCCATO");
            }
            
            if (i == selected) {
                attroff(COLOR_PAIR(1) | A_BOLD);
            }
        }
        
        // Footer hint
        attron(COLOR_PAIR(3));
        mvprintw(LINES - 2, (COLS - 45) / 2, "↑/↓: Naviga  ENTER: Seleziona  ESC: Esci");
        attroff(COLOR_PAIR(3));
        
        refresh();
        
        // Input
        key = getch();
        
        switch (key) {
            case 60610:  // Up
            case KEY_UP:
                selected = (selected - 1 + TOTAL_MENU_ITEMS) % TOTAL_MENU_ITEMS;
                break;
            
            case 60616:  // Down
            case KEY_DOWN:
                selected = (selected + 1) % TOTAL_MENU_ITEMS;
                break;
                
            case 10:  // ENTER
            case KEY_ENTER:
                if (menu_items[selected].is_special) {
                    // Special mode
                    int is_unlocked_mode = (menu_items[selected].special_type == 0) 
                        ? p->phrases_unlocked 
                        : p->review_unlocked;
                    
                    if (is_unlocked_mode) {
                        play_success_beep();
                        show_placeholder();  // TODO: implement actual modes
                    } else {
                        play_error_beep();
                    }
                } else {
                    // Normal stage
                    if (is_stage_unlocked(p, menu_items[selected].stage_number)) {
                        play_success_beep();
                        run_stage_placeholder(p, menu_items[selected].stage_number);
                        save_progress(p);  // Save after completing stage
                    } else {
                        play_error_beep();
                    }
                }
                break;
                
            case 27:  // ESC
                play_error_beep();
                return;  // Exit to main menu
        }
    }
}

static void run_stage_placeholder(Progress *p, int stage_num) {
    clear();
    
    draw_box(2, 5, COLS - 4, 8);
    
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(7, (COLS - 30) / 2, "STAGE %d", stage_num);
    attroff(COLOR_PAIR(2) | A_BOLD);
    
    mvprintw(9, (COLS - 40) / 2, "Questo e' uno stage placeholder.");
    mvprintw(10, (COLS - 40) / 2, "Premi ENTER per completare lo stage.");
    
    attron(COLOR_PAIR(3));
    mvprintw(LINES - 2, (COLS - 30) / 2, "ENTER: Completa  ESC: Annulla");
    attroff(COLOR_PAIR(3));
    
    refresh();
    
    int key = getch();
    
    if (key == 10 || key == KEY_ENTER) {
        // Complete the stage
        complete_stage(p, stage_num);
        play_success_beep();
        
        // Show completion message
        clear();
        draw_box(2, 5, COLS - 4, 6);
        attron(COLOR_PAIR(2) | A_BOLD);
        mvprintw(8, (COLS - 25) / 2, "STAGE %d COMPLETATO!", stage_num);
        attroff(COLOR_PAIR(2) | A_BOLD);
        
        attron(COLOR_PAIR(3));
        mvprintw(LINES - 2, (COLS - 30) / 2, "Premi un tasto per continuare...");
        attroff(COLOR_PAIR(3));
        
        refresh();
        getch();
    } else {
        play_error_beep();
    }
}