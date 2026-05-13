/*
 * Leopard-Watchdog - APT Package Audit Module
 *
 * Module 1: Detects old, outdated, and deprecated APT packages
 * Provides upgrade recommendations and security advisories.
 *
 * Copyright (c) 2026 Vugar Ahadli
 * Part of the Leopard-Watchdog security framework for Pardus GNU/Linux.
 */

#ifndef APT_AUDIT_H
#define APT_AUDIT_H

typedef struct {
    char package_name[256];
    char current_version[128];
    char available_version[128];
    int is_deprecated;
    int has_security_update;
    int risk_level;  /* 0=LOW, 1=NORMAL, 2=HIGH */
} Package_t;

typedef struct {
    Package_t *packages;
    int count;
    int total_deprecated;
    int total_security_updates;
} AptAudit_t;

/**
 * Initialize APT audit module
 * Returns 0 on success, -1 on error
 */
int apt_init(AptAudit_t *audit);

/**
 * Run APT package audit
 * Scans installed packages and detects issues
 * Returns 0 on success, -1 on error
 */
int apt_audit_run(AptAudit_t *audit);

/**
 * Print audit results
 */
void apt_print_results(const AptAudit_t *audit);

/**
 * Get upgrade recommendation for a package
 */
const char* apt_get_recommendation(const Package_t *package);

/**
 * Free audit results
 */
void apt_cleanup(AptAudit_t *audit);

#endif /* APT_AUDIT_H */
