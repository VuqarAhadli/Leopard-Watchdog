/*
 * Leopard-Watchdog - Report generation module
 *
 * Copyright (c) 2026 Vugar Ahadli
 * Part of the Leopard-Watchdog security framework for Pardus GNU/Linux.
 */

#ifndef REPORT_H
#define REPORT_H

#include <time.h>

typedef struct {
    time_t timestamp;
    char system_name[128];
    char hostname[128];
    /* Module results will be added here */
} Report_t;

/**
 * Create a new report structure
 */
Report_t* report_create(void);

/**
 * Export report to JSON format
 * Returns 0 on success, -1 on error
 */
int report_export_json(const Report_t *report, const char *filepath);

/**
 * Export report to plain text format
 * Returns 0 on success, -1 on error
 */
int report_export_text(const Report_t *report, const char *filepath);

/**
 * Print report summary to stdout
 */
void report_print_summary(const Report_t *report);

/**
 * Free report structure
 */
void report_free(Report_t *report);

#endif /* REPORT_H */
