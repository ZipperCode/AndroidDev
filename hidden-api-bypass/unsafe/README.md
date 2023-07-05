
## 参考 [LSPosed-AndroidHiddenApiBypass](https://github.com/LSPosed/AndroidHiddenApiBypass/tree/main)

注: *代码均从AndroidHiddenApiBypass拷贝而来* 

### 原理
- UnSafe
- Runtime#setHiddenApiExemptions

不直接通过反射的方式进行调用，而是通过映射自定义的Class，
而是通过UnSafe将源类的Method、Field的内存数据指针指向映射类的Method、Filed中调用