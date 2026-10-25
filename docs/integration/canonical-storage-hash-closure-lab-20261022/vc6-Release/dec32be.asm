	TITLE	C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\vendor\bearssl-0.6\src\codec\dec32be.c
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
;	COMDAT _br_enc32be
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _br_dec32be
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _br_range_dec32be
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	_br_range_dec32be
;	COMDAT _br_range_dec32be
_TEXT	SEGMENT
_v$ = 8
_num$ = 12
_src$ = 16
_br_range_dec32be PROC NEAR				; COMDAT

; 31   : 	const unsigned char *buf;
; 32   : 
; 33   : 	buf = src;
; 34   : 	while (num -- > 0) {

	mov	ecx, DWORD PTR _num$[esp-4]
	mov	eax, ecx
	dec	ecx
	test	eax, eax
	jbe	SHORT $L1105
	mov	edx, DWORD PTR _src$[esp-4]
	push	esi
	mov	esi, DWORD PTR _v$[esp]
	push	edi
	lea	eax, DWORD PTR [edx+2]
	lea	edi, DWORD PTR [ecx+1]
$L1104:

; 35   : 		*v ++ = br_dec32be(buf);

	xor	ecx, ecx
	xor	edx, edx
	mov	ch, BYTE PTR [eax-2]
	mov	dl, BYTE PTR [eax]
	mov	cl, BYTE PTR [eax-1]
	add	esi, 4
	shl	ecx, 8
	or	ecx, edx
	xor	edx, edx
	mov	dl, BYTE PTR [eax+1]

; 36   : 		buf += 4;

	add	eax, 4
	shl	ecx, 8
	or	ecx, edx
	dec	edi
	mov	DWORD PTR [esi-4], ecx
	jne	SHORT $L1104
	pop	edi
	pop	esi
$L1105:

; 37   : 	}
; 38   : }

	ret	0
_br_range_dec32be ENDP
_TEXT	ENDS
END
