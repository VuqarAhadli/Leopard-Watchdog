# Pardus Health Check - Build & Installation Guide

## Project Structure

```
pardus-health-check/
├── src/                           # Implementation files
│   ├── main.c                     # Main program, CLI argument parsing, module coordination
│   ├── logger.c                   # Centralized logging with color output
│   ├── ui.c                       # Banner, UI components, formatted output
│   ├── system_health.c            # CPU/Memory/Disk/Services health checks
│   ├── package_analyzer.c         # APT package security analysis
│   ├── port_scanner.c             # Network port enumeration and risk assessment
│   ├── hardware_inventory.c       # Hardware/device enumeration and validation
│   └── report_generator.c         # Report generation (CLI/JSON/HTML)
├── include/
│   └── pardus_health_check.h      # Header file with structures and function declarations
├── debian/
│   └── control                    # Debian package metadata
├── CMakeLists.txt                 # Build configuration
├── pardus-health-check.1          # Man page for documentation
├── README.md                       # User documentation
└── LICENSE                         # MIT License

## Build Instructions

### 1. From Source

```bash
# Install build dependencies
sudo apt update
sudo apt install build-essential cmake git

# Clone repository
git clone https://github.com/pardus/pardus-health-check.git
cd pardus-health-check

# Create build directory
mkdir -p build && cd build

# Configure and build
cmake ..
make -j$(nproc)

# Install (optional)
sudo make install
```

### 2. Create Debian Package

```bash
# In the build directory
cpack -G DEB

# Or manually with dpkg-buildpackage
cd ..
dpkg-buildpackage -us -uc

# Install the package
sudo dpkg -i ../pardus-health-check_1.0.0_amd64.deb
```

### 3. Direct Installation from Build

```bash
cd build
sudo make install
```

## Compilation Details

### Compiler Flags

```
- C Standard: C11 (-std=c11)
- Optimization: O2 (-O2)
- Warnings: Wall, Wextra (-Wall -Wextra)
- Platform: Linux x86_64, ARM (auto-detected)
```

### Dependencies

**Runtime:**
- libc (standard C library)
- Core Pardus utilities: apt, systemd, dpkg

**Build:**
- gcc or clang
- cmake >= 3.10
- make

**Optional (for full features):**
- dmidecode (BIOS info)
- lsusb (USB device listing)
- lspci (PCI device listing)
- net-tools or iproute2 (for netstat/ss)

## Module Details

### 1. logger.c / ui.c
- Provides centralized logging
- Terminal color support (ANSI codes)
- ASCII art banner for Pardus branding
- Progress indicators and formatted output

### 2. system_health.c
- Reads /proc/stat for CPU usage
- Reads /proc/meminfo for memory status
- Checks disk usage via df command
- Monitors systemd service failures
- Tracks thresholds: CPU >75%, Mem >80%, Disk >90% critical

### 3. package_analyzer.c
- Checks for upgradable packages via apt
- Identifies deprecated packages (python2, old apache2, mysql-server)
- Detects broken dependencies via apt check
- Recommends security updates

### 4. port_scanner.c
- Uses ss or netstat to enumerate listening ports
- Maps ports against known service database
- Risk scoring: critical (RDP), high (MongoDB, Redis), medium (databases)
- Flags excessive open ports (>10 indicates large attack surface)

### 5. hardware_inventory.c
- Lists USB devices (lsusb)
- Counts kernel modules
- Detects unknown PCI devices
- Suggests BIOS/UEFI updates
- Monitors removable storage

### 6. report_generator.c
- Generates formatted CLI output with colors
- Produces structured JSON for integration
- Creates styled HTML for web viewing
- Calculates risk scores (0-100) per module
- Provides actionable recommendations

## Performance Characteristics

- **Execution Time:** ~5-10 seconds for full scan
- **Memory Usage:** ~5-10 MB
- **Disk I/O:** Minimal (reading /proc, running system commands)
- **Network:** None (local analysis only)
- **CPU Usage:** Briefly spikes during scans, negligible otherwise

## Configuration & Customization

### Threshold Values (in source)

Edit these constants for different alert thresholds:

```c
#define CPU_WARN_THRESHOLD  75    // CPU warning threshold (%)
#define MEM_WARN_THRESHOLD  80    // Memory warning threshold (%)
#define DISK_CRIT_THRESHOLD 90    // Disk critical threshold (%)
#define DISK_WARN_THRESHOLD 80    // Disk warning threshold (%)
```

### Adding New Modules

To add a new check module:

1. Create `src/new_module.c`:
```c
int module_new_check(Finding_t **findings, int *count) {
    // Allocate findings array
    *findings = malloc(10 * sizeof(Finding_t));
    
    // Populate findings
    // Increment *count for each finding
    
    return 0; // or -1 on error
}
```

2. Add to `include/pardus_health_check.h`:
```c
int module_new_check(Finding_t **findings, int *count);
```

3. Update `CMakeLists.txt`:
```cmake
add_executable(pardus-health-check
    ... other files ...
    src/new_module.c
)
```

4. Call from `main.c`:
```c
if (opts.check_new) {
    Finding_t *findings = NULL;
    int count = 0;
    if (module_new_check(&findings, &count) == 0 && findings) {
        // Process findings
    }
    free(findings);
}
```

## Testing

### Manual Testing

```bash
# Test CLI output
./pardus-health-check --all

# Test JSON output
./pardus-health-check --json /tmp/test.json
cat /tmp/test.json | jq .

# Test HTML output
./pardus-health-check --html /tmp/test.html
firefox /tmp/test.html

# Test individual modules
./pardus-health-check --packages
./pardus-health-check --network
./pardus-health-check --hardware
./pardus-health-check --system
```

### Debugging

```bash
# Enable verbose logging
./pardus-health-check --all --verbose

# Check system log
tail -f /tmp/pardus-health-check.log
```

## Distribution & Packaging

### For Pardus Repository

1. Update version in CMakeLists.txt
2. Create git tag: `git tag v1.0.0`
3. Build Debian package
4. Submit to Pardus package maintainer
5. Package will be available via `apt install pardus-health-check`

### For PPA/External Distribution

The .deb package is self-contained and can be distributed via:
- Email
- Web server
- USB drive
- PPA (Personal Package Archive)

## Troubleshooting

### Build Failures

**CMake not found:**
```bash
sudo apt install cmake
```

**Compilation errors:**
- Ensure gcc/clang is installed: `sudo apt install build-essential`
- Check C11 support: `gcc --version` (should be GCC 4.9+)

**Missing dependencies:**
```bash
sudo apt install apt-dev systemd-dev
```

### Runtime Issues

**Permission denied errors:**
- Many checks require sudo: `sudo pardus-health-check --all`
- Or add user to sudoers for passwordless execution

**Port scanning fails:**
- Ensure `ss` or `netstat` is available
- Install: `sudo apt install net-tools` or `sudo apt install iproute2`

**No module found errors:**
- Check installation: `which pardus-health-check`
- Reinstall: `cd build && sudo make install`

## License & Contributing

MIT License - Open source and welcoming contributions.

For bug reports and feature requests: GitHub Issues
For development: Submit pull requests

---

**Built with ❤️ for Pardus Linux Community**
