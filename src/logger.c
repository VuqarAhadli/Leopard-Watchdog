#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include "pardus_health_check.h"



typedef struct {
    const char *colour;
    const char *label;
    const char *emoji;
} MessageType_t;

static const MessageType_t message_types[RISK_COUNT] = {
    [RISK_CRITICAL] = {RED_FG,     "CRITICAL", "🚨"},
    [RISK_HIGH]     = {RED_FG,     "HIGH",     "⚠️"},
    [RISK_MEDIUM]   = {YELLOW_FG,  "MEDIUM",   "⚡"},
    [RISK_LOW]      = {GREEN_FG,  "LOW",      "ℹ️"},
    [RISK_INFO]     = {BLUE_FG,    "INFO",     "ℹ️"},
};

static FILE *log_file = NULL;
static int use_colors = 1;

void LOG_init(void) {
    /* Check if stdout is a TTY (supports colors) */
    use_colors = isatty(STDOUT_FILENO);
    
    /* Open log file for persistent logging */
    log_file = fopen("/tmp/pardus-health-check.log", "a");
    if (!log_file) {
        fprintf(stderr, "Warning: Could not open log file\n");
    }
}

struct tm get_time(void) {
    time_t raw_time = time(NULL);
    struct tm *info = localtime(&raw_time);
    return *info;
}

void LOG_msg(RiskLevel_t level, const char *format, ...) {
    if (level >= RISK_COUNT) return;
    
    const MessageType_t *msg = &message_types[level];
    struct tm now = get_time();
    
    char timestamp[32];
    snprintf(timestamp, sizeof(timestamp), "%02d:%02d:%02d %d-%02d-%04d",
             now.tm_hour, now.tm_min, now.tm_sec,
             now.tm_mday, now.tm_mon + 1, now.tm_year + 1900);
    
    va_list args;
    char message[512];
    
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);
    
    if (opts.verbose){
        usleep(100000);
        if (use_colors) {
            printf("[%s%s%s] %s at %s\n", 
                msg->colour, msg->label, RESET, message, timestamp);
        } else {
            printf("[%s] %s at %s\n", msg->label, message, timestamp);
        }
    }
    
    /* Log to file */
    if (log_file) {
        fprintf(log_file, "[%s] %s at %s\n", msg->label, message, timestamp);
        
        fflush(log_file);
    }

}

void LOG_cleanup(void) {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

void LOG(RiskLevel_t  S, const char *msg, int time) {
    if (S >= RISK_COUNT) return;
    const MessageType_t *m = &message_types[S];
    struct tm now = get_time();
    printf("[%s%s%s] %s",
        m->colour,
		m->label,
		RESET,
        msg
    );
    if (time != 0){
        printf(" at %02d:%02d:%02d %d-%02d-%04d",
            now.tm_hour,
            now.tm_min,
            now.tm_sec,
            now.tm_mday,
            now.tm_mon + 1,
            now.tm_year + 1900
        );
    }
    printf("\n");
}