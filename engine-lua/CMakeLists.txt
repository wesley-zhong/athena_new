cmake_minimum_required(VERSION 3.16)
project(engine-lua VERSION 0.1.0 LANGUAGES C CXX)


# ========== 路径配置 ==========
set(SOLUTION_DIR ${PROJECT_SOURCE_DIR}/..)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SOLUTION_DIR}/libs)

set(3RD ${SOLUTION_DIR}/3rd)
set(SOL2 ${SOLUTION_DIR}/3rd/sol2)
set(LUA ${SOLUTION_DIR}/3rd/lua)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SOLUTION_DIR}/libs)


# ========== 源码收集 ==========
# 获取路径下所有的.cpp/.c/.cc文件，并赋值给变量中
aux_source_directory(${PROJECT_SOURCE_DIR} CPP_FILES)


# ========== 生成静态库 ==========
add_library(${PROJECT_NAME} STATIC
        ${CPP_FILES}
)
# ========== include 路径 ==========
target_include_directories(${PROJECT_NAME} PUBLIC
        ${SOL2}/include
        ${LUA}
)

# ========== 依赖链接 ==========
link_directories(${SOLUTION_DIR}/libs)
set(PROJECT_LINK_LIBS uv lua engine)

if (WIN32)
    set(PROJECT_LINK_LIBS uv lua54 engine)
endif ()


# 链接操作
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_LINK_LIBS})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)