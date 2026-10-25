	TITLE	C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\memory_ops.c
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
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	_pacc_hash_memcpy
_TEXT	SEGMENT
_dst$ = 8
_src$ = 12
_n$ = 16
_d$ = -8
_p$ = -4
_ret$ = -12
_pacc_hash_memcpy PROC NEAR

; 5    : {volatile unsigned char *d=dst;const unsigned char *p=src;void *ret=dst;while(n--)*d++=*p++;return ret;}

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	mov	eax, DWORD PTR _dst$[ebp]
	mov	DWORD PTR _d$[ebp], eax
	mov	ecx, DWORD PTR _src$[ebp]
	mov	DWORD PTR _p$[ebp], ecx
	mov	edx, DWORD PTR _dst$[ebp]
	mov	DWORD PTR _ret$[ebp], edx
$L700:
	mov	eax, DWORD PTR _n$[ebp]
	mov	ecx, DWORD PTR _n$[ebp]
	sub	ecx, 1
	mov	DWORD PTR _n$[ebp], ecx
	test	eax, eax
	je	SHORT $L701
	mov	edx, DWORD PTR _d$[ebp]
	mov	eax, DWORD PTR _p$[ebp]
	mov	cl, BYTE PTR [eax]
	mov	BYTE PTR [edx], cl
	mov	edx, DWORD PTR _d$[ebp]
	add	edx, 1
	mov	DWORD PTR _d$[ebp], edx
	mov	eax, DWORD PTR _p$[ebp]
	add	eax, 1
	mov	DWORD PTR _p$[ebp], eax
	jmp	SHORT $L700
$L701:
	mov	eax, DWORD PTR _ret$[ebp]
	mov	esp, ebp
	pop	ebp
	ret	0
_pacc_hash_memcpy ENDP
_TEXT	ENDS
PUBLIC	_pacc_hash_memset
_TEXT	SEGMENT
_dst$ = 8
_value$ = 12
_n$ = 16
_d$ = -4
_ret$ = -8
_pacc_hash_memset PROC NEAR

; 7    : {volatile unsigned char *d=dst;void *ret=dst;while(n--)*d++=(unsigned char)value;return ret;}

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, DWORD PTR _dst$[ebp]
	mov	DWORD PTR _d$[ebp], eax
	mov	ecx, DWORD PTR _dst$[ebp]
	mov	DWORD PTR _ret$[ebp], ecx
$L712:
	mov	edx, DWORD PTR _n$[ebp]
	mov	eax, DWORD PTR _n$[ebp]
	sub	eax, 1
	mov	DWORD PTR _n$[ebp], eax
	test	edx, edx
	je	SHORT $L713
	mov	ecx, DWORD PTR _d$[ebp]
	mov	dl, BYTE PTR _value$[ebp]
	mov	BYTE PTR [ecx], dl
	mov	eax, DWORD PTR _d$[ebp]
	add	eax, 1
	mov	DWORD PTR _d$[ebp], eax
	jmp	SHORT $L712
$L713:
	mov	eax, DWORD PTR _ret$[ebp]
	mov	esp, ebp
	pop	ebp
	ret	0
_pacc_hash_memset ENDP
_TEXT	ENDS
END
