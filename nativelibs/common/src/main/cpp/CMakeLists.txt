cmake_minimum_required(VERSION 3.22.1)

project("common")
message(">>>>>>> 构建common")

file(GLOB SRC *.cpp)
message("c files = ${SRC}")

add_library(common STATIC ${SRC})

find_library(log-lib log)
target_include_directories(common PUBLIC include .)
