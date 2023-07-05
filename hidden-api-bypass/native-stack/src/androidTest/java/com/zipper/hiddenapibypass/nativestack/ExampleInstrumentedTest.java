package com.zipper.hiddenapibypass.nativestack;

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
public class ExampleInstrumentedTest {
    @Test
    public void useAppContext() {
        // Context of the app under test.
        Context appContext = InstrumentationRegistry.getInstrumentation().getTargetContext();
        assertEquals("com.zipper.hiddenapibypass.nativestack.test", appContext.getPackageName());
    }

    @Test
    public void setHiddenApiExemptions() throws NoSuchFieldException {
        System.out.println("access hiddenapi mLoadedApk 1");
        Field mLoadedApk = Application.class.getDeclaredField("mLoadedApk");
        boolean ret = NativeStackReflection.exemptAll();
        assertTrue(ret);
        System.out.println("access hiddenapi mLoadedApk 2");
        mLoadedApk = Application.class.getDeclaredField("mLoadedApk");

    }
}