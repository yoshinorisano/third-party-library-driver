cmake_minimum_required(VERSION 3.24)
project(freetype-driver)

set(FREETYPE_DIR ${CMAKE_SOURCE_DIR}/deps/freetype)
set(FREETYPE_INCLUDE_DIR ${FREETYPE_DIR}/include)
set(FREETYPE_LIBRARY_DIR ${FREETYPE_DIR}/build/Debug)

set(STB_DIR ${CMAKE_SOURCE_DIR}/deps/stb)

set(SOURCE_FILES driver.cpp)

include_directories(${FREETYPE_INCLUDE_DIR} ${STB_DIR})

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} ${FREETYPE_LIBRARY_DIR}/freetyped.lib)

# Set startup project for convenience
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
