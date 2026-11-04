	TITLE	C:\Projetos\PapinhoBrowser\src\papinho_pst_owner.c
	.386P
include listing.inc
if @Version gt 510
.model FLAT
else
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT DWORD USE32 PUBLIC 'BSS'
_BSS	ENDS
_TLS	SEGMENT DWORD USE32 PUBLIC 'TLS'
_TLS	ENDS
;	COMDAT ??_C@_0P@JIHP@retrozilla?9nss?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT _owner_finish_dependencies
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _papinho_pst_owner_registry_init
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _papinho_pst_owner_create
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _papinho_pst_owner_callback_allowed
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _papinho_pst_owner_remove_membership
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _papinho_pst_owner_close
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _papinho_pst_owner_reclaim
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _papinho_pst_owner_registry_dispose
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _papinho_pst_owner_provider_snapshot
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	_papinho_pst_owner_registry_init
EXTRN	__imp__InitializeCriticalSection@4:NEAR
;	COMDAT _papinho_pst_owner_registry_init
_TEXT	SEGMENT
_r$ = 8
_papinho_pst_owner_registry_init PROC NEAR		; COMDAT

; 29   :     ZeroMemory(r, sizeof(*r));

	mov	edx, DWORD PTR _r$[esp-4]
	push	edi
	mov	ecx, 8
	xor	eax, eax
	mov	edi, edx

; 30   :     InitializeCriticalSection(&r->lock);

	push	edx
	rep stosd
	call	DWORD PTR __imp__InitializeCriticalSection@4
	pop	edi

; 31   : }

	ret	0
_papinho_pst_owner_registry_init ENDP
_TEXT	ENDS
PUBLIC	_papinho_pst_owner_create
EXTRN	_WSAStartup@8:NEAR
EXTRN	__imp__GetCurrentThreadId@0:NEAR
EXTRN	__imp__EnterCriticalSection@4:NEAR
EXTRN	__imp__LeaveCriticalSection@4:NEAR
EXTRN	_pst_release_info_init:NEAR
EXTRN	_pst_connection_try_release:NEAR
EXTRN	_pst_wait_set_remove_connection:NEAR
EXTRN	__imp__calloc:NEAR
EXTRN	__imp__free:NEAR
;	COMDAT _papinho_pst_owner_create
_TEXT	SEGMENT
_r$ = 8
_connection$ = 12
_identity$ = 16
_generation$ = 20
_role$ = 24
_dependencies$ = 28
_api$ = 32
_data$ = -400
_papinho_pst_owner_create PROC NEAR			; COMDAT

