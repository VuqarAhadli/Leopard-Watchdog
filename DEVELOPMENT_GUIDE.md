# Leopard-Watchdog Development Quick Start

## 🚀 Getting Started

### Build the Project
```bash
cd /home/vugar/Projects/Leopard-Watchdog
mkdir -p build && cd build
cmake ..
make
./leopard-watchdog --help
```

### Project Files at a Glance

| File | Purpose | Lines | Status |
|------|---------|-------|--------|
| **README.md** | Project overview and usage | - | ✅ Complete |
| **TODO.md** | Development plan with 100+ items | - | ✅ Complete |
| **PROJECT_STRUCTURE.md** | Architecture documentation | - | ✅ Complete |
| **REFINEMENT_SUMMARY.md** | What was done summary | - | ✅ Complete |
| **CMakeLists.txt** | Build configuration | 50 | ✅ Complete |
| **src/main.c** | CLI entry point | ~200 | 🟡 Stub |
| **src/log.c** | Logging utilities | 120 | ✅ Working |
| **src/common.h** | Types & constants | 80 | ✅ Complete |
| **src/config.c** | Config management | 40 | 🟡 Stub |
| **src/report.c** | Report generation | 50 | 🟡 Stub |
| **src/database.c** | Caching layer | 50 | 🟡 Stub |
| **src/ui.c** | CLI interface | 80 | ✅ Basic |
| **src/apt_audit.c** | APT package check | 60 | 🟡 Stub |
| **src/port_scanner.c** | Port security scan | 60 | 🟡 Stub |
| **src/hardware_inventory.c** | Device inventory | 50 | 🟡 Stub |
| **src/system_health.c** | Health monitoring | 100 | 🟡 Stub |

Legend: ✅ = Fully Implemented | 🟡 = Framework/Stub | 🔴 = Not Started

---

## 📋 Implementation Checklist

### Phase 1: Foundation (Priority: CRITICAL)
```
Core Framework
  [ ] Complete logging framework (log levels, file output)
  [ ] Configuration file parsing (INI/JSON format)
  [ ] Report generation (JSON, text, HTML exports)
  [ ] CLI argument parsing (getopt)
  [ ] Database/caching system
```

### Phase 2: APT Audit (Priority: HIGH)
```
  [ ] Parse /var/lib/apt/lists/ files
  [ ] Compare package versions with repositories
  [ ] Detect deprecated packages
  [ ] Identify security updates
  [ ] Generate upgrade recommendations
```

### Phase 3: Port Scanner (Priority: HIGH)
```
  [ ] Parse /proc/net/tcp and /proc/net/udp
  [ ] Map ports to services and PIDs
  [ ] Risk level classification
  [ ] Hardening recommendations
  [ ] Firewall rule generation
```

### Phase 4-5: Inventory & Health (Priority: MEDIUM)
```
  [ ] Hardware enumeration (CPU, GPU, disks, USB)
  [ ] Software catalog (installed packages)
  [ ] Anomaly detection
  [ ] CPU/memory/disk monitoring
  [ ] Health score calculation
```

---

## 💡 Key Functions to Implement

### For Each Module, Complete These Functions:
```c
int   module_init(Module_t *m);           // Initialize module
int   module_run(Module_t *m);            // Execute the check
void  module_print_results(Module_t *m);  // Display results
void  module_cleanup(Module_t *m);        // Free resources
```

### Example Implementation Pattern:
```c
// In apt_audit.c
int apt_audit_run(AptAudit_t *audit) {
    // TODO: 1. Read package database
    // TODO: 2. Parse version info
    // TODO: 3. Compare with repos
    // TODO: 4. Assess risks
    // TODO: 5. Generate recommendations
    
    LOG(NORMAL, "Audit completed");
    return 0;
}
```

---

## 🔍 Where to Find Things

**Need to understand the architecture?**
- Read: `PROJECT_STRUCTURE.md` (architecture overview)
- Read: `README.md` (feature descriptions)

**Need specific TODO items?**
- Read: `TODO.md` (100+ actionable tasks, prioritized by phase)

**Want to see compilation status?**
- Run: `make` in the build directory
- Check: `./leopard-watchdog --help`

**Need to add a new function?**
- Add declaration in `headers/module_name.h`
- Implement in `src/module_name.c`
- Update `CMakeLists.txt` if adding new file

---

## 🎯 Development Workflow

### For Each Feature/Task:
1. **Read** the corresponding section in `TODO.md`
2. **Find** the stub function in `src/module_name.c`
3. **Implement** the TODO comments
4. **Replace** the stub with real code
5. **Test** with `./leopard-watchdog`
6. **Rebuild** with `make clean && make`

### Testing Quick Commands:
```bash
# Build
cd build && make

# Run full audit
./leopard-watchdog

# Run with verbose output
./leopard-watchdog --verbose

# Generate JSON report
./leopard-watchdog --report json

# Help
./leopard-watchdog --help
```

---

## 📝 Code Style Guidelines

- **Language**: C11 (POSIX compliant)
- **Functions**: snake_case (e.g., `apt_audit_run`)
- **Types**: PascalCase with _t suffix (e.g., `AptAudit_t`)
- **Constants**: UPPERCASE (e.g., `CPU_WARN`)
- **Comments**: Doxygen-style for public functions
- **Logging**: Use `LOG(LEVEL, "message")` for all output

### Example:
```c
/**
 * Check for outdated packages
 * Returns number of outdated packages, -1 on error
 */
int apt_check_updates(AptAudit_t *audit) {
    if (!audit) return -1;
    
    LOG(NORMAL, "Checking for package updates");
    
    /* Implementation here */
    
    return audit->count;
}
```

---

## 🐛 Debugging Tips

1. **Add logging**: Use `LOG(HIGH, "debug message")`
2. **Check includes**: Verify all headers are in `#include` statements
3. **Build errors**: Run `make clean && make` to rebuild everything
4. **Linker errors**: Ensure functions are declared in headers
5. **Runtime errors**: Use `strace ./leopard-watchdog` to trace system calls

---

## 📚 Reference Documentation

- **TODO.md**: Full development plan (read first!)
- **PROJECT_STRUCTURE.md**: Architecture and design principles
- **README.md**: User-facing documentation
- **common.h**: Type definitions and constants
- **headers/*.h**: Module interfaces

---

## 🎓 Learning Resources

### Understanding the System:
- `/proc/stat` - CPU statistics
- `/proc/meminfo` - Memory information
- `/proc/net/tcp`, `/proc/net/udp` - Network sockets
- `/var/lib/apt/lists/` - APT database
- `/var/lib/dpkg/status` - Installed packages

### Linux Tools Referenced:
- `lspci` - List PCI devices
- `lsusb` - List USB devices
- `netstat` / `ss` - Socket statistics
- `systemctl` - Service management
- `journalctl` - System logs

---

## 🎉 Milestones

- **Week 1-2**: Complete Phase 1 (foundation) → Executable runs
- **Week 2-3**: Complete Phase 2-3 (audit modules) → Full scan works
- **Week 4-5**: Complete Phase 4-5 (inventory/health) → All modules functional
- **Week 6+**: Polish, testing, and advanced features

---

## 📞 Questions?

Refer to the TODO.md file for implementation details. Each TODO item has:
- Specific description of what to implement
- Expected function/file location
- Related system files or commands
- Notes on Pardus-specific handling

---

**Last Updated**: 14 May 2026  
**Status**: ✅ Ready for Development
