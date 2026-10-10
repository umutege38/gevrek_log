cmake_minimum_required(VERSION 3.20)
project(GevrekLog LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES "*.c")
file(GLOB_RECURSE HEADERS "*.h")

add_library(GevrekLog STATIC ${SOURCES} ${HEADERS})

target_include_directories(GevrekLog PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(GevrekLog PRIVATE
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Debug>:GEVREK_DEBUG>
)
