/*
 * Port Scanner Wrapper Implementation
 * Provides simple C interface to C++ port scanner
 */

#include "port_scanner_wrapper.h"
#include "optionsManager.h"
#include "Job.h"
#include <iostream>
#include <map>
#include <vector>
#include <pthread.h>
#include <cstring>
#include <ctime>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

// Forward declarations from portScanner.cpp
extern int processCommand(map<string, string> opDict);
extern void createJobQueue();
extern void destroyJobQueue();
extern void printJobStats();
extern void* sendPacket(void *ip);
extern void getCurrentSystemIP(char *ip);

void port_scanner_init(void) {
    // Initialize if needed
}

void port_scanner_cleanup(void) {
    // Cleanup if needed
}

static int execute_scan(map<string, string>& options) {
    try {
        time_t start = time(NULL);
        
        cout << "Scanning......" << endl;
        
        // Process command with options
        if (processCommand(options) < 0) {
            cerr << "Error processing scan options" << endl;
            return -1;
        }
        
        // Create job queue
        createJobQueue();
        
        // Get number of threads
        int numberOfThreads = 1;
        auto value = options.find("speedup");
        if (value != options.end())
            numberOfThreads = stoi(value->second);
        
        // Create threads for scanning
        vector<pthread_t> threads;
        char ip[INET_ADDRSTRLEN];
        getCurrentSystemIP(ip);
        
        for (int i = 0; i < numberOfThreads; i++) {
            pthread_t thread;
            if (pthread_create(&thread, NULL, sendPacket, (void *)ip) != 0) {
                cerr << "Failed to create thread" << endl;
                return -1;
            }
            threads.push_back(thread);
        }
        
        // Wait for all threads to complete
        for (size_t i = 0; i < threads.size(); i++) {
            pthread_join(threads[i], NULL);
        }
        
        // Print results
        time_t end = time(NULL);
        cout << "\nScanning completed in " << (end - start) << " seconds" << endl;
        printJobStats();
        
        // Cleanup
        destroyJobQueue();
        optionsManager::Instance()->deleteSingleTon();
        
        return 0;
    }
    catch (const exception &e) {
        cerr << "Error during scanning: " << e.what() << endl;
        return -1;
    }
}

int run_port_scan(const char *ip, const char *ports, const char *scan_type, int threads) {
    if (!ip || !ports) return -1;
    
    map<string, string> options;
    options["ip"] = string(ip);
    options["ports"] = string(ports);
    if (scan_type) {
        options["scan"] = string(scan_type);
    }
    if (threads > 1) {
        options["speedup"] = to_string(threads);
    }
    
    return execute_scan(options);
}

int run_port_scan_prefix(const char *prefix, const char *ports, const char *scan_type, int threads) {
    if (!prefix || !ports) return -1;
    
    map<string, string> options;
    options["prefix"] = string(prefix);
    options["ports"] = string(ports);
    if (scan_type) {
        options["scan"] = string(scan_type);
    }
    if (threads > 1) {
        options["speedup"] = to_string(threads);
    }
    
    return execute_scan(options);
}

int run_port_scan_file(const char *filename, const char *ports, const char *scan_type, int threads) {
    if (!filename || !ports) return -1;
    
    map<string, string> options;
    options["file"] = string(filename);
    options["ports"] = string(ports);
    if (scan_type) {
        options["scan"] = string(scan_type);
    }
    if (threads > 1) {
        options["speedup"] = to_string(threads);
    }
    
    return execute_scan(options);
}
