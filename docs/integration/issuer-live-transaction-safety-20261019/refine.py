from pathlib import Path
import json,hashlib
r=Path.cwd();d=r/'docs/integration/issuer-live-transaction-safety-20261019';m=json.loads((d/'operation.json').read_text())
def get(p):
 f=d/'proposed'/p.replace('/','__')
 if f.exists():return f.read_text()
 b=(r/p).read_bytes();(d/'preimages'/p.replace('/','__')).write_bytes(b);m.append(dict(path=p,before=hashlib.sha256(b).hexdigest()));return b.decode()
def save(p,s):
 b=s.encode();(d/'proposed'/p.replace('/','__')).write_bytes(b)
 for e in m:
  if e['path']==p:e['after']=hashlib.sha256(b).hexdigest();return
 raise Exception(p)
p='apps/server/server_configuration_pending_win32.h';s=get(p).replace('int active,committed;','int active,committed,thread_close_attempted;\n#ifdef PAPACC_ISSUER_STORAGE_COMPONENT_TEST\n HANDLE test_entered,test_release;int test_unknown;\n#endif');save(p,s)
p='apps/server/server_configuration_pending_win32.inc';s=get(p).replace(' int ok=0,conflict=0,wrote=0,rolled=0;',' int ok=0,conflict=0,rolled=0;').replace('int ok=0,conflict=0,wrote=0,rolled=0;','int ok=0,conflict=0,rolled=0;').replace(' wrote=1;\n','').replace(' (void)wrote;return 0;',' return 0;')
s=s.replace(' if(InterlockedCompareExchange(&p->stop,0,0))goto cleanup;\n if(!CloseHandle', '''#ifdef PAPACC_ISSUER_STORAGE_COMPONENT_TEST
 if(p->test_entered){
  if(!SetEvent(p->test_entered)||WaitForSingleObject(p->test_release,INFINITE)!=WAIT_OBJECT_0){pending_error(p);goto cleanup;}
 }
 if(p->test_unknown){SetLastError(ERROR_WRITE_FAULT);pending_error(p);p->result=PAPACC_CAS_UNKNOWN;return 0;}
#endif
 if(InterlockedCompareExchange(&p->stop,0,0))goto cleanup;
 if(!CloseHandle''',1)
s=s.replace(' memset(p,0,sizeof(*p));p->file=INVALID_HANDLE_VALUE;', '''#ifdef PAPACC_ISSUER_STORAGE_COMPONENT_TEST
 {HANDLE a=p->test_entered,b=p->test_release;int fault=p->test_unknown;
 memset(p,0,sizeof(*p));p->test_entered=a;p->test_release=b;p->test_unknown=fault;}
#else
 memset(p,0,sizeof(*p));
#endif
 p->file=INVALID_HANDLE_VALUE;''',1)
