#ifndef __DEFS_H_INCLUDED__
#define __DEFS_H_INCLUDED__

#include <cstdint>

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;

#define U8_MIN UINT8_MIN;
#define U16_MIN UINT16_MIN;
#define U32_MIN UINT32_MIN;
#define U64_MIN UINT64_MIN;
#define U8_MAX UINT8_MAX;
#define U16_MAX UINT16_MAX;
#define U32_MAX UINT32_MAX;
#define U64_MAX UINT64_MAX;

#define S8_MIN INT8_MIN;
#define S16_MIN INT16_MIN;
#define S32_MIN INT32_MIN;
#define S64_MIN INT64_MIN;
#define S8_MAX INT8_MAX;
#define S16_MAX INT16_MAX;
#define S32_MAX INT32_MAX;
#define S64_MAX INT64_MAX;

inline constexpr U8 operator"" _U8 (unsigned long long int n) { return n; }
inline constexpr U16 operator"" _U16 (unsigned long long int n) { return n; }
inline constexpr U32 operator"" _U32 (unsigned long long int n) { return n; }
inline constexpr U64 operator"" _U64 (unsigned long long int n) { return n; }

inline constexpr S8 operator"" _S8 (unsigned long long int n) { return n; }
inline constexpr S16 operator"" _S16 (unsigned long long int n) { return n; }
inline constexpr S32 operator"" _S32 (unsigned long long int n) { return n; }
inline constexpr S64 operator"" _S64 (unsigned long long int n) { return n; }

#endif /* __DEFS_H_INCLUDED__ */
