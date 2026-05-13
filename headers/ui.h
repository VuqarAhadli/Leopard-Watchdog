/*
 * Leopard-Watchdog - CLI User Interface module
 *
 * Copyright (c) 2026 Vugar Ahadli
 * Part of the Leopard-Watchdog security framework for Pardus GNU/Linux.
 */

#ifndef UI_H
#define UI_H

/**
 * Print application banner
 */
void ui_print_banner(void);

/**
 * Print main menu
 */
void ui_print_menu(void);

/**
 * Print colored status line
 * status: 0 = LOW, 1 = NORMAL, 2 = HIGH
 */
void ui_print_status(int status, const char *label, const char *value);

/**
 * Print section header
 */
void ui_print_section(const char *title);

/**
 * Print progress indicator
 */
void ui_print_progress(const char *message, int percent);

/**
 * Clear screen
 */
void ui_clear_screen(void);

#endif /* UI_H */
