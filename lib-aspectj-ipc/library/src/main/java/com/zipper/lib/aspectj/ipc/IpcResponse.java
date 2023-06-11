package com.zipper.lib.aspectj.ipc;

import android.os.Parcel;
import android.os.Parcelable;

import com.google.gson.annotations.SerializedName;

public class IpcResponse implements Parcelable {
    @SerializedName("uuid")
    public String originUuid;
    @SerializedName("payload")
    public String resultPayload;

    public IpcResponse(String uuid, String resultPayload) {
        this.originUuid = uuid;
        this.resultPayload = resultPayload;
    }

    protected IpcResponse(Parcel in) {
        originUuid = in.readString();
        resultPayload = in.readString();
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(originUuid);
        dest.writeString(resultPayload);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Creator<IpcResponse> CREATOR = new Creator<IpcResponse>() {
        @Override
        public IpcResponse createFromParcel(Parcel in) {
            return new IpcResponse(in);
        }

        @Override
        public IpcResponse[] newArray(int size) {
            return new IpcResponse[size];
        }
    };
}
