package com.zipper.lib.aspectj.ipc.sample;

import android.util.Log;

import com.zipper.lib.aspectj.ipc.IIpcLog;
import com.zipper.lib.aspectj.ipc.annotation.Ipc;
import com.zipper.lib.aspectj.ipc.annotation.IpcAnnotation;

import java.util.List;
import java.util.Map;

public class Test {

    @Ipc
    public static void callMain(){
        Log.e(IIpcLog.TAG, "callMain");
    }

    @IpcAnnotation
    public static void callMain1(int arg1, float arg2, String arg3){
        Log.e(IIpcLog.TAG, "callMain1");
    }

    @IpcAnnotation
    public static int callMain2(){
        Log.e(IIpcLog.TAG, "callMain1");
        return 100;
    }

    @IpcAnnotation
    public static void callMain3(TestBeanA beanA){
        Log.e(IIpcLog.TAG, "callMain3");
    }

    @IpcAnnotation
    public static void callMain4(List<TestBeanA> beanA){
        Log.e(IIpcLog.TAG, "callMain4 param = " + beanA);
    }

    @Ipc
    public static void callMain5(Map<String, TestBeanA> beanA){
        Log.e(IIpcLog.TAG, "callMain5 param = " + beanA);
    }
}
