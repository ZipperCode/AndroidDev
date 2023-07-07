package com.zipper.develop.binder.canary

/**
 * 阻塞时间
 */
const val BLOCK_TIME = 100L

// 默认超大数据判断的阈值
const val LARGE_DATA_FACTOR = 0.75f

// 同步调用1M
const val SYNC_BINDER_DATA_SIZE = 1 * 1024 * 1024

// oneway 仅512k
const val ASYNC_BINDER_DATA_SIZE = 1024 * 512

/**
 * 监控配置
 * @param monitorInMainThread 是否在主线程监控，true: 仅主线程，false: 子线程也可监控
 * @param monitorLargeData  是否监控Binder大数据
 * @param blockTimeThresholdMils Binder调用阻塞时间
 * @param
 * @author zhangzhipeng
 * @date 2023/7/7
 */
data class MonitorConfig(
    val monitorInMainThread: Boolean = true,
    val monitorLargeData: Boolean = false,
    val blockTimeThresholdMils: Long = BLOCK_TIME,
    val largeDataFactor: Float = LARGE_DATA_FACTOR,
)
