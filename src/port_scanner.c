#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pardus_health_check.h"

typedef struct {
    int port;
    const char *service;
    RiskLevel_t risk;
    const char *reason;
} KnownPort_t;

static const KnownPort_t known_ports[] = {
    {22, "SSH", RISK_LOW, "Remote shell - should be firewalled"},
    {80, "HTTP", RISK_LOW, "Web server - expected if running website"},
    {443, "HTTPS", RISK_LOW, "Secure web - expected if running website"},
    {3306, "MySQL", RISK_MEDIUM, "Database - should not be world-accessible"},
    {5432, "PostgreSQL", RISK_MEDIUM, "Database - should not be world-accessible"},
    {27017, "MongoDB", RISK_HIGH, "NoSQL DB - often misconfigured, exposed"},
    {6379, "Redis", RISK_HIGH, "Cache - should be behind firewall"},
    {9200, "Elasticsearch", RISK_HIGH, "Search engine - should be firewalled"},
    {8080, "HTTP-Alt", RISK_MEDIUM, "Alternative HTTP - check if intended"},
    {3389, "RDP", RISK_CRITICAL, "Remote Desktop - serious security risk if exposed"},
    {0, NULL, RISK_COUNT, NULL}
};

int module_port_scanner(Finding_t **findings, int *count) {
    if (!findings || !count) return -1;
    
    LOG_msg(RISK_INFO, "Scanning open ports...");
    
    *findings = malloc(30 * sizeof(Finding_t));
    *count = 0;
    
    /* Use ss/netstat to get open ports */
    FILE *f = popen("ss -tulpn 2>/dev/null | grep LISTEN", "r");
    if (!f) {
        LOG_msg(RISK_MEDIUM, "Could not enumerate ports, trying netstat fallback");
        f = popen("netstat -tulpn 2>/dev/null | grep LISTEN", "r");
        if (!f) return -1;
    }
    
    char line[512];
    int port_count = 0;
    
    while (fgets(line, sizeof(line), f) && *count < 30) {
        int port = 0;
        char proto[16];
        
        /* Extract port number from output */
        if (sscanf(line, "%s", proto) < 1) continue;
        
        /* Try to extract port from netstat/ss output */
        char *colon = strrchr(line, ':');
        if (colon) {
            sscanf(colon + 1, "%d", &port);
        }
        
        if (port <= 0 || port > 65535) continue;
        port_count++;
        
        /* Check against known ports */
        RiskLevel_t risk = RISK_INFO;
        const char *service = "Unknown";
        const char *reason = "Unknown service on this port";
        
        for (int i = 0; known_ports[i].port != 0; i++) {
            if (known_ports[i].port == port) {
                risk = known_ports[i].risk;
                service = known_ports[i].service;
                reason = known_ports[i].reason;
                break;
            }
        }
        
        if (risk >= RISK_MEDIUM) {  /* Only log concerning ports */
            (*findings)[*count].severity = risk;
            snprintf((*findings)[*count].title, 256, "Open port %d (%s)", port, service);
            snprintf((*findings)[*count].description, 1024, "%s", reason);
            snprintf((*findings)[*count].recommendation, 512, 
                    "Verify this port is needed. Close with firewall if not: sudo ufw deny %d/tcp",
                    port);
            (*count)++;
        }
    }
    pclose(f);
    
    /* If too many ports open, flag it */
    if (port_count > 10) {
        (*findings)[*count].severity = RISK_MEDIUM;
        snprintf((*findings)[*count].title, 256, "Excessive open ports: %d", port_count);
        snprintf((*findings)[*count].description, 1024, 
                "System has many open ports, increasing attack surface.");
        snprintf((*findings)[*count].recommendation, 512, 
                "Review each port and close unnecessary ones with firewall.");
        (*count)++;
    }
    
    LOG_msg(RISK_INFO, "Port scan completed: %d open ports, %d concerning", port_count, *count);
    return 0;
}
