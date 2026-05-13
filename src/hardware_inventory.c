/*
 * Leopard-Watchdog - Hardware & Software Inventory Module
 *
 * Module 3: Enumerates hardware devices and installed software,
 * detects anomalies and suspicious components.
 *
 * Copyright (c) 2026 Vugar Ahadli
 */

#include "common.h"
#include "../headers/hardware_inventory.h"
#include "../headers/ui.h"

int inventory_init(Inventory_t *inv) {
    if (!inv) return -1;
    
    inv->devices = NULL;
    inv->device_count = 0;
    inv->software = NULL;
    inv->software_count = 0;
    inv->suspicious_devices = 0;
    inv->unsigned_packages = 0;
    
    return 0;
}

int inventory_scan_run(Inventory_t *inv) {
    if (!inv) return -1;
    
    /* TODO: Parse /proc/cpuinfo for CPU info */
    /* TODO: Parse lspci output for PCI devices */
    /* TODO: Parse lsusb output for USB devices */
    /* TODO: Read /sys filesystem for device info */
    /* TODO: Get installed packages from dpkg */
    /* TODO: Check package signatures and checksums */
    /* TODO: Detect unsigned or rare packages */
    /* TODO: Compare against known device database */
    
    LOG(NORMAL, "Hardware and software inventory scan started");
    
    /* TODO: Populate inv->devices and inv->software arrays */
    /* TODO: Calculate anomaly statistics */
    
    LOG(NORMAL, "Inventory scan completed");
    return 0;
}

void inventory_print_results(const Inventory_t *inv) {
    if (!inv) return;
    
    ui_print_section("Hardware & Software Inventory");
    
    printf("  Devices Found: %d\n", inv->device_count);
    printf("  Suspicious Devices: %d\n", inv->suspicious_devices);
    printf("  Software Packages: %d\n", inv->software_count);
    printf("  Unsigned Packages: %d\n", inv->unsigned_packages);
    
    /* TODO: Print detailed inventory list */
    /* TODO: Highlight suspicious items */
    /* TODO: Show verification status */
}

void inventory_cleanup(Inventory_t *inv) {
    if (inv) {
        if (inv->devices) {
            free(inv->devices);
            inv->devices = NULL;
        }
        if (inv->software) {
            free(inv->software);
            inv->software = NULL;
        }
        inv->device_count = 0;
        inv->software_count = 0;
    }
}
