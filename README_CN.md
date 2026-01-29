# StarMap

StarMap 是一个基于 C/C++ 开发的星图/天文可视化引擎。

本项目 Fork 自 [stellarium-web-engine](https://github.com/Stellarium/stellarium-web-engine)。可运行示例请参考 [StarMapExample](https://github.com/dwarflab/StarMapExample)


**项目改造说明：**
原项目主要设计运行于 Web 环境 (通过 Emscripten 编译)，本项目对其进行了深度的原生化改造，包括但不限于：
1.  **移除 Web 依赖**：剥离了与浏览器环境强耦合的 Emscripten 接口代码。
2.  **多平台支持**：重构了构建系统 (CMake)，使其能够原生编译并运行在 **Windows (MinGW)**, **Android**, **iOS** 等主流原生平台。
3.  **兼容性修复**：修复了在不同编译器 (GCC, Clang) 下的兼容性问题。

## 依赖项 (Dependencies)

### 系统/外部依赖 (System Requirements)
编译本项目需要以下外部环境和库的支持：

*   **CMake**: 版本 3.8 或更高
*   **C/C++ 编译器**: 支持 C99/C++11 标准 (GCC, Clang, MinGW 等, MSVC 编译器不支持)
*   **ZLIB**: 必须 (用于数据压缩/解压)
*   **OpenGL ES 2.0**: 渲染后端

#### Windows (MinGW) 特定依赖
在 Windows 下使用 MinGW 编译时，还需要手动提供以下库：
*   **Pthread**: 线程支持库
*   **Regex**: 正则表达式库 (如 `libtre`, `gnurx` 等)
*   **EGL/GLESv2**: OpenGL ES 2.0 的 Windows 实现 (通常使用 ANGLE 库)

## 构建指南 (Build Instructions)

### Windows (MinGW)

在 Windows 上使用 MinGW 构建时，由于没有统一的包管理器，通常需要手动指定依赖库的路径。

1.  **准备环境**: 安装 MinGW-w64 工具链。
2.  **准备依赖**: 编译或下载 Zlib, Pthread, Regex, EGL/GLESv2(一般是 ANGLE) 的库 (包含头文件和库文件)。
3.  **配置 CMake**:

    创建一个构建目录 (例如 `build/`)，并在该目录下运行 CMake。你需要设置相关的变量指向你的依赖库路径。

    **示例命令**:

    ```bash
    mkdir build
    cd build
    cmake -G "MinGW Makefiles" .. ^
        -DZLIB_ROOT="E:/libs/zlib" ^
        -DREGEX_INCLUDE_DIRS="E:/libs/regex/include" ^
        -DREGEX_LIBRARYS="E:/libs/regex/lib/libregex.a" ^
        -DEGL_LIBRARYS="E:/libs/angle/libGLESv2.dll.a" ^
        -DPTHREAD_INCLUDE_DIRS="E:/libs/pthread/include" ^
        -DPTHREAD_LIBRARYS="E:/libs/pthread/lib/libpthread.a"

    mingw32-make
    ```

    **CMake 变量说明**:
    
    | 变量名 | 说明 |
    | :--- | :--- |
    | `ZLIB_ROOT` | Zlib 库的根目录 |
    | `REGEX_INCLUDE_DIRS` | 正则表达式库 (regex.h) 的头文件目录 |
    | `REGEX_LIBRARYS` | 正则表达式库文件路径 |
    | `EGL_LIBRARYS` | GLESv2 库文件路径 (通常需要链接 libGLESv2) |
    | `PTHREAD_INCLUDE_DIRS` | Pthread 头文件目录 |
    | `PTHREAD_LIBRARYS` | Pthread 库链接文件路径 |

### Android

对于 Android 平台，需要指定 NDK 提供的工具链文件。

`CMakeLists.txt` 会自动检测 `ANDROID` 平台并配置：
*   链接 Android 系统自带的 `GLESv2` 库。
*   启用针对 Android 的编译优化标志 (`-Os`, `-Wl,--gc-sections` 等)。
*   需要自行指定目标 ABI。

### iOS

需要 macOS 环境和 Xcode 工具链。
CMake 会自动查找并链接 iOS 系统的框架：`OpenGLES`, `CoreFoundation`, `Security`。

**Framework 配置**:
为了在 iOS 上成功编译并使用，通常需要在 `CMakeLists.txt` 中将目标属性配置为生成 Framework。请根据项目实际情况修改以下参数：

```cmake
set_target_properties(${PRJ_NAME} PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION C
  MACOSX_FRAMEWORK_IDENTIFIER "com.your.bundle.identifier"  # 修改为你的 Bundle ID
  MACOSX_FRAMEWORK_BUNDLE_VERSION "1.0.0"                   # 修改为你的版本号
  MACOSX_FRAMEWORK_SHORT_VERSION_STRING "1.0.0"
  VERSION 1.0.0
  SOVERSION 1.0.0
  PUBLIC_HEADER "${PROJECT_HEADER}"                         # 指定公开头文件
  XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"              # 1=iPhone, 2=iPad
)
```

## 代码结构

*   `src/`: 核心源代码
    *   `algos/`: 天文算法实现
    *   `modules/`: 功能模块 (大气、彗星、卫星等)
    *   `utils/`: 通用工具函数
*   `ext_src/`: 第三方依赖库源码
*   `include/`: 公共头文件
*   `assets/`: 资源文件 (字体、纹理、着色器等)

## 注意事项
