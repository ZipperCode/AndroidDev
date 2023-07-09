package com.zipper.android.core.activitylifecycle

import android.app.Activity
import java.util.*
import kotlin.collections.ArrayList

internal class ActivityStackImpl : IActivityStack {

    private val stack: MutableList<Activity> by lazy { ArrayList() }

    override fun isTop(activityName: String): Boolean {
        return stack.lastOrNull()?.javaClass?.name == activityName
    }

    override fun push(activity: Activity) {
        if (stack.contains(activity)) {
            return
        }
        stack.add(activity)
    }

    override fun peek(): Activity? {
        return stack.lastOrNull()
    }

    override fun pop(activity: Activity){
        if (stack.isEmpty()){
            return
        }
        if (stack.contains(activity)){
            stack.remove(activity)
        }
    }

    override fun finishAll() {
        for (activity in stack) {
            activity.finish()
        }
        stack.clear()
    }

    override fun stackSize(): Int {
        return stack.size
    }

}