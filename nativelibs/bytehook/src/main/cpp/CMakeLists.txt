cmake_minimum_required(VERSION 3.22.1)
project(bytehook)

enable_language(ASM)
message("构建bytehook第三方")

# 收集所有的*.c文件赋值到变量SRC中
file(GLOB SRC *.c)



# 构建静态库
add_library(bytehook STATIC ${SRC})

# 预编译功能 <target> <PRIVATE|PUBLIC|INTERFACE> <feature> [...]
target_compile_features(bytehook PUBLIC c_std_17)
# 添加编译参数
target_compile_options(bytehook PUBLIC -Weverything -Werror)
# 包含编译目录
target_include_directories(bytehook PUBLIC . include third_party/bsd third_party/lss)

# 链接log模块
target_link_libraries(bytehook log)

if(USEASAN)
    target_compile_options(bytehook PUBLIC -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(bytehook PUBLIC -fsanitize=address)
else()
    target_compile_options(bytehook PUBLIC -Oz -flto -faddrsig -ffunction-sections -fdata-sections)
    target_link_options(bytehook PUBLIC -Oz -flto -Wl,--icf=all -Wl,--exclude-libs,ALL -Wl,--gc-sections -Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/bytehook.map.txt)
endif()
