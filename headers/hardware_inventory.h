/*
 * Leopard-Watchdog - Hardware & Software Inventory Module
 *
 * Module 3: Enumerates hardware devices and installed software,
 * detects anomalies and suspicious components.
 *
 * Copyright (c) 2026 Vugar Ahadli
 * Part of the Leopard-Watchdog security framework for Pardus GNU/Linux.
 */

#ifndef HARDWARE_INVENTORY_H
#define HARDWARE_INVENTORY_H

typedef struct {
    char name[256];
    char type[64];  /* CPU, GPU, DISK, NIC, etc */
    char model[256];
    char serial[128];
    int is_suspicious;
    int is_unsigned;
} HardwareDevice_t;

typedef struct {
    char name[256];
    char version[128];
    char maintainer[256];
    int is_signed;
    int is_common;
} SoftwarePackage_t;

typedef struct {
    HardwareDevice_t *devices;
    int device_count;
    SoftwarePackage_t *software;
    int software_count;
    int suspicious_devices;
    int unsigned_packages;
} Inventory_t;

/**
 * Initialize hardware inventory module
 * Returns 0 on success, -1 on error
 */
int inventory_init(Inventory_t *inv);

/**
 * Run hardware and software inventory
 * Returns 0 on success, -1 on error
 */
int inventory_scan_run(Inventory_t *inv);

/**
 * Print inventory results
 */
void inventory_print_results(const Inventory_t *inv);

/**
 * Free inventory results
 */
void inventory_cleanup(Inventory_t *inv);

#endif /* HARDWARE_INVENTORY_H */
