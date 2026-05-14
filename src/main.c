#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <pardus_health_check.h>

/* UI functions */
void ui_print_banner(void);
void ui_print_header(const char *title);
void ui_print_module_start(const char *module);
void ui_print_progress(const char *message, int percent);
void ui_print_spacing(void);
void ui_print_help(const char *program_name);



Options_t parse_arguments(int argc, char *argv[]) {
    Options_t opts = {
        .verbose = 0,
        .run_all = 1,
        .check_packages = 1,
        .check_network = 1,
        .check_hardware = 1,
        .check_system = 1,
        .report_format = REPORT_CLI,
        .report_file = {0}
    };
    
    struct option long_opts[] = {
        {"help", no_argument, NULL, 'h'},
        {"verbose", no_argument, NULL, 'v'},
        {"quiet", no_argument, NULL, 'q'},
        {"cli", no_argument, NULL, 'c'},
        {"json", required_argument, NULL, 'j'},
        {"html", required_argument, NULL, 'w'},
        {"all", no_argument, NULL, 'a'},
        {"packages", no_argument, NULL, 'p'},
        {"network", no_argument, NULL, 'n'},
        {"hardware", no_argument, NULL, 'd'},
        {"system", no_argument, NULL, 's'},
        {NULL, 0, NULL, 0}
    };
    
    int opt;
    while ((opt = getopt_long(argc, argv, "hvqcj:w:apnds", long_opts, NULL)) != -1) {
        switch (opt) {
            case 'h':
                ui_print_help(argv[0]);
                exit(EXIT_SUCCESS);
            case 'v':
                opts.verbose = 1;
                break;
            case 'q':
                opts.verbose = 0;
                break;
            case 'c':
                opts.report_format = REPORT_CLI;
                break;
            case 'j':
                opts.report_format = REPORT_JSON;
                strncpy(opts.report_file, optarg, sizeof(opts.report_file) - 1);
                break;
            case 'w':
                opts.report_format = REPORT_HTML;
                strncpy(opts.report_file, optarg, sizeof(opts.report_file) - 1);
                break;
            case 'a':
                opts.run_all = 1;
                opts.check_packages = 1;
                opts.check_network = 1;
                opts.check_hardware = 1;
                opts.check_system = 1;
                break;
            case 'p':
                opts.run_all = 0;
                opts.check_packages = 1;
                opts.check_network = 0;
                opts.check_hardware = 0;
                opts.check_system = 0;
                break;
            case 'n':
                opts.run_all = 0;
                opts.check_packages = 0;
                opts.check_network = 1;
                opts.check_hardware = 0;
                opts.check_system = 0;
                break;
            case 'd':
                opts.run_all = 0;
                opts.check_packages = 0;
                opts.check_network = 0;
                opts.check_hardware = 1;
                opts.check_system = 0;
                break;
            case 's':
                opts.run_all = 0;
                opts.check_packages = 0;
                opts.check_network = 0;
                opts.check_hardware = 0;
                opts.check_system = 1;
                break;
            default:
                ui_print_help(argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    
    return opts;
}
Options_t opts;
int main(int argc, char *argv[]) {
    opts = parse_arguments(argc, argv);
    
    LOG_init();
    ui_print_banner();
    ui_print_header("Pardus System Security & Health Check");
    
    Finding_t *all_findings = malloc(256 * sizeof(Finding_t));
    int total_findings = 0;
    
    HealthScore_t score = {
        .overall_score = 100.0f,
        .package_score = 100.0f,
        .network_score = 100.0f,
        .hardware_score = 100.0f,
        .system_score = 100.0f,
        .critical_count = 0,
        .high_count = 0,
        .medium_count = 0,
        .low_count = 0,
        .total_findings = 0
    };
    
    int progress = 0;
    int total_modules = 0;
    if (opts.check_system) total_modules++;
    if (opts.check_packages) total_modules++;
    if (opts.check_network) total_modules++;
    if (opts.check_hardware) total_modules++;
    
    /* Module 1: System Health */
    if (opts.check_system) {
        ui_print_module_start("Checking System Health (CPU, Memory, Disk, Services)...");
        Finding_t *findings = NULL;
        int count = 0;
        
        if (module_system_health(&findings, &count) == 0 && findings && count > 0) {
            memcpy(&all_findings[total_findings], findings, count * sizeof(Finding_t));
            total_findings += count;
            score.system_score -= count * 5.0f;  /* Reduce score for each finding */
        }
        free(findings);
        progress++;
        ui_print_progress("System Health", (progress * 100) / total_modules);
    }
    
    /* Module 2: Package Analyzer */
    if (opts.check_packages) {
        ui_print_module_start("Analyzing Packages (outdated, deprecated, broken)...");
        Finding_t *findings = NULL;
        int count = 0;
        
        if (module_package_analyzer(&findings, &count) == 0 && findings && count > 0) {
            memcpy(&all_findings[total_findings], findings, count * sizeof(Finding_t));
            total_findings += count;
            score.package_score -= count * 5.0f;
        }
        free(findings);
        progress++;
        ui_print_progress("Package Check", (progress * 100) / total_modules);
    }
    
    /* Module 3: Port Scanner */
    if (opts.check_network) {
        ui_print_module_start("Scanning Open Ports (network security)...");
        Finding_t *findings = NULL;
        int count = 0;
        
        if (module_port_scanner(&findings, &count) == 0 && findings && count > 0) {
            memcpy(&all_findings[total_findings], findings, count * sizeof(Finding_t));
            total_findings += count;
            score.network_score -= count * 5.0f;
        }
        free(findings);
        progress++;
        ui_print_progress("Network Scan", (progress * 100) / total_modules);
    }
    
    /* Module 4: Hardware Inventory */
    if (opts.check_hardware) {
        ui_print_module_start("Checking Hardware (unknown devices, drivers)...");
        Finding_t *findings = NULL;
        int count = 0;
        
        if (module_hardware_inventory(&findings, &count) == 0 && findings && count > 0) {
            memcpy(&all_findings[total_findings], findings, count * sizeof(Finding_t));
            total_findings += count;
            score.hardware_score -= count * 5.0f;
        }
        free(findings);
        progress++;
        ui_print_progress("Hardware Check", (progress * 100) / total_modules);
    }
    
    /* Clamp scores to 0-100 */
    if (score.package_score < 0) score.package_score = 0;
    if (score.network_score < 0) score.network_score = 0;
    if (score.hardware_score < 0) score.hardware_score = 0;
    if (score.system_score < 0) score.system_score = 0;
    
    /* Count findings by severity */
    for (int i = 0; i < total_findings; i++) {
        switch (all_findings[i].severity) {
            case RISK_CRITICAL: score.critical_count++; break;
            case RISK_HIGH: score.high_count++; break;
            case RISK_MEDIUM: score.medium_count++; break;
            case RISK_LOW: score.low_count++; break;
            default: break;
        }
    }
    score.total_findings = total_findings;
    
    printf("\n");
    ui_print_spacing();
    
    /* Generate Report */
    generate_report(all_findings, total_findings, &score, opts.report_format, 
                   opts.report_file);
    
    if (opts.report_format == REPORT_JSON) {
        LOG_msg(RISK_INFO, "Report saved to: %s", opts.report_file);
    } else if (opts.report_format == REPORT_HTML) {
        LOG_msg(RISK_INFO, "Report saved to: %s", opts.report_file);
    }
    
    free(all_findings);
    LOG_cleanup();


    // if (argc < 3) {
    // fprintf(stderr,
    //     "Usage: %s <time> <interval_ms>\n",
    //     argv[0]);
    // return 1;
    // }

    // int time = atoi(argv[1]);
    // int interval = atoi(argv[2]);


}
