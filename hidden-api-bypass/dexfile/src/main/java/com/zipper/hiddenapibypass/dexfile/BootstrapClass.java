package com.zipper.hiddenapibypass.dexfile;

import android.os.Build;
import android.util.Log;

import java.lang.reflect.Method;


/**
 * @author zhangzhipeng
 * @date 2023/7/5
 */
public class BootstrapClass {
    private static final String TAG = BootstrapClass.class.getName();
    private static Object sVmRuntime;
    private static Method sSetHiddenApiExemptions;

    static {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            try {
                Method forName = Class.class.getDeclaredMethod("forName", String.class);
                Method getDeclaredMethod = Class.class.getDeclaredMethod("getDeclaredMethod", String.class, Class[].class);

                Class<?> vmRuntimeClass = (Class<?>) forName.invoke(null, "dalvik.system.VMRuntime");
                Method getRuntime = (Method) getDeclaredMethod.invoke(vmRuntimeClass, "getRuntime", null);
                sSetHiddenApiExemptions = (Method) getDeclaredMethod.invoke(vmRuntimeClass, "setHiddenApiExemptions", new Class[]{String[].class});
                assert getRuntime != null;
                sVmRuntime = getRuntime.invoke(null);
            } catch (Throwable var4) {
                Log.w(TAG, "reflect bootstrap fail", var4);
            }
        }
    }

    public static boolean exempt(String method) {
        return exempt(new String[]{method});
    }

    public static boolean exempt(String... methods) {
        if (sVmRuntime == null || sSetHiddenApiExemptions == null) {
            return false;
        }

        try {
            sSetHiddenApiExemptions.invoke(sVmRuntime, new Object[]{methods});
            return true;
        } catch (Throwable e) {
            e.printStackTrace();
            return false;
        }
    }

    public static boolean exemptAll() {
        return exempt(new String[]{"L"});
    }
}
