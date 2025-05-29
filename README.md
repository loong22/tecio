### tecio 源码
源代码从SU2中获取:

https://github.com/su2code/SU2/tree/master/externals/tecio

已下载并压缩, 见`./tecio.tar.gz`

### 在Windows平台使用GCC-14.2.0编译tecio静态库
源码路径为`./teciosrc`, 基于`./CMakeLists.txt`使用CMake进行编译

### 附上已编译的二进制文件
Windows GCC-14.2.0:
`./lib/libtecio.a`

Windows MSVC:
`./tecplot2023/include` `./tecplot2023/lib`

### 许可证
详见: `./tecio_license_agreement.txt`文件