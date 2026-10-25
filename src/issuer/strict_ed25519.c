/*
 * Copyright 2016-2024 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

/* Accelerator-owned downstream extraction, 2026-10-21.
 * Public DATA only: variable-time operations, never secret inputs.
 * Generic SUPERCOP ref10 20141124 arithmetic from OpenSSL 3.5.8.
 * Changes: isolated types/constants; C89 literals; public-data memcmp glue.
 * See extraction-manifest.json. No signature, keygen, RNG or authority API.
 */
#include <string.h>
#include "strict_ed25519.h"
#include "papacc/types.h"
typedef PAPACC_U8 uint8_t;
typedef PAPACC_U32 uint32_t;
typedef PAPACC_U64 uint64_t;
typedef PAPACC_I32 int32_t;
typedef PAPACC_I64 int64_t;
typedef int32_t fe[10];
#if defined(_MSC_VER) && _MSC_VER < 1600
#define PACC_I64(x) x##i64
#else
#define PACC_I64(x) x##LL
#endif
static const int64_t kBottom25Bits = PACC_I64(0x1ffffff);
static const int64_t kBottom26Bits = PACC_I64(0x3ffffff);
static const int64_t kTop39Bits = PACC_I64(0xfffffffffe000000);
static const int64_t kTop38Bits = PACC_I64(0xfffffffffc000000);
typedef struct {
    fe X;
    fe Y;
    fe Z;
} ge_p2;

typedef struct {
    fe X;
    fe Y;
    fe Z;
    fe T;
} ge_p3;

typedef struct {
    fe X;
    fe Y;
    fe Z;
    fe T;
} ge_p1p1;

typedef struct {
    fe yplusx;
    fe yminusx;
    fe xy2d;
} ge_precomp;

typedef struct {
    fe YplusX;
    fe YminusX;
    fe Z;
    fe T2d;
} ge_cached;

static uint64_t load_3(const uint8_t *in);
static uint64_t load_4(const uint8_t *in);
static void fe_frombytes(fe h, const uint8_t *s);
static void fe_tobytes(uint8_t *s, const fe h);
static void fe_copy(fe h, const fe f);
static void fe_0(fe h);
static void fe_1(fe h);
static void fe_add(fe h, const fe f, const fe g);
static void fe_sub(fe h, const fe f, const fe g);
static void fe_mul(fe h, const fe f, const fe g);
static void fe_sq(fe h, const fe f);
static void fe_invert(fe out, const fe z);
static void fe_neg(fe h, const fe f);
static int fe_isnonzero(const fe f);
static int fe_isnegative(const fe f);
static void fe_sq2(fe h, const fe f);
static void fe_pow22523(fe out, const fe z);
static void ge_tobytes(uint8_t *s, const ge_p2 *h);
static void ge_p3_tobytes(uint8_t *s, const ge_p3 *h);
static int ge_frombytes_vartime(ge_p3 *h, const uint8_t *s);
static void ge_p2_0(ge_p2 *h);
static void ge_p3_to_p2(ge_p2 *r, const ge_p3 *p);
static void ge_p3_to_cached(ge_cached *r, const ge_p3 *p);
static void ge_p1p1_to_p2(ge_p2 *r, const ge_p1p1 *p);
static void ge_p1p1_to_p3(ge_p3 *r, const ge_p1p1 *p);
static void ge_p2_dbl(ge_p1p1 *r, const ge_p2 *p);
static void ge_p3_dbl(ge_p1p1 *r, const ge_p3 *p);
static void ge_madd(ge_p1p1 *r, const ge_p3 *p, const ge_precomp *q);
static void ge_msub(ge_p1p1 *r, const ge_p3 *p, const ge_precomp *q);
static void ge_add(ge_p1p1 *r, const ge_p3 *p, const ge_cached *q);
static void ge_sub(ge_p1p1 *r, const ge_p3 *p, const ge_cached *q);
static void slide(signed char *r, const uint8_t *a);
static void ge_double_scalarmult_vartime(ge_p2 *r, const uint8_t *a,
    const ge_p3 *A, const uint8_t *b);
static uint64_t load_3(const uint8_t *in)
{
    uint64_t result;

    result = ((uint64_t)in[0]);
    result |= ((uint64_t)in[1]) << 8;
    result |= ((uint64_t)in[2]) << 16;
    return result;
}

static uint64_t load_4(const uint8_t *in)
{
    uint64_t result;

    result = ((uint64_t)in[0]);
    result |= ((uint64_t)in[1]) << 8;
    result |= ((uint64_t)in[2]) << 16;
    result |= ((uint64_t)in[3]) << 24;
    return result;
}

static void fe_frombytes(fe h, const uint8_t *s)
{
    /* Ignores top bit of h. */
    int64_t h0 = load_4(s);
    int64_t h1 = load_3(s + 4) << 6;
    int64_t h2 = load_3(s + 7) << 5;
    int64_t h3 = load_3(s + 10) << 3;
    int64_t h4 = load_3(s + 13) << 2;
    int64_t h5 = load_4(s + 16);
    int64_t h6 = load_3(s + 20) << 7;
    int64_t h7 = load_3(s + 23) << 5;
    int64_t h8 = load_3(s + 26) << 4;
    int64_t h9 = (load_3(s + 29) & 0x7fffff) << 2;
    int64_t carry0;
    int64_t carry1;
    int64_t carry2;
    int64_t carry3;
    int64_t carry4;
    int64_t carry5;
    int64_t carry6;
    int64_t carry7;
    int64_t carry8;
    int64_t carry9;

    carry9 = h9 + (1 << 24);
    h0 += (carry9 >> 25) * 19;
    h9 -= carry9 & kTop39Bits;
    carry1 = h1 + (1 << 24);
    h2 += carry1 >> 25;
    h1 -= carry1 & kTop39Bits;
    carry3 = h3 + (1 << 24);
    h4 += carry3 >> 25;
    h3 -= carry3 & kTop39Bits;
    carry5 = h5 + (1 << 24);
    h6 += carry5 >> 25;
    h5 -= carry5 & kTop39Bits;
    carry7 = h7 + (1 << 24);
    h8 += carry7 >> 25;
    h7 -= carry7 & kTop39Bits;

    carry0 = h0 + (1 << 25);
    h1 += carry0 >> 26;
    h0 -= carry0 & kTop38Bits;
    carry2 = h2 + (1 << 25);
    h3 += carry2 >> 26;
    h2 -= carry2 & kTop38Bits;
    carry4 = h4 + (1 << 25);
    h5 += carry4 >> 26;
    h4 -= carry4 & kTop38Bits;
    carry6 = h6 + (1 << 25);
    h7 += carry6 >> 26;
    h6 -= carry6 & kTop38Bits;
    carry8 = h8 + (1 << 25);
    h9 += carry8 >> 26;
    h8 -= carry8 & kTop38Bits;

    h[0] = (int32_t)h0;
    h[1] = (int32_t)h1;
    h[2] = (int32_t)h2;
    h[3] = (int32_t)h3;
    h[4] = (int32_t)h4;
    h[5] = (int32_t)h5;
    h[6] = (int32_t)h6;
    h[7] = (int32_t)h7;
    h[8] = (int32_t)h8;
    h[9] = (int32_t)h9;
}

