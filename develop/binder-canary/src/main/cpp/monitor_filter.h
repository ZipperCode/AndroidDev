//
// Created by Zipper on 2023/7/14.
//
#pragma once

#ifndef ANDROIDDEV_BINDER_MONITOR_FILTER_H
#define ANDROIDDEV_BINDER_MONITOR_FILTER_H

#include "system_util.h"
#include "global.h"

namespace binder_canary {

    constexpr int kFlagOneWay = 0x00000001;
    constexpr long kSyncIPCDataSizeThreshold = 1L * 1024 * 1024;
    constexpr long kAsyncIPCDataSizeThreshold = kSyncIPCDataSizeThreshold / 2;

    class TransactCallInfo {
    public:
        TransactCallInfo() : start_time_ms(common::GetCurrentTimeMills()) {}

        virtual int flags() const = 0;

        virtual int dataSize() const = 0;

        long costTime() const {
            return common::GetCurrentTimeMills() - start_time_ms;
        }

    private:
        long start_time_ms;
    };

    template<typename T>
    concept CallInfo = std::is_base_of_v<std::remove_pointer_t<TransactCallInfo>, std::remove_pointer_t<T>>;

    template<CallInfo T>
    class MonitorDispatcher {
        /**
         * 传输超过数据量回调
         */
        typedef void(*OnTransactDataLarge)(const T &);

        /**
         * 传输超过阻塞时间回调
         */
        typedef void(*OnTransactBlock)(const T &, long);

    public:
        MonitorDispatcher(OnTransactDataLarge onTransactDataLarge = nullptr, OnTransactBlock onTransactBlock = nullptr)
                : onTransactDataLarge_(onTransactDataLarge), onTransactBlock_(onTransactBlock) {

        }

    public:
        void onTransactStart(T &callInfo) {
            if (enableMonitorLargeData(callInfo) && onTransactDataLarge_ != nullptr) {
                // 监控大数据
                onTransactDataLarge_(callInfo);
            }
        }

        void onTransactEnd(T &callInfo) {
            if (nullptr == onTransactBlock_) {
                return;
            }
            if (!enableMainThreadMonitor()) {
                return;
            }

            if (isOneWay(callInfo)) {
                return;
            }

            long castTime = callInfo.castTime();
            if (castTime > gMonitorConfig.block_time_threshold_mills) {
                onTransactBlock_(callInfo, castTime);
            }
        }

    private:

        bool enableMonitorLargeData(T &callInfo) const {
            bool enable = gMonitorConfig.monitor_large_data;
            if (!enable) {
                return false;
            }
            float factor = gMonitorConfig.large_data_factor;
            long threshold = isOneWay(callInfo) ? kAsyncIPCDataSizeThreshold : kSyncIPCDataSizeThreshold;
            long dataSize = callInfo.dataSize();
            return dataSize >= threshold * factor;
        }

        /// isAsyncIPC
        bool isOneWay(T &callInfo) const {
            return 0 != (callInfo.flags() & kFlagOneWay);
        }

        bool enableMainThreadMonitor() const {
            return gMonitorConfig.monitor_in_main_thread && common::IsMainThread();
        }

    private:
        OnTransactDataLarge onTransactDataLarge_;
        OnTransactBlock onTransactBlock_;
    };

} // binder_canary

#endif //ANDROIDDEV_BINDER_MONITOR_FILTER_H
