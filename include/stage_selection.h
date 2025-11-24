/*
 * Stage Selection Screen
 * 
 * Displays available stages and special modes (Phrases, Review).
 * Shows completion status and lock/unlock state.
 */

#ifndef STAGE_SELECTION_H
#define STAGE_SELECTION_H

#include "progress.h"

/**
 * Display stage selection screen
 * 
 * Shows list of available stages with their status:
 * - [✓] Completed
 * - [ ] Available
 * - 🔒 Locked
 * 
 * Also shows special modes (Phrases, Review) when unlocked.
 * 
 * @param p Pointer to Progress struct for reading/updating progress
 */
void show_stage_selection(Progress *p);

#endif