papacc_server_launcher_scm_prepare PROC

; 39   : {

$LN44:
	mov	QWORD PTR [rsp+32], r9
	mov	QWORD PTR [rsp+24], r8
	mov	QWORD PTR [rsp+16], rdx
	mov	QWORD PTR [rsp+8], rcx
	push	rdi
	mov	eax, 60848				; 0000edb0H
	call	__chkstk
	sub	rsp, rax
	lea	rdi, QWORD PTR [rsp+80]
	mov	ecx, 15192				; 00003b58H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	rcx, QWORD PTR [rsp+60864]
	mov	rax, QWORD PTR __security_cookie
	xor	rax, rsp
	mov	QWORD PTR __$ArrayPad$[rsp], rax

; 40   :  PAPACC_CONFIGURATION_COMMIT *c;PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;
; 41   :  PC_DOCUMENT document;PC_DIAGNOSTIC diagnostic;PC_TARGET target={1U,0};

	mov	DWORD PTR target$[rsp], 1
	mov	DWORD PTR target$[rsp+4], 0

; 42   :  PS_LAB_BINDING binding;PS_BINDING_FACTS facts;const WCHAR *leaf;int prepared=0,clean=1;unsigned i;

	mov	DWORD PTR prepared$[rsp], 0
	mov	DWORD PTR clean$[rsp], 1

; 43   :  const char *stage="ENTRY";

	lea	rax, OFFSET FLAT:$SG111842
	mov	QWORD PTR stage$[rsp], rax

; 44   :  if(!installation||!configuration||!image||!image->handle||!image->path||!image->sha256||!out||!err||image->owner_thread!=GetCurrentThreadId())return 2;

	cmp	QWORD PTR installation$[rsp], 0
	je	SHORT $LN12@papacc_ser
	cmp	QWORD PTR configuration$[rsp], 0
	je	SHORT $LN12@papacc_ser
	cmp	QWORD PTR image$[rsp], 0
	je	SHORT $LN12@papacc_ser
	mov	rax, QWORD PTR image$[rsp]
	cmp	QWORD PTR [rax], 0
	je	SHORT $LN12@papacc_ser
	mov	rax, QWORD PTR image$[rsp]
	cmp	QWORD PTR [rax+64], 0
	je	SHORT $LN12@papacc_ser
	mov	rax, QWORD PTR image$[rsp]
	cmp	QWORD PTR [rax+72], 0
	je	SHORT $LN12@papacc_ser
	cmp	QWORD PTR out$[rsp], 0
	je	SHORT $LN12@papacc_ser
	cmp	QWORD PTR err$[rsp], 0
	je	SHORT $LN12@papacc_ser
	call	QWORD PTR __imp_GetCurrentThreadId
	mov	rcx, QWORD PTR image$[rsp]
	cmp	DWORD PTR [rcx+80], eax
	je	SHORT $LN11@papacc_ser
$LN12@papacc_ser:
	mov	eax, 2
	jmp	$LN1@papacc_ser
$LN11@papacc_ser:

; 45   :  if(InterlockedCompareExchange(&state,PAPACC_LAUNCHER_ACTIVE,0)!=0)return 2;

	mov	eax, 1
	mov	DWORD PTR tv493[rsp], eax
	lea	rcx, OFFSET FLAT:state
	xor	eax, eax
	mov	edx, DWORD PTR tv493[rsp]
	lock cmpxchg DWORD PTR [rcx], edx
	test	eax, eax
	je	SHORT $LN13@papacc_ser
	mov	eax, 2
	jmp	$LN1@papacc_ser
$LN13@papacc_ser:

; 46   :  if(next_invocation==(PAPACC_U64)-1){InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);return 2;}

	cmp	QWORD PTR next_invocation, -1
	jne	SHORT $LN14@papacc_ser
	mov	eax, 2
	lea	rcx, OFFSET FLAT:state
	xchg	DWORD PTR [rcx], eax
	mov	eax, 2
	jmp	$LN1@papacc_ser
$LN14@papacc_ser:

; 47   :  memset(&root,0,sizeof(root));root.invocation=++next_invocation;root.owner_thread=GetCurrentThreadId();c=&root.configuration_commit;p=&c->pending_commit;n=&p->personal_native;

	mov	r8d, 340640				; 000532a0H
	xor	edx, edx
	lea	rcx, OFFSET FLAT:root
	call	memset
	mov	rax, QWORD PTR next_invocation
	inc	rax
	mov	QWORD PTR next_invocation, rax
	mov	rax, QWORD PTR next_invocation
	mov	QWORD PTR root+152, rax
	call	QWORD PTR __imp_GetCurrentThreadId
	mov	DWORD PTR root+322624, eax
	lea	rax, OFFSET FLAT:root+1080
	mov	QWORD PTR c$[rsp], rax
	mov	rax, QWORD PTR c$[rsp]
	add	rax, 36776				; 00008fa8H
	mov	QWORD PTR p$[rsp], rax
	mov	rax, QWORD PTR p$[rsp]
	mov	QWORD PTR n$[rsp], rax

; 48   :  c->writer=root.owner_thread;p->file=INVALID_HANDLE_VALUE;memset(&binding,0,sizeof(binding));memset(&facts,0,sizeof(facts));

	mov	rax, QWORD PTR c$[rsp]
	mov	ecx, DWORD PTR root+322624
	mov	DWORD PTR [rax+36720], ecx
	mov	rax, QWORD PTR p$[rsp]
	mov	QWORD PTR [rax+73280], -1
	mov	r8d, 292				; 00000124H
	xor	edx, edx
	lea	rcx, QWORD PTR binding$[rsp]
	call	memset
	mov	r8d, 288				; 00000120H
	xor	edx, edx
	lea	rcx, QWORD PTR facts$[rsp]
	call	memset

