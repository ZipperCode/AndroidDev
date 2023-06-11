// IProcessHandler.aidl
package com.zipper.lib.aspectj.ipc;

import com.zipper.lib.aspectj.ipc.IpcRequest;
import com.zipper.lib.aspectj.ipc.IpcResponse;

interface IProcessHandler {

    IpcResponse call(in IpcRequest request);

    oneway void asyncCall(in IpcRequest request);
}