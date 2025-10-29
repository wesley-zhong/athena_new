cmake_minimum_required(VERSION 3.16)
project(game VERSION 0.1.0 LANGUAGES C CXX)


set(SOLUTION_DIR ${PROJECT_SOURCE_DIR}/..)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SOLUTION_DIR}/libs)

set(PROTO_GEN ${PROJECT_SOURCE_DIR}/progen)
find_package(protobuf CONFIG REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROTO_GEN}
)
# 设置项目间的依赖关系
#add_dependencies(engine engine-lua)

# 获取路径下所有的.cpp/.c/.cc文件，并赋值给变量中
#aux_source_directory(${PROJECT_SOURCE_DIR}/src CPP_FILES)
#aux_source_directory(${PROJECT_SOURCE_DIR}/src/network CPP_NETWORK)
#aux_source_directory(${PROJECT_SOURCE_DIR}/src/objs OBJS)
#aux_source_directory(${PROJECT_SOURCE_DIR}/src/thread CPP_THREAD)
#aux_source_directory(${PROJECT_SOURCE_DIR}/src/service CPP_SERVICE)
#aux_source_directory(${PROTO_GEN} PROTO_OBJS)

# 自动递归查找所有源码文件
file(GLOB_RECURSE SRC_FILES
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.cc
        ${PROJECT_SOURCE_DIR}/src/*.c

        ${PROJECT_SOURCE_DIR}/progen/*.cpp
        ${PROJECT_SOURCE_DIR}/progen/*.cc
        ${PROJECT_SOURCE_DIR}/progen/*.c
)


link_directories(${PROJECT_SOURCE_DIR}/../libs)

set(PROJECT_LINK_LIBS mysqlclient hiredis engine engine-lua protobuf::libprotobuf)
message("======== begin to configure game objs: ${PROTO_OBJS}")


# 生成可执行文件
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/progen
)

# 链接操作
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LINK_LIBS})