static void fe_tobytes(uint8_t *s, const fe h)
{
    int32_t h0 = h[0];
    int32_t h1 = h[1];
    int32_t h2 = h[2];
    int32_t h3 = h[3];
    int32_t h4 = h[4];
    int32_t h5 = h[5];
    int32_t h6 = h[6];
    int32_t h7 = h[7];
    int32_t h8 = h[8];
    int32_t h9 = h[9];
    int32_t q;

    q = (19 * h9 + (((int32_t)1) << 24)) >> 25;
    q = (h0 + q) >> 26;
    q = (h1 + q) >> 25;
    q = (h2 + q) >> 26;
    q = (h3 + q) >> 25;
    q = (h4 + q) >> 26;
    q = (h5 + q) >> 25;
    q = (h6 + q) >> 26;
    q = (h7 + q) >> 25;
    q = (h8 + q) >> 26;
    q = (h9 + q) >> 25;

    /* Goal: Output h-(2^255-19)q, which is between 0 and 2^255-20. */
    h0 += 19 * q;
    /* Goal: Output h-2^255 q, which is between 0 and 2^255-20. */

    h1 += h0 >> 26;
    h0 &= kBottom26Bits;
    h2 += h1 >> 25;
    h1 &= kBottom25Bits;
    h3 += h2 >> 26;
    h2 &= kBottom26Bits;
    h4 += h3 >> 25;
    h3 &= kBottom25Bits;
    h5 += h4 >> 26;
    h4 &= kBottom26Bits;
    h6 += h5 >> 25;
    h5 &= kBottom25Bits;
    h7 += h6 >> 26;
    h6 &= kBottom26Bits;
    h8 += h7 >> 25;
    h7 &= kBottom25Bits;
    h9 += h8 >> 26;
    h8 &= kBottom26Bits;
    h9 &= kBottom25Bits;
    /* h10 = carry9 */

    /*
     * Goal: Output h0+...+2^255 h10-2^255 q, which is between 0 and 2^255-20.
     * Have h0+...+2^230 h9 between 0 and 2^255-1;
     * evidently 2^255 h10-2^255 q = 0.
     * Goal: Output h0+...+2^230 h9.
     */
    s[0] = (uint8_t)(h0 >> 0);
    s[1] = (uint8_t)(h0 >> 8);
    s[2] = (uint8_t)(h0 >> 16);
    s[3] = (uint8_t)((h0 >> 24) | ((uint32_t)(h1) << 2));
    s[4] = (uint8_t)(h1 >> 6);
    s[5] = (uint8_t)(h1 >> 14);
    s[6] = (uint8_t)((h1 >> 22) | ((uint32_t)(h2) << 3));
    s[7] = (uint8_t)(h2 >> 5);
    s[8] = (uint8_t)(h2 >> 13);
    s[9] = (uint8_t)((h2 >> 21) | ((uint32_t)(h3) << 5));
    s[10] = (uint8_t)(h3 >> 3);
    s[11] = (uint8_t)(h3 >> 11);
    s[12] = (uint8_t)((h3 >> 19) | ((uint32_t)(h4) << 6));
    s[13] = (uint8_t)(h4 >> 2);
    s[14] = (uint8_t)(h4 >> 10);
    s[15] = (uint8_t)(h4 >> 18);
    s[16] = (uint8_t)(h5 >> 0);
    s[17] = (uint8_t)(h5 >> 8);
    s[18] = (uint8_t)(h5 >> 16);
    s[19] = (uint8_t)((h5 >> 24) | ((uint32_t)(h6) << 1));
    s[20] = (uint8_t)(h6 >> 7);
    s[21] = (uint8_t)(h6 >> 15);
    s[22] = (uint8_t)((h6 >> 23) | ((uint32_t)(h7) << 3));
    s[23] = (uint8_t)(h7 >> 5);
    s[24] = (uint8_t)(h7 >> 13);
    s[25] = (uint8_t)((h7 >> 21) | ((uint32_t)(h8) << 4));
    s[26] = (uint8_t)(h8 >> 4);
    s[27] = (uint8_t)(h8 >> 12);
    s[28] = (uint8_t)((h8 >> 20) | ((uint32_t)(h9) << 6));
    s[29] = (uint8_t)(h9 >> 2);
    s[30] = (uint8_t)(h9 >> 10);
    s[31] = (uint8_t)(h9 >> 18);
}

static void fe_copy(fe h, const fe f)
{
    memmove(h, f, sizeof(int32_t) * 10);
}

static void fe_0(fe h)
{
    memset(h, 0, sizeof(int32_t) * 10);
}

static void fe_1(fe h)
{
    memset(h, 0, sizeof(int32_t) * 10);
    h[0] = 1;
}

static void fe_add(fe h, const fe f, const fe g)
{
    unsigned i;

    for (i = 0; i < 10; i++) {
        h[i] = f[i] + g[i];
    }
}

static void fe_sub(fe h, const fe f, const fe g)
{
    unsigned i;

    for (i = 0; i < 10; i++) {
        h[i] = f[i] - g[i];
    }
}

