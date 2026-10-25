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
#endif
