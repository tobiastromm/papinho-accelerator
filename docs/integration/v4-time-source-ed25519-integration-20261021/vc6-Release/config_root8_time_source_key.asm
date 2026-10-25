	TITLE	C:\Projetos\PapinhoAccelerator\src\config\config_root8_time_source_key.c
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
;	COMDAT ??_C@_08IFEI@PACROOT8?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_08NPIP@PACTSR01?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT _pc_root8_time_source_key_data
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	_pc_root8_time_source_key_data
PUBLIC	??_C@_08IFEI@PACROOT8?$AA@			; `string'
PUBLIC	??_C@_08NPIP@PACTSR01?$AA@			; `string'
EXTRN	_papacc_time_source_ed25519_check:NEAR
;	COMDAT ??_C@_08IFEI@PACROOT8?$AA@
; File C:\Projetos\PapinhoAccelerator\src\config\config_root8_time_source_key.c
_DATA	SEGMENT
??_C@_08IFEI@PACROOT8?$AA@ DB 'PACROOT8', 00H		; `string'
_DATA	ENDS
;	COMDAT ??_C@_08NPIP@PACTSR01?$AA@
_DATA	SEGMENT
??_C@_08NPIP@PACTSR01?$AA@ DB 'PACTSR01', 00H		; `string'
_DATA	ENDS
;	COMDAT _pc_root8_time_source_key_data
_TEXT	SEGMENT
_kind$ = 12
_p$ = 16
_n$ = 20
_row$ = 24
_header$ = 28
_pc_root8_time_source_key_data PROC NEAR		; COMDAT

; 7    :  int result;
; 8    :  (void)ctx;
; 9    :  if(kind!=9U)return PC_ROOT8_UNSUPPORTED;

	mov	eax, DWORD PTR _kind$[esp-4]
	push	ebx
	push	esi
	cmp	eax, 9
	push	edi
	je	SHORT $L877
	pop	edi
	pop	esi
	mov	eax, 2
	pop	ebx

; 18   :  return PC_ROOT8_INCOMPLETE;
; 19   : }

	ret	0
$L877:

; 10   :  if(!p||n!=384U||p!=row||!header)return PC_ROOT8_INVALID;

	mov	eax, DWORD PTR _p$[esp+8]
	test	eax, eax
	je	$L879
	cmp	DWORD PTR _n$[esp+8], 384		; 00000180H
	jne	$L879
	cmp	eax, DWORD PTR _row$[esp+8]
	jne	$L879
	mov	edx, DWORD PTR _header$[esp+8]
	test	edx, edx
	je	$L879

; 11   :  if(memcmp(header,"PACROOT8",8)||header[8]!=4U||header[9]||
; 12   :     memcmp(p,"PACTSR01",8)||p[8]!=1U||p[9]||p[10]!=128U||p[11]!=1U)

	mov	ecx, 2
	mov	edi, OFFSET FLAT:??_C@_08IFEI@PACROOT8?$AA@ ; `string'
	mov	esi, edx
	xor	ebx, ebx
	repe cmpsd
	mov	ebx, 1
	jne	SHORT $L881
	cmp	BYTE PTR [edx+8], 4
	jne	SHORT $L881
	mov	cl, BYTE PTR [edx+9]
	test	cl, cl
	jne	SHORT $L881
	mov	ecx, 2
	mov	edi, OFFSET FLAT:??_C@_08NPIP@PACTSR01?$AA@ ; `string'
	mov	esi, eax
	xor	edx, edx
	repe cmpsd
	jne	SHORT $L881
	cmp	BYTE PTR [eax+8], bl
	jne	SHORT $L881
	mov	cl, BYTE PTR [eax+9]
	test	cl, cl
	jne	SHORT $L881
	cmp	BYTE PTR [eax+10], 128			; 00000080H
	jne	SHORT $L881
	cmp	BYTE PTR [eax+11], bl
	jne	SHORT $L881

; 13   :   return PC_ROOT8_INVALID;
; 14   :  result=papacc_time_source_ed25519_check(p+88U,32U);

	add	eax, 88					; 00000058H
	push	32					; 00000020H
	push	eax
	call	_papacc_time_source_ed25519_check
	add	esp, 8

; 15   :  if(result==PAPACC_ED25519_VALID)return PC_ROOT8_OK;

	cmp	eax, ebx
	jne	SHORT $L884
	pop	edi
	pop	esi
	xor	eax, eax
	pop	ebx

; 18   :  return PC_ROOT8_INCOMPLETE;
; 19   : }

	ret	0
$L884:

; 16   :  if(result==PAPACC_ED25519_INVALID)return PC_ROOT8_INVALID;

	test	eax, eax
	jne	SHORT $L885
$L881:
	pop	edi
	mov	eax, ebx
	pop	esi
	pop	ebx

; 18   :  return PC_ROOT8_INCOMPLETE;
; 19   : }

	ret	0
$L885:

; 17   :  if(result==PAPACC_ED25519_BACKEND_UNAVAILABLE)return PC_ROOT8_UNSUPPORTED;

	sub	eax, -2					; fffffffeH
	pop	edi
	neg	eax
	sbb	eax, eax
	pop	esi
	and	eax, 2
	pop	ebx
	add	eax, 2

; 18   :  return PC_ROOT8_INCOMPLETE;
; 19   : }

	ret	0
$L879:
	pop	edi
	pop	esi

; 10   :  if(!p||n!=384U||p!=row||!header)return PC_ROOT8_INVALID;

	mov	eax, 1
	pop	ebx

; 18   :  return PC_ROOT8_INCOMPLETE;
; 19   : }

	ret	0
_pc_root8_time_source_key_data ENDP
_TEXT	ENDS
END