static void fe_mul(fe h, const fe f, const fe g)
{
    int32_t f0 = f[0];
    int32_t f1 = f[1];
    int32_t f2 = f[2];
    int32_t f3 = f[3];
    int32_t f4 = f[4];
    int32_t f5 = f[5];
    int32_t f6 = f[6];
    int32_t f7 = f[7];
    int32_t f8 = f[8];
    int32_t f9 = f[9];
    int32_t g0 = g[0];
    int32_t g1 = g[1];
    int32_t g2 = g[2];
    int32_t g3 = g[3];
    int32_t g4 = g[4];
    int32_t g5 = g[5];
    int32_t g6 = g[6];
    int32_t g7 = g[7];
    int32_t g8 = g[8];
    int32_t g9 = g[9];
    int32_t g1_19 = 19 * g1; /* 1.959375*2^29 */
    int32_t g2_19 = 19 * g2; /* 1.959375*2^30; still ok */
    int32_t g3_19 = 19 * g3;
    int32_t g4_19 = 19 * g4;
    int32_t g5_19 = 19 * g5;
    int32_t g6_19 = 19 * g6;
    int32_t g7_19 = 19 * g7;
    int32_t g8_19 = 19 * g8;
    int32_t g9_19 = 19 * g9;
    int32_t f1_2 = 2 * f1;
    int32_t f3_2 = 2 * f3;
    int32_t f5_2 = 2 * f5;
    int32_t f7_2 = 2 * f7;
    int32_t f9_2 = 2 * f9;
    int64_t f0g0 = f0 * (int64_t)g0;
    int64_t f0g1 = f0 * (int64_t)g1;
    int64_t f0g2 = f0 * (int64_t)g2;
    int64_t f0g3 = f0 * (int64_t)g3;
    int64_t f0g4 = f0 * (int64_t)g4;
    int64_t f0g5 = f0 * (int64_t)g5;
    int64_t f0g6 = f0 * (int64_t)g6;
    int64_t f0g7 = f0 * (int64_t)g7;
    int64_t f0g8 = f0 * (int64_t)g8;
    int64_t f0g9 = f0 * (int64_t)g9;
    int64_t f1g0 = f1 * (int64_t)g0;
    int64_t f1g1_2 = f1_2 * (int64_t)g1;
    int64_t f1g2 = f1 * (int64_t)g2;
    int64_t f1g3_2 = f1_2 * (int64_t)g3;
    int64_t f1g4 = f1 * (int64_t)g4;
    int64_t f1g5_2 = f1_2 * (int64_t)g5;
    int64_t f1g6 = f1 * (int64_t)g6;
    int64_t f1g7_2 = f1_2 * (int64_t)g7;
    int64_t f1g8 = f1 * (int64_t)g8;
    int64_t f1g9_38 = f1_2 * (int64_t)g9_19;
    int64_t f2g0 = f2 * (int64_t)g0;
    int64_t f2g1 = f2 * (int64_t)g1;
    int64_t f2g2 = f2 * (int64_t)g2;
    int64_t f2g3 = f2 * (int64_t)g3;
    int64_t f2g4 = f2 * (int64_t)g4;
    int64_t f2g5 = f2 * (int64_t)g5;
    int64_t f2g6 = f2 * (int64_t)g6;
    int64_t f2g7 = f2 * (int64_t)g7;
    int64_t f2g8_19 = f2 * (int64_t)g8_19;
    int64_t f2g9_19 = f2 * (int64_t)g9_19;
    int64_t f3g0 = f3 * (int64_t)g0;
    int64_t f3g1_2 = f3_2 * (int64_t)g1;
    int64_t f3g2 = f3 * (int64_t)g2;
    int64_t f3g3_2 = f3_2 * (int64_t)g3;
    int64_t f3g4 = f3 * (int64_t)g4;
    int64_t f3g5_2 = f3_2 * (int64_t)g5;
    int64_t f3g6 = f3 * (int64_t)g6;
    int64_t f3g7_38 = f3_2 * (int64_t)g7_19;
    int64_t f3g8_19 = f3 * (int64_t)g8_19;
    int64_t f3g9_38 = f3_2 * (int64_t)g9_19;
    int64_t f4g0 = f4 * (int64_t)g0;
    int64_t f4g1 = f4 * (int64_t)g1;
    int64_t f4g2 = f4 * (int64_t)g2;
    int64_t f4g3 = f4 * (int64_t)g3;
    int64_t f4g4 = f4 * (int64_t)g4;
    int64_t f4g5 = f4 * (int64_t)g5;
    int64_t f4g6_19 = f4 * (int64_t)g6_19;
    int64_t f4g7_19 = f4 * (int64_t)g7_19;
    int64_t f4g8_19 = f4 * (int64_t)g8_19;
    int64_t f4g9_19 = f4 * (int64_t)g9_19;
    int64_t f5g0 = f5 * (int64_t)g0;
    int64_t f5g1_2 = f5_2 * (int64_t)g1;
    int64_t f5g2 = f5 * (int64_t)g2;
    int64_t f5g3_2 = f5_2 * (int64_t)g3;
    int64_t f5g4 = f5 * (int64_t)g4;
    int64_t f5g5_38 = f5_2 * (int64_t)g5_19;
    int64_t f5g6_19 = f5 * (int64_t)g6_19;
    int64_t f5g7_38 = f5_2 * (int64_t)g7_19;
    int64_t f5g8_19 = f5 * (int64_t)g8_19;
    int64_t f5g9_38 = f5_2 * (int64_t)g9_19;
    int64_t f6g0 = f6 * (int64_t)g0;
    int64_t f6g1 = f6 * (int64_t)g1;
    int64_t f6g2 = f6 * (int64_t)g2;
    int64_t f6g3 = f6 * (int64_t)g3;
    int64_t f6g4_19 = f6 * (int64_t)g4_19;
    int64_t f6g5_19 = f6 * (int64_t)g5_19;
    int64_t f6g6_19 = f6 * (int64_t)g6_19;
    int64_t f6g7_19 = f6 * (int64_t)g7_19;
    int64_t f6g8_19 = f6 * (int64_t)g8_19;
    int64_t f6g9_19 = f6 * (int64_t)g9_19;
    int64_t f7g0 = f7 * (int64_t)g0;
    int64_t f7g1_2 = f7_2 * (int64_t)g1;
    int64_t f7g2 = f7 * (int64_t)g2;
    int64_t f7g3_38 = f7_2 * (int64_t)g3_19;
    int64_t f7g4_19 = f7 * (int64_t)g4_19;
    int64_t f7g5_38 = f7_2 * (int64_t)g5_19;
    int64_t f7g6_19 = f7 * (int64_t)g6_19;
    int64_t f7g7_38 = f7_2 * (int64_t)g7_19;
    int64_t f7g8_19 = f7 * (int64_t)g8_19;
    int64_t f7g9_38 = f7_2 * (int64_t)g9_19;
    int64_t f8g0 = f8 * (int64_t)g0;
    int64_t f8g1 = f8 * (int64_t)g1;
    int64_t f8g2_19 = f8 * (int64_t)g2_19;
    int64_t f8g3_19 = f8 * (int64_t)g3_19;
    int64_t f8g4_19 = f8 * (int64_t)g4_19;
    int64_t f8g5_19 = f8 * (int64_t)g5_19;
    int64_t f8g6_19 = f8 * (int64_t)g6_19;
    int64_t f8g7_19 = f8 * (int64_t)g7_19;
    int64_t f8g8_19 = f8 * (int64_t)g8_19;
    int64_t f8g9_19 = f8 * (int64_t)g9_19;
    int64_t f9g0 = f9 * (int64_t)g0;
    int64_t f9g1_38 = f9_2 * (int64_t)g1_19;
    int64_t f9g2_19 = f9 * (int64_t)g2_19;
    int64_t f9g3_38 = f9_2 * (int64_t)g3_19;
    int64_t f9g4_19 = f9 * (int64_t)g4_19;
    int64_t f9g5_38 = f9_2 * (int64_t)g5_19;
    int64_t f9g6_19 = f9 * (int64_t)g6_19;
    int64_t f9g7_38 = f9_2 * (int64_t)g7_19;
    int64_t f9g8_19 = f9 * (int64_t)g8_19;
    int64_t f9g9_38 = f9_2 * (int64_t)g9_19;
    int64_t h0 = f0g0 + f1g9_38 + f2g8_19 + f3g7_38 + f4g6_19 + f5g5_38 + f6g4_19 + f7g3_38 + f8g2_19 + f9g1_38;
    int64_t h1 = f0g1 + f1g0 + f2g9_19 + f3g8_19 + f4g7_19 + f5g6_19 + f6g5_19 + f7g4_19 + f8g3_19 + f9g2_19;
    int64_t h2 = f0g2 + f1g1_2 + f2g0 + f3g9_38 + f4g8_19 + f5g7_38 + f6g6_19 + f7g5_38 + f8g4_19 + f9g3_38;
    int64_t h3 = f0g3 + f1g2 + f2g1 + f3g0 + f4g9_19 + f5g8_19 + f6g7_19 + f7g6_19 + f8g5_19 + f9g4_19;
    int64_t h4 = f0g4 + f1g3_2 + f2g2 + f3g1_2 + f4g0 + f5g9_38 + f6g8_19 + f7g7_38 + f8g6_19 + f9g5_38;
    int64_t h5 = f0g5 + f1g4 + f2g3 + f3g2 + f4g1 + f5g0 + f6g9_19 + f7g8_19 + f8g7_19 + f9g6_19;
    int64_t h6 = f0g6 + f1g5_2 + f2g4 + f3g3_2 + f4g2 + f5g1_2 + f6g0 + f7g9_38 + f8g8_19 + f9g7_38;
    int64_t h7 = f0g7 + f1g6 + f2g5 + f3g4 + f4g3 + f5g2 + f6g1 + f7g0 + f8g9_19 + f9g8_19;
    int64_t h8 = f0g8 + f1g7_2 + f2g6 + f3g5_2 + f4g4 + f5g3_2 + f6g2 + f7g1_2 + f8g0 + f9g9_38;
    int64_t h9 = f0g9 + f1g8 + f2g7 + f3g6 + f4g5 + f5g4 + f6g3 + f7g2 + f8g1 + f9g0;
    int64_t carry0;
    int64_t carry1;
    int64_t carry2;
    int64_t carry3;
    int64_t carry4;
    int64_t carry5;
    int64_t carry6;
    int64_t carry7;
    int64_t carry8;
    int64_t carry9;

    /* |h0| <= (1.65*1.65*2^52*(1+19+19+19+19)+1.65*1.65*2^50*(38+38+38+38+38))
     *   i.e. |h0| <= 1.4*2^60; narrower ranges for h2, h4, h6, h8
     * |h1| <= (1.65*1.65*2^51*(1+1+19+19+19+19+19+19+19+19))
     *   i.e. |h1| <= 1.7*2^59; narrower ranges for h3, h5, h7, h9 */

    carry0 = h0 + (1 << 25);
    h1 += carry0 >> 26;
    h0 -= carry0 & kTop38Bits;
    carry4 = h4 + (1 << 25);
    h5 += carry4 >> 26;
    h4 -= carry4 & kTop38Bits;
    /* |h0| <= 2^25 */
    /* |h4| <= 2^25 */
    /* |h1| <= 1.71*2^59 */
    /* |h5| <= 1.71*2^59 */

    carry1 = h1 + (1 << 24);
    h2 += carry1 >> 25;
    h1 -= carry1 & kTop39Bits;
    carry5 = h5 + (1 << 24);
    h6 += carry5 >> 25;
    h5 -= carry5 & kTop39Bits;
    /* |h1| <= 2^24; from now on fits into int32 */
    /* |h5| <= 2^24; from now on fits into int32 */
    /* |h2| <= 1.41*2^60 */
    /* |h6| <= 1.41*2^60 */

    carry2 = h2 + (1 << 25);
    h3 += carry2 >> 26;
    h2 -= carry2 & kTop38Bits;
    carry6 = h6 + (1 << 25);
    h7 += carry6 >> 26;
    h6 -= carry6 & kTop38Bits;
    /* |h2| <= 2^25; from now on fits into int32 unchanged */
    /* |h6| <= 2^25; from now on fits into int32 unchanged */
    /* |h3| <= 1.71*2^59 */
    /* |h7| <= 1.71*2^59 */

    carry3 = h3 + (1 << 24);
    h4 += carry3 >> 25;
    h3 -= carry3 & kTop39Bits;
    carry7 = h7 + (1 << 24);
    h8 += carry7 >> 25;
    h7 -= carry7 & kTop39Bits;
    /* |h3| <= 2^24; from now on fits into int32 unchanged */
    /* |h7| <= 2^24; from now on fits into int32 unchanged */
    /* |h4| <= 1.72*2^34 */
    /* |h8| <= 1.41*2^60 */

    carry4 = h4 + (1 << 25);
    h5 += carry4 >> 26;
    h4 -= carry4 & kTop38Bits;
    carry8 = h8 + (1 << 25);
    h9 += carry8 >> 26;
    h8 -= carry8 & kTop38Bits;
    /* |h4| <= 2^25; from now on fits into int32 unchanged */
    /* |h8| <= 2^25; from now on fits into int32 unchanged */
    /* |h5| <= 1.01*2^24 */
    /* |h9| <= 1.71*2^59 */

    carry9 = h9 + (1 << 24);
    h0 += (carry9 >> 25) * 19;
    h9 -= carry9 & kTop39Bits;
    /* |h9| <= 2^24; from now on fits into int32 unchanged */
    /* |h0| <= 1.1*2^39 */

    carry0 = h0 + (1 << 25);
    h1 += carry0 >> 26;
    h0 -= carry0 & kTop38Bits;
    /* |h0| <= 2^25; from now on fits into int32 unchanged */
    /* |h1| <= 1.01*2^24 */

    h[0] = (int32_t)h0;
    h[1] = (int32_t)h1;
    h[2] = (int32_t)h2;
    h[3] = (int32_t)h3;
    h[4] = (int32_t)h4;
    h[5] = (int32_t)h5;
    h[6] = (int32_t)h6;
    h[7] = (int32_t)h7;
    h[8] = (int32_t)h8;
    h[9] = (int32_t)h9;
}

