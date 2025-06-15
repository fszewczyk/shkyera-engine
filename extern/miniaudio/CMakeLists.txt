cmake_minimum_required(VERSION 3.10)
project(miniaudio
    VERSION 0.11
)

add_library(
	miniaudio
	STATIC
	miniaudio.c 
)

target_include_directories(miniaudio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

