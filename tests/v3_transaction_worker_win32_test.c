/* Synthetic full v3 administrative context, REAL same-worker native TxF CAS.
 * No root CURRENT/authority, no keys/certificates generated or signature. */
#define _WIN32_WINNT 0x0600
#include "config_root8_v3_test.c"
extern PAPACC_RESULT papacc_v3_worker_fixture_start(PAPACC_CONFIGURATION_COMMIT *,HANDLE);
extern PAPACC_RESULT papacc_v3_worker_fixture_poll(PAPACC_CONFIGURATION_COMMIT *,PAPACC_ISSUER_CAS_RESULT *);
static PAPACC_U8 observed_v3[PC_ROOT8_V3_MAX];
int main(int argc,char **argv)
{
 PAPACC_CONFIG_PENDING *p=&owner.pending_commit;HANDLE file,identity;DWORD count,start;
 PAPACC_ISSUER_CAS_RESULT outcome=PAPACC_COMMIT_IDLE;PAPACC_SIZE bn;unsigned q;int stop,unknown,conflict;
 CHECK(argc==3);stop=!strcmp(argv[1],"stop");unknown=!strcmp(argv[1],"unknown");conflict=!strcmp(argv[1],"conflict");
 CHECK(papacc_configuration_commit_init03(&owner,argv[2])==0);
 CHECK(setup3(4,4,4));CHECK(cp3());bn=z3;memcpy(base3,e3,bn);CHECK(next3(3,bn));
 CHECK(pc_root8_v3_base_link(e3,z3,base3,bn,hash,&owner,component3,NULL)==PC_ROOT8_OK);
 CHECK(sizeof(p->next)==PC_ROOT8_V3_MAX&&sizeof(owner.committed)==PC_ROOT8_V3_MAX);
 memcpy(owner.committed,base3,bn);owner.committed_bytes=(DWORD)bn;
 memcpy(p->previous,base3,bn);p->previous_bytes=(DWORD)bn;memcpy(p->next,e3,z3);p->next_bytes=(DWORD)z3;
 file=CreateFileA(argv[2],GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);CHECK(file!=INVALID_HANDLE_VALUE);
 CHECK(WriteFile(file,base3,(DWORD)bn,&count,NULL)&&count==bn&&FlushFileBuffers(file));CHECK(CloseHandle(file));
 identity=CreateFileA(argv[2],0,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);CHECK(identity!=INVALID_HANDLE_VALUE);
 q=r32(p->previous+56);p->previous[q+64]^=1;CHECK(papacc_v3_worker_fixture_start(&owner,identity)!=0&&!p->thread);p->previous[q+64]^=1;
 p->next[112]^=1;CHECK(papacc_v3_worker_fixture_start(&owner,identity)!=0&&!p->thread);p->next[112]^=1;
 p->next[z3-1]^=1;CHECK(papacc_v3_worker_fixture_start(&owner,identity)!=0&&!p->thread);p->next[z3-1]^=1;
 owner.published=1;CHECK(papacc_v3_worker_fixture_start(&owner,identity)!=0);owner.published=0;
 if(conflict){file=CreateFileA(argv[2],GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,0,NULL);CHECK(file!=INVALID_HANDLE_VALUE);CHECK(SetFilePointer(file,(LONG)(bn-1U),NULL,FILE_BEGIN)!=INVALID_SET_FILE_POINTER);CHECK(WriteFile(file,"X",1,&count,NULL)&&count==1&&FlushFileBuffers(file));CHECK(CloseHandle(file));}
 if(stop){p->test_entered=CreateEventA(NULL,TRUE,FALSE,NULL);p->test_release=CreateEventA(NULL,TRUE,FALSE,NULL);CHECK(p->test_entered&&p->test_release);}
 p->test_unknown=unknown;
 CHECK(papacc_v3_worker_fixture_start(&owner,identity)==0);CHECK(CloseHandle(identity));
 if(stop){CHECK(WaitForSingleObject(p->test_entered,10000)==WAIT_OBJECT_0);start=GetTickCount();CHECK(papacc_configuration_stop(&owner)==0);CHECK(GetTickCount()-start<1000);CHECK(papacc_configuration_finish(&owner)!=0);CHECK(SetEvent(p->test_release));}
 start=GetTickCount();do{CHECK(papacc_v3_worker_fixture_poll(&owner,&outcome)==0);if(outcome==PAPACC_COMMIT_PENDING)Sleep(1);}while(outcome==PAPACC_COMMIT_PENDING&&GetTickCount()-start<10000);
 CHECK(outcome==(stop?PAPACC_COMMIT_FAILED:unknown?PAPACC_CAS_UNKNOWN:conflict?PAPACC_CAS_CONFLICT:PAPACC_CAS_SUCCESS));
 CHECK(!memcmp(owner.committed,base3,bn)&&!memcmp(p->previous,base3,bn));
 CHECK(!memcmp(p->next,e3,z3)&&!owner.published);
 CHECK(!memcmp(p->previous+r32(base3+48),p->next+r32(e3+48),16384));
 CHECK(papacc_configuration_publish(&owner)!=0);
 if(!unknown){file=CreateFileA(argv[2],GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);CHECK(file!=INVALID_HANDLE_VALUE);CHECK(ReadFile(file,observed_v3,sizeof(observed_v3),&count,NULL));CHECK(CloseHandle(file));
 if(!stop&&!conflict){CHECK(count==z3&&!memcmp(observed_v3,e3,z3));}else if(stop){CHECK(count==bn&&!memcmp(observed_v3,base3,bn));}}
 if(stop){CHECK(CloseHandle(p->test_entered));CHECK(CloseHandle(p->test_release));}
 CHECK(papacc_configuration_stop(&owner)==0);if(!stop)CHECK(papacc_configuration_finish(&owner)!=0);
 printf("PASS V3_NATIVE_SAME_WORKER mode=%s outcome=%d bytes=%Iu old_bytes=%Iu owner=%Iu pending=%Iu SYNTHETIC_ADMIN_NO_CURRENT_NO_TRUST_RETAINED\n",argv[1],outcome,z3,bn,sizeof(owner),sizeof(*p));return 0;
}
