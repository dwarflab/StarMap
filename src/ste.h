#if defined(__cplusplus)
    #if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__) 
        #ifdef STE_EXPORTS
            #define STE_API extern "C" __declspec(dllexport)
        #else
            #define STE_API extern "C" __declspec(dllimport)
        #endif
    #elif defined(__GNUC__) || defined(__clang__) 
        #ifdef STE_EXPORTS
            #define STE_API extern "C" __attribute__((visibility ("default")))
        #else
            #define STE_API extern "C"
        #endif
    #else
        #error "Unsupported compiler/platform"
    #endif
#else
    #define STE_API
#endif


STE_API int ste_initScene(int win_width, int win_height, const char* url, double dpi);

STE_API int ste_render(int win_width, int win_height, double dpi);

STE_API int ste_onZoom(double k, double x, double y);

STE_API int ste_onMouse(int id, int state, double x, double y, int buttons);

STE_API int ste_getMouduleProperty(const char* type, const char* pro_name, void* value);

STE_API int ste_setMouduleProperty(const char* type, const char* pro_name, ...);

STE_API int ste_setMoudulePropertyDouble(const char* type, const char* pro_name, double value);

STE_API int ste_setMoudulePropertyInt(const char* type, const char* pro_name, int value);

STE_API int ste_setMoudulePropertyBool(const char* type, const char* pro_name, bool value);

STE_API int ste_setMoudulePropertyString(const char* type, const char* pro_name, const char* value);

STE_API int ste_setMoudulePropertyVec2(const char* type, const char* pro_name, double values[2]);

STE_API int ste_setMoudulePropertyVec3(const char* type, const char* pro_name, double values[3]);

STE_API int ste_setMoudulePropertyVec4(const char* type, const char* pro_name, double values[4]);

STE_API int ste_setMoudulePropertyPtr(const char* type, const char* pro_name, void* value);