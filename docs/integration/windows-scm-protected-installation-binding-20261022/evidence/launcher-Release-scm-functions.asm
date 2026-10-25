papacc_server_launcher_scm_prepare PROC			; COMDAT

; 39   : {

$LN122:
	push	rbp
	push	rbx
	push	rsi
	push	rdi
	push	r12
	push	r13
	push	r14
	push	r15
	lea	rbp, QWORD PTR [rsp-60376]
	mov	eax, 60632				; 0000ecd8H
	call	__chkstk
	sub	rsp, rax
	mov	rax, QWORD PTR __security_cookie
	xor	rax, rsp
	mov	QWORD PTR __$ArrayPad$[rbp-256], rax
	mov	rax, QWORD PTR err$[rbp-256]

; 40   :  PAPACC_CONFIGURATION_COMMIT *c;PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;
; 41   :  PC_DOCUMENT document;PC_DIAGNOSTIC diagnostic;PC_TARGET target={1U,0};

	xor	esi, esi
	mov	QWORD PTR err$GSCopy$1$[rsp], rax
	mov	r12d, 1
	mov	QWORD PTR target$[rsp], r12
	mov	rbx, r9
	mov	r15, r8
	mov	r13, rdx
	mov	r14, rcx

; 43   :  const char *stage="ENTRY";
; 44   :  if(!installation||!configuration||!image||!image->handle||!image->path||!image->sha256||!out||!err||image->owner_thread!=GetCurrentThreadId())return 2;

	test	rcx, rcx
	je	$LN12@papacc_ser
	test	rdx, rdx
	je	$LN12@papacc_ser
	test	r8, r8
	je	$LN12@papacc_ser
	cmp	QWORD PTR [r8], rsi
	je	$LN12@papacc_ser
	cmp	QWORD PTR [r8+64], rsi
	je	$LN12@papacc_ser
	cmp	QWORD PTR [r8+72], rsi
	je	$LN12@papacc_ser
	test	rbx, rbx
	je	$LN12@papacc_ser
	test	rax, rax
	je	$LN12@papacc_ser
	mov	edi, DWORD PTR [r8+80]
	call	QWORD PTR __imp_GetCurrentThreadId
	cmp	edi, eax
	jne	$LN12@papacc_ser

; 45   :  if(InterlockedCompareExchange(&state,PAPACC_LAUNCHER_ACTIVE,0)!=0)return 2;

	xor	eax, eax
	lock cmpxchg DWORD PTR state, r12d
	jne	$LN12@papacc_ser

; 46   :  if(next_invocation==(PAPACC_U64)-1){InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);return 2;}

	mov	rdi, QWORD PTR next_invocation
	cmp	rdi, -1
	jne	SHORT $LN14@papacc_ser
	mov	esi, 2
	mov	eax, esi
	xchg	DWORD PTR state, eax
	mov	eax, esi
	jmp	$LN1@papacc_ser
$LN14@papacc_ser:
	xorps	xmm0, xmm0

; 42   :  PS_LAB_BINDING binding;PS_BINDING_FACTS facts;const WCHAR *leaf;int prepared=0,clean=1;unsigned i;

	mov	DWORD PTR prepared$1$[rsp], esi

; 47   :  memset(&root,0,sizeof(root));root.invocation=++next_invocation;root.owner_thread=GetCurrentThreadId();c=&root.configuration_commit;p=&c->pending_commit;n=&p->personal_native;

	xor	eax, eax
	lea	rcx, OFFSET FLAT:root+160
	xor	edx, edx
	mov	QWORD PTR root+144, rax
	mov	r8d, 340480				; 00053200H
	movups	XMMWORD PTR root, xmm0
	movups	XMMWORD PTR root+16, xmm0
	movups	XMMWORD PTR root+32, xmm0
	movups	XMMWORD PTR root+48, xmm0
	movups	XMMWORD PTR root+64, xmm0
	movups	XMMWORD PTR root+80, xmm0
	movups	XMMWORD PTR root+96, xmm0
	movups	XMMWORD PTR root+112, xmm0
	movups	XMMWORD PTR root+128, xmm0
	call	memset
	inc	rdi
	mov	QWORD PTR next_invocation, rdi
	mov	QWORD PTR root+152, rdi
	call	QWORD PTR __imp_GetCurrentThreadId

; 48   :  c->writer=root.owner_thread;p->file=INVALID_HANDLE_VALUE;memset(&binding,0,sizeof(binding));memset(&facts,0,sizeof(facts));

	xor	edx, edx
	mov	QWORD PTR root+111136, -1
	mov	r8d, 292				; 00000124H
	mov	DWORD PTR root+322624, eax
	lea	rcx, QWORD PTR binding$[rbp-256]
	mov	DWORD PTR root+37800, eax
	call	memset
	xor	edx, edx
	lea	rcx, QWORD PTR facts$[rbp-256]
	mov	r8d, 288				; 00000120H
	call	memset

; 49   :  root.reserved_bytes=PAPACC_C8_KNOWN_MANAGED_BYTES+sizeof(document)+sizeof(diagnostic)+sizeof(binding)+sizeof(facts)+papacc_server_service_storage_bytes()+sizeof(LONG)+4096U;

	call	papacc_server_service_storage_bytes
	add	rax, 2029712				; 001ef890H

