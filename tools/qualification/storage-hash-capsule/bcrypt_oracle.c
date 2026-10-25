/* Separate oracle executable. NEVER linked into capsule. */
#include <windows.h>
#include <bcrypt.h>
#include <stdio.h>
#include <string.h>
#include "vectors.h"
int main(void){unsigned i,j,n;unsigned long at;unsigned char block[1024],out[32];BCRYPT_ALG_HANDLE a=NULL;BCRYPT_HASH_HANDLE h=NULL;
 for(i=0;i<VECTOR_COUNT;i++){
 if(BCryptOpenAlgorithmProvider(&a,BCRYPT_SHA256_ALGORITHM,NULL,0)!=0)return 1;
 if(BCryptCreateHash(a,&h,NULL,0,NULL,0,0)!=0)return 2;
 for(at=0;at<vectors[i].length;at+=n){n=(unsigned)(vectors[i].length-at);if(n>1024)n=1024;
 for(j=0;j<n;j++)block[j]=(unsigned char)(vectors[i].prefix&&at+j<8?vectors[i].prefix[at+j]:vectors[i].kind==1?"abc"[(at+j)%3]:vectors[i].kind==2?'a':(unsigned char)((at+j)*17UL+3UL));
 if(BCryptHashData(h,block,n,0)!=0)return 3;}
 if(BCryptFinishHash(h,out,32,0)!=0)return 4;
 if(BCryptDestroyHash(h)!=0)return 5;h=NULL;if(BCryptCloseAlgorithmProvider(a,0)!=0)return 6;a=NULL;
 if(memcmp(out,vectors[i].expected,32))return 7;
 }printf("BCrypt separate oracle PASS %u public vectors\n",(unsigned)VECTOR_COUNT);return 0;}
