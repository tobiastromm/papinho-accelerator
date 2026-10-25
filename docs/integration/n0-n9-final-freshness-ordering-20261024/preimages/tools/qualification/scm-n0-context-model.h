/* Bounded input to the EXISTING owner context. Not authority or a recorder. */
#ifndef PAPACC_N0_CONTEXT_MODEL_H
#define PAPACC_N0_CONTEXT_MODEL_H
#include <stdio.h>
#include <string.h>
typedef PS_BD_FILE_RECORD IG_N0_FILE_RECORD;
static int ig_n0_parse(const char *text,size_t bytes,IG_N0_FILE_RECORD *record)
{
 IG_N0_FILE_RECORD x;char canonical[512];int n;ZeroMemory(&x,sizeof(x));
 if(!text||!record||!bytes||bytes>=sizeof(canonical)||memchr(text,0,bytes)||text[bytes])return 0;
 if(sscanf_s(text,"PACC_N2_OPENED_FILE_V1\nvolume=%lu\nhigh=%lu\nlow=%lu\nsize=%lu\nlinks=%lu\nsha256=%64s\nsecurity_sha256=%64s\nwindows_evidence_sha256=%64s\n",
 &x.volume,&x.high,&x.low,&x.size,&x.links,x.hash,(unsigned)sizeof(x.hash),x.security,(unsigned)sizeof(x.security),x.evidence,(unsigned)sizeof(x.evidence))!=8||
 !x.volume||(!x.high&&!x.low)||!x.links||!x.size||x.size>16777216UL||!ps_bd_hex(x.hash)||!ps_bd_hex(x.security)||!ps_bd_hex(x.evidence))return 0;
 n=sprintf_s(canonical,sizeof(canonical),"PACC_N2_OPENED_FILE_V1\nvolume=%lu\nhigh=%lu\nlow=%lu\nsize=%lu\nlinks=%lu\nsha256=%s\nsecurity_sha256=%s\nwindows_evidence_sha256=%s\n",x.volume,x.high,x.low,x.size,x.links,x.hash,x.security,x.evidence);
 if(n!=(int)bytes||memcmp(text,canonical,bytes))return 0;*record=x;return 1;
}
static __inline int ig_n0_fresh(ULONGLONG now,ULONGLONG observed,ULONGLONG maximum)
{return now>=observed&&now-observed<=maximum;}
/* Receipt-pinned identities, not operational authority or live metrics. */
static __inline int ig_n0_input_identity(const BY_HANDLE_FILE_INFORMATION *id,unsigned which)
{
 if(!id||which>1U||id->dwVolumeSerialNumber!=417614100UL||id->nFileSizeHigh||id->nNumberOfLinks!=1U||
 (id->dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT)))return 0;
 return id->nFileIndexHigh==(which?196608UL:131072UL)&&id->nFileIndexLow==(which?2841429UL:2841430UL)&&id->nFileSizeLow==(which?5358UL:326UL);
}
static __inline int ig_n0_root_identity(const BY_HANDLE_FILE_INFORMATION *id,unsigned which)
{
 static const DWORD hi[]={327680UL,131072UL,786432UL,851968UL,720896UL};
 static const DWORD lo[]={5UL,227641UL,1087874UL,2672648UL,2672656UL};
 return id&&which<5U&&id->dwVolumeSerialNumber==417614100UL&&id->nFileIndexHigh==hi[which]&&id->nFileIndexLow==lo[which]&&
 (id->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)&&!(id->dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT);
}
/* This compilation may prepare the exact read-only path, never a cycle. */
static __inline int ig_n0_readonly_command(int argc,char **argv)
{return argc==8&&argv&&argv[1]&&!strcmp(argv[1],"--prepare-n0");}
static __inline int ig_n0_volume_profile(DWORD serial,DWORD flags,const WCHAR *filesystem)
{return serial==417614100UL&&(flags&FS_PERSISTENT_ACLS)&&filesystem&&!_wcsicmp(filesystem,L"NTFS");}
#endif