; 50   :  stage="CAPACITY";if(root.reserved_bytes>PAPACC_LAUNCHER_MANAGED_LIMIT)goto done;

	mov	esi, 2
	mov	QWORD PTR root+322632, rax
	cmp	rax, 3145728				; 00300000H
	ja	$LN60@papacc_ser

; 51   :  stage="NATIVE_OPERATOR_PARENT";if(papacc_configuration_personal_native_open(c,installation)!=PAPACC_RESULT_OK){stage=p->bootstrap_token?"PARENT_OR_REVALIDATION":"OPERATOR_ELIGIBILITY_OR_CAPTURE";goto done;}

	mov	rdx, r14
	lea	rcx, OFFSET FLAT:root+1080
	call	papacc_configuration_personal_native_open
	test	eax, eax
	je	SHORT $LN16@papacc_ser
	cmp	QWORD PTR root+111080, 0
	lea	rax, OFFSET FLAT:??_C@_0BH@EIKCBGKC@PARENT_OR_REVALIDATION@
	lea	r14, OFFSET FLAT:??_C@_0CA@IEMBMEOP@OPERATOR_ELIGIBILITY_OR_CAPTURE@
	cmovne	r14, rax
	jmp	$LN83@papacc_ser
$LN16@papacc_ser:

; 52   :  stage="PARENT_ACL";if(!scm_descriptor(n->descriptor,n->sid))goto done;

	mov	rcx, QWORD PTR root+38224
	lea	rdx, OFFSET FLAT:root+38016
	lea	r14, OFFSET FLAT:??_C@_0L@JELHEKMH@PARENT_ACL@
	call	scm_descriptor
	test	eax, eax
	je	$LN83@papacc_ser

; 53   :  leaf=wcsrchr(image->path,(WCHAR)92);if(!leaf||!leaf[1])goto done;leaf++;

	mov	rcx, QWORD PTR [r15+64]
	mov	edx, 92					; 0000005cH
	call	wcsrchr
	test	rax, rax
	je	$LN83@papacc_ser
	cmp	WORD PTR [rax+2], 0
	lea	rdx, QWORD PTR [rax+2]
	je	$LN83@papacc_ser

; 54   :  stage="RELATIVE_FILES";if(papacc_configuration_scm_lab_files_open(c,leaf,configuration)!=PAPACC_RESULT_OK)goto done;

	mov	r8, r13
	lea	rcx, OFFSET FLAT:root+1080
	call	papacc_configuration_scm_lab_files_open
	test	eax, eax
	jne	$LN61@papacc_ser

; 55   :  stage="FILE_IDENTITY_BYTES";if(papacc_configuration_scm_lab_files_current(c,&image->identity)!=PAPACC_RESULT_OK)goto done;

	lea	rdx, QWORD PTR [r15+8]
	lea	rcx, OFFSET FLAT:root+1080
	call	papacc_configuration_scm_lab_files_current
	test	eax, eax
	jne	$LN62@papacc_ser

; 56   :  stage="CONFIGURATION_DATA";if(pc_parse(p->next,n->source.nFileSizeLow,&document,&diagnostic)!=PC_OK||pc_validate(&document,&target,&diagnostic)!=PC_OK)goto done;

	mov	edx, DWORD PTR root+38180
	lea	r9, QWORD PTR diagnostic$[rsp]
	lea	r8, QWORD PTR document$[rbp-256]
	lea	rcx, OFFSET FLAT:root+78416
	lea	r14, OFFSET FLAT:??_C@_0BD@HKJLPDEI@CONFIGURATION_DATA@
	call	pc_parse
	test	eax, eax
	jne	$LN83@papacc_ser
	lea	r8, QWORD PTR diagnostic$[rsp]
	lea	rdx, QWORD PTR target$[rsp]
	lea	rcx, QWORD PTR document$[rbp-256]
	call	pc_validate
	test	eax, eax
	jne	$LN83@papacc_ser

