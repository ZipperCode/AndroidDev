package com.zipper.hiddenapibypass.dexfile;

import android.annotation.TargetApi;
import android.content.Context;
import android.os.Build;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Method;
import dalvik.system.DexFile;

/**
 * @author zhangzhipeng
 * @date 2023/7/5
 */
@TargetApi(Build.VERSION_CODES.P)
public final class Reflection {

    public static int unseal(Context context) {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.P){
            return 0;
        }

        if (unsealByFile(context)) {
            return 0;
        }

        return -1;
    }


    private static boolean unsealByFile(Context context) {
        File cacheFile = new File(context.getFilesDir(), "reflect");
        if (!cacheFile.exists()) {
            try(InputStream in = context.getAssets().open("boot-reflect.dex")) {
                try(FileOutputStream fos = new FileOutputStream(cacheFile)) {
                    byte[] buf = new byte[in.available()];
                    if (in.read(buf) > 0) {
                        fos.write(buf);
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
                return false;
            }
        }

        try {
            DexFile dexFile = new DexFile(cacheFile);
            Class<?> aClass = dexFile.loadClass(BootstrapClass.class.getName(), null);
            Method exemptAll = aClass.getDeclaredMethod("exemptAll");
            return Boolean.TRUE.equals(exemptAll.invoke(null));
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }
}
