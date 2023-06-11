package com.zipper.lib.aspectj.ipc.sample

import android.app.Application
import com.zipper.lib.aspectj.ipc.IpcHelper

class App: Application() {

    override fun onCreate() {
        super.onCreate()
        IpcHelper.getInstance().init(this, true)
    }
}