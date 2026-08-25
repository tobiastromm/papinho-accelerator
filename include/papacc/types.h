#ifndef PAPACC_TYPES_H
#define PAPACC_TYPES_H

#include <stddef.h>
#include <stdint.h>

typedef uint8_t PAPACC_U8;
typedef uint16_t PAPACC_U16;
typedef uint32_t PAPACC_U32;
typedef uint64_t PAPACC_U64;

typedef int8_t PAPACC_I8;
typedef int16_t PAPACC_I16;
typedef int32_t PAPACC_I32;
typedef int64_t PAPACC_I64;

typedef size_t PAPACC_SIZE;

typedef enum PAPACC_BOOL {
    PAPACC_FALSE = 0,
    PAPACC_TRUE = 1
} PAPACC_BOOL;

typedef enum PAPACC_RESULT {
    PAPACC_RESULT_OK = 0,
    PAPACC_RESULT_INVALID_ARGUMENT = 1,
    PAPACC_RESULT_OUT_OF_MEMORY = 2,
    PAPACC_RESULT_NOT_SUPPORTED = 3,
    PAPACC_RESULT_INVALID_STATE = 4,
    PAPACC_RESULT_LIMIT_EXCEEDED = 5,
    PAPACC_RESULT_INTERNAL_ERROR = 6
} PAPACC_RESULT;

/*
 * PAPACC_SIZE represents an in-process memory size or element count. It must
 * never be serialized directly. Future wire formats must use explicitly
 * defined fixed-width fields and byte order.
 *
 * PAPACC_RESULT is a portable result category, not a native diagnostic code.
 * Future PAL and backend implementations may retain native diagnostics
 * separately, but Win32, POSIX, or other platform codes must not be exposed as
 * PAPACC_RESULT values.
 */

#define PAPACC_COMPILE_ASSERT(name, expression) \
    typedef char papacc_compile_assert_##name[(expression) ? 1 : -1]

PAPACC_COMPILE_ASSERT(u8_is_8_bits, sizeof(PAPACC_U8) == 1);
PAPACC_COMPILE_ASSERT(u16_is_16_bits, sizeof(PAPACC_U16) == 2);
PAPACC_COMPILE_ASSERT(u32_is_32_bits, sizeof(PAPACC_U32) == 4);
PAPACC_COMPILE_ASSERT(u64_is_64_bits, sizeof(PAPACC_U64) == 8);
PAPACC_COMPILE_ASSERT(i8_is_8_bits, sizeof(PAPACC_I8) == 1);
PAPACC_COMPILE_ASSERT(i16_is_16_bits, sizeof(PAPACC_I16) == 2);
PAPACC_COMPILE_ASSERT(i32_is_32_bits, sizeof(PAPACC_I32) == 4);
PAPACC_COMPILE_ASSERT(i64_is_64_bits, sizeof(PAPACC_I64) == 8);

#undef PAPACC_COMPILE_ASSERT

#endif
