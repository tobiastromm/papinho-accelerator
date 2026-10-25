papacc_server_service_cli PROC

; 62   : {

$LN40:
	mov	QWORD PTR [rsp+32], r9
	mov	QWORD PTR [rsp+24], r8
	mov	QWORD PTR [rsp+16], rdx
	mov	DWORD PTR [rsp+8], ecx
	push	rsi
	push	rdi
	sub	rsp, 728				; 000002d8H
	mov	rax, QWORD PTR __security_cookie
	xor	rax, rsp
	mov	QWORD PTR __$ArrayPad$[rsp], rax

; 63   :  PS_REQUEST q;PAPACC_SERVICE_IMAGE_VIEW view;SERVICE_STATUS_PROCESS s;DWORD e=0,close_error,bytes=0,rights;int parsed;

	mov	DWORD PTR e$[rsp], 0
	mov	DWORD PTR bytes$[rsp], 0

; 64   :  parsed=ps_parse(argc,argv,&q);if(parsed<0)return -1;

	lea	r8, QWORD PTR q$[rsp]
	mov	rdx, QWORD PTR argv$[rsp]
	mov	ecx, DWORD PTR argc$[rsp]
	call	ps_parse
	mov	DWORD PTR parsed$[rsp], eax
	cmp	DWORD PTR parsed$[rsp], 0
	jge	SHORT $LN2@papacc_ser
	mov	eax, -1
	jmp	$LN1@papacc_ser
$LN2@papacc_ser:

; 65   :  if(!out||!err)return 1;

	cmp	QWORD PTR out$[rsp], 0
	je	SHORT $LN4@papacc_ser
	cmp	QWORD PTR err$[rsp], 0
	jne	SHORT $LN3@papacc_ser
$LN4@papacc_ser:
	mov	eax, 1
	jmp	$LN1@papacc_ser
$LN3@papacc_ser:

; 66   :  if(!parsed){fputs("SERVICE=INVALID_COMMAND; expected install | start | stop | status | startup automatic|manual | remove\n",err);return 1;}

	cmp	DWORD PTR parsed$[rsp], 0
	jne	SHORT $LN5@papacc_ser
	mov	rdx, QWORD PTR err$[rsp]
	lea	rcx, OFFSET FLAT:$SG100940
	call	fputs
	mov	eax, 1
	jmp	$LN1@papacc_ser
$LN5@papacc_ser:

; 67   :  if(InterlockedCompareExchange(&cli_active,1,0)!=0){fputs("SERVICE=BUSY_OR_RETAINED; REUSE=DENY\n",err);return 2;}

	mov	eax, 1
	mov	DWORD PTR tv274[rsp], eax
	lea	rcx, OFFSET FLAT:cli_active
	xor	eax, eax
	mov	edx, DWORD PTR tv274[rsp]
	lock cmpxchg DWORD PTR [rcx], edx
	test	eax, eax
	je	SHORT $LN6@papacc_ser
	mov	rdx, QWORD PTR err$[rsp]
	lea	rcx, OFFSET FLAT:$SG100942
	call	fputs
	mov	eax, 2
	jmp	$LN1@papacc_ser
$LN6@papacc_ser:

; 68   :  if(native.retained||native.manager||native.service||native.image||native.install_effect){fputs("SERVICE=RETAINED; REUSE=DENY\n",err);return 2;}

	cmp	DWORD PTR native+24, 0
	jne	SHORT $LN8@papacc_ser
	cmp	QWORD PTR native, 0
	jne	SHORT $LN8@papacc_ser
	cmp	QWORD PTR native+8, 0
	jne	SHORT $LN8@papacc_ser
	cmp	QWORD PTR native+32, 0
	jne	SHORT $LN8@papacc_ser
	cmp	DWORD PTR native+4276, 0
	je	SHORT $LN7@papacc_ser
$LN8@papacc_ser:
	mov	rdx, QWORD PTR err$[rsp]
	lea	rcx, OFFSET FLAT:$SG100945
	call	fputs
	mov	eax, 2
	jmp	$LN1@papacc_ser
$LN7@papacc_ser:

; 69   :  native.thread=GetCurrentThreadId();native.error=0;

	call	QWORD PTR __imp_GetCurrentThreadId
	mov	DWORD PTR native+16, eax
	mov	DWORD PTR native+20, 0

; 70   :  if(q.action==PS_INSTALL){

	cmp	DWORD PTR q$[rsp], 1
	jne	SHORT $LN9@papacc_ser

; 71   :   if(papacc_server_console_win32_stop_requested()){e=ERROR_OPERATION_ABORTED;goto done;}

	call	papacc_server_console_win32_stop_requested
	test	eax, eax
	je	SHORT $LN10@papacc_ser
	mov	DWORD PTR e$[rsp], 995			; 000003e3H
	jmp	$done$41
$LN10@papacc_ser:

; 72   :   if(papacc_server_console_win32_install(&native.console)!=PAPACC_RESULT_OK){e=ERROR_INVALID_STATE;goto done;}

	lea	rcx, OFFSET FLAT:native+28
	call	papacc_server_console_win32_install
	test	eax, eax
	je	SHORT $LN11@papacc_ser
	mov	DWORD PTR e$[rsp], 5023			; 0000139fH
	jmp	$done$41
$LN11@papacc_ser:

; 73   :   e=ps_image_open();if(e)goto done;

	call	ps_image_open
	mov	DWORD PTR e$[rsp], eax
	cmp	DWORD PTR e$[rsp], 0
	je	SHORT $LN12@papacc_ser
	jmp	$done$41
$LN12@papacc_ser:
$LN9@papacc_ser:

; 74   :  }
; 75   :  if(q.prepare_lab){

	cmp	DWORD PTR q$[rsp+4], 0
	je	$LN13@papacc_ser

; 76   :   if(!prepare){e=ERROR_NOT_SUPPORTED;goto done;}

	cmp	QWORD PTR prepare$[rsp], 0
	jne	SHORT $LN14@papacc_ser
	mov	DWORD PTR e$[rsp], 50			; 00000032H
	jmp	$done$41
$LN14@papacc_ser:

; 77   :   view.handle=native.image;view.identity=native.image_id;view.path=native.image_path;view.sha256=native.image_sha256;view.owner_thread=native.thread;

	mov	rax, QWORD PTR native+32
	mov	QWORD PTR view$[rsp], rax
	lea	rax, QWORD PTR view$[rsp+8]
	lea	rcx, OFFSET FLAT:native+40
	mov	rdi, rax
	mov	rsi, rcx
	mov	ecx, 52					; 00000034H
	rep movsb
	lea	rax, OFFSET FLAT:native+96
	mov	QWORD PTR view$[rsp+64], rax
	lea	rax, OFFSET FLAT:native+4244
	mov	QWORD PTR view$[rsp+72], rax
	mov	eax, DWORD PTR native+16
	mov	DWORD PTR view$[rsp+80], eax

; 78   :   e=prepare(q.installation,q.configuration,&view,out,err)?ERROR_ACCESS_DENIED:ERROR_SUCCESS;

	mov	rax, QWORD PTR err$[rsp]
	mov	QWORD PTR [rsp+32], rax
	mov	r9, QWORD PTR out$[rsp]
	lea	r8, QWORD PTR view$[rsp]
	lea	rdx, QWORD PTR q$[rsp+396]
	lea	rcx, QWORD PTR q$[rsp+136]
	call	QWORD PTR prepare$[rsp]
	test	eax, eax
	je	SHORT $LN34@papacc_ser
	mov	DWORD PTR tv138[rsp], 5
	jmp	SHORT $LN35@papacc_ser
$LN34@papacc_ser:
	mov	DWORD PTR tv138[rsp], 0
$LN35@papacc_ser:
	mov	eax, DWORD PTR tv138[rsp]
	mov	DWORD PTR e$[rsp], eax

; 79   :   goto done;

	jmp	$done$41
$LN13@papacc_ser:

; 80   :  }
; 81   :  if(q.action!=PS_STATUS&&!ps_binding_ready()){

	cmp	DWORD PTR q$[rsp], 4
	je	SHORT $LN15@papacc_ser
	call	ps_binding_ready
	test	eax, eax
	jne	SHORT $LN15@papacc_ser

; 82   :   fputs("SERVICE=DENY; INSTALLATION_CONFIG_ACL_TOKEN_BINDING=NOT_QUALIFIED; MUTATION=NOT_RUN; CA_AUTHORITY=DENY\n",err);e=ERROR_ACCESS_DENIED;goto done;

	mov	rdx, QWORD PTR err$[rsp]
	lea	rcx, OFFSET FLAT:$SG100953
	call	fputs
	mov	DWORD PTR e$[rsp], 5
	jmp	$done$41
$LN15@papacc_ser:

; 83   :  }
; 84   :  rights=SERVICE_QUERY_STATUS;

	mov	DWORD PTR rights$[rsp], 4

; 85   :  if(q.action==PS_START)rights|=SERVICE_START;

	cmp	DWORD PTR q$[rsp], 2
	jne	SHORT $LN16@papacc_ser
	mov	eax, DWORD PTR rights$[rsp]
	or	eax, 16
	mov	DWORD PTR rights$[rsp], eax
$LN16@papacc_ser:

; 86   :  if(q.action==PS_STOP)rights|=SERVICE_STOP;

	cmp	DWORD PTR q$[rsp], 3
	jne	SHORT $LN17@papacc_ser
	mov	eax, DWORD PTR rights$[rsp]
	or	eax, 32					; 00000020H
	mov	DWORD PTR rights$[rsp], eax
$LN17@papacc_ser:

; 87   :  if(q.action==PS_REMOVE)rights|=DELETE;

	cmp	DWORD PTR q$[rsp], 7
	jne	SHORT $LN18@papacc_ser
	mov	eax, DWORD PTR rights$[rsp]
	bts	eax, 16
	mov	DWORD PTR rights$[rsp], eax
$LN18@papacc_ser:

; 88   :  if(q.action==PS_AUTOMATIC||q.action==PS_MANUAL)rights|=SERVICE_CHANGE_CONFIG;

	cmp	DWORD PTR q$[rsp], 5
	je	SHORT $LN20@papacc_ser
	cmp	DWORD PTR q$[rsp], 6
	jne	SHORT $LN19@papacc_ser
$LN20@papacc_ser:
	mov	eax, DWORD PTR rights$[rsp]
	or	eax, 2
	mov	DWORD PTR rights$[rsp], eax
$LN19@papacc_ser:

; 89   :  native.manager=OpenSCManagerW(NULL,NULL,SC_MANAGER_CONNECT|(q.action==PS_INSTALL?SC_MANAGER_CREATE_SERVICE:0));

	cmp	DWORD PTR q$[rsp], 1
	jne	SHORT $LN36@papacc_ser
	mov	DWORD PTR tv155[rsp], 2
	jmp	SHORT $LN37@papacc_ser
$LN36@papacc_ser:
	mov	DWORD PTR tv155[rsp], 0
$LN37@papacc_ser:
	mov	eax, DWORD PTR tv155[rsp]
	or	eax, 1
	mov	r8d, eax
	xor	edx, edx
	xor	ecx, ecx
	call	QWORD PTR __imp_OpenSCManagerW
	mov	QWORD PTR native, rax

; 90   :  if(!native.manager){e=ps_last_error();goto done;}

	cmp	QWORD PTR native, 0
	jne	SHORT $LN21@papacc_ser
	call	ps_last_error
	mov	DWORD PTR e$[rsp], eax
	jmp	$done$41
$LN21@papacc_ser:

; 91   :  if(q.action==PS_INSTALL){e=ps_install_virtual();goto done;}

	cmp	DWORD PTR q$[rsp], 1
	jne	SHORT $LN22@papacc_ser
	call	ps_install_virtual
	mov	DWORD PTR e$[rsp], eax
	jmp	$done$41
$LN22@papacc_ser:

; 92   :  native.service=OpenServiceW(native.manager,ps_name,rights);

	mov	r8d, DWORD PTR rights$[rsp]
	lea	rdx, OFFSET FLAT:ps_name
	mov	rcx, QWORD PTR native
	call	QWORD PTR __imp_OpenServiceW
	mov	QWORD PTR native+8, rax

; 93   :  if(!native.service){e=ps_last_error();if(q.action==PS_STATUS&&e==ERROR_SERVICE_DOES_NOT_EXIST){fputs("SERVICE=ABSENT; IDENTITY=NOT_QUALIFIED\n",out);e=0;}goto done;}

	cmp	QWORD PTR native+8, 0
	jne	SHORT $LN23@papacc_ser
	call	ps_last_error
	mov	DWORD PTR e$[rsp], eax
	cmp	DWORD PTR q$[rsp], 4
	jne	SHORT $LN24@papacc_ser
	cmp	DWORD PTR e$[rsp], 1060			; 00000424H
	jne	SHORT $LN24@papacc_ser
	mov	rdx, QWORD PTR out$[rsp]
	lea	rcx, OFFSET FLAT:$SG100963
	call	fputs
	mov	DWORD PTR e$[rsp], 0
$LN24@papacc_ser:
	jmp	$done$41
$LN23@papacc_ser:

; 94   :  memset(&s,0,sizeof(s));

	mov	r8d, 36					; 00000024H
	xor	edx, edx
	lea	rcx, QWORD PTR s$[rsp]
	call	memset

; 95   :  if(!QueryServiceStatusEx(native.service,SC_STATUS_PROCESS_INFO,(LPBYTE)&s,sizeof(s),&bytes)){e=ps_last_error();goto done;}

	lea	rax, QWORD PTR bytes$[rsp]
	mov	QWORD PTR [rsp+32], rax
	mov	r9d, 36					; 00000024H
	lea	r8, QWORD PTR s$[rsp]
	xor	edx, edx
	mov	rcx, QWORD PTR native+8
	call	QWORD PTR __imp_QueryServiceStatusEx
	test	eax, eax
	jne	SHORT $LN25@papacc_ser
	call	ps_last_error
	mov	DWORD PTR e$[rsp], eax
	jmp	$done$41
$LN25@papacc_ser:

; 96   :  if(q.action==PS_STATUS)fprintf(out,"SERVICE=OBSERVED; STATE=%lu; PID=%lu; WIN32_EXIT=%lu; IDENTITY=NOT_QUALIFIED; AUTHORITY=NONE\n",

	cmp	DWORD PTR q$[rsp], 4
	jne	SHORT $LN26@papacc_ser
	mov	eax, DWORD PTR s$[rsp+12]
	mov	DWORD PTR [rsp+32], eax
	mov	r9d, DWORD PTR s$[rsp+28]
	mov	r8d, DWORD PTR s$[rsp+4]
	lea	rdx, OFFSET FLAT:$SG100967
	mov	rcx, QWORD PTR out$[rsp]
	call	fprintf
	npad	1
	jmp	SHORT $LN27@papacc_ser
$LN26@papacc_ser:

; 97   :   (unsigned long)s.dwCurrentState,(unsigned long)s.dwProcessId,(unsigned long)s.dwWin32ExitCode);
; 98   :  else {
; 99   :   e=ps_mutate(&q,&s);

	lea	rdx, QWORD PTR s$[rsp]
	lea	rcx, QWORD PTR q$[rsp]
	call	ps_mutate
	mov	DWORD PTR e$[rsp], eax

; 100  :   if(!e)fputs(q.action==PS_REMOVE?"SERVICE=DELETE_REQUESTED; ABSENCE=NOT_PROVEN\n":"SERVICE=REQUEST_SUBMITTED; COMPLETION=NOT_PROVEN\n",out);

	cmp	DWORD PTR e$[rsp], 0
	jne	SHORT $LN28@papacc_ser
	cmp	DWORD PTR q$[rsp], 7
	jne	SHORT $LN38@papacc_ser
	lea	rax, OFFSET FLAT:$SG100969
	mov	QWORD PTR tv230[rsp], rax
	jmp	SHORT $LN39@papacc_ser
$LN38@papacc_ser:
	lea	rax, OFFSET FLAT:$SG100970
	mov	QWORD PTR tv230[rsp], rax
$LN39@papacc_ser:
	mov	rdx, QWORD PTR out$[rsp]
	mov	rcx, QWORD PTR tv230[rsp]
	call	fputs
	npad	1
$LN28@papacc_ser:
$LN27@papacc_ser:
$done$41:

; 101  :  }
; 102  :  done:
; 103  :  native.error=e;close_error=ps_close();if(!native.error)native.error=close_error;

	mov	eax, DWORD PTR e$[rsp]
	mov	DWORD PTR native+20, eax
	call	ps_close
	mov	DWORD PTR close_error$[rsp], eax
	cmp	DWORD PTR native+20, 0
	jne	SHORT $LN29@papacc_ser
	mov	eax, DWORD PTR close_error$[rsp]
	mov	DWORD PTR native+20, eax
$LN29@papacc_ser:

; 104  :  if(!native.retained&&!native.install_effect)InterlockedExchange(&cli_active,0);

	cmp	DWORD PTR native+24, 0
	jne	SHORT $LN30@papacc_ser
	cmp	DWORD PTR native+4276, 0
	jne	SHORT $LN30@papacc_ser
	xor	eax, eax
	lea	rcx, OFFSET FLAT:cli_active
	xchg	DWORD PTR [rcx], eax
$LN30@papacc_ser:

; 105  :  if(e||close_error){fprintf(err,"SERVICE=ERROR; WIN32=%lu; CLOSE=%lu; RETAINED=%u; NO_AUTOMATIC_RETRY\n",(unsigned long)e,(unsigned long)close_error,native.retained);return 2;}

	cmp	DWORD PTR e$[rsp], 0
	jne	SHORT $LN32@papacc_ser
	cmp	DWORD PTR close_error$[rsp], 0
	je	SHORT $LN31@papacc_ser
$LN32@papacc_ser:
	mov	eax, DWORD PTR native+24
	mov	DWORD PTR [rsp+32], eax
	mov	r9d, DWORD PTR close_error$[rsp]
	mov	r8d, DWORD PTR e$[rsp]
	lea	rdx, OFFSET FLAT:$SG100975
	mov	rcx, QWORD PTR err$[rsp]
	call	fprintf
	mov	eax, 2
	jmp	SHORT $LN1@papacc_ser
$LN31@papacc_ser:

; 106  :  return 0;

	xor	eax, eax
$LN1@papacc_ser:

; 107  : }

	mov	rcx, QWORD PTR __$ArrayPad$[rsp]
	xor	rcx, rsp
	call	__security_check_cookie
	add	rsp, 728				; 000002d8H
	pop	rdi
	pop	rsi
	ret	0
