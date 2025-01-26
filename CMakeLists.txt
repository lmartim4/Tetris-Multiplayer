# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.15)

# Project name and version
project(TetrisProject VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories for each module
set(COMMON_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/common)
set(SERVER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/server ${CMAKE_SOURCE_DIR}/include/server/game)
set(CLIENT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/client)
set(CLIENT_INCLUDE_SCREENS_DIR ${CMAKE_SOURCE_DIR}/include/client/screens)

include_directories(${COMMON_INCLUDE_DIR} ${SERVER_INCLUDE_DIR} ${CLIENT_INCLUDE_DIR} ${CLIENT_INCLUDE_SCREENS_DIR})

# Find SFML package (required)
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

# Find ENet library
find_library(ENET_LIB enet REQUIRED)
find_path(ENET_INCLUDE_DIR enet/enet.h REQUIRED)

# Add include directories for ENet
include_directories(${ENET_INCLUDE_DIR})

# Find threads (pthread)
find_package(Threads REQUIRED)

# Source files for the server
set(SERVER_SOURCES
    src/server/TetrisServer.cpp
    src/server/ServerManager.cpp
    src/server/game/BoardController.cpp
    src/server/game/TetrominoFactory.cpp
    src/server/game/TetrominoManager.cpp
    src/server/game/GameManager.cpp
    src/server/game/Game.cpp
    src/server/game/GamePhysics.cpp
)

# Source files for the client
set(CLIENT_SOURCES
    src/client/TetrisClient.cpp
    src/client/ClientManager.cpp
    src/client/AudioManager.cpp
    src/client/CellRenderer.cpp
    src/client/screens/MiniBoard.cpp
    src/client/screens/Button.cpp
    src/client/screens/BoardScreen.cpp
    src/client/screens/EndGameScreen.cpp
    src/client/screens/MenuScreen.cpp
    src/client/screens/GameScreen.cpp
    src/client/screens/LobbyScreen.cpp
    src/client/screens/WaitingConnectionScreen.cpp
    src/client/ScreenManager.cpp
    src/client/Screen.cpp
)

# Source files for both
set(COMMON_SOURCES
    src/common/NetworkManager.cpp
    src/common/EndGameData.cpp
    src/common/Cell.cpp
    src/common/TetrisBoard.cpp
    src/common/PlayerList.cpp
    src/common/PlayerData.cpp
    src/common/Logger.cpp
    src/common/Tetromino.cpp
)

# Configure build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Select Debug or Release" FORCE)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type (Debug or Release)" FORCE)
endif()

# Server executable
add_executable(server ${SERVER_SOURCES} ${COMMON_SOURCES})
target_include_directories(server PRIVATE ${COMMON_INCLUDE_DIR} ${SERVER_INCLUDE_DIR})
target_link_libraries(server sfml-system ${ENET_LIB} Threads::Threads)

# Client executable
add_executable(client ${CLIENT_SOURCES} ${COMMON_SOURCES})
target_include_directories(client PRIVATE ${COMMON_INCLUDE_DIR} ${CLIENT_INCLUDE_DIR})
target_link_libraries(client sfml-system sfml-window sfml-graphics sfml-audio ${ENET_LIB} Threads::Threads)

# Custom build rules for server and client
add_custom_target(server_build_release
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target server --config Release
    COMMENT "Building the server (Release)"
)

add_custom_target(server_build_debug
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target server --config Debug
    COMMENT "Building the server (Debug)"
)

add_custom_target(client_build_release
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target client --config Release
    COMMENT "Building the client (Release)"
)

add_custom_target(client_build_debug
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target client --config Debug
    COMMENT "Building the client (Debug)"
)

# Custom run commands for server and client
add_custom_target(run-server
    COMMAND $<TARGET_FILE:server>
    COMMENT "Running the server in Release mode"
    USES_TERMINAL
)

add_custom_target(run-server-debug
    COMMAND $<TARGET_FILE:server> --debug
    COMMENT "Running the server in Debug mode"
    USES_TERMINAL
)

add_custom_target(run-client
    COMMAND $<TARGET_FILE:client>
    COMMENT "Running the client in Release mode"
    USES_TERMINAL
)

add_custom_target(run-client-debug
    COMMAND $<TARGET_FILE:client> --debug
    COMMENT "Running the client in Debug mode"
    USES_TERMINAL
)
