#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

// Colours
#define RED_FG "\x1b[0;31m"
#define GREEN_FG "\x1b[0;32m"
#define YELLOW_FG "\x1b[0;33m"
#define BLUE_FG "\x1b[0;34m"
#define RESET "\x1b[0m"

#define CPU_NORMAL  50
#define CPU_WARN 75
#define MEM_NORMAL  50
#define MEM_WARN 75


typedef enum
{
	HIGH,
	NORMAL,
	LOW,
	COUNT
} State_t;

struct tm get_time()
{
	time_t raw_time = time(NULL);
	struct tm *info = localtime(&raw_time);
	return *info;
}

typedef struct
{
	const char *colour; 
	const char *label;
} MessageType_t;

static const  MessageType_t messages[COUNT] = 
{
    [HIGH] = {RED_FG,"WARNING"},
    [NORMAL] = {YELLOW_FG, "NORMAL"},
    [LOW] = {GREEN_FG, "LOW"},
};

void LOG(State_t S, const char *msg) {
    if (S >= COUNT) return;
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


typedef struct {
    long user, nice, system, idle, iowait, irq, softirq;
} CpuStats;

CpuStats read_cpu_stat() {
    CpuStats s = {0};
    FILE *f = fopen("/proc/stat", "r");
    if (!f) return s;
    fscanf(f, "cpu %ld %ld %ld %ld %ld %ld %ld", &s.user, &s.nice, &s.system, &s.idle, &s.iowait, &s.irq, &s.softirq);
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

typedef struct {
    long total_kb;
    long available_kb;
} MemStats;

MemStats read_mem_stat() {
    MemStats m = {0};
    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) return m;
 
    char key[64];
    long val;
    char unit[16];
 
    for (int i = 0; i < 10; i++) {
        if (fscanf(f, "%s %ld %s\n", key, &val, unit) < 2) break;
        if (strcmp(key, "MemTotal:") == 0) m.total_kb = val;
        if (strcmp(key, "MemAvailable:") == 0) m.available_kb = val;
    }
    fclose(f);
    return m;
}

float mem_percent(MemStats m) {
    if (m.total_kb == 0) return 0.0f;
    return (1.0f - (float)m.available_kb / (float)m.total_kb) * 100.0f;
}


int main()
{
	CpuStats previous = read_cpu_stat();
	sleep(1);
	int first = 1;
	for (;;) {
		if (!first) {
            printf("\x1b[2A\x1b[J"); 
        }
        first = 0;
        CpuStats current = read_cpu_stat();
        float cpu = cpu_percent(previous, current);
        previous = current;
 
        char cpu_msg[128];
        snprintf(cpu_msg, sizeof(cpu_msg), "CPU usage: %.1f%%", cpu);
 
        if (cpu >= CPU_WARN) LOG(HIGH, cpu_msg);
        else if (cpu >= CPU_NORMAL) LOG(NORMAL, cpu_msg);
        else LOG(LOW, cpu_msg);
 

        MemStats mem = read_mem_stat();
        float mem_used = mem_percent(mem);
        long used_mb = (mem.total_kb - mem.available_kb) / 1024;
        long total_mb = mem.total_kb / 1024;
 
        char mem_msg[128];
        snprintf(mem_msg, sizeof(mem_msg), "MEM usage: %.1f%% (%ld / %ld MB)", mem_used, used_mb, total_mb);
 
        if (mem_used >= MEM_WARN) LOG(HIGH, mem_msg);
        else if (mem_used >= MEM_NORMAL) LOG(NORMAL, mem_msg);
        else LOG(LOW, mem_msg);
 
        fflush(stdout);
        usleep(100000);
    }

	return 0;
}
