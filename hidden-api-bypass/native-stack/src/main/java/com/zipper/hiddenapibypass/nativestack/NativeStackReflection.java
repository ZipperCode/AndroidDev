package com.zipper.hiddenapibypass.nativestack;

public class NativeStackReflection {

    static {
        System.loadLibrary("nativestack");
    }

    public static boolean exemptAll(){
        return exempt(new String[]{"L"});
    }

    public static native boolean exempt(String[] args);
}