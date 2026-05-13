# Leopard-Watchdog Development TODO

## Project Phases

### Phase 1: Foundation & Infrastructure (Priority: CRITICAL)

#### Core Framework
- [ ] **Setup logging framework**
  - [ ] Implement structured logging with timestamps
  - [ ] Support multiple log levels (DEBUG, INFO, WARN, ERROR)
  - [ ] Add file and console output options
  - [ ] TODO: Extend `log.c` with log levels and file output

- [ ] **Implement configuration management (`config.c`)**
  - [ ] Load config from `/etc/leopard-watchdog/config.conf`
  - [ ] Support user config in `~/.config/leopard-watchdog/`
  - [ ] Parse and validate configuration parameters
  - [ ] TODO: Define config file schema and parsing logic

- [ ] **Create report generation module (`report.c`)**
  - [ ] Design report data structure
  - [ ] Implement JSON export
  - [ ] Implement plain-text report format
  - [ ] TODO: Add HTML and XML export formats (Phase 2)

- [ ] **Implement CLI argument parsing (`main.c`)**
  - [ ] Parse command-line arguments (getopt)
  - [ ] Handle flags: `--verbose`, `--quiet`, `--report`
  - [ ] Support module selection flags
  - [ ] TODO: Add interactive CLI menu (Phase 2)

#### Build System Improvements
- [ ] **Enhance CMakeLists.txt**
  - [ ] Add all module source files
  - [ ] Set up compiler flags and optimization levels
  - [ ] Add `DCMAKE_BUILD_TYPE=Release` for production builds
  - [ ] Create library targets for reusable modules

---

### Phase 2: Module 1 - APT Package Audit (Priority: HIGH)

#### APT Audit Module Implementation (`apt_audit.c`)
- [ ] **Parse APT package database**
  - [ ] Read `/var/lib/apt/lists/` files
  - [ ] Extract installed package information
  - [ ] Parse version and source repository data
  - [ ] TODO: Handle multi-release (stable/testing/unstable)

- [ ] **Detect outdated packages**
  - [ ] Compare installed versions with repository versions
  - [ ] Identify packages from removed/archived repositories
  - [ ] Flag packages with no available updates
  - [ ] TODO: Build Pardus-specific version comparison logic

- [ ] **Identify deprecated packages**
  - [ ] Check against Pardus package deprecation list
  - [ ] Mark packages with security advisories
  - [ ] Flag packages with known vulnerabilities
  - [ ] TODO: Integrate with CVE databases

- [ ] **Generate security recommendations**
  - [ ] Create safe upgrade paths
  - [ ] Identify security-critical updates
  - [ ] Suggest package removals for obsolete software
  - [ ] TODO: Add dependency conflict detection

- [ ] **Performance optimization**
  - [ ] Cache package database results
  - [ ] Implement incremental updates
  - [ ] TODO: Add background refresh capabilities

---

### Phase 3: Module 2 - Open Port Checking (Priority: HIGH)

#### Port Scanner Module Implementation (`port_scanner.c`)
- [ ] **Enumerate active ports**
  - [ ] Parse `/proc/net/tcp`, `/proc/net/udp`, `/proc/net/tcp6`, `/proc/net/udp6`
  - [ ] Map ports to process IDs (PIDs)
  - [ ] Resolve service names from `/etc/services`
  - [ ] TODO: Support raw socket scanning as fallback

- [ ] **Associate services with ports**
  - [ ] Read `/proc/[pid]/cmdline` to identify services
  - [ ] Extract executable paths and arguments
  - [ ] Collect service metadata
  - [ ] TODO: Query systemd for service status

- [ ] **Risk assessment**
  - [ ] Mark common attack vectors (SSH, RDP, etc.)
  - [ ] Flag non-standard port usage
  - [ ] Categorize by risk level (HIGH, NORMAL, LOW)
  - [ ] TODO: Build risk profile database

- [ ] **Hardening recommendations**
  - [ ] Suggest firewall rules via iptables/ufw
  - [ ] Recommend service disabling for unused ports
  - [ ] Provide bind address restrictions
  - [ ] TODO: Generate systemd socket unit files

- [ ] **Filtering capabilities**
  - [ ] Allow whitelist/blacklist configuration
  - [ ] Support port range filtering
  - [ ] TODO: Save port profiles for different system roles

---

### Phase 4: Module 3 - Hardware & Software Inventory (Priority: MEDIUM)

#### Hardware/Software Inventory Module (`hardware_inventory.c`)
- [ ] **Hardware enumeration**
  - [ ] Parse `/proc/cpuinfo` for CPU information
  - [ ] Read `/proc/meminfo` for memory details
  - [ ] Enumerate PCI devices via `lspci` or `/proc/bus/pci`
  - [ ] Enumerate USB devices via `lsusb` or `/proc/bus/usb`
  - [ ] TODO: Include disk and partition information

- [ ] **Software catalog**
  - [ ] Generate full installed package list
  - [ ] Extract package signatures and checksums
  - [ ] Identify unsigned packages
  - [ ] Collect package metadata (maintainer, source)
  - [ ] TODO: Monitor installed snap/flatpak packages

- [ ] **Anomaly detection**
  - [ ] Compare against known device database
  - [ ] Flag rare or suspicious hardware combinations
  - [ ] Identify unsigned or unverified packages
  - [ ] Detect unknown kernel modules
  - [ ] TODO: Build historical database for change detection

- [ ] **Configuration audit**
  - [ ] Check BIOS/UEFI settings accessibility
  - [ ] Verify device permissions and ownership
  - [ ] Scan for unauthorized device mounts
  - [ ] TODO: Check for rootkit signatures