; 36   :     const papinho_pst_owner_api *api) {

	sub	esp, 400				; 00000190H
	push	ebx
	push	ebp

; 37   :     papinho_pst_owner *o, *p;
; 38   :     WSADATA data;
; 39   :     if (!r || !identity || !generation || !dependencies)

	mov	ebp, DWORD PTR _r$[esp+404]
	push	esi
	test	ebp, ebp
	push	edi
	je	$L18896
	mov	edi, DWORD PTR _identity$[esp+412]
	test	edi, edi
	je	$L18896
	mov	eax, DWORD PTR _generation$[esp+412]
	test	eax, eax
	je	$L18896
	mov	esi, DWORD PTR _dependencies$[esp+412]
	test	esi, esi
	je	$L18896

; 40   :         return NULL;
; 41   :     EnterCriticalSection(&r->lock);

	push	ebp
	call	DWORD PTR __imp__EnterCriticalSection@4

; 42   :     if (r->admission_closed) { LeaveCriticalSection(&r->lock); return NULL; }

	mov	eax, DWORD PTR [ebp+28]
	test	eax, eax
	jne	$L19056

; 43   :     for (p = r->head; p; p = p->next) {

	mov	eax, DWORD PTR [ebp+24]
	test	eax, eax
	je	SHORT $L18902
$L18900:

; 44   :         /* Tombstones prevent token reuse even after finalization. */
; 45   :         if (p->identity == identity) { LeaveCriticalSection(&r->lock); return NULL; }

	cmp	DWORD PTR [eax+52], edi
	je	$L19056
	mov	eax, DWORD PTR [eax]
	test	eax, eax
	jne	SHORT $L18900
$L18902:

; 46   :     }
; 47   :     o = (papinho_pst_owner *)calloc(1, sizeof(*o));

	mov	edi, 1
	push	148					; 00000094H
	push	edi
	call	DWORD PTR __imp__calloc
	mov	ebx, eax
	add	esp, 8

; 48   :     if (!o) { LeaveCriticalSection(&r->lock); return NULL; }

	test	ebx, ebx
	je	$L19056

; 49   :     o->api.release = api ? api->release : pst_connection_try_release;

	mov	ecx, DWORD PTR _api$[esp+412]
	test	ecx, ecx
	je	SHORT $L19051
	mov	eax, DWORD PTR [ecx]
	jmp	SHORT $L19052
$L19051:
	mov	eax, OFFSET FLAT:_pst_connection_try_release
$L19052:

; 50   :     o->api.remove = api ? api->remove : pst_wait_set_remove_connection;

	test	ecx, ecx
	mov	DWORD PTR [ebx+40], eax
	je	SHORT $L19053
	mov	ecx, DWORD PTR [ecx+4]
	jmp	SHORT $L19054
$L19053:
	mov	ecx, OFFSET FLAT:_pst_wait_set_remove_connection
$L19054:

; 51   :     if (!o->api.release || !o->api.remove ||
; 52   :         pst_release_info_init(&o->release_info, sizeof(o->release_info)) != PST_RESULT_OK) {

	test	eax, eax
	mov	DWORD PTR [ebx+44], ecx
	je	$L18910
	test	ecx, ecx
	je	$L18910
	lea	eax, DWORD PTR [ebx+92]
	push	28					; 0000001cH
	push	eax
	call	_pst_release_info_init
	add	esp, 8
	test	eax, eax
	jne	SHORT $L18910

; 54   :     }
; 55   :     if (WSAStartup(MAKEWORD(1,1), &data) != 0) {

	lea	ecx, DWORD PTR _data$[esp+416]
	push	ecx
	push	257					; 00000101H
	call	_WSAStartup@8
	test	eax, eax

; 56   :         free(o); LeaveCriticalSection(&r->lock); return NULL;

	jne	SHORT $L18910

; 57   :     }
; 58   :     o->winsock_owned = 1;
; 59   :     o->release_eligible = 1;
; 60   :     o->registry = r; o->connection = connection; o->dependencies = *dependencies;

	mov	edx, DWORD PTR _connection$[esp+412]

; 61   :     o->identity = identity; o->generation = generation; o->role = role;

	mov	eax, DWORD PTR _identity$[esp+412]
	mov	DWORD PTR [ebx+76], edi
	mov	DWORD PTR [ebx+80], edi
	lea	edi, DWORD PTR [ebx+16]
	mov	ecx, 6
	mov	DWORD PTR [ebx+4], ebp
	mov	DWORD PTR [ebx+8], edx
	mov	edx, DWORD PTR _role$[esp+412]
	rep movsd
	mov	ecx, DWORD PTR _generation$[esp+412]
	mov	DWORD PTR [ebx+52], eax
	mov	DWORD PTR [ebx+56], ecx
	mov	DWORD PTR [ebx+60], edx

; 62   :     o->thread_id = GetCurrentThreadId();

	call	DWORD PTR __imp__GetCurrentThreadId@0
	mov	DWORD PTR [ebx+48], eax

; 63   : 
; 64   :     o->next = r->head; r->head = o;

	mov	eax, DWORD PTR [ebp+24]
	mov	DWORD PTR [ebx], eax

; 65   :     LeaveCriticalSection(&r->lock);

	push	ebp
	mov	DWORD PTR [ebp+24], ebx
	call	DWORD PTR __imp__LeaveCriticalSection@4
	pop	edi
	pop	esi

; 66   :     return o;

	mov	eax, ebx
	pop	ebp
	pop	ebx

; 67   : }

	add	esp, 400				; 00000190H
	ret	0
$L18910:

; 53   :         free(o); LeaveCriticalSection(&r->lock); return NULL;

	push	ebx
	call	DWORD PTR __imp__free
	add	esp, 4
$L19056:
	push	ebp
	call	DWORD PTR __imp__LeaveCriticalSection@4
$L18896:
	pop	edi
	pop	esi
	pop	ebp
	xor	eax, eax
	pop	ebx

; 67   : }

	add	esp, 400				; 00000190H
	ret	0
_papinho_pst_owner_create ENDP
_TEXT	ENDS
PUBLIC	_papinho_pst_owner_callback_allowed
;	COMDAT _papinho_pst_owner_callback_allowed
_TEXT	SEGMENT
_o$ = 8
_identity$ = 12
_generation$ = 16
_papinho_pst_owner_callback_allowed PROC NEAR		; COMDAT

; 70   :     unsigned long identity, unsigned long generation) {

	push	esi

; 71   :     int allowed;
; 72   :     if (!o) return 0;

	mov	esi, DWORD PTR _o$[esp]
	test	esi, esi
	jne	SHORT $L18926
	xor	eax, eax
	pop	esi

; 80   : }

	ret	0
$L18926:

; 73   :     EnterCriticalSection(&o->registry->lock);

	mov	eax, DWORD PTR [esi+4]
	push	edi
	push	eax
	call	DWORD PTR __imp__EnterCriticalSection@4

; 74   :     allowed = o->identity == identity && o->generation == generation &&
; 75   :         !o->registry->admission_closed &&
; 76   :         !o->closing && !o->finalized && !o->terminal_retained &&
; 77   :         o->thread_id == GetCurrentThreadId();

	mov	ecx, DWORD PTR [esi+52]
	mov	eax, DWORD PTR _identity$[esp+4]
	cmp	ecx, eax
	jne	SHORT $L19061
	mov	edx, DWORD PTR [esi+56]
	mov	eax, DWORD PTR _generation$[esp+4]
	cmp	edx, eax
	jne	SHORT $L19061
	mov	eax, DWORD PTR [esi+4]
	mov	ecx, DWORD PTR [eax+28]
	test	ecx, ecx
	jne	SHORT $L19061
	mov	eax, DWORD PTR [esi+64]
	test	eax, eax
	jne	SHORT $L19061
	mov	eax, DWORD PTR [esi+68]
	test	eax, eax
	jne	SHORT $L19061
	mov	eax, DWORD PTR [esi+72]
	test	eax, eax
	jne	SHORT $L19061
	call	DWORD PTR __imp__GetCurrentThreadId@0
	cmp	DWORD PTR [esi+48], eax
	jne	SHORT $L19061

