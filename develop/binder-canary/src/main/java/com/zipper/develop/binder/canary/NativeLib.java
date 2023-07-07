package com.zipper.develop.binder.canary;

public class NativeLib {

    // Used to load the 'canary' library on application startup.
    static {
        System.loadLibrary("canary");
    }

    /**
     * A native method that is implemented by the 'canary' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}