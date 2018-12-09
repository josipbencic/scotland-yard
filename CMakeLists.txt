cmake_minimum_required(VERSION 3.10)

project(ScotlandYard LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
	set(cxx_compile_options -Wall -Wextra)
endif()

add_subdirectory(src)
