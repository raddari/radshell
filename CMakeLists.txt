cmake_minimum_required(VERSION 3.20)
project(radshell C)

add_executable(radshell)
target_include_directories(radshell PRIVATE
  ${CMAKE_SOURCE_DIR}/include/radshell)

add_subdirectory(src)