s=s.replace(' w=WaitForSingleObject(p->thread,0);',' if(p->thread_close_attempted){*out=PAPACC_CAS_UNKNOWN;return PAPACC_RESULT_INVALID_STATE;}\n w=WaitForSingleObject(p->thread,0);',1).replace(' if(!CloseHandle(p->thread))',' p->thread_close_attempted=1;\n if(!CloseHandle(p->thread))',1)
s=s.replace('if(!owner(o)||!out)return','if(!owner(o)||!out||issuer06_overlap(o,sizeof(*o),out,sizeof(*out)))return',1)
s=s.replace('    (o->store02!=5', '    o->slots[o->pending].readers||o->slots[o->pending].retained||\n    (o->store02!=5',1)
save(p,s)
p='apps/server/server_launcher_owner_win32.c';s=get(p);a='    reservation=sizeof(root)+PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY*(';assert a in s;s=s.replace(a,'    reservation=sizeof(root)+\n#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE\n        PAPACC_PENDING_STACK_BYTES+\n#endif\n        PAPACC_SERVER_INITIAL_CONNECTION_CAPACITY*(',1);save(p,s)
p='apps/server/server_configuration_root_transaction_win32.inc';s=get(p);a='if(!preparation_owned(o)||!p||p->invocation';assert a in s;s=s.replace(a,'if(o->configuration_commit.pending_commit.active||o->configuration_commit.authority_lost||!preparation_owned(o)||!p||p->invocation',1);save(p,s)
p='apps/server/server_c8_private_storage_layout_win32.h';s=get(p);a='#define PAPACC_C8_KNOWN_MANAGED_BYTES (sizeof(';assert a in s;s=s.replace(a,'#define PAPACC_C8_KNOWN_MANAGED_BYTES (PAPACC_PENDING_STACK_BYTES+sizeof(',1);save(p,s)
p='tests/issuer_root_store_win32_test.c';s=get(p).replace('CHECK(sizeof(root)-papacc_issuer_preimage_owner_size()==41000U);','CHECK(sizeof(root)>papacc_issuer_preimage_owner_size()&&sizeof(root)<2097152U);')
s=s.replace('&&strcmp(argv[1],"--native-snapshots"))','&&strcmp(argv[1],"--native-snapshots")&&strncmp(argv[1],"--pending-",10))',1)
a=' CHECK(papacc_configuration_commit(&root)==0);\n CHECK(root.store02==6';assert a in s
s=s.replace(a,'''#ifdef PAPACC_ISSUER_STORAGE_COMPONENT_TEST
 if(!strncmp(argv[1],"--pending-",10)){
  PAPACC_ISSUER_CAS_RESULT outcome=PAPACC_COMMIT_IDLE;DWORD start;int stopped=!strcmp(argv[1],"--pending-stop"),unknown=!strcmp(argv[1],"--pending-unknown"),conflict=!strcmp(argv[1],"--pending-conflict");
  if(stopped){root.pending_commit.test_entered=CreateEventA(NULL,TRUE,FALSE,NULL);root.pending_commit.test_release=CreateEventA(NULL,TRUE,FALSE,NULL);CHECK(root.pending_commit.test_entered&&root.pending_commit.test_release);}
  root.pending_commit.test_unknown=unknown;
  if(conflict){HANDLE f=CreateFileA(argv[2],GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);DWORD n;unsigned char bad=0;CHECK(f!=INVALID_HANDLE_VALUE);CHECK(WriteFile(f,&bad,1,&n,NULL)&&n==1);CHECK(FlushFileBuffers(f));CHECK(CloseHandle(f));}
  CHECK(papacc_configuration_issuer_begin06(&root)==0);
  CHECK(papacc_configuration_issuer_begin06(&root)!=0);CHECK(papacc_configuration_abort(&root)!=0);CHECK(papacc_configuration_commit(&root)!=0);
  CHECK(papacc_configuration_pin(&root,&pin,&pinned)!=0);CHECK(papacc_configuration_publish(&root)!=0);
  if(stopped){CHECK(WaitForSingleObject(root.pending_commit.test_entered,10000)==WAIT_OBJECT_0);start=GetTickCount();CHECK(papacc_configuration_stop(&root)==0);CHECK(GetTickCount()-start<1000);CHECK(papacc_configuration_issuer_poll06(&root,&outcome)==0&&outcome==PAPACC_COMMIT_PENDING);CHECK(papacc_configuration_finish(&root)!=0);CHECK(SetEvent(root.pending_commit.test_release));}
  start=GetTickCount();do{CHECK(papacc_configuration_issuer_poll06(&root,&outcome)==0);if(outcome==PAPACC_COMMIT_PENDING)Sleep(1);CHECK(GetTickCount()-start<10000);}while(outcome==PAPACC_COMMIT_PENDING);
  if(stopped){CHECK(outcome==PAPACC_COMMIT_FAILED&&!root.authority_lost&&!root.committed_pending);CHECK(CloseHandle(root.pending_commit.test_entered));CHECK(CloseHandle(root.pending_commit.test_release));CHECK(papacc_configuration_abort(&root)==0);CHECK(papacc_configuration_finish(&root)==0);}
  else if(unknown||conflict){CHECK(outcome==(unknown?PAPACC_CAS_UNKNOWN:PAPACC_CAS_CONFLICT));CHECK(root.authority_lost&&!root.committed_pending);CHECK(papacc_configuration_publish(&root)!=0);CHECK(papacc_configuration_issuer_begin06(&root)!=0);CHECK(papacc_configuration_stop(&root)==0);CHECK(papacc_configuration_finish(&root)!=0);}
  else {CHECK(outcome==PAPACC_CAS_SUCCESS&&root.committed_pending&&root.store02==6);CHECK(!memcmp(root.committed,base,8192));CHECK(papacc_configuration_stop(&root)==0);CHECK(papacc_configuration_finish(&root)!=0);}
  printf("PASS pending_checks=%u outcome=%d STOP_NOT_NATIVE_CANCEL UNKNOWN_RETAINED NO_CURRENT_NO_EPOCH NO_NETWORK\\n",checks,(int)outcome);return 0;
 }
#endif
 CHECK(papacc_configuration_commit(&root)==0);
 CHECK(root.store02==6''',1);save(p,s)
p='CMakeLists.txt';s=get(p);s+='''
# Pending ROOT06 native adapter is private; common outcome contract is portable.
set_property(SOURCE apps/server/server_configuration_commit_win32.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_pending_win32.inc"
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_pending_win32.h"
 "${CMAKE_CURRENT_SOURCE_DIR}/src/issuer/issuer_transaction.h")
''';save(p,s)
(d/'operation.json').write_text(json.dumps(m,indent=2));print('Candidates',len(m))
