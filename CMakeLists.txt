project(jlsvr)
cmake_minimum_required(VERSION 2.8)

#设置__LINE__为相对路径
string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")

#添加第三方需要库
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
FILE(GLOB THIRD_CMAKES "${CMAKE_SOURCE_DIR}/cmake/*cmake")
FOREACH(CMAKE_PATH ${THIRD_CMAKES})
    STRING(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${CMAKE_PATH})
    include(${FILE_NAME})
ENDFOREACH(CMAKE_PATH)

add_subdirectory(jlsvrsrc)