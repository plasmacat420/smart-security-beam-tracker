cmake_minimum_required(VERSION 3.10)
project(SecurityLaserVisualizer VERSION 1.0.0 LANGUAGES CXX)

# C++17 required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific flags
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Source files
set(CORE_SOURCES
    src/core/Grid.cpp
    src/core/SecurityLaser.cpp
)

set(VISUALIZER_SOURCES
    src/visualizer/Renderer.cpp
    src/visualizer/Animation.cpp
    src/visualizer/UIManager.cpp
)

set(UTILS_SOURCES
    src/utils/ColorManager.cpp
    src/utils/FileIO.cpp
    src/utils/Benchmark.cpp
)

set(ALL_SOURCES
    ${CORE_SOURCES}
    ${VISUALIZER_SOURCES}
    ${UTILS_SOURCES}
)

# Main executable
add_executable(SecurityLaserVisualizer
    src/main.cpp
    ${ALL_SOURCES}
)

# Platform-specific libraries
if(WIN32)
    # Windows requires no additional libraries for console I/O
elseif(UNIX AND NOT APPLE)
    # Linux
    target_link_libraries(SecurityLaserVisualizer PRIVATE pthread)
elseif(APPLE)
    # macOS
    target_link_libraries(SecurityLaserVisualizer PRIVATE pthread)
endif()

# Installation
install(TARGETS SecurityLaserVisualizer
    RUNTIME DESTINATION bin
)

# Install web files
install(DIRECTORY ${CMAKE_SOURCE_DIR}/web/
    DESTINATION share/SecurityLaserVisualizer/web
    FILES_MATCHING PATTERN "*.html" PATTERN "*.css" PATTERN "*.js"
)

# Install documentation
install(FILES README.md
    DESTINATION share/doc/SecurityLaserVisualizer
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/docs/
    DESTINATION share/doc/SecurityLaserVisualizer/docs
)

# Optional: Build tests
option(BUILD_TESTS "Build test suite" OFF)

if(BUILD_TESTS)
    enable_testing()

    add_executable(SecurityLaserTests
        tests/test_suite.cpp
        ${ALL_SOURCES}
    )

    add_test(NAME SecurityLaserTests
        COMMAND SecurityLaserTests
    )
endif()

# Package configuration
set(CPACK_PACKAGE_NAME "SecurityLaserVisualizer")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Interactive Security Laser Beam Visualizer")
set(CPACK_PACKAGE_VENDOR "Your Name")

include(CPack)

# Print configuration summary
message(STATUS "")
message(STATUS "=== Security Laser Visualizer Configuration ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Build Tests: ${BUILD_TESTS}")
message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "===============================================")
message(STATUS "")
