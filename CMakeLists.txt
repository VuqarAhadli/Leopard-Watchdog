cmake_minimum_required(VERSION 3.10)
project(pardus-health-check VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O2")

# Main executable
add_executable(pardus-health-check
    src/main.c
    src/logger.c
    src/ui.c
    src/system_health.c
    src/package_analyzer.c
    src/port_scanner.c
    src/hardware_inventory.c
    src/report_generator.c
)

target_include_directories(pardus-health-check PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Optional: Python module for advanced features
find_package(Python3 COMPONENTS Development)
if(Python3_FOUND)
    message(STATUS "Python3 found - enabling enhanced modules")
    target_link_libraries(pardus-health-check PRIVATE ${Python3_LIBRARIES})
    target_include_directories(pardus-health-check PRIVATE ${Python3_INCLUDE_DIRS})
endif()

# Installation
install(TARGETS pardus-health-check DESTINATION bin)
install(FILES pardus-health-check.1 DESTINATION share/man/man1)
install(FILES README.md DESTINATION share/doc/pardus-health-check)
