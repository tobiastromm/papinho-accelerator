#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "pal_time.h"

#define PAPACC_NANOSECONDS_PER_SECOND UINT64_C(1000000000)

/*
 * Computes floor(value * multiplier / divisor) without forming the possibly
 * overflowing product. The caller supplies value < divisor and a divisor no
 * greater than INT64_MAX, as guaranteed by validated QPC values.
 */
static PAPACC_U64 papacc_pal_mul_div_fraction(
    PAPACC_U64 value,
    PAPACC_U64 multiplier,
    PAPACC_U64 divisor)
{
    PAPACC_U64 quotient = 0;
    PAPACC_U64 remainder = 0;
    PAPACC_U64 bit = UINT64_C(1) << 63;

    while (bit != 0) {
        quotient *= 2;
        remainder *= 2;

        if (remainder >= divisor) {
            remainder -= divisor;
            ++quotient;
        }

        if ((multiplier & bit) != 0) {
            remainder += value;
            if (remainder >= divisor) {
                remainder -= divisor;
                ++quotient;
            }
        }

        bit >>= 1;
    }

    return quotient;
}

PAPACC_RESULT papacc_pal_monotonic_time_ns(PAPACC_U64 *out_nanoseconds)
{
    LARGE_INTEGER native_counter;
    LARGE_INTEGER native_frequency;
    PAPACC_U64 counter;
    PAPACC_U64 frequency;
    PAPACC_U64 whole_seconds;
    PAPACC_U64 remainder;
    PAPACC_U64 fractional_nanoseconds;

    if (out_nanoseconds == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }

    if (!QueryPerformanceFrequency(&native_frequency) ||
        native_frequency.QuadPart <= 0) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }

    if (!QueryPerformanceCounter(&native_counter) ||
        native_counter.QuadPart < 0) {
        return PAPACC_RESULT_INTERNAL_ERROR;
    }

    counter = (PAPACC_U64)native_counter.QuadPart;
    frequency = (PAPACC_U64)native_frequency.QuadPart;
    whole_seconds = counter / frequency;
    remainder = counter % frequency;

    if (whole_seconds > UINT64_MAX / PAPACC_NANOSECONDS_PER_SECOND) {
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    }

    fractional_nanoseconds = papacc_pal_mul_div_fraction(
        remainder,
        PAPACC_NANOSECONDS_PER_SECOND,
        frequency);

    *out_nanoseconds =
        whole_seconds * PAPACC_NANOSECONDS_PER_SECOND +
        fractional_nanoseconds;

    return PAPACC_RESULT_OK;
}
