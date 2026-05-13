/*
 * Leopard-Watchdog - Configuration management module
 *
 * Copyright (c) 2026 Vugar Ahadli
 * Part of the Leopard-Watchdog security framework for Pardus GNU/Linux.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>

typedef struct {
    int verbose;
    int quiet;
    char report_format[32];  /* json, text, html */
    char config_file[256];
} Config_t;

/**
 * Load configuration from default locations
 * Returns 0 on success, -1 on error
 */
int config_load(Config_t *config);

/**
 * Parse configuration file
 * Returns 0 on success, -1 on error
 */
int config_parse_file(Config_t *config, const char *filepath);

/**
 * Print current configuration
 */
void config_print(const Config_t *config);

#endif /* CONFIG_H */
