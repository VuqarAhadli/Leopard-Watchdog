/*
 * Leopard-Watchdog - Common header
 *
 * Copyright (c) 2026 Vugar Ahadli
 *
 * Part of the Leopard-Watchdog security and
 * system health auditing framework for Pardus GNU/Linux.
 *
 * Author:  Vugar Ahadli
 * Contact: vuqarahadli17@gmail.com
 */

#ifndef COMMON_H
#define COMMON_H


#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* ANSI colour escape sequences */
#define RED_FG     "\x1b[0;31m"
#define GREEN_FG   "\x1b[0;32m"
#define YELLOW_FG  "\x1b[0;33m"
#define BLUE_FG    "\x1b[0;34m"
#define RESET      "\x1b[0m"

/* CPU usage thresholds (%) */
#define CPU_NORMAL 50
#define CPU_WARN   75

/* Memory usage thresholds (%) */
#define MEM_NORMAL 50
#define MEM_WARN   75

typedef enum
{
    HIGH,
    NORMAL,
    LOW,
    COUNT
} State_t;

typedef struct
{
    const char *colour;
    const char *label;
} MessageType_t;

typedef struct {
    long user, nice, system, idle, iowait, irq, softirq;
} CpuStats;

typedef struct {
    long total_kb;
    long available_kb;
} MemStats;


/*
 * Log message types:
 * *  HIGH   - critical/warning state
 * *  NORMAL - normal operating state
 * *  LOW    - low-risk/informational state
 */
extern const MessageType_t messages[COUNT];

/**
 * Logging function with formatted messages
 * S: severity level (HIGH, NORMAL, LOW)
 * msg: message to log
 */
void LOG(State_t S, const char *msg);

/**
 * Get current local time structure
 */
struct tm get_time(void);

#endif /* COMMON_H */