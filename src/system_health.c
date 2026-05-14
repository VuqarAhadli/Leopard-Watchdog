#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pardus_health_check.h"

#define CPU_WARN_THRESHOLD  75
#define CPU_NORM_THRESHOLD  50
#define MEM_WARN_THRESHOLD  80
#define MEM_NORM_THRESHOLD  50
#define DISK_CRIT_THRESHOLD 90
#define DISK_WARN_THRESHOLD 80

typedef struct {
    long user, nice, system, idle, iowait, irq, softirq;
} CpuStats_t;

CpuStats_t read_cpu_stat(void) {
    CpuStats_t s = {0};
    FILE *f = fopen("/proc/stat", "r");
    if (!f) return s;
    fscanf(f, "cpu %ld %ld %ld %ld %ld %ld %ld", 
           &s.user, &s.nice, &s.system, &s.idle, &s.iowait, &s.irq, &s.softirq);
    fclose(f);
    return s;
}

float cpu_percent(CpuStats_t first, CpuStats_t second) {
    long idle_first = first.idle + first.iowait;
    long idle_second = second.idle + second.iowait;
    long total_first = first.user + first.nice + first.system + idle_first + 
                      first.irq + first.softirq;
    long total_second = second.user + second.nice + second.system + idle_second + 
                       second.irq + second.softirq;
    
    long res_idle = idle_second - idle_first;
    long res_total = total_second - total_first;
    
    if (res_total == 0) return 0.0f;
    return (1.0f - (float)res_idle / (float)res_total) * 100.0f;
}

typedef struct {
    long total_kb;
    long available_kb;
} MemStats_t;

MemStats_t read_mem_stat(void) {
    MemStats_t m = {0};
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

float mem_percent(MemStats_t m) {
    if (m.total_kb == 0) return 0.0f;
    return (1.0f - (float)m.available_kb / (float)m.total_kb) * 100.0f;
}

typedef struct {
    long blocks_total;
    long blocks_used;
} DiskStats_t;

DiskStats_t read_disk_stat(const char *mount) {
    DiskStats_t d = {0};
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "df %s | tail -1", mount);
    
    FILE *f = popen(cmd, "r");
    if (!f) return d;
    
    char dummy[64];
    fscanf(f, "%s %ld %ld", dummy, &d.blocks_total, &d.blocks_used);
    pclose(f);
    
    return d;
}

int check_services(void) {
    FILE *f = popen("systemctl list-units --type=service --state=failed --no-pager", "r");
    if (!f) return 0;
    
    int failed = 0;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "failed")) failed++;
    }
    pclose(f);
    return failed;
}

int module_system_health(Finding_t **findings, int *count) {
    if (!findings || !count) return -1;
    
    LOG_msg(RISK_INFO, "Scanning system health...");
    
    *findings = malloc(10 * sizeof(Finding_t));
    *count = 0;
    
    /* CPU Usage */
    CpuStats_t cpu1 = read_cpu_stat();
    sleep(1);
    CpuStats_t cpu2 = read_cpu_stat();
    float cpu_usage = cpu_percent(cpu1, cpu2);
    
    if (cpu_usage >= CPU_WARN_THRESHOLD) {
        (*findings)[*count].severity = RISK_HIGH;
        snprintf((*findings)[*count].title, 256, "High CPU Usage: %.1f%%", cpu_usage);
        snprintf((*findings)[*count].description, 1024, 
                "CPU usage is above threshold. Check running processes with 'top'.");
        snprintf((*findings)[*count].recommendation, 512, 
                "Run 'top' or 'htop' to identify and kill resource-heavy processes.");
        (*count)++;
    }
    
    /* Memory Usage */
    MemStats_t mem = read_mem_stat();
    float mem_usage = mem_percent(mem);
    
    if (mem_usage >= MEM_WARN_THRESHOLD) {
        (*findings)[*count].severity = RISK_HIGH;
        snprintf((*findings)[*count].title, 256, "High Memory Usage: %.1f%%", mem_usage);
        snprintf((*findings)[*count].description, 1024, 
                "Memory usage is critical. System may be slow.");
        snprintf((*findings)[*count].recommendation, 512, 
                "Close unnecessary applications or add more RAM.");
        (*count)++;
    }
    
    /* Disk Usage */
    DiskStats_t disk = read_disk_stat("/");
    float disk_usage = (float)disk.blocks_used / (float)disk.blocks_total * 100.0f;
    
    if (disk_usage >= DISK_CRIT_THRESHOLD) {
        (*findings)[*count].severity = RISK_CRITICAL;
        snprintf((*findings)[*count].title, 256, "Critical Disk Space: %.1f%%", disk_usage);
        snprintf((*findings)[*count].description, 1024, 
                "Root partition is almost full!");
        snprintf((*findings)[*count].recommendation, 512, 
                "Clean up temporary files: 'sudo apt clean' and 'sudo journalctl --vacuum=30d'");
        (*count)++;
    } else if (disk_usage >= DISK_WARN_THRESHOLD) {
        (*findings)[*count].severity = RISK_MEDIUM;
        snprintf((*findings)[*count].title, 256, "High Disk Usage: %.1f%%", disk_usage);
        snprintf((*findings)[*count].description, 1024, 
                "Disk space is getting low.");
        snprintf((*findings)[*count].recommendation, 512, 
                "Monitor disk usage and clean up unnecessary files.");
        (*count)++;
    }
    
    /* Failed Services */
    int failed = check_services();
    if (failed > 0) {
        (*findings)[*count].severity = RISK_HIGH;
        snprintf((*findings)[*count].title, 256, "%d Failed Services", failed);
        snprintf((*findings)[*count].description, 1024, 
                "One or more systemd services have failed.");
        snprintf((*findings)[*count].recommendation, 512, 
                "Check with: systemctl status <service-name>");
        (*count)++;
    }
    
    LOG_msg(RISK_INFO, "System health check completed: %d findings", *count);
    return 0;
}



void monitor_cpu_mem(int time,int interval){
    CpuStats_t previous = read_cpu_stat();
	sleep(1);
	int first = 1;
	for (;;) {
		if (!first) {
            printf("\x1b[2A\x1b[J"); 
        }
        first = 0;
        CpuStats_t current = read_cpu_stat();
        float cpu = cpu_percent(previous, current);
        previous = current;
 
        char cpu_msg[128];
        snprintf(cpu_msg, sizeof(cpu_msg), "CPU usage: %.1f%%", cpu);
 
        if (cpu >= CPU_WARN_THRESHOLD) LOG(RISK_HIGH, cpu_msg,time);
        else if (cpu >= CPU_NORM_THRESHOLD) LOG(RISK_MEDIUM, cpu_msg,time);
        else LOG(RISK_LOW, cpu_msg,time);
 

        MemStats_t mem = read_mem_stat();
        float mem_used = mem_percent(mem);
        long used_mb = (mem.total_kb - mem.available_kb) / 1024;
        long total_mb = mem.total_kb / 1024;
 
        char mem_msg[128];
        snprintf(mem_msg, sizeof(mem_msg), "MEM usage: %.1f%% (%ld / %ld MB)", mem_used, used_mb, total_mb);
 
        if (mem_used >= MEM_WARN_THRESHOLD) LOG(RISK_HIGH, mem_msg,time);
        else if (mem_used >= MEM_NORM_THRESHOLD) LOG(RISK_MEDIUM, mem_msg,time);
        else LOG(RISK_LOW, mem_msg,time);
 
        fflush(stdout);
        usleep(interval * 1000);
    }

}