; 57   :  stage="ACL_IDENTITY";if(!scm_acl_hash(n,p->file,facts.config_acl)||!scm_acl_hash(n,n->scm_image,facts.image_acl)||

	mov	rdx, QWORD PTR root+111136
	lea	r8, QWORD PTR facts$[rbp-36]
	lea	rcx, OFFSET FLAT:root+37856
	lea	r14, OFFSET FLAT:??_C@_0N@JEOEJEPK@ACL_IDENTITY@
	call	scm_acl_hash
	test	eax, eax
	je	$LN83@papacc_ser
	mov	rdx, QWORD PTR root+38216
	lea	r8, QWORD PTR facts$[rbp-4]
	lea	rcx, OFFSET FLAT:root+37856
	call	scm_acl_hash
	test	eax, eax
	je	$LN83@papacc_ser
	mov	r8d, DWORD PTR root+38248
	lea	r9, QWORD PTR facts$[rbp-68]
	mov	rdx, QWORD PTR root+38224
	xor	ecx, ecx
	call	pacc_storage_hash_callback
	test	eax, eax
	je	$LN83@papacc_ser

; 37   : {id[0]=f->dwVolumeSerialNumber;id[1]=f->nFileIndexHigh;id[2]=f->nFileIndexLow;}

	mov	eax, DWORD PTR root+38120

; 61   :  if(!pacc_storage_hash_callback(NULL,p->next,facts.configuration_bytes,facts.config_hash)||!pacc_storage_hash_callback(NULL,n->sid,n->sid_bytes,facts.operator_id))goto done;

	lea	r9, QWORD PTR facts$[rbp-132]
	mov	ecx, DWORD PTR root+38180
	lea	rdx, OFFSET FLAT:root+78416

; 37   : {id[0]=f->dwVolumeSerialNumber;id[1]=f->nFileIndexHigh;id[2]=f->nFileIndexLow;}

	mov	DWORD PTR facts$[rbp-256], eax

; 61   :  if(!pacc_storage_hash_callback(NULL,p->next,facts.configuration_bytes,facts.config_hash)||!pacc_storage_hash_callback(NULL,n->sid,n->sid_bytes,facts.operator_id))goto done;

	mov	r8d, ecx

; 37   : {id[0]=f->dwVolumeSerialNumber;id[1]=f->nFileIndexHigh;id[2]=f->nFileIndexLow;}

	mov	eax, DWORD PTR root+38136
	mov	DWORD PTR facts$[rbp-252], eax
	mov	eax, DWORD PTR root+38140
	mov	DWORD PTR facts$[rbp-248], eax
	mov	eax, DWORD PTR root+38172
	mov	DWORD PTR facts$[rbp-244], eax
	mov	eax, DWORD PTR root+38188
	mov	DWORD PTR facts$[rbp-240], eax
	mov	eax, DWORD PTR root+38192
	mov	DWORD PTR facts$[rbp-236], eax
	mov	eax, DWORD PTR [r15+36]
	mov	DWORD PTR facts$[rbp-232], eax
	mov	eax, DWORD PTR [r15+52]
	mov	DWORD PTR facts$[rbp-228], eax
	mov	eax, DWORD PTR [r15+56]
	mov	DWORD PTR facts$[rbp-224], eax

; 58   :  !pacc_storage_hash_callback(NULL,(const unsigned char *)n->descriptor,n->descriptor_bytes,facts.parent_acl))goto done;
; 59   :  scm_id(facts.installation,&n->parent);scm_id(facts.configuration,&n->source);scm_id(facts.executable,&image->identity);
; 60   :  facts.configuration_bytes=n->source.nFileSizeLow;memcpy(facts.image_hash,image->sha256,32);

	mov	rax, QWORD PTR [r15+72]
	mov	DWORD PTR facts$[rbp+28], ecx

; 61   :  if(!pacc_storage_hash_callback(NULL,p->next,facts.configuration_bytes,facts.config_hash)||!pacc_storage_hash_callback(NULL,n->sid,n->sid_bytes,facts.operator_id))goto done;

	xor	ecx, ecx
	movups	xmm0, XMMWORD PTR [rax]
	movups	XMMWORD PTR facts$[rbp-100], xmm0
	movups	xmm1, XMMWORD PTR [rax+16]
	movups	XMMWORD PTR facts$[rbp-84], xmm1
	call	pacc_storage_hash_callback
	test	eax, eax
	je	$LN83@papacc_ser
	mov	r8d, DWORD PTR root+38084
	lea	r9, QWORD PTR facts$[rbp-164]
	lea	rdx, OFFSET FLAT:root+38016
	xor	ecx, ecx
	call	pacc_storage_hash_callback
	test	eax, eax
	je	$LN83@papacc_ser

; 62   :  facts.token[0]=n->token.TokenId.LowPart;facts.token[1]=(unsigned long)n->token.TokenId.HighPart;

	mov	eax, DWORD PTR root+37960

