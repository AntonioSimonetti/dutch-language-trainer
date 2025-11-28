/*
 * Quiz System - Implementation
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <pdcurses.h>
#include "../include/quiz.h"
#include "../include/ui.h"
#include "../include/audio.h"
#include "../include/progress.h"

// Helper function prototypes
static void normalize_string(char *str);
static int compare_answers(const char *user_input, const char *correct_answer);
static void show_quiz_summary(Quiz *quiz);
static void shuffle_quiz(Quiz *quiz);

static void shuffle_quiz(Quiz *quiz) {
    // Fisher-Yates shuffle algorithm
    for (int i = quiz->count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        
        // Swap items[i] and items[j]
        QuizItem temp = quiz->items[i];
        quiz->items[i] = quiz->items[j];
        quiz->items[j] = temp;
    }
}

int load_quiz(Quiz *quiz, const char *filename) {
    FILE *f = fopen(filename, "r");
    
    if (f == NULL) {
        return 0;  // File not found
    }
    
    quiz->count = 0;
    quiz->correct = 0;
    quiz->wrong = 0;
    
    char line[300];
    
    while (fgets(line, sizeof(line), f) != NULL && quiz->count < MAX_QUESTIONS) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        // Skip empty lines
        if (strlen(line) == 0) {
            continue;
        }
        
        // Skip comments (lines starting with #)
        if (line[0] == '#') {
            continue;
        }
        
        // Parse line: Italian|Dutch
        char *italian = strtok(line, "|");
        char *dutch = strtok(NULL, "|");
        
        if (italian != NULL && dutch != NULL) {
            // Copy to quiz item
            strncpy(quiz->items[quiz->count].question, italian, MAX_TEXT_LENGTH - 1);
            strncpy(quiz->items[quiz->count].answer, dutch, MAX_TEXT_LENGTH - 1);
            quiz->items[quiz->count].question[MAX_TEXT_LENGTH - 1] = '\0';
            quiz->items[quiz->count].answer[MAX_TEXT_LENGTH - 1] = '\0';
            
            quiz->count++;
        }
    }
    
    fclose(f);
    
    if (quiz->count > 0) {
        // Seed random generator (only once)
        static int seeded = 0;
        if (!seeded) {
            srand(time(NULL));
            seeded = 1;
        }
        
        // Shuffle questions for random order
        shuffle_quiz(quiz);
        return 1;
    }
    
    return 0;
}

int run_quiz(Quiz *quiz, Progress *p, int stage_num) {
    if (quiz->count == 0) {
        return 0;  // No questions loaded
    }
    
    // Reset stats
    quiz->correct = 0;
    quiz->wrong = 0;
    
    char user_input[MAX_TEXT_LENGTH];
    
    // Loop through all questions
    for (int i = 0; i < quiz->count; i++) {
        clear();
        
        // Header
        draw_box(2, 1, COLS - 4, 3);
        attron(COLOR_PAIR(2) | A_BOLD);
        mvprintw(3, (COLS - 30) / 2, "ESERCIZIO - Domanda %d/%d", i + 1, quiz->count);
        attroff(COLOR_PAIR(2) | A_BOLD);
        
        // Question box
        draw_box(2, 5, COLS - 4, 10);
        
        // Show question
        attron(COLOR_PAIR(2));
        mvprintw(7, (COLS - 40) / 2, "Traduci in Olandese:");
        attroff(COLOR_PAIR(2));
        
        attron(A_BOLD);
        mvprintw(9, (COLS - strlen(quiz->items[i].question)) / 2, "%s", quiz->items[i].question);
        attroff(A_BOLD);
        
        mvprintw(11, (COLS - 20) / 2, "Tua risposta:");
        
        // Input prompt
        int input_y = 13;
        int input_x = (COLS - 60) / 2;
        mvprintw(input_y, input_x, "> ");
        
        // Footer
        attron(COLOR_PAIR(3));
        mvprintw(LINES - 2, (COLS - 40) / 2, "Scrivi la risposta e premi ENTER");
        attroff(COLOR_PAIR(3));
        
        refresh();
        
        // Manual input handling
        memset(user_input, 0, MAX_TEXT_LENGTH);
        int pos = 0;
        curs_set(1);
        
        while (1) {
            move(input_y, input_x + 2 + pos);
            refresh();
            
            int ch = getch();
            
            if (ch == 10 || ch == KEY_ENTER) {
                // Enter pressed
                break;
            } else if (ch == KEY_BACKSPACE || ch == 8 || ch == 127) {
                // Backspace
                if (pos > 0) {
                    pos--;
                    user_input[pos] = '\0';
                    mvaddch(input_y, input_x + 2 + pos, ' ');
                }
            } else if (ch >= 32 && ch < 127 && pos < MAX_TEXT_LENGTH - 1) {
                // Printable character
                user_input[pos] = (char)ch;
                mvaddch(input_y, input_x + 2 + pos, ch);
                pos++;
                user_input[pos] = '\0';
            }
        }
        
        curs_set(0);
        
        // Validate answer
        int is_correct = compare_answers(user_input, quiz->items[i].answer);
        
        // Show feedback
        clear();
        draw_box(2, 5, COLS - 4, 12);
        
        if (is_correct) {
            quiz->correct++;
            play_success_beep();
            
            attron(COLOR_PAIR(2) | A_BOLD);
            mvprintw(7, (COLS - 10) / 2, "CORRETTO!");
            attroff(COLOR_PAIR(2) | A_BOLD);
        } else {
            quiz->wrong++;
            play_error_beep();
            
            attron(COLOR_PAIR(1) | A_BOLD);
            mvprintw(7, (COLS - 12) / 2, "SBAGLIATO!");
            attroff(COLOR_PAIR(1) | A_BOLD);
        }
        
        // Show correct answer
        mvprintw(9, (COLS - 20) / 2, "Risposta corretta:");
        attron(A_BOLD);
        mvprintw(10, (COLS - strlen(quiz->items[i].answer)) / 2, "%s", quiz->items[i].answer);
        attroff(A_BOLD);
        
        if (!is_correct) {
            mvprintw(12, (COLS - 15) / 2, "Tu hai scritto:");
            mvprintw(13, (COLS - strlen(user_input)) / 2, "%s", user_input);
        }
        
        // Continue prompt
        attron(COLOR_PAIR(3));
        mvprintw(LINES - 2, (COLS - 30) / 2, "Premi un tasto per continuare...");
        attroff(COLOR_PAIR(3));
        
        refresh();
        getch();
    }
    
    // Show summary
    show_quiz_summary(quiz);
    
    // Check if passed (70% threshold)
    float accuracy = (float)quiz->correct / quiz->count;
    
    if (accuracy >= 0.70 && stage_num > 0) {
        complete_stage(p, stage_num);
        save_progress(p);
        return 1;
    }
    
    return (accuracy >= 0.70) ? 1 : 0;
}

static void normalize_string(char *str) {
    // Convert to lowercase and trim spaces
    int len = strlen(str);
    int write_pos = 0;
    
    for (int i = 0; i < len; i++) {
        char c = tolower((unsigned char)str[i]);
        
        // Skip leading spaces
        if (c == ' ' && write_pos == 0) {
            continue;
        }
        
        str[write_pos++] = c;
    }
    
    // Remove trailing spaces
    while (write_pos > 0 && str[write_pos - 1] == ' ') {
        write_pos--;
    }
    
    str[write_pos] = '\0';
}

static int compare_answers(const char *user_input, const char *correct_answer) {
    char user_normalized[MAX_TEXT_LENGTH];
    char correct_normalized[MAX_TEXT_LENGTH];
    
    // Copy strings
    strncpy(user_normalized, user_input, MAX_TEXT_LENGTH - 1);
    strncpy(correct_normalized, correct_answer, MAX_TEXT_LENGTH - 1);
    user_normalized[MAX_TEXT_LENGTH - 1] = '\0';
    correct_normalized[MAX_TEXT_LENGTH - 1] = '\0';
    
    // Normalize both
    normalize_string(user_normalized);
    normalize_string(correct_normalized);
    
    // Compare
    return (strcmp(user_normalized, correct_normalized) == 0);
}

static void show_quiz_summary(Quiz *quiz) {
    clear();
    
    draw_box(2, 5, COLS - 4, 12);
    
    attron(COLOR_PAIR(2) | A_BOLD);
    mvprintw(7, (COLS - 20) / 2, "RIEPILOGO ESERCIZIO");
    attroff(COLOR_PAIR(2) | A_BOLD);
    
    // Stats
    float accuracy = (float)quiz->correct / quiz->count * 100;
    
    mvprintw(9, (COLS - 30) / 2, "Domande totali: %d", quiz->count);
    
    attron(COLOR_PAIR(2));
    mvprintw(10, (COLS - 30) / 2, "Risposte corrette: %d", quiz->correct);
    attroff(COLOR_PAIR(2));
    
    attron(COLOR_PAIR(1));
    mvprintw(11, (COLS - 30) / 2, "Risposte sbagliate: %d", quiz->wrong);
    attroff(COLOR_PAIR(1));
    
    mvprintw(13, (COLS - 30) / 2, "Precisione: %.1f%%", accuracy);
    
    // Pass/Fail
    if (accuracy >= 70.0) {
        attron(COLOR_PAIR(2) | A_BOLD);
        mvprintw(15, (COLS - 20) / 2, "ESERCIZIO SUPERATO!");
        attroff(COLOR_PAIR(2) | A_BOLD);
    } else {
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(15, (COLS - 25) / 2, "Riprova! (Minimo 70%)");
        attroff(COLOR_PAIR(1) | A_BOLD);
    }
    
    attron(COLOR_PAIR(3));
    mvprintw(LINES - 2, (COLS - 30) / 2, "Premi un tasto per continuare...");
    attroff(COLOR_PAIR(3));
    
    refresh();
    getch();
}