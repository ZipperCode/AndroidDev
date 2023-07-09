package com.zipper.android.core.util.view

import android.view.View
import kotlin.math.abs

const val ANTI_SHAKE_TIME: Long = 1000
const val VIEW_CLICK_TIME_TAG: Int = 0xFF000000.toInt()


fun View.isFastClick(): Boolean{
    return isFastClick(ANTI_SHAKE_TIME)
}

fun View.isFastClick(interval: Long): Boolean {
    val lastClickTime = getTag(VIEW_CLICK_TIME_TAG) as? Long
    val currentTime = System.currentTimeMillis()
    if (lastClickTime != null){
        if (abs(lastClickTime - currentTime) < interval){
            return true
        }
    }
    setTag(VIEW_CLICK_TIME_TAG, currentTime)
    return false
}

fun View.setAntiShakeClickListener(listener: View.OnClickListener){
    setOnClickListener {
        if (isFastClick()){
            return@setOnClickListener
        }
        listener.onClick(it)
    }
}