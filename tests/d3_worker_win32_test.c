#include "../src/core/ed11_process_profile.h"
/* Native filesystem/CAS only; deliberately SYNTHETIC CURRENT/CP fixture.
 * Does not exercise operational root publication/bootstrap or any network. */
#define main root8_codec_fixture_main
#include "config_root8_test.c"
#undef main
int main(int argc,char **argv)
{
 HANDLE file,identity;DWORD written,start;PAPACC_CONFIG_PIN actual_pin;
 PAPACC_CONFIG_VIEW oldview;PAPACC_ISSUER_CAS_RESULT outcome=PAPACC_COMMIT_IDLE;
 PAPACC_D3_COMMITTED_RECEIPT receipt;PAPACC_SIZE exported;
 unsigned i;int stop_case,unknown_case,conflict_case;
 PAPACC_SIZE reservation=2078563U;
 CHECK(argc==3);if(!strcmp(argv[1],"profile-success"))reservation=PAPACC_ED11_PROCESS_LIMIT-8192U;stop_case=!strcmp(argv[1],"stop");unknown_case=!strcmp(argv[1],"unknown");conflict_case=!strcmp(argv[1],"conflict")||!strcmp(argv[1],"conflict-last");
 CHECK(papacc_configuration_commit_init03(&owner,argv[2])==0);
 CHECK(setup(4,0));CHECK(synthetic_checkpoint());base_used=used;memcpy(base,bytes,used);
 config_init(0,1);oldview=view;
 CHECK(setup(4,1));
 /* A test fixture assigns context, not a product bootstrap/recovery path. */
 owner.store02=8;memcpy(owner.domain,base+72,32);memcpy(owner.committed,base,base_used);owner.committed_bytes=(DWORD)base_used;
 owner.current=0;owner.slots[0].view=oldview;owner.slots[0].serial=1;owner.next_serial=1;
 owner.slots[0].context_epoch=55;owner.slots[0].operational_epoch=66;owner.published=1;
 CHECK(papacc_configuration_pin(&owner,&actual_pin,&oldview)==0);
 file=CreateFileA(argv[2],GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);CHECK(file!=INVALID_HANDLE_VALUE);
 CHECK(WriteFile(file,base,(DWORD)base_used,&written,NULL)&&written==base_used);CHECK(FlushFileBuffers(file));CHECK(CloseHandle(file));
 /* Access0 handle permits worker exclusive RW; only native identity is read. */
 identity=CreateFileA(argv[2],0,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);CHECK(identity!=INVALID_HANDLE_VALUE);
 CHECK(papacc_configuration_d3_begin08(&owner,&actual_pin,bytes,used,identity,PAPACC_ED11_PROCESS_LIMIT)==PAPACC_RESULT_LIMIT_EXCEEDED);
 CHECK(!owner.pending_commit.active&&!owner.pending_commit.thread);
 CHECK(papacc_configuration_d3_begin08(&owner,&actual_pin,bytes,used-1U,identity,reservation)!=0);
 bytes[0]^=1;CHECK(papacc_configuration_d3_begin08(&owner,&actual_pin,bytes,used,identity,reservation)!=0);bytes[0]^=1;
 bytes[used-1U]^=1;CHECK(papacc_configuration_d3_begin08(&owner,&actual_pin,bytes,used,identity,reservation)!=0);bytes[used-1U]^=1;
 bytes[16]|=2;CHECK(papacc_configuration_d3_begin08(&owner,&actual_pin,bytes,used,identity,reservation)!=0);bytes[16]&=(PAPACC_U8)0xfdU;
 owner.issuer_snapshot_valid[2]=1;CHECK(papacc_configuration_d3_begin08(&owner,&actual_pin,bytes,used,identity,reservation)!=0);owner.issuer_snapshot_valid[2]=0;
 owner.published=0;CHECK(papacc_configuration_d3_begin08(&owner,&actual_pin,bytes,used,identity,reservation)!=0);owner.published=1;
 actual_pin.serial++;CHECK(papacc_configuration_d3_begin08(&owner,&actual_pin,bytes,used,identity,reservation)!=0);actual_pin.serial--;
 if(!strcmp(argv[1],"reject")){
  CHECK(CloseHandle(identity));CHECK(papacc_configuration_unpin(&owner,&actual_pin,0)==0);CHECK(papacc_configuration_stop(&owner)==0);CHECK(papacc_configuration_finish(&owner)==0);
  printf("PASS D3_REJECTIONS SYNTHETIC_CURRENT NO_WORKER NO_NETWORK\n");return 0;
 }
 if(conflict_case){
  file=CreateFileA(argv[2],GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,NULL,OPEN_EXISTING,0,NULL);CHECK(file!=INVALID_HANDLE_VALUE);
  if(!strcmp(argv[1],"conflict-last")){
   unsigned char changed=(unsigned char)(base[base_used-1U]^1U);
   CHECK(SetFilePointer(file,(LONG)(base_used-1U),NULL,FILE_BEGIN)!=INVALID_SET_FILE_POINTER);
   CHECK(WriteFile(file,&changed,1,&written,NULL)&&written==1);
  }else CHECK(WriteFile(file,"X",1,&written,NULL)&&written==1);CHECK(FlushFileBuffers(file));CHECK(CloseHandle(file));
 }
 if(stop_case){owner.pending_commit.test_entered=CreateEventA(NULL,TRUE,FALSE,NULL);owner.pending_commit.test_release=CreateEventA(NULL,TRUE,FALSE,NULL);CHECK(owner.pending_commit.test_entered&&owner.pending_commit.test_release);}
 owner.pending_commit.test_unknown=unknown_case;
 CHECK(papacc_configuration_d3_begin08(&owner,&actual_pin,bytes,used,identity,reservation)==0);CHECK(CloseHandle(identity));
 CHECK(papacc_configuration_issuer_poll06(&owner,&outcome)!=0);
 if(stop_case){CHECK(WaitForSingleObject(owner.pending_commit.test_entered,10000)==WAIT_OBJECT_0);start=GetTickCount();CHECK(papacc_configuration_stop(&owner)==0);CHECK(GetTickCount()-start<1000);
  CHECK(papacc_configuration_d3_poll08(&owner,&outcome)==0&&outcome==PAPACC_COMMIT_PENDING);CHECK(papacc_configuration_finish(&owner)!=0);CHECK(SetEvent(owner.pending_commit.test_release));}
 start=GetTickCount();do{CHECK(papacc_configuration_d3_poll08(&owner,&outcome)==0);if(outcome==PAPACC_COMMIT_PENDING)Sleep(1);}while(outcome==PAPACC_COMMIT_PENDING&&GetTickCount()-start<10000);
 CHECK(outcome==(stop_case?PAPACC_COMMIT_FAILED:unknown_case?PAPACC_CAS_UNKNOWN:conflict_case?PAPACC_CAS_CONFLICT:PAPACC_CAS_SUCCESS));
 CHECK(!memcmp(owner.committed,base,base_used));CHECK(owner.slots[0].serial==1&&owner.slots[0].readers==1);
 for(i=0;i<PAPACC_CONFIG_SLOTS;i++)CHECK(!owner.issuer_snapshot_valid[i]);
 CHECK(papacc_configuration_publish(&owner)!=0);CHECK(papacc_configuration_d3_begin08(&owner,&actual_pin,bytes,used,INVALID_HANDLE_VALUE,reservation)!=0);
 exported=99;memset(&receipt,0xff,sizeof(receipt));
 if(outcome==PAPACC_CAS_SUCCESS){
  CHECK(papacc_configuration_d3_committed08(&owner,bounded_output,sizeof(bounded_output),&exported,&receipt,reservation+sizeof(receipt))==0);
  CHECK(exported==used&&receipt.bytes==used&&receipt.listener_count==4&&!receipt.epochs_present);
  CHECK(receipt.revision==2&&receipt.predecessor==1&&!memcmp(receipt.root,owner.domain,32));
  CHECK(!memcmp(bounded_output,owner.pending_commit.next,used));
  file=CreateFileA(argv[2],GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);CHECK(file!=INVALID_HANDLE_VALUE);
  CHECK(ReadFile(file,bytes,(DWORD)used,&written,NULL)&&written==used);CHECK(CloseHandle(file));
  CHECK(!memcmp(bytes,bounded_output,used));
  CHECK(!memcmp(receipt.whole_digest,bounded_output+used-32U,32));
  CHECK(!memcmp(owner.committed,base,base_used)&&owner.slots[0].readers==1);
  /* Caller byte image can be reused after begin returns: packet is copied. */
  memset(bytes,0,sizeof(bytes));exported=99;
  CHECK(papacc_configuration_d3_committed08(&owner,bounded_output,sizeof(bounded_output),&exported,&receipt,reservation+sizeof(receipt))==0&&exported==used);
  exported=99;CHECK(papacc_configuration_d3_committed08(&owner,bounded_output,sizeof(bounded_output),&exported,&receipt,PAPACC_ED11_PROCESS_LIMIT)==PAPACC_RESULT_LIMIT_EXCEEDED&&!exported);
  exported=99;CHECK(papacc_configuration_d3_committed08(&owner,(PAPACC_U8 *)&owner,PC_ROOT8_NO_ISSUER_MAX,&exported,&receipt,reservation+sizeof(receipt))!=0&&exported==99);
  owner.pending_commit.next[used-1U]^=1;exported=99;
  CHECK(papacc_configuration_d3_committed08(&owner,bounded_output,sizeof(bounded_output),&exported,&receipt,reservation+sizeof(receipt))!=0&&!exported);
  owner.pending_commit.next[used-1U]^=1;
 }else CHECK(papacc_configuration_d3_committed08(&owner,bounded_output,sizeof(bounded_output),&exported,&receipt,reservation+sizeof(receipt))!=0&&!exported);
 printf("COMMITTED_RECEIPT_BYTES=%Iu COPY_DIGEST_SCRATCH=32 BEGIN_VIEW_IDENTITY=1156 ELIMINATED_COPY=9376 EXPLICIT_EPOCH_ABSENCE\n",sizeof(receipt));
 if(stop_case){CHECK(CloseHandle(owner.pending_commit.test_entered));CHECK(CloseHandle(owner.pending_commit.test_release));}
 CHECK(papacc_configuration_stop(&owner)==0);exported=99;
 CHECK(papacc_configuration_d3_committed08(&owner,bounded_output,sizeof(bounded_output),&exported,&receipt,reservation+sizeof(receipt))!=0&&!exported);
 CHECK(papacc_configuration_finish(&owner)!=0);
 printf("PASS D3_NATIVE_CAS checks=%u mode=%s outcome=%d owner=%Iu pending=%Iu RETAINED_NO_CURRENT_NO_EPOCH SYNTHETIC_CONTEXT NO_NETWORK\n",checks,argv[1],outcome,sizeof(owner),sizeof(owner.pending_commit));return 0;
}
