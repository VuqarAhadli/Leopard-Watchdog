/*
 * Leopard-Watchdog - Main Entry Point
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
#include "../headers/config.h"
#include "../headers/report.h"
#include "../headers/database.h"
#include "../headers/ui.h"
#include "../headers/apt_audit.h"
#include "../headers/hardware_inventory.h"
#include "../headers/system_health.h"
#include "../port-scanner/port_scanner_wrapper.h"

/* Global message types for logging */
const MessageType_t messages[COUNT] = {
    [HIGH]    = {    RED_FG,    "WARNING" },
    [NORMAL]  = { YELLOW_FG,    "NORMAL"  },
    [LOW]     = {  GREEN_FG,    "INFO"    }
};

/**
 * Print usage information
 */
void print_usage(const char *prog_name) {
    printf("Usage: %s [OPTIONS]\n", prog_name);
    printf("\nOptions:\n");
    printf("  --help                 Show this help message\n");
    printf("  --version              Show version information\n");
    printf("  --verbose              Enable verbose output\n");
    printf("  --quiet                Suppress non-critical output\n");
    printf("  --report FORMAT        Generate report (json, text, html)\n");
    printf("\nModules:\n");
    printf("  --full-audit           Run all audit modules\n");
    printf("  --apt-audit            APT package audit only\n");
    printf("  --network OPTS         Network port scanner\n");
    printf("      -i IP              Single IP to scan\n");
    printf("      -p PORTS           Ports (e.g. 1-1000 or 22,80,443)\n");
    printf("      --prefix CIDR      IP range (e.g. 192.168.1.0/24)\n");
    printf("      --file FILE        IPs from file\n");
    printf("      --scan TYPE        Scan type (SYN, UDP, FIN, NULL, XMAS)\n");
    printf("      --threads N        Parallel threads (default 1)\n");
    printf("  --hardware-check       Hardware/software inventory only\n");
    printf("  --health-check         System health check only\n");
    printf("\nExamples:\n");
    printf("  %s --full-audit --verbose\n", prog_name);
    printf("  %s --apt-audit --report json\n", prog_name);
    printf("  %s --network -i 192.168.1.1 -p 1-1000\n", prog_name);
    printf("  %s --network --prefix 192.168.1.0/24 -p 22,80,443 --threads 10\n", prog_name);
    printf("\n");
}

/**
 * Parse command-line arguments
 */
int parse_arguments(int argc, char *argv[], Config_t *config) {
    if (argc < 1) return -1;
    
    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 1;
        }
        else if (strcmp(argv[i], "--version") == 0) {
            printf("Leopard-Watchdog v1.0\n");
            return 1;
        }
        else if (strcmp(argv[i], "--network") == 0) {
            /* Parse network scanner arguments */
            const char *ip = NULL;
            const char *ports = NULL;
            const char *prefix = NULL;
            const char *file = NULL;
            const char *scan_type = "SYN";
            int threads = 1;
            
            i++;
            while (i < argc && argv[i][0] == '-') {
                if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
                    ip = argv[++i];
                }
                else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
                    ports = argv[++i];
                }
                else if (strcmp(argv[i], "--prefix") == 0 && i + 1 < argc) {
                    prefix = argv[++i];
                }
                else if (strcmp(argv[i], "--file") == 0 && i + 1 < argc) {
                    file = argv[++i];
                }
                else if (strcmp(argv[i], "--scan") == 0 && i + 1 < argc) {
                    scan_type = argv[++i];
                }
                else if (strcmp(argv[i], "--threads") == 0 && i + 1 < argc) {
                    threads = atoi(argv[++i]);
                }
                else {
                    printf("Unknown network option: %s\n", argv[i]);
                    return 1;
                }
                i++;
            }
            
            if (!ports) {
                printf("Error: --network requires -p <ports>\n");
                return 1;
            }
            
            ui_print_banner();
            ui_print_section("Network Port Scanner");
            
            int result = -1;
            if (ip) {
                result = run_port_scan(ip, ports, scan_type, threads);
            }
            else if (prefix) {
                result = run_port_scan_prefix(prefix, ports, scan_type, threads);
            }
            else if (file) {
                result = run_port_scan_file(file, ports, scan_type, threads);
            }
            else {
                printf("Error: --network requires -i <IP>, --prefix <CIDR>, or --file <FILE>\n");
                return 1;
            }
            
            /* Return 1 to signal main() not to run full_audit, exit immediately */
            return 1;
        }
        else if (strcmp(argv[i], "--verbose") == 0) {
            config->verbose = 1;
        }
        else if (strcmp(argv[i], "--quiet") == 0) {
            config->quiet = 1;
        }
        else if (strcmp(argv[i], "--report") == 0 && i + 1 < argc) {
            strncpy(config->report_format, argv[++i], sizeof(config->report_format) - 1);
        }
        i++;
    }
    
    return 0;
}

/**
 * Run full system audit
 */
int run_full_audit(const Config_t *config) {
    int ret = 0;
    
    ui_print_section("Starting Full System Audit");
    
    /* Module 1: APT Audit */
    AptAudit_t apt_audit = {0};
    if (apt_init(&apt_audit) == 0) {
        if (apt_audit_run(&apt_audit) == 0) {
            apt_print_results(&apt_audit);
        }
        apt_cleanup(&apt_audit);
    }
    
    /* Module 2: Network/Port Scanner - NOTE: Run with --network flag separately */
    ui_print_section("Network Scanning");
    printf("  Use: ./leopard-watchdog --network -i <IP> -p <ports>\n");
    printf("  Example: ./leopard-watchdog --network -i 192.168.1.1 -p 1-1000\n");
    
    /* Module 3: Hardware Inventory */
    Inventory_t inventory = {0};
    if (inventory_init(&inventory) == 0) {
        if (inventory_scan_run(&inventory) == 0) {
            inventory_print_results(&inventory);
        }
        inventory_cleanup(&inventory);
    }
    
    /* Module 4: System Health */
    if (health_init() == 0) {
        SystemHealth_t health = {0};
        if (health_check_run(&health) == 0) {
            health_print_results(&health);
        }
        health_cleanup();
    }
    
    /* TODO: Generate report if requested */
    if (config->report_format[0] != '\0' && strcmp(config->report_format, "text") != 0) {
        Report_t *report = report_create();
        if (report) {
            /* TODO: Populate report with module results */
            if (strcmp(config->report_format, "json") == 0) {
                report_export_json(report, "/tmp/leopard-watchdog-report.json");
            }
            report_free(report);
        }
    }
    
    ui_print_section("Audit Complete");
    LOG(LOW, "Full system audit completed successfully");
    
    return ret;
}

/**
 * Main entry point
 */
int main(int argc, char *argv[]) {
    Config_t config = {0};
    
    /* Parse configuration */
    if (config_load(&config) != 0) {
        LOG(HIGH, "Failed to load configuration");
        return 1;
    }
    
    /* Parse command-line arguments */
    int arg_ret = parse_arguments(argc, argv, &config);
    if (arg_ret > 0) return 0;
    if (arg_ret < 0) {
        print_usage(argv[0]);
        return 1;
    }
    
    /* Print banner */
    if (!config.quiet) {
        ui_print_banner();
    }
    
    /* Run full audit as default behavior */
    if (run_full_audit(&config) != 0) {
        LOG(HIGH, "Audit encountered errors");
        return 1;
    }
    
    return 0;
}
