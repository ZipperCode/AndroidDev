/*
 * Copyright (C) 2021-2023 LSPosed
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.zipper.hiddenapibypass.unsafe;

import java.lang.invoke.MethodHandleInfo;
import java.lang.invoke.MethodType;
import java.lang.reflect.Member;

@SuppressWarnings("unused")
public class Helper {
    /**
     * 对应 java.lang.invoke.MethodHandle
     */
    static public class MethodHandle {
        private final MethodType type = null;
        private MethodType nominalType;
        private MethodHandle cachedSpreadInvoker;
        protected final int handleKind = 0;

        // The ArtMethod* or ArtField* associated with this method handle (used by the runtime).
        protected final long artFieldOrMethod = 0;
    }

    /**
     * 对应 java.lang.invoke.MethodHandleImpl
     */
    static final public class MethodHandleImpl extends MethodHandle {
        private final MethodHandleInfo info = null;
    }

    /**
     * 对应 java.lang.invoke.MethodHandleImpl$HandleInfo
     */
    static final public class HandleInfo {
        private final Member member = null;
        private final MethodHandle handle = null;
    }

    /**
     * 对应java.lang.Class
     */
    static final public class Class {
        private transient ClassLoader classLoader;
        /**
         * 数组类InstanceOf比较的类型
         */
        private transient java.lang.Class<?> componentType;
        private transient Object dexCache;
        private transient Object extData;
        /**
         * 接口表，包含接口类和接口数据对，未实现任何接口内容为空
         */
        private transient Object[] ifTable;
        private transient String name;
        private transient java.lang.Class<?> superClass;
        /**
         * 虚方法表
         */
        private transient Object vtable;
        /**
         * 实例字段，描述对象的实例属性布局，只包含当前类声明的属性，超类声明的在超类的Class.iFields
         * 所有引用对象的实例字段都位于此字段为开头的列表
         */
        private transient long iFields;
        /**
         * 方法数量基址，当前类的所有方法都通过这个基址分配
         */
        private transient long methods;
        /**
         * 静态字段，同iFields
         */
        private transient long sFields;
        private transient int accessFlags;
        private transient int classFlags;
        private transient int classSize;
        private transient int clinitThreadId;
        private transient int dexClassDefIndex;
        private transient volatile int dexTypeIndex;
        /**
         * 对象引用实例字段的数量
         */
        private transient int numReferenceInstanceFields;
        /**
         * 静态字段的数量
         */
        private transient int numReferenceStaticFields;
        private transient int objectSize;
        private transient int objectSizeAllocFastPath;
        private transient int primitiveType;
        /**
         * iField 偏移量
         */
        private transient int referenceInstanceOffsets;
        private transient int status;
        private transient short copiedMethodsOffset;
        /**
         * 定义的虚方法在方法数组中的偏移量
         */
        private transient short virtualMethodsOffset;
    }

    /**
     * 对于java.lang.reflect.AccessibleObject
     */
    static public class AccessibleObject {
        private boolean override;
    }

    /**
     * 对应java.lang.reflect.Executable
     */
    static final public class Executable extends AccessibleObject {
        private Class declaringClass;
        private Class declaringClassOfOverriddenMethod;
        private Object[] parameters;
        private long artMethod;
        private int accessFlags;
    }

    /**
     * 占位类，主要用来获取字段和方法的偏移
     */
    @SuppressWarnings("EmptyMethod")
    public static class NeverCall {
        private static void a() {
        }

        private static void b() {
        }

        private static int s;
        private static int t;
        private int i;
        private int j;
    }

    /**
     * invoke占位类，使用反射调用的时候，获取到当前类的MethodHandle#invoke方法
     * 再使用内存拷贝从而构造一个与反射方法一样的Method
     */
    public static class InvokeStub {
        private static Object invoke(Object... args) {
            throw new IllegalStateException("Failed to invoke the method");
        }

        private InvokeStub(Object... args) {
            throw new IllegalStateException("Failed to new a instance");
        }
    }
}