; 49   :  root.reserved_bytes=PAPACC_C8_KNOWN_MANAGED_BYTES+sizeof(document)+sizeof(diagnostic)+sizeof(binding)+sizeof(facts)+papacc_server_service_storage_bytes()+sizeof(LONG)+4096U;

	call	papacc_server_service_storage_bytes
	add	rax, 2029712				; 001ef890H
	mov	QWORD PTR root+322632, rax

; 50   :  stage="CAPACITY";if(root.reserved_bytes>PAPACC_LAUNCHER_MANAGED_LIMIT)goto done;

	lea	rax, OFFSET FLAT:$SG111847
	mov	QWORD PTR stage$[rsp], rax
	cmp	QWORD PTR root+322632, 3145728		; 00300000H
	jbe	SHORT $LN15@papacc_ser
	jmp	$done$45
$LN15@papacc_ser:

; 51   :  stage="NATIVE_OPERATOR_PARENT";if(papacc_configuration_personal_native_open(c,installation)!=PAPACC_RESULT_OK){stage=p->bootstrap_token?"PARENT_OR_REVALIDATION":"OPERATOR_ELIGIBILITY_OR_CAPTURE";goto done;}

	lea	rax, OFFSET FLAT:$SG111849
	mov	QWORD PTR stage$[rsp], rax
	mov	rdx, QWORD PTR installation$[rsp]
	mov	rcx, QWORD PTR c$[rsp]
	call	papacc_configuration_personal_native_open
	test	eax, eax
	je	SHORT $LN16@papacc_ser
	mov	rax, QWORD PTR p$[rsp]
	cmp	QWORD PTR [rax+73224], 0
	je	SHORT $LN36@papacc_ser
	lea	rax, OFFSET FLAT:$SG111851
	mov	QWORD PTR tv144[rsp], rax
	jmp	SHORT $LN37@papacc_ser
$LN36@papacc_ser:
	lea	rax, OFFSET FLAT:$SG111852
	mov	QWORD PTR tv144[rsp], rax
$LN37@papacc_ser:
	mov	rax, QWORD PTR tv144[rsp]
	mov	QWORD PTR stage$[rsp], rax
	jmp	$done$45
$LN16@papacc_ser:

; 52   :  stage="PARENT_ACL";if(!scm_descriptor(n->descriptor,n->sid))goto done;

	lea	rax, OFFSET FLAT:$SG111853
	mov	QWORD PTR stage$[rsp], rax
	mov	rax, QWORD PTR n$[rsp]
	add	rax, 160				; 000000a0H
	mov	rdx, rax
	mov	rax, QWORD PTR n$[rsp]
	mov	rcx, QWORD PTR [rax+368]
	call	scm_descriptor
	test	eax, eax
	jne	SHORT $LN17@papacc_ser
	jmp	$done$45
$LN17@papacc_ser:

; 53   :  leaf=wcsrchr(image->path,(WCHAR)92);if(!leaf||!leaf[1])goto done;leaf++;

	mov	dx, 92					; 0000005cH
	mov	rax, QWORD PTR image$[rsp]
	mov	rcx, QWORD PTR [rax+64]
	call	wcsrchr
	mov	QWORD PTR leaf$[rsp], rax
	cmp	QWORD PTR leaf$[rsp], 0
	je	SHORT $LN19@papacc_ser
	mov	eax, 2
	imul	rax, rax, 1
	mov	rcx, QWORD PTR leaf$[rsp]
	movzx	eax, WORD PTR [rcx+rax]
	test	eax, eax
	jne	SHORT $LN18@papacc_ser
$LN19@papacc_ser:
	jmp	$done$45
$LN18@papacc_ser:
	mov	rax, QWORD PTR leaf$[rsp]
	add	rax, 2
	mov	QWORD PTR leaf$[rsp], rax

; 54   :  stage="RELATIVE_FILES";if(papacc_configuration_scm_lab_files_open(c,leaf,configuration)!=PAPACC_RESULT_OK)goto done;

	lea	rax, OFFSET FLAT:$SG111857
	mov	QWORD PTR stage$[rsp], rax
	mov	r8, QWORD PTR configuration$[rsp]
	mov	rdx, QWORD PTR leaf$[rsp]
	mov	rcx, QWORD PTR c$[rsp]
	call	papacc_configuration_scm_lab_files_open
	test	eax, eax
	je	SHORT $LN20@papacc_ser
	jmp	$done$45
$LN20@papacc_ser:

; 55   :  stage="FILE_IDENTITY_BYTES";if(papacc_configuration_scm_lab_files_current(c,&image->identity)!=PAPACC_RESULT_OK)goto done;

	lea	rax, OFFSET FLAT:$SG111859
	mov	QWORD PTR stage$[rsp], rax
	mov	rax, QWORD PTR image$[rsp]
	add	rax, 8
	mov	rdx, rax
	mov	rcx, QWORD PTR c$[rsp]
	call	papacc_configuration_scm_lab_files_current
	test	eax, eax
	je	SHORT $LN21@papacc_ser
	jmp	$done$45
$LN21@papacc_ser:

; 56   :  stage="CONFIGURATION_DATA";if(pc_parse(p->next,n->source.nFileSizeLow,&document,&diagnostic)!=PC_OK||pc_validate(&document,&target,&diagnostic)!=PC_OK)goto done;

	lea	rax, OFFSET FLAT:$SG111861
	mov	QWORD PTR stage$[rsp], rax
	mov	rax, QWORD PTR n$[rsp]
	mov	eax, DWORD PTR [rax+324]
	mov	rcx, QWORD PTR p$[rsp]
	add	rcx, 40560				; 00009e70H
	lea	r9, QWORD PTR diagnostic$[rsp]
	lea	r8, QWORD PTR document$[rsp]
	mov	edx, eax
	call	pc_parse
	test	eax, eax
	jne	SHORT $LN23@papacc_ser
	lea	r8, QWORD PTR diagnostic$[rsp]
	lea	rdx, QWORD PTR target$[rsp]
	lea	rcx, QWORD PTR document$[rsp]
	call	pc_validate
	test	eax, eax
	je	SHORT $LN22@papacc_ser
