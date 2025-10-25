cmake_minimum_required(VERSION 3.16)
project(engine-lua VERSION 0.1.0 LANGUAGES C CXX)

# 设置变量
set (SOLUTION_DIR  ${PROJECT_SOURCE_DIR}/../..)
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

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SOLUTION_DIR}/libs)


include_directories(${LIBUV}/include
        ${SOL2}/include
        ${LUA}
        ${3RD}
        ${3RD}/mysql
        ${3RD}/hiredis
        ${SPDLOG}/include
        ${ENGINE_COMMON}
        ${ENGINE_DB}
        ${ENGINE_NETWORK}
        ${ENGINE_SERVERIAL}
        ${ENGINE_UTILS}
)

#message("======== begin to configureeeeee  :${ENGINE_DIR}  ${ENGINE_NETWORK}")
# 获取路径下所有的.cpp/.c/.cc文件，并赋值给变量中
aux_source_directory(${PROJECT_SOURCE_DIR}  CPP_FILES)


#message("======== begin to configure game libs: ${PROJECT_LINK_LIBS}")
#message("======== begin to configure game cpps:  ${COMMON_CPP_FILES}")


# 生成libs
add_library(${PROJECT_NAME} STATIC
        ${CPP_FILES}
)
# linked libs
link_directories(${SOLUTION_DIR}/libs)
set(PROJECT_LINK_LIBS hv lua engine)

if(WIN32)
  set(PROJECT_LINK_LIBS hv lua54 engine)
endif()


# 链接操作
target_link_libraries(${PROJECT_NAME} ${PROJECT_LINK_LIBS})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)