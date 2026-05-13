/*
 * Leopard-Watchdog - System Health & Diagnostics Module
 *
 * Module 4: Monitors CPU, memory, disk, services, and logs.
 * Generates unified health score and recommendations.
 *
 * Copyright (c) 2026 Vugar Ahadli
 * Part of the Leopard-Watchdog security framework for Pardus GNU/Linux.
 */

#ifndef SYSTEM_HEALTH_H
#define SYSTEM_HEALTH_H

typedef struct {
    float cpu_usage;        /* Percentage */
    float memory_usage;     /* Percentage */
    float disk_usage;       /* Percentage */
    int failed_services;
    int critical_errors;
    int health_score;       /* 0-100 */
    char recommendation[512];
} SystemHealth_t;

typedef struct {
    float cpu_percent;
    int load_average[3];
    int cpu_cores;
} CpuStats_t;

typedef struct {
    float used_percent;
    unsigned long total_kb;
    unsigned long available_kb;
} MemoryStats_t;

typedef struct {
    float used_percent;
    unsigned long total_kb;
    unsigned long free_kb;
} DiskStats_t;

/**
 * Initialize system health module
 * Returns 0 on success, -1 on error
 */
int health_init(void);

/**
 * Run full system health check
 * Returns 0 on success, -1 on error
 */
int health_check_run(SystemHealth_t *health);

/**
 * Get CPU statistics
 */
int health_get_cpu_stats(CpuStats_t *stats);

/**
 * Get memory statistics
 */
int health_get_memory_stats(MemoryStats_t *stats);

/**
 * Get disk statistics for root partition
 */
int health_get_disk_stats(DiskStats_t *stats);

/**
 * Calculate overall health score
 * Returns score 0-100
 */
int health_calculate_score(const SystemHealth_t *health);

/**
 * Print health check results
 */
void health_print_results(const SystemHealth_t *health);

/**
 * Cleanup health module resources
 */
void health_cleanup(void);

#endif /* SYSTEM_HEALTH_H */
