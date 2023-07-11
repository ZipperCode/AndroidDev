package com.zipper.develop.binder.canary;

import androidx.annotation.NonNull;

/**
 * @author zhangzhipeng
 * @date 2023/7/7
 */
public class BinderCanary {

    static {
        System.loadLibrary("canary");
    }

    public static native boolean init(@NonNull MonitorConfig monitorConfig);

}
