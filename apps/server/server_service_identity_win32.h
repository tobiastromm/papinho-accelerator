#ifndef PAPACC_SERVICE_IDENTITY_WIN32_H
#define PAPACC_SERVICE_IDENTITY_WIN32_H
/* Compile-time qualification profile only; product defaults remain fixed. */
#ifdef PAPACC_SCM_REGISTRATION_LAB
#define PAPACC_SCM_NAME_LITERAL L"PapinhoAcceleratorLab20261023"
#define PAPACC_SCM_ACCOUNT_LITERAL L"NT SERVICE\\PapinhoAcceleratorLab20261023"
#define PAPACC_SCM_START_MODE SERVICE_DISABLED
#else
#define PAPACC_SCM_NAME_LITERAL L"PapinhoAccelerator"
#define PAPACC_SCM_ACCOUNT_LITERAL L"NT SERVICE\\PapinhoAccelerator"
#define PAPACC_SCM_START_MODE SERVICE_DEMAND_START
#endif
#endif
