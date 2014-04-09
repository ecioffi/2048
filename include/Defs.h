#ifndef __DEFS_H_INCLUDED__
#define __DEFS_H_INCLUDED__

#include <stdint.h>

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;

inline constexpr U8 operator"" _U8 (U64 n) { return n; }
inline constexpr U16 operator"" _U16 (U64 n) { return n; }
inline constexpr U32 operator"" _U32 (U64 n) { return n; }
inline constexpr U64 operator"" _U64 (U64 n) { return n; }

inline constexpr S8 operator"" _S8 (U64 n) { return n; }
inline constexpr S16 operator"" _S16 (U64 n) { return n; }
inline constexpr S32 operator"" _S32 (U64 n) { return n; }
inline constexpr S64 operator"" _S64 (U64 n) { return n; }

#endif /* __DEFS_H_INCLUDED__ */
