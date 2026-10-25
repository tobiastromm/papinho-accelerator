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
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	_br_range_enc32be
_TEXT	SEGMENT
_dst$ = 8
_v$ = 12
_num$ = 16
_buf$ = -4
_br_range_enc32be PROC NEAR

; 30   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8

; 31   : 	unsigned char *buf;
; 32   : 
; 33   : 	buf = dst;

	mov	eax, DWORD PTR _dst$[ebp]
	mov	DWORD PTR _buf$[ebp], eax
$L1079:

; 34   : 	while (num -- > 0) {

	mov	ecx, DWORD PTR _num$[ebp]
	mov	edx, DWORD PTR _num$[ebp]
	sub	edx, 1
	mov	DWORD PTR _num$[ebp], edx
	test	ecx, ecx
	jbe	SHORT $L1080

; 35   : 		br_enc32be(buf, *v ++);

	mov	eax, DWORD PTR _v$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR -8+[ebp], ecx
	mov	edx, DWORD PTR -8+[ebp]
	push	edx
	mov	eax, DWORD PTR _buf$[ebp]
	push	eax
	mov	ecx, DWORD PTR _v$[ebp]
	add	ecx, 4
	mov	DWORD PTR _v$[ebp], ecx
	call	_br_enc32be
	add	esp, 8

; 36   : 		buf += 4;

	mov	edx, DWORD PTR _buf$[ebp]
	add	edx, 4
	mov	DWORD PTR _buf$[ebp], edx

; 37   : 	}

	jmp	SHORT $L1079
$L1080:

; 38   : }

	mov	esp, ebp
	pop	ebp
	ret	0
_br_range_enc32be ENDP
_dst$ = 8
_x$ = 12
_buf$ = -4
_br_enc32be PROC NEAR

; 36   : {

	push	ebp
	mov	ebp, esp
	push	ecx

; 37   : #if BR_BE_UNALIGNED
; 38   : 	((br_union_u32 *)dst)->u = x;
; 39   : #else
; 40   : 	unsigned char *buf;
; 41   : 
; 42   : 	buf = dst;

	mov	eax, DWORD PTR _dst$[ebp]
	mov	DWORD PTR _buf$[ebp], eax

; 43   : 	buf[0] = (unsigned char)(x >> 24);

	mov	ecx, DWORD PTR _x$[ebp]
	shr	ecx, 24					; 00000018H
	mov	edx, DWORD PTR _buf$[ebp]
	mov	BYTE PTR [edx], cl

; 44   : 	buf[1] = (unsigned char)(x >> 16);

	mov	eax, DWORD PTR _x$[ebp]
	shr	eax, 16					; 00000010H
	mov	ecx, DWORD PTR _buf$[ebp]
	mov	BYTE PTR [ecx+1], al

; 45   : 	buf[2] = (unsigned char)(x >> 8);

	mov	edx, DWORD PTR _x$[ebp]
	shr	edx, 8
	mov	eax, DWORD PTR _buf$[ebp]
	mov	BYTE PTR [eax+2], dl

; 46   : 	buf[3] = (unsigned char)x;

	mov	ecx, DWORD PTR _buf$[ebp]
	mov	dl, BYTE PTR _x$[ebp]
	mov	BYTE PTR [ecx+3], dl

; 47   : #endif
; 48   : }

	mov	esp, ebp
	pop	ebp
	ret	0
_br_enc32be ENDP
_TEXT	ENDS
END
