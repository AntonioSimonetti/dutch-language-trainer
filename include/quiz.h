/*
 * Quiz System
 * 
 * Generic quiz/exercise system for language learning.
 * Loads questions from text files and handles user interaction.
 */

#ifndef QUIZ_H
#define QUIZ_H

#include "progress.h"

#define MAX_QUESTIONS 100
#define MAX_TEXT_LENGTH 150

typedef struct {
    char question[MAX_TEXT_LENGTH];   // Italian text
    char answer[MAX_TEXT_LENGTH];     // Dutch text
} QuizItem;

typedef struct {
    QuizItem items[MAX_QUESTIONS];
    int count;                        // Number of questions loaded
    int correct;                      // Correct answers
    int wrong;                        // Wrong answers
} Quiz;

/**
 * Load quiz from file
 * 
 * Reads questions from a file in format: Italian|Dutch
 * Skips empty lines and comments (lines starting with #)
 * 
 * @param quiz Pointer to Quiz struct to populate
 * @param filename Path to quiz file (e.g., "data/exercises/greetings.txt")
 * @return 1 on success, 0 on failure
 */
int load_quiz(Quiz *quiz, const char *filename);

/**
 * Run quiz session
 * 
 * Presents questions to user, validates answers, tracks score.
 * Shows summary at the end.
 * 
 * @param quiz Pointer to Quiz struct with loaded questions
 * @param p Pointer to Progress struct (for updating completion)
 * @param stage_num Stage number (0 for non-stage exercises)
 * @return 1 if completed successfully, 0 if cancelled
 */
int run_quiz(Quiz *quiz, Progress *p, int stage_num);

#endif