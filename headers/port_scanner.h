/*
 * Leopard-Watchdog - Port Scanner Module
 *
 * Module 2: Enumerates open ports, identifies services,
 * and provides security hardening recommendations.
 *
 * Copyright (c) 2026 Vugar Ahadli
 * Part of the Leopard-Watchdog security framework for Pardus GNU/Linux.
 */

#ifndef PORT_SCANNER_H
#define PORT_SCANNER_H

typedef struct {
    unsigned short port;
    int protocol;  /* 0=TCP, 1=UDP */
    char service[64];
    char process_name[256];
    int pid;
    int risk_level;  /* 0=LOW, 1=NORMAL, 2=HIGH */
    char bind_address[64];
} Port_t;

typedef struct {
    Port_t *ports;
    int count;
    int high_risk_count;
    int normal_risk_count;
    int low_risk_count;
} PortScan_t;

/**
 * Initialize port scanner module
 * Returns 0 on success, -1 on error
 */
int port_init(PortScan_t *scan);

/**
 * Run port scan
 * Enumerates all open ports and associated services
 * Returns 0 on success, -1 on error
 */
int port_scan_run(PortScan_t *scan);

/**
 * Print scan results
 */
void port_print_results(const PortScan_t *scan);

/**
 * Get security recommendation for a port
 */
const char* port_get_recommendation(const Port_t *port);

/**
 * Free port scan results
 */
void port_cleanup(PortScan_t *scan);

#endif /* PORT_SCANNER_H */
