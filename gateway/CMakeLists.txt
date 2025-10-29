cmake_minimum_required(VERSION 3.16)
project(gateway VERSION 0.1.0 LANGUAGES C CXX)

set(SOLUTION_DIR ${PROJECT_SOURCE_DIR}/../)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SOLUTION_DIR}/bin)

find_package(protobuf CONFIG REQUIRED)

# 源码
file(GLOB_RECURSE SRC_FILES
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.cc
        ${PROJECT_SOURCE_DIR}/src/*.c

        ${PROJECT_SOURCE_DIR}/progen/*.cpp
        ${PROJECT_SOURCE_DIR}/progen/*.cc
        ${PROJECT_SOURCE_DIR}/progen/*.c
)

add_executable(${PROJECT_NAME}
        ${SRC_FILES}
)

# 统一头文件路径
target_include_directories(${PROJECT_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/progen
)

# 只需要链接 engine，所有 include 会自动继承
target_link_libraries(${PROJECT_NAME} PRIVATE engine  hiredis protobuf::libprotobuf)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)
