cmake_minimum_required(VERSION 3.22.1)
project(xhook)

message(">>>>>>> 构建xhook")
file(GLOB XHOOK_SRC *.c)
# 构建静态库
add_library(xhook STATIC ${XHOOK_SRC})

target_compile_features(xhook PUBLIC c_std_17)
target_compile_options(xhook PUBLIC -std=c17 -Weverything -Werror)
target_include_directories(xhook PUBLIC include .)

if(USEASAN)
target_compile_options(xhook PUBLIC -fsanitize=address -fno-omit-frame-pointer)
target_link_options(xhook PUBLIC -fsanitize=address)
else()
target_compile_options(xhook PUBLIC -Oz -flto -faddrsig -ffunction-sections -fdata-sections)
target_link_options(xhook PUBLIC -Oz -flto -Wl,--icf=all -Wl,--exclude-libs,ALL -Wl,--gc-sections -Wl)
endif()