$LN23@papacc_ser:
	jmp	$done$45
$LN22@papacc_ser:

; 57   :  stage="ACL_IDENTITY";if(!scm_acl_hash(n,p->file,facts.config_acl)||!scm_acl_hash(n,n->scm_image,facts.image_acl)||

	lea	rax, OFFSET FLAT:$SG111864
	mov	QWORD PTR stage$[rsp], rax
	lea	r8, QWORD PTR facts$[rsp+220]
	mov	rax, QWORD PTR p$[rsp]
	mov	rdx, QWORD PTR [rax+73280]
	mov	rcx, QWORD PTR n$[rsp]
	call	scm_acl_hash
	test	eax, eax
	je	SHORT $LN25@papacc_ser
	lea	r8, QWORD PTR facts$[rsp+252]
	mov	rax, QWORD PTR n$[rsp]
	mov	rdx, QWORD PTR [rax+360]
	mov	rcx, QWORD PTR n$[rsp]
	call	scm_acl_hash
	test	eax, eax
	je	SHORT $LN25@papacc_ser
	mov	rax, QWORD PTR n$[rsp]
	mov	eax, DWORD PTR [rax+392]
	lea	r9, QWORD PTR facts$[rsp+188]
	mov	r8d, eax
	mov	rax, QWORD PTR n$[rsp]
	mov	rdx, QWORD PTR [rax+368]
	xor	ecx, ecx
	call	pacc_storage_hash_callback
	test	eax, eax
	jne	SHORT $LN24@papacc_ser
$LN25@papacc_ser:

; 58   :  !pacc_storage_hash_callback(NULL,(const unsigned char *)n->descriptor,n->descriptor_bytes,facts.parent_acl))goto done;

	jmp	$done$45
$LN24@papacc_ser:

; 59   :  scm_id(facts.installation,&n->parent);scm_id(facts.configuration,&n->source);scm_id(facts.executable,&image->identity);

	mov	rax, QWORD PTR n$[rsp]
	add	rax, 236				; 000000ecH
	mov	rdx, rax
	lea	rcx, QWORD PTR facts$[rsp]
	call	scm_id
	mov	rax, QWORD PTR n$[rsp]
	add	rax, 288				; 00000120H
	mov	rdx, rax
	lea	rcx, QWORD PTR facts$[rsp+12]
	call	scm_id
	mov	rax, QWORD PTR image$[rsp]
	add	rax, 8
	mov	rdx, rax
	lea	rcx, QWORD PTR facts$[rsp+24]
	call	scm_id

; 60   :  facts.configuration_bytes=n->source.nFileSizeLow;memcpy(facts.image_hash,image->sha256,32);

	mov	rax, QWORD PTR n$[rsp]
	mov	eax, DWORD PTR [rax+324]
	mov	DWORD PTR facts$[rsp+284], eax
	mov	r8d, 32					; 00000020H
	mov	rax, QWORD PTR image$[rsp]
	mov	rdx, QWORD PTR [rax+72]
	lea	rcx, QWORD PTR facts$[rsp+156]
	call	memcpy

; 61   :  if(!pacc_storage_hash_callback(NULL,p->next,facts.configuration_bytes,facts.config_hash)||!pacc_storage_hash_callback(NULL,n->sid,n->sid_bytes,facts.operator_id))goto done;

	mov	eax, DWORD PTR facts$[rsp+284]
	mov	rcx, QWORD PTR p$[rsp]
	add	rcx, 40560				; 00009e70H
	lea	r9, QWORD PTR facts$[rsp+124]
	mov	r8d, eax
	mov	rdx, rcx
	xor	ecx, ecx
	call	pacc_storage_hash_callback
	test	eax, eax
	je	SHORT $LN27@papacc_ser
	mov	rax, QWORD PTR n$[rsp]
	mov	eax, DWORD PTR [rax+228]
	mov	rcx, QWORD PTR n$[rsp]
	add	rcx, 160				; 000000a0H
	lea	r9, QWORD PTR facts$[rsp+92]
	mov	r8d, eax
	mov	rdx, rcx
	xor	ecx, ecx
	call	pacc_storage_hash_callback
	test	eax, eax
	jne	SHORT $LN26@papacc_ser
$LN27@papacc_ser:
	jmp	$done$45
$LN26@papacc_ser:

; 62   :  facts.token[0]=n->token.TokenId.LowPart;facts.token[1]=(unsigned long)n->token.TokenId.HighPart;

	mov	eax, 4
	imul	rax, rax, 0
	mov	rcx, QWORD PTR n$[rsp]
	mov	ecx, DWORD PTR [rcx+104]
	mov	DWORD PTR facts$[rsp+rax+36], ecx
	mov	eax, 4
	imul	rax, rax, 1
	mov	rcx, QWORD PTR n$[rsp]
	mov	ecx, DWORD PTR [rcx+108]
	mov	DWORD PTR facts$[rsp+rax+36], ecx

; 63   :  facts.token[2]=n->token.AuthenticationId.LowPart;facts.token[3]=(unsigned long)n->token.AuthenticationId.HighPart;

	mov	eax, 4
	imul	rax, rax, 2
	mov	rcx, QWORD PTR n$[rsp]
	mov	ecx, DWORD PTR [rcx+112]
	mov	DWORD PTR facts$[rsp+rax+36], ecx
	mov	eax, 4
	imul	rax, rax, 3
	mov	rcx, QWORD PTR n$[rsp]
	mov	ecx, DWORD PTR [rcx+116]
	mov	DWORD PTR facts$[rsp+rax+36], ecx

; 64   :  facts.token[4]=n->token.ModifiedId.LowPart;facts.token[5]=(unsigned long)n->token.ModifiedId.HighPart;

	mov	eax, 4
	imul	rax, rax, 4
	mov	rcx, QWORD PTR n$[rsp]
	mov	ecx, DWORD PTR [rcx+152]
	mov	DWORD PTR facts$[rsp+rax+36], ecx
	mov	eax, 4
	imul	rax, rax, 5
	mov	rcx, QWORD PTR n$[rsp]
	mov	ecx, DWORD PTR [rcx+156]
	mov	DWORD PTR facts$[rsp+rax+36], ecx

