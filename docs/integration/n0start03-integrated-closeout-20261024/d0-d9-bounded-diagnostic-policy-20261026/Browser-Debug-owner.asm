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
$$SYMBOLS	SEGMENT BYTE USE32 'DEBSYM'
$$SYMBOLS	ENDS
$$TYPES	SEGMENT BYTE USE32 'DEBTYP'
$$TYPES	ENDS
_TLS	SEGMENT DWORD USE32 PUBLIC 'TLS'
_TLS	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	_papinho_pst_owner_registry_init
EXTRN	__imp__InitializeCriticalSection@4:NEAR
EXTRN	_memset:NEAR
_TEXT	SEGMENT
_r$ = 8
_papinho_pst_owner_registry_init PROC NEAR

; 28   : void papinho_pst_owner_registry_init(papinho_pst_owner_registry *r) {

	push	ebp
	mov	ebp, esp

; 29   :     ZeroMemory(r, sizeof(*r));

	push	32					; 00000020H
	push	0
	mov	eax, DWORD PTR _r$[ebp]
	push	eax
	call	_memset
	add	esp, 12					; 0000000cH

; 30   :     InitializeCriticalSection(&r->lock);

	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	call	DWORD PTR __imp__InitializeCriticalSection@4

; 31   : }

	pop	ebp
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
_TEXT	SEGMENT
_r$ = 8
_connection$ = 12
_identity$ = 16
_generation$ = 20
_role$ = 24
_dependencies$ = 28
_api$ = 32
_o$ = -408
_p$ = -4
_data$ = -404
_papinho_pst_owner_create PROC NEAR

