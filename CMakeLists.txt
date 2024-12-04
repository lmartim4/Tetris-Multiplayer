# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.15)

# Project name and version
project(TetrisProject VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directory for headers
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
include_directories(${INCLUDE_DIR})

# Find SFML package (required)
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

# Find ENet library
find_library(ENET_LIB enet REQUIRED)
find_path(ENET_INCLUDE_DIR enet/enet.h REQUIRED)

# Add include directories for ENet
include_directories(${ENET_INCLUDE_DIR})

# Source files for the server
set(SERVER_SOURCES
    src/TetrisServer.cpp
    src/ServerManager.cpp
    src/NetworkManager.cpp
)

# Source files for the client
set(CLIENT_SOURCES
    src/TetrisClient.cpp
    src/ClientManager.cpp
    src/NetworkManager.cpp
)

# Configure build types
set(CMAKE_BUILD_TYPE_INIT Release)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Select Debug or Release" FORCE)

# Server executable
add_executable(server ${SERVER_SOURCES})
target_link_libraries(server sfml-system sfml-network ${ENET_LIB})

# Client executable
add_executable(client ${CLIENT_SOURCES})
target_link_libraries(client sfml-system sfml-window sfml-graphics sfml-network ${ENET_LIB})

# Custom commands to run in debug or release mode
add_custom_target(run_server_simple
    COMMAND $<TARGET_FILE:server>
    COMMENT "Running the server in simple mode (Release configuration)"
    USES_TERMINAL
)

add_custom_target(run_server_debug
    COMMAND $<TARGET_FILE:server> --debug
    COMMENT "Running the server in debug mode (Debug configuration)"
    USES_TERMINAL
)

add_custom_target(run_client_simple
    COMMAND $<TARGET_FILE:client>
    COMMENT "Running the client in simple mode (Release configuration)"
    USES_TERMINAL
)

add_custom_target(run_client_debug
    COMMAND $<TARGET_FILE:client> --debug
    COMMENT "Running the client in debug mode (Debug configuration)"
    USES_TERMINAL
)

# Custom build rules for server and client
add_custom_target(server_build
    COMMAND ${CMAKE_COMMAND} --build . --target server --config Release
    COMMENT "Building the server (Release)"
)

add_custom_target(server_debug_build
    COMMAND ${CMAKE_COMMAND} --build . --target server --config Debug
    COMMENT "Building the server (Debug)"
)

add_custom_target(client_build
    COMMAND ${CMAKE_COMMAND} --build . --target client --config Release
    COMMENT "Building the client (Release)"
)

add_custom_target(client_debug_build
    COMMAND ${CMAKE_COMMAND} --build . --target client --config Debug
    COMMENT "Building the client (Debug)"
)