- [ ] **Report generation**
  - [ ] Export hardware inventory to JSON/CSV
  - [ ] Generate change logs (delta reports)
  - [ ] TODO: Visual hardware topology diagrams

---

### Phase 5: Module 4 - System Health & Diagnostics (Priority: MEDIUM)

#### System Health Module Implementation (`system_health.c`)
- [ ] **CPU monitoring**
  - [ ] Read `/proc/stat` for CPU metrics
  - [ ] Calculate usage percentage
  - [ ] Track process CPU consumption
  - [ ] Detect high-CPU processes
  - [ ] TODO: Monitor thermal information

- [ ] **Memory analysis**
  - [ ] Parse `/proc/meminfo` for detailed breakdown
  - [ ] Calculate available vs. used memory
  - [ ] Identify memory leaks via process RSS tracking
  - [ ] TODO: Monitor page faults and swap usage

- [ ] **Disk health check**
  - [ ] Parse `df` output for partition usage
  - [ ] Monitor inode exhaustion
  - [ ] Check filesystem integrity with fsck info
  - [ ] TODO: SMART status for HDDs, health for SSDs

- [ ] **Service monitoring**
  - [ ] Query systemd for failed services
  - [ ] Track service restart counts
  - [ ] Collect error logs from failed services
  - [ ] TODO: Analyze journal entries for errors

- [ ] **Log analysis**
  - [ ] Parse `/var/log/syslog` and system logs
  - [ ] Extract critical errors and warnings
  - [ ] Categorize by severity
  - [ ] Track recurring issues
  - [ ] TODO: Implement log anomaly detection

- [ ] **Health score calculation**
  - [ ] Define weighted scoring system
  - [ ] Combine CPU, memory, disk metrics
  - [ ] Factor in service health
  - [ ] Calculate overall system health (0-100)
  - [ ] TODO: Trend analysis and historical scoring

- [ ] **Recommendations engine**
  - [ ] Generate actionable health recommendations
  - [ ] Prioritize by impact and urgency
  - [ ] Provide optimization suggestions
  - [ ] TODO: Predict future issues

---

### Phase 6: Data Management & Caching (Priority: MEDIUM)

#### Database Module (`database.c`)
- [ ] **Design cache structure**
  - [ ] Define data serialization format
  - [ ] Choose storage backend (sqlite, JSON files, or binary)
  - [ ] TODO: Implement versioning and migration

- [ ] **Implement caching**
  - [ ] Cache package database results
  - [ ] Store historical system metrics
  - [ ] Track changes over time
  - [ ] TODO: Automatic cache expiration

- [ ] **Change tracking**
  - [ ] Record deltas between scans
  - [ ] Generate change reports
  - [ ] Alert on significant changes
  - [ ] TODO: Audit trail for compliance

---

### Phase 7: User Interface & Reporting (Priority: MEDIUM)

#### CLI Interface (`ui.c`)
- [ ] **Basic CLI output**
  - [ ] Implement table formatting for results
  - [ ] Add color-coded status indicators
  - [ ] Support verbose and quiet modes
  - [ ] TODO: Progress bars for long operations

- [ ] **Interactive menus**
  - [ ] Create main menu system
  - [ ] Allow module selection
  - [ ] Implement filter options
  - [ ] TODO: Full TUI with ncurses (Phase 3)

#### Report Generation (`report.c`)
- [ ] **Multi-format support**
  - [ ] JSON export with complete data
  - [ ] Plain-text summaries
  - [ ] TODO: HTML with interactive visualizations
  - [ ] TODO: PDF with charts and graphs
  - [ ] TODO: Email integration for automated reports

- [ ] **Report customization**
  - [ ] Template-based generation
  - [ ] Filtering and sections selection
  - [ ] Custom thresholds in reports
  - [ ] TODO: Scheduled report generation

---

### Phase 8: Documentation & Testing (Priority: LOW)

#### Documentation
- [ ] Write module-specific documentation
- [ ] Create API documentation with doxygen comments
- [ ] User guide and quick-start
- [ ] TODO: Man pages for CLI interface
- [ ] TODO: Architecture and design documents

#### Testing
- [ ] Unit tests for core functions
- [ ] Integration tests for module interactions
- [ ] TODO: Regression test suite
- [ ] TODO: Performance benchmarks

---

### Phase 9: Advanced Features (Priority: LOW)

#### Security Hardening
- [ ] [ ] TODO: SELinux/AppArmor profile creation
- [ ] TODO: Automatic security patches application
- [ ] TODO: Firewall rule generation and application

#### GUI Development
- [ ] TODO: GTK+ based GUI (future)
- [ ] TODO: Web-based dashboard (future)
- [ ] TODO: Real-time monitoring interface

#### System Integration
- [ ] TODO: Systemd service unit file
- [ ] TODO: Cron/timer-based automatic scanning
- [ ] TODO: dbus interface for privileged operations
- [ ] TODO: Logging to systemd journal

---

## Critical Path (Minimum Viable Product)

**Deliverables for MVP:**
1. ✅ Logging framework (Phase 1)
2. ✅ CLI argument parsing (Phase 1)
3. ✅ APT audit module (Phase 2)
4. ✅ Port scanner module (Phase 3)
5. ✅ System health module (Phase 5)
6. ✅ Basic report generation (Phase 6)
7. ✅ CLI interface (Phase 7)

**Target MVP**: End of Phase 7

---

## Notes

- All modules must be Pardus-specific where applicable
- Ensure compatibility with Pardus package formats and repositories
- Maintain low resource footprint for lightweight systems
- Design for security: minimal external dependencies, no network requests unless configured
- Each module should be independently testable and usable
- Performance: Aim for &lt;5 second full scan on typical systems
