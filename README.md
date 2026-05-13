# Leopard-Watchdog 🐆

A modular security and system health check pipeline for Pardus GNU/Linux systems.

## Overview

Leopard-Watchdog is a lightweight CLI/GUI application that automatically scans Pardus systems, detects security risks and health issues, and provides clear diagnostic reports with actionable recommendations.

## Features

### 1. 📦 Old & Outdated APT Package Detection
- Analyzes local package database
- Compares installed packages with repository information
- Identifies old, unsupported, or deprecated packages
- Marks critical security updates
- Provides safe upgrade suggestions

### 2. 🔌 Open Port Checking
- Scans active network ports and connected services
- Highlights unexpected or risky ports
- Categorizes findings by risk level (HIGH, NORMAL, LOW)
- Provides security hardening recommendations

### 3. 🔧 Unknown Hardware & Software Detection
- Inventories connected hardware devices
- Catalogs installed software
- Marks unusual, unsigned, or rare components
- Detects misconfigurations and unauthorized devices
- Identifies suspicious software

### 4. 🏥 System Health & Diagnostics Module
- Comprehensively monitors CPU, memory, disk status
- Tracks failed services and critical logs
- Generates unified system health score
- Provides actionable recommendations for optimization

## Project Structure

```
Leopard-Watchdog/
├── CMakeLists.txt
├── README.md
├── TODO.md                      # Development plan
├── src/
│   ├── common.h                 # Common definitions & utilities
│   ├── log.c                    # Logging module
│   ├── main.c                   # Main CLI entry point
│   ├── config.c                 # Configuration management
│   ├── report.c                 # Report generation
│   │
│   ├── apt_audit.c              # Module 1: APT Package Audit
│   ├── port_scanner.c           # Module 2: Port Scanner
│   ├── hardware_inventory.c     # Module 3: Hardware/Software Inventory
│   ├── system_health.c          # Module 4: System Health Check
│   │
│   ├── database.c               # Cache/database operations
│   └── ui.c                     # CLI/UI components
│
├── headers/                     # Module headers
├── tests/                       # Unit tests (future)
└── docs/                        # Documentation (future)
```

## Building

```bash
mkdir build && cd build
cmake ..
make
./leopard-watchdog
```

## Usage

```bash
# Run full system audit
./leopard-watchdog

# Specific checks
./leopard-watchdog --apt-audit
./leopard-watchdog --port-check
./leopard-watchdog --hardware-check
./leopard-watchdog --health-check

# Generate report
./leopard-watchdog --report json  # JSON format
./leopard-watchdog --report html  # HTML format
```

## Author

**Vugar Ahadli**  
Contact: vuqarahadli17@gmail.com  
Copyright (c) 2026

## License

TBD - Pardus-compatible open-source license
