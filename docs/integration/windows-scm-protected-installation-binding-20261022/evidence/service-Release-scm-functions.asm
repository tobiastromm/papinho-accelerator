papacc_server_service_cli PROC				; COMDAT

; 62   : {

$LN124:
	push	rbp
	push	r12
	push	r13
	push	r14
	push	r15
	lea	rbp, QWORD PTR [rsp-448]
	sub	rsp, 704				; 000002c0H
	mov	rax, QWORD PTR __security_cookie
	xor	rax, rsp
	mov	QWORD PTR __$ArrayPad$[rbp-256], rax
	mov	r12, QWORD PTR prepare$[rbp-256]
	mov	r15, r8

; 63   :  PS_REQUEST q;PAPACC_SERVICE_IMAGE_VIEW view;SERVICE_STATUS_PROCESS s;DWORD e=0,close_error,bytes=0,rights;int parsed;

	xor	r13d, r13d

; 64   :  parsed=ps_parse(argc,argv,&q);if(parsed<0)return -1;

	lea	r8, QWORD PTR q$[rbp-256]
	mov	DWORD PTR bytes$[rsp], r13d
	mov	r14, r9
	call	ps_parse
	test	eax, eax
	jns	SHORT $LN2@papacc_ser
	mov	eax, -1
	jmp	$LN1@papacc_ser
$LN2@papacc_ser:
	mov	QWORD PTR [rsp+680], rdi

; 65   :  if(!out||!err)return 1;

	test	r15, r15
	je	SHORT $LN4@papacc_ser
	test	r14, r14
	je	SHORT $LN4@papacc_ser

; 66   :  if(!parsed){fputs("SERVICE=INVALID_COMMAND; expected install | start | stop | status | startup automatic|manual | remove\n",err);return 1;}

	test	eax, eax
	jne	SHORT $LN5@papacc_ser
	mov	rdx, r14
	lea	rcx, OFFSET FLAT:??_C@_0GH@HJGMJAEL@SERVICE?$DNINVALID_COMMAND?$DL?5expecte@
	call	fputs
$LN4@papacc_ser:
	mov	eax, 1
	jmp	$LN119@papacc_ser
$LN5@papacc_ser:

; 67   :  if(InterlockedCompareExchange(&cli_active,1,0)!=0){fputs("SERVICE=BUSY_OR_RETAINED; REUSE=DENY\n",err);return 2;}

	mov	edi, 1
	xor	eax, eax
	lock cmpxchg DWORD PTR cli_active, edi
	je	SHORT $LN6@papacc_ser
	mov	rdx, r14
	lea	rcx, OFFSET FLAT:??_C@_0CG@IJPKNGOF@SERVICE?$DNBUSY_OR_RETAINED?$DL?5REUSE?$DN@
	call	fputs
	mov	eax, 2
	jmp	$LN119@papacc_ser
$LN6@papacc_ser:

; 68   :  if(native.retained||native.manager||native.service||native.image||native.install_effect){fputs("SERVICE=RETAINED; REUSE=DENY\n",err);return 2;}

	cmp	DWORD PTR native+24, r13d
	jne	$LN8@papacc_ser
	cmp	QWORD PTR native, r13
	jne	$LN8@papacc_ser
	cmp	QWORD PTR native+8, r13
	jne	$LN8@papacc_ser
	cmp	QWORD PTR native+32, r13
	jne	$LN8@papacc_ser
	cmp	DWORD PTR native+4276, r13d
	jne	$LN8@papacc_ser
	mov	QWORD PTR [rsp+696], rbx
	mov	ebx, r13d
	mov	QWORD PTR [rsp+688], rsi

; 69   :  native.thread=GetCurrentThreadId();native.error=0;

	call	QWORD PTR __imp_GetCurrentThreadId

; 70   :  if(q.action==PS_INSTALL){

	mov	ecx, DWORD PTR q$[rbp-256]
	mov	esi, 31
	mov	DWORD PTR native+16, eax
	mov	edx, eax
	mov	DWORD PTR native+20, r13d
	cmp	ecx, edi
	jne	SHORT $LN12@papacc_ser

; 71   :   if(papacc_server_console_win32_stop_requested()){e=ERROR_OPERATION_ABORTED;goto done;}

	call	papacc_server_console_win32_stop_requested
	test	eax, eax
	je	SHORT $LN10@papacc_ser
	mov	ebx, 995				; 000003e3H
	jmp	$done$125
$LN10@papacc_ser:

; 72   :   if(papacc_server_console_win32_install(&native.console)!=PAPACC_RESULT_OK){e=ERROR_INVALID_STATE;goto done;}

	lea	rcx, OFFSET FLAT:native+28
	call	papacc_server_console_win32_install
	test	eax, eax
	je	SHORT $LN11@papacc_ser
	mov	ebx, 5023				; 0000139fH
	jmp	$done$125
$LN11@papacc_ser:

; 73   :   e=ps_image_open();if(e)goto done;

	call	ps_image_open
	mov	ebx, eax
	test	eax, eax
	jne	$done$125
	mov	ecx, DWORD PTR q$[rbp-256]
	mov	edx, DWORD PTR native+16
$LN12@papacc_ser:

; 74   :  }
; 75   :  if(q.prepare_lab){

	cmp	DWORD PTR q$[rbp-252], r13d
	je	$LN13@papacc_ser

; 76   :   if(!prepare){e=ERROR_NOT_SUPPORTED;goto done;}

	test	r12, r12
	jne	SHORT $LN14@papacc_ser
	mov	ebx, 50					; 00000032H
	jmp	$done$125
$LN14@papacc_ser:

; 77   :   view.handle=native.image;view.identity=native.image_id;view.path=native.image_path;view.sha256=native.image_sha256;view.owner_thread=native.thread;

	mov	rax, QWORD PTR native+32

; 78   :   e=prepare(q.installation,q.configuration,&view,out,err)?ERROR_ACCESS_DENIED:ERROR_SUCCESS;

	lea	r8, QWORD PTR view$[rsp]
	movups	xmm0, XMMWORD PTR native+40
	mov	QWORD PTR view$[rsp], rax
	mov	r9, r15
	mov	eax, DWORD PTR native+88
	lea	rcx, QWORD PTR q$[rbp-120]
	movups	xmm1, XMMWORD PTR native+56
	mov	DWORD PTR view$[rsp+56], eax
	lea	rax, OFFSET FLAT:native+96
	movups	XMMWORD PTR view$[rsp+8], xmm0
	mov	QWORD PTR view$[rbp-192], rax
	lea	rax, OFFSET FLAT:native+4244
	movups	xmm0, XMMWORD PTR native+72
	mov	DWORD PTR view$[rbp-176], edx
	lea	rdx, QWORD PTR q$[rbp+140]
	mov	QWORD PTR view$[rbp-184], rax
	movups	XMMWORD PTR view$[rsp+24], xmm1
	mov	QWORD PTR [rsp+32], r14
	movups	XMMWORD PTR view$[rsp+40], xmm0
	call	r12
	neg	eax
	sbb	ebx, ebx
	and	ebx, 5

; 79   :   goto done;

	jmp	SHORT $done$125
$LN13@papacc_ser:

; 80   :  }
; 81   :  if(q.action!=PS_STATUS&&!ps_binding_ready()){

	cmp	ecx, 4
	je	SHORT $LN15@papacc_ser

; 82   :   fputs("SERVICE=DENY; INSTALLATION_CONFIG_ACL_TOKEN_BINDING=NOT_QUALIFIED; MUTATION=NOT_RUN; CA_AUTHORITY=DENY\n",err);e=ERROR_ACCESS_DENIED;goto done;

	mov	rdx, r14
	lea	rcx, OFFSET FLAT:??_C@_0GI@ECPMGIFL@SERVICE?$DNDENY?$DL?5INSTALLATION_CONFI@
	call	fputs
$LN26@papacc_ser:

; 97   :   (unsigned long)s.dwCurrentState,(unsigned long)s.dwProcessId,(unsigned long)s.dwWin32ExitCode);
; 98   :  else {
; 99   :   e=ps_mutate(&q,&s);
; 100  :   if(!e)fputs(q.action==PS_REMOVE?"SERVICE=DELETE_REQUESTED; ABSENCE=NOT_PROVEN\n":"SERVICE=REQUEST_SUBMITTED; COMPLETION=NOT_PROVEN\n",out);
; 101  :  }
; 102  :  done:
; 103  :  native.error=e;close_error=ps_close();if(!native.error)native.error=close_error;

	mov	ebx, 5
$done$125:

; 36   :  if(native.service){if(!CloseServiceHandle(native.service)){e=ps_last_error();native.retained=1U;return e;}native.service=NULL;}

	mov	rcx, QWORD PTR native+8

; 97   :   (unsigned long)s.dwCurrentState,(unsigned long)s.dwProcessId,(unsigned long)s.dwWin32ExitCode);
; 98   :  else {
; 99   :   e=ps_mutate(&q,&s);
; 100  :   if(!e)fputs(q.action==PS_REMOVE?"SERVICE=DELETE_REQUESTED; ABSENCE=NOT_PROVEN\n":"SERVICE=REQUEST_SUBMITTED; COMPLETION=NOT_PROVEN\n",out);
; 101  :  }
; 102  :  done:
; 103  :  native.error=e;close_error=ps_close();if(!native.error)native.error=close_error;

	mov	DWORD PTR native+20, ebx