; 65   :  stage="ATTEMPT";if(BCryptGenRandom(NULL,facts.attempt,32U,BCRYPT_USE_SYSTEM_PREFERRED_RNG)<0||!ps_lab_binding_capture(&binding,&facts))goto done;

	lea	rax, OFFSET FLAT:$SG111869
	mov	QWORD PTR stage$[rsp], rax
	mov	r9d, 2
	mov	r8d, 32					; 00000020H
	lea	rdx, QWORD PTR facts$[rsp+60]
	xor	ecx, ecx
	call	BCryptGenRandom
	test	eax, eax
	jl	SHORT $LN29@papacc_ser
	lea	rdx, QWORD PTR facts$[rsp]
	lea	rcx, QWORD PTR binding$[rsp]
	call	ps_lab_binding_capture
	test	eax, eax
	jne	SHORT $LN28@papacc_ser
$LN29@papacc_ser:
	jmp	$done$45
$LN28@papacc_ser:

; 66   :  stage="REVALIDATION";if(papacc_configuration_scm_lab_files_current(c,&image->identity)!=PAPACC_RESULT_OK||

	lea	rax, OFFSET FLAT:$SG111872
	mov	QWORD PTR stage$[rsp], rax

; 67   :  !scm_acl_hash(n,p->file,facts.config_acl)||!scm_acl_hash(n,n->scm_image,facts.image_acl)||

	mov	rax, QWORD PTR image$[rsp]
	add	rax, 8
	mov	rdx, rax
	mov	rcx, QWORD PTR c$[rsp]
	call	papacc_configuration_scm_lab_files_current
	test	eax, eax
	jne	SHORT $LN31@papacc_ser
	lea	r8, QWORD PTR facts$[rsp+220]
	mov	rax, QWORD PTR p$[rsp]
	mov	rdx, QWORD PTR [rax+73280]
	mov	rcx, QWORD PTR n$[rsp]
	call	scm_acl_hash
	test	eax, eax
	je	SHORT $LN31@papacc_ser
	lea	r8, QWORD PTR facts$[rsp+252]
	mov	rax, QWORD PTR n$[rsp]
	mov	rdx, QWORD PTR [rax+360]
	mov	rcx, QWORD PTR n$[rsp]
	call	scm_acl_hash
	test	eax, eax
	je	SHORT $LN31@papacc_ser
	call	papacc_server_console_win32_stop_requested
	mov	r8d, eax
	lea	rdx, QWORD PTR facts$[rsp]
	lea	rcx, QWORD PTR binding$[rsp]
	call	ps_lab_binding_complete
	test	eax, eax
	jne	SHORT $LN30@papacc_ser
$LN31@papacc_ser:

; 68   :  !ps_lab_binding_complete(&binding,&facts,papacc_server_console_win32_stop_requested()))goto done;

	jmp	$done$45
$LN30@papacc_ser:

; 69   :  prepared=1;

	mov	DWORD PTR prepared$[rsp], 1

; 70   :  fprintf(out,"SCM_LAB_PREPARE=BOUND_READ_ONLY; OPERATOR=%s; INSTALLATION=%lu:%lu:%lu; CONFIGURATION=%lu:%lu:%lu; BYTES=%u; ATTEMPT=",

	mov	eax, 4
	imul	rax, rax, 2
	mov	ecx, 4
	imul	rcx, rcx, 1
	mov	edx, 4
	imul	rdx, rdx, 0
	mov	r8d, 4
	imul	r8, r8, 2
	mov	r9d, 4
	imul	r9, r9, 1
	mov	r10d, 4
	imul	r10, r10, 0
	mov	r11d, DWORD PTR facts$[rsp+284]
	mov	DWORD PTR [rsp+72], r11d
	mov	eax, DWORD PTR facts$[rsp+rax+12]
	mov	DWORD PTR [rsp+64], eax
	mov	eax, DWORD PTR facts$[rsp+rcx+12]
	mov	DWORD PTR [rsp+56], eax
	mov	eax, DWORD PTR facts$[rsp+rdx+12]
	mov	DWORD PTR [rsp+48], eax
	mov	eax, DWORD PTR facts$[rsp+r8]
	mov	DWORD PTR [rsp+40], eax
	mov	eax, DWORD PTR facts$[rsp+r9]
	mov	DWORD PTR [rsp+32], eax
	mov	r9d, DWORD PTR facts$[rsp+r10]
	mov	rax, QWORD PTR n$[rsp]
	mov	r8, QWORD PTR [rax+384]
	lea	rdx, OFFSET FLAT:$SG111875
	mov	rcx, QWORD PTR out$[rsp]
	call	fprintf
	npad	1

; 71   :  n->sid_text,facts.installation[0],facts.installation[1],facts.installation[2],facts.configuration[0],facts.configuration[1],facts.configuration[2],facts.configuration_bytes);
; 72   :  for(i=0;i<32;i++)fprintf(out,"%02x",facts.attempt[i]);

	mov	DWORD PTR i$[rsp], 0
	jmp	SHORT $LN4@papacc_ser
$LN2@papacc_ser:
	mov	eax, DWORD PTR i$[rsp]
	inc	eax
	mov	DWORD PTR i$[rsp], eax
$LN4@papacc_ser:
	cmp	DWORD PTR i$[rsp], 32			; 00000020H
	jae	SHORT $LN3@papacc_ser
	mov	eax, DWORD PTR i$[rsp]
	movzx	eax, BYTE PTR facts$[rsp+rax+60]
	mov	r8d, eax
	lea	rdx, OFFSET FLAT:$SG111876
	mov	rcx, QWORD PTR out$[rsp]
	call	fprintf
	npad	1
	jmp	SHORT $LN2@papacc_ser
$LN3@papacc_ser:

; 73   :  fprintf(out,"; SERVICE=PapinhoAccelerator; ACCOUNT=NT SERVICE%cPapinhoAccelerator; IMAGE=%lu:%lu:%lu; IMAGE_SHA256=",92,facts.executable[0],facts.executable[1],facts.executable[2]);

	mov	eax, 4
	imul	rax, rax, 2
	mov	ecx, 4
	imul	rcx, rcx, 1
	mov	edx, 4
	imul	rdx, rdx, 0
	mov	eax, DWORD PTR facts$[rsp+rax+24]
	mov	DWORD PTR [rsp+40], eax
	mov	eax, DWORD PTR facts$[rsp+rcx+24]
	mov	DWORD PTR [rsp+32], eax
	mov	r9d, DWORD PTR facts$[rsp+rdx+24]
	mov	r8d, 92					; 0000005cH
	lea	rdx, OFFSET FLAT:$SG111877
	mov	rcx, QWORD PTR out$[rsp]
	call	fprintf
	npad	1

; 74   :  for(i=0;i<32;i++)fprintf(out,"%02x",facts.image_hash[i]);

	mov	DWORD PTR i$[rsp], 0
	jmp	SHORT $LN7@papacc_ser
$LN5@papacc_ser:
	mov	eax, DWORD PTR i$[rsp]
	inc	eax
	mov	DWORD PTR i$[rsp], eax
$LN7@papacc_ser:
	cmp	DWORD PTR i$[rsp], 32			; 00000020H
	jae	SHORT $LN6@papacc_ser
	mov	eax, DWORD PTR i$[rsp]
	movzx	eax, BYTE PTR facts$[rsp+rax+156]
	mov	r8d, eax
	lea	rdx, OFFSET FLAT:$SG111878
	mov	rcx, QWORD PTR out$[rsp]
	call	fprintf
	npad	1
	jmp	SHORT $LN5@papacc_ser
$LN6@papacc_ser:

; 75   :  fputs("; CONFIGURATION_SHA256=",out);for(i=0;i<32;i++)fprintf(out,"%02x",facts.config_hash[i]);

	mov	rdx, QWORD PTR out$[rsp]
	lea	rcx, OFFSET FLAT:$SG111879
	call	QWORD PTR __imp_fputs
	npad	1
	mov	DWORD PTR i$[rsp], 0
	jmp	SHORT $LN10@papacc_ser
$LN8@papacc_ser:
	mov	eax, DWORD PTR i$[rsp]
	inc	eax
	mov	DWORD PTR i$[rsp], eax
$LN10@papacc_ser:
	cmp	DWORD PTR i$[rsp], 32			; 00000020H
	jae	SHORT $LN9@papacc_ser
	mov	eax, DWORD PTR i$[rsp]
	movzx	eax, BYTE PTR facts$[rsp+rax+124]
	mov	r8d, eax
	lea	rdx, OFFSET FLAT:$SG111880
	mov	rcx, QWORD PTR out$[rsp]
	call	fprintf
	npad	1
	jmp	SHORT $LN8@papacc_ser
$LN9@papacc_ser:

; 76   :  fputs("; AUTHORIZATION=NOT_ACQUIRED; CREATE_SERVICE=DENY; SCOPE=LAB_ONLY\n",out);

	mov	rdx, QWORD PTR out$[rsp]
	lea	rcx, OFFSET FLAT:$SG111881
	call	QWORD PTR __imp_fputs
	npad	1
$done$45:

; 77   :  done:
; 78   :  ps_lab_binding_invalidate(&binding);

	lea	rcx, QWORD PTR binding$[rsp]
	call	ps_lab_binding_invalidate

; 79   :  if(papacc_configuration_personal_native_close(c)!=PAPACC_RESULT_OK)clean=0;

	mov	rcx, QWORD PTR c$[rsp]
	call	papacc_configuration_personal_native_close
	test	eax, eax
	je	SHORT $LN32@papacc_ser
	mov	DWORD PTR clean$[rsp], 0
$LN32@papacc_ser:

; 80   :  if(clean){SecureZeroMemory(p->next,sizeof(p->next));SecureZeroMemory(p->observed,sizeof(p->observed));InterlockedExchange(&state,PAPACC_LAUNCHER_FINISHED);}

	cmp	DWORD PTR clean$[rsp], 0
	je	SHORT $LN33@papacc_ser
	mov	rax, QWORD PTR p$[rsp]
	add	rax, 40560				; 00009e70H
	mov	edx, 31840				; 00007c60H
	mov	rcx, rax
	call	RtlSecureZeroMemory
	mov	rax, QWORD PTR p$[rsp]
	add	rax, 72400				; 00011ad0H
	mov	edx, 512				; 00000200H
	mov	rcx, rax
	call	RtlSecureZeroMemory
	mov	eax, 3
	lea	rcx, OFFSET FLAT:state
	xchg	DWORD PTR [rcx], eax
	jmp	SHORT $LN34@papacc_ser
$LN33@papacc_ser:

; 81   :  else InterlockedExchange(&state,PAPACC_LAUNCHER_RETAINED);

	mov	eax, 2
	lea	rcx, OFFSET FLAT:state
	xchg	DWORD PTR [rcx], eax
$LN34@papacc_ser:

; 82   :  fprintf(err,"SCM_LAB_BINDING=%s; STAGE=%s; RESOURCES=%s; NO_SCM_EFFECT; CA_CURRENT_AUTHORITY=DENY\n",prepared?"PREPARED_NOT_AUTHORIZED":"DENY",stage,clean?"CLOSED":"RETAINED_UNKNOWN");

	cmp	DWORD PTR clean$[rsp], 0
	je	SHORT $LN38@papacc_ser
	lea	rax, OFFSET FLAT:$SG111885
	mov	QWORD PTR tv403[rsp], rax
	jmp	SHORT $LN39@papacc_ser
$LN38@papacc_ser:
	lea	rax, OFFSET FLAT:$SG111886
	mov	QWORD PTR tv403[rsp], rax