static void fe_sq(fe h, const fe f)
{
    int32_t f0 = f[0];
    int32_t f1 = f[1];
    int32_t f2 = f[2];
    int32_t f3 = f[3];
    int32_t f4 = f[4];
    int32_t f5 = f[5];
    int32_t f6 = f[6];
    int32_t f7 = f[7];
    int32_t f8 = f[8];
    int32_t f9 = f[9];
    int32_t f0_2 = 2 * f0;
    int32_t f1_2 = 2 * f1;
    int32_t f2_2 = 2 * f2;
    int32_t f3_2 = 2 * f3;
    int32_t f4_2 = 2 * f4;
    int32_t f5_2 = 2 * f5;
    int32_t f6_2 = 2 * f6;
    int32_t f7_2 = 2 * f7;
    int32_t f5_38 = 38 * f5; /* 1.959375*2^30 */
    int32_t f6_19 = 19 * f6; /* 1.959375*2^30 */
    int32_t f7_38 = 38 * f7; /* 1.959375*2^30 */
    int32_t f8_19 = 19 * f8; /* 1.959375*2^30 */
    int32_t f9_38 = 38 * f9; /* 1.959375*2^30 */
    int64_t f0f0 = f0 * (int64_t)f0;
    int64_t f0f1_2 = f0_2 * (int64_t)f1;
    int64_t f0f2_2 = f0_2 * (int64_t)f2;
    int64_t f0f3_2 = f0_2 * (int64_t)f3;
    int64_t f0f4_2 = f0_2 * (int64_t)f4;
    int64_t f0f5_2 = f0_2 * (int64_t)f5;
    int64_t f0f6_2 = f0_2 * (int64_t)f6;
    int64_t f0f7_2 = f0_2 * (int64_t)f7;
    int64_t f0f8_2 = f0_2 * (int64_t)f8;
    int64_t f0f9_2 = f0_2 * (int64_t)f9;
    int64_t f1f1_2 = f1_2 * (int64_t)f1;
    int64_t f1f2_2 = f1_2 * (int64_t)f2;
    int64_t f1f3_4 = f1_2 * (int64_t)f3_2;
    int64_t f1f4_2 = f1_2 * (int64_t)f4;
    int64_t f1f5_4 = f1_2 * (int64_t)f5_2;
    int64_t f1f6_2 = f1_2 * (int64_t)f6;
    int64_t f1f7_4 = f1_2 * (int64_t)f7_2;
    int64_t f1f8_2 = f1_2 * (int64_t)f8;
    int64_t f1f9_76 = f1_2 * (int64_t)f9_38;
    int64_t f2f2 = f2 * (int64_t)f2;
    int64_t f2f3_2 = f2_2 * (int64_t)f3;
    int64_t f2f4_2 = f2_2 * (int64_t)f4;
    int64_t f2f5_2 = f2_2 * (int64_t)f5;
    int64_t f2f6_2 = f2_2 * (int64_t)f6;
    int64_t f2f7_2 = f2_2 * (int64_t)f7;
    int64_t f2f8_38 = f2_2 * (int64_t)f8_19;
    int64_t f2f9_38 = f2 * (int64_t)f9_38;
    int64_t f3f3_2 = f3_2 * (int64_t)f3;
    int64_t f3f4_2 = f3_2 * (int64_t)f4;
    int64_t f3f5_4 = f3_2 * (int64_t)f5_2;
    int64_t f3f6_2 = f3_2 * (int64_t)f6;
    int64_t f3f7_76 = f3_2 * (int64_t)f7_38;
    int64_t f3f8_38 = f3_2 * (int64_t)f8_19;
    int64_t f3f9_76 = f3_2 * (int64_t)f9_38;
    int64_t f4f4 = f4 * (int64_t)f4;
    int64_t f4f5_2 = f4_2 * (int64_t)f5;
    int64_t f4f6_38 = f4_2 * (int64_t)f6_19;
    int64_t f4f7_38 = f4 * (int64_t)f7_38;
    int64_t f4f8_38 = f4_2 * (int64_t)f8_19;
    int64_t f4f9_38 = f4 * (int64_t)f9_38;
    int64_t f5f5_38 = f5 * (int64_t)f5_38;
    int64_t f5f6_38 = f5_2 * (int64_t)f6_19;
    int64_t f5f7_76 = f5_2 * (int64_t)f7_38;
    int64_t f5f8_38 = f5_2 * (int64_t)f8_19;
    int64_t f5f9_76 = f5_2 * (int64_t)f9_38;
    int64_t f6f6_19 = f6 * (int64_t)f6_19;
    int64_t f6f7_38 = f6 * (int64_t)f7_38;
    int64_t f6f8_38 = f6_2 * (int64_t)f8_19;
    int64_t f6f9_38 = f6 * (int64_t)f9_38;
    int64_t f7f7_38 = f7 * (int64_t)f7_38;
    int64_t f7f8_38 = f7_2 * (int64_t)f8_19;
    int64_t f7f9_76 = f7_2 * (int64_t)f9_38;
    int64_t f8f8_19 = f8 * (int64_t)f8_19;
    int64_t f8f9_38 = f8 * (int64_t)f9_38;
    int64_t f9f9_38 = f9 * (int64_t)f9_38;
    int64_t h0 = f0f0 + f1f9_76 + f2f8_38 + f3f7_76 + f4f6_38 + f5f5_38;
    int64_t h1 = f0f1_2 + f2f9_38 + f3f8_38 + f4f7_38 + f5f6_38;
    int64_t h2 = f0f2_2 + f1f1_2 + f3f9_76 + f4f8_38 + f5f7_76 + f6f6_19;
    int64_t h3 = f0f3_2 + f1f2_2 + f4f9_38 + f5f8_38 + f6f7_38;
    int64_t h4 = f0f4_2 + f1f3_4 + f2f2 + f5f9_76 + f6f8_38 + f7f7_38;
    int64_t h5 = f0f5_2 + f1f4_2 + f2f3_2 + f6f9_38 + f7f8_38;
    int64_t h6 = f0f6_2 + f1f5_4 + f2f4_2 + f3f3_2 + f7f9_76 + f8f8_19;
    int64_t h7 = f0f7_2 + f1f6_2 + f2f5_2 + f3f4_2 + f8f9_38;
    int64_t h8 = f0f8_2 + f1f7_4 + f2f6_2 + f3f5_4 + f4f4 + f9f9_38;
    int64_t h9 = f0f9_2 + f1f8_2 + f2f7_2 + f3f6_2 + f4f5_2;
    int64_t carry0;
    int64_t carry1;
    int64_t carry2;
    int64_t carry3;
    int64_t carry4;
    int64_t carry5;
    int64_t carry6;
    int64_t carry7;
    int64_t carry8;
    int64_t carry9;

    carry0 = h0 + (1 << 25);
    h1 += carry0 >> 26;
    h0 -= carry0 & kTop38Bits;
    carry4 = h4 + (1 << 25);
    h5 += carry4 >> 26;
    h4 -= carry4 & kTop38Bits;

    carry1 = h1 + (1 << 24);
    h2 += carry1 >> 25;
    h1 -= carry1 & kTop39Bits;
    carry5 = h5 + (1 << 24);
    h6 += carry5 >> 25;
    h5 -= carry5 & kTop39Bits;

    carry2 = h2 + (1 << 25);
    h3 += carry2 >> 26;
    h2 -= carry2 & kTop38Bits;
    carry6 = h6 + (1 << 25);
    h7 += carry6 >> 26;
    h6 -= carry6 & kTop38Bits;

    carry3 = h3 + (1 << 24);
    h4 += carry3 >> 25;
    h3 -= carry3 & kTop39Bits;
    carry7 = h7 + (1 << 24);
    h8 += carry7 >> 25;
    h7 -= carry7 & kTop39Bits;

    carry4 = h4 + (1 << 25);
    h5 += carry4 >> 26;
    h4 -= carry4 & kTop38Bits;
    carry8 = h8 + (1 << 25);
    h9 += carry8 >> 26;
    h8 -= carry8 & kTop38Bits;

    carry9 = h9 + (1 << 24);
    h0 += (carry9 >> 25) * 19;
    h9 -= carry9 & kTop39Bits;

    carry0 = h0 + (1 << 25);
    h1 += carry0 >> 26;
    h0 -= carry0 & kTop38Bits;

    h[0] = (int32_t)h0;
    h[1] = (int32_t)h1;
    h[2] = (int32_t)h2;
    h[3] = (int32_t)h3;
    h[4] = (int32_t)h4;
    h[5] = (int32_t)h5;
    h[6] = (int32_t)h6;
    h[7] = (int32_t)h7;
    h[8] = (int32_t)h8;
    h[9] = (int32_t)h9;
}