; 36   :  if(native.service){if(!CloseServiceHandle(native.service)){e=ps_last_error();native.retained=1U;return e;}native.service=NULL;}

	test	rcx, rcx
	je	$LN67@papacc_ser
	call	QWORD PTR __imp_CloseServiceHandle
	test	eax, eax
	jne	$LN68@papacc_ser
$LN122@papacc_ser:

; 97   :   (unsigned long)s.dwCurrentState,(unsigned long)s.dwProcessId,(unsigned long)s.dwWin32ExitCode);
; 98   :  else {
; 99   :   e=ps_mutate(&q,&s);
; 100  :   if(!e)fputs(q.action==PS_REMOVE?"SERVICE=DELETE_REQUESTED; ABSENCE=NOT_PROVEN\n":"SERVICE=REQUEST_SUBMITTED; COMPLETION=NOT_PROVEN\n",out);
; 101  :  }
; 102  :  done:
; 103  :  native.error=e;close_error=ps_close();if(!native.error)native.error=close_error;

	call	QWORD PTR __imp_GetLastError
	test	eax, eax
	mov	DWORD PTR native+24, edi
	cmovne	esi, eax
	jmp	$LN66@papacc_ser
$LN15@papacc_ser:

; 83   :  }
; 84   :  rights=SERVICE_QUERY_STATUS;
; 85   :  if(q.action==PS_START)rights|=SERVICE_START;
; 86   :  if(q.action==PS_STOP)rights|=SERVICE_STOP;
; 87   :  if(q.action==PS_REMOVE)rights|=DELETE;
; 88   :  if(q.action==PS_AUTOMATIC||q.action==PS_MANUAL)rights|=SERVICE_CHANGE_CONFIG;
; 89   :  native.manager=OpenSCManagerW(NULL,NULL,SC_MANAGER_CONNECT|(q.action==PS_INSTALL?SC_MANAGER_CREATE_SERVICE:0));

	mov	r8d, edi
	xor	edx, edx
	xor	ecx, ecx
	call	QWORD PTR __imp_OpenSCManagerW
	mov	QWORD PTR native, rax
	test	rax, rax

; 90   :  if(!native.manager){e=ps_last_error();goto done;}

	jne	SHORT $LN21@papacc_ser

; 21   : static DWORD ps_last_error(void){DWORD e=GetLastError();return e?e:ERROR_GEN_FAILURE;}

	call	QWORD PTR __imp_GetLastError
	test	eax, eax
	mov	ebx, esi
	cmovne	ebx, eax

; 90   :  if(!native.manager){e=ps_last_error();goto done;}

	jmp	SHORT $done$125
$LN21@papacc_ser:

; 91   :  if(q.action==PS_INSTALL){e=ps_install_virtual();goto done;}

	cmp	DWORD PTR q$[rbp-256], edi
	jne	SHORT $LN22@papacc_ser
	call	ps_install_virtual
	mov	ebx, eax
	jmp	SHORT $done$125
$LN22@papacc_ser:

; 92   :  native.service=OpenServiceW(native.manager,ps_name,rights);

	mov	r8d, 4
	lea	rdx, OFFSET FLAT:ps_name
	mov	rcx, rax
	call	QWORD PTR __imp_OpenServiceW
	mov	QWORD PTR native+8, rax
	test	rax, rax

; 93   :  if(!native.service){e=ps_last_error();if(q.action==PS_STATUS&&e==ERROR_SERVICE_DOES_NOT_EXIST){fputs("SERVICE=ABSENT; IDENTITY=NOT_QUALIFIED\n",out);e=0;}goto done;}

	jne	SHORT $LN23@papacc_ser

; 21   : static DWORD ps_last_error(void){DWORD e=GetLastError();return e?e:ERROR_GEN_FAILURE;}

	call	QWORD PTR __imp_GetLastError
	test	eax, eax
	mov	ebx, esi
	cmovne	ebx, eax

; 93   :  if(!native.service){e=ps_last_error();if(q.action==PS_STATUS&&e==ERROR_SERVICE_DOES_NOT_EXIST){fputs("SERVICE=ABSENT; IDENTITY=NOT_QUALIFIED\n",out);e=0;}goto done;}

	cmp	DWORD PTR q$[rbp-256], 4
	jne	$done$125
	cmp	ebx, 1060				; 00000424H
	jne	$done$125
	mov	rdx, r15
	lea	rcx, OFFSET FLAT:??_C@_0CI@BEAFCAKF@SERVICE?$DNABSENT?$DL?5IDENTITY?$DNNOT_QUA@
	call	fputs
	mov	ebx, r13d
	jmp	$done$125
$LN23@papacc_ser:

; 94   :  memset(&s,0,sizeof(s));

	xor	ecx, ecx

; 95   :  if(!QueryServiceStatusEx(native.service,SC_STATUS_PROCESS_INFO,(LPBYTE)&s,sizeof(s),&bytes)){e=ps_last_error();goto done;}

	lea	r8, QWORD PTR s$[rbp-256]
	mov	DWORD PTR s$[rbp-224], ecx
	xorps	xmm0, xmm0
	lea	rcx, QWORD PTR bytes$[rsp]
	mov	r9d, 36					; 00000024H
	mov	QWORD PTR [rsp+32], rcx
	xor	edx, edx
	mov	rcx, rax
	movups	XMMWORD PTR s$[rbp-256], xmm0
	movups	XMMWORD PTR s$[rbp-240], xmm0
	call	QWORD PTR __imp_QueryServiceStatusEx
	test	eax, eax
	jne	SHORT $LN25@papacc_ser

