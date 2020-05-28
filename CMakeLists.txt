project(jlsvr)
cmake_minimum_required(VERSION 2.8)
#设置__LINE__为相对路径
string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")
add_subdirectory(jlsvrsrc)