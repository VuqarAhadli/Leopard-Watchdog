/*
 * Leopard-Watchdog - CLI User Interface Module
 *
 * Copyright (c) 2026 Vugar Ahadli
 */

#include "common.h"
#include "../headers/ui.h"

void ui_print_banner(void) {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                                                           ║\n");
    printf("║        🐆  Leopard-Watchdog v1.0                         ║\n");
    printf("║        Pardus Security & System Health Audit             ║\n");
    printf("║                                                           ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

void ui_print_menu(void) {
    ui_print_section("Main Menu");
    printf("  [1] Run Full System Audit\n");
    printf("  [2] APT Package Audit\n");
    printf("  [3] Port Security Scan\n");
    printf("  [4] Hardware/Software Inventory\n");
    printf("  [5] System Health Check\n");
    printf("  [6] View Cached Results\n");
    printf("  [7] Generate Report\n");
    printf("  [0] Exit\n");
    printf("\n");
}

void ui_print_status(int status, const char *label, const char *value) {
    const char *color;
    const char *indicator;
    
    switch (status) {
        case 0:  /* LOW */
            color = GREEN_FG;
            indicator = "✓";
            break;
        case 1:  /* NORMAL */
            color = YELLOW_FG;
            indicator = "⚠";
            break;
        case 2:  /* HIGH */
            color = RED_FG;
            indicator = "✗";
            break;
        default:
            color = RESET;
            indicator = "?";
    }
    
    printf("  %s%s%s %s: %s\n", color, indicator, RESET, label, value);
}

void ui_print_section(const char *title) {
    if (!title) return;
    
    printf("\n%s=== %s ===%s\n", BLUE_FG, title, RESET);
}

void ui_print_progress(const char *message, int percent) {
    if (!message) return;
    
    printf("\r[");
    for (int i = 0; i < 50; i++) {
        printf("%s", i * 2 < percent ? "=" : " ");
    }
    printf("] %d%% - %s", percent, message);
    fflush(stdout);
}

void ui_clear_screen(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}