; 21   : static DWORD ps_last_error(void){DWORD e=GetLastError();return e?e:ERROR_GEN_FAILURE;}

	call	QWORD PTR __imp_GetLastError
	test	eax, eax
	mov	ebx, esi
	cmovne	ebx, eax

; 95   :  if(!QueryServiceStatusEx(native.service,SC_STATUS_PROCESS_INFO,(LPBYTE)&s,sizeof(s),&bytes)){e=ps_last_error();goto done;}

	jmp	$done$125
$LN25@papacc_ser:

; 96   :  if(q.action==PS_STATUS)fprintf(out,"SERVICE=OBSERVED; STATE=%lu; PID=%lu; WIN32_EXIT=%lu; IDENTITY=NOT_QUALIFIED; AUTHORITY=NONE\n",

	cmp	DWORD PTR q$[rbp-256], 4
	jne	$LN26@papacc_ser
	mov	eax, DWORD PTR s$[rbp-244]
	lea	rdx, OFFSET FLAT:??_C@_0FO@PHGGABED@SERVICE?$DNOBSERVED?$DL?5STATE?$DN?$CFlu?$DL?5PID@
	mov	r9d, DWORD PTR s$[rbp-228]
	mov	rcx, r15
	mov	r8d, DWORD PTR s$[rbp-252]
	mov	DWORD PTR [rsp+32], eax
	call	fprintf
	jmp	$done$125
$LN68@papacc_ser:

; 36   :  if(native.service){if(!CloseServiceHandle(native.service)){e=ps_last_error();native.retained=1U;return e;}native.service=NULL;}

	mov	QWORD PTR native+8, r13
$LN67@papacc_ser:

; 37   :  if(native.manager){if(!CloseServiceHandle(native.manager)){e=ps_last_error();native.retained=1U;return e;}native.manager=NULL;}

	mov	rcx, QWORD PTR native
	test	rcx, rcx
	je	SHORT $LN69@papacc_ser
	call	QWORD PTR __imp_CloseServiceHandle
	test	eax, eax
	je	$LN122@papacc_ser
	mov	QWORD PTR native, r13
$LN69@papacc_ser:

; 38   :  if(native.image){if(!CloseHandle(native.image)){e=ps_last_error();native.retained=1U;return e;}native.image=NULL;native.image_ready=0U;}

	mov	rcx, QWORD PTR native+32
	test	rcx, rcx
	je	SHORT $LN71@papacc_ser
	call	QWORD PTR __imp_CloseHandle
	test	eax, eax
	je	$LN122@papacc_ser
	mov	QWORD PTR native+32, r13
	mov	DWORD PTR native+92, r13d
$LN71@papacc_ser:

; 39   :  if(native.console.is_installed&&papacc_server_console_win32_uninstall(&native.console)!=PAPACC_RESULT_OK){native.retained=1U;return ERROR_GEN_FAILURE;}

	cmp	DWORD PTR native+28, r13d
	je	SHORT $LN82@papacc_ser
	lea	rcx, OFFSET FLAT:native+28
	call	papacc_server_console_win32_uninstall
	test	eax, eax
	je	SHORT $LN82@papacc_ser
	mov	DWORD PTR native+24, edi
	jmp	SHORT $LN66@papacc_ser
$LN82@papacc_ser:

; 40   :  return ERROR_SUCCESS;

	mov	edi, DWORD PTR native+24
	mov	esi, r13d
$LN66@papacc_ser:

; 97   :   (unsigned long)s.dwCurrentState,(unsigned long)s.dwProcessId,(unsigned long)s.dwWin32ExitCode);
; 98   :  else {
; 99   :   e=ps_mutate(&q,&s);
; 100  :   if(!e)fputs(q.action==PS_REMOVE?"SERVICE=DELETE_REQUESTED; ABSENCE=NOT_PROVEN\n":"SERVICE=REQUEST_SUBMITTED; COMPLETION=NOT_PROVEN\n",out);
; 101  :  }
; 102  :  done:
; 103  :  native.error=e;close_error=ps_close();if(!native.error)native.error=close_error;

	cmp	DWORD PTR native+20, r13d
	jne	SHORT $LN29@papacc_ser
	mov	DWORD PTR native+20, esi
$LN29@papacc_ser:

; 104  :  if(!native.retained&&!native.install_effect)InterlockedExchange(&cli_active,0);

	test	edi, edi
	jne	SHORT $LN30@papacc_ser
	cmp	DWORD PTR native+4276, r13d
	jne	SHORT $LN30@papacc_ser
	mov	eax, r13d
	xchg	DWORD PTR cli_active, eax
	mov	edi, DWORD PTR native+24
$LN30@papacc_ser:

; 105  :  if(e||close_error){fprintf(err,"SERVICE=ERROR; WIN32=%lu; CLOSE=%lu; RETAINED=%u; NO_AUTOMATIC_RETRY\n",(unsigned long)e,(unsigned long)close_error,native.retained);return 2;}

	mov	eax, ebx
	or	eax, esi
	jne	SHORT $LN32@papacc_ser
$LN121@papacc_ser:
	mov	rsi, QWORD PTR [rsp+688]
	mov	rbx, QWORD PTR [rsp+696]
$LN119@papacc_ser:
	mov	rdi, QWORD PTR [rsp+680]
$LN1@papacc_ser:

; 106  :  return 0;
; 107  : }

	mov	rcx, QWORD PTR __$ArrayPad$[rbp-256]
	xor	rcx, rsp
	call	__security_check_cookie
	add	rsp, 704				; 000002c0H
	pop	r15
	pop	r14
	pop	r13
	pop	r12
	pop	rbp
	ret	0
$LN32@papacc_ser:

; 105  :  if(e||close_error){fprintf(err,"SERVICE=ERROR; WIN32=%lu; CLOSE=%lu; RETAINED=%u; NO_AUTOMATIC_RETRY\n",(unsigned long)e,(unsigned long)close_error,native.retained);return 2;}

	mov	r9d, esi
	mov	DWORD PTR [rsp+32], edi
	mov	r8d, ebx
	lea	rdx, OFFSET FLAT:??_C@_0EG@IKAACFKM@SERVICE?$DNERROR?$DL?5WIN32?$DN?$CFlu?$DL?5CLOSE?$DN@
	mov	rcx, r14
	call	fprintf
	mov	eax, 2
	jmp	SHORT $LN121@papacc_ser
$LN8@papacc_ser:

; 68   :  if(native.retained||native.manager||native.service||native.image||native.install_effect){fputs("SERVICE=RETAINED; REUSE=DENY\n",err);return 2;}

	mov	rdx, r14
	lea	rcx, OFFSET FLAT:??_C@_0BO@PJDMONH@SERVICE?$DNRETAINED?$DL?5REUSE?$DNDENY?6@
	call	fputs
	mov	eax, 2
	jmp	SHORT $LN119@papacc_ser
papacc_server_service_cli ENDP

papacc_server_service_host_active PROC			; COMDAT

; 130  : {return host.handle&&host.run_active&&host.thread==GetCurrentThreadId();}

$LN6:
	sub	rsp, 40					; 00000028H
	cmp	QWORD PTR host, 0
	je	SHORT $LN3@papacc_ser
	cmp	DWORD PTR host+56, 0
	je	SHORT $LN3@papacc_ser
	call	QWORD PTR __imp_GetCurrentThreadId
	cmp	DWORD PTR host+52, eax
	jne	SHORT $LN3@papacc_ser
	mov	eax, 1
	add	rsp, 40					; 00000028H
	ret	0
