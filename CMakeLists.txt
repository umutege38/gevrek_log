cmake_minimum_required(VERSION 3.20)
project(GevrekLog LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(GevrekLog STATIC
    src/gevrek_log.c
    include/gevrek_log.h
)

target_include_directories(GevrekLog PUBLIC include)

target_compile_definitions(GevrekLog PRIVATE
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Debug>:GEVREK_DEBUG>
)