papacc_server_service_cli ENDP

papacc_server_service_host_active PROC

; 130  : {return host.handle&&host.run_active&&host.thread==GetCurrentThreadId();}

$LN5:
	sub	rsp, 56					; 00000038H
	cmp	QWORD PTR host, 0
	je	SHORT $LN3@papacc_ser
	cmp	DWORD PTR host+56, 0
	je	SHORT $LN3@papacc_ser
	call	QWORD PTR __imp_GetCurrentThreadId
	cmp	DWORD PTR host+52, eax
	jne	SHORT $LN3@papacc_ser
	mov	DWORD PTR tv68[rsp], 1
	jmp	SHORT $LN4@papacc_ser
$LN3@papacc_ser:
	mov	DWORD PTR tv68[rsp], 0
$LN4@papacc_ser:
	mov	eax, DWORD PTR tv68[rsp]
	add	rsp, 56					; 00000038H
	ret	0
papacc_server_service_host_active ENDP

papacc_server_service_runtime_ready PROC

; 132  : {

$LN11:
	sub	rsp, 56					; 00000038H

; 133  :  if(!host.handle)return 1; /* direct console runtime, no SCM requirement */

	cmp	QWORD PTR host, 0
	jne	SHORT $LN2@papacc_ser
	mov	eax, 1
	jmp	$LN1@papacc_ser
$LN2@papacc_ser:

; 134  :  if(!host.run_active||host.thread!=GetCurrentThreadId()||host.terminal_receipt||host.life.phase!=PS_START_PENDING)return 0;

	cmp	DWORD PTR host+56, 0
	je	SHORT $LN4@papacc_ser
	call	QWORD PTR __imp_GetCurrentThreadId
	cmp	DWORD PTR host+52, eax
	jne	SHORT $LN4@papacc_ser
	cmp	DWORD PTR host+60, 0
	jne	SHORT $LN4@papacc_ser
	cmp	DWORD PTR host+36, 1
	je	SHORT $LN3@papacc_ser
$LN4@papacc_ser:
	xor	eax, eax
	jmp	$LN1@papacc_ser
$LN3@papacc_ser:

; 135  :  if(InterlockedCompareExchange(&host.stop,0,0)||papacc_server_console_win32_stop_requested()){

	xor	eax, eax
	mov	DWORD PTR tv137[rsp], eax
	lea	rcx, OFFSET FLAT:host+48
	xor	eax, eax
	mov	edx, DWORD PTR tv137[rsp]
	lock cmpxchg DWORD PTR [rcx], edx
	test	eax, eax
	jne	SHORT $LN6@papacc_ser
	call	papacc_server_console_win32_stop_requested
	test	eax, eax
	je	SHORT $LN5@papacc_ser
$LN6@papacc_ser:

; 136  :   (void)ps_stop(&host.life);(void)ps_report(SERVICE_STOP_PENDING,ERROR_SUCCESS);return 0;

	lea	rcx, OFFSET FLAT:host+36
	call	ps_stop
	xor	edx, edx
	mov	ecx, 3
	call	ps_report
	xor	eax, eax
	jmp	SHORT $LN1@papacc_ser
$LN5@papacc_ser:

; 137  :  }
; 138  :  if(!ps_running(&host.life)||!ps_report(SERVICE_RUNNING,ERROR_SUCCESS))return 0;

	lea	rcx, OFFSET FLAT:host+36
	call	ps_running
	test	eax, eax
	je	SHORT $LN8@papacc_ser
	xor	edx, edx
	mov	ecx, 4
	call	ps_report
	test	eax, eax
	jne	SHORT $LN7@papacc_ser
$LN8@papacc_ser:
	xor	eax, eax
	jmp	SHORT $LN1@papacc_ser
$LN7@papacc_ser:

; 139  :  if(InterlockedCompareExchange(&host.stop,0,0)){(void)ps_stop(&host.life);(void)ps_report(SERVICE_STOP_PENDING,ERROR_SUCCESS);return 0;}

	xor	eax, eax
	mov	DWORD PTR tv92[rsp], eax
	lea	rcx, OFFSET FLAT:host+48
	xor	eax, eax
	mov	edx, DWORD PTR tv92[rsp]
	lock cmpxchg DWORD PTR [rcx], edx
	test	eax, eax
	je	SHORT $LN9@papacc_ser
	lea	rcx, OFFSET FLAT:host+36
	call	ps_stop
	xor	edx, edx
	mov	ecx, 3
	call	ps_report
	xor	eax, eax
	jmp	SHORT $LN1@papacc_ser
$LN9@papacc_ser:

; 140  :  return 1;

	mov	eax, 1
$LN1@papacc_ser:

; 141  : }

	add	rsp, 56					; 00000038H
	ret	0