$LN3@papacc_ser:
	xor	eax, eax
	add	rsp, 40					; 00000028H
	ret	0
papacc_server_service_host_active ENDP

papacc_server_service_runtime_ready PROC		; COMDAT

; 132  : {

$LN24:
	sub	rsp, 40					; 00000028H

; 133  :  if(!host.handle)return 1; /* direct console runtime, no SCM requirement */

	cmp	QWORD PTR host, 0
	jne	SHORT $LN2@papacc_ser
$LN9@papacc_ser:

; 140  :  return 1;
; 141  : }

	mov	eax, 1
	add	rsp, 40					; 00000028H
	ret	0
$LN2@papacc_ser:

; 134  :  if(!host.run_active||host.thread!=GetCurrentThreadId()||host.terminal_receipt||host.life.phase!=PS_START_PENDING)return 0;

	cmp	DWORD PTR host+56, 0
	je	$LN4@papacc_ser
	call	QWORD PTR __imp_GetCurrentThreadId
	cmp	DWORD PTR host+52, eax
	jne	$LN4@papacc_ser
	cmp	DWORD PTR host+60, 0
	jne	$LN4@papacc_ser
	cmp	DWORD PTR host+36, 1
	jne	$LN4@papacc_ser

; 135  :  if(InterlockedCompareExchange(&host.stop,0,0)||papacc_server_console_win32_stop_requested()){

	xor	ecx, ecx
	xor	eax, eax
	lock cmpxchg DWORD PTR host+48, ecx
	jne	SHORT $LN6@papacc_ser
	call	papacc_server_console_win32_stop_requested
	test	eax, eax
	jne	SHORT $LN6@papacc_ser

; 136  :   (void)ps_stop(&host.life);(void)ps_report(SERVICE_STOP_PENDING,ERROR_SUCCESS);return 0;
; 137  :  }
; 138  :  if(!ps_running(&host.life)||!ps_report(SERVICE_RUNNING,ERROR_SUCCESS))return 0;

	lea	rcx, OFFSET FLAT:host+36
	call	ps_running
	test	eax, eax
	je	$LN4@papacc_ser

; 127  :  return SetServiceStatus(host.handle,&host.status)!=0;

	mov	rcx, QWORD PTR host
	lea	rdx, OFFSET FLAT:host+8
	xorps	xmm0, xmm0

; 121  :  memset(&host.status,0,sizeof(host.status));host.status.dwServiceType=SERVICE_WIN32_OWN_PROCESS;

	movups	XMMWORD PTR host+8, xmm0
	mov	DWORD PTR host+8, 16
	movups	XMMWORD PTR host+20, xmm0

; 122  :  host.status.dwCurrentState=state;host.status.dwWin32ExitCode=error;
; 123  :  if(error==ERROR_SERVICE_SPECIFIC_ERROR)host.status.dwServiceSpecificExitCode=(DWORD)host.result;
; 124  :  if(state==SERVICE_RUNNING)host.status.dwControlsAccepted=SERVICE_ACCEPT_STOP|SERVICE_ACCEPT_SHUTDOWN;

	mov	QWORD PTR host+16, 5
	mov	DWORD PTR host+12, 4

; 125  :  /* Startup is denied before runtime. No positive readiness/Stop controls
; 126  :   * advertised without the missing native owner/config/admission binding. */
; 127  :  return SetServiceStatus(host.handle,&host.status)!=0;

	call	QWORD PTR __imp_SetServiceStatus

; 136  :   (void)ps_stop(&host.life);(void)ps_report(SERVICE_STOP_PENDING,ERROR_SUCCESS);return 0;
; 137  :  }
; 138  :  if(!ps_running(&host.life)||!ps_report(SERVICE_RUNNING,ERROR_SUCCESS))return 0;

	test	eax, eax
	je	SHORT $LN4@papacc_ser

; 139  :  if(InterlockedCompareExchange(&host.stop,0,0)){(void)ps_stop(&host.life);(void)ps_report(SERVICE_STOP_PENDING,ERROR_SUCCESS);return 0;}

	xor	r8d, r8d
	xor	eax, eax
	lock cmpxchg DWORD PTR host+48, r8d
	je	$LN9@papacc_ser
$LN6@papacc_ser:

; 140  :  return 1;
; 141  : }

	lea	rcx, OFFSET FLAT:host+36
	call	ps_stop
	mov	rcx, QWORD PTR host
	lea	rdx, OFFSET FLAT:host+8
	xorps	xmm0, xmm0
	xor	eax, eax
	movups	XMMWORD PTR host+8, xmm0
	mov	DWORD PTR host+8, 16
	movups	XMMWORD PTR host+20, xmm0
	mov	DWORD PTR host+20, eax
	mov	DWORD PTR host+12, 3
	call	QWORD PTR __imp_SetServiceStatus
$LN4@papacc_ser:
	xor	eax, eax
	add	rsp, 40					; 00000028H
	ret	0
papacc_server_service_runtime_ready ENDP

papacc_server_service_runtime_stopping PROC		; COMDAT

; 143  : {

$LN16:
	sub	rsp, 40					; 00000028H

; 144  :  if(!host.handle)return 1;

	cmp	QWORD PTR host, 0
	je	$LN13@papacc_ser

; 145  :  if(!host.run_active||host.thread!=GetCurrentThreadId()||host.terminal_receipt)return 0;

	cmp	DWORD PTR host+56, 0
	je	SHORT $LN4@papacc_ser
	call	QWORD PTR __imp_GetCurrentThreadId
	cmp	DWORD PTR host+52, eax
	jne	SHORT $LN4@papacc_ser
	cmp	DWORD PTR host+60, 0
	jne	SHORT $LN4@papacc_ser

; 146  :  if(host.life.phase==PS_STOP_PENDING)return 1;

	cmp	DWORD PTR host+36, 3
	je	SHORT $LN13@papacc_ser

; 147  :  if(!ps_stop(&host.life))return 0;

	lea	rcx, OFFSET FLAT:host+36
	call	ps_stop
	test	eax, eax
	je	SHORT $LN4@papacc_ser

; 127  :  return SetServiceStatus(host.handle,&host.status)!=0;

	mov	rcx, QWORD PTR host
	lea	rdx, OFFSET FLAT:host+8
	xorps	xmm0, xmm0

; 121  :  memset(&host.status,0,sizeof(host.status));host.status.dwServiceType=SERVICE_WIN32_OWN_PROCESS;

	xor	eax, eax
	movups	XMMWORD PTR host+8, xmm0
	mov	DWORD PTR host+8, 16
	movups	XMMWORD PTR host+20, xmm0

; 122  :  host.status.dwCurrentState=state;host.status.dwWin32ExitCode=error;

	mov	DWORD PTR host+20, eax
	mov	DWORD PTR host+12, 3

; 123  :  if(error==ERROR_SERVICE_SPECIFIC_ERROR)host.status.dwServiceSpecificExitCode=(DWORD)host.result;
; 124  :  if(state==SERVICE_RUNNING)host.status.dwControlsAccepted=SERVICE_ACCEPT_STOP|SERVICE_ACCEPT_SHUTDOWN;
; 125  :  /* Startup is denied before runtime. No positive readiness/Stop controls
; 126  :   * advertised without the missing native owner/config/admission binding. */
; 127  :  return SetServiceStatus(host.handle,&host.status)!=0;

	call	QWORD PTR __imp_SetServiceStatus
	xor	ecx, ecx
	test	eax, eax
	setne	cl

; 148  :  return ps_report(SERVICE_STOP_PENDING,ERROR_SUCCESS);

	mov	eax, ecx

; 149  : }

	add	rsp, 40					; 00000028H
	ret	0
