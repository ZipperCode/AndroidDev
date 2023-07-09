package com.zipper.android.core.activitylifecycle

import android.app.Activity
import android.app.Application
import android.os.Build
import android.os.Bundle
import androidx.annotation.MainThread
import androidx.annotation.RequiresApi

internal class ActivityLifecycleCallbackDispatcher : DefaultActivityLifecycleCallbacks {

    private val lifecycleCallbackList: MutableList<Application.ActivityLifecycleCallbacks> = mutableListOf()

    internal var activityStack: IActivityStack? = null

    fun registerLifecycleCallbacks(lifecycleCallbacks: Application.ActivityLifecycleCallbacks) {
        synchronized(lifecycleCallbackList) {
            lifecycleCallbackList.add(lifecycleCallbacks)
        }
    }


    fun unregisterLifecycleCallbacks(lifecycleCallbacks: Application.ActivityLifecycleCallbacks) {
        synchronized(lifecycleCallbackList){
            lifecycleCallbackList.remove(lifecycleCallbacks)
        }
    }

    @RequiresApi(Build.VERSION_CODES.Q)
    override fun onActivityPreCreated(activity: Activity, savedInstanceState: Bundle?) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityPreCreated(activity, savedInstanceState)
        }
    }

    override fun onActivityCreated(activity: Activity, savedInstanceState: Bundle?) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityCreated(activity, savedInstanceState)
        }
    }

    @RequiresApi(Build.VERSION_CODES.Q)
    override fun onActivityPostCreated(activity: Activity, savedInstanceState: Bundle?) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityPostCreated(activity, savedInstanceState)
        }
    }

    @RequiresApi(Build.VERSION_CODES.Q)
    override fun onActivityPreStarted(activity: Activity) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityPreStarted(activity)
        }
    }

    override fun onActivityStarted(activity: Activity) {
        activityStack?.push(activity)
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityStarted(activity)
        }
    }

    @RequiresApi(Build.VERSION_CODES.Q)
    override fun onActivityPostStarted(activity: Activity) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityPostStarted(activity)
        }
    }

    @RequiresApi(Build.VERSION_CODES.Q)
    override fun onActivityPreResumed(activity: Activity) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityPreResumed(activity)
        }
    }

    override fun onActivityResumed(activity: Activity) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityResumed(activity)
        }
    }

    @RequiresApi(Build.VERSION_CODES.Q)
    override fun onActivityPostResumed(activity: Activity) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityPostResumed(activity)
        }
    }

    @RequiresApi(Build.VERSION_CODES.Q)
    override fun onActivityPrePaused(activity: Activity) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityPrePaused(activity)
        }
    }

    override fun onActivityPaused(activity: Activity) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityPaused(activity)
        }
    }

    @RequiresApi(Build.VERSION_CODES.Q)
    override fun onActivityPostPaused(activity: Activity) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityPostPaused(activity)
        }
    }

    @RequiresApi(Build.VERSION_CODES.Q)
    override fun onActivityPreStopped(activity: Activity) {
        activityStack?.pop(activity)
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityPreStopped(activity)
        }
    }

    override fun onActivityStopped(activity: Activity) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityStopped(activity)
        }
    }

    @RequiresApi(Build.VERSION_CODES.Q)
    override fun onActivityPostStopped(activity: Activity) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityPostStopped(activity)
        }
    }

    @RequiresApi(Build.VERSION_CODES.Q)
    override fun onActivityPreSaveInstanceState(activity: Activity, outState: Bundle) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityPreSaveInstanceState(activity, outState)
        }
    }

    override fun onActivitySaveInstanceState(activity: Activity, outState: Bundle) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivitySaveInstanceState(activity, outState)
        }
    }

    @RequiresApi(Build.VERSION_CODES.Q)
    override fun onActivityPostSaveInstanceState(activity: Activity, outState: Bundle) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityPostSaveInstanceState(activity, outState)
        }
    }

    @RequiresApi(Build.VERSION_CODES.Q)
    override fun onActivityPreDestroyed(activity: Activity) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityPreDestroyed(activity)
        }
    }

    override fun onActivityDestroyed(activity: Activity) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityDestroyed(activity)
        }
    }

    @RequiresApi(Build.VERSION_CODES.Q)
    override fun onActivityPostDestroyed(activity: Activity) {
        for (lifecycleCallback in lifecycleCallbackList) {
            lifecycleCallback.onActivityPostDestroyed(activity)
        }
    }
}