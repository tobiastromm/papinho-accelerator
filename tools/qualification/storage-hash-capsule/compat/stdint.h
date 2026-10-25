#ifndef PACC_LAB_VC6_STDINT_H
#define PACC_LAB_VC6_STDINT_H
#if !defined(_MSC_VER) || _MSC_VER > 1200
#error VC6_only_type_adapter
#endif
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed long int32_t;
typedef unsigned long uint32_t;
typedef signed __int64 int64_t;
typedef unsigned __int64 uint64_t;
typedef signed int intptr_t;
typedef unsigned int uintptr_t;
#endif
