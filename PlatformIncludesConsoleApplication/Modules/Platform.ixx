module;  // Start the global module fragment

// Include standard headers or define necessary macros globally if required
#include <type_traits>
#include <bit>
#include <cassert>
#include <span>
#include <tuple>
#include <iostream>

export module Platform;

// Platform Detection and Definitions
export namespace PlatformNS
{

    export enum class PlatformType : unsigned char
    {
        Windows,
        MacOS,
        Linux,
        Undefined
    };

    // Set the CurrentPlatform based on the detected platform
#if defined(_WIN32)
    inline constexpr PlatformType CurrentPlatform = PlatformType::Windows;
#elif defined(__APPLE__)
    inline constexpr PlatformType CurrentPlatform = PlatformType::MacOS;
#elif defined(__linux__)
    inline constexpr PlatformType CurrentPlatform = PlatformType::Linux;
#else
    inline constexpr PlatformType CurrentPlatform = PlatformType::Undefined;
    static_assert(false, "Unsupported platform");
#endif

    // CPU Architecture Detection
    export enum class CPUArchitecture
    {
        x86,
        x64,
        ARM32,
        ARM64,
        Undefined
    };

#if defined(__x86_64__) || defined(_M_X64)
    inline constexpr CPUArchitecture CurrentArchitecture = CPUArchitecture::x64;
#elif defined(__i386) || defined(_M_IX86)
    inline constexpr CPUArchitecture CurrentArchitecture = CPUArchitecture::x86;
#elif defined(__aarch64__) || defined(_M_ARM64)
    inline constexpr CPUArchitecture CurrentArchitecture = CPUArchitecture::ARM64;
#elif defined(__arm__) || defined(_M_ARM)
    inline constexpr CPUArchitecture CurrentArchitecture = CPUArchitecture::ARM32;
#else
    inline constexpr CPUArchitecture CurrentArchitecture = CPUArchitecture::Undefined;
#endif

    // Compiler Detection
    export enum class CompilerType
    {
        MSVC,
        Clang,
        GCC,
        Undefined
    };

#if defined(_MSC_VER)
    inline constexpr CompilerType CurrentCompiler = CompilerType::MSVC;
#elif defined(__clang__)
    inline constexpr CompilerType CurrentCompiler = CompilerType::Clang;
#elif defined(__GNUC__)
    inline constexpr CompilerType CurrentCompiler = CompilerType::GCC;
#else
    inline constexpr CompilerType CurrentCompiler = CompilerType::Undefined;
#endif

    // Static Analysis and Sanitizers
    export consteval bool IsUsingAddressSanitizer()
    {
#if defined(__has_feature)
        return __has_feature(address_sanitizer);
#else
        return false;
#endif
    }

    // Platform-Specific Data Types
    using uint8 = std::uint8_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;
    using uint64 = std::uint64_t;

    using int8 = std::int8_t;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using int64 = std::int64_t;

    // Character Encoding
    using ANSICHAR = char;
    using WIDECHAR = wchar_t;
    using UTF8CHAR = char8_t;
    using UTF16CHAR = char16_t;
    using UTF32CHAR = char32_t;

    // Vector Intrinsics
    export consteval bool SupportsSSE4_1()
    {
        return CurrentArchitecture == CPUArchitecture::x64;
    }

    // Memory Alignment and Cache Control
    template <std::size_t Alignment>
    struct alignas(Alignment) AlignedMemory
    {
        uint8 data[Alignment];
    };

    inline constexpr std::size_t CacheLineSize = 64;

    // Function Modifiers
#define FORCEINLINE [[gnu::always_inline]] inline
#define NOINLINE [[gnu::noinline]]

    // Branch Prediction
#define LIKELY(x)   [[likely]] if(x)
#define UNLIKELY(x) [[unlikely]] if(x)

    // Static Assertions
#define STATIC_ASSERT(expr, msg) static_assert(expr, msg)

    // Compile-Time Warnings and Errors
#define COMPILE_WARNING(msg) _Pragma(GCC warning msg)
#define COMPILE_ERROR(msg) _Pragma(GCC error msg)

    // POD Type Conversion
    template <typename T>
    struct PODWrapper
    {
        T value;
        static_assert(std::is_standard_layout_v<T>, "Type must be POD");
    };

    // Platform-Specific Limits
    inline constexpr std::size_t MaxFilePathLength = 260;

    // Type Validation
    namespace TypeValidation
    {
        template <typename T>
        constexpr bool ValidateSizeAndAlignment()
        {
            return (sizeof(T) % alignof(T)) == 0;
        }

        STATIC_ASSERT(ValidateSizeAndAlignment<int32>(), "int32 type has incorrect alignment or size");
        STATIC_ASSERT(ValidateSizeAndAlignment<uint64>(), "uint64 type has incorrect alignment or size");
    }

    // Example usage within the module
    export void PrintPlatformInfo()
    {
        std::cout << "Platform: ";
        switch (CurrentPlatform)
        {
        case PlatformType::Windows: std::cout << "Windows";
            break;
        case PlatformType::MacOS: std::cout << "MacOS";
            break;
        case PlatformType::Linux: std::cout << "Linux";
            break;
        default: std::cout << "Undefined";
            break;
        }
        std::cout << "\nArchitecture: ";
        switch (CurrentArchitecture)
        {
        case CPUArchitecture::x64: std::cout << "x64";
            break;
        case CPUArchitecture::x86: std::cout << "x86";
            break;
        case CPUArchitecture::ARM64: std::cout << "ARM64";
            break;
        case CPUArchitecture::ARM32: std::cout << "ARM32";
            break;
        default: std::cout << "Undefined";
            break;
        }
        std::cout << "\nCompiler: ";
        switch (CurrentCompiler)
        {
        case CompilerType::MSVC: std::cout << "MSVC";
            break;
        case CompilerType::Clang: std::cout << "Clang";
            break;
        case CompilerType::GCC: std::cout << "GCC";
            break;
        default: std::cout << "Undefined";
            break;
        }
        std::cout << "\n";
    }
}

#define FORCEINLINE [[gnu::always_inline]] inline

// Platform-specific imports should be placed at the global scope and conditioned based on preprocessor checks.
#if defined(_WIN32)
export import :Windows;  // Import the Windows-specific module if on Windows
#elif defined(__APPLE__)
import :MacOS;  // Import the MacOS-specific module if on MacOS
#elif defined(__linux__)
import :Linux;  // Import the Linux-specific module if on Linux
#endif
