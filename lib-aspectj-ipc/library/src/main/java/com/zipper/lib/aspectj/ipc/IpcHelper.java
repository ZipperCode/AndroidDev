package com.zipper.lib.aspectj.ipc;

import android.annotation.SuppressLint;
import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.os.IBinder;
import android.os.IInterface;
import android.text.TextUtils;
import android.util.Log;
import android.util.Pair;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.google.gson.Gson;
import com.zipper.lib.aspectj.ipc.annotation.Ipc;

import java.io.Serializable;
import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.Map;

public class IpcHelper implements IIpcLog {

    private static final String AUTHORITY_FORMAT = "content://%s.aspectj.ipc.provider";
    public static final String REQUEST_KEY = "ipc|request|key";
    public static final String RESPONSE_KEY = "ipc|response|key";

    public static final String CALL_METHOD = "IPC_METHOD_1";

    public static final Gson sGson = new Gson();

    Context mContext;

    boolean initialized;

    String mPackageName;

    String mProcessName;

    @Nullable
    String mCustomAuthority;

    boolean debug;

    final Dispatcher dispatcher = new Dispatcher(this);

    public void init(Context context, boolean debug) {
        log("IpcHelper init debug = %s", debug);
        mContext = context.getApplicationContext();
        mPackageName = context.getPackageName();
        mProcessName = Utils.getProcessName(context);
        this.debug = debug;
        initialized = true;
    }

    public void init(Context context, boolean debug, String authority) {
        log("IpcHelper init debug = %s, authority = %s", debug, authority);
        mContext = context.getApplicationContext();
        mPackageName = context.getPackageName();
        mProcessName = Utils.getProcessName(context);
        mCustomAuthority = authority;
        this.debug = debug;
        initialized = true;
    }

    public boolean isMainProcess() {
        if (initialized){
            return TextUtils.equals(mProcessName, mPackageName);
        }
        return false;
    }

    @Nullable
    public IpcResponse request(IpcRequest request) {
        if (!initialized){
            lw("IpcHelper communicate fail not init");
            return null;
        }

        Bundle requestBundle = new Bundle();
        requestBundle.putParcelable(REQUEST_KEY, request);
        Bundle responseBundle = callIpcProvider(CALL_METHOD, requestBundle);
        if (responseBundle == null) {
            lw("IpcHelper communicate fail response is null");
            return null;
        }
        responseBundle.setClassLoader(IpcResponse.class.getClassLoader());
        return responseBundle.getParcelable(RESPONSE_KEY);
    }

    public Bundle callIpcProvider(@NonNull String method, Bundle extras) {
        String authority;
        if (!TextUtils.isEmpty(mCustomAuthority)) {
            authority = mCustomAuthority;
        } else {
            authority = String.format(AUTHORITY_FORMAT, mPackageName);
        }
        log("IpcHelper callIpcProvider start... authority: %s method = %s", authority, method);
        return mContext.getContentResolver().call(Uri.parse(authority), method,"", extras);
    }

    @Nullable
    public IpcResponse handleRequest(@NonNull IpcRequest request) {
        long start = System.currentTimeMillis();
        try {
            Class<?>[] paramType = request.getParamTypes();
            Object[] args = request.getArgs();
            Class<?> aClass = Class.forName(request.className);
            Object target = null;
            if (!TextUtils.isEmpty(request.singletonMethodName)) {
                Method method = aClass.getMethod(request.singletonMethodName);
                target = method.invoke(null);
            }

            Object result = null;
            if (args == null) {
                Method method = aClass.getMethod(request.methodName);
                result = method.invoke(target);
            } else {
                Method method = aClass.getMethod(request.methodName, paramType);
                result = method.invoke(target, args);
            }

            if (result == null) {
                return new IpcResponse(request.uuid, null);
            }

            return new IpcResponse(request.uuid, sGson.toJson(result));
        } catch (Exception e) {
            le("2 Failed to %s", Log.getStackTraceString(e));
        } finally {
            log("handleRequest 耗时 = " + (System.currentTimeMillis() - start));
        }
        return null;
    }

    public IProcessHandler getProcessHandler() {
        if (!initialized){
            lw("IpcHelper communicate fail not init");
            return null;
        }

        IBinder remoteHandler = dispatcher.getRemoteHandler(Ipc.DEFAULT_HANDLE_KEY);
        if (remoteHandler == null) {
            lw("IpcHelper communicate fail getRemoteHandler is null");
            return null;
        }
        return IProcessHandler.Stub.asInterface(remoteHandler);
    }


    private static class Holder {
        @SuppressLint("StaticFieldLeak")
        private static final IpcHelper INSTANCE = new IpcHelper();
    }

    private IpcHelper(){}

    public static IpcHelper getInstance() {
        return Holder.INSTANCE;
    }
}
