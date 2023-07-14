cmake_minimum_required(VERSION 3.22.1)

project("common")
message(">>>>>>> 构建common")

file(GLOB SRC *.cpp)
message("c files = ${SRC}")
include_directories(.)

add_library(common STATIC ${SRC})

find_library(log-lib log)
target_include_directories(common PUBLIC include . utils)

if(USEASAN)
    target_compile_options(common PUBLIC -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(common PUBLIC -fsanitize=address)
else()
    target_compile_options(common PUBLIC -Oz -flto -faddrsig -ffunction-sections -fdata-sections)
    target_link_options(common PUBLIC -Oz -flto -Wl,--icf=all -Wl,--exclude-libs,ALL -Wl,--gc-sections -Wl)
endif()
