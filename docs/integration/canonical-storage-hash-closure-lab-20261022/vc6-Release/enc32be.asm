	TITLE	C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\vendor\bearssl-0.6\src\codec\enc32be.c
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
;	COMDAT _br_range_enc32be
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	_br_range_enc32be
;	COMDAT _br_range_enc32be
_TEXT	SEGMENT
_dst$ = 8
_v$ = 12
_num$ = 16
_br_range_enc32be PROC NEAR				; COMDAT

; 31   : 	unsigned char *buf;
; 32   : 
; 33   : 	buf = dst;
; 34   : 	while (num -- > 0) {

	mov	ecx, DWORD PTR _num$[esp-4]
	mov	eax, ecx
	dec	ecx
	test	eax, eax
	jbe	SHORT $L1105
	mov	edx, DWORD PTR _dst$[esp-4]
	push	esi
	mov	esi, DWORD PTR _v$[esp]
	push	edi
	lea	eax, DWORD PTR [edx+2]
	lea	edi, DWORD PTR [ecx+1]
$L1104:

; 35   : 		br_enc32be(buf, *v ++);

	mov	ecx, DWORD PTR [esi]
	add	esi, 4
	mov	edx, ecx
	mov	BYTE PTR [eax+1], cl
	shr	edx, 24					; 00000018H
	mov	BYTE PTR [eax-2], dl
	mov	edx, ecx
	shr	edx, 16					; 00000010H
	mov	BYTE PTR [eax-1], dl
	mov	edx, ecx
	shr	edx, 8
	mov	BYTE PTR [eax], dl

; 36   : 		buf += 4;

	add	eax, 4
	dec	edi
	jne	SHORT $L1104
	pop	edi
	pop	esi
$L1105:

; 37   : 	}
; 38   : }

	ret	0
_br_range_enc32be ENDP
_TEXT	ENDS
END
