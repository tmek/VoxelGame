// copyright

#pragma once

#define UBT_COMPILED_PLATFORM Windows

#include "Generated/CoreExports.h"
#include "Misc/EnumClassFlags.h"


#define PLATFORM_WINDOWS 1
#define PLATFORM_DESKTOP 1
#define PLATFORM_64BITS 1

#define WITH_ENGINE 1

#define UE_DEPRECATED(version, message)

#define LAUNCH_API

#define IMPLEMENT_MODULE(ImplClass, ModuleImpl)
#define ACCUM_LOADTIME(StatId, Time)            // ACCUM_LOADTIME(TEXT("EngineInitialization"), EngineInitializationTime);

#define TRACE_BOOKMARK(Format, ...) \
    TE_LOG(LogTemp, Log, TEXT("TRACE_BOOKMARK(") Format TEXT(")"), ##__VA_ARGS__)

#define PRAGMA_PUSH_PLATFORM_DEFAULT_PACKING
#define PRAGMA_POP_PLATFORM_DEFAULT_PACKING

#define THIRD_PARTY_INCLUDES_START
#define THIRD_PARTY_INCLUDES_END

#define IMPLEMENT_FOREIGN_ENGINE_DIR()
//#define IMPLEMENT_FOREIGN_ENGINE_DIR(X)

#define LOCTEXT(InKey, InTextLiteral)

class FMalloc;

#ifndef TRUE 
    #define TRUE true
#endif

#ifndef FALSE
    #define FALSE false
#endif



#define FLazyName FString

#define TSAN_ATOMIC(type) std::atomic<bool>         //TAtomic<##type>)

 struct ELogTimes
 {
     enum Type
     {
         None = 0,
         UTC = 1,
         Local = 2,
     };
 };

// todo: these globals are super hacky but temporary while refactoring
CORE_API extern bool GEnableDebugTint;
CORE_API extern bool GEnableVSync;
CORE_API extern bool GFreezeFrustumCulling;
CORE_API extern int GTotalDrawCalls;
CORE_API extern bool GUseDepthPass;


// #define PLATFORM_CAN_SUPPORT_EDITORONLY_DATA 1
// #define PLATFORM_LITTLE_ENDIAN 1
// #define PLATFORM_SUPPORTS_UNALIGNED_LOADS 1
// #define PLATFORM_SUPPORTS_PRAGMA_PACK 1
// #define PLATFORM_CPU_ARM_FAMILY 1
// #define PLATFORM_ENABLE_VECTORINTRINSICS_NEON 1
// #define PLATFORM_ENABLE_VECTORINTRINSICS 1
// #define PLATFORM_COMPILER_COMMON_LANGUAGE_RUNTIME_COMPILATION 1
// #define PLATFORM_USE_LS_SPEC_FOR_WIDECHAR 1
// #define PLATFORM_USE_SYSTEM_VSWPRINTF 1
// #define PLATFORM_ALWAYS_HAS_FMA3 1
// #define PLATFORM_USE_SYSTEM_VSWPRINTF 1
// #define PLATFORM_USE_SYSTEM_VSNWPRINTF 1
// #define PLATFORM_MAX_FILEPATH_LENGTH 260
// #define PLATFORM_MAX_FILEPATH_LENGTH_DEPRECATED 260
// #define PLATFORM_SUPPORTS_JEMALLOC 1
// #define PLATFORM_HAS_BSD_TIME 1
