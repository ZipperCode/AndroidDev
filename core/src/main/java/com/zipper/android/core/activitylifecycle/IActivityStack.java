package com.zipper.android.core.activitylifecycle;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.text.TextUtils;

import androidx.annotation.MainThread;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.util.List;

public interface IActivityStack {

    boolean isTop(@NonNull String activityName);

    @MainThread
    void push(@NonNull Activity activity);

    @Nullable
    Activity peek();

    @MainThread
    void pop(@NonNull Activity activity);

    @MainThread
    void finishAll();

    int stackSize();

}
