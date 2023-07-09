package com.zipper.android.core.activitylifecycle

import android.app.Application
import android.content.Context
import androidx.startup.Initializer

/**
 * 通过jetpack-startup 库启用窗口栈管理
 */
class ActivityManagerInitializer: Initializer<Unit> {
    override fun create(context: Context) {
        var application: Application? = null
        if (context is Application){
            application = context
        } else if (context.applicationContext is Application) {
            application = context.applicationContext as Application
        }
        ActivityStackManager.getInstance().initial(application)
    }

    override fun dependencies(): MutableList<Class<out Initializer<*>>> {
        return mutableListOf()
    }
}