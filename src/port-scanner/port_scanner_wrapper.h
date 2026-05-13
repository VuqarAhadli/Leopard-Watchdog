/*
 * Port Scanner Wrapper - Simple C interface for port scanning
 * Integrates the C++ port scanner into the main CLI
 */

#ifndef PORT_SCANNER_WRAPPER_H
#define PORT_SCANNER_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize port scanner
 */
void port_scanner_init(void);

/**
 * Run port scanner with specified options
 * Returns 0 on success, -1 on error
 */
int run_port_scan(const char *ip, const char *ports, const char *scan_type, int threads);

/**
 * Run port scanner on IP range/prefix
 */
int run_port_scan_prefix(const char *prefix, const char *ports, const char *scan_type, int threads);

/**
 * Run port scanner from file
 */
int run_port_scan_file(const char *filename, const char *ports, const char *scan_type, int threads);

/**
 * Cleanup port scanner resources
 */
void port_scanner_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif /* PORT_SCANNER_WRAPPER_H */