$LN4@papacc_ser:

; 145  :  if(!host.run_active||host.thread!=GetCurrentThreadId()||host.terminal_receipt)return 0;

	xor	eax, eax

; 149  : }

	add	rsp, 40					; 00000028H
	ret	0
$LN13@papacc_ser:

; 146  :  if(host.life.phase==PS_STOP_PENDING)return 1;

	mov	eax, 1

; 149  : }

	add	rsp, 40					; 00000028H
	ret	0
papacc_server_service_runtime_stopping ENDP

papacc_server_service_runtime_closed PROC		; COMDAT

; 151  : {

$LN7:
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
	jne	SHORT $LN3@papacc_ser

; 153  :  if(ps_finish(&host.life,1,0))host.terminal_receipt=1U;

	xor	r8d, r8d
	lea	rcx, OFFSET FLAT:host+36
	mov	edx, 1
	call	ps_finish
	test	eax, eax
	je	SHORT $LN3@papacc_ser
	mov	DWORD PTR host+60, 1
$LN3@papacc_ser:

; 154  : }

	add	rsp, 40					; 00000028H
	ret	0
papacc_server_service_runtime_closed ENDP

papacc_server_service_runtime_retained PROC		; COMDAT

; 156  : {

$LN6:
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
	jne	SHORT $LN3@papacc_ser

; 158  :  (void)ps_finish(&host.life,0,1);host.terminal_receipt=2U;

	xor	edx, edx
	lea	rcx, OFFSET FLAT:host+36
	mov	r8d, 1
	call	ps_finish
	mov	DWORD PTR host+60, 2
$LN3@papacc_ser:

; 159  : }

	add	rsp, 40					; 00000028H
	ret	0
papacc_server_service_runtime_retained ENDP

papacc_server_service_dispatch PROC			; COMDAT

; 192  : {

$LN23:
	push	rbx
	sub	rsp, 64					; 00000040H
	mov	r11d, ecx

; 193  :  SERVICE_TABLE_ENTRYW table[2];
; 194  :  if(argc<3||!argv||!argv[1]||!argv[2]||strcmp(argv[1],"service")||strcmp(argv[2],"host"))return -1;

	cmp	ecx, 3
	jl	$LN3@papacc_ser
	test	rdx, rdx
	je	$LN3@papacc_ser
	mov	r9, QWORD PTR [rdx+8]
	test	r9, r9
	je	$LN3@papacc_ser
	mov	r10, QWORD PTR [rdx+16]
	test	r10, r10
	je	$LN3@papacc_ser
	xor	ecx, ecx
	lea	rbx, OFFSET FLAT:??_C@_07EFMBDOBA@service@
	npad	2
$LL9@papacc_ser:
	movzx	eax, BYTE PTR [r9+rcx]
	inc	rcx
	cmp	al, BYTE PTR [rbx+rcx-1]
	jne	$LN3@papacc_ser
	cmp	rcx, 8
	jne	SHORT $LL9@papacc_ser
	xor	ecx, ecx
	lea	r9, OFFSET FLAT:??_C@_04OODOLJMA@host@
$LL8@papacc_ser:
	movzx	eax, BYTE PTR [r10+rcx]
	inc	rcx
	cmp	al, BYTE PTR [r9+rcx-1]
	jne	$LN3@papacc_ser
	cmp	rcx, 5
	jne	SHORT $LL8@papacc_ser

; 195  :  if(argc!=3||!run||host.handle||host.life.phase)return 1;

	cmp	r11d, 3
	jne	$LN5@papacc_ser
	test	r8, r8
	je	$LN5@papacc_ser
	cmp	QWORD PTR host, 0
	jne	SHORT $LN5@papacc_ser
	cmp	DWORD PTR host+36, 0
	jne	SHORT $LN5@papacc_ser

; 196  :  host.argc=1;host.argv=argv;host.run=run;host.result=2;
; 197  :  table[0].lpServiceName=(LPWSTR)ps_name;table[0].lpServiceProc=ps_main;

	lea	rax, OFFSET FLAT:ps_name
	mov	DWORD PTR host+64, 1
	mov	QWORD PTR table$[rsp], rax

; 198  :  table[1].lpServiceName=NULL;table[1].lpServiceProc=NULL;
; 199  :  if(!StartServiceCtrlDispatcherW(table))return 2; /* no console fallback */

	lea	rcx, QWORD PTR table$[rsp]
	lea	rax, OFFSET FLAT:ps_main
	mov	QWORD PTR host+72, rdx
	mov	ebx, 2
	mov	QWORD PTR table$[rsp+8], rax
	mov	QWORD PTR host+80, r8
	mov	DWORD PTR host+88, ebx
	mov	QWORD PTR table$[rsp+16], 0
	mov	QWORD PTR table$[rsp+24], 0
	call	QWORD PTR __imp_StartServiceCtrlDispatcherW
	mov	ecx, DWORD PTR host+88
	test	eax, eax
	cmove	ecx, ebx
	mov	eax, ecx

; 200  :  return host.result;
; 201  : }

	add	rsp, 64					; 00000040H
	pop	rbx
	ret	0
$LN5@papacc_ser:

; 195  :  if(argc!=3||!run||host.handle||host.life.phase)return 1;

	mov	eax, 1

; 200  :  return host.result;
; 201  : }

	add	rsp, 64					; 00000040H
	pop	rbx
	ret	0
$LN3@papacc_ser:

; 193  :  SERVICE_TABLE_ENTRYW table[2];
; 194  :  if(argc<3||!argv||!argv[1]||!argv[2]||strcmp(argv[1],"service")||strcmp(argv[2],"host"))return -1;

	mov	eax, -1

; 200  :  return host.result;
; 201  : }

	add	rsp, 64					; 00000040H
	pop	rbx
	ret	0
papacc_server_service_dispatch ENDP

papacc_server_service_storage_bytes PROC		; COMDAT

; 27   : size_t papacc_server_service_storage_bytes(void){return sizeof(native)+sizeof(host);}

	mov	eax, 12912				; 00003270H
	ret	0
papacc_server_service_storage_bytes ENDP

ps_image_open PROC					; COMDAT

