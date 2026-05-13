/*
 * Leopard-Watchdog - CPU/RAM workload logger and audit module
 *
 * Copyright (c) 2026 Vugar Ahadli
 *
 * Part of the Leopard-Watchdog security and
 * system health auditing framework for Pardus GNU/Linux.
 *
 * Author:  Vugar Ahadli
 * Contact: vuqarahadli17@gmail.com
 */



#include "common.h"


/*
 *
 * Get time for accurate logging
 * 
 */
struct tm get_time()
{
    time_t raw_time = time(NULL);
    struct tm *info = localtime(&raw_time);

    if (!info)
        return (struct tm){0};

    return *info;
}



void LOG(State_t S, const char *msg) {
    if (S >= COUNT || S < 0) return;
    const MessageType_t *m = &messages[S];
    struct tm now = get_time();

    printf("[%s%s%s] %s at %02d:%02d:%02d %d-%02d-%04d\n",
        m->colour,
		m->label,
		RESET,
        msg,
        now.tm_hour,
		now.tm_min,
		now.tm_sec,
        now.tm_mday,
		now.tm_mon + 1,
		now.tm_year + 1900
    );
}




CpuStats read_cpu_stat()
{
    CpuStats s = {0};

    FILE *f = fopen("/proc/stat", "r");
    if (!f)
        return s;

    if (fscanf(f, "cpu %ld %ld %ld %ld %ld %ld %ld",
        &s.user, &s.nice, &s.system, &s.idle,
        &s.iowait, &s.irq, &s.softirq) != 7)
    {
        fclose(f);
        return (CpuStats){0};
    }

    fclose(f);
    return s;
}

float cpu_percent(CpuStats first, CpuStats second) {
    long idle_first = first.idle + first.iowait;
    long idle_second  = second.idle + second.iowait;
    long total_first = first.user + first.nice + first.system + idle_first + first.irq + first.softirq;
    long total_second = second.user + second.nice + second.system + idle_second + second.irq + second.softirq;
 
    long res_idle = idle_second - idle_first;
    long res_total = total_second - total_first;
 
    if (res_total == 0) return 0.0f;
    return (1.0f - (float)res_idle / (float)res_total) * 100.0f;
}



MemStats read_mem_stat()
{
    MemStats m = {0};

    FILE *f = fopen("/proc/meminfo", "r");
    if (!f)
        return m;

    char key[64];
    long val;
    char unit[16];

    while (fscanf(f, "%63s %ld %15s", key, &val, unit) == 3)
    {
        if (strcmp(key, "MemTotal:") == 0)
            m.total_kb = val;

        if (strcmp(key, "MemAvailable:") == 0)
            m.available_kb = val;
    }

    fclose(f);
    return m;
}

float mem_percent(MemStats m) {
    if (m.total_kb == 0) return 0.0f;
    return (1.0f - (float)m.available_kb / (float)m.total_kb) * 100.0f;
}


/**
 * TODO: Remove this section and implement complete logging module
 * Reference implementation for CPU/memory monitoring available if needed
 */
