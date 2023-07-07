//
// Created by Zipper on 2023/7/7.
//

#ifndef ANDROIDDEV_BINDER_GLOBAL_H
#define ANDROIDDEV_BINDER_GLOBAL_H

#include <stdin.h>

static struct hook_config {
    /// Java和C函数之间的偏移
    uint32_t native_method_offset;
} gHookConfig;

#endif //ANDROIDDEV_BINDER_GLOBAL_H
