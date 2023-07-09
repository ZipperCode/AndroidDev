//
// Created by Zipper on 2023/7/7.
//

#ifndef ANDROIDDEV_BINDER_GLOBAL_H
#define ANDROIDDEV_BINDER_GLOBAL_H

static struct java_art_method_offset_t {
    bool has_initialized = false;
    /// java native方法与ArtMethod的偏移值
    uint32_t art_method_offset;
    /// accessFlag的偏移
    uint32_t art_access_flag_offset;
} gArtMethodOffset;

#endif //ANDROIDDEV_BINDER_GLOBAL_H