; 11   : {

	mov	r11, rsp
	push	rbp
	lea	rbp, QWORD PTR [r11-1096]
	sub	rsp, 1344				; 00000540H
	mov	rax, QWORD PTR __security_cookie
	xor	rax, rsp
	mov	QWORD PTR __$ArrayPad$[rbp-256], rax

; 13   :  unsigned char block[1024],digest[32];size_t i;HANDLE f;
; 14   :  if(native.image||native.image_ready)return ERROR_INVALID_STATE;

	cmp	QWORD PTR native+32, 0
	jne	$LN9@ps_image_o
	cmp	DWORD PTR native+92, 0
	jne	$LN9@ps_image_o
	mov	QWORD PTR [r11-16], r14

; 15   :  n=GetModuleFileNameW(NULL,native.image_path,1024);

	lea	rdx, OFFSET FLAT:native+96
	mov	r8d, 1024				; 00000400H
	mov	QWORD PTR [r11+8], rbx
	xor	ecx, ecx
	call	QWORD PTR __imp_GetModuleFileNameW
	mov	r14d, eax

; 16   :  if(!n)return ps_last_error();if(n>=1024)return ERROR_INSUFFICIENT_BUFFER;

	test	eax, eax
	jne	SHORT $LN10@ps_image_o
; File C:\Projetos\PapinhoAccelerator\apps\server\server_service_win32.c

; 21   : static DWORD ps_last_error(void){DWORD e=GetLastError();return e?e:ERROR_GEN_FAILURE;}

	call	QWORD PTR __imp_GetLastError
	test	eax, eax
	mov	ebx, 31
	cmovne	ebx, eax
; File C:\Projetos\PapinhoAccelerator\apps\server\server_service_virtual_win32.inc

; 16   :  if(!n)return ps_last_error();if(n>=1024)return ERROR_INSUFFICIENT_BUFFER;

	mov	eax, ebx
	jmp	$LN91@ps_image_o
$LN10@ps_image_o:
	cmp	r14d, 1024				; 00000400H
	jb	SHORT $LN11@ps_image_o
	mov	eax, 122				; 0000007aH
	jmp	$LN91@ps_image_o
$LN11@ps_image_o:

; 17   :  if(!((native.image_path[0]>=L'A'&&native.image_path[0]<=L'Z')||(native.image_path[0]>=L'a'&&native.image_path[0]<=L'z'))||
; 18   :  native.image_path[1]!=L':'||native.image_path[2]!=L'\\')return ERROR_INVALID_NAME;

	movzx	ecx, WORD PTR native+96
	mov	QWORD PTR [rsp+1368], rsi
	mov	QWORD PTR [rsp+1384], r12
	mov	QWORD PTR [rsp+1328], r15
	lea	eax, DWORD PTR [rcx-65]
	cmp	ax, 25
	jbe	SHORT $LN14@ps_image_o
	sub	cx, 97					; 00000061H
	cmp	cx, 25
	ja	$LN13@ps_image_o
$LN14@ps_image_o:
	cmp	WORD PTR native+98, 58			; 0000003aH
	jne	$LN13@ps_image_o
	cmp	WORD PTR native+100, 92			; 0000005cH
	jne	$LN13@ps_image_o

; 19   :  for(i=0;i<n;i++)if(native.image_path[i]==L'"'||native.image_path[i]<L' ')return ERROR_INVALID_NAME;

	xor	r15d, r15d
	lea	r12, OFFSET FLAT:native
	mov	eax, r15d
	test	r14d, r14d
	je	SHORT $LN3@ps_image_o
$LL4@ps_image_o:
	movzx	ecx, WORD PTR [r12+rax*2+96]
	cmp	cx, 34					; 00000022H
	je	$LN13@ps_image_o
	cmp	cx, 32					; 00000020H
	jb	$LN13@ps_image_o
	inc	rax
	cmp	rax, r14
	jb	SHORT $LL4@ps_image_o
$LN3@ps_image_o:

; 20   :  f=CreateFileW(native.image_path,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);

	mov	QWORD PTR [rsp+48], r15
	lea	rcx, OFFSET FLAT:native+96
	mov	DWORD PTR [rsp+40], 2097152		; 00200000H
	xor	r9d, r9d
	mov	edx, -2147483648			; 80000000H
	mov	DWORD PTR [rsp+32], 3
	mov	r8d, 1
	mov	QWORD PTR [rsp+1376], rdi
	call	QWORD PTR __imp_CreateFileW
	mov	rdi, rax

; 21   :  if(f==INVALID_HANDLE_VALUE)return ps_last_error();native.image=f;

	cmp	rax, -1
	je	$LN49@ps_image_o

; 22   :  if(!GetFileInformationByHandle(f,&native.image_id))return ps_last_error();

	lea	rdx, OFFSET FLAT:native+40
	mov	QWORD PTR native+32, rax
	mov	rcx, rax
	call	QWORD PTR __imp_GetFileInformationByHandle
	test	eax, eax
	je	$LN49@ps_image_o

; 23   :  if(native.image_id.dwFileAttributes&(FILE_ATTRIBUTE_REPARSE_POINT|FILE_ATTRIBUTE_DIRECTORY))return ERROR_ACCESS_DENIED;

	test	DWORD PTR native+40, 1040		; 00000410H
	je	SHORT $LN20@ps_image_o
	mov	eax, 5
	jmp	$LN95@ps_image_o
$LN20@ps_image_o:
	xorps	xmm0, xmm0

; 24   :  memset(&hash,0,sizeof(hash));if(pacc_storage_sha_init(&hash)!=PACC_HASH_OK)return ERROR_INVALID_DATA;

	lea	rcx, QWORD PTR hash$[rbp-256]
	movups	XMMWORD PTR hash$[rbp-256], xmm0
	movups	XMMWORD PTR hash$[rbp-240], xmm0
	movups	XMMWORD PTR hash$[rbp-224], xmm0
	movups	XMMWORD PTR hash$[rbp-208], xmm0
	movups	XMMWORD PTR hash$[rbp-192], xmm0
	movups	XMMWORD PTR hash$[rbp-176], xmm0
	movups	XMMWORD PTR hash$[rbp-160], xmm0
	movups	XMMWORD PTR hash$[rbp-144], xmm0
	call	pacc_storage_sha_init
	test	eax, eax
	je	SHORT $LN21@ps_image_o
	mov	eax, 13
	jmp	SHORT $LN95@ps_image_o
$LN21@ps_image_o:

; 12   :  DWORD n,read,e=ERROR_INVALID_DATA;BY_HANDLE_FILE_INFORMATION after;PACC_STORAGE_SHA256 hash;

	mov	ebx, 13

; 25   :  for(;;){
; 26   :   if(papacc_server_console_win32_stop_requested()) {e=ERROR_OPERATION_ABORTED;goto done;}

	call	papacc_server_console_win32_stop_requested
	test	eax, eax
	jne	SHORT $LN48@ps_image_o
	npad	15
$LL7@ps_image_o:

; 27   :   if(!ReadFile(f,block,sizeof(block),&read,NULL)){e=ps_last_error();goto done;}

	lea	r9, QWORD PTR read$[rsp]
	mov	QWORD PTR [rsp+32], r15
	mov	r8d, 1024				; 00000400H
	lea	rdx, QWORD PTR block$[rbp-256]
	mov	rcx, rdi
	call	QWORD PTR __imp_ReadFile
	test	eax, eax
	je	$LN49@ps_image_o

; 28   :   if(!read)break;

	mov	eax, DWORD PTR read$[rsp]
	lea	rcx, QWORD PTR hash$[rbp-256]
	xor	r9d, r9d
	mov	QWORD PTR [rsp+32], r15
	test	eax, eax
	je	SHORT $LN50@ps_image_o

; 29   :   if(pacc_storage_sha_update(&hash,block,read,NULL,NULL)!=PACC_HASH_OK)goto done;

	mov	r8d, eax
	lea	rdx, QWORD PTR block$[rbp-256]
	call	pacc_storage_sha_update
	test	eax, eax
	jne	SHORT $done$98

; 25   :  for(;;){
; 26   :   if(papacc_server_console_win32_stop_requested()) {e=ERROR_OPERATION_ABORTED;goto done;}

	call	papacc_server_console_win32_stop_requested
	test	eax, eax
	je	SHORT $LL7@ps_image_o
$LN48@ps_image_o:
	mov	ebx, 995				; 000003e3H
$done$98:

; 22   :  if(!GetFileInformationByHandle(f,&native.image_id))return ps_last_error();

	mov	eax, ebx
$LN95@ps_image_o:
	mov	rdi, QWORD PTR [rsp+1376]
$LN94@ps_image_o:
	mov	r12, QWORD PTR [rsp+1384]
	mov	rsi, QWORD PTR [rsp+1368]
	mov	r15, QWORD PTR [rsp+1328]
$LN91@ps_image_o:
	mov	rbx, QWORD PTR [rsp+1360]
	mov	r14, QWORD PTR [rsp+1336]
$LN1@ps_image_o:

; 38   :  done:memset(&hash,0,sizeof(hash));memset(block,0,sizeof(block));memset(digest,0,sizeof(digest));return e;
; 39   : }

	mov	rcx, QWORD PTR __$ArrayPad$[rbp-256]
	xor	rcx, rsp
	call	__security_check_cookie
	add	rsp, 1344				; 00000540H
	pop	rbp
	ret	0
$LN50@ps_image_o:

; 30   :  }
; 31   :  if(pacc_storage_sha_final(&hash,digest,sizeof(digest),NULL,NULL)!=PACC_HASH_OK)goto done;

	mov	r8d, 32					; 00000020H
	lea	rdx, QWORD PTR digest$[rbp-256]
	call	pacc_storage_sha_final
	test	eax, eax
	jne	SHORT $done$98

; 32   :  if(!GetFileInformationByHandle(f,&after)){e=ps_last_error();goto done;}

	lea	rdx, QWORD PTR after$[rsp]
	mov	rcx, rdi
	call	QWORD PTR __imp_GetFileInformationByHandle
	test	eax, eax
	je	$LN49@ps_image_o

; 6    :  return a->dwVolumeSerialNumber==b->dwVolumeSerialNumber&&a->nFileIndexHigh==b->nFileIndexHigh&&

	mov	eax, DWORD PTR after$[rsp+28]
	cmp	DWORD PTR native+68, eax
	jne	SHORT $done$98
	mov	eax, DWORD PTR after$[rsp+44]
	cmp	DWORD PTR native+84, eax
	jne	$done$98
	mov	eax, DWORD PTR after$[rsp+48]
	cmp	DWORD PTR native+88, eax
	jne	$done$98
	mov	eax, DWORD PTR after$[rsp+32]
	cmp	DWORD PTR native+72, eax
	jne	$done$98
	mov	eax, DWORD PTR after$[rsp+36]
	cmp	DWORD PTR native+76, eax
	jne	$done$98
	mov	eax, DWORD PTR after$[rsp+24]
	cmp	DWORD PTR native+64, eax
	jne	$done$98
	mov	eax, DWORD PTR after$[rsp+20]
	cmp	DWORD PTR native+60, eax
	jne	$done$98

; 33   :  if(!ps_image_same(&native.image_id,&after)||papacc_server_console_win32_stop_requested())goto done;

	call	papacc_server_console_win32_stop_requested
	test	eax, eax
	jne	$done$98

; 34   :  native.command[0]=L'"';memcpy(native.command+1,native.image_path,(n+1U)*sizeof(wchar_t));

	mov	eax, 34					; 00000022H
	lea	r8d, DWORD PTR [r14+1]
	add	r8, r8
	mov	WORD PTR native+2144, ax
	lea	rdx, OFFSET FLAT:native+96
	lea	rcx, OFFSET FLAT:native+2146
	call	memcpy

; 35   :  /* Fixed suffix only; no argv interpolation. 1024 path bound leaves room. */
; 36   :  memcpy(native.command+1+n,L"\" service host",sizeof(L"\" service host"));

	movups	xmm0, XMMWORD PTR ??_C@_1BO@EAPHNLDA@?$AA?$CC?$AA?5?$AAs?$AAe?$AAr?$AAv?$AAi?$AAc?$AAe?$AA?5?$AAh?$AAo?$AAs?$AAt@

; 37   :  memcpy(native.image_sha256,digest,32);native.image_ready=1U;e=ERROR_SUCCESS;

	mov	ebx, r15d
	movups	xmm1, XMMWORD PTR ??_C@_1BO@EAPHNLDA@?$AA?$CC?$AA?5?$AAs?$AAe?$AAr?$AAv?$AAi?$AAc?$AAe?$AA?5?$AAh?$AAo?$AAs?$AAt@+14
	movups	XMMWORD PTR [r12+r14*2+2146], xmm0
	movups	xmm0, XMMWORD PTR digest$[rbp-256]
	movups	XMMWORD PTR [r12+r14*2+2160], xmm1
	mov	DWORD PTR native+92, 1
	movups	xmm1, XMMWORD PTR digest$[rbp-240]
	movups	XMMWORD PTR native+4244, xmm0
	movups	XMMWORD PTR native+4260, xmm1
	jmp	$done$98
$LN49@ps_image_o:

; 22   :  if(!GetFileInformationByHandle(f,&native.image_id))return ps_last_error();

	call	QWORD PTR __imp_GetLastError
	test	eax, eax
	mov	ebx, 31
	cmovne	ebx, eax
	jmp	$done$98
$LN13@ps_image_o:

; 17   :  if(!((native.image_path[0]>=L'A'&&native.image_path[0]<=L'Z')||(native.image_path[0]>=L'a'&&native.image_path[0]<=L'z'))||
; 18   :  native.image_path[1]!=L':'||native.image_path[2]!=L'\\')return ERROR_INVALID_NAME;

	mov	eax, 123				; 0000007bH
	jmp	$LN94@ps_image_o
$LN9@ps_image_o:

; 13   :  unsigned char block[1024],digest[32];size_t i;HANDLE f;
; 14   :  if(native.image||native.image_ready)return ERROR_INVALID_STATE;

	mov	eax, 5023				; 0000139fH
	jmp	$LN1@ps_image_o
ps_image_open ENDP

ps_install_virtual PROC					; COMDAT

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
	je	$LN3@ps_install
	cmp	QWORD PTR native+8, 0
	jne	$LN3@ps_install
	cmp	QWORD PTR native+32, 0
	je	$LN3@ps_install
	cmp	DWORD PTR native+92, 0
	je	$LN3@ps_install
	cmp	DWORD PTR native+4276, 0
	jne	$LN3@ps_install

; 77   :  if(papacc_server_console_win32_stop_requested())return ERROR_OPERATION_ABORTED;

	call	papacc_server_console_win32_stop_requested
	test	eax, eax
	jne	$LN26@ps_install

; 78   :  if(!GetFileInformationByHandle(native.image,&current))return ps_last_error();

	mov	rcx, QWORD PTR native+32
	lea	rdx, QWORD PTR current$[rsp]
	call	QWORD PTR __imp_GetFileInformationByHandle
	test	eax, eax
	jne	SHORT $LN5@ps_install
$LN30@ps_install:

; 91   :  return ERROR_SUCCESS;
; 92   : }

	call	QWORD PTR __imp_GetLastError
	test	eax, eax
	mov	ecx, 31
	cmovne	ecx, eax
	mov	eax, ecx
	mov	rcx, QWORD PTR __$ArrayPad$[rsp]
	xor	rcx, rsp
	call	__security_check_cookie
	add	rsp, 200				; 000000c8H
	ret	0
