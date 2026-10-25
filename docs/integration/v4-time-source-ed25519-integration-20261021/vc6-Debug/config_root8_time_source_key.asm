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
$$SYMBOLS	SEGMENT BYTE USE32 'DEBSYM'
$$SYMBOLS	ENDS
$$TYPES	SEGMENT BYTE USE32 'DEBTYP'
$$TYPES	ENDS
_TLS	SEGMENT DWORD USE32 PUBLIC 'TLS'
_TLS	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	_pc_root8_time_source_key_data
EXTRN	_papacc_time_source_ed25519_check:NEAR
EXTRN	_memcmp:NEAR
_DATA	SEGMENT
$SG857	DB	'PACROOT8', 00H
	ORG $+3
$SG858	DB	'PACTSR01', 00H
_DATA	ENDS
_TEXT	SEGMENT
_kind$ = 12
_p$ = 16
_n$ = 20
_row$ = 24
_header$ = 28
_result$ = -4
_pc_root8_time_source_key_data PROC NEAR

; 6    : {

	push	ebp
	mov	ebp, esp
	push	ecx

; 7    :  int result;
; 8    :  (void)ctx;
; 9    :  if(kind!=9U)return PC_ROOT8_UNSUPPORTED;

	cmp	DWORD PTR _kind$[ebp], 9
	je	SHORT $L852
	mov	eax, 2
	jmp	$L849
$L852:

; 10   :  if(!p||n!=384U||p!=row||!header)return PC_ROOT8_INVALID;

	cmp	DWORD PTR _p$[ebp], 0
	je	SHORT $L854
	cmp	DWORD PTR _n$[ebp], 384			; 00000180H
	jne	SHORT $L854
	mov	eax, DWORD PTR _p$[ebp]
	cmp	eax, DWORD PTR _row$[ebp]
	jne	SHORT $L854
	cmp	DWORD PTR _header$[ebp], 0
	jne	SHORT $L853
$L854:
	mov	eax, 1
	jmp	$L849
$L853:

; 11   :  if(memcmp(header,"PACROOT8",8)||header[8]!=4U||header[9]||
; 12   :     memcmp(p,"PACTSR01",8)||p[8]!=1U||p[9]||p[10]!=128U||p[11]!=1U)

	push	8
	push	OFFSET FLAT:$SG857
	mov	ecx, DWORD PTR _header$[ebp]
	push	ecx
	call	_memcmp
	add	esp, 12					; 0000000cH
	test	eax, eax
	jne	SHORT $L856
	mov	edx, DWORD PTR _header$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+8]
	cmp	eax, 4
	jne	SHORT $L856
	mov	ecx, DWORD PTR _header$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+9]
	test	edx, edx
	jne	SHORT $L856
	push	8
	push	OFFSET FLAT:$SG858
	mov	eax, DWORD PTR _p$[ebp]
	push	eax
	call	_memcmp
	add	esp, 12					; 0000000cH
	test	eax, eax
	jne	SHORT $L856
	mov	ecx, DWORD PTR _p$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+8]
	cmp	edx, 1
	jne	SHORT $L856
	mov	eax, DWORD PTR _p$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax+9]
	test	ecx, ecx
	jne	SHORT $L856
	mov	edx, DWORD PTR _p$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+10]
	cmp	eax, 128				; 00000080H
	jne	SHORT $L856
	mov	ecx, DWORD PTR _p$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+11]
	cmp	edx, 1
	je	SHORT $L855
$L856:

; 13   :   return PC_ROOT8_INVALID;

	mov	eax, 1
	jmp	SHORT $L849
$L855:

; 14   :  result=papacc_time_source_ed25519_check(p+88U,32U);

	push	32					; 00000020H
	mov	eax, DWORD PTR _p$[ebp]
	add	eax, 88					; 00000058H
	push	eax
	call	_papacc_time_source_ed25519_check
	add	esp, 8
	mov	DWORD PTR _result$[ebp], eax

; 15   :  if(result==PAPACC_ED25519_VALID)return PC_ROOT8_OK;

	cmp	DWORD PTR _result$[ebp], 1
	jne	SHORT $L859
	xor	eax, eax
	jmp	SHORT $L849
$L859:

; 16   :  if(result==PAPACC_ED25519_INVALID)return PC_ROOT8_INVALID;

	cmp	DWORD PTR _result$[ebp], 0
	jne	SHORT $L860
	mov	eax, 1
	jmp	SHORT $L849
$L860:

; 17   :  if(result==PAPACC_ED25519_BACKEND_UNAVAILABLE)return PC_ROOT8_UNSUPPORTED;

	cmp	DWORD PTR _result$[ebp], -2		; fffffffeH
	jne	SHORT $L861
	mov	eax, 2
	jmp	SHORT $L849
$L861:

; 18   :  return PC_ROOT8_INCOMPLETE;

	mov	eax, 4
$L849:

; 19   : }

	mov	esp, ebp
	pop	ebp
	ret	0
_pc_root8_time_source_key_data ENDP
_TEXT	ENDS
END