papacc_server_service_runtime_ready ENDP

papacc_server_service_runtime_stopping PROC

; 143  : {

$LN8:
	sub	rsp, 40					; 00000028H

; 144  :  if(!host.handle)return 1;

	cmp	QWORD PTR host, 0
	jne	SHORT $LN2@papacc_ser
	mov	eax, 1
	jmp	SHORT $LN1@papacc_ser
$LN2@papacc_ser:

; 145  :  if(!host.run_active||host.thread!=GetCurrentThreadId()||host.terminal_receipt)return 0;

	cmp	DWORD PTR host+56, 0
	je	SHORT $LN4@papacc_ser
	call	QWORD PTR __imp_GetCurrentThreadId
	cmp	DWORD PTR host+52, eax
	jne	SHORT $LN4@papacc_ser
	cmp	DWORD PTR host+60, 0
	je	SHORT $LN3@papacc_ser
$LN4@papacc_ser:
	xor	eax, eax
	jmp	SHORT $LN1@papacc_ser
$LN3@papacc_ser:

; 146  :  if(host.life.phase==PS_STOP_PENDING)return 1;

	cmp	DWORD PTR host+36, 3
	jne	SHORT $LN5@papacc_ser
	mov	eax, 1
	jmp	SHORT $LN1@papacc_ser
$LN5@papacc_ser:

; 147  :  if(!ps_stop(&host.life))return 0;

	lea	rcx, OFFSET FLAT:host+36
	call	ps_stop
	test	eax, eax
	jne	SHORT $LN6@papacc_ser
	xor	eax, eax
	jmp	SHORT $LN1@papacc_ser
$LN6@papacc_ser:

; 148  :  return ps_report(SERVICE_STOP_PENDING,ERROR_SUCCESS);

	xor	edx, edx
	mov	ecx, 3
	call	ps_report
$LN1@papacc_ser:

; 149  : }

	add	rsp, 40					; 00000028H
	ret	0
