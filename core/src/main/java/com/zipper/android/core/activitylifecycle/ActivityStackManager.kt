package com.zipper.android.core.activitylifecycle

import android.app.Activity
import android.app.Application
import androidx.annotation.MainThread

open class ActivityStackManager private constructor(
    private val dispatcher:ActivityLifecycleCallbackDispatcher = ActivityLifecycleCallbackDispatcher()
) : DefaultActivityLifecycleCallbacks by dispatcher {

    companion object{
        private var sInstance: ActivityStackManager? = null

        fun getInstance(): ActivityStackManager{
            if (sInstance == null){
                synchronized(this) {
                    if (sInstance == null) {
                        sInstance = ActivityStackManager()
                    }
                }
            }
            return sInstance!!
        }
    }

    private var isInit = false

    @MainThread
    fun initial(context: Application?){
        if (context == null) {
            return
        }
        if (isInit){
            return
        }
        dispatcher.activityStack = ActivityStackImpl()
        context.registerActivityLifecycleCallbacks(dispatcher)
        isInit = true
    }

    @MainThread
    fun setDelegate(delegate: IActivityStack){
        if (isInit){
            return
        }
        if (dispatcher.activityStack?.stackSize() != 0){
            throw IllegalStateException("must call at Application")
        }
        dispatcher.activityStack = delegate
    }

    fun registerLifecycleCallbacks(lifecycleCallbacks: Application.ActivityLifecycleCallbacks){
        if (isInit){
            return
        }
        dispatcher.registerLifecycleCallbacks(lifecycleCallbacks)
    }

    fun unregisterLifecycleCallbacks(lifecycleCallbacks: Application.ActivityLifecycleCallbacks){
        if (isInit){
            return
        }
        dispatcher.unregisterLifecycleCallbacks(lifecycleCallbacks)
    }
}