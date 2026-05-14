#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pardus_health_check.h"

int module_package_analyzer(Finding_t **findings, int *count) {
    if (!findings || !count) return -1;
    
    LOG_msg(RISK_INFO, "Analyzing installed packages...");
    
    *findings = malloc(20 * sizeof(Finding_t));
    *count = 0;
    
    /* Check for security updates */
    FILE *f = popen("apt list --upgradable 2>/dev/null | wc -l", "r");
    if (!f) return -1;
    
    int upgradable = 0;
    fscanf(f, "%d", &upgradable);
    pclose(f);
    
    if (upgradable > 0) {
        (*findings)[*count].severity = RISK_MEDIUM;
        snprintf((*findings)[*count].title, 256, "%d Package Updates Available", upgradable);
        snprintf((*findings)[*count].description, 1024, 
                "Outdated packages may have security vulnerabilities.");
        snprintf((*findings)[*count].recommendation, 512, 
                "Run 'sudo apt update && sudo apt upgrade' to update packages.");
        (*count)++;
    }
    
    /* Check for deprecated packages (common old packages) */
    const char *deprecated[] = {
        "apache2",      /* Old web server */
        "mysql-server", /* Old database */
        "python2",      /* Deprecated Python version */
        NULL
    };
    
    for (int i = 0; deprecated[i]; i++) {
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "dpkg -l | grep '^ii' | grep -q ' %s ' && echo 1 || echo 0", 
                deprecated[i]);
        f = popen(cmd, "r");
        if (!f) continue;
        
        int installed = 0;
        fscanf(f, "%d", &installed);
        pclose(f);
        
        if (installed) {
            (*findings)[*count].severity = RISK_MEDIUM;
            snprintf((*findings)[*count].title, 256, "Deprecated package detected: %s", 
                    deprecated[i]);
            snprintf((*findings)[*count].description, 1024, 
                    "This package is outdated and may pose security risks.");
            snprintf((*findings)[*count].recommendation, 512, 
                    "Consider replacing with modern alternatives or removing.");
            (*count)++;
        }
    }
    
    /* Check for broken dependencies */
    f = popen("apt check 2>&1", "r");
    if (f) {
        char line[256];
        while (fgets(line, sizeof(line), f)) {
            if (strstr(line, "broken") || strstr(line, "unfixed")) {
                (*findings)[*count].severity = RISK_HIGH;
                snprintf((*findings)[*count].title, 256, "Broken Package Dependencies");
                snprintf((*findings)[*count].description, 1024, 
                        "Some packages have unmet dependencies.");
                snprintf((*findings)[*count].recommendation, 512, 
                        "Run: sudo apt --fix-broken install");
                (*count)++;
                break;
            }
        }
        pclose(f);
    }
    
    LOG_msg(RISK_INFO, "Package analysis completed: %d findings", *count);
    return 0;
}
