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

if(MSVC)
	add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
	# Or simply:
	add_compile_options(/utf-8)
endif()

#add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 打印消息
message("begin to compiler Athena ")

add_subdirectory(engine)
add_subdirectory(engine-lua)
add_subdirectory(game)
add_subdirectory(gateway)
target_compile_features(engine PUBLIC cxx_std_17)