; 78   :     LeaveCriticalSection(&o->registry->lock);

	mov	ecx, DWORD PTR [esi+4]
	mov	edi, 1
	push	ecx
	call	DWORD PTR __imp__LeaveCriticalSection@4

; 79   :     return allowed;

	mov	eax, edi
	pop	edi
	pop	esi

; 80   : }

	ret	0
$L19061:

; 78   :     LeaveCriticalSection(&o->registry->lock);

	mov	ecx, DWORD PTR [esi+4]
	xor	edi, edi
	push	ecx
	call	DWORD PTR __imp__LeaveCriticalSection@4

; 79   :     return allowed;

	mov	eax, edi
	pop	edi
	pop	esi

; 80   : }

	ret	0
_papinho_pst_owner_callback_allowed ENDP
_TEXT	ENDS
PUBLIC	_papinho_pst_owner_remove_membership
;	COMDAT _papinho_pst_owner_remove_membership
_TEXT	SEGMENT
_o$ = 8
_papinho_pst_owner_remove_membership PROC NEAR		; COMDAT

; 82   : int papinho_pst_owner_remove_membership(papinho_pst_owner *o) {

	push	esi

; 83   :     PST_RESULT rc;
; 84   :     if (!o || o->thread_id != GetCurrentThreadId() ||
; 85   :         o->terminal_retained || o->finalized) return 0;

	mov	esi, DWORD PTR _o$[esp]
	test	esi, esi
	je	SHORT $L18932
	call	DWORD PTR __imp__GetCurrentThreadId@0
	cmp	DWORD PTR [esi+48], eax
	jne	SHORT $L18932
	mov	eax, DWORD PTR [esi+72]
	test	eax, eax
	jne	SHORT $L18932
	mov	eax, DWORD PTR [esi+68]
	test	eax, eax
	jne	SHORT $L18932

; 86   :     if (!o->wait_set) return 1;

	mov	eax, DWORD PTR [esi+12]
	test	eax, eax
	jne	SHORT $L18933
	mov	eax, 1
	pop	esi

; 92   :     return 1;
; 93   : }

	ret	0
$L18933:

; 87   :     rc = o->api.remove(o->wait_set, o->connection);

	mov	ecx, DWORD PTR [esi+8]
	push	ecx
	push	eax
	call	DWORD PTR [esi+44]
	add	esp, 8

; 88   :     o->preclose_result = rc;

	mov	DWORD PTR [esi+120], eax

; 89   :     if (rc != PST_RESULT_OK) return 0;

	test	eax, eax
	jne	SHORT $L18932

; 90   :     o->wait_set = NULL;

	mov	DWORD PTR [esi+12], eax

; 91   :     o->release_eligible = 1; /* factual owner-thread precondition correction */

	mov	eax, 1
	mov	DWORD PTR [esi+80], eax
	pop	esi

; 92   :     return 1;
; 93   : }

	ret	0
$L18932:

; 83   :     PST_RESULT rc;
; 84   :     if (!o || o->thread_id != GetCurrentThreadId() ||
; 85   :         o->terminal_retained || o->finalized) return 0;

	xor	eax, eax
	pop	esi

; 92   :     return 1;
; 93   : }

	ret	0
_papinho_pst_owner_remove_membership ENDP
_TEXT	ENDS
PUBLIC	_papinho_pst_owner_close
;	COMDAT _papinho_pst_owner_close
_TEXT	SEGMENT
_o$ = 8
_identity$ = 12
_generation$ = 16
_papinho_pst_owner_close PROC NEAR			; COMDAT

; 96   :     unsigned long identity, unsigned long generation) {

	push	ebx
	push	esi

; 97   :     PST_RESULT result;
; 98   :     papinho_pst_owner_registry *r;
; 99   :     if (!o) return 0;

	mov	esi, DWORD PTR _o$[esp+4]
	push	edi
	test	esi, esi
	je	$L19068

; 100  :     r = o->registry;

	mov	ebx, DWORD PTR [esi+4]

; 101  :     EnterCriticalSection(&r->lock);

	push	ebx
	call	DWORD PTR __imp__EnterCriticalSection@4

; 102  :     if (o->identity != identity || o->generation != generation ||
; 103  :         o->thread_id != GetCurrentThreadId()) {

	mov	eax, DWORD PTR [esi+52]
	mov	ecx, DWORD PTR _identity$[esp+8]
	cmp	eax, ecx
	jne	$L18964
	mov	ecx, DWORD PTR [esi+56]
	mov	eax, DWORD PTR _generation$[esp+8]
	cmp	ecx, eax
	jne	$L18964
	call	DWORD PTR __imp__GetCurrentThreadId@0
	cmp	DWORD PTR [esi+48], eax
	jne	$L18964

; 105  :     }
; 106  :     if (o->finalized) { LeaveCriticalSection(&r->lock); return 1; }

	mov	eax, DWORD PTR [esi+68]
	test	eax, eax
	je	SHORT $L18949
	push	ebx

; 129  :         LeaveCriticalSection(&r->lock); return 1;

	call	DWORD PTR __imp__LeaveCriticalSection@4
	pop	edi
	pop	esi
	mov	eax, 1
	pop	ebx

; 138  :     }
; 139  :     LeaveCriticalSection(&r->lock);
; 140  :     return 0;
; 141  : }

	ret	0