static void fe_invert(fe out, const fe z)
{
    fe t0;
    fe t1;
    fe t2;
    fe t3;
    int i;

    /*
     * Compute z ** -1 = z ** (2 ** 255 - 19 - 2) with the exponent as
     * 2 ** 255 - 21 = (2 ** 5) * (2 ** 250 - 1) + 11.
     */

    /* t0 = z ** 2 */
    fe_sq(t0, z);

    /* t1 = t0 ** (2 ** 2) = z ** 8 */
    fe_sq(t1, t0);
    fe_sq(t1, t1);

    /* t1 = z * t1 = z ** 9 */
    fe_mul(t1, z, t1);
    /* t0 = t0 * t1 = z ** 11 -- stash t0 away for the end. */
    fe_mul(t0, t0, t1);

    /* t2 = t0 ** 2 = z ** 22 */
    fe_sq(t2, t0);

    /* t1 = t1 * t2 = z ** (2 ** 5 - 1) */
    fe_mul(t1, t1, t2);

    /* t2 = t1 ** (2 ** 5) = z ** ((2 ** 5) * (2 ** 5 - 1)) */
    fe_sq(t2, t1);
    for (i = 1; i < 5; ++i) {
        fe_sq(t2, t2);
    }

    /* t1 = t1 * t2 = z ** ((2 ** 5 + 1) * (2 ** 5 - 1)) = z ** (2 ** 10 - 1) */
    fe_mul(t1, t2, t1);

    /* Continuing similarly... */

    /* t2 = z ** (2 ** 20 - 1) */
    fe_sq(t2, t1);
    for (i = 1; i < 10; ++i) {
        fe_sq(t2, t2);
    }
    fe_mul(t2, t2, t1);

    /* t2 = z ** (2 ** 40 - 1) */
    fe_sq(t3, t2);
    for (i = 1; i < 20; ++i) {
        fe_sq(t3, t3);
    }
    fe_mul(t2, t3, t2);

    /* t2 = z ** (2 ** 10) * (2 ** 40 - 1) */
    for (i = 0; i < 10; ++i) {
        fe_sq(t2, t2);
    }
    /* t1 = z ** (2 ** 50 - 1) */
    fe_mul(t1, t2, t1);

    /* t2 = z ** (2 ** 100 - 1) */
    fe_sq(t2, t1);
    for (i = 1; i < 50; ++i) {
        fe_sq(t2, t2);
    }
    fe_mul(t2, t2, t1);

    /* t2 = z ** (2 ** 200 - 1) */
    fe_sq(t3, t2);
    for (i = 1; i < 100; ++i) {
        fe_sq(t3, t3);
    }
    fe_mul(t2, t3, t2);

    /* t2 = z ** ((2 ** 50) * (2 ** 200 - 1) */
    fe_sq(t2, t2);
    for (i = 1; i < 50; ++i) {
        fe_sq(t2, t2);
    }

    /* t1 = z ** (2 ** 250 - 1) */
    fe_mul(t1, t2, t1);

    /* t1 = z ** ((2 ** 5) * (2 ** 250 - 1)) */
    fe_sq(t1, t1);
    for (i = 1; i < 5; ++i) {
        fe_sq(t1, t1);
    }

    /* Recall t0 = z ** 11; out = z ** (2 ** 255 - 21) */
    fe_mul(out, t1, t0);
}

static void fe_neg(fe h, const fe f)
{
    unsigned i;

    for (i = 0; i < 10; i++) {
        h[i] = -f[i];
    }
}

static int fe_isnonzero(const fe f)
{
    uint8_t s[32];
    static const uint8_t zero[32] = { 0 };

    fe_tobytes(s, f);

    return memcmp(s, zero, sizeof(zero)) != 0;
}

static int fe_isnegative(const fe f)
{
    uint8_t s[32];

    fe_tobytes(s, f);
    return s[0] & 1;
}