$LN5@ps_install:

; 6    :  return a->dwVolumeSerialNumber==b->dwVolumeSerialNumber&&a->nFileIndexHigh==b->nFileIndexHigh&&

	mov	eax, DWORD PTR current$[rsp+28]
	cmp	DWORD PTR native+68, eax
	jne	$LN19@ps_install
	mov	eax, DWORD PTR current$[rsp+44]
	cmp	DWORD PTR native+84, eax
	jne	$LN19@ps_install
	mov	eax, DWORD PTR current$[rsp+48]
	cmp	DWORD PTR native+88, eax
	jne	$LN19@ps_install
	mov	eax, DWORD PTR current$[rsp+32]
	cmp	DWORD PTR native+72, eax
	jne	$LN19@ps_install
	mov	eax, DWORD PTR current$[rsp+36]
	cmp	DWORD PTR native+76, eax
	jne	$LN19@ps_install
	mov	eax, DWORD PTR current$[rsp+24]
	cmp	DWORD PTR native+64, eax
	jne	$LN19@ps_install
	mov	eax, DWORD PTR current$[rsp+20]
	cmp	DWORD PTR native+60, eax
	jne	$LN19@ps_install

; 80   :  native.install_effect=1U; /* REQUESTED: an uncertain Create is never retried. */

	xor	ecx, ecx
	mov	DWORD PTR native+4276, 1