; 63   :  facts.token[2]=n->token.AuthenticationId.LowPart;facts.token[3]=(unsigned long)n->token.AuthenticationId.HighPart;
; 64   :  facts.token[4]=n->token.ModifiedId.LowPart;facts.token[5]=(unsigned long)n->token.ModifiedId.HighPart;
; 65   :  stage="ATTEMPT";if(BCryptGenRandom(NULL,facts.attempt,32U,BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0||!ps_lab_binding_capture(&binding,&facts))goto done;

	lea	rdx, QWORD PTR facts$[rbp-196]
	mov	DWORD PTR facts$[rbp-220], eax
	lea	r14, OFFSET FLAT:??_C@_07HEGJIEJJ@ATTEMPT@
	mov	eax, DWORD PTR root+37964
	mov	r9d, esi
	mov	DWORD PTR facts$[rbp-216], eax
	mov	r8d, 32					; 00000020H
	mov	eax, DWORD PTR root+37968
	xor	ecx, ecx
	mov	DWORD PTR facts$[rbp-212], eax
	mov	eax, DWORD PTR root+37972
	mov	DWORD PTR facts$[rbp-208], eax
	mov	eax, DWORD PTR root+38008
	mov	DWORD PTR facts$[rbp-204], eax
	mov	eax, DWORD PTR root+38012
	mov	DWORD PTR facts$[rbp-200], eax
	call	BCryptGenRandom
	test	eax, eax
	js	$LN83@papacc_ser
	lea	rdx, QWORD PTR facts$[rbp-256]
	lea	rcx, QWORD PTR binding$[rbp-256]
	call	ps_lab_binding_capture
	test	eax, eax
	je	$LN83@papacc_ser

; 66   :  stage="REVALIDATION";if(papacc_configuration_scm_lab_files_current(c,&image->identity)!=PAPACC_RESULT_OK||
; 67   :  !scm_acl_hash(n,p->file,facts.config_acl)||!scm_acl_hash(n,n->scm_image,facts.image_acl)||

	lea	rdx, QWORD PTR [r15+8]
	lea	rcx, OFFSET FLAT:root+1080
	lea	r14, OFFSET FLAT:??_C@_0N@JEGJPHOC@REVALIDATION@
	call	papacc_configuration_scm_lab_files_current
	test	eax, eax
	jne	$LN83@papacc_ser
	mov	rdx, QWORD PTR root+111136
	lea	r8, QWORD PTR facts$[rbp-36]
	lea	rcx, OFFSET FLAT:root+37856
	call	scm_acl_hash
	test	eax, eax
	je	$LN83@papacc_ser
	mov	rdx, QWORD PTR root+38216
	lea	r8, QWORD PTR facts$[rbp-4]
	lea	rcx, OFFSET FLAT:root+37856
	call	scm_acl_hash
	test	eax, eax
	je	$LN83@papacc_ser
	call	papacc_server_console_win32_stop_requested
	mov	r8d, eax
	lea	rdx, QWORD PTR facts$[rbp-256]
	lea	rcx, QWORD PTR binding$[rbp-256]
	call	ps_lab_binding_complete
	test	eax, eax
	je	$LN83@papacc_ser

; 68   :  !ps_lab_binding_complete(&binding,&facts,papacc_server_console_win32_stop_requested()))goto done;
; 69   :  prepared=1;
; 70   :  fprintf(out,"SCM_LAB_PREPARE=BOUND_READ_ONLY; OPERATOR=%s; INSTALLATION=%lu:%lu:%lu; CONFIGURATION=%lu:%lu:%lu; BYTES=%u; ATTEMPT=",

	mov	eax, DWORD PTR facts$[rbp+28]
	lea	rdx, OFFSET FLAT:??_C@_0HG@IMFMBGOJ@SCM_LAB_PREPARE?$DNBOUND_READ_ONLY?$DL@
	mov	r9d, DWORD PTR facts$[rbp-256]
	mov	rcx, rbx
	mov	r8, QWORD PTR root+38240
	mov	r15d, r12d
	mov	DWORD PTR [rsp+72], eax
	mov	eax, DWORD PTR facts$[rbp-236]
	mov	DWORD PTR [rsp+64], eax
	mov	eax, DWORD PTR facts$[rbp-240]
	mov	DWORD PTR [rsp+56], eax
	mov	eax, DWORD PTR facts$[rbp-244]
	mov	DWORD PTR [rsp+48], eax
	mov	eax, DWORD PTR facts$[rbp-248]
	mov	DWORD PTR [rsp+40], eax
	mov	eax, DWORD PTR facts$[rbp-252]
	mov	DWORD PTR [rsp+32], eax
	call	fprintf
	xor	edi, edi
	npad	5
$LL4@papacc_ser:

; 71   :  n->sid_text,facts.installation[0],facts.installation[1],facts.installation[2],facts.configuration[0],facts.configuration[1],facts.configuration[2],facts.configuration_bytes);
; 72   :  for(i=0;i<32;i++)fprintf(out,"%02x",facts.attempt[i]);

	movzx	r8d, BYTE PTR facts$[rbp+rdi-196]
	lea	rdx, OFFSET FLAT:??_C@_04NOJCDH@?$CF02x@
	mov	rcx, rbx
	call	fprintf
	inc	rdi
	cmp	rdi, 32					; 00000020H
	jne	SHORT $LL4@papacc_ser

; 73   :  fprintf(out,"; SERVICE=PapinhoAccelerator; ACCOUNT=NT SERVICE%cPapinhoAccelerator; IMAGE=%lu:%lu:%lu; IMAGE_SHA256=",92,facts.executable[0],facts.executable[1],facts.executable[2]);

	mov	eax, DWORD PTR facts$[rbp-224]
	lea	rdx, OFFSET FLAT:??_C@_0GH@LAELCMMB@?$DL?5SERVICE?$DNPapinhoAccelerator?$DL?5AC@
	mov	r9d, DWORD PTR facts$[rbp-232]
	mov	r8d, 92					; 0000005cH
	mov	DWORD PTR [rsp+40], eax
	mov	rcx, rbx
	mov	eax, DWORD PTR facts$[rbp-228]
	mov	DWORD PTR [rsp+32], eax
	call	fprintf
	xor	edi, edi
	npad	9
$LL7@papacc_ser:

; 74   :  for(i=0;i<32;i++)fprintf(out,"%02x",facts.image_hash[i]);

	movzx	r8d, BYTE PTR facts$[rbp+rdi-100]
	lea	rdx, OFFSET FLAT:??_C@_04NOJCDH@?$CF02x@
	mov	rcx, rbx
	call	fprintf
	inc	rdi
	cmp	rdi, 32					; 00000020H
	jne	SHORT $LL7@papacc_ser

; 75   :  fputs("; CONFIGURATION_SHA256=",out);for(i=0;i<32;i++)fprintf(out,"%02x",facts.config_hash[i]);

	mov	rdx, rbx
	lea	rcx, OFFSET FLAT:??_C@_0BI@KHBCOKOA@?$DL?5CONFIGURATION_SHA256?$DN@
	call	QWORD PTR __imp_fputs
	xor	edi, edi
$LL10@papacc_ser:
	movzx	r8d, BYTE PTR facts$[rbp+rdi-132]
	lea	rdx, OFFSET FLAT:??_C@_04NOJCDH@?$CF02x@
	mov	rcx, rbx
	call	fprintf
	inc	rdi
	cmp	rdi, 32					; 00000020H
	jne	SHORT $LL10@papacc_ser

; 76   :  fputs("; AUTHORIZATION=NOT_ACQUIRED; CREATE_SERVICE=DENY; SCOPE=LAB_ONLY\n",out);

	mov	rdx, rbx
	lea	rcx, OFFSET FLAT:??_C@_0ED@MEPCJDAD@?$DL?5AUTHORIZATION?$DNNOT_ACQUIRED?$DL?5CR@
	call	QWORD PTR __imp_fputs
	jmp	SHORT $done$123
$LN62@papacc_ser:

; 55   :  stage="FILE_IDENTITY_BYTES";if(papacc_configuration_scm_lab_files_current(c,&image->identity)!=PAPACC_RESULT_OK)goto done;

	lea	r14, OFFSET FLAT:??_C@_0BE@MIEAGJDH@FILE_IDENTITY_BYTES@
	jmp	SHORT $LN83@papacc_ser
$LN61@papacc_ser:

; 54   :  stage="RELATIVE_FILES";if(papacc_configuration_scm_lab_files_open(c,leaf,configuration)!=PAPACC_RESULT_OK)goto done;

	lea	r14, OFFSET FLAT:??_C@_0P@OCFPHJHP@RELATIVE_FILES@
	jmp	SHORT $LN83@papacc_ser
$LN60@papacc_ser:

; 50   :  stage="CAPACITY";if(root.reserved_bytes>PAPACC_LAUNCHER_MANAGED_LIMIT)goto done;

	lea	r14, OFFSET FLAT:??_C@_08FIMHDMB@CAPACITY@
$LN83@papacc_ser:

; 77   :  done:
; 78   :  ps_lab_binding_invalidate(&binding);

	mov	r15d, DWORD PTR prepared$1$[rsp]
$done$123:
	lea	rcx, QWORD PTR binding$[rbp-256]
	call	ps_lab_binding_invalidate

; 79   :  if(papacc_configuration_personal_native_close(c)!=PAPACC_RESULT_OK)clean=0;

	lea	rcx, OFFSET FLAT:root+1080
	call	papacc_configuration_personal_native_close
	test	eax, eax
	je	SHORT $LN66@papacc_ser
	xor	r12d, r12d

; 81   :  else InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);

	mov	eax, esi
	jmp	SHORT $LN34@papacc_ser