static void fe_sq2(fe h, const fe f)
{
    int32_t f0 = f[0];
    int32_t f1 = f[1];
    int32_t f2 = f[2];
    int32_t f3 = f[3];
    int32_t f4 = f[4];
    int32_t f5 = f[5];
    int32_t f6 = f[6];
    int32_t f7 = f[7];
    int32_t f8 = f[8];
    int32_t f9 = f[9];
    int32_t f0_2 = 2 * f0;
    int32_t f1_2 = 2 * f1;
    int32_t f2_2 = 2 * f2;
    int32_t f3_2 = 2 * f3;
    int32_t f4_2 = 2 * f4;
    int32_t f5_2 = 2 * f5;
    int32_t f6_2 = 2 * f6;
    int32_t f7_2 = 2 * f7;
    int32_t f5_38 = 38 * f5; /* 1.959375*2^30 */
    int32_t f6_19 = 19 * f6; /* 1.959375*2^30 */
    int32_t f7_38 = 38 * f7; /* 1.959375*2^30 */
    int32_t f8_19 = 19 * f8; /* 1.959375*2^30 */
    int32_t f9_38 = 38 * f9; /* 1.959375*2^30 */
    int64_t f0f0 = f0 * (int64_t)f0;
    int64_t f0f1_2 = f0_2 * (int64_t)f1;
    int64_t f0f2_2 = f0_2 * (int64_t)f2;
    int64_t f0f3_2 = f0_2 * (int64_t)f3;
    int64_t f0f4_2 = f0_2 * (int64_t)f4;
    int64_t f0f5_2 = f0_2 * (int64_t)f5;
    int64_t f0f6_2 = f0_2 * (int64_t)f6;
    int64_t f0f7_2 = f0_2 * (int64_t)f7;
    int64_t f0f8_2 = f0_2 * (int64_t)f8;
    int64_t f0f9_2 = f0_2 * (int64_t)f9;
    int64_t f1f1_2 = f1_2 * (int64_t)f1;
    int64_t f1f2_2 = f1_2 * (int64_t)f2;
    int64_t f1f3_4 = f1_2 * (int64_t)f3_2;
    int64_t f1f4_2 = f1_2 * (int64_t)f4;
    int64_t f1f5_4 = f1_2 * (int64_t)f5_2;
    int64_t f1f6_2 = f1_2 * (int64_t)f6;
    int64_t f1f7_4 = f1_2 * (int64_t)f7_2;
    int64_t f1f8_2 = f1_2 * (int64_t)f8;
    int64_t f1f9_76 = f1_2 * (int64_t)f9_38;
    int64_t f2f2 = f2 * (int64_t)f2;
    int64_t f2f3_2 = f2_2 * (int64_t)f3;
    int64_t f2f4_2 = f2_2 * (int64_t)f4;
    int64_t f2f5_2 = f2_2 * (int64_t)f5;
    int64_t f2f6_2 = f2_2 * (int64_t)f6;
    int64_t f2f7_2 = f2_2 * (int64_t)f7;
    int64_t f2f8_38 = f2_2 * (int64_t)f8_19;
    int64_t f2f9_38 = f2 * (int64_t)f9_38;
    int64_t f3f3_2 = f3_2 * (int64_t)f3;
    int64_t f3f4_2 = f3_2 * (int64_t)f4;
    int64_t f3f5_4 = f3_2 * (int64_t)f5_2;
    int64_t f3f6_2 = f3_2 * (int64_t)f6;
    int64_t f3f7_76 = f3_2 * (int64_t)f7_38;
    int64_t f3f8_38 = f3_2 * (int64_t)f8_19;
    int64_t f3f9_76 = f3_2 * (int64_t)f9_38;
    int64_t f4f4 = f4 * (int64_t)f4;
    int64_t f4f5_2 = f4_2 * (int64_t)f5;
    int64_t f4f6_38 = f4_2 * (int64_t)f6_19;
    int64_t f4f7_38 = f4 * (int64_t)f7_38;
    int64_t f4f8_38 = f4_2 * (int64_t)f8_19;
    int64_t f4f9_38 = f4 * (int64_t)f9_38;
    int64_t f5f5_38 = f5 * (int64_t)f5_38;
    int64_t f5f6_38 = f5_2 * (int64_t)f6_19;
    int64_t f5f7_76 = f5_2 * (int64_t)f7_38;
    int64_t f5f8_38 = f5_2 * (int64_t)f8_19;
    int64_t f5f9_76 = f5_2 * (int64_t)f9_38;
    int64_t f6f6_19 = f6 * (int64_t)f6_19;
    int64_t f6f7_38 = f6 * (int64_t)f7_38;
    int64_t f6f8_38 = f6_2 * (int64_t)f8_19;
    int64_t f6f9_38 = f6 * (int64_t)f9_38;
    int64_t f7f7_38 = f7 * (int64_t)f7_38;
    int64_t f7f8_38 = f7_2 * (int64_t)f8_19;
    int64_t f7f9_76 = f7_2 * (int64_t)f9_38;
    int64_t f8f8_19 = f8 * (int64_t)f8_19;
    int64_t f8f9_38 = f8 * (int64_t)f9_38;
    int64_t f9f9_38 = f9 * (int64_t)f9_38;
    int64_t h0 = f0f0 + f1f9_76 + f2f8_38 + f3f7_76 + f4f6_38 + f5f5_38;
    int64_t h1 = f0f1_2 + f2f9_38 + f3f8_38 + f4f7_38 + f5f6_38;
    int64_t h2 = f0f2_2 + f1f1_2 + f3f9_76 + f4f8_38 + f5f7_76 + f6f6_19;
    int64_t h3 = f0f3_2 + f1f2_2 + f4f9_38 + f5f8_38 + f6f7_38;
    int64_t h4 = f0f4_2 + f1f3_4 + f2f2 + f5f9_76 + f6f8_38 + f7f7_38;
    int64_t h5 = f0f5_2 + f1f4_2 + f2f3_2 + f6f9_38 + f7f8_38;
    int64_t h6 = f0f6_2 + f1f5_4 + f2f4_2 + f3f3_2 + f7f9_76 + f8f8_19;
    int64_t h7 = f0f7_2 + f1f6_2 + f2f5_2 + f3f4_2 + f8f9_38;
    int64_t h8 = f0f8_2 + f1f7_4 + f2f6_2 + f3f5_4 + f4f4 + f9f9_38;
    int64_t h9 = f0f9_2 + f1f8_2 + f2f7_2 + f3f6_2 + f4f5_2;
    int64_t carry0;
    int64_t carry1;
    int64_t carry2;
    int64_t carry3;
    int64_t carry4;
    int64_t carry5;
    int64_t carry6;
    int64_t carry7;
    int64_t carry8;
    int64_t carry9;

    h0 += h0;
    h1 += h1;
    h2 += h2;
    h3 += h3;
    h4 += h4;
    h5 += h5;
    h6 += h6;
    h7 += h7;
    h8 += h8;
    h9 += h9;

    carry0 = h0 + (1 << 25);
    h1 += carry0 >> 26;
    h0 -= carry0 & kTop38Bits;
    carry4 = h4 + (1 << 25);
    h5 += carry4 >> 26;
    h4 -= carry4 & kTop38Bits;

    carry1 = h1 + (1 << 24);
    h2 += carry1 >> 25;
    h1 -= carry1 & kTop39Bits;
    carry5 = h5 + (1 << 24);
    h6 += carry5 >> 25;
    h5 -= carry5 & kTop39Bits;

    carry2 = h2 + (1 << 25);
    h3 += carry2 >> 26;
    h2 -= carry2 & kTop38Bits;
    carry6 = h6 + (1 << 25);
    h7 += carry6 >> 26;
    h6 -= carry6 & kTop38Bits;

    carry3 = h3 + (1 << 24);
    h4 += carry3 >> 25;
    h3 -= carry3 & kTop39Bits;
    carry7 = h7 + (1 << 24);
    h8 += carry7 >> 25;
    h7 -= carry7 & kTop39Bits;

    carry4 = h4 + (1 << 25);
    h5 += carry4 >> 26;
    h4 -= carry4 & kTop38Bits;
    carry8 = h8 + (1 << 25);
    h9 += carry8 >> 26;
    h8 -= carry8 & kTop38Bits;

    carry9 = h9 + (1 << 24);
    h0 += (carry9 >> 25) * 19;
    h9 -= carry9 & kTop39Bits;

    carry0 = h0 + (1 << 25);
    h1 += carry0 >> 26;
    h0 -= carry0 & kTop38Bits;

    h[0] = (int32_t)h0;
    h[1] = (int32_t)h1;
    h[2] = (int32_t)h2;
    h[3] = (int32_t)h3;
    h[4] = (int32_t)h4;
    h[5] = (int32_t)h5;
    h[6] = (int32_t)h6;
    h[7] = (int32_t)h7;
    h[8] = (int32_t)h8;
    h[9] = (int32_t)h9;
}

static void fe_pow22523(fe out, const fe z)
{
    fe t0;
    fe t1;
    fe t2;
    int i;

    fe_sq(t0, z);
    fe_sq(t1, t0);
    for (i = 1; i < 2; ++i) {
        fe_sq(t1, t1);
    }
    fe_mul(t1, z, t1);
    fe_mul(t0, t0, t1);
    fe_sq(t0, t0);
    fe_mul(t0, t1, t0);
    fe_sq(t1, t0);
    for (i = 1; i < 5; ++i) {
        fe_sq(t1, t1);
    }
    fe_mul(t0, t1, t0);
    fe_sq(t1, t0);
    for (i = 1; i < 10; ++i) {
        fe_sq(t1, t1);
    }
    fe_mul(t1, t1, t0);
    fe_sq(t2, t1);
    for (i = 1; i < 20; ++i) {
        fe_sq(t2, t2);
    }
    fe_mul(t1, t2, t1);
    fe_sq(t1, t1);
    for (i = 1; i < 10; ++i) {
        fe_sq(t1, t1);
    }
    fe_mul(t0, t1, t0);
    fe_sq(t1, t0);
    for (i = 1; i < 50; ++i) {
        fe_sq(t1, t1);
    }
    fe_mul(t1, t1, t0);
    fe_sq(t2, t1);
    for (i = 1; i < 100; ++i) {
        fe_sq(t2, t2);
    }
    fe_mul(t1, t2, t1);
    fe_sq(t1, t1);
    for (i = 1; i < 50; ++i) {
        fe_sq(t1, t1);
    }
    fe_mul(t0, t1, t0);
    fe_sq(t0, t0);
    for (i = 1; i < 2; ++i) {
        fe_sq(t0, t0);
    }
    fe_mul(out, t0, z);
}

