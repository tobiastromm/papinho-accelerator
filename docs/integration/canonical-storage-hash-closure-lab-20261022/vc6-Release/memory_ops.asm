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
;	COMDAT _pacc_hash_memcpy
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _pacc_hash_memset
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	_pacc_hash_memcpy
;	COMDAT _pacc_hash_memcpy
_TEXT	SEGMENT
_dst$ = 8
_src$ = 12
_n$ = 16
_pacc_hash_memcpy PROC NEAR				; COMDAT

; 5    : {volatile unsigned char *d=dst;const unsigned char *p=src;void *ret=dst;while(n--)*d++=*p++;return ret;}

	push	esi
	mov	esi, DWORD PTR _n$[esp]
	push	edi
	mov	edi, DWORD PTR _dst$[esp+4]
	mov	eax, esi
	dec	esi
	test	eax, eax
	mov	ecx, edi
	je	SHORT $L747
	mov	edx, DWORD PTR _src$[esp+4]
	sub	edx, edi
	inc	esi
$L725:
	mov	al, BYTE PTR [edx+ecx]
	mov	BYTE PTR [ecx], al
	inc	ecx
	dec	esi
	jne	SHORT $L725
$L747:
	mov	eax, edi
	pop	edi
	pop	esi
	ret	0
_pacc_hash_memcpy ENDP
_TEXT	ENDS
PUBLIC	_pacc_hash_memset
;	COMDAT _pacc_hash_memset
_TEXT	SEGMENT
_dst$ = 8
_value$ = 12
_n$ = 16
_pacc_hash_memset PROC NEAR				; COMDAT

; 7    : {volatile unsigned char *d=dst;void *ret=dst;while(n--)*d++=(unsigned char)value;return ret;}

	mov	edx, DWORD PTR _n$[esp-4]
	mov	eax, DWORD PTR _dst$[esp-4]
	push	esi
	mov	esi, edx
	dec	edx
	mov	ecx, eax
	test	esi, esi
	pop	esi
	je	SHORT $L738
	push	ebx
	mov	bl, BYTE PTR _value$[esp]
	inc	edx
$L737:
	mov	BYTE PTR [ecx], bl
	inc	ecx
	dec	edx
	jne	SHORT $L737
	pop	ebx
$L738:
	ret	0
_pacc_hash_memset ENDP
_TEXT	ENDS
END
