#include "scm-lab-attempt-binding.h"
/* Closed bootstrap/policy helpers, no I/O or authority. T10-T14 exact03 only. */
#ifndef PACC_PRIVATE_STDOUT_PROFILE_H
#define PACC_PRIVATE_STDOUT_PROFILE_H
#include <windows.h>
#include <string.h>
#define IG_PRIVATE_POSITIVE_APPROVAL_PREFIX "{\n  \"status\": \"EXACT_MANUAL_POSITIVE_OWNER_APPROVED\",\n  \"operation\": \"" PACC_LAB_OPERATION "\",\n  \"attempt\": \"" PACC_LAB_ATTEMPT "\",\n"
static __inline int ig_transport_decimal(const char *s,ULONGLONG limit,ULONGLONG *out){ULONGLONG n=0;unsigned d;if(!s||!*s||*s=='0')return 0;for(;*s;s++){if(*s<'0'||*s>'9')return 0;d=(unsigned)(*s-'0');if(n>(limit-d)/10)return 0;n=n*10+d;}*out=n;return n!=0;}
static __inline int ig_transport_hex(const char *s){unsigned i;if(!s||strlen(s)!=64)return 0;for(i=0;i<64;i++)if(!((s[i]>='0'&&s[i]<='9')||(s[i]>='a'&&s[i]<='f')))return 0;return 1;}
static __inline int ig_transport_bootstrap(int argc,char **argv)
{
#if defined(PAPACC_N0START03_PRIVATE_POSITIVE) || defined(PAPACC_N0START03_READONLY_PINS)
 unsigned i;ULONGLONG n;static const char prefix[]="\\\\.\\pipe\\papacc-n0-stdout-";size_t z=sizeof(prefix)-1;
 if(argc!=13||!argv)return 0;for(i=0;i<13;i++)if(!argv[i])return 0;
#ifdef PAPACC_N0START03_PRIVATE_POSITIVE
 if(strcmp(argv[1],"--cycle-n0"))return 0;
#elif defined(PAPACC_N0START03_READONLY_PINS)
 if(strcmp(argv[1],"--prepare-n0"))return 0;
#endif
 if(strcmp(argv[2],"" PACC_LAB_ATTEMPT "")||strcmp(argv[9],"--private-stdout")||!ig_transport_hex(argv[8])||strcmp(argv[3],argv[11])||strcmp(argv[4],argv[12])||!strcmp(argv[3],argv[6]))return 0;
 for(i=3;i<=7;i++)if(!ig_transport_decimal(argv[i],(i==4||i==7)?~(ULONGLONG)0:MAXDWORD,&n))return 0;
 if(!ig_transport_decimal(argv[11],MAXDWORD,&n)||!ig_transport_decimal(argv[12],~(ULONGLONG)0,&n))return 0;
 if(strlen(argv[10])!=z+32||strncmp(argv[10],prefix,z))return 0;
 for(i=(unsigned)z;i<z+32;i++)if(!((argv[10][i]>='0'&&argv[10][i]<='9')||(argv[10][i]>='a'&&argv[10][i]<='f')))return 0;
 return 1;
#else
 (void)argc;(void)argv;return 0;
#endif
}
static __inline int ig_transport_positive_record(const char *bytes,size_t size)
{return bytes&&size>=sizeof(IG_PRIVATE_POSITIVE_APPROVAL_PREFIX)-1&&!memcmp(bytes,IG_PRIVATE_POSITIVE_APPROVAL_PREFIX,sizeof(IG_PRIVATE_POSITIVE_APPROVAL_PREFIX)-1);}
#endif
