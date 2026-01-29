# StarMap

StarMap is a star map / astronomical visualization engine written in C/C++.

This project is a fork of [stellarium-web-engine](https://github.com/Stellarium/stellarium-web-engine). For runnable examples, see [StarMapExample](https://github.com/dwarflab/StarMapExample).

**Project migration notes:**
The original project was primarily designed for the Web (compiled with Emscripten). This repository contains deep native adaptations, including but not limited to:

1. **Removal of Web dependencies**: Stripped out Emscripten/browser-coupled interface code.
2. **Multi-platform support**: Refactored the build system (CMake) to enable native compilation and execution on major platforms such as **Windows (MinGW)**, **Android**, and **iOS**.
3. **Compatibility fixes**: Resolved compatibility issues across different compilers (GCC, Clang).

## Dependencies

### System / External Requirements
To build this project you will need the following external tools and libraries:

- **CMake**: version 3.8 or newer
- **C/C++ compiler**: supporting C99/C++11 (GCC, Clang, MinGW, etc.; MSVC is not supported)
- **ZLIB**: required (for compression/decompression)
- **OpenGL ES 2.0**: rendering backend

#### Windows (MinGW) specific requirements
When building on Windows with MinGW you must provide the following libraries manually:

- **Pthread**: threading support library
- **Regex**: regular expression library (e.g. `libtre`, `gnurx`)
- **EGL/GLESv2**: a Windows implementation of OpenGL ES 2.0 (commonly provided by ANGLE)

## Build Instructions

### Windows (MinGW)

On Windows there is no single package manager for these dependencies, so you typically need to specify dependency paths manually.

1. **Prepare the toolchain**: install MinGW-w64.
2. **Prepare dependencies**: build or obtain Zlib, Pthread, Regex, EGL/GLESv2 (usually ANGLE) libraries including headers and library files.
3. **Configure CMake**:

     Create a build directory (for example `build/`) and run CMake from that directory. You must set variables pointing to your dependency locations.

     **Example commands:**

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

     **CMake variables reference:**

     | Variable | Description |
     | :--- | :--- |
     | `ZLIB_ROOT` | Root directory of the Zlib library |
     | `REGEX_INCLUDE_DIRS` | Header directory for the regex library (regex.h) |
     | `REGEX_LIBRARYS` | Library file path for the regex library |
     | `EGL_LIBRARYS` | Library file path for GLESv2 (typically libGLESv2) |
     | `PTHREAD_INCLUDE_DIRS` | Header directory for Pthread |
     | `PTHREAD_LIBRARYS` | Library file path for Pthread |

### Android

For Android you should provide the NDK toolchain file.

`CMakeLists.txt` will detect the `ANDROID` platform and configure:
- Linking against the system `GLESv2` library provided by Android.
- Enabling Android-specific build optimizations (e.g. `-Os`, `-Wl,--gc-sections`).
- You must specify the target ABI yourself.

### iOS

Requires macOS and the Xcode toolchain.
CMake will attempt to find and link iOS frameworks such as `OpenGLES`, `CoreFoundation`, and `Security`.

**Framework configuration:**
To build and use as an iOS Framework you may need to set target properties in `CMakeLists.txt`. Adjust the following as appropriate for your project:

```cmake
set_target_properties(${PRJ_NAME} PROPERTIES
    FRAMEWORK TRUE
    FRAMEWORK_VERSION C
    MACOSX_FRAMEWORK_IDENTIFIER "com.your.bundle.identifier"
    MACOSX_FRAMEWORK_BUNDLE_VERSION "1.0.0"
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING "1.0.0"
    VERSION 1.0.0
    SOVERSION 1.0.0
    PUBLIC_HEADER "${PROJECT_HEADER}"
    XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
)
```

## Code Layout

- `src/`: core source code
    - `algos/`: astronomical algorithms
    - `modules/`: functional modules (atmosphere, comets, satellites, etc.)
    - `utils/`: utility functions
- `ext_src/`: third-party dependency sources
- `include/`: public headers
- `assets/`: resources (fonts, textures, shaders, etc.)

## Notes