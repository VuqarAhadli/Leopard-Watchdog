# Project Structure Overview

## Directory Layout

```
Leopard-Watchdog/
│
├── CMakeLists.txt              # Build configuration
├── README.md                   # Project overview and usage
├── TODO.md                     # Comprehensive development plan with TODO items
├── PROJECT_STRUCTURE.md        # This file
│
├── src/                        # Source code
│   ├── main.c                  # Main entry point and CLI interface
│   ├── common.h                # Common definitions, constants, and types
│   ├── log.c                   # Logging module with timestamps and levels
│   │
│   ├── config.c                # Configuration management (loads from /etc and ~/)
│   ├── report.c                # Report generation (JSON, text, HTML)
│   ├── database.c              # Caching and database operations
│   ├── ui.c                    # CLI UI components and formatting
│   │
│   ├── apt_audit.c             # Module 1: APT Package Audit
│   ├── port_scanner.c          # Module 2: Network Port Scanner
│   ├── hardware_inventory.c    # Module 3: Hardware/Software Inventory
│   └── system_health.c         # Module 4: System Health & Diagnostics
│
└── headers/                    # Header files (mirrors src/ structure)
    ├── config.h
    ├── report.h
    ├── database.h
    ├── ui.h
    ├── apt_audit.h
    ├── port_scanner.h
    ├── hardware_inventory.h
    └── system_health.h
```

## Module Architecture

### Core Infrastructure (Foundation)
- **common.h/log.c**: Logging framework with ANSI colors
- **config.c**: Configuration management
- **report.c**: Multi-format report generation
- **database.c**: Caching and data persistence
- **ui.c**: CLI interface components

### Audit Modules (4-Phase Pipeline)
1. **apt_audit.c**: Old/outdated APT package detection
2. **port_scanner.c**: Open port enumeration and security assessment
3. **hardware_inventory.c**: Hardware/software inventory and anomaly detection
4. **system_health.c**: CPU/memory/disk monitoring and health scoring

### Main Application
- **main.c**: CLI entry point, argument parsing, module orchestration

## Build System

The project uses **CMake 3.16+** for cross-platform build configuration:

```bash
mkdir build && cd build
cmake ..
make
./leopard-watchdog --help
```

**Compiler Flags:**
- Standard: C11
- Warnings: `-Wall -Wextra -Wpedantic -Wformat=2`
- Optimization: `-O3` (Release) / `-g -O0` (Debug)
- Libraries: `libm` (math library)

## Development Workflow

### Phase-Based Development
Each phase adds complete modules with all dependencies:

1. **Phase 1**: Foundation & Infrastructure (logging, config, CLI)
2. **Phase 2**: APT Audit Module 
3. **Phase 3**: Port Scanner Module
4. **Phase 4**: Hardware/Software Inventory
5. **Phase 5**: System Health & Diagnostics
6. **Phase 6**: Caching & Data Management
7. **Phase 7**: Report Generation & UI Refinement
8. **Phase 8**: Documentation & Testing
9. **Phase 9**: Advanced Features

See **TODO.md** for detailed task breakdown and dependencies.

## Key Design Principles

1. **Modularity**: Each audit module is independent and testable
2. **Performance**: Lightweight, low resource footprint
3. **Security**: Minimal dependencies, no network requests without configuration
4. **Pardus-First**: Optimized for Pardus GNU/Linux specifics
5. **Extensibility**: Plugin architecture for future enhancements

## Integration Points

### System Files Read
- `/proc/stat` - CPU statistics
- `/proc/meminfo` - Memory information
- `/proc/cpuinfo` - Processor details
- `/proc/net/tcp`, `/proc/net/tcp6`, `/proc/net/udp`, `/proc/net/udp6` - Active ports
- `/var/lib/apt/lists/` - APT package database
- `/var/lib/dpkg/status` - Installed packages
- `/etc/services` - Service name mapping
- `/var/log/syslog` - System logs
- `lspci`, `lsusb` output - Hardware enumeration

### Configuration Locations
- `/etc/leopard-watchdog/config.conf` - System-wide configuration
- `~/.config/leopard-watchdog/` - User-specific settings

### Cache Locations
- `/var/cache/leopard-watchdog/` - System cache (needs root or sudo)
- `~/.cache/leopard-watchdog/` - User cache

## Performance Targets

- Full audit scan: < 5 seconds on typical systems
- Memory footprint: < 20 MB
- CPU impact: Minimal during scanning
- Report generation: < 1 second

## Testing Strategy

- Unit tests for each module (future)
- Integration tests for module interactions
- Performance benchmarks
- Pardus-specific regression tests

## Security Considerations

1. Requires root/sudo for full functionality
2. Validates all file inputs
3. Minimal external dependencies
4. No network communication (configurable)
5. Safe handling of sensitive system information

## Future Enhancements

- GUI interface (GTK+)
- Web dashboard
- Real-time monitoring
- Automated security patching
- dbus/systemd integration
- SELinux/AppArmor profile generation
- Scheduled automated scans
