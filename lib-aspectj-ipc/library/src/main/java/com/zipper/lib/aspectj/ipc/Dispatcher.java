package com.zipper.lib.aspectj.ipc;

import android.os.Bundle;
import android.os.IBinder;
import android.os.IInterface;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.zipper.lib.aspectj.ipc.annotation.Ipc;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

class Dispatcher {

    public static final String FETCH_HANDLER_METHOD = "ipc|handler|method|key";
    public static final String HANDLER_KEY = "ipc|handler|key";

    private final IpcHelper mIpcHelper;

    private final Map<String, IBinder> mRemoteInterfaceMap = new ConcurrentHashMap<>();

    private final Map<String, IInterface> mInterfaces = new ConcurrentHashMap<>();

    public Dispatcher(IpcHelper mIpcHelper) {
        this.mIpcHelper = mIpcHelper;
        mInterfaces.put(Ipc.DEFAULT_HANDLE_KEY, new ProcessHandlerImpl());
    }

    @Nullable
    public IBinder getRemoteHandler(@NonNull String key) {
        IBinder binder = mRemoteInterfaceMap.get(key);

        if (binder == null || !binder.isBinderAlive()) {
            Bundle bundle = new Bundle();
            bundle.putString(HANDLER_KEY, key);
            Bundle resultBundle = mIpcHelper.callIpcProvider(FETCH_HANDLER_METHOD, bundle);
            if (resultBundle != null) {
                binder = resultBundle.getBinder(key);
                if (binder != null) {
                    mRemoteInterfaceMap.put(key, binder);
                }
            }
        }

        return binder;
    }

    @Nullable
    public Bundle getInterface(@NonNull String key) {
        IInterface iInterface = mInterfaces.get(key);
        if (iInterface != null) {
            Bundle bundle = new Bundle();
            bundle.putBinder(key, iInterface.asBinder());
            return bundle;
        }
        return null;
    }

}
