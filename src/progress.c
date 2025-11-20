/*
 * Progress Tracking System - Implementation
 * 
 * Handles loading, saving, and managing user progress.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/progress.h"

// ============================================
// INITIALIZATION
// ============================================

void init_progress(Progress *p, const char *language) {
    // Copy language (max 2 chars + null terminator)
    strncpy(p->language, language, 2);
    p->language[2] = '\0';
    
    // Initialize arrays to 0
    for (int i = 0; i < MAX_STAGES; i++) {
        p->completed_stages[i] = 0;
        p->unlocked_stages[i] = 0;
    }
    
    // No stages completed yet
    p->num_completed = 0;
    
    // Stage 1 is always unlocked
    p->unlocked_stages[0] = 1;
    p->num_unlocked = 1;
    
    // Special modes locked
    p->phrases_unlocked = 0;
    p->review_unlocked = 0;
}

// ============================================
// CHECK FUNCTIONS
// ============================================

int is_stage_unlocked(Progress *p, int stage_num) {
    // Loop through unlocked stages array
    for (int i = 0; i < p->num_unlocked; i++) {
        if (p->unlocked_stages[i] == stage_num) {
            return 1;  // Found! Stage is unlocked
        }
    }
    return 0;  // Not found, stage is locked
}

int is_stage_completed(Progress *p, int stage_num) {
    // Loop through completed stages array
    for (int i = 0; i < p->num_completed; i++) {
        if (p->completed_stages[i] == stage_num) {
            return 1;  // Found! Stage is completed
        }
    }
    return 0;  // Not found, stage not completed
}

// ============================================
// MODIFICATION FUNCTIONS
// ============================================

void unlock_stage(Progress *p, int stage_num) {
    // Check if already unlocked
    if (is_stage_unlocked(p, stage_num)) {
        return;  // Already unlocked, nothing to do
    }
    
    // Add to unlocked_stages array
    p->unlocked_stages[p->num_unlocked] = stage_num;
    p->num_unlocked++;
}

void complete_stage(Progress *p, int stage_num) {
    // Check if already completed
    if (is_stage_completed(p, stage_num)) {
        return;  // Already completed, nothing to do
    }
    
    // Add to completed_stages array
    p->completed_stages[p->num_completed] = stage_num;
    p->num_completed++;
    
    // Unlock next stage
    unlock_stage(p, stage_num + 1);
    
    // Check if we should unlock special modes
    // Unlock Phrases Mode after completing Stage 1
    if (stage_num == 1) {
        p->phrases_unlocked = 1;
    }
    
    // Unlock Review Mode after completing 2 stages
    if (p->num_completed >= 2) {
        p->review_unlocked = 1;
    }
}

// ============================================
// FILE I/O FUNCTIONS
// ============================================

void save_progress(Progress *p) {
    FILE *f = fopen("data/progress.dat", "w");
    
    if (f == NULL) {
        // Cannot create file (permissions? disk full?)
        return;
    }
    
    // Write language
    fprintf(f, "language=%s\n", p->language);
    
    // Write completed stages (comma-separated)
    fprintf(f, "completed_stages=");
    for (int i = 0; i < p->num_completed; i++) {
        fprintf(f, "%d", p->completed_stages[i]);
        if (i < p->num_completed - 1) {
            fprintf(f, ",");  // Add comma between numbers
        }
    }
    fprintf(f, "\n");
    
    // Write unlocked stages (comma-separated)
    fprintf(f, "unlocked_stages=");
    for (int i = 0; i < p->num_unlocked; i++) {
        fprintf(f, "%d", p->unlocked_stages[i]);
        if (i < p->num_unlocked - 1) {
            fprintf(f, ",");
        }
    }
    fprintf(f, "\n");
    
    // Write special modes
    fprintf(f, "phrases_unlocked=%d\n", p->phrases_unlocked);
    fprintf(f, "review_unlocked=%d\n", p->review_unlocked);
    
    fclose(f);
}

int load_progress(Progress *p) {
    FILE *f = fopen("data/progress.dat", "r");
    
    if (f == NULL) {
        // File doesn't exist - first run!
        return 0;
    }
    
    char line[256];
    
    // Read line by line
    while (fgets(line, sizeof(line), f) != NULL) {
        // Parse language
        if (strncmp(line, "language=", 9) == 0) {
            sscanf(line, "language=%2s", p->language);
        }
        
        // Parse completed stages
        else if (strncmp(line, "completed_stages=", 17) == 0) {
            p->num_completed = 0;
            char *token = strtok(line + 17, ",\n");
            while (token != NULL && p->num_completed < MAX_STAGES) {
                p->completed_stages[p->num_completed] = atoi(token);
                p->num_completed++;
                token = strtok(NULL, ",\n");
            }
        }
        
        // Parse unlocked stages
        else if (strncmp(line, "unlocked_stages=", 16) == 0) {
            p->num_unlocked = 0;
            char *token = strtok(line + 16, ",\n");
            while (token != NULL && p->num_unlocked < MAX_STAGES) {
                p->unlocked_stages[p->num_unlocked] = atoi(token);
                p->num_unlocked++;
                token = strtok(NULL, ",\n");
            }
        }
        
        // Parse phrases_unlocked
        else if (strncmp(line, "phrases_unlocked=", 17) == 0) {
            sscanf(line, "phrases_unlocked=%d", &p->phrases_unlocked);
        }
        
        // Parse review_unlocked
        else if (strncmp(line, "review_unlocked=", 16) == 0) {
            sscanf(line, "review_unlocked=%d", &p->review_unlocked);
        }
    }
    
    fclose(f);
    return 1;  // Success!
}