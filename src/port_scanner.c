/*
 * Leopard-Watchdog - Port Scanner Module
 *
 * Module 2: Enumerates open ports, identifies services,
 * and provides security hardening recommendations.
 *
 * Copyright (c) 2026 Vugar Ahadli
 */

#include "common.h"
#include "../headers/port_scanner.h"
#include "../headers/ui.h"

int port_init(PortScan_t *scan) {
    if (!scan) return -1;
    
    scan->ports = NULL;
    scan->count = 0;
    scan->high_risk_count = 0;
    scan->normal_risk_count = 0;
    scan->low_risk_count = 0;
    
    return 0;
}

int port_scan_run(PortScan_t *scan) {
    if (!scan) return -1;
    
    /* TODO: Parse /proc/net/tcp and /proc/net/tcp6 */
    /* TODO: Parse /proc/net/udp and /proc/net/udp6 */
    /* TODO: Map ports to process IDs */
    /* TODO: Read /proc/[pid]/cmdline for process names */
    /* TODO: Look up service names from /etc/services */
    /* TODO: Assess risk level for each port */
    /* TODO: Build process/service mapping */
    
    LOG(NORMAL, "Port scan started");
    
    /* TODO: Populate scan->ports array */
    /* TODO: Calculate risk statistics */
    
    LOG(NORMAL, "Port scan completed");
    return 0;
}

void port_print_results(const PortScan_t *scan) {
    if (!scan) return;
    
    ui_print_section("Port Security Scan Results");
    
    if (scan->count == 0) {
        printf("  No open ports found\n");
        return;
    }
    
    printf("  Total Open Ports: %d\n", scan->count);
    printf("  High Risk: %d\n", scan->high_risk_count);
    printf("  Normal Risk: %d\n", scan->normal_risk_count);
    printf("  Low Risk: %d\n", scan->low_risk_count);
    
    /* TODO: Print detailed port list with colors */
    /* TODO: Sort by risk level */
    /* TODO: Show hardening recommendations */
}

const char* port_get_recommendation(const Port_t *port) {
    if (!port) return "N/A";
    
    /* TODO: Generate hardening recommendations based on service and risk */
    switch (port->risk_level) {
        case 2:  /* HIGH */
            return "Disable if not needed, or restrict access via firewall";
        case 1:  /* NORMAL */
            return "Monitor for unusual activity";
        case 0:  /* LOW */
            return "Safe to leave enabled";
        default:
            return "Unknown risk level";
    }
}

void port_cleanup(PortScan_t *scan) {
    if (scan && scan->ports) {
        free(scan->ports);
        scan->ports = NULL;
        scan->count = 0;
    }
}