papacc_server_service_runtime_stopping ENDP

papacc_server_service_runtime_closed PROC

; 151  : {

$LN6:
	sub	rsp, 40					; 00000028H

; 152  :  if(!host.handle||!host.run_active||host.thread!=GetCurrentThreadId()||host.terminal_receipt)return;

	cmp	QWORD PTR host, 0
	je	SHORT $LN3@papacc_ser
	cmp	DWORD PTR host+56, 0
	je	SHORT $LN3@papacc_ser
	call	QWORD PTR __imp_GetCurrentThreadId
	cmp	DWORD PTR host+52, eax
	jne	SHORT $LN3@papacc_ser
	cmp	DWORD PTR host+60, 0
	je	SHORT $LN2@papacc_ser
$LN3@papacc_ser:
	jmp	SHORT $LN1@papacc_ser
$LN2@papacc_ser:

; 153  :  if(ps_finish(&host.life,1,0))host.terminal_receipt=1U;

	xor	r8d, r8d
	mov	edx, 1
	lea	rcx, OFFSET FLAT:host+36
	call	ps_finish
	test	eax, eax
	je	SHORT $LN4@papacc_ser
	mov	DWORD PTR host+60, 1
$LN4@papacc_ser:
$LN1@papacc_ser:

; 154  : }

	add	rsp, 40					; 00000028H
	ret	0
papacc_server_service_runtime_closed ENDP

papacc_server_service_runtime_retained PROC

