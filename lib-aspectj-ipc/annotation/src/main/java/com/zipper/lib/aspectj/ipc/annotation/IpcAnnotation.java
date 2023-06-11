package com.zipper.lib.aspectj.ipc.annotation;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.METHOD)
public @interface IpcAnnotation {

    /**
     * @return 单例名称
     */
    String singleton() default "";

    /**
     * @return 是否异步处理
     */
    boolean async() default false;
}