static void ge_tobytes(uint8_t *s, const ge_p2 *h)
{
    fe recip;
    fe x;
    fe y;

    fe_invert(recip, h->Z);
    fe_mul(x, h->X, recip);
    fe_mul(y, h->Y, recip);
    fe_tobytes(s, y);
    s[31] ^= fe_isnegative(x) << 7;
}

static void ge_p3_tobytes(uint8_t *s, const ge_p3 *h)
{
    fe recip;
    fe x;
    fe y;

    fe_invert(recip, h->Z);
    fe_mul(x, h->X, recip);
    fe_mul(y, h->Y, recip);
    fe_tobytes(s, y);
    s[31] ^= fe_isnegative(x) << 7;
}

static const fe d = {
    -10913610, 13857413, -15372611, 6949391, 114729,
    -8787816, -6275908, -3247719, -18696448, -12055116
};

static const fe sqrtm1 = {
    -32595792, -7943725, 9377950, 3500415, 12389472,
    -272473, -25146209, -2005654, 326686, 11406482
};

static int ge_frombytes_vartime(ge_p3 *h, const uint8_t *s)
{
    fe u;
    fe v;
    fe w;
    fe vxx;
    fe check;

    fe_frombytes(h->Y, s);
    fe_1(h->Z);
    fe_sq(u, h->Y);
    fe_mul(v, u, d);
    fe_sub(u, u, h->Z); /* u = y^2-1 */
    fe_add(v, v, h->Z); /* v = dy^2+1 */

    fe_mul(w, u, v); /* w = u*v */

    fe_pow22523(h->X, w); /* x = w^((q-5)/8) */
    fe_mul(h->X, h->X, u); /* x = u * w^((q-5)/8) */

    fe_sq(vxx, h->X);
    fe_mul(vxx, vxx, v);
    fe_sub(check, vxx, u); /* vx^2-u */
    if (fe_isnonzero(check)) {
        fe_add(check, vxx, u); /* vx^2+u */
        if (fe_isnonzero(check)) {
            return -1;
        }
        fe_mul(h->X, h->X, sqrtm1);
    }

    if (fe_isnegative(h->X) != (s[31] >> 7)) {
        fe_neg(h->X, h->X);
    }

    fe_mul(h->T, h->X, h->Y);
    return 0;
}

static void ge_p2_0(ge_p2 *h)
{
    fe_0(h->X);
    fe_1(h->Y);
    fe_1(h->Z);
}

static void ge_p3_to_p2(ge_p2 *r, const ge_p3 *p)
{
    fe_copy(r->X, p->X);
    fe_copy(r->Y, p->Y);
    fe_copy(r->Z, p->Z);
}

static const fe d2 = {
    -21827239, -5839606, -30745221, 13898782, 229458,
    15978800, -12551817, -6495438, 29715968, 9444199
};

static void ge_p3_to_cached(ge_cached *r, const ge_p3 *p)
{
    fe_add(r->YplusX, p->Y, p->X);
    fe_sub(r->YminusX, p->Y, p->X);
    fe_copy(r->Z, p->Z);
    fe_mul(r->T2d, p->T, d2);
}

static void ge_p1p1_to_p2(ge_p2 *r, const ge_p1p1 *p)
{
    fe_mul(r->X, p->X, p->T);
    fe_mul(r->Y, p->Y, p->Z);
    fe_mul(r->Z, p->Z, p->T);
}

static void ge_p1p1_to_p3(ge_p3 *r, const ge_p1p1 *p)
{
    fe_mul(r->X, p->X, p->T);
    fe_mul(r->Y, p->Y, p->Z);
    fe_mul(r->Z, p->Z, p->T);
    fe_mul(r->T, p->X, p->Y);
}

static void ge_p2_dbl(ge_p1p1 *r, const ge_p2 *p)
{
    fe t0;

    fe_sq(r->X, p->X);
    fe_sq(r->Z, p->Y);
    fe_sq2(r->T, p->Z);
    fe_add(r->Y, p->X, p->Y);
    fe_sq(t0, r->Y);
    fe_add(r->Y, r->Z, r->X);
    fe_sub(r->Z, r->Z, r->X);
    fe_sub(r->X, t0, r->Y);
    fe_sub(r->T, r->T, r->Z);
}

static void ge_p3_dbl(ge_p1p1 *r, const ge_p3 *p)
{
    ge_p2 q;
    ge_p3_to_p2(&q, p);
    ge_p2_dbl(r, &q);
}

static void ge_madd(ge_p1p1 *r, const ge_p3 *p, const ge_precomp *q)
{
    fe t0;

    fe_add(r->X, p->Y, p->X);
    fe_sub(r->Y, p->Y, p->X);
    fe_mul(r->Z, r->X, q->yplusx);
    fe_mul(r->Y, r->Y, q->yminusx);
    fe_mul(r->T, q->xy2d, p->T);
    fe_add(t0, p->Z, p->Z);
    fe_sub(r->X, r->Z, r->Y);
    fe_add(r->Y, r->Z, r->Y);
    fe_add(r->Z, t0, r->T);
    fe_sub(r->T, t0, r->T);
}

static void ge_msub(ge_p1p1 *r, const ge_p3 *p, const ge_precomp *q)
{
    fe t0;

    fe_add(r->X, p->Y, p->X);
    fe_sub(r->Y, p->Y, p->X);
    fe_mul(r->Z, r->X, q->yminusx);
    fe_mul(r->Y, r->Y, q->yplusx);
    fe_mul(r->T, q->xy2d, p->T);
    fe_add(t0, p->Z, p->Z);
    fe_sub(r->X, r->Z, r->Y);
    fe_add(r->Y, r->Z, r->Y);
    fe_sub(r->Z, t0, r->T);
    fe_add(r->T, t0, r->T);
}

static void ge_add(ge_p1p1 *r, const ge_p3 *p, const ge_cached *q)
{
    fe t0;

    fe_add(r->X, p->Y, p->X);
    fe_sub(r->Y, p->Y, p->X);
    fe_mul(r->Z, r->X, q->YplusX);
    fe_mul(r->Y, r->Y, q->YminusX);
    fe_mul(r->T, q->T2d, p->T);
    fe_mul(r->X, p->Z, q->Z);
    fe_add(t0, r->X, r->X);
    fe_sub(r->X, r->Z, r->Y);
    fe_add(r->Y, r->Z, r->Y);
    fe_add(r->Z, t0, r->T);
    fe_sub(r->T, t0, r->T);
}

static void ge_sub(ge_p1p1 *r, const ge_p3 *p, const ge_cached *q)
{
    fe t0;

    fe_add(r->X, p->Y, p->X);
    fe_sub(r->Y, p->Y, p->X);
    fe_mul(r->Z, r->X, q->YminusX);
    fe_mul(r->Y, r->Y, q->YplusX);
    fe_mul(r->T, q->T2d, p->T);
    fe_mul(r->X, p->Z, q->Z);
    fe_add(t0, r->X, r->X);
    fe_sub(r->X, r->Z, r->Y);
    fe_add(r->Y, r->Z, r->Y);
    fe_sub(r->Z, t0, r->T);
    fe_add(r->T, t0, r->T);
}

static void slide(signed char *r, const uint8_t *a)
{
    int i;
    int b;
    int k;

    for (i = 0; i < 256; ++i) {
        r[i] = (signed char)(1 & (a[i >> 3] >> (i & 7)));
    }

    for (i = 0; i < 256; ++i) {
        if (r[i]) {
            for (b = 1; b <= 6 && i + b < 256; ++b) {
                if (r[i + b]) {
                    if (r[i] + (r[i + b] << b) <= 15) {
                        r[i] = (signed char)(r[i] + (r[i + b] << b));
                        r[i + b] = 0;
                    } else if (r[i] - (r[i + b] << b) >= -15) {
                        r[i] = (signed char)(r[i] - (r[i + b] << b));
                        for (k = i + b; k < 256; ++k) {
                            if (!r[k]) {
                                r[k] = 1;
                                break;
                            }
                            r[k] = 0;
                        }
                    } else {
                        break;
                    }
                }
            }
        }
    }
}

