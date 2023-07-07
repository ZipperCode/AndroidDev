cmake_minimum_required(VERSION 3.22.1)

project("binder-canary")

# 获取项目根目录
set(ROOT_DIR "${PROJECT_SOURCE_DIR}/../../../../../")
get_filename_component(ROOT_DIR ${ROOT_DIR} ABSOLUTE)
message("ROOT_DIR = ${ROOT_DIR}")

# 静态库存放位置
set(LIB_STATIC_ROOT ${ROOT_DIR}/nativeLibs/libs-static)
message("LIB_STATIC_ROOT = ${LIB_STATIC_ROOT}")

set(LIB_XDL "xdl")
include_directories(
        /include/**/*.h
)

add_library(
        xdl
        STATIC
        IMPORTED
)

set_target_properties(
        xdl
        PROPERTIES IMPORTED_LOCATION
        "${LIB_STATIC_ROOT}/${CMAKE_BUILD_TYPE}/${LIB_XDL}/${ANDROID_ABI}/lib${LIB_XDL}.a"
)

add_library(
        canary
        SHARED
        canary.cpp)


find_library(
        log-lib

        log)


target_link_libraries(
        canary
        ${log-lib}
        ${LIB_XDL}
)