$L18949:

; 107  :     if (o->terminal_retained) { LeaveCriticalSection(&r->lock); return 0; }

	mov	eax, DWORD PTR [esi+72]
	test	eax, eax
	jne	$L18964

; 108  :     o->closing = 1;
; 109  :     if (!o->quiescence_confirmed) { LeaveCriticalSection(&r->lock); return 0; }

	mov	eax, DWORD PTR [esi+124]
	mov	DWORD PTR [esi+64], 1
	test	eax, eax
	je	$L18964

; 110  :     /* PRE_CLOSE failure is not a fabricated PST release disposition. */
; 111  :     if (!papinho_pst_owner_remove_membership(o)) {

	push	esi
	call	_papinho_pst_owner_remove_membership
	add	esp, 4
	test	eax, eax

; 112  :         LeaveCriticalSection(&r->lock); return 0;

	je	$L18964

; 113  :     }
; 114  :     if (!o->release_eligible) { LeaveCriticalSection(&r->lock); return 0; }

	mov	eax, DWORD PTR [esi+80]
	test	eax, eax
	je	$L18964

; 115  :     pst_release_info_init(&o->release_info, sizeof(o->release_info));

	lea	edi, DWORD PTR [esi+92]
	push	28					; 0000001cH
	push	edi
	call	_pst_release_info_init

; 116  :     ++o->release_attempts;

	mov	eax, DWORD PTR [esi+84]
	add	esp, 8
	inc	eax
	mov	DWORD PTR [esi+84], eax
	mov	ecx, eax

; 117  :     if (o->connection == NULL) {

	mov	eax, DWORD PTR [esi+8]
	test	eax, eax
	jne	SHORT $L18955

; 118  :         /* Reserved owner whose PST creation never succeeded. No PST destruction
; 119  :          * is claimed: NONE remains the release disposition and attempts is zero. */
; 120  :         --o->release_attempts;

	dec	ecx
	mov	DWORD PTR [esi+84], ecx

; 121  :         if (!owner_finish_dependencies(o)) { LeaveCriticalSection(&r->lock); return 0; }
; 122  :         LeaveCriticalSection(&r->lock); return 1;

	jmp	SHORT $L19070
$L18955:

; 123  :     }
; 124  :     result = o->api.release(o->connection, &o->release_info);

	push	edi
	push	eax
	call	DWORD PTR [esi+40]
	add	esp, 8

; 125  :     if (result == PST_RESULT_OK && o->release_info.result == result &&
; 126  :         o->release_info.disposition == PST_RELEASE_DISPOSITION_DESTROYED) {

	test	eax, eax
	jne	SHORT $L18958
	mov	eax, DWORD PTR [esi+104]
	test	eax, eax
	jne	SHORT $L19067
	cmp	DWORD PTR [esi+100], 2
	jne	SHORT $L19067

; 127  :         o->connection = NULL;

	mov	DWORD PTR [esi+8], 0
$L19070:

; 128  :         if (!owner_finish_dependencies(o)) { LeaveCriticalSection(&r->lock); return 0; }

	push	esi
	call	_owner_finish_dependencies
	add	esp, 4
	test	eax, eax
	push	ebx
	je	SHORT $L19069

; 129  :         LeaveCriticalSection(&r->lock); return 1;

	call	DWORD PTR __imp__LeaveCriticalSection@4
	pop	edi
	pop	esi
	mov	eax, 1
	pop	ebx

; 138  :     }
; 139  :     LeaveCriticalSection(&r->lock);
; 140  :     return 0;
; 141  : }

	ret	0
$L18958:

; 130  :     }
; 131  :     if (result != PST_RESULT_INVALID_STATE ||
; 132  :         o->release_info.result != result ||
; 133  :         o->release_info.disposition != PST_RELEASE_DISPOSITION_PRECONDITION_RETAINED) {

	cmp	eax, 2
	jne	SHORT $L19067
	cmp	DWORD PTR [esi+104], 2
	jne	SHORT $L19067
	mov	ecx, DWORD PTR [esi+100]
	mov	eax, 1
	cmp	ecx, eax
	jne	SHORT $L18963
	push	ebx

; 136  :     } else {
; 137  :         o->release_eligible = 0;

	mov	DWORD PTR [esi+80], 0
	call	DWORD PTR __imp__LeaveCriticalSection@4
	pop	edi
	pop	esi
	xor	eax, eax
	pop	ebx

; 138  :     }
; 139  :     LeaveCriticalSection(&r->lock);
; 140  :     return 0;
; 141  : }

	ret	0
