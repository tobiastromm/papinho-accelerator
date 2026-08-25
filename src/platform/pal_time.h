#ifndef PAPACC_PAL_TIME_H
#define PAPACC_PAL_TIME_H

#include "papacc/types.h"

/*
 * Returns monotonic time in nanoseconds through out_nanoseconds.
 *
 * Nanoseconds are the API unit, not a promise of one-nanosecond clock
 * resolution. The value is suitable for durations and relative deadlines; it
 * is not civil time and has no calendar or timezone meaning.
 */
PAPACC_RESULT papacc_pal_monotonic_time_ns(PAPACC_U64 *out_nanoseconds);

#endif
