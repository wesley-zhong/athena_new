cmake_minimum_required(VERSION 3.16)
project(engine VERSION 0.1.0 LANGUAGES C CXX)

set(SOLUTION_DIR ${PROJECT_SOURCE_DIR}/..)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SOLUTION_DIR}/libs)
set(3RD ${SOLUTION_DIR}/3rd)

# include paths
set(SPDLOG ${3RD}/spdlog)
set(ENGINE_COMMON ${PROJECT_SOURCE_DIR}/common)
set(ENGINE_DB ${PROJECT_SOURCE_DIR}/db)
set(ENGINE_NETWORK ${PROJECT_SOURCE_DIR}/network)
set(ENGINE_SERIAL ${PROJECT_SOURCE_DIR}/serialization)
set(ENGINE_UTILS ${PROJECT_SOURCE_DIR}/utils)
set(ENGINE_TRANSPORT ${PROJECT_SOURCE_DIR}/transport)

find_package(protobuf CONFIG REQUIRED)

# 自动递归查找所有源码文件
file(GLOB_RECURSE SRC_FILES
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/*.cc
        ${PROJECT_SOURCE_DIR}/*.c
)

add_library(engine STATIC
        ${SRC_FILES}
)

# 统一头文件路径，公开给下游（如 gateway）
target_include_directories(engine PUBLIC
        ${PROJECT_SOURCE_DIR}
        ${ENGINE_COMMON}
        ${ENGINE_DB}
        ${ENGINE_NETWORK}
        ${ENGINE_SERIAL}
        ${ENGINE_UTILS}
        ${SPDLOG}/include
        ${3RD}/uv/include
        ${3RD}
        ${3RD}/hiredis
        ${3RD}/mysql
)

# 链接依赖库
target_link_libraries(engine PUBLIC
        protobuf::libprotobuf
        spdlog
)
