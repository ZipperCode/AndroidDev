package com.zipper.develop.binder.canary

/**
 * @author zhangzhipeng
 * @date 2023/7/7
 */
object BinderCanary {
    init {
        System.loadLibrary("canary")
    }

    @JvmStatic
    external fun init(monitorConfig: MonitorConfig): Boolean

    @JvmStatic
    external fun monitor(): Boolean

    @JvmStatic
    external fun unmonitored(): Boolean

    @JvmStatic
    fun onReport(errorCode: Int, dataSize: Int, backtrace: String) {

    }
}
