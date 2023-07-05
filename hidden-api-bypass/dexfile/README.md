## 参考 [FreeReflection](https://github.com/tiann/FreeReflection)

### 原理
- DexFile
- Runtime#setHiddenApiExemptions

通过使用DexFile加载生成的dex，反射其中的`BootstrapClass`类初始化时通过双重反射机制
查找Runtime类的setHiddenApiExemptions进行豁免