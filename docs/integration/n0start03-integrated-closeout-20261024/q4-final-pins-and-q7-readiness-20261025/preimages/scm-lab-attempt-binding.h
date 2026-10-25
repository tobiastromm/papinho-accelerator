/* Private compile-time tuple; never a runtime identity selector or authority. */
#ifndef PAPACC_LAB_ATTEMPT_BINDING_H
#define PAPACC_LAB_ATTEMPT_BINDING_H
#ifdef PAPACC_Q4_TOKEN_DIAGNOSTIC
#define PACC_LAB_ATTEMPT "10584f02e11f92c92b43cddf0d9d316f"
#define PACC_LAB_ATTEMPT_W L"10584f02e11f92c92b43cddf0d9d316f"
#define PACC_LAB_OPERATION "N0TOKEN-20261025-01"
#define PACC_LAB_OPERATION_W L"N0TOKEN-20261025-01"
/* Protected inputs, exact N7, final binding and fresh budget remain absent. */
#define PACC_LAB_BINDING_READY 0
#else
#define PACC_LAB_ATTEMPT "0b1de869e11493b7cb08fb329578f6ce"
#define PACC_LAB_ATTEMPT_W L"0b1de869e11493b7cb08fb329578f6ce"
#define PACC_LAB_OPERATION "N0START-20261024-03"
#define PACC_LAB_OPERATION_W L"N0START-20261024-03"
#define PACC_LAB_BINDING_READY 1
#endif
#endif
