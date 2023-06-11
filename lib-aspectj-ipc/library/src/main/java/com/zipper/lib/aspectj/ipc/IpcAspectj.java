package com.zipper.lib.aspectj.ipc;

import android.text.TextUtils;
import android.util.Log;

import com.zipper.lib.aspectj.ipc.annotation.Ipc;
import com.zipper.lib.aspectj.ipc.annotation.IpcAnnotation;

import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Pointcut;
import org.aspectj.lang.reflect.MethodSignature;

import java.lang.reflect.Method;
import java.util.UUID;

@Aspect
public class IpcAspectj {

    @Pointcut("execution(@com.zipper.lib.aspectj.ipc.annotation.IpcAnnotation * *(..))")
    public void pointCut(){
    }

    @Pointcut("execution(@com.zipper.lib.aspectj.ipc.annotation.Ipc * *(..))")
    public void pointCut2(){
    }

    @Around("pointCut()")
    public Object around(ProceedingJoinPoint joinPoint) {
        Object result = null;
        long start = System.currentTimeMillis();
        try {
            if (IpcHelper.getInstance().isMainProcess()) {
                result = joinPoint.proceed();
                return result;
            }
            Log.e(IIpcLog.TAG, "跨进程执行 开始");
            MethodSignature signature = (MethodSignature) joinPoint.getSignature();
            // 调用类
            String callClassName = signature.getDeclaringTypeName();
            // 调用的方法
            Method method = signature.getMethod();
            // 方法名
            String methodName = method.getName();
            // 参数类型
            Class<?>[] parameterTypes = signature.getParameterTypes();
            // 参数
            Object[] args = joinPoint.getArgs();

            String reqUuid = UUID.randomUUID().toString();
            IpcAnnotation annotation = method.getAnnotation(IpcAnnotation.class);
            String singletonMethodName = null;
            if (annotation != null) {
                singletonMethodName = annotation.singleton();
            }
            IpcRequest request = new IpcRequest(reqUuid, callClassName, methodName, args, parameterTypes, singletonMethodName);
            Log.d(IIpcLog.TAG, "开始调用 methodItem = " + request);
            IpcResponse response = IpcHelper.getInstance().request(request);
            if (response == null) {
                return null;
            }
            if (!TextUtils.equals(reqUuid, response.originUuid)){
                Log.w(IIpcLog.TAG,"around request uuid mismatch reqUuid = " + reqUuid + ", respUUid = " + response.originUuid);
            }
            String resultPayload = response.resultPayload;
            if (TextUtils.isEmpty(resultPayload)) {
                return null;
            }
            // 返回值类型
            Class<?> returnType = signature.getReturnType();
            result = IpcHelper.sGson.fromJson(resultPayload, returnType);
        } catch (Throwable e) {
            e.printStackTrace();
            Log.e(IIpcLog.TAG,"Log = " + Log.getStackTraceString(e));
        } finally {
            Log.e(IIpcLog.TAG,"final 耗时：" + (System.currentTimeMillis() - start) + " Result = " + result);
        }
        return result;
    }

    @Around("pointCut2()")
    public Object around2(ProceedingJoinPoint joinPoint) {
        Object result = null;
        if (IpcHelper.getInstance().isMainProcess()) {
            try {
                result = joinPoint.proceed();
            } catch (Throwable e) {
                throw new RuntimeException(e);
            }
            return result;
        }
        long start = System.currentTimeMillis();
        try {
            MethodSignature signature = (MethodSignature) joinPoint.getSignature();
            // 调用类
            String callClassName = signature.getDeclaringTypeName();
            // 调用的方法
            Method method = signature.getMethod();
            // 方法名
            String methodName = method.getName();
            // 参数类型
            Class<?>[] parameterTypes = signature.getParameterTypes();
            // 参数
            Object[] args = joinPoint.getArgs();

            Ipc annotation = method.getAnnotation(Ipc.class);
            boolean isAsync = false;
            String key = Ipc.DEFAULT_HANDLE_KEY;
            String singletonMethodName = null;
            if (annotation != null) {
                singletonMethodName = annotation.singleton();
                isAsync = annotation.async();
                key = annotation.key();
            }

            IProcessHandler processHandler = IpcHelper.getInstance().getProcessHandler();
            if (processHandler == null) {
                Log.w(IIpcLog.TAG, "unable get processHandler interface no call");
                return null;
            }
            Log.e(IIpcLog.TAG,"Aspect2 获取Handler耗时1：" + (System.currentTimeMillis() - start));
            String reqUuid = UUID.randomUUID().toString();
            IpcRequest request = new IpcRequest(reqUuid, callClassName, methodName, args, parameterTypes, singletonMethodName);
            if (IpcHelper.getInstance().debug) {
                Log.d(IIpcLog.TAG, "开始调用 request = " + request);
            }
            Log.e(IIpcLog.TAG,"Aspect2 创建Request耗时2：" + (System.currentTimeMillis() - start));
            if (isAsync) {
                processHandler.asyncCall(request);
            } else {
                IpcResponse response = processHandler.call(request);
                if (response == null) {
                    return null;
                }
                if (!TextUtils.equals(reqUuid, response.originUuid)){
                    Log.w(IIpcLog.TAG,"around request uuid mismatch reqUuid = " + reqUuid + ", respUUid = " + response.originUuid);
                }
                String resultPayload = response.resultPayload;
                if (TextUtils.isEmpty(resultPayload)) {
                    return null;
                }
                // 返回值类型
                Class<?> returnType = signature.getReturnType();
                result = IpcHelper.sGson.fromJson(resultPayload, returnType);
            }
        } catch (Exception e) {
            e.printStackTrace();
            Log.e(IIpcLog.TAG,"Log = " + Log.getStackTraceString(e));
        } finally {
            Log.e(IIpcLog.TAG,"Aspect2 耗时：" + (System.currentTimeMillis() - start) + " Result = " + result);
        }
        return result;
    }
}