$L19067:

; 130  :     }
; 131  :     if (result != PST_RESULT_INVALID_STATE ||
; 132  :         o->release_info.result != result ||
; 133  :         o->release_info.disposition != PST_RELEASE_DISPOSITION_PRECONDITION_RETAINED) {

	mov	eax, 1
$L18963:

; 134  :         o->terminal_retained = 1;

	mov	DWORD PTR [esi+72], eax

; 135  :         r->admission_closed = 1;

	mov	DWORD PTR [ebx+28], eax
$L18964:

; 104  :         LeaveCriticalSection(&r->lock); return 0;

	push	ebx
$L19069:
	call	DWORD PTR __imp__LeaveCriticalSection@4
$L19068:
	pop	edi
	pop	esi
	xor	eax, eax
	pop	ebx

; 138  :     }
; 139  :     LeaveCriticalSection(&r->lock);
; 140  :     return 0;
; 141  : }

	ret	0
_papinho_pst_owner_close ENDP
_TEXT	ENDS
EXTRN	_WSACleanup@0:NEAR
EXTRN	_WSAGetLastError@0:NEAR
;	COMDAT _owner_finish_dependencies
_TEXT	SEGMENT
_o$ = 8
_owner_finish_dependencies PROC NEAR			; COMDAT

; 6    : static int owner_finish_dependencies(papinho_pst_owner *o) {

	push	ebx
	push	esi

; 7    :     if (o->winsock_owned) {

	mov	esi, DWORD PTR _o$[esp+4]
	mov	ebx, 1
	mov	eax, DWORD PTR [esi+76]
	test	eax, eax
	je	SHORT $L18870

; 8    :         if (o->winsock_cleanup_attempted) return 0;

	mov	eax, DWORD PTR [esi+128]
	test	eax, eax
	je	SHORT $L18871
	pop	esi
	xor	eax, eax
	pop	ebx

; 27   : }

	ret	0
$L18871:

; 9    :         o->winsock_cleanup_attempted = 1;

	mov	DWORD PTR [esi+128], ebx

; 10   :         o->winsock_cleanup_result = WSACleanup();

	call	_WSACleanup@0

; 11   :         if (o->winsock_cleanup_result != 0) {

	test	eax, eax
	mov	DWORD PTR [esi+132], eax
	je	SHORT $L18872

; 12   :             o->winsock_cleanup_error = WSAGetLastError();

	call	_WSAGetLastError@0
	mov	DWORD PTR [esi+136], eax

; 13   :             o->terminal_retained = 1; /* Browser retention, not a PST disposition */
; 14   :             o->registry->admission_closed = 1;

	mov	eax, DWORD PTR [esi+4]
	mov	DWORD PTR [esi+72], ebx
	pop	esi
	mov	DWORD PTR [eax+28], ebx

; 15   :             return 0;

	xor	eax, eax
	pop	ebx

; 27   : }

	ret	0
$L18872:

; 16   :         }
; 17   :         o->winsock_owned = 0;

	mov	DWORD PTR [esi+76], 0
$L18870:

; 18   :     }
; 19   :     /* D's callback only drops this owner's group lease; it does not destroy
; 20   :      * the group or its resources. Keep that lease on WinSock cleanup failure. */
; 21   :     if (o->dependencies.finalize)

	mov	eax, DWORD PTR [esi+36]
	test	eax, eax
	je	SHORT $L18873

; 22   :         o->dependencies.finalize(o->dependencies.context);

	mov	ecx, DWORD PTR [esi+32]
	push	ecx
	call	eax
	add	esp, 4
$L18873:
	push	edi

; 23   :     ZeroMemory(&o->dependencies, sizeof(o->dependencies));

	mov	ecx, 6
	xor	eax, eax
	lea	edi, DWORD PTR [esi+16]
	rep stosd

; 24   :     o->finalized = 1;
; 25   :     ++o->finalizations;

	mov	eax, DWORD PTR [esi+88]
	mov	DWORD PTR [esi+68], ebx
	inc	eax
	pop	edi
	mov	DWORD PTR [esi+88], eax

; 26   :     return 1;

	mov	eax, ebx
	pop	esi
	pop	ebx

; 27   : }

	ret	0
_owner_finish_dependencies ENDP
_TEXT	ENDS
PUBLIC	_papinho_pst_owner_reclaim
;	COMDAT _papinho_pst_owner_reclaim
_TEXT	SEGMENT
_o$ = 8
_papinho_pst_owner_reclaim PROC NEAR			; COMDAT

