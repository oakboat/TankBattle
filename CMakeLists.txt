cmake_minimum_required(VERSION 3.16)

project(TankBattle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    add_compile_options(/utf-8)
endif()

# =========================
# Dependencies
# =========================

find_package(Protobuf REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# =========================
# Protobuf
# =========================

set(PROTO_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/game.proto
)

protobuf_generate_cpp(PROTO_SRC PROTO_HDR ${PROTO_FILE})

# =========================
# Server
# =========================

add_executable(TankServer
    ${PROTO_SRC}
    ${PROTO_HDR}

    src/server/Main.cpp
    src/server/GameServer.cpp
    src/server/Session.cpp
    src/server/systems/MovementSystem.cpp
    src/server/systems/CollisionSystem.cpp
    src/server/systems/ProjectileSystem.cpp
    src/server/systems/ShootingSystem.cpp
)

target_include_directories(TankServer PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(TankServer PRIVATE
    protobuf::libprotobuf
    Boost::system
)

# =========================
# Client
# =========================

add_executable(TankClient
    ${PROTO_SRC}
    ${PROTO_HDR}

    src/client/Main.cpp
    src/client/GameClient.cpp
    src/client/Renderer.cpp
    src/client/NetClient.cpp
)

target_include_directories(TankClient PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(TankClient PRIVATE
    protobuf::libprotobuf
    Boost::system

    SDL2::SDL2
    SDL2::SDL2main
    SDL2_image::SDL2_image
)