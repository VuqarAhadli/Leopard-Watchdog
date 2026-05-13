/*
 * Leopard-Watchdog - Database/Caching module
 *
 * Copyright (c) 2026 Vugar Ahadli
 * Part of the Leopard-Watchdog security framework for Pardus GNU/Linux.
 */

#ifndef DATABASE_H
#define DATABASE_H

#include <time.h>

typedef struct {
    time_t last_update;
    char cache_dir[256];
} Database_t;

/**
 * Initialize database module
 * Returns 0 on success, -1 on error
 */
int db_init(Database_t *db);

/**
 * Load cached data
 * Returns 0 on success, -1 if cache invalid or doesn't exist
 */
int db_load_cache(const Database_t *db, const char *key, void *data);

/**
 * Save data to cache
 * Returns 0 on success, -1 on error
 */
int db_save_cache(const Database_t *db, const char *key, const void *data, size_t size);

/**
 * Check if cache is valid (age < threshold)
 * Returns 1 if valid, 0 if expired or missing
 */
int db_cache_valid(const Database_t *db, const char *key, int max_age_seconds);

/**
 * Cleanup database resources
 */
void db_cleanup(Database_t *db);

#endif /* DATABASE_H */
