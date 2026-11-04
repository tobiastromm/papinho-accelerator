/* Synthetic full v3 administrative context, REAL same-worker native TxF CAS.
 * No root CURRENT/authority, no keys/certificates generated or signature. */
#define _WIN32_WINNT 0x0600
#include "config_root8_v5_test.c"
#include <stdlib.h>
extern PAPACC_RESULT papacc_v5_worker_fixture_start(PAPACC_CONFIGURATION_COMMIT *,HANDLE);
extern PAPACC_RESULT papacc_v5_worker_fixture_poll(PAPACC_CONFIGURATION_COMMIT *,PAPACC_ISSUER_CAS_RESULT *);
static PAPACC_U8 observed_v5[PC_ROOT8_V5_MAX];
int main(int argc,char **argv)
{
 PAPACC_CONFIG_PENDING *p=&owner.pending_commit;HANDLE file,identity;DWORD count,start;
 PAPACC_ISSUER_CAS_RESULT outcome=PAPACC_COMMIT_IDLE;PAPACC_SIZE bn;const PAPACC_U8 *old_image;unsigned q,base_version;int stop,unknown,conflict,identity_conflict;char identity_path[MAX_PATH];
 CHECK(argc==4);base_version=(unsigned)atoi(argv[3]);CHECK(base_version==4U||base_version==5U);stop=!strcmp(argv[1],"stop");unknown=!strcmp(argv[1],"unknown");identity_conflict=!strcmp(argv[1],"identity");conflict=identity_conflict||!strcmp(argv[1],"conflict");
 CHECK(papacc_configuration_commit_init03(&owner,argv[2])==0);
 CHECK(initial5(4,4,4,3));
 if(base_version==4U){bn=z4;old_image=base4;}
 else{CHECK(cp5());bn=z5;memcpy(saved5,e5,bn);old_image=saved5;CHECK(next5(bn,2));}
 CHECK(pc_root8_v5_base_link(e5,z5,old_image,bn,hash,&owner,component5,NULL)==0);
 CHECK(sizeof(p->next)==PAPACC_CONFIG_TRANSACTION_BYTES&&sizeof(owner.committed)==PAPACC_CONFIG_TRANSACTION_BYTES);
 memcpy(owner.committed,old_image,bn);owner.committed_bytes=(DWORD)bn;
 memcpy(p->previous,old_image,bn);p->previous_bytes=(DWORD)bn;memcpy(p->next,e5,z5);p->next_bytes=(DWORD)z5;
 file=CreateFileA(argv[2],GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);CHECK(file!=INVALID_HANDLE_VALUE);
 CHECK(WriteFile(file,old_image,(DWORD)bn,&count,NULL)&&count==bn&&FlushFileBuffers(file));CHECK(CloseHandle(file));
 identity=CreateFileA(argv[2],0,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);CHECK(identity!=INVALID_HANDLE_VALUE);
 q=r32(p->previous+56);p->previous[q+64]^=1;CHECK(papacc_v5_worker_fixture_start(&owner,identity)!=0&&!p->thread);p->previous[q+64]^=1;
 p->next[112]^=1;CHECK(papacc_v5_worker_fixture_start(&owner,identity)!=0&&!p->thread);p->next[112]^=1;
 p->next[z5-1]^=1;CHECK(papacc_v5_worker_fixture_start(&owner,identity)!=0&&!p->thread);p->next[z5-1]^=1;
 /* Resealed outer digest cannot authorize unknown versions or lost history. */
 p->next[8]=6U;CHECK(hash(&owner,p->next,z5-32U,p->next+z5-32U));CHECK(papacc_v5_worker_fixture_start(&owner,identity)!=0&&!p->thread);p->next[8]=5U;
 CHECK(hash(&owner,p->next,z5-32U,p->next+z5-32U));
 q=r32(p->next+48);p->next[q+8192U+112U]^=1;CHECK(hash(&owner,p->next,z5-32U,p->next+z5-32U));CHECK(papacc_v5_worker_fixture_start(&owner,identity)!=0&&!p->thread);p->next[q+8192U+112U]^=1;
 CHECK(hash(&owner,p->next,z5-32U,p->next+z5-32U));
 if(base_version>=4U){q=r32(p->next+276)+64U+88U;p->next[q]^=1;CHECK(hash(&owner,p->next,z5-32U,p->next+z5-32U));CHECK(papacc_v5_worker_fixture_start(&owner,identity)!=0&&!p->thread);p->next[q]^=1;CHECK(hash(&owner,p->next,z5-32U,p->next+z5-32U));}
 owner.published=1;CHECK(papacc_v5_worker_fixture_start(&owner,identity)!=0);owner.published=0;
 if(identity_conflict){CHECK(strlen(argv[2])+10U<MAX_PATH);strcpy_s(identity_path,MAX_PATH,argv[2]);strcat_s(identity_path,MAX_PATH,".identity");CHECK(CloseHandle(identity));
 identity=CreateFileA(identity_path,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);CHECK(identity!=INVALID_HANDLE_VALUE);}
 if(conflict&&!identity_conflict){file=CreateFileA(argv[2],GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,0,NULL);CHECK(file!=INVALID_HANDLE_VALUE);CHECK(SetFilePointer(file,(LONG)(bn-1U),NULL,FILE_BEGIN)!=INVALID_SET_FILE_POINTER);CHECK(WriteFile(file,"X",1,&count,NULL)&&count==1&&FlushFileBuffers(file));CHECK(CloseHandle(file));}
 if(stop){p->test_entered=CreateEventA(NULL,TRUE,FALSE,NULL);p->test_release=CreateEventA(NULL,TRUE,FALSE,NULL);CHECK(p->test_entered&&p->test_release);}
 p->test_unknown=unknown;
 CHECK(papacc_v5_worker_fixture_start(&owner,identity)==0);CHECK(papacc_v5_worker_fixture_start(&owner,identity)!=0);CHECK(CloseHandle(identity));
 if(stop){CHECK(WaitForSingleObject(p->test_entered,10000)==WAIT_OBJECT_0);start=GetTickCount();CHECK(papacc_configuration_stop(&owner)==0);CHECK(GetTickCount()-start<1000);CHECK(papacc_configuration_finish(&owner)!=0);CHECK(SetEvent(p->test_release));}
 start=GetTickCount();do{CHECK(papacc_v5_worker_fixture_poll(&owner,&outcome)==0);if(outcome==PAPACC_COMMIT_PENDING)Sleep(1);}while(outcome==PAPACC_COMMIT_PENDING&&GetTickCount()-start<10000);
 CHECK(outcome==(stop?PAPACC_COMMIT_FAILED:unknown?PAPACC_CAS_UNKNOWN:conflict?PAPACC_CAS_CONFLICT:PAPACC_CAS_SUCCESS));
 if(unknown)CHECK(owner.unresolved&&owner.authority_lost&&p->transaction&&p->file!=INVALID_HANDLE_VALUE&&!p->committed);
 if(stop)CHECK(!p->transaction&&p->file==INVALID_HANDLE_VALUE&&!p->committed);
 CHECK(!memcmp(owner.committed,old_image,bn)&&!memcmp(p->previous,old_image,bn));
 CHECK(!memcmp(p->next,e5,z5)&&!owner.published);
 CHECK(!memcmp(p->previous+r32(old_image+48),p->next+r32(e5+48),16384));
 CHECK(papacc_configuration_publish(&owner)!=0);
 if(!unknown){file=CreateFileA(argv[2],GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);CHECK(file!=INVALID_HANDLE_VALUE);CHECK(ReadFile(file,observed_v5,sizeof(observed_v5),&count,NULL));CHECK(CloseHandle(file));
 if(!stop&&!conflict){CHECK(count==z5&&!memcmp(observed_v5,e5,z5));}else if(stop||identity_conflict){CHECK(count==bn&&!memcmp(observed_v5,old_image,bn));}
 else if(conflict){CHECK(count==bn&&!memcmp(observed_v5,old_image,bn-1U)&&observed_v5[bn-1U]=='X');}}
 if(stop){CHECK(CloseHandle(p->test_entered));CHECK(CloseHandle(p->test_release));}
 CHECK(papacc_configuration_stop(&owner)==0);if(!stop)CHECK(papacc_configuration_finish(&owner)!=0);
 printf("PASS V5_NATIVE_SAME_WORKER mode=%s outcome=%d bytes=%Iu old_bytes=%Iu owner=%Iu pending=%Iu SYNTHETIC_ADMIN_NO_CURRENT_NO_TRUST_RETAINED\n",argv[1],outcome,z5,bn,sizeof(owner),sizeof(*p));return 0;
}
