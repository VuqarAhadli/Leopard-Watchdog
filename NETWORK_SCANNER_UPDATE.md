# Port Scanner Integration - Update Summary

## Changes Made

### ✅ Completed
1. **Integrated C++ Port Scanner**
   - Replaced old stub `port_scanner.c` with new C++ implementation
   - Port scanner now located in `src/port-scanner/` directory
   - Features TCP, UDP, SYN, FIN, NULL, XMAS scan types

2. **Updated CLI Interface**
   - Removed `--port-check` flag
   - Added `--network [OPTIONS]` flag for network scanning
   - Port scanner arguments passed directly through

3. **Build System Integration**
   - Updated `CMakeLists.txt` to enable C++11 support
   - Added C/C++ mixed language compilation
   - Integrated all port-scanner source files
   - Added pthread library dependency

4. **Cleaned Up**
   - Removed old `src/port_scanner.c` (stub)
   - Removed old `headers/port_scanner.h`
   - Renamed `main()` → `port_scanner_main()` in portScanner.cpp

---

## Usage

### View Help
```bash
./leopard-watchdog --help
./leopard-watchdog --network -h
```

### Port Scanner Options
```bash
# Scan specific IP and port range
./leopard-watchdog --network --ip 192.168.1.1 --ports 1-1000

# Scan with multiple threads (faster)
./leopard-watchdog --network --ip 192.168.1.1 --ports 22,80,443 --speedup 10

# Scan subnet with CIDR notation
./leopard-watchdog --network --prefix 192.168.1.0/24 --ports 1-65535

# Scan from file containing IP addresses
./leopard-watchdog --network --file ips.txt --ports 1-1000

# Different scan types
./leopard-watchdog --network --ip 192.168.1.1 --ports 80 --scan SYN UDP NULL

# Full example with all options
./leopard-watchdog --network --ip 192.168.1.1 --ports 1,80,443-445 --speedup 20
```

---

## Full Audit Behavior

When running `--full-audit`:
- **APT Audit**: ✅ Executes normally
- **Network Scan**: Displays help text with usage instructions (must use `--network` flag separately)
- **Hardware Inventory**: ✅ Executes normally
- **System Health**: ✅ Executes normally

---

## Port Scanner Capabilities

The integrated port scanner can:
- ✅ Perform TCP/UDP scans (SYN, FIN, NULL, XMAS)
- ✅ Identify open, closed, filtered ports
- ✅ Detect service names and versions
- ✅ Scan multiple IPs with parallel threads
- ✅ Support IP ranges, CIDR notation, and file input
- ✅ Handle multi-threaded concurrent scanning

---

## Technical Details

### C/C++ Integration
- Main application remains in C
- Port scanner implemented in C++11
- Shared linkage through `port_scanner_main()` function
- Pthread library handles multithreading

### File Structure
```
src/port-scanner/
├── portScanner.cpp       (Main entry point)
├── optionsManager.cpp    (CLI argument parsing)
├── TCPUtilities.cpp      (TCP packet handling)
├── UDPUtilities.cpp      (UDP packet handling)
├── CommonUtilities.cpp   (Shared functions)
├── Job.cpp               (Job queue management)
├── [.h files]            (Headers)
└── README.md             (Original documentation)
```

### Compilation
- C11 standard for C modules
- C++11 standard for C++ modules
- O3 optimization in Release mode
- All warnings enabled (-Wall -Wextra -Wpedantic)

---

## Example Output

```
$ ./leopard-watchdog --network --ip 127.0.0.1 --ports 22,80,443
Scanning......

-----------Port Scan Stats-----------

IP Address: 127.0.0.1

Open Ports: 
Port    Service Name               Results                                    Version         Conclusion
...
```

---

## Requirements

- Linux/Pardus system with:
  - Raw socket access (may need sudo for some scans)
  - Threading support (pthread)
  - g++ compiler with C++11 support

---

## Next Steps

1. Test network scanning with actual targets
2. Add result filtering and reporting
3. Integrate port scanner results into audit reports
4. Optimize thread pool management
5. Add firewall bypass detection

---

**Last Updated**: 14 May 2026