; 36   :     const papinho_pst_owner_api *api) {

	push	ebp
	mov	ebp, esp
	sub	esp, 416				; 000001a0H
	push	esi
	push	edi

; 37   :     papinho_pst_owner *o, *p;
; 38   :     WSADATA data;
; 39   :     if (!r || !identity || !generation || !dependencies)

	cmp	DWORD PTR _r$[ebp], 0
	je	SHORT $L18854
	cmp	DWORD PTR _identity$[ebp], 0
	je	SHORT $L18854
	cmp	DWORD PTR _generation$[ebp], 0
	je	SHORT $L18854
	cmp	DWORD PTR _dependencies$[ebp], 0
	jne	SHORT $L18853
$L18854:

; 40   :         return NULL;

	xor	eax, eax
	jmp	$L18849
$L18853:

; 41   :     EnterCriticalSection(&r->lock);

	mov	eax, DWORD PTR _r$[ebp]
	push	eax
	call	DWORD PTR __imp__EnterCriticalSection@4

; 42   :     if (r->admission_closed) { LeaveCriticalSection(&r->lock); return NULL; }

	mov	ecx, DWORD PTR _r$[ebp]
	cmp	DWORD PTR [ecx+28], 0
	je	SHORT $L18856
	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	DWORD PTR __imp__LeaveCriticalSection@4
	xor	eax, eax
	jmp	$L18849
$L18856:

; 43   :     for (p = r->head; p; p = p->next) {

	mov	eax, DWORD PTR _r$[ebp]
	mov	ecx, DWORD PTR [eax+24]
	mov	DWORD PTR _p$[ebp], ecx
	jmp	SHORT $L18858
$L18859:
	mov	edx, DWORD PTR _p$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _p$[ebp], eax
$L18858:
	cmp	DWORD PTR _p$[ebp], 0
	je	SHORT $L18860

; 44   :         /* Tombstones prevent token reuse even after finalization. */
; 45   :         if (p->identity == identity) { LeaveCriticalSection(&r->lock); return NULL; }

	mov	ecx, DWORD PTR _p$[ebp]
	mov	edx, DWORD PTR [ecx+52]
	cmp	edx, DWORD PTR _identity$[ebp]
	jne	SHORT $L18861
	mov	eax, DWORD PTR _r$[ebp]
	push	eax
	call	DWORD PTR __imp__LeaveCriticalSection@4
	xor	eax, eax
	jmp	$L18849
$L18861:

; 46   :     }

	jmp	SHORT $L18859
$L18860:

; 47   :     o = (papinho_pst_owner *)calloc(1, sizeof(*o));

	push	148					; 00000094H
	push	1
	call	DWORD PTR __imp__calloc
	add	esp, 8
	mov	DWORD PTR _o$[ebp], eax

; 48   :     if (!o) { LeaveCriticalSection(&r->lock); return NULL; }

	cmp	DWORD PTR _o$[ebp], 0
	jne	SHORT $L18864
	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	call	DWORD PTR __imp__LeaveCriticalSection@4
	xor	eax, eax
	jmp	$L18849
$L18864:

; 49   :     o->api.release = api ? api->release : pst_connection_try_release;

	cmp	DWORD PTR _api$[ebp], 0
	je	SHORT $L18998
	mov	edx, DWORD PTR _api$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR -412+[ebp], eax
	jmp	SHORT $L18999
$L18998:
	mov	DWORD PTR -412+[ebp], OFFSET FLAT:_pst_connection_try_release
$L18999:
	mov	ecx, DWORD PTR _o$[ebp]
	mov	edx, DWORD PTR -412+[ebp]
	mov	DWORD PTR [ecx+40], edx

; 50   :     o->api.remove = api ? api->remove : pst_wait_set_remove_connection;

	cmp	DWORD PTR _api$[ebp], 0
	je	SHORT $L19000
	mov	eax, DWORD PTR _api$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR -416+[ebp], ecx
	jmp	SHORT $L19001
$L19000:
	mov	DWORD PTR -416+[ebp], OFFSET FLAT:_pst_wait_set_remove_connection
$L19001:
	mov	edx, DWORD PTR _o$[ebp]
	mov	eax, DWORD PTR -416+[ebp]
	mov	DWORD PTR [edx+44], eax

; 51   :     if (!o->api.release || !o->api.remove ||
; 52   :         pst_release_info_init(&o->release_info, sizeof(o->release_info)) != PST_RESULT_OK) {

	mov	ecx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [ecx+40], 0
	je	SHORT $L18868
	mov	edx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [edx+44], 0
	je	SHORT $L18868
	push	28					; 0000001cH
	mov	eax, DWORD PTR _o$[ebp]
	add	eax, 92					; 0000005cH
	push	eax
	call	_pst_release_info_init
	add	esp, 8
	test	eax, eax
	je	SHORT $L18867
$L18868:

; 53   :         free(o); LeaveCriticalSection(&r->lock); return NULL;

	mov	ecx, DWORD PTR _o$[ebp]
	push	ecx
	call	DWORD PTR __imp__free
	add	esp, 4
	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	DWORD PTR __imp__LeaveCriticalSection@4
	xor	eax, eax
	jmp	$L18849
$L18867:

; 55   :     if (WSAStartup(MAKEWORD(1,1), &data) != 0) {

	lea	eax, DWORD PTR _data$[ebp]
	push	eax
	push	257					; 00000101H
	call	_WSAStartup@8
	test	eax, eax
	je	SHORT $L18874

; 56   :         free(o); LeaveCriticalSection(&r->lock); return NULL;

	mov	ecx, DWORD PTR _o$[ebp]
	push	ecx
	call	DWORD PTR __imp__free
	add	esp, 4
	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	DWORD PTR __imp__LeaveCriticalSection@4
	xor	eax, eax
	jmp	$L18849
$L18874:

; 58   :     o->winsock_owned = 1;

	mov	eax, DWORD PTR _o$[ebp]
	mov	DWORD PTR [eax+76], 1

; 59   :     o->release_eligible = 1;

	mov	ecx, DWORD PTR _o$[ebp]
	mov	DWORD PTR [ecx+80], 1

; 60   :     o->registry = r; o->connection = connection; o->dependencies = *dependencies;

	mov	edx, DWORD PTR _o$[ebp]
	mov	eax, DWORD PTR _r$[ebp]
	mov	DWORD PTR [edx+4], eax
	mov	ecx, DWORD PTR _o$[ebp]
	mov	edx, DWORD PTR _connection$[ebp]
	mov	DWORD PTR [ecx+8], edx
	mov	esi, DWORD PTR _dependencies$[ebp]
	mov	edi, DWORD PTR _o$[ebp]
	add	edi, 16					; 00000010H
	mov	ecx, 6
	rep movsd

; 61   :     o->identity = identity; o->generation = generation; o->role = role;

	mov	eax, DWORD PTR _o$[ebp]
	mov	ecx, DWORD PTR _identity$[ebp]
	mov	DWORD PTR [eax+52], ecx
	mov	edx, DWORD PTR _o$[ebp]
	mov	eax, DWORD PTR _generation$[ebp]
	mov	DWORD PTR [edx+56], eax
	mov	ecx, DWORD PTR _o$[ebp]
	mov	edx, DWORD PTR _role$[ebp]
	mov	DWORD PTR [ecx+60], edx

; 62   :     o->thread_id = GetCurrentThreadId();

	call	DWORD PTR __imp__GetCurrentThreadId@0
	mov	ecx, DWORD PTR _o$[ebp]
	mov	DWORD PTR [ecx+48], eax

; 63   : 
; 64   :     o->next = r->head; r->head = o;

	mov	edx, DWORD PTR _o$[ebp]
	mov	eax, DWORD PTR _r$[ebp]
	mov	ecx, DWORD PTR [eax+24]
	mov	DWORD PTR [edx], ecx
	mov	edx, DWORD PTR _r$[ebp]
	mov	eax, DWORD PTR _o$[ebp]
	mov	DWORD PTR [edx+24], eax

; 65   :     LeaveCriticalSection(&r->lock);

	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	call	DWORD PTR __imp__LeaveCriticalSection@4

; 66   :     return o;

	mov	eax, DWORD PTR _o$[ebp]
$L18849:

; 67   : }

	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	0
_papinho_pst_owner_create ENDP
_TEXT	ENDS
PUBLIC	_papinho_pst_owner_callback_allowed
_TEXT	SEGMENT
_o$ = 8
_identity$ = 12
_generation$ = 16
_allowed$ = -4
_papinho_pst_owner_callback_allowed PROC NEAR

; 70   :     unsigned long identity, unsigned long generation) {

	push	ebp
	mov	ebp, esp
	sub	esp, 8

; 71   :     int allowed;
; 72   :     if (!o) return 0;

	cmp	DWORD PTR _o$[ebp], 0
	jne	SHORT $L18884
	xor	eax, eax
	jmp	SHORT $L18882
$L18884:

; 73   :     EnterCriticalSection(&o->registry->lock);

	mov	eax, DWORD PTR _o$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	push	ecx
	call	DWORD PTR __imp__EnterCriticalSection@4

; 74   :     allowed = o->identity == identity && o->generation == generation &&
; 75   :         !o->registry->admission_closed &&
; 76   :         !o->closing && !o->finalized && !o->terminal_retained &&
; 77   :         o->thread_id == GetCurrentThreadId();

	mov	edx, DWORD PTR _o$[ebp]
	mov	eax, DWORD PTR [edx+52]
	cmp	eax, DWORD PTR _identity$[ebp]
	jne	SHORT $L19003
	mov	ecx, DWORD PTR _o$[ebp]
	mov	edx, DWORD PTR [ecx+56]
	cmp	edx, DWORD PTR _generation$[ebp]
	jne	SHORT $L19003
	mov	eax, DWORD PTR _o$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	cmp	DWORD PTR [ecx+28], 0
	jne	SHORT $L19003
	mov	edx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [edx+64], 0
	jne	SHORT $L19003
	mov	eax, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [eax+68], 0
	jne	SHORT $L19003
	mov	ecx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [ecx+72], 0
	jne	SHORT $L19003
	call	DWORD PTR __imp__GetCurrentThreadId@0
	mov	edx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [edx+48], eax
	jne	SHORT $L19003
	mov	DWORD PTR -8+[ebp], 1
	jmp	SHORT $L19004
$L19003:
	mov	DWORD PTR -8+[ebp], 0
$L19004:
	mov	eax, DWORD PTR -8+[ebp]
	mov	DWORD PTR _allowed$[ebp], eax

; 78   :     LeaveCriticalSection(&o->registry->lock);

	mov	ecx, DWORD PTR _o$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	push	edx
	call	DWORD PTR __imp__LeaveCriticalSection@4

; 79   :     return allowed;

	mov	eax, DWORD PTR _allowed$[ebp]
$L18882:

; 80   : }

	mov	esp, ebp
	pop	ebp
	ret	0
_papinho_pst_owner_callback_allowed ENDP
_TEXT	ENDS
PUBLIC	_papinho_pst_owner_remove_membership
_TEXT	SEGMENT
_o$ = 8
_rc$ = -4
_papinho_pst_owner_remove_membership PROC NEAR

; 82   : int papinho_pst_owner_remove_membership(papinho_pst_owner *o) {

	push	ebp
	mov	ebp, esp
	push	ecx

; 83   :     PST_RESULT rc;
; 84   :     if (!o || o->thread_id != GetCurrentThreadId() ||
; 85   :         o->terminal_retained || o->finalized) return 0;

	cmp	DWORD PTR _o$[ebp], 0
	je	SHORT $L18890
	call	DWORD PTR __imp__GetCurrentThreadId@0
	mov	ecx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [ecx+48], eax
	jne	SHORT $L18890
	mov	edx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [edx+72], 0
	jne	SHORT $L18890
	mov	eax, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [eax+68], 0
	je	SHORT $L18889
$L18890:
	xor	eax, eax
	jmp	SHORT $L18887
$L18889:

; 86   :     if (!o->wait_set) return 1;

	mov	ecx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [ecx+12], 0
	jne	SHORT $L18891
	mov	eax, 1
	jmp	SHORT $L18887
$L18891:

; 87   :     rc = o->api.remove(o->wait_set, o->connection);

	mov	edx, DWORD PTR _o$[ebp]
	mov	eax, DWORD PTR [edx+8]
	push	eax
	mov	ecx, DWORD PTR _o$[ebp]
	mov	edx, DWORD PTR [ecx+12]
	push	edx
	mov	eax, DWORD PTR _o$[ebp]
	call	DWORD PTR [eax+44]
	add	esp, 8
	mov	DWORD PTR _rc$[ebp], eax

; 88   :     o->preclose_result = rc;

	mov	ecx, DWORD PTR _o$[ebp]
	mov	edx, DWORD PTR _rc$[ebp]
	mov	DWORD PTR [ecx+120], edx

; 89   :     if (rc != PST_RESULT_OK) return 0;

	cmp	DWORD PTR _rc$[ebp], 0
	je	SHORT $L18893
	xor	eax, eax
	jmp	SHORT $L18887
$L18893:

; 90   :     o->wait_set = NULL;

	mov	eax, DWORD PTR _o$[ebp]
	mov	DWORD PTR [eax+12], 0

; 91   :     o->release_eligible = 1; /* factual owner-thread precondition correction */

	mov	ecx, DWORD PTR _o$[ebp]
	mov	DWORD PTR [ecx+80], 1

; 92   :     return 1;

	mov	eax, 1
$L18887:

; 93   : }

	mov	esp, ebp
	pop	ebp
	ret	0
_papinho_pst_owner_remove_membership ENDP
_TEXT	ENDS
PUBLIC	_papinho_pst_owner_close
_TEXT	SEGMENT
_o$ = 8
_identity$ = 12
_generation$ = 16
_result$ = -4
_r$ = -8
_papinho_pst_owner_close PROC NEAR

; 96   :     unsigned long identity, unsigned long generation) {

	push	ebp
	mov	ebp, esp
	sub	esp, 8

; 97   :     PST_RESULT result;
; 98   :     papinho_pst_owner_registry *r;
; 99   :     if (!o) return 0;

	cmp	DWORD PTR _o$[ebp], 0
	jne	SHORT $L18904
	xor	eax, eax
	jmp	$L18901
$L18904:

; 100  :     r = o->registry;

	mov	eax, DWORD PTR _o$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR _r$[ebp], ecx

; 101  :     EnterCriticalSection(&r->lock);

	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	DWORD PTR __imp__EnterCriticalSection@4

; 102  :     if (o->identity != identity || o->generation != generation ||
; 103  :         o->thread_id != GetCurrentThreadId()) {

	mov	eax, DWORD PTR _o$[ebp]
	mov	ecx, DWORD PTR [eax+52]
	cmp	ecx, DWORD PTR _identity$[ebp]
	jne	SHORT $L18906
	mov	edx, DWORD PTR _o$[ebp]
	mov	eax, DWORD PTR [edx+56]
	cmp	eax, DWORD PTR _generation$[ebp]
	jne	SHORT $L18906
	call	DWORD PTR __imp__GetCurrentThreadId@0
	mov	ecx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [ecx+48], eax
	je	SHORT $L18905
$L18906:

; 104  :         LeaveCriticalSection(&r->lock); return 0;

	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	DWORD PTR __imp__LeaveCriticalSection@4
	xor	eax, eax
	jmp	$L18901
$L18905:

; 106  :     if (o->finalized) { LeaveCriticalSection(&r->lock); return 1; }

	mov	eax, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [eax+68], 0
	je	SHORT $L18907
	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	call	DWORD PTR __imp__LeaveCriticalSection@4
	mov	eax, 1
	jmp	$L18901
$L18907:

; 107  :     if (o->terminal_retained) { LeaveCriticalSection(&r->lock); return 0; }

	mov	edx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [edx+72], 0
	je	SHORT $L18908
	mov	eax, DWORD PTR _r$[ebp]
	push	eax
	call	DWORD PTR __imp__LeaveCriticalSection@4
	xor	eax, eax
	jmp	$L18901
$L18908:

; 108  :     o->closing = 1;

	mov	ecx, DWORD PTR _o$[ebp]
	mov	DWORD PTR [ecx+64], 1

; 109  :     if (!o->quiescence_confirmed) { LeaveCriticalSection(&r->lock); return 0; }

	mov	edx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [edx+124], 0
	jne	SHORT $L18909
	mov	eax, DWORD PTR _r$[ebp]
	push	eax
	call	DWORD PTR __imp__LeaveCriticalSection@4
	xor	eax, eax
	jmp	$L18901
$L18909:

; 110  :     /* PRE_CLOSE failure is not a fabricated PST release disposition. */
; 111  :     if (!papinho_pst_owner_remove_membership(o)) {

	mov	ecx, DWORD PTR _o$[ebp]
	push	ecx
	call	_papinho_pst_owner_remove_membership
	add	esp, 4
	test	eax, eax
	jne	SHORT $L18910

; 112  :         LeaveCriticalSection(&r->lock); return 0;

	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	DWORD PTR __imp__LeaveCriticalSection@4
	xor	eax, eax
	jmp	$L18901
$L18910:

; 114  :     if (!o->release_eligible) { LeaveCriticalSection(&r->lock); return 0; }

	mov	eax, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [eax+80], 0
	jne	SHORT $L18911
	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	call	DWORD PTR __imp__LeaveCriticalSection@4
	xor	eax, eax
	jmp	$L18901
$L18911:

; 115  :     pst_release_info_init(&o->release_info, sizeof(o->release_info));

	push	28					; 0000001cH
	mov	edx, DWORD PTR _o$[ebp]
	add	edx, 92					; 0000005cH
	push	edx
	call	_pst_release_info_init
	add	esp, 8

; 116  :     ++o->release_attempts;

	mov	eax, DWORD PTR _o$[ebp]
	mov	ecx, DWORD PTR [eax+84]
	add	ecx, 1
	mov	edx, DWORD PTR _o$[ebp]
	mov	DWORD PTR [edx+84], ecx

; 117  :     if (o->connection == NULL) {

	mov	eax, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [eax+8], 0
	jne	SHORT $L18913

; 118  :         /* Reserved owner whose PST creation never succeeded. No PST destruction
; 119  :          * is claimed: NONE remains the release disposition and attempts is zero. */
; 120  :         --o->release_attempts;

	mov	ecx, DWORD PTR _o$[ebp]
	mov	edx, DWORD PTR [ecx+84]
	sub	edx, 1
	mov	eax, DWORD PTR _o$[ebp]
	mov	DWORD PTR [eax+84], edx

; 121  :         if (!owner_finish_dependencies(o)) { LeaveCriticalSection(&r->lock); return 0; }

	mov	ecx, DWORD PTR _o$[ebp]
	push	ecx
	call	_owner_finish_dependencies
	add	esp, 4
	test	eax, eax
	jne	SHORT $L18914
	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	DWORD PTR __imp__LeaveCriticalSection@4
	xor	eax, eax
	jmp	$L18901
$L18914:

; 122  :         LeaveCriticalSection(&r->lock); return 1;

	mov	eax, DWORD PTR _r$[ebp]
	push	eax
	call	DWORD PTR __imp__LeaveCriticalSection@4
	mov	eax, 1
	jmp	$L18901
$L18913:

; 124  :     result = o->api.release(o->connection, &o->release_info);

	mov	ecx, DWORD PTR _o$[ebp]
	add	ecx, 92					; 0000005cH
	push	ecx
	mov	edx, DWORD PTR _o$[ebp]
	mov	eax, DWORD PTR [edx+8]
	push	eax
	mov	ecx, DWORD PTR _o$[ebp]
	call	DWORD PTR [ecx+40]
	add	esp, 8
	mov	DWORD PTR _result$[ebp], eax

; 125  :     if (result == PST_RESULT_OK && o->release_info.result == result &&
; 126  :         o->release_info.disposition == PST_RELEASE_DISPOSITION_DESTROYED) {

	cmp	DWORD PTR _result$[ebp], 0
	jne	SHORT $L18916
	mov	edx, DWORD PTR _o$[ebp]
	mov	eax, DWORD PTR [edx+104]
	cmp	eax, DWORD PTR _result$[ebp]
	jne	SHORT $L18916
	mov	ecx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [ecx+100], 2
	jne	SHORT $L18916

; 127  :         o->connection = NULL;

	mov	edx, DWORD PTR _o$[ebp]
	mov	DWORD PTR [edx+8], 0

; 128  :         if (!owner_finish_dependencies(o)) { LeaveCriticalSection(&r->lock); return 0; }

	mov	eax, DWORD PTR _o$[ebp]
	push	eax
	call	_owner_finish_dependencies
	add	esp, 4
	test	eax, eax
	jne	SHORT $L18918
	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	call	DWORD PTR __imp__LeaveCriticalSection@4
	xor	eax, eax
	jmp	SHORT $L18901
$L18918:

; 129  :         LeaveCriticalSection(&r->lock); return 1;

	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	DWORD PTR __imp__LeaveCriticalSection@4
	mov	eax, 1
	jmp	SHORT $L18901
$L18916:

; 131  :     if (result != PST_RESULT_INVALID_STATE ||
; 132  :         o->release_info.result != result ||
; 133  :         o->release_info.disposition != PST_RELEASE_DISPOSITION_PRECONDITION_RETAINED) {

	cmp	DWORD PTR _result$[ebp], 2
	jne	SHORT $L18921
	mov	eax, DWORD PTR _o$[ebp]
	mov	ecx, DWORD PTR [eax+104]
	cmp	ecx, DWORD PTR _result$[ebp]
	jne	SHORT $L18921
	mov	edx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [edx+100], 1
	je	SHORT $L18920
$L18921:

; 134  :         o->terminal_retained = 1;

	mov	eax, DWORD PTR _o$[ebp]
	mov	DWORD PTR [eax+72], 1

; 135  :         r->admission_closed = 1;

	mov	ecx, DWORD PTR _r$[ebp]
	mov	DWORD PTR [ecx+28], 1

; 136  :     } else {

	jmp	SHORT $L18922
$L18920:

; 137  :         o->release_eligible = 0;

	mov	edx, DWORD PTR _o$[ebp]
	mov	DWORD PTR [edx+80], 0
$L18922:

; 139  :     LeaveCriticalSection(&r->lock);

	mov	eax, DWORD PTR _r$[ebp]
	push	eax
	call	DWORD PTR __imp__LeaveCriticalSection@4

; 140  :     return 0;

	xor	eax, eax
$L18901:

; 141  : }

	mov	esp, ebp
	pop	ebp
	ret	0
_papinho_pst_owner_close ENDP
_TEXT	ENDS
EXTRN	_WSACleanup@0:NEAR
EXTRN	_WSAGetLastError@0:NEAR
_TEXT	SEGMENT
_o$ = 8
_owner_finish_dependencies PROC NEAR

; 6    : static int owner_finish_dependencies(papinho_pst_owner *o) {

	push	ebp
	mov	ebp, esp

; 7    :     if (o->winsock_owned) {

	mov	eax, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [eax+76], 0
	je	SHORT $L18828

; 8    :         if (o->winsock_cleanup_attempted) return 0;

	mov	ecx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [ecx+128], 0
	je	SHORT $L18829
	xor	eax, eax
	jmp	$L18827
$L18829:

; 9    :         o->winsock_cleanup_attempted = 1;

	mov	edx, DWORD PTR _o$[ebp]
	mov	DWORD PTR [edx+128], 1

; 10   :         o->winsock_cleanup_result = WSACleanup();

	call	_WSACleanup@0
	mov	ecx, DWORD PTR _o$[ebp]
	mov	DWORD PTR [ecx+132], eax

; 11   :         if (o->winsock_cleanup_result != 0) {

	mov	edx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [edx+132], 0
	je	SHORT $L18830

; 12   :             o->winsock_cleanup_error = WSAGetLastError();

	call	_WSAGetLastError@0
	mov	ecx, DWORD PTR _o$[ebp]
	mov	DWORD PTR [ecx+136], eax

; 13   :             o->terminal_retained = 1; /* Browser retention, not a PST disposition */

	mov	edx, DWORD PTR _o$[ebp]
	mov	DWORD PTR [edx+72], 1

; 14   :             o->registry->admission_closed = 1;

	mov	eax, DWORD PTR _o$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR [ecx+28], 1

; 15   :             return 0;

	xor	eax, eax
	jmp	SHORT $L18827
$L18830:

; 17   :         o->winsock_owned = 0;

	mov	edx, DWORD PTR _o$[ebp]
	mov	DWORD PTR [edx+76], 0
$L18828:

; 19   :     /* D's callback only drops this owner's group lease; it does not destroy
; 20   :      * the group or its resources. Keep that lease on WinSock cleanup failure. */
; 21   :     if (o->dependencies.finalize)

	mov	eax, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [eax+36], 0
	je	SHORT $L18831

; 22   :         o->dependencies.finalize(o->dependencies.context);

	mov	ecx, DWORD PTR _o$[ebp]
	mov	edx, DWORD PTR [ecx+32]
	push	edx
	mov	eax, DWORD PTR _o$[ebp]
	call	DWORD PTR [eax+36]
	add	esp, 4
$L18831:

; 23   :     ZeroMemory(&o->dependencies, sizeof(o->dependencies));

	push	24					; 00000018H
	push	0
	mov	ecx, DWORD PTR _o$[ebp]
	add	ecx, 16					; 00000010H
	push	ecx
	call	_memset
	add	esp, 12					; 0000000cH

; 24   :     o->finalized = 1;

	mov	edx, DWORD PTR _o$[ebp]
	mov	DWORD PTR [edx+68], 1

; 25   :     ++o->finalizations;

	mov	eax, DWORD PTR _o$[ebp]
	mov	ecx, DWORD PTR [eax+88]
	add	ecx, 1
	mov	edx, DWORD PTR _o$[ebp]
	mov	DWORD PTR [edx+88], ecx

; 26   :     return 1;

	mov	eax, 1
$L18827:

; 27   : }

	pop	ebp
	ret	0
_owner_finish_dependencies ENDP
_TEXT	ENDS
PUBLIC	_papinho_pst_owner_reclaim
_TEXT	SEGMENT
_o$ = 8
_r$ = -4
_link$ = -8
_papinho_pst_owner_reclaim PROC NEAR

; 143  : int papinho_pst_owner_reclaim(papinho_pst_owner *o) {

	push	ebp
	mov	ebp, esp
	sub	esp, 8

; 144  :     papinho_pst_owner_registry *r;
; 145  :     papinho_pst_owner **link;
; 146  :     if (!o || !o->finalized || o->thread_id != GetCurrentThreadId()) return 0;

	cmp	DWORD PTR _o$[ebp], 0
	je	SHORT $L18929
	mov	eax, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [eax+68], 0
	je	SHORT $L18929
	call	DWORD PTR __imp__GetCurrentThreadId@0
	mov	ecx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [ecx+48], eax
	je	SHORT $L18928
$L18929:
	xor	eax, eax
	jmp	SHORT $L18925
$L18928:

; 147  :     r = o->registry;

	mov	edx, DWORD PTR _o$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR _r$[ebp], eax

; 148  :     EnterCriticalSection(&r->lock);

	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	call	DWORD PTR __imp__EnterCriticalSection@4

; 149  :     for (link = &r->head; *link && *link != o; link = &(*link)->next) {}

	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 24					; 00000018H
	mov	DWORD PTR _link$[ebp], edx
	jmp	SHORT $L18930
$L18931:
	mov	eax, DWORD PTR _link$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _link$[ebp], ecx
$L18930:
	mov	edx, DWORD PTR _link$[ebp]
	cmp	DWORD PTR [edx], 0
	je	SHORT $L18932
	mov	eax, DWORD PTR _link$[ebp]
	mov	ecx, DWORD PTR [eax]
	cmp	ecx, DWORD PTR _o$[ebp]
	je	SHORT $L18932
	jmp	SHORT $L18931
$L18932:

; 150  :     if (*link != o) { LeaveCriticalSection(&r->lock); return 0; }

	mov	edx, DWORD PTR _link$[ebp]
	mov	eax, DWORD PTR [edx]
	cmp	eax, DWORD PTR _o$[ebp]
	je	SHORT $L18933
	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	call	DWORD PTR __imp__LeaveCriticalSection@4
	xor	eax, eax
	jmp	SHORT $L18925
$L18933:

; 151  :     *link = o->next;

	mov	edx, DWORD PTR _link$[ebp]
	mov	eax, DWORD PTR _o$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR [edx], ecx

; 152  :     LeaveCriticalSection(&r->lock);

	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	DWORD PTR __imp__LeaveCriticalSection@4

; 153  :     free(o);

	mov	eax, DWORD PTR _o$[ebp]
	push	eax
	call	DWORD PTR __imp__free
	add	esp, 4

; 154  :     return 1;

	mov	eax, 1
$L18925:

; 155  : }

	mov	esp, ebp
	pop	ebp
	ret	0
_papinho_pst_owner_reclaim ENDP
_TEXT	ENDS
PUBLIC	_papinho_pst_owner_registry_dispose
EXTRN	__imp__DeleteCriticalSection@4:NEAR
_TEXT	SEGMENT
_r$ = 8
_o$ = -8
_next$ = -4
_papinho_pst_owner_registry_dispose PROC NEAR

; 157  : int papinho_pst_owner_registry_dispose(papinho_pst_owner_registry *r) {

	push	ebp
	mov	ebp, esp
	sub	esp, 8

; 158  :     papinho_pst_owner *o, *next;
; 159  :     if (!r) return 0;

	cmp	DWORD PTR _r$[ebp], 0
	jne	SHORT $L18939
	xor	eax, eax
	jmp	$L18936
$L18939:

; 160  :     EnterCriticalSection(&r->lock);

	mov	eax, DWORD PTR _r$[ebp]
	push	eax
	call	DWORD PTR __imp__EnterCriticalSection@4

; 161  :     for (o = r->head; o; o = o->next) {

	mov	ecx, DWORD PTR _r$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	mov	DWORD PTR _o$[ebp], edx
	jmp	SHORT $L18940
$L18941:
	mov	eax, DWORD PTR _o$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _o$[ebp], ecx
$L18940:
	cmp	DWORD PTR _o$[ebp], 0
	je	SHORT $L18942

; 162  :         if (!o->finalized) { LeaveCriticalSection(&r->lock); return 0; }

	mov	edx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [edx+68], 0
	jne	SHORT $L18943
	mov	eax, DWORD PTR _r$[ebp]
	push	eax
	call	DWORD PTR __imp__LeaveCriticalSection@4
	xor	eax, eax
	jmp	SHORT $L18936
$L18943:

; 163  :     }

	jmp	SHORT $L18941
$L18942:

; 164  :     o = r->head; r->head = NULL;

	mov	ecx, DWORD PTR _r$[ebp]
	mov	edx, DWORD PTR [ecx+24]
	mov	DWORD PTR _o$[ebp], edx
	mov	eax, DWORD PTR _r$[ebp]
	mov	DWORD PTR [eax+24], 0

; 165  :     LeaveCriticalSection(&r->lock);

	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	call	DWORD PTR __imp__LeaveCriticalSection@4
$L18946:

; 166  :     while (o) { next = o->next; free(o); o = next; }

	cmp	DWORD PTR _o$[ebp], 0
	je	SHORT $L18947
	mov	edx, DWORD PTR _o$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _next$[ebp], eax
	mov	ecx, DWORD PTR _o$[ebp]
	push	ecx
	call	DWORD PTR __imp__free
	add	esp, 4
	mov	edx, DWORD PTR _next$[ebp]
	mov	DWORD PTR _o$[ebp], edx
	jmp	SHORT $L18946
$L18947:

; 167  :     /* Caller must have quiesced all callback sources before disposal. */
; 168  :     DeleteCriticalSection(&r->lock);

	mov	eax, DWORD PTR _r$[ebp]
	push	eax
	call	DWORD PTR __imp__DeleteCriticalSection@4

; 169  :     return 1;

	mov	eax, 1
$L18936:

; 170  : }

	mov	esp, ebp
	pop	ebp
	ret	0
_papinho_pst_owner_registry_dispose ENDP
_TEXT	ENDS
PUBLIC	_papinho_pst_owner_provider_snapshot
EXTRN	_pst_connection_get_provider_info:NEAR
EXTRN	_strcmp:NEAR
EXTRN	__imp__memchr:NEAR
_DATA	SEGMENT
$SG18982 DB	'retrozilla-nss', 00H
_DATA	ENDS
_TEXT	SEGMENT
_o$ = 8
_identity$ = 12
_generation$ = 16
_out$ = 20
_f$ = -80
_rc$ = -12
_r$ = -8
_connection$ = -16
_a$ = -4
_z$ = -84
_t$ = -20
_papinho_pst_owner_provider_snapshot PROC NEAR

; 175  :  unsigned long identity,unsigned long generation,PST_PROVIDER_INFO *out) {

	push	ebp
	mov	ebp, esp
	sub	esp, 84					; 00000054H
	push	esi
	push	edi

; 176  :  PST_PROVIDER_INFO f;PST_RESULT rc;papinho_pst_owner_registry *r;const pst_connection *connection;
; 177  :  size_t a=(size_t)o,z=(size_t)out,t;

	mov	eax, DWORD PTR _o$[ebp]
	mov	DWORD PTR _a$[ebp], eax
	mov	ecx, DWORD PTR _out$[ebp]
	mov	DWORD PTR _z$[ebp], ecx

; 178  :  if(!o||!out||!o->registry)return PST_RESULT_INVALID_ARGUMENT;

	cmp	DWORD PTR _o$[ebp], 0
	je	SHORT $L18967
	cmp	DWORD PTR _out$[ebp], 0
	je	SHORT $L18967
	mov	edx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [edx+4], 0
	jne	SHORT $L18966
$L18967:
	mov	eax, 1
	jmp	$L18956
$L18966:

; 179  :  r=o->registry;t=(size_t)r;

	mov	eax, DWORD PTR _o$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR _r$[ebp], ecx
	mov	edx, DWORD PTR _r$[ebp]
	mov	DWORD PTR _t$[ebp], edx

; 180  :  if((z>=a&&z-a<sizeof(*o))||(z<a&&a-z<sizeof(*out))||
; 181  :     (z>=t&&z-t<sizeof(*r))||(z<t&&t-z<sizeof(*out)))return PST_RESULT_INVALID_ARGUMENT;

	mov	eax, DWORD PTR _z$[ebp]
	cmp	eax, DWORD PTR _a$[ebp]
	jb	SHORT $L18972
	mov	ecx, DWORD PTR _z$[ebp]
	sub	ecx, DWORD PTR _a$[ebp]
	cmp	ecx, 148				; 00000094H
	jb	SHORT $L18971
$L18972:
	mov	edx, DWORD PTR _z$[ebp]
	cmp	edx, DWORD PTR _a$[ebp]
	jae	SHORT $L18973
	mov	eax, DWORD PTR _a$[ebp]
	sub	eax, DWORD PTR _z$[ebp]
	cmp	eax, 60					; 0000003cH
	jb	SHORT $L18971
$L18973:
	mov	ecx, DWORD PTR _z$[ebp]
	cmp	ecx, DWORD PTR _t$[ebp]
	jb	SHORT $L18974
	mov	edx, DWORD PTR _z$[ebp]
	sub	edx, DWORD PTR _t$[ebp]
	cmp	edx, 32					; 00000020H
	jb	SHORT $L18971
$L18974:
	mov	eax, DWORD PTR _z$[ebp]
	cmp	eax, DWORD PTR _t$[ebp]
	jae	SHORT $L18970
	mov	ecx, DWORD PTR _t$[ebp]
	sub	ecx, DWORD PTR _z$[ebp]
	cmp	ecx, 60					; 0000003cH
	jae	SHORT $L18970
$L18971:
	mov	eax, 1
	jmp	$L18956
$L18970:

; 182  :  EnterCriticalSection(&r->lock);

	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	DWORD PTR __imp__EnterCriticalSection@4

; 183  :  if(o->identity!=identity||o->generation!=generation||!identity||!generation||
; 184  :     r->admission_closed||o->closing||o->finalized||o->terminal_retained||
; 185  :     o->release_attempts||!o->connection||o->thread_id!=GetCurrentThreadId()) {

	mov	eax, DWORD PTR _o$[ebp]
	mov	ecx, DWORD PTR [eax+52]
	cmp	ecx, DWORD PTR _identity$[ebp]
	jne	SHORT $L18977
	mov	edx, DWORD PTR _o$[ebp]
	mov	eax, DWORD PTR [edx+56]
	cmp	eax, DWORD PTR _generation$[ebp]
	jne	SHORT $L18977
	cmp	DWORD PTR _identity$[ebp], 0
	je	SHORT $L18977
	cmp	DWORD PTR _generation$[ebp], 0
	je	SHORT $L18977
	mov	ecx, DWORD PTR _r$[ebp]
	cmp	DWORD PTR [ecx+28], 0
	jne	SHORT $L18977
	mov	edx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [edx+64], 0
	jne	SHORT $L18977
	mov	eax, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [eax+68], 0
	jne	SHORT $L18977
	mov	ecx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [ecx+72], 0
	jne	SHORT $L18977
	mov	edx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [edx+84], 0
	jne	SHORT $L18977
	mov	eax, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [eax+8], 0
	je	SHORT $L18977
	call	DWORD PTR __imp__GetCurrentThreadId@0
	mov	ecx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [ecx+48], eax
	je	SHORT $L18976
$L18977:

; 186  :   LeaveCriticalSection(&r->lock);return PST_RESULT_INVALID_STATE;

	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	DWORD PTR __imp__LeaveCriticalSection@4
	mov	eax, 2
	jmp	$L18956
$L18976:

; 188  :  connection=o->connection;ZeroMemory(&f,sizeof(f));f.struct_size=sizeof(f);f.api_version=PST_API_VERSION;

	mov	eax, DWORD PTR _o$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	DWORD PTR _connection$[ebp], ecx
	push	60					; 0000003cH
	push	0
	lea	edx, DWORD PTR _f$[ebp]
	push	edx
	call	_memset
	add	esp, 12					; 0000000cH
	mov	DWORD PTR _f$[ebp], 60			; 0000003cH
	mov	DWORD PTR _f$[ebp+4], 196608		; 00030000H

; 189  :  rc=pst_connection_get_provider_info(connection,&f);

	lea	eax, DWORD PTR _f$[ebp]
	push	eax
	mov	ecx, DWORD PTR _connection$[ebp]
	push	ecx
	call	_pst_connection_get_provider_info
	add	esp, 8
	mov	DWORD PTR _rc$[ebp], eax

; 190  :  if(rc==PST_RESULT_OK&&(o->connection!=connection||o->closing||o->finalized||
; 191  :     o->terminal_retained||o->release_attempts||o->thread_id!=GetCurrentThreadId()||
; 192  :     o->registry!=r||r->admission_closed||o->identity!=identity||o->generation!=generation||
; 193  :     !memchr(f.provider_id,0,sizeof(f.provider_id))||strcmp(f.provider_id,"retrozilla-nss")||
; 194  :     f.available!=1U||f.initialized>1U))rc=PST_RESULT_INVALID_STATE;

	cmp	DWORD PTR _rc$[ebp], 0
	jne	$L18980
	mov	edx, DWORD PTR _o$[ebp]
	mov	eax, DWORD PTR [edx+8]
	cmp	eax, DWORD PTR _connection$[ebp]
	jne	$L18981
	mov	ecx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [ecx+64], 0
	jne	$L18981
	mov	edx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [edx+68], 0
	jne	$L18981
	mov	eax, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [eax+72], 0
	jne	SHORT $L18981
	mov	ecx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [ecx+84], 0
	jne	SHORT $L18981
	call	DWORD PTR __imp__GetCurrentThreadId@0
	mov	edx, DWORD PTR _o$[ebp]
	cmp	DWORD PTR [edx+48], eax
	jne	SHORT $L18981
	mov	eax, DWORD PTR _o$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	cmp	ecx, DWORD PTR _r$[ebp]
	jne	SHORT $L18981
	mov	edx, DWORD PTR _r$[ebp]
	cmp	DWORD PTR [edx+28], 0
	jne	SHORT $L18981
	mov	eax, DWORD PTR _o$[ebp]
	mov	ecx, DWORD PTR [eax+52]
	cmp	ecx, DWORD PTR _identity$[ebp]
	jne	SHORT $L18981
	mov	edx, DWORD PTR _o$[ebp]
	mov	eax, DWORD PTR [edx+56]
	cmp	eax, DWORD PTR _generation$[ebp]
	jne	SHORT $L18981
	push	32					; 00000020H
	push	0
	lea	ecx, DWORD PTR _f$[ebp+20]
	push	ecx
	call	DWORD PTR __imp__memchr
	add	esp, 12					; 0000000cH
	test	eax, eax
	je	SHORT $L18981
	push	OFFSET FLAT:$SG18982
	lea	edx, DWORD PTR _f$[ebp+20]
	push	edx
	call	_strcmp
	add	esp, 8
	test	eax, eax
	jne	SHORT $L18981
	cmp	DWORD PTR _f$[ebp+8], 1
	jne	SHORT $L18981
	cmp	DWORD PTR _f$[ebp+12], 1
	jbe	SHORT $L18980
$L18981:
	mov	DWORD PTR _rc$[ebp], 2
$L18980:

; 195  :  if(rc==PST_RESULT_OK)*out=f;

	cmp	DWORD PTR _rc$[ebp], 0
	jne	SHORT $L18985
	mov	ecx, 15					; 0000000fH
	lea	esi, DWORD PTR _f$[ebp]
	mov	edi, DWORD PTR _out$[ebp]
	rep movsd
$L18985:

; 196  :  LeaveCriticalSection(&r->lock);return rc;

	mov	eax, DWORD PTR _r$[ebp]
	push	eax
	call	DWORD PTR __imp__LeaveCriticalSection@4
	mov	eax, DWORD PTR _rc$[ebp]
$L18956:

; 197  : }

	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	0
_papinho_pst_owner_provider_snapshot ENDP
_TEXT	ENDS
END