$LN39@papacc_ser:
	cmp	DWORD PTR prepared$[rsp], 0
	je	SHORT $LN40@papacc_ser
	lea	rax, OFFSET FLAT:$SG111887
	mov	QWORD PTR tv407[rsp], rax
	jmp	SHORT $LN41@papacc_ser
$LN40@papacc_ser:
	lea	rax, OFFSET FLAT:$SG111888
	mov	QWORD PTR tv407[rsp], rax
$LN41@papacc_ser:
	mov	rax, QWORD PTR tv403[rsp]
	mov	QWORD PTR [rsp+32], rax
	mov	r9, QWORD PTR stage$[rsp]
	mov	r8, QWORD PTR tv407[rsp]
	lea	rdx, OFFSET FLAT:$SG111889
	mov	rcx, QWORD PTR err$[rsp]
	call	fprintf

; 83   :  return prepared&&clean?0:2;

	cmp	DWORD PTR prepared$[rsp], 0
	je	SHORT $LN42@papacc_ser
	cmp	DWORD PTR clean$[rsp], 0
	je	SHORT $LN42@papacc_ser
	mov	DWORD PTR tv412[rsp], 0
	jmp	SHORT $LN43@papacc_ser
$LN42@papacc_ser:
	mov	DWORD PTR tv412[rsp], 2
$LN43@papacc_ser:
	mov	eax, DWORD PTR tv412[rsp]
$LN1@papacc_ser:

; 84   : }

	mov	rdi, rax
	mov	rcx, rsp
	lea	rdx, OFFSET FLAT:papacc_server_launcher_scm_prepare$rtcFrameData
	call	_RTC_CheckStackVars
	mov	rax, rdi
	mov	rcx, QWORD PTR __$ArrayPad$[rsp]
	xor	rcx, rsp
	call	__security_check_cookie
	add	rsp, 60848				; 0000edb0H
	pop	rdi
	ret	0
papacc_server_launcher_scm_prepare ENDP

scm_descriptor PROC

