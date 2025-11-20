/*
 * Progress Tracking System
 * 
 * Manages user progress, unlocked stages, and completed exercises.
 * Progress is saved to progress.dat file.
 */

#ifndef PROGRESS_H
#define PROGRESS_H

#define MAX_STAGES 10

typedef struct {
    char language[3];              // User's native language: "IT", "EN", "ES"
    int completed_stages[MAX_STAGES]; // Array of completed stage numbers
    int num_completed;             // Count of completed stages
    int unlocked_stages[MAX_STAGES];  // Array of unlocked stage numbers
    int num_unlocked;              // Count of unlocked stages
    int phrases_unlocked;          // 1 if phrase mode unlocked, 0 otherwise
    int review_unlocked;           // 1 if review mode unlocked, 0 otherwise
} Progress;

/**
 * Load progress from file
 * 
 * @param p Pointer to Progress struct to fill
 * @return 1 if file found and loaded, 0 if first run (file not found)
 */
int load_progress(Progress *p);

/**
 * Save progress to file
 * 
 * @param p Pointer to Progress struct to save
 */
void save_progress(Progress *p);

/**
 * Check if a stage is unlocked
 * 
 * @param p Pointer to Progress struct
 * @param stage_num Stage number to check
 * @return 1 if unlocked, 0 if locked
 */
int is_stage_unlocked(Progress *p, int stage_num);

/**
 * Check if a stage is completed
 * 
 * @param p Pointer to Progress struct
 * @param stage_num Stage number to check
 * @return 1 if completed, 0 if not completed
 */
int is_stage_completed(Progress *p, int stage_num);

/**
 * Unlock a stage
 * 
 * @param p Pointer to Progress struct
 * @param stage_num Stage number to unlock
 */
void unlock_stage(Progress *p, int stage_num);

/**
 * Mark a stage as completed and unlock next stage
 * 
 * @param p Pointer to Progress struct
 * @param stage_num Stage number to complete
 */
void complete_stage(Progress *p, int stage_num);

/**
 * Initialize progress for first run
 * 
 * @param p Pointer to Progress struct
 * @param language User's native language code
 */
void init_progress(Progress *p, const char *language);

#endif