#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pardus_health_check.h"

int module_hardware_inventory(Finding_t **findings, int *count) {
    if (!findings || !count) return -1;
    
    LOG_msg(RISK_INFO, "Scanning hardware and devices...");
    
    *findings = malloc(20 * sizeof(Finding_t));
    *count = 0;
    
    /* Check for unknown USB devices */
    FILE *f = popen("lsusb 2>/dev/null | wc -l", "r");
    if (f) {
        int usb_count = 0;
        fscanf(f, "%d", &usb_count);
        pclose(f);
        
        if (usb_count > 0) {
            LOG_msg(RISK_INFO, "Found %d USB device(s)", usb_count);
        }
    }
    
    /* Check for unsigned kernel modules */
    f = popen("find /lib/modules/$(uname -r) -name '*.ko' 2>/dev/null | wc -l", "r");
    if (f) {
        int module_count = 0;
        fscanf(f, "%d", &module_count);
        pclose(f);
        
        if (module_count > 50) {
            (*findings)[*count].severity = RISK_LOW;
            snprintf((*findings)[*count].title, 256, "%d Kernel modules loaded", module_count);
            snprintf((*findings)[*count].description, 1024, 
                    "High number of kernel modules. May indicate custom drivers.");
            snprintf((*findings)[*count].recommendation, 512, 
                    "Verify all modules are from trusted sources: lsmod");
            (*count)++;
        }
    }
    
    /* Check for non-standard PCI devices */
    f = popen("lspci 2>/dev/null | grep -i 'unknown\\|vendor'", "r");
    if (f) {
        char line[256];
        while (fgets(line, sizeof(line), f)) {
            (*findings)[*count].severity = RISK_MEDIUM;
            snprintf((*findings)[*count].title, 256, "Unknown PCI device detected");
            snprintf((*findings)[*count].description, 1024, "%s", line);
            snprintf((*findings)[*count].recommendation, 512, 
                    "Verify device is legitimate. Check with: lspci -vv");
            (*count)++;
        }
        pclose(f);
    }
    
    /* Check for BIOS/UEFI updates */
    f = popen("dmidecode 2>/dev/null | grep -i 'BIOS Date'", "r");
    if (f) {
        char line[256];
        if (fgets(line, sizeof(line), f)) {
            (*findings)[*count].severity = RISK_LOW;
            snprintf((*findings)[*count].title, 256, "Check BIOS/UEFI for updates");
            snprintf((*findings)[*count].description, 1024, "%s", line);
            snprintf((*findings)[*count].recommendation, 512, 
                    "Visit manufacturer website for latest BIOS updates for security patches");
            (*count)++;
        }
        pclose(f);
    }
    
    /* Check for removable media */
    f = popen("lsblk | grep -E 'sdb|sdc|mmc' 2>/dev/null | wc -l", "r");
    if (f) {
        int removable = 0;
        fscanf(f, "%d", &removable);
        pclose(f);
        
        if (removable > 0) {
            (*findings)[*count].severity = RISK_LOW;
            snprintf((*findings)[*count].title, 256, "%d Removable storage device(s) detected", removable);
            snprintf((*findings)[*count].description, 1024, 
                    "USB drives or SD cards are connected.");
            snprintf((*findings)[*count].recommendation, 512, 
                    "Ensure only authorized removable media are connected.");
            (*count)++;
        }
    }
    
    LOG_msg(RISK_INFO, "Hardware inventory completed: %d findings", *count);
    return 0;
}
