cmake_minimum_required(VERSION 3.16)

project(Leopard-Watchdog C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Main executable
add_executable(leopard-watchdog
    src/main.c
    src/log.c
    src/config.c
    src/report.c
    src/database.c
    src/ui.c
    src/apt_audit.c
    src/port_scanner.c
    src/hardware_inventory.c
    src/system_health.c
)

target_include_directories(leopard-watchdog PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/headers
)

# Compiler warnings (important for system tools)
target_compile_options(leopard-watchdog PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wformat=2
    -Wno-unused-parameter
)

# Optimization flags
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(leopard-watchdog PRIVATE -O3)
else()
    target_compile_options(leopard-watchdog PRIVATE -g -O0)
endif()

# Required system libraries
target_link_libraries(leopard-watchdog PRIVATE
    m  # Math library
)

# Installation rules (future)
# install(TARGETS leopard-watchdog DESTINATION /usr/local/bin)
# install(FILES leopard-watchdog.conf DESTINATION /etc/leopard-watchdog)