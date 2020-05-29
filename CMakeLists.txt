project(jlsvr)
cmake_minimum_required(VERSION 2.8)

#设置__LINE__为相对路径
string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")

#add_subdirectory(jlsvrsrc)

add_executable(libuv test.cpp)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
include(libuv)

target_link_libraries(libuv ${LIBUV_LIB})
target_include_directories(libuv PRIVATE ${LIBUV_INCLUDE_DIR})