cmake_minimum_required (VERSION 3.8)

if(ANDROID)
	message(STATUS "Target Platform: Android")

    set(BUILD_PLATFORM "Android")
elseif(IOS)
    message(STATUS "Target Platform: iOS")

    set(BUILD_PLATFORM "IOS")
elseif(WIN32 AND MINGW)
	message(STATUS "Target Platform: Windows Mingw")

	# 定义一个布尔选项，用于启用或禁用某个功能
	option(CMAKE_GNUtoMS "Enable GNU to MS conversion" OFF)

	# 定义一个路径变量，用于指定 Zlib 库的根目录
	set(ZLIB_ROOT "" CACHE PATH "Path to the root directory of Zlib library")

	# 定义一个路径变量，用于指定正则表达式库的头文件目录
	set(REGEX_INCLUDE_DIRS "" CACHE PATH "Paths to the include directories for regex libraries")

	# 定义一个字符串变量，用于指定多个正则表达式库文件的路径
	set(REGEX_LIBRARYS "" CACHE STRING "Paths to regex libraries, separated by semicolons")

	# 定义一个字符串变量，用于指定多个 EGL 库文件的路径
	set(EGL_LIBRARYS "" CACHE STRING "Paths to EGL libraries, separated by semicolons")

    # 定义一个路径变量，用于指定pthread的头文件目录
	set(PTHREAD_INCLUDE_DIRS "" CACHE STRING "Paths to the include directories for pthread libraries")

    # 定义一个路径变量，用于指定pthread的库文件路径
	set(PTHREAD_LIBRARYS "" CACHE STRING "Paths to pthread libraries, separated by semicolons")

	# 检查 ZLIB_ROOT 是否为空
    if(NOT ZLIB_ROOT)
        message(WARNING "ZLIB_ROOT is not set. Please provide the path to the root directory of Zlib library.")
    else()
        message(STATUS "ZLIB_ROOT is set to ${ZLIB_ROOT}")
    endif()

    # 检查 REGEX_INCLUDE_DIRS 是否为空
    if(NOT REGEX_INCLUDE_DIRS)
        message(WARNING "REGEX_INCLUDE_DIRS is not set. Please provide the paths to the include directories for regex libraries.")
    else()
        message(STATUS "REGEX_INCLUDE_DIRS is set to ${REGEX_INCLUDE_DIRS}")
    endif()

    # 检查 REGEX_LIBRARYS 是否为空
    if(NOT REGEX_LIBRARYS)
        message(WARNING "REGEX_LIBRARYS is not set. Please provide the paths to regex libraries.")
    else()
        # 使用分号作为分隔符拆分路径
        string(REPLACE ";" "\n" REGEX_LIBRARYS_LIST "${REGEX_LIBRARYS}")
        message(STATUS "Regex libraries:")
        foreach(lib ${REGEX_LIBRARYS_LIST})
            message(STATUS " - ${lib}")
        endforeach()
    endif()

    # 检查 EGL_LIBRARYS 是否为空
    if(NOT EGL_LIBRARYS)
        message(WARNING "EGL_LIBRARYS is not set. Please provide the paths to EGL libraries.")
    else()
        # 使用分号作为分隔符拆分路径
        string(REPLACE ";" "\n" EGL_LIBRARYS_LIST "${EGL_LIBRARYS}")
        message(STATUS "EGL libraries:")
        foreach(lib ${EGL_LIBRARYS_LIST})
            message(STATUS " - ${lib}")
        endforeach()
    endif()

     # 检查 REGEX_INCLUDE_DIRS 是否为空
    if(NOT PTHREAD_INCLUDE_DIRS)
        message(WARNING "PTHREAD_INCLUDE_DIRS is not set. Please provide the paths to the include directories for regex libraries.")
    else()
        message(STATUS "PTHREAD_INCLUDE_DIRS is set to ${PTHREAD_INCLUDE_DIRS}")
    endif()

    # 检查 REGEX_LIBRARYS 是否为空
    if(NOT PTHREAD_LIBRARYS)
        message(WARNING "PTHREAD_LIBRARYS is not set. Please provide the paths to regex libraries.")
    else()
        # 使用分号作为分隔符拆分路径
        string(REPLACE ";" "\n" PTHREAD_LIBRARYS_LIST "${PTHREAD_LIBRARYS}")
        message(STATUS "Pthread libraries:")
        foreach(lib ${PTHREAD_LIBRARYS_LIST})
            message(STATUS " - ${lib}")
        endforeach()
    endif()

    set(BUILD_PLATFORM "Windows")
else()
	message(FATAL_ERROR "Unknown target platform")
endif()

set(PRJ_NAME "StarMap")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -include config.h")

file(GLOB_RECURSE PROJECT_HEADER src/*.h)
file(GLOB_RECURSE PROJECT_C_SOURCE src/*.c)
file(GLOB_RECURSE PROJECT_CPP_SOURCE src/*.cpp)

file(GLOB_RECURSE EXT_PROJECT_HEADER ext_src/*.h)
file(GLOB_RECURSE EXT_PROJECT_C_SOURCE ext_src/*.c)
file(GLOB_RECURSE EXT_PROJECT_CPP_SOURCE ext_src/*.cpp)

add_definitions(-D_USE_MATH_DEFINES)
add_definitions(-DNO_LIBCURL)
add_definitions(-DGLES2)
add_definitions(-DSTE_EXPORTS)

# 将源代码添加到此项目的可执行文件。
add_library (${PRJ_NAME} SHARED
			${PROJECT_HEADER}
			${PROJECT_C_SOURCE}
			${PROJECT_CPP_SOURCE}
			${EXT_PROJECT_HEADER}
			${EXT_PROJECT_C_SOURCE}
			${EXT_PROJECT_CPP_SOURCE}
			)

include_directories("./include")

include_directories("./ext_src/erfa")
include_directories("./ext_src/inih")
include_directories("./ext_src/json")
include_directories("./ext_src/md4c")
include_directories("./ext_src/nanovg")
include_directories("./ext_src/stb")
include_directories("./ext_src/uthash")
include_directories("./ext_src/webp")

include_directories("./src")

find_package(ZLIB REQUIRED)
target_link_libraries(${PRJ_NAME} ZLIB::ZLIB)

if(BUILD_PLATFORM STREQUAL "Windows")
	target_link_libraries(${PRJ_NAME} ${EGL_LIBRARYS})

	include_directories(${PRJ_NAME} ${REGEX_INCLUDE_DIRS})
	target_link_libraries(${PRJ_NAME} ${REGEX_LIBRARYS})

    include_directories(${PRJ_NAME} ${PTHREAD_INCLUDE_DIRS})
	target_link_libraries(${PRJ_NAME} ${PTHREAD_LIBRARYS})
elseif(BUILD_PLATFORM STREQUAL "Android")
    find_library(GLES2-lib GLESv2)
	target_link_libraries(${PRJ_NAME} ${GLES2-lib})

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections -Os -fvisibility=hidden")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections,--icf=safe,-s")
elseif(BUILD_PLATFORM STREQUAL "IOS")
    find_library(lib-gles OpenGLES)
	find_library(lib-corefoundation CoreFoundation)
	find_library(lib-security Security)
	target_link_libraries(${PRJ_NAME} ${lib-gles})
	target_link_libraries(${PRJ_NAME} ${lib-corefoundation})
	target_link_libraries(${PRJ_NAME} ${lib-security})

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections -fvisibility=hidden -Os -mllvm -align-all-functions=3")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-dead_strip")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fblocks -fvisibility=hidden")
endif()