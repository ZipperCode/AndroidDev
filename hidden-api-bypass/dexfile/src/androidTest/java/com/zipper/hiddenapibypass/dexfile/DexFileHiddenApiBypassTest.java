package com.zipper.hiddenapibypass.dexfile;

import android.app.Application;
import android.content.Context;

import androidx.test.platform.app.InstrumentationRegistry;
import androidx.test.ext.junit.runners.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

import static org.junit.Assert.*;

import java.lang.reflect.Field;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
public class DexFileHiddenApiBypassTest {

    @Test
    public void setHiddenApiExemptions() throws NoSuchFieldException {
        System.out.println("access hiddenapi mLoadedApk 1");
        Field mLoadedApk = Application.class.getDeclaredField("mLoadedApk");

        Context appContext = InstrumentationRegistry.getInstrumentation().getTargetContext();
        int ret = Reflection.unseal(appContext);
        assertEquals(ret, 0);

        System.out.println("access hiddenapi mLoadedApk 2");
        mLoadedApk = Application.class.getDeclaredField("mLoadedApk");

    }
}