$LN66@papacc_ser:
; File C:\Program Files (x86)\Windows Kits\10\include\10.0.26100.0\um\winnt.h

; 23417:     __stosb((PBYTE )((DWORD64)vptr), 0, cnt);

	xor	eax, eax
	lea	rdi, OFFSET FLAT:root+78416
	mov	ecx, 31840				; 00007c60H
	rep stosb
	lea	rdi, OFFSET FLAT:root+110256
	mov	ecx, 512				; 00000200H
	rep stosb
; File C:\Projetos\PapinhoAccelerator\apps\server\server_launcher_scm_prepare_win32.inc

; 80   :  if(clean){SecureZeroMemory(p->next,sizeof(p->next));SecureZeroMemory(p->observed,sizeof(p->observed));InterlockedExchange(&state,PAPACC_LAUNCHER_FINISHED);}

	mov	eax, 3
$LN34@papacc_ser:

; 82   :  fprintf(err,"SCM_LAB_BINDING=%s; STAGE=%s; RESOURCES=%s; NO_SCM_EFFECT; CA_CURRENT_AUTHORITY=DENY\n",prepared?"PREPARED_NOT_AUTHORIZED":"DENY",stage,clean?"CLOSED":"RETAINED_UNKNOWN");

	xchg	DWORD PTR state, eax
	mov	rcx, QWORD PTR err$GSCopy$1$[rsp]
	lea	rdx, OFFSET FLAT:??_C@_06IOLFCPIM@CLOSED@
	test	r12d, r12d
	lea	rax, OFFSET FLAT:??_C@_0BB@BFHAEAJP@RETAINED_UNKNOWN@
	lea	r8, OFFSET FLAT:??_C@_04CJBJBILD@DENY@
	mov	r9, r14
	cmovne	rax, rdx
	lea	rdx, OFFSET FLAT:??_C@_0BI@FENIIHJF@PREPARED_NOT_AUTHORIZED@
	test	r15d, r15d
	mov	QWORD PTR [rsp+32], rax
	cmovne	r8, rdx
	lea	rdx, OFFSET FLAT:??_C@_0FG@MBFJFEBH@SCM_LAB_BINDING?$DN?$CFs?$DL?5STAGE?$DN?$CFs?$DL?5RE@
	call	fprintf

