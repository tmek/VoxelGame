// #include <iostream>
// #include <cstdlib>
// #include <cassert>
//
// #define DEBUG_BREAK() \
// do { \
// __debugbreak(); \
// } while (false)
//
// #define check(expr) \
// do { \
// if (!(expr)) { \
// std::cerr << "Check failed: " << #expr << "\n" \
// << "Function: " << __FUNCTION__ << "\n" \
// << "File: " << __FILE__ << "\n" \
// << "Line: " << __LINE__ << std::endl; \
// DEBUG_BREAK(); \
// std::abort(); \
// } \
// } while (false)