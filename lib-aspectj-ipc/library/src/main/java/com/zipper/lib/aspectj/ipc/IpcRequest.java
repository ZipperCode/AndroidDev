package com.zipper.lib.aspectj.ipc;

import android.os.Parcel;
import android.os.Parcelable;
import android.text.TextUtils;
import android.util.Log;

import androidx.annotation.Nullable;

import java.io.Serializable;
import java.util.Arrays;
import java.util.Objects;

public class IpcRequest implements Parcelable {
    public final String uuid;
    public final String className;
    public final String methodName;
    public String[] argsString;
    public final Serializable[] paramTypes;
    @Nullable
    public String singletonMethodName;

    public int hashCode = 0;

    public IpcRequest(String uuid, String className, String methodName, Object[] args, Class<?>[] paramType, String singletonMethodName){
        this.uuid = uuid;
        this.className = className;
        this.methodName = methodName;
        this.serialArgs(args);
        this.paramTypes = paramType;
        this.singletonMethodName = singletonMethodName;
    }

    protected IpcRequest(Parcel in) {
        this.uuid = in.readString();
        this.className = in.readString();
        this.methodName = in.readString();
        int argLength = in.readInt();
        if (argLength > 0) {
            this.argsString = new String[argLength];
            for (int i = 0; i < argLength; i++) {
                argsString[i] = in.readString();
            }
        } else {
            this.argsString = new String[0];
        }

        int argTypeLength = in.readInt();
        if (argTypeLength > 0) {
            paramTypes = new Serializable[argTypeLength];
            for (int i = 0; i < argTypeLength; i++) {
                paramTypes[i] = in.readSerializable();
            }
        } else {
            paramTypes = new Serializable[0];
        }
        this.singletonMethodName = in.readString();
        this.hashCode = in.readInt();
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(uuid);
        dest.writeString(className);
        dest.writeString(methodName);
        if (argsString == null || argsString.length == 0) {
            dest.writeInt(0);
        } else {
            dest.writeInt(argsString.length);
            for (String argString : argsString) {
                dest.writeString(argString);
            }
        }

        if (paramTypes == null || paramTypes.length == 0) {
            dest.writeInt(0);
        } else {
            dest.writeInt(paramTypes.length);
            for (Serializable serializable : paramTypes) {
                dest.writeSerializable(serializable);
            }
        }
        dest.writeString(singletonMethodName);
        dest.writeInt(hashCode());
    }

    private void serialArgs(Object[] args) {
        long start = System.currentTimeMillis();
        if (args != null && args.length > 0) {
            this.argsString = new String[args.length];
            for (int i = 0; i < args.length; i++) {
                Object arg = args[i];
                this.argsString[i] = IpcHelper.sGson.toJson(arg);
            }
        }
        Log.e(IIpcLog.TAG, "Json 序列化耗时 = " + (System.currentTimeMillis() - start));
    }

    public Class<?>[] getParamTypes() {
        if (paramTypes == null || paramTypes.length == 0) {
            return null;
        }
        Class<?>[] result = new Class<?>[paramTypes.length];
        for (int i = 0; i < paramTypes.length; i++) {
            result[i] = (Class<?>) paramTypes[i];
        }
        return result;
    }

    public Object[] getArgs() {
        if (argsString == null || paramTypes == null) {
            return null;
        }
        if (argsString.length != paramTypes.length) {
            return null;
        }
        Object[] args = new Object[argsString.length];
        for (int i = 0; i < argsString.length; i++) {
            String arg = argsString[i];
            if (TextUtils.isEmpty(arg)) {
                continue;
            }
            try {
                Object obj = IpcHelper.sGson.fromJson(arg, (Class<?>) paramTypes[i]);
                args[i] = obj;
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return args;
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public String toString() {
        return "IpcRequest{" +
                "uuid='" + uuid + '\'' +
                ", className='" + className + '\'' +
                ", methodName='" + methodName + '\'' +
                ", argsString=" + Arrays.toString(argsString) +
                ", paramTypes=" + Arrays.toString(paramTypes) +
                ", singletonMethodName='" + singletonMethodName + '\'' +
                ", hashCode= [" + hashCode() + "]" +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        IpcRequest request = (IpcRequest) o;
        return uuid.equals(request.uuid) && className.equals(request.className) && methodName.equals(request.methodName) && Arrays.equals(argsString, request.argsString) && Arrays.equals(paramTypes, request.paramTypes) && Objects.equals(singletonMethodName, request.singletonMethodName);
    }

    @Override
    public int hashCode() {
        int result = Objects.hash(uuid, className, methodName, singletonMethodName);
        result = 31 * result + Arrays.hashCode(argsString);
        result = 31 * result + Arrays.hashCode(paramTypes);
        return result;
    }

    public static final Creator<IpcRequest> CREATOR = new Creator<IpcRequest>() {
        @Override
        public IpcRequest createFromParcel(Parcel in) {
            return new IpcRequest(in);
        }

        @Override
        public IpcRequest[] newArray(int size) {
            return new IpcRequest[size];
        }
    };
}
