cmake_minimum_required(VERSION 3.16)
project(gateway VERSION 0.1.0 LANGUAGES C CXX)


set (SOLUTION_DIR  ${PROJECT_SOURCE_DIR}/..)
set(ENGINE_DIR  ${SOLUTION_DIR}/engine)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SOLUTION_DIR}/libs)

set(3RD ${SOLUTION_DIR}/3rd)
set(SOL2 ${SOLUTION_DIR}/3rd/sol2)
set(LUA ${SOLUTION_DIR}/3rd/lua)
set(SPDLOG ${SOLUTION_DIR}/3rd/spdlog)

set(ENGINE_COMMON ${ENGINE_DIR}/common)
set(ENGINE_DB ${ENGINE_DIR}/db)
set(ENGINE_NETWORK ${ENGINE_DIR}/network)
set(ENGINE_SERVERIAL ${ENGINE_DIR}/serialization)
set(ENGINE_UTILS ${ENGINE_DIR}/utils)
set(PROTO_GEN ${PROJECT_SOURCE_DIR}/progen)
find_package(protobuf CONFIG REQUIRED)

include_directories(
        ${SOL2}/include
        ${LUA}
        ${3RD}
        ${3RD}/mysql
        ${3RD}/hiredis
        ${SPDLOG}/include
        ${ENGINE_DIR}
        ${ENGINE_COMMON}
        ${ENGINE_DB}
        ${ENGINE_NETWORK}
        ${ENGINE_SERVERIAL}
        ${ENGINE_UTILS}
)
include_directories(
        ${PROJECT_SOURCE_DIR}/src/network
        ${PROJECT_SOURCE_DIR}/src/objs
        ${PROJECT_SOURCE_DIR}/src/thread
        ${PROTO_GEN}
)
# 设置项目间的依赖关系
add_dependencies(engine engine-lua)

# 获取路径下所有的.cpp/.c/.cc文件，并赋值给变量中
aux_source_directory(${PROJECT_SOURCE_DIR}/src  CPP_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/network  CPP_SERVICES)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/objs  OBJS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/thread  CPP_THREAD)
aux_source_directory(${PROTO_GEN}  PROTO_OBJS)

link_directories(${PROJECT_SOURCE_DIR}/../libs)

#set(protobuf_DIR "d:/temp/camke")


set (PROJECT_LINK_LIBS  mysqlclient  hiredis  engine-lua protobuf::libprotobuf)
message("======== begin to configure game objs: ${PROTO_OBJS}")
message("======== begin to configure game libs: ${PROJECT_LINK_LIBS}")


# 生成可执行文件
add_executable(${PROJECT_NAME}  ${CPP_FILES} ${OBJS} ${PROTO_OBJS} ${CPP_SERVICES} ${CPP_THREAD})

# 链接操作
target_link_libraries(${PROJECT_NAME}  ${PROJECT_LINK_LIBS})