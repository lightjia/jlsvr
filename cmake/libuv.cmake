include(ExternalProject)

set(LIBUV_ROOT ${CMAKE_SOURCE_DIR}/third/libuv)
set(LIBUV_LIB_PATH ${LIBUV_ROOT}/LIBUV)  #编译后库目录
set(LIBUV_BUILD_PATH ${LIBUV_ROOT}/src/LIBUV-build) #Build目录
set(LIBUV_SRC_PATH ${LIBUV_ROOT}/src/LIBUV) #Build目录
set(LIBUV_GIT_TAG  v1.37.0)  # 指定版本
set(LIBUV_GIT_URL      https://github.com/libuv/libuv.git)  # 指定git仓库地址
set(LIBUV_CONFIGURE    mkdir -p ${LIBUV_ROOT} && cd ${LIBUV_BUILD_PATH} && cmake -D CMAKE_INSTALL_PREFIX=${LIBUV_LIB_PATH} ${LIBUV_SRC_PATH})  # 指定配置指令（注意此处修改了安装目录，否则默认情况下回安装到系统目录）
set(LIBUV_MAKE         cd ${LIBUV_BUILD_PATH} && make)  # 指定编译指令（需要覆盖默认指令，进入我们指定的LIBUV_ROOT目录下）
set(LIBUV_INSTALL      cd ${LIBUV_BUILD_PATH} && make install)  # 指定安装指令（需要覆盖默认指令，进入我们指定的LIBUV_ROOT目录下）

ExternalProject_Add(LIBUV
        PREFIX            ${LIBUV_ROOT}
        GIT_REPOSITORY    ${LIBUV_GIT_URL}
        GIT_TAG           ${LIBUV_GIT_TAG}
        CONFIGURE_COMMAND ${LIBUV_CONFIGURE}
        BUILD_COMMAND     ${LIBUV_MAKE}
        INSTALL_COMMAND   ${LIBUV_INSTALL}
)

# 指定编译好的静态库文件的路径
set(LIBUV_LIB       ${LIBUV_LIB_PATH}/lib64/libuv.so)
# 指定头文件所在的目录
set(LIBUV_INCLUDE_DIR   ${LIBUV_LIB_PATH}/include)

#使用库
#target_link_libraries(libuv ${LIBUV_LIB})
#target_include_directories(libuv PRIVATE ${LIBUV_INCLUDE_DIR})