; 143  : int papinho_pst_owner_reclaim(papinho_pst_owner *o) {

	push	esi

; 144  :     papinho_pst_owner_registry *r;
; 145  :     papinho_pst_owner **link;
; 146  :     if (!o || !o->finalized || o->thread_id != GetCurrentThreadId()) return 0;

	mov	esi, DWORD PTR _o$[esp]
	test	esi, esi
	push	edi
	je	SHORT $L18971
	mov	eax, DWORD PTR [esi+68]
	test	eax, eax
	je	SHORT $L18971
	call	DWORD PTR __imp__GetCurrentThreadId@0
	cmp	DWORD PTR [esi+48], eax
	jne	SHORT $L18971

; 147  :     r = o->registry;

	mov	edi, DWORD PTR [esi+4]

; 148  :     EnterCriticalSection(&r->lock);

	push	edi
	call	DWORD PTR __imp__EnterCriticalSection@4

; 149  :     for (link = &r->head; *link && *link != o; link = &(*link)->next) {}

	mov	eax, DWORD PTR [edi+24]
	lea	ecx, DWORD PTR [edi+24]
	test	eax, eax
	je	SHORT $L18974
$L18972:
	cmp	eax, esi
	je	SHORT $L18974
	mov	ecx, eax
	mov	eax, DWORD PTR [ecx]
	test	eax, eax
	jne	SHORT $L18972
$L18974:

; 150  :     if (*link != o) { LeaveCriticalSection(&r->lock); return 0; }

	cmp	DWORD PTR [ecx], esi
	je	SHORT $L18975
	push	edi
	call	DWORD PTR __imp__LeaveCriticalSection@4
$L18971:
	pop	edi
	xor	eax, eax
	pop	esi

; 155  : }

	ret	0
$L18975:

; 151  :     *link = o->next;

	mov	eax, DWORD PTR [esi]

; 152  :     LeaveCriticalSection(&r->lock);

	push	edi
	mov	DWORD PTR [ecx], eax
	call	DWORD PTR __imp__LeaveCriticalSection@4

; 153  :     free(o);

	push	esi
	call	DWORD PTR __imp__free
	add	esp, 4

; 154  :     return 1;

	mov	eax, 1
	pop	edi
	pop	esi

; 155  : }

	ret	0
_papinho_pst_owner_reclaim ENDP
_TEXT	ENDS
PUBLIC	_papinho_pst_owner_registry_dispose
EXTRN	__imp__DeleteCriticalSection@4:NEAR
;	COMDAT _papinho_pst_owner_registry_dispose
_TEXT	SEGMENT
_r$ = 8
_papinho_pst_owner_registry_dispose PROC NEAR		; COMDAT

; 157  : int papinho_pst_owner_registry_dispose(papinho_pst_owner_registry *r) {

	push	ebx

; 158  :     papinho_pst_owner *o, *next;
; 159  :     if (!r) return 0;

	mov	ebx, DWORD PTR _r$[esp]
	test	ebx, ebx
	push	esi
	jne	SHORT $L18981
	pop	esi
	xor	eax, eax
	pop	ebx

; 170  : }

	ret	0
$L18981:

; 160  :     EnterCriticalSection(&r->lock);

	push	ebx
	call	DWORD PTR __imp__EnterCriticalSection@4

; 161  :     for (o = r->head; o; o = o->next) {

	mov	esi, DWORD PTR [ebx+24]
	mov	eax, esi
	test	eax, eax
	je	SHORT $L18984
$L18982:

; 162  :         if (!o->finalized) { LeaveCriticalSection(&r->lock); return 0; }

	mov	ecx, DWORD PTR [eax+68]
	test	ecx, ecx
	je	SHORT $L19082
	mov	eax, DWORD PTR [eax]
	test	eax, eax
	jne	SHORT $L18982
$L18984:

; 163  :     }
; 164  :     o = r->head; r->head = NULL;
; 165  :     LeaveCriticalSection(&r->lock);

	push	ebx
	mov	DWORD PTR [ebx+24], 0
	call	DWORD PTR __imp__LeaveCriticalSection@4

; 166  :     while (o) { next = o->next; free(o); o = next; }

	test	esi, esi
	je	SHORT $L18989
	push	ebp
	mov	ebp, DWORD PTR __imp__free
	push	edi
$L18988:
	mov	edi, DWORD PTR [esi]
	push	esi
	call	ebp
	add	esp, 4
	mov	esi, edi
	test	edi, edi
	jne	SHORT $L18988
	pop	edi
	pop	ebp
$L18989:

; 167  :     /* Caller must have quiesced all callback sources before disposal. */
; 168  :     DeleteCriticalSection(&r->lock);

	push	ebx
	call	DWORD PTR __imp__DeleteCriticalSection@4
	pop	esi

; 169  :     return 1;

	mov	eax, 1
	pop	ebx

; 170  : }

	ret	0
$L19082:

; 162  :         if (!o->finalized) { LeaveCriticalSection(&r->lock); return 0; }

	push	ebx
	call	DWORD PTR __imp__LeaveCriticalSection@4
	pop	esi
	xor	eax, eax
	pop	ebx

; 170  : }

	ret	0