; 156  : {

$LN5:
	sub	rsp, 40					; 00000028H

; 157  :  if(!host.handle||!host.run_active||host.thread!=GetCurrentThreadId()||host.terminal_receipt)return;

	cmp	QWORD PTR host, 0
	je	SHORT $LN3@papacc_ser
	cmp	DWORD PTR host+56, 0
	je	SHORT $LN3@papacc_ser
	call	QWORD PTR __imp_GetCurrentThreadId
	cmp	DWORD PTR host+52, eax
	jne	SHORT $LN3@papacc_ser
	cmp	DWORD PTR host+60, 0
	je	SHORT $LN2@papacc_ser
$LN3@papacc_ser:
	jmp	SHORT $LN1@papacc_ser
$LN2@papacc_ser:

; 158  :  (void)ps_finish(&host.life,0,1);host.terminal_receipt=2U;

	mov	r8d, 1
	xor	edx, edx
	lea	rcx, OFFSET FLAT:host+36
	call	ps_finish
	mov	DWORD PTR host+60, 2
$LN1@papacc_ser:

; 159  : }

	add	rsp, 40					; 00000028H
	ret	0
papacc_server_service_runtime_retained ENDP

papacc_server_service_dispatch PROC

; 192  : {

$LN8:
	mov	QWORD PTR [rsp+24], r8
	mov	QWORD PTR [rsp+16], rdx
	mov	DWORD PTR [rsp+8], ecx
	sub	rsp, 72					; 00000048H

; 193  :  SERVICE_TABLE_ENTRYW table[2];
; 194  :  if(argc<3||!argv||!argv[1]||!argv[2]||strcmp(argv[1],"service")||strcmp(argv[2],"host"))return -1;

	cmp	DWORD PTR argc$[rsp], 3
	jl	SHORT $LN3@papacc_ser
	cmp	QWORD PTR argv$[rsp], 0
	je	SHORT $LN3@papacc_ser
	mov	eax, 8
	imul	rax, rax, 1
	mov	rcx, QWORD PTR argv$[rsp]
	cmp	QWORD PTR [rcx+rax], 0
	je	SHORT $LN3@papacc_ser
	mov	eax, 8
	imul	rax, rax, 2
	mov	rcx, QWORD PTR argv$[rsp]
	cmp	QWORD PTR [rcx+rax], 0
	je	SHORT $LN3@papacc_ser
	mov	eax, 8
	imul	rax, rax, 1
	lea	rdx, OFFSET FLAT:$SG101058
	mov	rcx, QWORD PTR argv$[rsp]
	mov	rcx, QWORD PTR [rcx+rax]
	call	strcmp
	test	eax, eax
	jne	SHORT $LN3@papacc_ser
	mov	eax, 8
	imul	rax, rax, 2
	lea	rdx, OFFSET FLAT:$SG101059
	mov	rcx, QWORD PTR argv$[rsp]
	mov	rcx, QWORD PTR [rcx+rax]
	call	strcmp
	test	eax, eax
	je	SHORT $LN2@papacc_ser
$LN3@papacc_ser:
	mov	eax, -1
	jmp	$LN1@papacc_ser
$LN2@papacc_ser:

; 195  :  if(argc!=3||!run||host.handle||host.life.phase)return 1;

	cmp	DWORD PTR argc$[rsp], 3
	jne	SHORT $LN5@papacc_ser
	cmp	QWORD PTR run$[rsp], 0
	je	SHORT $LN5@papacc_ser
	cmp	QWORD PTR host, 0
	jne	SHORT $LN5@papacc_ser
	cmp	DWORD PTR host+36, 0
	je	SHORT $LN4@papacc_ser
$LN5@papacc_ser:
	mov	eax, 1
	jmp	$LN1@papacc_ser
$LN4@papacc_ser:

; 196  :  host.argc=1;host.argv=argv;host.run=run;host.result=2;

	mov	DWORD PTR host+64, 1
	mov	rax, QWORD PTR argv$[rsp]
	mov	QWORD PTR host+72, rax
	mov	rax, QWORD PTR run$[rsp]
	mov	QWORD PTR host+80, rax
	mov	DWORD PTR host+88, 2

; 197  :  table[0].lpServiceName=(LPWSTR)ps_name;table[0].lpServiceProc=ps_main;

	mov	eax, 16
	imul	rax, rax, 0
	lea	rcx, OFFSET FLAT:ps_name
	mov	QWORD PTR table$[rsp+rax], rcx
	mov	eax, 16
	imul	rax, rax, 0
	lea	rcx, OFFSET FLAT:ps_main
	mov	QWORD PTR table$[rsp+rax+8], rcx

; 198  :  table[1].lpServiceName=NULL;table[1].lpServiceProc=NULL;

	mov	eax, 16
	imul	rax, rax, 1
	mov	QWORD PTR table$[rsp+rax], 0
	mov	eax, 16
	imul	rax, rax, 1
	mov	QWORD PTR table$[rsp+rax+8], 0

; 199  :  if(!StartServiceCtrlDispatcherW(table))return 2; /* no console fallback */

	lea	rcx, QWORD PTR table$[rsp]
	call	QWORD PTR __imp_StartServiceCtrlDispatcherW
	test	eax, eax
	jne	SHORT $LN6@papacc_ser
	mov	eax, 2
	jmp	SHORT $LN1@papacc_ser
$LN6@papacc_ser:

; 200  :  return host.result;

	mov	eax, DWORD PTR host+88
$LN1@papacc_ser:

; 201  : }

	add	rsp, 72					; 00000048H
	ret	0
papacc_server_service_dispatch ENDP

papacc_server_service_storage_bytes PROC

; 27   : size_t papacc_server_service_storage_bytes(void){return sizeof(native)+sizeof(host);}

	mov	eax, 12912				; 00003270H
	ret	0
papacc_server_service_storage_bytes ENDP

ps_image_open PROC