; 8    : {

	mov	QWORD PTR [rsp+16], rdx
	mov	QWORD PTR [rsp+8], rcx
	push	rdi
	sub	rsp, 512				; 00000200H
	lea	rdi, QWORD PTR [rsp+32]
	mov	ecx, 120				; 00000078H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	rcx, QWORD PTR [rsp+528]
	mov	rax, QWORD PTR __security_cookie
	xor	rax, rsp
	mov	QWORD PTR __$ArrayPad$[rsp], rax

; 9    :  PACL acl;PSID owner;BOOL present,def;BYTE system[SECURITY_MAX_SID_SIZE],admins[SECURITY_MAX_SID_SIZE];DWORD sn=sizeof(system),an=sizeof(admins),i;

	mov	DWORD PTR sn$[rsp], 68			; 00000044H
	mov	DWORD PTR an$[rsp], 68			; 00000044H

; 10   :  const DWORD write=GENERIC_ALL|GENERIC_WRITE|DELETE|WRITE_DAC|WRITE_OWNER|FILE_WRITE_DATA|FILE_APPEND_DATA|FILE_WRITE_EA|FILE_WRITE_ATTRIBUTES|FILE_DELETE_CHILD;

	mov	DWORD PTR write$[rsp], 1343029590	; 500d0156H

; 11   :  if(!data||!IsValidSecurityDescriptor((PSECURITY_DESCRIPTOR)data)||!GetSecurityDescriptorOwner((PSECURITY_DESCRIPTOR)data,&owner,&def)||!owner||!IsValidSid(owner)||
; 12   :  !CreateWellKnownSid(WinLocalSystemSid,NULL,system,&sn)||!CreateWellKnownSid(WinBuiltinAdministratorsSid,NULL,admins,&an)||
; 13   :  (!EqualSid(owner,(PSID)operator_sid)&&!EqualSid(owner,system)&&!EqualSid(owner,admins))||
; 14   :  !GetSecurityDescriptorDacl((PSECURITY_DESCRIPTOR)data,&present,&acl,&def)||!present||!acl||!IsValidAcl(acl))return 0;

	cmp	QWORD PTR data$[rsp], 0
	je	$LN6@scm_descri
	mov	rcx, QWORD PTR data$[rsp]
	call	QWORD PTR __imp_IsValidSecurityDescriptor
	test	eax, eax
	je	$LN6@scm_descri
	lea	r8, QWORD PTR def$[rsp]
	lea	rdx, QWORD PTR owner$[rsp]
	mov	rcx, QWORD PTR data$[rsp]
	call	QWORD PTR __imp_GetSecurityDescriptorOwner
	test	eax, eax
	je	$LN6@scm_descri
	cmp	QWORD PTR owner$[rsp], 0
	je	$LN6@scm_descri
	mov	rcx, QWORD PTR owner$[rsp]
	call	QWORD PTR __imp_IsValidSid
	test	eax, eax
	je	$LN6@scm_descri
	lea	r9, QWORD PTR sn$[rsp]
	lea	r8, QWORD PTR system$[rsp]
	xor	edx, edx
	mov	ecx, 22
	call	QWORD PTR __imp_CreateWellKnownSid
	test	eax, eax
	je	$LN6@scm_descri
	lea	r9, QWORD PTR an$[rsp]
	lea	r8, QWORD PTR admins$[rsp]
	xor	edx, edx
	mov	ecx, 26
	call	QWORD PTR __imp_CreateWellKnownSid
	test	eax, eax
	je	$LN6@scm_descri
	mov	rdx, QWORD PTR operator_sid$[rsp]
	mov	rcx, QWORD PTR owner$[rsp]
	call	QWORD PTR __imp_EqualSid
	test	eax, eax
	jne	SHORT $LN7@scm_descri
	lea	rdx, QWORD PTR system$[rsp]
	mov	rcx, QWORD PTR owner$[rsp]
	call	QWORD PTR __imp_EqualSid
	test	eax, eax
	jne	SHORT $LN7@scm_descri
	lea	rdx, QWORD PTR admins$[rsp]
	mov	rcx, QWORD PTR owner$[rsp]
	call	QWORD PTR __imp_EqualSid
	test	eax, eax
	je	SHORT $LN6@scm_descri
$LN7@scm_descri:
	lea	r9, QWORD PTR def$[rsp]
	lea	r8, QWORD PTR acl$[rsp]
	lea	rdx, QWORD PTR present$[rsp]
	mov	rcx, QWORD PTR data$[rsp]
	call	QWORD PTR __imp_GetSecurityDescriptorDacl
	test	eax, eax
	je	SHORT $LN6@scm_descri
	cmp	DWORD PTR present$[rsp], 0
	je	SHORT $LN6@scm_descri
	cmp	QWORD PTR acl$[rsp], 0
	je	SHORT $LN6@scm_descri
	mov	rcx, QWORD PTR acl$[rsp]
	call	QWORD PTR __imp_IsValidAcl
	test	eax, eax
	jne	SHORT $LN5@scm_descri
$LN6@scm_descri:
	xor	eax, eax
	jmp	$LN1@scm_descri
$LN5@scm_descri:

; 15   :  for(i=0;i<acl->AceCount;i++){

	mov	DWORD PTR i$[rsp], 0
	jmp	SHORT $LN4@scm_descri
$LN2@scm_descri:
	mov	eax, DWORD PTR i$[rsp]
	inc	eax
	mov	DWORD PTR i$[rsp], eax
$LN4@scm_descri:
	mov	rax, QWORD PTR acl$[rsp]
	movzx	eax, WORD PTR [rax+4]
	cmp	DWORD PTR i$[rsp], eax
	jae	$LN3@scm_descri

; 16   :   ACCESS_ALLOWED_ACE *ace;PACE_HEADER header;PSID sid;
; 17   :   if(!GetAce(acl,i,(void **)&header)||header->AceSize<FIELD_OFFSET(ACCESS_ALLOWED_ACE,SidStart)+8U)return 0;

	lea	r8, QWORD PTR header$13[rsp]
	mov	edx, DWORD PTR i$[rsp]
	mov	rcx, QWORD PTR acl$[rsp]
	call	QWORD PTR __imp_GetAce
	test	eax, eax
	je	SHORT $LN9@scm_descri
	mov	rax, QWORD PTR header$13[rsp]
	movzx	eax, WORD PTR [rax+2]
	cmp	eax, 16
	jae	SHORT $LN8@scm_descri
$LN9@scm_descri:
	xor	eax, eax
	jmp	$LN1@scm_descri
$LN8@scm_descri:

; 18   :   if(header->AceFlags&INHERIT_ONLY_ACE)continue;

	mov	rax, QWORD PTR header$13[rsp]
	movzx	eax, BYTE PTR [rax+1]
	and	eax, 8
	test	eax, eax
	je	SHORT $LN10@scm_descri
	jmp	SHORT $LN2@scm_descri
$LN10@scm_descri:

; 19   :   if(header->AceType!=ACCESS_ALLOWED_ACE_TYPE&&header->AceType!=ACCESS_DENIED_ACE_TYPE)return 0;

	mov	rax, QWORD PTR header$13[rsp]
	movzx	eax, BYTE PTR [rax]
	test	eax, eax
	je	SHORT $LN11@scm_descri
	mov	rax, QWORD PTR header$13[rsp]
	movzx	eax, BYTE PTR [rax]
	cmp	eax, 1
	je	SHORT $LN11@scm_descri
	xor	eax, eax
	jmp	$LN1@scm_descri
$LN11@scm_descri:

; 20   :   ace=(ACCESS_ALLOWED_ACE *)header;sid=(PSID)&ace->SidStart;

	mov	rax, QWORD PTR header$13[rsp]
	mov	QWORD PTR ace$12[rsp], rax
	mov	rax, QWORD PTR ace$12[rsp]
	add	rax, 8
	mov	QWORD PTR sid$14[rsp], rax

; 21   :   if(!IsValidSid(sid)||GetLengthSid(sid)>(DWORD)(header->AceSize-FIELD_OFFSET(ACCESS_ALLOWED_ACE,SidStart)))return 0;

	mov	rcx, QWORD PTR sid$14[rsp]
	call	QWORD PTR __imp_IsValidSid
	test	eax, eax
	je	SHORT $LN13@scm_descri
	mov	rcx, QWORD PTR sid$14[rsp]
	call	QWORD PTR __imp_GetLengthSid
	mov	rcx, QWORD PTR header$13[rsp]
	movzx	ecx, WORD PTR [rcx+2]
	sub	ecx, 8
	cmp	eax, ecx
	jbe	SHORT $LN12@scm_descri
$LN13@scm_descri:
	xor	eax, eax
	jmp	$LN1@scm_descri
$LN12@scm_descri:

; 22   :   if(header->AceType==ACCESS_ALLOWED_ACE_TYPE&&(ace->Mask&write)&&!EqualSid(sid,(PSID)operator_sid)&&!EqualSid(sid,system)&&!EqualSid(sid,admins))return 0;

	mov	rax, QWORD PTR header$13[rsp]
	movzx	eax, BYTE PTR [rax]
	test	eax, eax
	jne	SHORT $LN14@scm_descri
	mov	rax, QWORD PTR ace$12[rsp]
	mov	ecx, DWORD PTR write$[rsp]
	mov	eax, DWORD PTR [rax+4]
	and	eax, ecx
	test	eax, eax
	je	SHORT $LN14@scm_descri
	mov	rdx, QWORD PTR operator_sid$[rsp]
	mov	rcx, QWORD PTR sid$14[rsp]
	call	QWORD PTR __imp_EqualSid
	test	eax, eax
	jne	SHORT $LN14@scm_descri
	lea	rdx, QWORD PTR system$[rsp]
	mov	rcx, QWORD PTR sid$14[rsp]
	call	QWORD PTR __imp_EqualSid
	test	eax, eax
	jne	SHORT $LN14@scm_descri
	lea	rdx, QWORD PTR admins$[rsp]
	mov	rcx, QWORD PTR sid$14[rsp]
	call	QWORD PTR __imp_EqualSid
	test	eax, eax
	jne	SHORT $LN14@scm_descri
	xor	eax, eax
	jmp	SHORT $LN1@scm_descri
$LN14@scm_descri:

; 23   :  }

	jmp	$LN2@scm_descri
$LN3@scm_descri:

; 24   :  return 1;

	mov	eax, 1
$LN1@scm_descri:

; 25   : }

	mov	rdi, rax
	mov	rcx, rsp
	lea	rdx, OFFSET FLAT:scm_descriptor$rtcFrameData
	call	_RTC_CheckStackVars
	mov	rax, rdi
	mov	rcx, QWORD PTR __$ArrayPad$[rsp]
	xor	rcx, rsp
	call	__security_check_cookie
	add	rsp, 512				; 00000200H
	pop	rdi
	ret	0
