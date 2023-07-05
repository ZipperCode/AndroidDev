package com.zipper.hiddenapibypass.unsafe;

import android.app.Application;
import android.content.Context;
import android.os.Build;

import androidx.test.filters.SdkSuppress;
import androidx.test.platform.app.InstrumentationRegistry;
import androidx.test.ext.junit.runners.AndroidJUnit4;

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;
import org.junit.runner.RunWith;

import static org.junit.Assert.*;

import java.lang.reflect.Executable;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.List;
import java.util.Optional;

import dalvik.system.VMRuntime;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
@SdkSuppress(minSdkVersion = Build.VERSION_CODES.P)
public class ExampleInstrumentedTest {

    /**
     * 豁免隐藏Api
     */
    @Test
    public void setHiddenApiExemptions() throws NoSuchFieldException {
        HiddenApiBypass.setHiddenApiExemptions("L");
        Field mLoadedApk = Application.class.getDeclaredField("mLoadedApk");
        System.out.println(mLoadedApk);
        assertNotNull(mLoadedApk);
    }

    @Test
    public void reflectHiddenField() throws Exception {
        Context appContext = InstrumentationRegistry.getInstrumentation().getTargetContext();
        {
            System.out.println("warning access hidden method");
            Method getUserId = appContext.getClass().getDeclaredMethod("getUserId");
            System.out.println(getUserId);
        }

        {
            System.out.println("no waring access hidden method");
            Method getUserId = HiddenApiBypass.getDeclaredMethod(Context.class, "getUserId");
            System.out.println(getUserId);
        }

    }
}