; 83   :  return prepared&&clean?0:2;

	test	r15d, r15d
	je	SHORT $LN36@papacc_ser
	test	r12d, r12d
	je	SHORT $LN36@papacc_ser
	xor	esi, esi
$LN36@papacc_ser:
	mov	eax, esi
	jmp	SHORT $LN1@papacc_ser
$LN12@papacc_ser:

; 43   :  const char *stage="ENTRY";
; 44   :  if(!installation||!configuration||!image||!image->handle||!image->path||!image->sha256||!out||!err||image->owner_thread!=GetCurrentThreadId())return 2;

	mov	eax, 2
$LN1@papacc_ser:

; 84   : }

	mov	rcx, QWORD PTR __$ArrayPad$[rbp-256]
	xor	rcx, rsp
	call	__security_check_cookie
	add	rsp, 60632				; 0000ecd8H
	pop	r15
	pop	r14
	pop	r13
	pop	r12
	pop	rdi
	pop	rsi
	pop	rbx
	pop	rbp
	ret	0
papacc_server_launcher_scm_prepare ENDP

scm_descriptor PROC					; COMDAT

; 8    : {

	mov	QWORD PTR [rsp+24], rbx
	push	rbp
	push	rsi
	push	rdi
	push	r14
	push	r15
	lea	rbp, QWORD PTR [rsp-55]
	sub	rsp, 256				; 00000100H
	mov	rax, QWORD PTR __security_cookie
	xor	rax, rsp
	mov	QWORD PTR __$ArrayPad$[rbp-201], rax

; 9    :  PACL acl;PSID owner;BOOL present,def;BYTE system[SECURITY_MAX_SID_SIZE],admins[SECURITY_MAX_SID_SIZE];DWORD sn=sizeof(system),an=sizeof(admins),i;

	mov	DWORD PTR sn$[rsp], 68			; 00000044H
	mov	r15, rdx
	mov	DWORD PTR an$[rsp], 68			; 00000044H
	mov	rbx, rcx

; 10   :  const DWORD write=GENERIC_ALL|GENERIC_WRITE|DELETE|WRITE_DAC|WRITE_OWNER|FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_EA|FILE_WRITE_ATTRIBUTES|FILE_DELETE_CHILD;
; 11   :  if(!data||!IsValidSecurityDescriptor((PSECURITY_DESCRIPTOR)data)||!GetSecurityDescriptorOwner((PSECURITY_DESCRIPTOR)data,&owner,&def)||!owner||!IsValidSid(owner)||
; 12   :  !CreateWellKnownSid(WinLocalSystemSid,NULL,system,&sn)||!CreateWellKnownSid(WinBuiltinAdministratorsSid,NULL,admins,&an)||
; 13   :  (!EqualSid(owner,(PSID)operator_sid)&&!EqualSid(owner,system)&&!EqualSid(owner,admins))||
; 14   :  !GetSecurityDescriptorDacl((PSECURITY_DESCRIPTOR)data,&present,&acl,&def)||!present||!acl||!IsValidAcl(acl))return 0;

	test	rcx, rcx
	je	$LN6@scm_descri
	call	QWORD PTR __imp_IsValidSecurityDescriptor
	test	eax, eax
	je	$LN6@scm_descri
	lea	r8, QWORD PTR def$[rsp]
	mov	rcx, rbx
	lea	rdx, QWORD PTR owner$[rsp]
	call	QWORD PTR __imp_GetSecurityDescriptorOwner
	test	eax, eax
	je	$LN6@scm_descri
	mov	rcx, QWORD PTR owner$[rsp]
	test	rcx, rcx
	je	$LN6@scm_descri
	call	QWORD PTR __imp_IsValidSid
	test	eax, eax
	je	$LN6@scm_descri
	lea	r9, QWORD PTR sn$[rsp]
	xor	edx, edx
	lea	r8, QWORD PTR system$[rbp-201]
	mov	ecx, 22
	call	QWORD PTR __imp_CreateWellKnownSid
	test	eax, eax
	je	$LN6@scm_descri
	lea	r9, QWORD PTR an$[rsp]
	xor	edx, edx
	lea	r8, QWORD PTR admins$[rbp-201]
	mov	ecx, 26
	call	QWORD PTR __imp_CreateWellKnownSid
	test	eax, eax
	je	$LN6@scm_descri
	mov	rcx, QWORD PTR owner$[rsp]
	mov	rdx, r15
	call	QWORD PTR __imp_EqualSid
	test	eax, eax
	jne	SHORT $LN7@scm_descri
	mov	rcx, QWORD PTR owner$[rsp]
	lea	rdx, QWORD PTR system$[rbp-201]
	call	QWORD PTR __imp_EqualSid
	test	eax, eax
	jne	SHORT $LN7@scm_descri
	mov	rcx, QWORD PTR owner$[rsp]
	lea	rdx, QWORD PTR admins$[rbp-201]
	call	QWORD PTR __imp_EqualSid
	test	eax, eax
	je	$LN6@scm_descri
$LN7@scm_descri:
	lea	r9, QWORD PTR def$[rsp]
	mov	rcx, rbx
	lea	r8, QWORD PTR acl$[rsp]
	lea	rdx, QWORD PTR present$[rsp]
	call	QWORD PTR __imp_GetSecurityDescriptorDacl
	test	eax, eax
	je	$LN6@scm_descri
	cmp	DWORD PTR present$[rsp], 0
	je	$LN6@scm_descri
	mov	rcx, QWORD PTR acl$[rsp]
	test	rcx, rcx
	je	$LN6@scm_descri
	call	QWORD PTR __imp_IsValidAcl
	test	eax, eax
	je	$LN6@scm_descri

; 15   :  for(i=0;i<acl->AceCount;i++){

	mov	rcx, QWORD PTR acl$[rsp]
	xor	r14d, r14d
	xor	eax, eax
	cmp	ax, WORD PTR [rcx+4]
	jae	$LN3@scm_descri
	npad	3
$LL4@scm_descri:

; 16   :   ACCESS_ALLOWED_ACE *ace;PACE_HEADER header;PSID sid;
; 17   :   if(!GetAce(acl,i,(void **)&header)||header->AceSize<FIELD_OFFSET(ACCESS_ALLOWED_ACE,SidStart)+8U)return 0;

	lea	r8, QWORD PTR header$1[rsp]
	mov	edx, r14d
	call	QWORD PTR __imp_GetAce
	test	eax, eax
	je	$LN6@scm_descri
	mov	rdi, QWORD PTR header$1[rsp]
	cmp	WORD PTR [rdi+2], 16
	jb	$LN6@scm_descri

; 18   :   if(header->AceFlags&INHERIT_ONLY_ACE)continue;

	test	BYTE PTR [rdi+1], 8
	jne	SHORT $LN2@scm_descri

; 19   :   if(header->AceType!=ACCESS_ALLOWED_ACE_TYPE&&header->AceType!=ACCESS_DENIED_ACE_TYPE)return 0;

	cmp	BYTE PTR [rdi], 1
	ja	$LN6@scm_descri

; 20   :   ace=(ACCESS_ALLOWED_ACE *)header;sid=(PSID)&ace->SidStart;
; 21   :   if(!IsValidSid(sid)||GetLengthSid(sid)>(DWORD)(header->AceSize-FIELD_OFFSET(ACCESS_ALLOWED_ACE,SidStart)))return 0;

	lea	rcx, QWORD PTR [rdi+8]
	call	QWORD PTR __imp_IsValidSid
	test	eax, eax
	je	SHORT $LN6@scm_descri
	mov	rax, QWORD PTR header$1[rsp]
	lea	rcx, QWORD PTR [rdi+8]
	movzx	ebx, WORD PTR [rax+2]
	add	ebx, -8					; fffffff8H
	call	QWORD PTR __imp_GetLengthSid
	cmp	eax, ebx
	ja	SHORT $LN6@scm_descri

; 22   :   if(header->AceType==ACCESS_ALLOWED_ACE_TYPE&&(ace->Mask&write)&&!EqualSid(sid,(PSID)operator_sid)&&!EqualSid(sid,system)&&!EqualSid(sid,admins))return 0;

	mov	rax, QWORD PTR header$1[rsp]
	cmp	BYTE PTR [rax], 0
	jne	SHORT $LN2@scm_descri
	test	DWORD PTR [rdi+4], 1343029590		; 500d0156H
	je	SHORT $LN2@scm_descri
	mov	rdx, r15
	lea	rcx, QWORD PTR [rdi+8]
	call	QWORD PTR __imp_EqualSid
	test	eax, eax
	jne	SHORT $LN2@scm_descri
	lea	rdx, QWORD PTR system$[rbp-201]
	lea	rcx, QWORD PTR [rdi+8]
	call	QWORD PTR __imp_EqualSid
	test	eax, eax
	jne	SHORT $LN2@scm_descri
	lea	rdx, QWORD PTR admins$[rbp-201]
	lea	rcx, QWORD PTR [rdi+8]
	call	QWORD PTR __imp_EqualSid
	test	eax, eax
	je	SHORT $LN6@scm_descri
$LN2@scm_descri:

; 15   :  for(i=0;i<acl->AceCount;i++){

	mov	rcx, QWORD PTR acl$[rsp]
	inc	r14d
	movzx	eax, WORD PTR [rcx+4]
	cmp	r14d, eax
	jb	$LL4@scm_descri
$LN3@scm_descri:

; 23   :  }
; 24   :  return 1;

	mov	eax, 1
	jmp	SHORT $LN1@scm_descri
$LN6@scm_descri:

; 10   :  const DWORD write=GENERIC_ALL|GENERIC_WRITE|DELETE|WRITE_DAC|WRITE_OWNER|FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_EA|FILE_WRITE_ATTRIBUTES|FILE_DELETE_CHILD;
; 11   :  if(!data||!IsValidSecurityDescriptor((PSECURITY_DESCRIPTOR)data)||!GetSecurityDescriptorOwner((PSECURITY_DESCRIPTOR)data,&owner,&def)||!owner||!IsValidSid(owner)||
; 12   :  !CreateWellKnownSid(WinLocalSystemSid,NULL,system,&sn)||!CreateWellKnownSid(WinBuiltinAdministratorsSid,NULL,admins,&an)||
; 13   :  (!EqualSid(owner,(PSID)operator_sid)&&!EqualSid(owner,system)&&!EqualSid(owner,admins))||
; 14   :  !GetSecurityDescriptorDacl((PSECURITY_DESCRIPTOR)data,&present,&acl,&def)||!present||!acl||!IsValidAcl(acl))return 0;

	xor	eax, eax
$LN1@scm_descri:

; 25   : }

	mov	rcx, QWORD PTR __$ArrayPad$[rbp-201]
	xor	rcx, rsp
	call	__security_check_cookie
	mov	rbx, QWORD PTR [rsp+320]
	add	rsp, 256				; 00000100H
	pop	r15
	pop	r14
	pop	rdi
	pop	rsi
	pop	rbp
	ret	0
scm_descriptor ENDP

scm_acl_hash PROC					; COMDAT

; 28   : {

	push	rbx
	push	rbp
	push	rdi
	push	r14
	sub	rsp, 72					; 00000048H

; 29   :  int ok;DWORD bytes;
; 30   :  if(n->check_descriptor)return 0;

	lea	rbx, QWORD PTR [rcx+376]
	mov	rbp, r8
	cmp	QWORD PTR [rbx], 0
	mov	rax, rdx
	mov	rdi, rcx
	jne	$LN7@scm_acl_ha

; 31   :  if(GetSecurityInfo(file,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&n->check_descriptor)!=ERROR_SUCCESS)return 0;

	xor	r14d, r14d
	mov	QWORD PTR [rsp+56], rbx
	mov	QWORD PTR [rsp+48], r14
	xor	r9d, r9d
	mov	QWORD PTR [rsp+40], r14
	mov	edx, 1
	mov	r8d, 5
	mov	QWORD PTR [rsp+32], r14
	mov	rcx, rax
	call	QWORD PTR __imp_GetSecurityInfo
	test	eax, eax
	jne	SHORT $LN7@scm_acl_ha

; 32   :  ok=scm_descriptor(n->check_descriptor,n->sid);bytes=GetSecurityDescriptorLength(n->check_descriptor);

	mov	rcx, QWORD PTR [rbx]
	lea	rdx, QWORD PTR [rdi+160]
	mov	QWORD PTR [rsp+112], rsi
	call	scm_descriptor
	mov	rcx, QWORD PTR [rbx]
	mov	esi, eax
	call	QWORD PTR __imp_GetSecurityDescriptorLength

; 33   :  if(ok)ok=pacc_storage_hash_callback(NULL,(const unsigned char *)n->check_descriptor,bytes,out);

	test	esi, esi
	je	SHORT $LN4@scm_acl_ha
	mov	rdx, QWORD PTR [rbx]
	mov	r9, rbp
	mov	r8d, eax
	xor	ecx, ecx
	call	pacc_storage_hash_callback
	mov	esi, eax
$LN4@scm_acl_ha:

; 34   :  if(LocalFree(n->check_descriptor)){n->phase=2U;return 0;}n->check_descriptor=NULL;return ok;

	mov	rcx, QWORD PTR [rbx]
	call	QWORD PTR __imp_LocalFree
	test	rax, rax
	je	SHORT $LN5@scm_acl_ha
	mov	rsi, QWORD PTR [rsp+112]
	xor	eax, eax
	mov	DWORD PTR [rdi+404], 2

; 35   : }

	add	rsp, 72					; 00000048H
	pop	r14
	pop	rdi
	pop	rbp
	pop	rbx
	ret	0
$LN5@scm_acl_ha:

; 34   :  if(LocalFree(n->check_descriptor)){n->phase=2U;return 0;}n->check_descriptor=NULL;return ok;

	mov	eax, esi
	mov	QWORD PTR [rbx], r14
	mov	rsi, QWORD PTR [rsp+112]

; 35   : }

	add	rsp, 72					; 00000048H
	pop	r14
	pop	rdi
	pop	rbp
	pop	rbx
	ret	0
$LN7@scm_acl_ha:

; 31   :  if(GetSecurityInfo(file,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&n->check_descriptor)!=ERROR_SUCCESS)return 0;

	xor	eax, eax

; 35   : }

	add	rsp, 72					; 00000048H
	pop	r14
	pop	rdi
	pop	rbp
	pop	rbx
	ret	0
scm_acl_hash ENDP

scm_id	PROC						; COMDAT

; 37   : {id[0]=f->dwVolumeSerialNumber;id[1]=f->nFileIndexHigh;id[2]=f->nFileIndexLow;}

	mov	eax, DWORD PTR [rdx+28]
	mov	DWORD PTR [rcx], eax
	mov	eax, DWORD PTR [rdx+44]
	mov	DWORD PTR [rcx+4], eax
	mov	eax, DWORD PTR [rdx+48]
	mov	DWORD PTR [rcx+8], eax
	ret	0
scm_id	ENDP