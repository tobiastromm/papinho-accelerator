/* Reviewed existing inputs; enabled ONLY in the explicit N0 LAB qualifier profiles. */
#ifndef PAPACC_N0_CONTEXT_PINS_H
#define PAPACC_N0_CONTEXT_PINS_H
#if (defined(PAPACC_N0_READONLY_PINNED_LAB) || defined(PAPACC_N0_POSITIVE_PINNED_LAB)) && defined(PAPACC_N0_N9_NATIVE_OWNER) && defined(PAPACC_SCM_INSTRUMENTED_LAB)
#define IG_N0_DOSSIER_SHA "5a2b7203afdece0adca77be278d6b86c18cc2cf457cab87474ee7dce0311c294"
#define IG_N0_EVIDENCE_SHA "babab3ea393b605802933054ab011e6fcb66b21bffcc80923448715e911f27e0"
#else
#define IG_N0_DOSSIER_SHA ""
#define IG_N0_EVIDENCE_SHA ""
#endif
#endif