static const ge_precomp Bi[8] = {
    {
        { 25967493, -14356035, 29566456, 3660896, -12694345, 4014787, 27544626,
            -11754271, -6079156, 2047605 },
        { -12545711, 934262, -2722910, 3049990, -727428, 9406986, 12720692,
            5043384, 19500929, -15469378 },
        { -8738181, 4489570, 9688441, -14785194, 10184609, -12363380, 29287919,
            11864899, -24514362, -4438546 },
    },
    {
        { 15636291, -9688557, 24204773, -7912398, 616977, -16685262, 27787600,
            -14772189, 28944400, -1550024 },
        { 16568933, 4717097, -11556148, -1102322, 15682896, -11807043, 16354577,
            -11775962, 7689662, 11199574 },
        { 30464156, -5976125, -11779434, -15670865, 23220365, 15915852, 7512774,
            10017326, -17749093, -9920357 },
    },
    {
        { 10861363, 11473154, 27284546, 1981175, -30064349, 12577861, 32867885,
            14515107, -15438304, 10819380 },
        { 4708026, 6336745, 20377586, 9066809, -11272109, 6594696, -25653668,
            12483688, -12668491, 5581306 },
        { 19563160, 16186464, -29386857, 4097519, 10237984, -4348115, 28542350,
            13850243, -23678021, -15815942 },
    },
    {
        { 5153746, 9909285, 1723747, -2777874, 30523605, 5516873, 19480852,
            5230134, -23952439, -15175766 },
        { -30269007, -3463509, 7665486, 10083793, 28475525, 1649722, 20654025,
            16520125, 30598449, 7715701 },
        { 28881845, 14381568, 9657904, 3680757, -20181635, 7843316, -31400660,
            1370708, 29794553, -1409300 },
    },
    {
        { -22518993, -6692182, 14201702, -8745502, -23510406, 8844726, 18474211,
            -1361450, -13062696, 13821877 },
        { -6455177, -7839871, 3374702, -4740862, -27098617, -10571707, 31655028,
            -7212327, 18853322, -14220951 },
        { 4566830, -12963868, -28974889, -12240689, -7602672, -2830569, -8514358,
            -10431137, 2207753, -3209784 },
    },
    {
        { -25154831, -4185821, 29681144, 7868801, -6854661, -9423865, -12437364,
            -663000, -31111463, -16132436 },
        { 25576264, -2703214, 7349804, -11814844, 16472782, 9300885, 3844789,
            15725684, 171356, 6466918 },
        { 23103977, 13316479, 9739013, -16149481, 817875, -15038942, 8965339,
            -14088058, -30714912, 16193877 },
    },
    {
        { -33521811, 3180713, -2394130, 14003687, -16903474, -16270840, 17238398,
            4729455, -18074513, 9256800 },
        { -25182317, -4174131, 32336398, 5036987, -21236817, 11360617, 22616405,
            9761698, -19827198, 630305 },
        { -13720693, 2639453, -24237460, -7406481, 9494427, -5774029, -6554551,
            -15960994, -2449256, -14291300 },
    },
    {
        { -3151181, -5046075, 9282714, 6866145, -31907062, -863023, -18940575,
            15033784, 25105118, -7894876 },
        { -24326370, 15950226, -31801215, -14592823, -11662737, -5090925,
            1573892, -2625887, 2198790, -15804619 },
        { -3099351, 10324967, -2241613, 7453183, -5446979, -2735503, -13812022,
            -16236442, -32461234, -12290683 },
    },
};

static void ge_double_scalarmult_vartime(ge_p2 *r, const uint8_t *a,
    const ge_p3 *A, const uint8_t *b)
{
    signed char aslide[256];
    signed char bslide[256];
    ge_cached Ai[8]; /* A,3A,5A,7A,9A,11A,13A,15A */
    ge_p1p1 t;
    ge_p3 u;
    ge_p3 A2;
    int i;

    slide(aslide, a);
    slide(bslide, b);

    ge_p3_to_cached(&Ai[0], A);
    ge_p3_dbl(&t, A);
    ge_p1p1_to_p3(&A2, &t);
    ge_add(&t, &A2, &Ai[0]);
    ge_p1p1_to_p3(&u, &t);
    ge_p3_to_cached(&Ai[1], &u);
    ge_add(&t, &A2, &Ai[1]);
    ge_p1p1_to_p3(&u, &t);
    ge_p3_to_cached(&Ai[2], &u);
    ge_add(&t, &A2, &Ai[2]);
    ge_p1p1_to_p3(&u, &t);
    ge_p3_to_cached(&Ai[3], &u);
    ge_add(&t, &A2, &Ai[3]);
    ge_p1p1_to_p3(&u, &t);
    ge_p3_to_cached(&Ai[4], &u);
    ge_add(&t, &A2, &Ai[4]);
    ge_p1p1_to_p3(&u, &t);
    ge_p3_to_cached(&Ai[5], &u);
    ge_add(&t, &A2, &Ai[5]);
    ge_p1p1_to_p3(&u, &t);
    ge_p3_to_cached(&Ai[6], &u);
    ge_add(&t, &A2, &Ai[6]);
    ge_p1p1_to_p3(&u, &t);
    ge_p3_to_cached(&Ai[7], &u);

    ge_p2_0(r);

    for (i = 255; i >= 0; --i) {
        if (aslide[i] || bslide[i]) {
            break;
        }
    }

    for (; i >= 0; --i) {
        ge_p2_dbl(&t, r);

        if (aslide[i] > 0) {
            ge_p1p1_to_p3(&u, &t);
            ge_add(&t, &u, &Ai[aslide[i] / 2]);
        } else if (aslide[i] < 0) {
            ge_p1p1_to_p3(&u, &t);
            ge_sub(&t, &u, &Ai[(-aslide[i]) / 2]);
        }

        if (bslide[i] > 0) {
            ge_p1p1_to_p3(&u, &t);
            ge_madd(&t, &u, &Bi[bslide[i] / 2]);
        } else if (bslide[i] < 0) {
            ge_p1p1_to_p3(&u, &t);
            ge_msub(&t, &u, &Bi[(-bslide[i]) / 2]);
        }

        ge_p1p1_to_p2(r, &t);
    }
}
/* Downstream public DATA check; no source/trust/status authority.
 * This entry reuses ONLY this translation unit's existing arithmetic.
 * Return: 1 valid, 0 invalid, -1 internal invariant failure.
 * -2 is reserved for an unavailable backend at the consumer boundary.
 */
int papacc_time_source_ed25519_check(const unsigned char *key, size_t bytes)
{
    static const unsigned char order[32] = {
        0xed,0xd3,0xf5,0x5c,0x1a,0x63,0x12,0x58,
        0xd6,0x9c,0xf7,0xa2,0xde,0xf9,0xde,0x14,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x10
    };
    static const unsigned char zero[32] = {0};
    static const unsigned char identity[32] = {1};
    ge_p3 point;
    ge_p2 multiple;
    unsigned char encoded[32];
    unsigned int i;

    if (key == NULL || bytes != 32)
        return 0;
    if (ge_frombytes_vartime(&point, key) != 0)
        return 0;
    ge_p3_tobytes(encoded, &point);
    if (memcmp(encoded, key, 32) != 0)
        return 0;
    if (memcmp(encoded, identity, 32) == 0)
        return 0;
    /* Integer L, NOT x25519_sc_reduce(L), NOT cofactor-cleared input. */
    ge_double_scalarmult_vartime(&multiple, order, &point, zero);
    /* Projective zero denominator is an internal invariant failure. */
    if (fe_isnonzero(multiple.Z) == 0)
        return -1;
    ge_tobytes(encoded, &multiple);
    for (i = 0; i < 32; ++i) {
        if (encoded[i] != identity[i])
            return 0;
    }
    return 1;
}