scm_descriptor ENDP

scm_acl_hash PROC

; 28   : {

	mov	QWORD PTR [rsp+24], r8
	mov	QWORD PTR [rsp+16], rdx
	mov	QWORD PTR [rsp+8], rcx
	push	rdi
	sub	rsp, 80					; 00000050H

; 29   :  int ok;DWORD bytes;
; 30   :  if(n->check_descriptor)return 0;

	mov	rax, QWORD PTR n$[rsp]
	cmp	QWORD PTR [rax+376], 0
	je	SHORT $LN2@scm_acl_ha
	xor	eax, eax
	jmp	$LN1@scm_acl_ha
$LN2@scm_acl_ha:

; 31   :  if(GetSecurityInfo(file,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&n->check_descriptor)!=ERROR_SUCCESS)return 0;

	mov	rax, QWORD PTR n$[rsp]
	add	rax, 376				; 00000178H
	mov	QWORD PTR [rsp+56], rax
	mov	QWORD PTR [rsp+48], 0
	mov	QWORD PTR [rsp+40], 0
	mov	QWORD PTR [rsp+32], 0
	xor	r9d, r9d
	mov	r8d, 5
	mov	edx, 1
	mov	rcx, QWORD PTR file$[rsp]
	call	QWORD PTR __imp_GetSecurityInfo
	test	eax, eax
	je	SHORT $LN3@scm_acl_ha
	xor	eax, eax
	jmp	$LN1@scm_acl_ha
$LN3@scm_acl_ha:

; 32   :  ok=scm_descriptor(n->check_descriptor,n->sid);bytes=GetSecurityDescriptorLength(n->check_descriptor);

	mov	rax, QWORD PTR n$[rsp]
	add	rax, 160				; 000000a0H
	mov	rdx, rax
	mov	rax, QWORD PTR n$[rsp]
	mov	rcx, QWORD PTR [rax+376]
	call	scm_descriptor
	mov	DWORD PTR ok$[rsp], eax
	mov	rax, QWORD PTR n$[rsp]
	mov	rcx, QWORD PTR [rax+376]
	call	QWORD PTR __imp_GetSecurityDescriptorLength
	mov	DWORD PTR bytes$[rsp], eax

; 33   :  if(ok)ok=pacc_storage_hash_callback(NULL,(const unsigned char *)n->check_descriptor,bytes,out);

	cmp	DWORD PTR ok$[rsp], 0
	je	SHORT $LN4@scm_acl_ha
	mov	eax, DWORD PTR bytes$[rsp]
	mov	r9, QWORD PTR out$[rsp]
	mov	r8d, eax
	mov	rax, QWORD PTR n$[rsp]
	mov	rdx, QWORD PTR [rax+376]
	xor	ecx, ecx
	call	pacc_storage_hash_callback
	mov	DWORD PTR ok$[rsp], eax
$LN4@scm_acl_ha:

; 34   :  if(LocalFree(n->check_descriptor)){n->phase=2U;return 0;}n->check_descriptor=NULL;return ok;

	mov	rax, QWORD PTR n$[rsp]
	mov	rcx, QWORD PTR [rax+376]
	call	QWORD PTR __imp_LocalFree
	test	rax, rax
	je	SHORT $LN5@scm_acl_ha
	mov	rax, QWORD PTR n$[rsp]
	mov	DWORD PTR [rax+404], 2
	xor	eax, eax
	jmp	SHORT $LN1@scm_acl_ha
$LN5@scm_acl_ha:
	mov	rax, QWORD PTR n$[rsp]
	mov	QWORD PTR [rax+376], 0
	mov	eax, DWORD PTR ok$[rsp]
$LN1@scm_acl_ha:

; 35   : }

	add	rsp, 80					; 00000050H
	pop	rdi
	ret	0
scm_acl_hash ENDP

scm_id	PROC

; 37   : {id[0]=f->dwVolumeSerialNumber;id[1]=f->nFileIndexHigh;id[2]=f->nFileIndexLow;}

	mov	QWORD PTR [rsp+16], rdx
	mov	QWORD PTR [rsp+8], rcx
	push	rdi
	mov	eax, 4
	imul	rax, rax, 0
	mov	rcx, QWORD PTR id$[rsp]
	mov	rdx, QWORD PTR f$[rsp]
	mov	edx, DWORD PTR [rdx+28]
	mov	DWORD PTR [rcx+rax], edx
	mov	eax, 4
	imul	rax, rax, 1
	mov	rcx, QWORD PTR id$[rsp]
	mov	rdx, QWORD PTR f$[rsp]
	mov	edx, DWORD PTR [rdx+44]
	mov	DWORD PTR [rcx+rax], edx
	mov	eax, 4
	imul	rax, rax, 2
	mov	rcx, QWORD PTR id$[rsp]
	mov	rdx, QWORD PTR f$[rsp]
	mov	edx, DWORD PTR [rdx+48]
	mov	DWORD PTR [rcx+rax], edx
	pop	rdi
	ret	0
scm_id	ENDP