; 11   : {

	sub	rsp, 1368				; 00000558H
	mov	rax, QWORD PTR __security_cookie
	xor	rax, rsp
	mov	QWORD PTR __$ArrayPad$[rsp], rax

; 12   :  DWORD n,read,e=ERROR_INVALID_DATA;BY_HANDLE_FILE_INFORMATION after;PACC_STORAGE_SHA256 hash;

	mov	DWORD PTR e$[rsp], 13

; 13   :  unsigned char block[1024],digest[32];size_t i;HANDLE f;
; 14   :  if(native.image||native.image_ready)return ERROR_INVALID_STATE;

	cmp	QWORD PTR native+32, 0
	jne	SHORT $LN9@ps_image_o
	cmp	DWORD PTR native+92, 0
	je	SHORT $LN8@ps_image_o
$LN9@ps_image_o:
	mov	eax, 5023				; 0000139fH
	jmp	$LN1@ps_image_o
$LN8@ps_image_o:

; 15   :  n=GetModuleFileNameW(NULL,native.image_path,1024);

	mov	r8d, 1024				; 00000400H
	lea	rdx, OFFSET FLAT:native+96
	xor	ecx, ecx
	call	QWORD PTR __imp_GetModuleFileNameW
	mov	DWORD PTR n$[rsp], eax

; 16   :  if(!n)return ps_last_error();if(n>=1024)return ERROR_INSUFFICIENT_BUFFER;

	cmp	DWORD PTR n$[rsp], 0
	jne	SHORT $LN10@ps_image_o
	call	ps_last_error
	jmp	$LN1@ps_image_o
$LN10@ps_image_o:
	cmp	DWORD PTR n$[rsp], 1024			; 00000400H
	jb	SHORT $LN11@ps_image_o
	mov	eax, 122				; 0000007aH
	jmp	$LN1@ps_image_o
$LN11@ps_image_o:

; 17   :  if(!((native.image_path[0]>=L'A'&&native.image_path[0]<=L'Z')||(native.image_path[0]>=L'a'&&native.image_path[0]<=L'z'))||
; 18   :  native.image_path[1]!=L':'||native.image_path[2]!=L'\\')return ERROR_INVALID_NAME;

	mov	eax, 2
	imul	rax, rax, 0
	lea	rcx, OFFSET FLAT:native+96
	movzx	eax, WORD PTR [rcx+rax]
	cmp	eax, 65					; 00000041H
	jl	SHORT $LN15@ps_image_o
	mov	eax, 2
	imul	rax, rax, 0
	lea	rcx, OFFSET FLAT:native+96
	movzx	eax, WORD PTR [rcx+rax]
	cmp	eax, 90					; 0000005aH
	jle	SHORT $LN14@ps_image_o
$LN15@ps_image_o:
	mov	eax, 2
	imul	rax, rax, 0
	lea	rcx, OFFSET FLAT:native+96
	movzx	eax, WORD PTR [rcx+rax]
	cmp	eax, 97					; 00000061H
	jl	SHORT $LN13@ps_image_o
	mov	eax, 2
	imul	rax, rax, 0
	lea	rcx, OFFSET FLAT:native+96
	movzx	eax, WORD PTR [rcx+rax]
	cmp	eax, 122				; 0000007aH
	jg	SHORT $LN13@ps_image_o
$LN14@ps_image_o:
	mov	eax, 2
	imul	rax, rax, 1
	lea	rcx, OFFSET FLAT:native+96
	movzx	eax, WORD PTR [rcx+rax]
	cmp	eax, 58					; 0000003aH
	jne	SHORT $LN13@ps_image_o
	mov	eax, 2
	imul	rax, rax, 2
	lea	rcx, OFFSET FLAT:native+96
	movzx	eax, WORD PTR [rcx+rax]
	cmp	eax, 92					; 0000005cH
	je	SHORT $LN12@ps_image_o
$LN13@ps_image_o:
	mov	eax, 123				; 0000007bH
	jmp	$LN1@ps_image_o
$LN12@ps_image_o:

; 19   :  for(i=0;i<n;i++)if(native.image_path[i]==L'"'||native.image_path[i]<L' ')return ERROR_INVALID_NAME;

	mov	QWORD PTR i$[rsp], 0
	jmp	SHORT $LN4@ps_image_o
$LN2@ps_image_o:
	mov	rax, QWORD PTR i$[rsp]
	inc	rax
	mov	QWORD PTR i$[rsp], rax
$LN4@ps_image_o:
	mov	eax, DWORD PTR n$[rsp]
	cmp	QWORD PTR i$[rsp], rax
	jae	SHORT $LN3@ps_image_o
	lea	rax, OFFSET FLAT:native+96
	mov	rcx, QWORD PTR i$[rsp]
	movzx	eax, WORD PTR [rax+rcx*2]
	cmp	eax, 34					; 00000022H
	je	SHORT $LN17@ps_image_o
	lea	rax, OFFSET FLAT:native+96
	mov	rcx, QWORD PTR i$[rsp]
	movzx	eax, WORD PTR [rax+rcx*2]
	cmp	eax, 32					; 00000020H
	jge	SHORT $LN16@ps_image_o
$LN17@ps_image_o:
	mov	eax, 123				; 0000007bH
	jmp	$LN1@ps_image_o
$LN16@ps_image_o:
	jmp	SHORT $LN2@ps_image_o
$LN3@ps_image_o:

; 20   :  f=CreateFileW(native.image_path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);

	mov	QWORD PTR [rsp+48], 0
	mov	DWORD PTR [rsp+40], 2097152		; 00200000H
	mov	DWORD PTR [rsp+32], 3
	xor	r9d, r9d
	mov	r8d, 1
	mov	edx, -2147483648			; 80000000H
	lea	rcx, OFFSET FLAT:native+96
	call	QWORD PTR __imp_CreateFileW
	mov	QWORD PTR f$[rsp], rax

; 21   :  if(f==INVALID_HANDLE_VALUE)return ps_last_error();native.image=f;

	cmp	QWORD PTR f$[rsp], -1
	jne	SHORT $LN18@ps_image_o
	call	ps_last_error
	jmp	$LN1@ps_image_o
$LN18@ps_image_o:
	mov	rax, QWORD PTR f$[rsp]
	mov	QWORD PTR native+32, rax

; 22   :  if(!GetFileInformationByHandle(f,&native.image_id))return ps_last_error();

	lea	rdx, OFFSET FLAT:native+40
	mov	rcx, QWORD PTR f$[rsp]
	call	QWORD PTR __imp_GetFileInformationByHandle
	test	eax, eax
	jne	SHORT $LN19@ps_image_o
	call	ps_last_error
	jmp	$LN1@ps_image_o
$LN19@ps_image_o:

; 23   :  if(native.image_id.dwFileAttributes&(FILE_ATTRIBUTE_REPARSE_POINT|FILE_ATTRIBUTE_DIRECTORY))return ERROR_ACCESS_DENIED;

	mov	eax, DWORD PTR native+40
	and	eax, 1040				; 00000410H
	test	eax, eax
	je	SHORT $LN20@ps_image_o
	mov	eax, 5
	jmp	$LN1@ps_image_o
$LN20@ps_image_o:

; 24   :  memset(&hash,0,sizeof(hash));if(pacc_storage_sha_init(&hash)!=PACC_HASH_OK)return ERROR_INVALID_DATA;

	mov	r8d, 128				; 00000080H
	xor	edx, edx
	lea	rcx, QWORD PTR hash$[rsp]
	call	memset
	lea	rcx, QWORD PTR hash$[rsp]
	call	pacc_storage_sha_init
	test	eax, eax
	je	SHORT $LN21@ps_image_o
	mov	eax, 13
	jmp	$LN1@ps_image_o
$LN21@ps_image_o:
$LN5@ps_image_o:

; 25   :  for(;;){
; 26   :   if(papacc_server_console_win32_stop_requested()) {e=ERROR_OPERATION_ABORTED;goto done;}

	call	papacc_server_console_win32_stop_requested
	test	eax, eax
	je	SHORT $LN22@ps_image_o
	mov	DWORD PTR e$[rsp], 995			; 000003e3H
	jmp	$done$31
$LN22@ps_image_o:

; 27   :   if(!ReadFile(f,block,sizeof(block),&read,NULL)){e=ps_last_error();goto done;}

	mov	QWORD PTR [rsp+32], 0
	lea	r9, QWORD PTR read$[rsp]
	mov	r8d, 1024				; 00000400H
	lea	rdx, QWORD PTR block$[rsp]
	mov	rcx, QWORD PTR f$[rsp]
	call	QWORD PTR __imp_ReadFile
	test	eax, eax
	jne	SHORT $LN23@ps_image_o
	call	ps_last_error
	mov	DWORD PTR e$[rsp], eax
	jmp	$done$31
$LN23@ps_image_o:

; 28   :   if(!read)break;

	cmp	DWORD PTR read$[rsp], 0
	jne	SHORT $LN24@ps_image_o
	jmp	SHORT $LN6@ps_image_o
$LN24@ps_image_o:

; 29   :   if(pacc_storage_sha_update(&hash,block,read,NULL,NULL)!=PACC_HASH_OK)goto done;

	mov	eax, DWORD PTR read$[rsp]
	mov	QWORD PTR [rsp+32], 0
	xor	r9d, r9d
	mov	r8d, eax
	lea	rdx, QWORD PTR block$[rsp]
	lea	rcx, QWORD PTR hash$[rsp]
	call	pacc_storage_sha_update
	test	eax, eax
	je	SHORT $LN25@ps_image_o
	jmp	$done$31
$LN25@ps_image_o:

; 30   :  }

	jmp	$LN5@ps_image_o
$LN6@ps_image_o:

; 31   :  if(pacc_storage_sha_final(&hash,digest,sizeof(digest),NULL,NULL)!=PACC_HASH_OK)goto done;

	mov	QWORD PTR [rsp+32], 0
	xor	r9d, r9d
	mov	r8d, 32					; 00000020H
	lea	rdx, QWORD PTR digest$[rsp]
	lea	rcx, QWORD PTR hash$[rsp]
	call	pacc_storage_sha_final
	test	eax, eax
	je	SHORT $LN26@ps_image_o
	jmp	$done$31
$LN26@ps_image_o:

; 32   :  if(!GetFileInformationByHandle(f,&after)){e=ps_last_error();goto done;}

	lea	rdx, QWORD PTR after$[rsp]
	mov	rcx, QWORD PTR f$[rsp]
	call	QWORD PTR __imp_GetFileInformationByHandle
	test	eax, eax
	jne	SHORT $LN27@ps_image_o
	call	ps_last_error
	mov	DWORD PTR e$[rsp], eax
	jmp	$done$31
$LN27@ps_image_o:

; 33   :  if(!ps_image_same(&native.image_id,&after)||papacc_server_console_win32_stop_requested())goto done;

	lea	rdx, QWORD PTR after$[rsp]
	lea	rcx, OFFSET FLAT:native+40
	call	ps_image_same
	test	eax, eax
	je	SHORT $LN29@ps_image_o
	call	papacc_server_console_win32_stop_requested
	test	eax, eax
	je	SHORT $LN28@ps_image_o
$LN29@ps_image_o:
	jmp	$done$31
$LN28@ps_image_o:

; 34   :  native.command[0]=L'"';memcpy(native.command+1,native.image_path,(n+1U)*sizeof(wchar_t));

	mov	eax, 2
	imul	rax, rax, 0
	lea	rcx, OFFSET FLAT:native+2144
	mov	edx, 34					; 00000022H
	mov	WORD PTR [rcx+rax], dx
	mov	eax, DWORD PTR n$[rsp]
	inc	eax
	mov	eax, eax
	shl	rax, 1
	lea	rcx, OFFSET FLAT:native+2144
	add	rcx, 2
	mov	r8, rax
	lea	rdx, OFFSET FLAT:native+96
	call	memcpy

; 35   :  /* Fixed suffix only; no argv interpolation. 1024 path bound leaves room. */
; 36   :  memcpy(native.command+1+n,L"\" service host",sizeof(L"\" service host"));

	lea	rax, OFFSET FLAT:native+2144
	mov	ecx, DWORD PTR n$[rsp]
	lea	rax, QWORD PTR [rax+rcx*2+2]
	mov	r8d, 30
	lea	rdx, OFFSET FLAT:$SG100804
	mov	rcx, rax
	call	memcpy

; 37   :  memcpy(native.image_sha256,digest,32);native.image_ready=1U;e=ERROR_SUCCESS;

	mov	r8d, 32					; 00000020H
	lea	rdx, QWORD PTR digest$[rsp]
	lea	rcx, OFFSET FLAT:native+4244
	call	memcpy
	mov	DWORD PTR native+92, 1
	mov	DWORD PTR e$[rsp], 0
$done$31:

; 38   :  done:memset(&hash,0,sizeof(hash));memset(block,0,sizeof(block));memset(digest,0,sizeof(digest));return e;

	mov	r8d, 128				; 00000080H
	xor	edx, edx
	lea	rcx, QWORD PTR hash$[rsp]
	call	memset
	mov	r8d, 1024				; 00000400H
	xor	edx, edx
	lea	rcx, QWORD PTR block$[rsp]
	call	memset
	mov	r8d, 32					; 00000020H
	xor	edx, edx
	lea	rcx, QWORD PTR digest$[rsp]
	call	memset
	mov	eax, DWORD PTR e$[rsp]
$LN1@ps_image_o:

; 39   : }

	mov	rcx, QWORD PTR __$ArrayPad$[rsp]
	xor	rcx, rsp
	call	__security_check_cookie
	add	rsp, 1368				; 00000558H
	ret	0
