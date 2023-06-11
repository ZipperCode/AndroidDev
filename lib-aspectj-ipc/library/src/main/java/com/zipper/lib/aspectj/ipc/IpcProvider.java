package com.zipper.lib.aspectj.ipc;

import android.content.ContentProvider;
import android.content.ContentValues;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.text.TextUtils;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;


public class IpcProvider extends ContentProvider implements IIpcLog {

    @Override
    public boolean onCreate() {
        return true;
    }

    @Nullable
    @Override
    public Bundle call(@NonNull String authority, @NonNull String method, @Nullable String arg, @Nullable Bundle extras) {
        log("call authority = %s, method = %s", authority, method);
        long start = System.currentTimeMillis();
        try {
            if (extras == null) {
                return null;
            }
            if (TextUtils.equals(IpcHelper.CALL_METHOD, method)) {
                extras.setClassLoader(IpcRequest.class.getClassLoader());
                IpcRequest request = extras.getParcelable(IpcHelper.REQUEST_KEY);
                if (request != null) {
                    Bundle bundle = new Bundle();
                    IpcResponse ipcResponse = IpcHelper.getInstance().handleRequest(request);
                    bundle.putParcelable(IpcHelper.RESPONSE_KEY, ipcResponse);
                    return bundle;
                }

            } else if (TextUtils.equals(Dispatcher.FETCH_HANDLER_METHOD, method)) {
                String handleKey = extras.getString(Dispatcher.HANDLER_KEY);
                if (TextUtils.isEmpty(handleKey)) {
                    return null;
                }
                return IpcHelper.getInstance().dispatcher.getInterface(handleKey);
            }

            return super.call(authority, method, arg, extras);
        } finally {
            le("call 耗时 = %s", System.currentTimeMillis() - start);
        }
    }

    @Nullable
    @Override
    public Cursor query(@NonNull Uri uri, @Nullable String[] projection, @Nullable String selection, @Nullable String[] selectionArgs, @Nullable String sortOrder) {
        return null;
    }

    @Nullable
    @Override
    public String getType(@NonNull Uri uri) {
        return null;
    }

    @Nullable
    @Override
    public Uri insert(@NonNull Uri uri, @Nullable ContentValues values) {
        return null;
    }

    @Override
    public int delete(@NonNull Uri uri, @Nullable String selection, @Nullable String[] selectionArgs) {
        return 0;
    }


    @Override
    public int update(@NonNull Uri uri, @Nullable ContentValues values, @Nullable String selection, @Nullable String[] selectionArgs) {
        return 0;
    }
}
