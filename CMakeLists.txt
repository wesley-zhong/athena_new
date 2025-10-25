cmake_minimum_required(VERSION 3.16)

project(athena VERSION 0.1.0 LANGUAGES C CXX)

# 设置整个项目的 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 必须使用指定标准
set(CMAKE_CXX_EXTENSIONS OFF)        # 禁用编译器扩展


if(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -std=c++17")
	add_definitions("-Wall -g")
endif()

#add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 打印消息
message("begin to compiler Athena ")

# 设置变量，下面的代码都可以用到
set(ENGINE ${PROJECT_SOURCE_DIR}/engine)
set(ENGINE-LUA ${PROJECT_SOURCE_DIR}/engine-lua)
set(GAME ${PROJECT_SOURCE_DIR}/game)
set(GATEWAY ${PROJECT_SOURCE_DIR}/gateway)


add_subdirectory(${ENGINE})
add_subdirectory(${ENGINE-LUA})
add_subdirectory(${GAME})
add_subdirectory(${GATEWAY})
