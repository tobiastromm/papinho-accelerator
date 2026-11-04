static int scqa_console_hex(HANDLE out,const char *label,const BYTE *bytes,DWORD n)
{
 static const char hex[]="0123456789abcdef";char value[137];DWORD i;
 if(n>68U||!personal_console_write(out,label))return 0;
 for(i=0;i<n;i++){value[2*i]=hex[bytes[i]>>4];value[2*i+1]=hex[bytes[i]&15U];}value[2*n]=0;
 return personal_console_write(out,value)&&personal_console_write(out,"\r\n");
}
static int scqa_console_projection(PAPACC_CONFIGURATION_COMMIT *o,HANDLE out,const PAPACC_SCQ_PLAN_DATA *p,const PAPACC_PERSONAL_INSTALL_PATHS *paths)
{
 static const char *peer_fields[]={"pid","creation.low","creation.high","token.low","token.high","authentication.low","authentication.high","modified.low","modified.high","groups","enumerated","restricted","elevated","elevation_type","matches","attributes","sid_bytes","token_type","query_error","wait_status","exit_code","session","logon_role","admin_attributes","service_type","service_state","service_sid_type","image.volume","image.high","image.low"};
 static const char *ref_fields[]={"role","rights","volume","high","low","size.low","size.high","attributes","links"};
 DWORD words[30],written,length;char line[224];unsigned i,j;int binary_only;const WCHAR *values[3]={paths->binary,paths->parent,paths->data};
 const char *labels[3]={"BINARIES=","DATA_PARENT=","DATA_CONTAINER="};
 if(!scqa_scope(personal_scq_registered_owner_decision(o),p))return 0;
 /* Binary-only originals intentionally carry no ProgramData destination.
  * Presentation does not select/validate a path or confer any authority. */
 binary_only=PAPACC_SCQ_EXISTING_PATH_PURPOSE(paths,p->purpose)&&
  (paths->mode==PAPACC_PERSONAL_PATHS_BINARY_ONLY||paths->mode==PAPACC_PERSONAL_PATHS_SELECTED_OBJECT);
 if(binary_only&&(paths->parent[0]||paths->data[0]||paths->leaf[0]))return 0;
 if(!personal_console_write(out,"SCQ ONLY: one diagnostic, no NEW_INSTALL/CA/production authority.\r\nSERVICE=PapinhoAccelerator ACCOUNT=NT SERVICE\\PapinhoAccelerator\r\n"))return 0;
 (void)sprintf_s(line,sizeof(line),"version=%lu profile=%lu purpose=%lu schema=%lu service=%lu from=%lu:%lu until=%lu:%lu\r\n",p->version,p->profile,p->purpose,p->schema,p->service,p->valid_from[1],p->valid_from[0],p->valid_until[1],p->valid_until[0]);if(!personal_console_write(out,line))return 0;
#define SCQA_HEX(f) if(!scqa_console_hex(out,#f "=",p->f,sizeof(p->f)))return 0
 SCQA_HEX(attempt);SCQA_HEX(policy);SCQA_HEX(request);SCQA_HEX(decision);SCQA_HEX(target);
#undef SCQA_HEX
 for(i=0;i<(binary_only?1U:3U);i++){length=(DWORD)wcsnlen(values[i],MAX_PATH);if(!length||length>=MAX_PATH||!personal_console_write(out,labels[i])||
  !WriteConsoleW(out,values[i],length,&written,NULL)||written!=length||!personal_console_write(out,"\r\n"))return 0;}
 if(binary_only&&!personal_console_write(out,paths->read_diagnostic?"DATA_PARENT=NOT_APPLICABLE_MACHINE_READ\r\nDATA_CONTAINER=NOT_APPLICABLE_MACHINE_READ\r\nP=SEPARATE_ORIGINAL_ACT_REQUIRED; NO_CNG_AUTHORITY_FROM_E_Q_S\r\n":"DATA_PARENT=NOT_APPLICABLE_TRANSPORT_ONLY\r\nDATA_CONTAINER=NOT_APPLICABLE_TRANSPORT_ONLY\r\n"))return 0;
 if(p->purpose==PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY){if(!personal_console_write(out,"KEY=NOT_APPLICABLE_TRANSPORT_ONLY_NO_KSP\r\n"))return 0;}
 else if(!personal_console_write(out,"KEY=")||!personal_console_write(out,p->key)||!personal_console_write(out,"\r\n"))return 0;
 for(i=0;i<2;i++){
  const PAPACC_SCQ_PEER_DATA *q=i?&p->runtime:&p->administrator;
  if(!personal_console_write(out,i?"ROLE=RUNTIME_VIRTUAL\r\n":"ROLE=ADMINISTRATOR\r\n"))return 0;
  memcpy(words,q,sizeof(words));for(j=0;j<30;j++){(void)sprintf_s(line,sizeof(line),"%s=%lu\r\n",peer_fields[j],words[j]);if(!personal_console_write(out,line))return 0;}
  if(!scqa_console_hex(out,"SID=",q->sid,q->sid_bytes)||!scqa_console_hex(out,"IMAGE_SHA256=",q->image_digest,32))return 0;
 }
 for(i=0;i<3;i++){
  memcpy(words,&p->refs[i],9U*sizeof(DWORD));for(j=0;j<9;j++){(void)sprintf_s(line,sizeof(line),"reference[%u].%s=%lu\r\n",i,ref_fields[j],words[j]);if(!personal_console_write(out,line))return 0;}
  if(!scqa_console_hex(out,"CONTENT_SHA256=",p->refs[i].digest,32)||!scqa_console_hex(out,"DESCRIPTOR_SHA256=",p->refs[i].descriptor,32))return 0;
 }
 if(!personal_console_write(out,"By typing the operation-specific line, declare legitimate ownership/administration and that these are the original non-secret SCQ request/decision for THIS live attempt, not a consumed approval or restored grant. Cancel on any mismatch.\r\n"))return 0;
 if(o->pending_commit.personal_native.scq_start_assurance.phase){
  PAPACC_SCQ_START_ASSURANCE *g=&o->pending_commit.personal_native.scq_start_assurance;
  char limits[512];
  if(!scqg_scope(g,o,&o->pending_commit.stop))return 0;
  (void)sprintf_s(limits,sizeof(limits),"G0_G9 LIMITED RISK: private_growth=%llu working_growth=%llu physical_floor=%llu commit_floor=%llu max_handles=%lu freshness_ms=%lu duration_ms=%lu; OPAQUE_UNKNOWN_NOT_ZERO; NO_PRODUCTIVE_PROMOTION\r\n",
   g->limits.private_growth,g->limits.working_growth,g->limits.physical_floor,g->limits.commit_floor,
   (unsigned long)g->limits.max_handles,(unsigned long)g->limits.freshness_ms,(unsigned long)g->limits.duration_ms);
  if(!personal_console_write(out,limits)||scqg_presented(g,o,&o->pending_commit.stop))return 0;
 }else if(!personal_console_write(out,"G3_PLAN=ABSENT; RESOURCE_ADMISSION=DENY\r\n"))return 0;
 return 1;
}
