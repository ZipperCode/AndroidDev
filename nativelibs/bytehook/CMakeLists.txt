cmake_minimum_required(VERSION 3.22.1)

set(MODULE_NAME "bytehook")

#set(LIB_STATIC_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../libs-static)
#
## 静态库输出路径
#set(STATIC_OUTPUT ${LIB_STATIC_ROOT}/${CMAKE_BUILD_TYPE}/${MODULE_NAME}/${ANDROID_ABI})
#message("bytehook静态库输出目录为 = ${STATIC_OUTPUT}")

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${STATIC_OUTPUT})
set(SUB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp)

add_subdirectory(${SUB_DIR})

# 收集所有头文件
#file(GLOB_RECURSE H_SRC ${SUB_DIR}/include/*.h)
#message("查找所有头文件 = ${H_SRC}")
## 拷贝到include目录下
#file(COPY ${H_SRC} DESTINATION ${STATIC_OUTPUT}/../include)

