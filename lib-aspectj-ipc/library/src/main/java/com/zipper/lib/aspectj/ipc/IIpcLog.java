package com.zipper.lib.aspectj.ipc;

import android.util.Log;

public interface IIpcLog {

    String TAG = "[IPC]";

    default void li(String format, Object... args){
        if (!IpcHelper.getInstance().debug){
            return;
        }
        Log.i(TAG, String.format(format, args));
    }

    default void log(String format, Object... args){
        if (!IpcHelper.getInstance().debug){
            return;
        }
        Log.d(TAG, String.format(format, args));
    }

    default void lw(String format, Object... args){
        if (!IpcHelper.getInstance().debug){
            return;
        }
        Log.w(TAG, String.format(format, args));
    }

    default void le(String format, Object... args){
        if (!IpcHelper.getInstance().debug){
            return;
        }
        Log.e(TAG, String.format(format, args));
    }
}