_papinho_pst_owner_registry_dispose ENDP
_TEXT	ENDS
PUBLIC	??_C@_0P@JIHP@retrozilla?9nss?$AA@		; `string'
PUBLIC	_papinho_pst_owner_provider_snapshot
EXTRN	_pst_connection_get_provider_info:NEAR
EXTRN	__imp__memchr:NEAR
;	COMDAT ??_C@_0P@JIHP@retrozilla?9nss?$AA@
; File C:\Projetos\PapinhoBrowser\src\papinho_pst_owner.c
_DATA	SEGMENT
??_C@_0P@JIHP@retrozilla?9nss?$AA@ DB 'retrozilla-nss', 00H ; `string'
_DATA	ENDS
;	COMDAT _papinho_pst_owner_provider_snapshot
_TEXT	SEGMENT
_o$ = 8
_identity$ = 12
_generation$ = 16
_out$ = 20
_f$ = -60
_rc$ = -64
_papinho_pst_owner_provider_snapshot PROC NEAR		; COMDAT

; 175  :  unsigned long identity,unsigned long generation,PST_PROVIDER_INFO *out) {

	sub	esp, 64					; 00000040H
	push	ebp
	push	esi

; 176  :  PST_PROVIDER_INFO f;PST_RESULT rc;papinho_pst_owner_registry *r;const pst_connection *connection;
; 177  :  size_t a=(size_t)o,z=(size_t)out,t;
; 178  :  if(!o||!out||!o->registry)return PST_RESULT_INVALID_ARGUMENT;

	mov	esi, DWORD PTR _o$[esp+68]
	push	edi
	test	esi, esi
	je	$L19013
	mov	eax, DWORD PTR _out$[esp+72]
	test	eax, eax
	je	$L19013
	mov	edi, DWORD PTR [esi+4]
	test	edi, edi
	mov	DWORD PTR 8+[esp+72], edi
	je	$L19013

; 179  :  r=o->registry;t=(size_t)r;
; 180  :  if((z>=a&&z-a<sizeof(*o))||(z<a&&a-z<sizeof(*out))||
; 181  :     (z>=t&&z-t<sizeof(*r))||(z<t&&t-z<sizeof(*out)))return PST_RESULT_INVALID_ARGUMENT;

	cmp	eax, esi
	jb	SHORT $L19087
	mov	ecx, eax
	sub	ecx, esi
	cmp	ecx, 148				; 00000094H
	jb	$L19013
	jmp	SHORT $L19015
$L19087:
	mov	edx, esi
	sub	edx, eax
	cmp	edx, 60					; 0000003cH
	jb	$L19013
$L19015:
	cmp	eax, edi
	jb	SHORT $L19088
	sub	eax, edi
	cmp	eax, 32					; 00000020H
	jb	$L19013
	jmp	SHORT $L19012
$L19088:
	mov	ecx, edi
	sub	ecx, eax
	cmp	ecx, 60					; 0000003cH
	jb	$L19013
$L19012:

; 182  :  EnterCriticalSection(&r->lock);

	push	edi
	call	DWORD PTR __imp__EnterCriticalSection@4

; 183  :  if(o->identity!=identity||o->generation!=generation||!identity||!generation||
; 184  :     r->admission_closed||o->closing||o->finalized||o->terminal_retained||
; 185  :     o->release_attempts||!o->connection||o->thread_id!=GetCurrentThreadId()) {

	mov	eax, DWORD PTR _identity$[esp+72]
	mov	ecx, DWORD PTR [esi+52]
	cmp	ecx, eax
	jne	$L19019
	mov	ecx, DWORD PTR _generation$[esp+72]
	mov	edx, DWORD PTR [esi+56]
	cmp	edx, ecx
	jne	$L19019
	test	eax, eax
	je	$L19019
	test	ecx, ecx
	je	$L19019
	mov	eax, DWORD PTR [edi+28]
	test	eax, eax
	jne	$L19019
	mov	eax, DWORD PTR [esi+64]
	test	eax, eax
	jne	$L19019
	mov	eax, DWORD PTR [esi+68]
	test	eax, eax
	jne	$L19019
	mov	eax, DWORD PTR [esi+72]
	test	eax, eax
	jne	$L19019
	mov	eax, DWORD PTR [esi+84]
	test	eax, eax
	jne	$L19019
	mov	eax, DWORD PTR [esi+8]
	test	eax, eax
	je	$L19019
	mov	ebp, DWORD PTR __imp__GetCurrentThreadId@0
	call	ebp
	cmp	DWORD PTR [esi+48], eax
	jne	$L19019
	push	ebx

; 187  :  }
; 188  :  connection=o->connection;ZeroMemory(&f,sizeof(f));f.struct_size=sizeof(f);f.api_version=PST_API_VERSION;

	mov	ebx, DWORD PTR [esi+8]

; 189  :  rc=pst_connection_get_provider_info(connection,&f);

	lea	edx, DWORD PTR _f$[esp+80]
	mov	ecx, 15					; 0000000fH
	xor	eax, eax
	lea	edi, DWORD PTR _f$[esp+80]
	push	edx
	push	ebx
	rep stosd
	mov	DWORD PTR _f$[esp+88], 60		; 0000003cH
	mov	DWORD PTR _f$[esp+92], 196608		; 00030000H
	call	_pst_connection_get_provider_info
	add	esp, 8
	mov	DWORD PTR _rc$[esp+80], eax

; 190  :  if(rc==PST_RESULT_OK&&(o->connection!=connection||o->closing||o->finalized||
; 191  :     o->terminal_retained||o->release_attempts||o->thread_id!=GetCurrentThreadId()||
; 192  :     o->registry!=r||r->admission_closed||o->identity!=identity||o->generation!=generation||
; 193  :     !memchr(f.provider_id,0,sizeof(f.provider_id))||strcmp(f.provider_id,"retrozilla-nss")||
; 194  :     f.available!=1U||f.initialized>1U))rc=PST_RESULT_INVALID_STATE;

	test	eax, eax
	jne	$L19027
	cmp	DWORD PTR [esi+8], ebx
	jne	$L19023
	mov	eax, DWORD PTR [esi+64]
	test	eax, eax
	jne	$L19023
	mov	eax, DWORD PTR [esi+68]
	test	eax, eax
	jne	$L19023
	mov	eax, DWORD PTR [esi+72]
	test	eax, eax
	jne	$L19023
	mov	eax, DWORD PTR [esi+84]
	test	eax, eax
	jne	$L19023
	call	ebp
	cmp	DWORD PTR [esi+48], eax
	jne	$L19023
	mov	eax, DWORD PTR 8+[esp+76]
	mov	ecx, DWORD PTR [esi+4]
	cmp	ecx, eax
	jne	$L19023
	mov	ecx, DWORD PTR [eax+28]
	test	ecx, ecx
	jne	SHORT $L19023
	mov	eax, DWORD PTR _identity$[esp+76]
	mov	ecx, DWORD PTR [esi+52]
	cmp	ecx, eax
	jne	SHORT $L19023
	mov	ecx, DWORD PTR _generation$[esp+76]
	mov	eax, DWORD PTR [esi+56]
	cmp	eax, ecx
	jne	SHORT $L19023
	push	32					; 00000020H
	lea	edx, DWORD PTR _f$[esp+104]
	push	0
	push	edx
	call	DWORD PTR __imp__memchr
	add	esp, 12					; 0000000cH
	test	eax, eax
	je	SHORT $L19023
	mov	esi, OFFSET FLAT:??_C@_0P@JIHP@retrozilla?9nss?$AA@ ; `string'
	lea	eax, DWORD PTR _f$[esp+100]
$L19090:
	mov	dl, BYTE PTR [eax]
	mov	bl, BYTE PTR [esi]
	mov	cl, dl
	cmp	dl, bl
	jne	SHORT $L19091
	test	cl, cl
	je	SHORT $L19092
	mov	dl, BYTE PTR [eax+1]
	mov	bl, BYTE PTR [esi+1]
	mov	cl, dl
	cmp	dl, bl
	jne	SHORT $L19091
	add	eax, 2
	add	esi, 2
	test	cl, cl
	jne	SHORT $L19090
$L19092:
	xor	eax, eax
	jmp	SHORT $L19093
$L19091:
	sbb	eax, eax
	sbb	eax, -1
$L19093:
	test	eax, eax
	jne	SHORT $L19023
	mov	ecx, DWORD PTR _f$[esp+88]
	mov	eax, 1
	cmp	ecx, eax
	jne	SHORT $L19023
	cmp	DWORD PTR _f$[esp+92], eax
	jbe	SHORT $L19089
$L19023:
	mov	DWORD PTR _rc$[esp+80], 2

; 195  :  if(rc==PST_RESULT_OK)*out=f;

	jmp	SHORT $L19027
$L19089:
	mov	edi, DWORD PTR _out$[esp+76]
	mov	ecx, 15					; 0000000fH
	lea	esi, DWORD PTR _f$[esp+80]
	rep movsd
$L19027:

; 196  :  LeaveCriticalSection(&r->lock);return rc;

	mov	eax, DWORD PTR 8+[esp+76]
	push	eax
	call	DWORD PTR __imp__LeaveCriticalSection@4
	mov	eax, DWORD PTR _rc$[esp+80]
	pop	ebx
	pop	edi
	pop	esi
	pop	ebp

; 197  : }

	add	esp, 64					; 00000040H
	ret	0
$L19019:

; 186  :   LeaveCriticalSection(&r->lock);return PST_RESULT_INVALID_STATE;

	push	edi
	call	DWORD PTR __imp__LeaveCriticalSection@4
	pop	edi
	pop	esi
	mov	eax, 2
	pop	ebp

; 197  : }

	add	esp, 64					; 00000040H
	ret	0
$L19013:
	pop	edi
	pop	esi

; 176  :  PST_PROVIDER_INFO f;PST_RESULT rc;papinho_pst_owner_registry *r;const pst_connection *connection;
; 177  :  size_t a=(size_t)o,z=(size_t)out,t;
; 178  :  if(!o||!out||!o->registry)return PST_RESULT_INVALID_ARGUMENT;

	mov	eax, 1
	pop	ebp

; 197  : }

	add	esp, 64					; 00000040H
	ret	0
_papinho_pst_owner_provider_snapshot ENDP
_TEXT	ENDS
END
