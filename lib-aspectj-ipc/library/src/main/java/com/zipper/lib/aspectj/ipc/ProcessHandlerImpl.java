package com.zipper.lib.aspectj.ipc;

import android.os.RemoteException;

public class ProcessHandlerImpl extends IProcessHandler.Stub {

    @Override
    public IpcResponse call(IpcRequest request) throws RemoteException {
        return IpcHelper.getInstance().handleRequest(request);
    }

    @Override
    public void asyncCall(IpcRequest request) throws RemoteException {
        IpcHelper.getInstance().handleRequest(request);
    }
}