; 81   :  native.service=CreateServiceW(native.manager,ps_name,ps_name,SERVICE_QUERY_CONFIG|SERVICE_CHANGE_CONFIG|SERVICE_QUERY_STATUS,

	mov	QWORD PTR [rsp+96], rcx
	lea	rax, OFFSET FLAT:ps_account
	mov	QWORD PTR [rsp+88], rax
	lea	r8, OFFSET FLAT:ps_name
	mov	QWORD PTR [rsp+80], rcx
	lea	rax, OFFSET FLAT:native+2144
	mov	QWORD PTR [rsp+72], rcx
	lea	rdx, OFFSET FLAT:ps_name
	mov	QWORD PTR [rsp+64], rcx
	mov	r9d, 7
	mov	rcx, QWORD PTR native
	mov	QWORD PTR [rsp+56], rax
	mov	DWORD PTR [rsp+48], 1
	mov	DWORD PTR [rsp+40], 3
	mov	DWORD PTR [rsp+32], 16
	call	QWORD PTR __imp_CreateServiceW
	mov	QWORD PTR native+8, rax
	test	rax, rax

; 83   :  if(!native.service){e=ps_last_error();native.install_disposition=e==ERROR_SERVICE_EXISTS||e==ERROR_DUPLICATE_SERVICE_NAME||e==ERROR_SERVICE_MARKED_FOR_DELETE?1U:2U;return e;}

	jne	SHORT $LN7@ps_install
; File C:\Projetos\PapinhoAccelerator\apps\server\server_service_win32.c

; 21   : static DWORD ps_last_error(void){DWORD e=GetLastError();return e?e:ERROR_GEN_FAILURE;}

	call	QWORD PTR __imp_GetLastError
	test	eax, eax
	mov	ecx, 31
	cmovne	ecx, eax
; File C:\Projetos\PapinhoAccelerator\apps\server\server_service_virtual_win32.inc

; 83   :  if(!native.service){e=ps_last_error();native.install_disposition=e==ERROR_SERVICE_EXISTS||e==ERROR_DUPLICATE_SERVICE_NAME||e==ERROR_SERVICE_MARKED_FOR_DELETE?1U:2U;return e;}

	lea	eax, DWORD PTR [rcx-1072]
	cmp	eax, 1
	jbe	SHORT $LN13@ps_install
	mov	DWORD PTR native+4280, 2
	cmp	ecx, 1078				; 00000436H
	jne	SHORT $LN14@ps_install
$LN13@ps_install:
	mov	DWORD PTR native+4280, 1
$LN14@ps_install:
	mov	eax, ecx

; 91   :  return ERROR_SUCCESS;
; 92   : }

	mov	rcx, QWORD PTR __$ArrayPad$[rsp]
	xor	rcx, rsp
	call	__security_check_cookie
	add	rsp, 200				; 000000c8H
	ret	0
$LN7@ps_install:

; 84   :  native.install_disposition=3U; /* Created, not configured/finalized runtime. */

	mov	DWORD PTR native+4280, 3

; 85   :  if(papacc_server_console_win32_stop_requested())return ERROR_OPERATION_ABORTED;

	call	papacc_server_console_win32_stop_requested
	test	eax, eax
	jne	SHORT $LN26@ps_install

; 86   :  info.dwServiceSidType=SERVICE_SID_TYPE_UNRESTRICTED;
; 87   :  if(!ChangeServiceConfig2W(native.service,SERVICE_CONFIG_SERVICE_SID_INFO,&info))return ps_last_error();

	mov	rcx, QWORD PTR native+8
	lea	r8, QWORD PTR info$[rsp]
	mov	edx, 5
	mov	DWORD PTR info$[rsp], 1
	call	QWORD PTR __imp_ChangeServiceConfig2W
	test	eax, eax
	je	$LN30@ps_install

; 88   :  e=ps_virtual_readback();if(e)return e;

	call	ps_virtual_readback
	test	eax, eax
	jne	SHORT $LN1@ps_install

; 89   :  if(papacc_server_console_win32_stop_requested())return ERROR_OPERATION_ABORTED;

	call	papacc_server_console_win32_stop_requested
	test	eax, eax
	jne	SHORT $LN26@ps_install

; 90   :  native.install_disposition=4U; /* Configuration confirmed, NOT token/ACL/readiness. */

	mov	DWORD PTR native+4280, 4

; 91   :  return ERROR_SUCCESS;
; 92   : }

	mov	rcx, QWORD PTR __$ArrayPad$[rsp]
	xor	rcx, rsp
	call	__security_check_cookie
	add	rsp, 200				; 000000c8H
	ret	0
$LN19@ps_install:

; 79   :  if(!ps_image_same(&native.image_id,&current))return ERROR_INVALID_DATA;

	mov	eax, 13

; 91   :  return ERROR_SUCCESS;
; 92   : }

	mov	rcx, QWORD PTR __$ArrayPad$[rsp]
	xor	rcx, rsp
	call	__security_check_cookie
	add	rsp, 200				; 000000c8H
	ret	0
$LN26@ps_install:

; 89   :  if(papacc_server_console_win32_stop_requested())return ERROR_OPERATION_ABORTED;

	mov	eax, 995				; 000003e3H

; 91   :  return ERROR_SUCCESS;
; 92   : }

	mov	rcx, QWORD PTR __$ArrayPad$[rsp]
	xor	rcx, rsp
	call	__security_check_cookie
	add	rsp, 200				; 000000c8H
	ret	0
$LN3@ps_install:

; 73   :  SERVICE_SID_INFO info;DWORD e;BY_HANDLE_FILE_INFORMATION current;
; 74   :  /* Caller must first prove configuration/target/authorization via existing
; 75   :   * gate. This private primitive is independently tested with modeled APIs. */
; 76   :  if(!native.manager||native.service||!native.image||!native.image_ready||native.install_effect)return ERROR_INVALID_STATE;

	mov	eax, 5023				; 0000139fH
$LN1@ps_install:

; 91   :  return ERROR_SUCCESS;
; 92   : }

	mov	rcx, QWORD PTR __$ArrayPad$[rsp]
	xor	rcx, rsp
	call	__security_check_cookie
	add	rsp, 200				; 000000c8H
	ret	0
ps_install_virtual ENDP