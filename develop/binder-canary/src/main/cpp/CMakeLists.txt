cmake_minimum_required(VERSION 3.22.1)

project("binder-canary")

# 获取项目根目录
set(ROOT_DIR "${PROJECT_SOURCE_DIR}/../../../../../")
get_filename_component(ROOT_DIR ${ROOT_DIR} ABSOLUTE)
message("ROOT_DIR = ${ROOT_DIR}")

find_package(xdl REQUIRED CONFIG)
find_package(common REQUIRED CONFIG)
find_package(bytehook REQUIRED CONFIG)

add_library(
        canary
        SHARED
        binder_jni.cpp
        binder_hook.cpp
)


find_library(
        log-lib

        log)


target_link_libraries(
        canary
        ${log-lib}
        common::common
        xdl::xdl
        bytehook::bytehook
)