ps_image_open ENDP

ps_install_virtual PROC

; 72   : {

	sub	rsp, 200				; 000000c8H
	mov	rax, QWORD PTR __security_cookie
	xor	rax, rsp
	mov	QWORD PTR __$ArrayPad$[rsp], rax

; 73   :  SERVICE_SID_INFO info;DWORD e;BY_HANDLE_FILE_INFORMATION current;
; 74   :  /* Caller must first prove configuration/target/authorization via existing
; 75   :   * gate. This private primitive is independently tested with modeled APIs. */
; 76   :  if(!native.manager||native.service||!native.image||!native.image_ready||native.install_effect)return ERROR_INVALID_STATE;

	cmp	QWORD PTR native, 0
	je	SHORT $LN3@ps_install
	cmp	QWORD PTR native+8, 0
	jne	SHORT $LN3@ps_install
	cmp	QWORD PTR native+32, 0
	je	SHORT $LN3@ps_install
	cmp	DWORD PTR native+92, 0
	je	SHORT $LN3@ps_install
	cmp	DWORD PTR native+4276, 0
	je	SHORT $LN2@ps_install
$LN3@ps_install:
	mov	eax, 5023				; 0000139fH
	jmp	$LN1@ps_install
$LN2@ps_install:

; 77   :  if(papacc_server_console_win32_stop_requested())return ERROR_OPERATION_ABORTED;

	call	papacc_server_console_win32_stop_requested
	test	eax, eax
	je	SHORT $LN4@ps_install
	mov	eax, 995				; 000003e3H
	jmp	$LN1@ps_install
$LN4@ps_install:

; 78   :  if(!GetFileInformationByHandle(native.image,&current))return ps_last_error();

	lea	rdx, QWORD PTR current$[rsp]
	mov	rcx, QWORD PTR native+32
	call	QWORD PTR __imp_GetFileInformationByHandle
	test	eax, eax
	jne	SHORT $LN5@ps_install
	call	ps_last_error
	jmp	$LN1@ps_install
$LN5@ps_install:

; 79   :  if(!ps_image_same(&native.image_id,&current))return ERROR_INVALID_DATA;

	lea	rdx, QWORD PTR current$[rsp]
	lea	rcx, OFFSET FLAT:native+40
	call	ps_image_same
	test	eax, eax
	jne	SHORT $LN6@ps_install
	mov	eax, 13
	jmp	$LN1@ps_install
$LN6@ps_install:

; 80   :  native.install_effect=1U; /* REQUESTED: an uncertain Create is never retried. */

	mov	DWORD PTR native+4276, 1

; 81   :  native.service=CreateServiceW(native.manager,ps_name,ps_name,SERVICE_QUERY_CONFIG|SERVICE_CHANGE_CONFIG|SERVICE_QUERY_STATUS,

	mov	QWORD PTR [rsp+96], 0
	lea	rax, OFFSET FLAT:ps_account
	mov	QWORD PTR [rsp+88], rax
	mov	QWORD PTR [rsp+80], 0
	mov	QWORD PTR [rsp+72], 0
	mov	QWORD PTR [rsp+64], 0
	lea	rax, OFFSET FLAT:native+2144
	mov	QWORD PTR [rsp+56], rax
	mov	DWORD PTR [rsp+48], 1
	mov	DWORD PTR [rsp+40], 3
	mov	DWORD PTR [rsp+32], 16
	mov	r9d, 7
	lea	r8, OFFSET FLAT:ps_name
	lea	rdx, OFFSET FLAT:ps_name
	mov	rcx, QWORD PTR native
	call	QWORD PTR __imp_CreateServiceW
	mov	QWORD PTR native+8, rax

; 82   :  SERVICE_WIN32_OWN_PROCESS,SERVICE_DEMAND_START,SERVICE_ERROR_NORMAL,native.command,NULL,NULL,NULL,ps_account,NULL);
; 83   :  if(!native.service){e=ps_last_error();native.install_disposition=e==ERROR_SERVICE_EXISTS||e==ERROR_DUPLICATE_SERVICE_NAME||e==ERROR_SERVICE_MARKED_FOR_DELETE?1U:2U;return e;}

	cmp	QWORD PTR native+8, 0
	jne	SHORT $LN7@ps_install
	call	ps_last_error
	mov	DWORD PTR e$[rsp], eax
	cmp	DWORD PTR e$[rsp], 1073			; 00000431H
	je	SHORT $LN13@ps_install
	cmp	DWORD PTR e$[rsp], 1078			; 00000436H
	je	SHORT $LN13@ps_install
	cmp	DWORD PTR e$[rsp], 1072			; 00000430H
	je	SHORT $LN13@ps_install
	mov	DWORD PTR tv131[rsp], 2
	jmp	SHORT $LN14@ps_install
$LN13@ps_install:
	mov	DWORD PTR tv131[rsp], 1
$LN14@ps_install:
	mov	eax, DWORD PTR tv131[rsp]
	mov	DWORD PTR native+4280, eax
	mov	eax, DWORD PTR e$[rsp]
	jmp	SHORT $LN1@ps_install
$LN7@ps_install:

; 84   :  native.install_disposition=3U; /* Created, not configured/finalized runtime. */

	mov	DWORD PTR native+4280, 3

; 85   :  if(papacc_server_console_win32_stop_requested())return ERROR_OPERATION_ABORTED;

	call	papacc_server_console_win32_stop_requested
	test	eax, eax
	je	SHORT $LN8@ps_install
	mov	eax, 995				; 000003e3H
	jmp	SHORT $LN1@ps_install
$LN8@ps_install:

; 86   :  info.dwServiceSidType=SERVICE_SID_TYPE_UNRESTRICTED;

	mov	DWORD PTR info$[rsp], 1

; 87   :  if(!ChangeServiceConfig2W(native.service,SERVICE_CONFIG_SERVICE_SID_INFO,&info))return ps_last_error();

	lea	r8, QWORD PTR info$[rsp]
	mov	edx, 5
	mov	rcx, QWORD PTR native+8
	call	QWORD PTR __imp_ChangeServiceConfig2W
	test	eax, eax
	jne	SHORT $LN9@ps_install
	call	ps_last_error
	jmp	SHORT $LN1@ps_install
$LN9@ps_install:

; 88   :  e=ps_virtual_readback();if(e)return e;

	call	ps_virtual_readback
	mov	DWORD PTR e$[rsp], eax
	cmp	DWORD PTR e$[rsp], 0
	je	SHORT $LN10@ps_install
	mov	eax, DWORD PTR e$[rsp]
	jmp	SHORT $LN1@ps_install
$LN10@ps_install:

; 89   :  if(papacc_server_console_win32_stop_requested())return ERROR_OPERATION_ABORTED;

	call	papacc_server_console_win32_stop_requested
	test	eax, eax
	je	SHORT $LN11@ps_install
	mov	eax, 995				; 000003e3H
	jmp	SHORT $LN1@ps_install
$LN11@ps_install:

; 90   :  native.install_disposition=4U; /* Configuration confirmed, NOT token/ACL/readiness. */

	mov	DWORD PTR native+4280, 4

; 91   :  return ERROR_SUCCESS;

	xor	eax, eax
$LN1@ps_install:

; 92   : }

	mov	rcx, QWORD PTR __$ArrayPad$[rsp]
	xor	rcx, rsp
	call	__security_check_cookie
	add	rsp, 200				; 000000c8H
	ret	0
ps_install_virtual ENDP