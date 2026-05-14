#ifndef PARDUS_HEALTH_CHECK_H
#define PARDUS_HEALTH_CHECK_H

#include <time.h>
#include <stddef.h>

/* Color codes for terminal output */
#define RED_FG      "\x1b[0;31m"
#define GREEN_FG    "\x1b[0;32m"
#define YELLOW_FG   "\x1b[0;33m"
#define BLUE_FG     "\x1b[0;34m"
#define MAGENTA_FG  "\x1b[0;35m"
#define CYAN_FG     "\x1b[0;36m"
#define RESET       "\x1b[0m"
#define BOLD        "\x1b[1m"

/* Risk severity levels */
typedef enum {
    RISK_CRITICAL = 0,  /* Red - immediate action needed */
    RISK_HIGH = 1,      /* Orange/Red - important to fix */
    RISK_MEDIUM = 2,    /* Yellow - should be addressed */
    RISK_LOW = 3,       /* Light yellow - monitor */
    RISK_INFO = 4,      /* Blue - informational */
    RISK_COUNT = 5      /* Enum end.*/
} RiskLevel_t;

/* Finding structure */
typedef struct {
    RiskLevel_t severity;
    char title[256];
    char description[1024];
    char recommendation[512];
    time_t detected_at;
} Finding_t;

/* System Health Score */
typedef struct {
    float overall_score;        /* 0-100 */
    float package_score;        /* Package health */
    float network_score;        /* Open ports risk */
    float hardware_score;       /* Hardware integrity */
    float system_score;         /* CPU/Memory/Disk/Services */
    int total_findings;
    int critical_count;
    int high_count;
    int medium_count;
    int low_count;
} HealthScore_t;

/* Report output format */
typedef enum {
    REPORT_CLI,             /* Colored terminal output */
    REPORT_JSON,            /* Machine-readable JSON */
    REPORT_HTML,            /* Web-viewable HTML */
    REPORT_FORMAT_COUNT     /* Enum end.*/
} ReportFormat_t;

typedef struct {
    int verbose;
    int run_all;
    int check_packages;
    int check_network;
    int check_hardware;
    int check_system;
    ReportFormat_t report_format;
    char report_file[256];
} Options_t;

/* Module function pointers */
typedef int (*ModuleFunc_t)(Finding_t **findings, int *count);

/* Logger */
void LOG_init(void);
void LOG_msg(RiskLevel_t level, const char *format, ...);
void LOG_cleanup(void);

/* System Health Module */
int module_system_health(Finding_t **findings, int *count);

/* Package Analyzer Module */
int module_package_analyzer(Finding_t **findings, int *count);

/* Port Scanner Module */
int module_port_scanner(Finding_t **findings, int *count);

/* Hardware Inventory Module */
int module_hardware_inventory(Finding_t **findings, int *count);

/* Report Generator */
int generate_report(Finding_t *findings, int count, HealthScore_t *score, 
                   ReportFormat_t format, const char *output_file);

void monitor_cpu_mem(int time,int interval);
void LOG(RiskLevel_t  S, const char *msg, int time);
extern Options_t opts;

#endif
