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
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	_br_range_dec32be
_TEXT	SEGMENT
_v$ = 8
_num$ = 12
_src$ = 16
_buf$ = -4
_br_range_dec32be PROC NEAR

; 30   : {

	push	ebp
	mov	ebp, esp
	push	ecx

; 31   : 	const unsigned char *buf;
; 32   : 
; 33   : 	buf = src;

	mov	eax, DWORD PTR _src$[ebp]
	mov	DWORD PTR _buf$[ebp], eax
$L1079:

; 34   : 	while (num -- > 0) {

	mov	ecx, DWORD PTR _num$[ebp]
	mov	edx, DWORD PTR _num$[ebp]
	sub	edx, 1
	mov	DWORD PTR _num$[ebp], edx
	test	ecx, ecx
	jbe	SHORT $L1080

; 35   : 		*v ++ = br_dec32be(buf);

	mov	eax, DWORD PTR _buf$[ebp]
	push	eax
	call	_br_dec32be
	add	esp, 4
	mov	ecx, DWORD PTR _v$[ebp]
	mov	DWORD PTR [ecx], eax
	mov	edx, DWORD PTR _v$[ebp]
	add	edx, 4
	mov	DWORD PTR _v$[ebp], edx

; 36   : 		buf += 4;

	mov	eax, DWORD PTR _buf$[ebp]
	add	eax, 4
	mov	DWORD PTR _buf$[ebp], eax

; 37   : 	}

	jmp	SHORT $L1079
$L1080:

; 38   : }

	mov	esp, ebp
	pop	ebp
	ret	0
_br_range_dec32be ENDP
_src$ = 8
_buf$ = -4
_br_dec32be PROC NEAR

; 51   : {

	push	ebp
	mov	ebp, esp
	push	ecx

; 52   : #if BR_BE_UNALIGNED
; 53   : 	return ((const br_union_u32 *)src)->u;
; 54   : #else
; 55   : 	const unsigned char *buf;
; 56   : 
; 57   : 	buf = src;

	mov	eax, DWORD PTR _src$[ebp]
	mov	DWORD PTR _buf$[ebp], eax

; 58   : 	return ((uint32_t)buf[0] << 24)
; 59   : 		| ((uint32_t)buf[1] << 16)
; 60   : 		| ((uint32_t)buf[2] << 8)
; 61   : 		| (uint32_t)buf[3];

	mov	ecx, DWORD PTR _buf$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx]
	mov	eax, edx
	shl	eax, 24					; 00000018H
	mov	ecx, DWORD PTR _buf$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+1]
	shl	edx, 16					; 00000010H
	or	eax, edx
	mov	ecx, DWORD PTR _buf$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+2]
	shl	edx, 8
	or	eax, edx
	mov	ecx, DWORD PTR _buf$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+3]
	or	eax, edx

; 62   : #endif
; 63   : }

	mov	esp, ebp
	pop	ebp
	ret	0
_br_dec32be ENDP
_TEXT	ENDS
END
