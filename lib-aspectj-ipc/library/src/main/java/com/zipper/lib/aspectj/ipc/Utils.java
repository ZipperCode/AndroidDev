package com.zipper.lib.aspectj.ipc;

import android.app.ActivityManager;
import android.app.ActivityThread;
import android.app.Application;
import android.content.Context;
import android.os.Build;
import android.os.Process;

import java.util.List;

public class Utils {


    public static String getProcessName(Context context) {
        String processName = null;
        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                processName = Application.getProcessName();
            } else {
                // 小于9.0 Stub 方式，不反射
                processName = ActivityThread.currentProcessName();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (processName == null) {
            return getProcessNameWithAms(context);
        }
        return processName;
    }

    private static String getProcessNameWithAms(Context context) {
        int pid = Process.myPid();
        try {
            ActivityManager am = (ActivityManager) context.getSystemService(Context.ACTIVITY_SERVICE);
            if (am != null) {
                List<ActivityManager.RunningAppProcessInfo> runningAppList = am.getRunningAppProcesses();
                if (runningAppList != null) {
                    for (ActivityManager.RunningAppProcessInfo processInfo : runningAppList) {
                        if (processInfo.pid == pid) {
                            return processInfo.processName;
                        }
                    }
                }
            }
        }catch (Exception ignored) {}
        return null;
    }
}
