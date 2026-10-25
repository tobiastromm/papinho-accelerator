	TITLE	C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\closure_probe.c
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
_BSS	SEGMENT PARA USE32 PUBLIC 'BSS'
_BSS	ENDS
_TLS	SEGMENT DWORD USE32 PUBLIC 'TLS'
_TLS	ENDS
;	COMDAT ??_C@_01FNLH@a?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_03MPFC@abc?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_02FKDL@bc?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT _probe_main
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
CONST	SEGMENT
_expected DB	0baH
	DB	078H
	DB	016H
	DB	0bfH
	DB	08fH
	DB	01H
	DB	0cfH
	DB	0eaH
	DB	041H
	DB	041H
	DB	040H
	DB	0deH
	DB	05dH
	DB	0aeH
	DB	022H
	DB	023H
	DB	0b0H
	DB	03H
	DB	061H
	DB	0a3H
	DB	096H
	DB	017H
	DB	07aH
	DB	09cH
	DB	0b4H
	DB	010H
	DB	0ffH
	DB	061H
	DB	0f2H
	DB	00H
	DB	015H
	DB	0adH
CONST	ENDS
PUBLIC	_probe_main
PUBLIC	??_C@_01FNLH@a?$AA@				; `string'
PUBLIC	??_C@_03MPFC@abc?$AA@				; `string'
PUBLIC	??_C@_02FKDL@bc?$AA@				; `string'
EXTRN	_pacc_storage_sha_init:NEAR
EXTRN	_pacc_storage_sha_update:NEAR
EXTRN	_pacc_storage_sha_final:NEAR
EXTRN	_pacc_storage_hash_callback:NEAR
_BSS	SEGMENT
_a	DB	080H DUP (?)
_b	DB	080H DUP (?)
_x	DB	020H DUP (?)
_y	DB	020H DUP (?)
_BSS	ENDS
;	COMDAT ??_C@_01FNLH@a?$AA@
; File C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\closure_probe.c
_DATA	SEGMENT
??_C@_01FNLH@a?$AA@ DB 'a', 00H				; `string'
_DATA	ENDS
;	COMDAT ??_C@_03MPFC@abc?$AA@
_DATA	SEGMENT
??_C@_03MPFC@abc?$AA@ DB 'abc', 00H			; `string'
_DATA	ENDS
;	COMDAT ??_C@_02FKDL@bc?$AA@
_DATA	SEGMENT
??_C@_02FKDL@bc?$AA@ DB 'bc', 00H			; `string'
_DATA	ENDS
;	COMDAT _probe_main
_TEXT	SEGMENT
_probe_main PROC NEAR					; COMDAT

; 7    :  if(pacc_storage_sha_init(&a)||pacc_storage_sha_init(&b))return 1;

	push	OFFSET FLAT:_a
	call	_pacc_storage_sha_init
	add	esp, 4
	test	eax, eax
	jne	$L1101
	push	OFFSET FLAT:_b
	call	_pacc_storage_sha_init
	add	esp, 4
	test	eax, eax
	jne	$L1101

; 8    :  if(pacc_storage_sha_update(&a,"a",1,0,0)||pacc_storage_sha_update(&b,"abc",3,0,0)||pacc_storage_sha_update(&a,"bc",2,0,0))return 2;

	push	eax
	push	eax
	push	1
	push	OFFSET FLAT:??_C@_01FNLH@a?$AA@		; `string'
	push	OFFSET FLAT:_a
	call	_pacc_storage_sha_update
	add	esp, 20					; 00000014H
	test	eax, eax
	jne	$L1103
	push	eax
	push	eax
	push	3
	push	OFFSET FLAT:??_C@_03MPFC@abc?$AA@	; `string'
	push	OFFSET FLAT:_b
	call	_pacc_storage_sha_update
	add	esp, 20					; 00000014H
	test	eax, eax
	jne	$L1103
	push	eax
	push	eax
	push	2
	push	OFFSET FLAT:??_C@_02FKDL@bc?$AA@	; `string'
	push	OFFSET FLAT:_a
	call	_pacc_storage_sha_update
	add	esp, 20					; 00000014H
	test	eax, eax
	jne	$L1103

; 9    :  if(pacc_storage_sha_final(&a,x,32,0,0)||pacc_storage_sha_final(&b,y,32,0,0))return 3;

	push	eax
	push	eax
	push	32					; 00000020H
	push	OFFSET FLAT:_x
	push	OFFSET FLAT:_a
	call	_pacc_storage_sha_final
	add	esp, 20					; 00000014H
	test	eax, eax
	jne	SHORT $L1108
	push	eax
	push	eax
	push	32					; 00000020H
	push	OFFSET FLAT:_y
	push	OFFSET FLAT:_b
	call	_pacc_storage_sha_final
	add	esp, 20					; 00000014H
	test	eax, eax
	jne	SHORT $L1108
$L1109:

; 10   :  for(i=0;i<32;i++)if(x[i]!=expected[i]||y[i]!=expected[i])return 4;

	mov	cl, BYTE PTR _expected[eax]
	mov	dl, BYTE PTR _x[eax]
	cmp	dl, cl
	jne	SHORT $L1129
	cmp	BYTE PTR _y[eax], cl
	jne	SHORT $L1129
	inc	eax
	cmp	eax, 32					; 00000020H
	jb	SHORT $L1109

; 11   :  if(!pacc_storage_hash_callback(0,(const unsigned char *)"abc",3,x))return 5;

	push	OFFSET FLAT:_x
	push	3
	push	OFFSET FLAT:??_C@_03MPFC@abc?$AA@	; `string'
	push	0
	call	_pacc_storage_hash_callback
	add	esp, 16					; 00000010H
	test	eax, eax
	jne	SHORT $L1115
	mov	eax, 5

; 13   :  return 0;}

	ret	0
$L1129:

; 10   :  for(i=0;i<32;i++)if(x[i]!=expected[i]||y[i]!=expected[i])return 4;

	mov	eax, 4

; 13   :  return 0;}

	ret	0
$L1115:

; 12   :  for(i=0;i<32;i++)if(x[i]!=expected[i])return 6;

	xor	eax, eax
$L1116:
	mov	cl, BYTE PTR _x[eax]
	mov	dl, BYTE PTR _expected[eax]
	cmp	cl, dl
	jne	SHORT $L1130
	inc	eax
	cmp	eax, 32					; 00000020H
	jb	SHORT $L1116

; 13   :  return 0;}

	xor	eax, eax
	ret	0
$L1130:

; 12   :  for(i=0;i<32;i++)if(x[i]!=expected[i])return 6;

	mov	eax, 6

; 13   :  return 0;}

	ret	0
$L1108:

; 9    :  if(pacc_storage_sha_final(&a,x,32,0,0)||pacc_storage_sha_final(&b,y,32,0,0))return 3;

	mov	eax, 3

; 13   :  return 0;}

	ret	0
$L1103:

; 8    :  if(pacc_storage_sha_update(&a,"a",1,0,0)||pacc_storage_sha_update(&b,"abc",3,0,0)||pacc_storage_sha_update(&a,"bc",2,0,0))return 2;

	mov	eax, 2

; 13   :  return 0;}

	ret	0
$L1101:

; 7    :  if(pacc_storage_sha_init(&a)||pacc_storage_sha_init(&b))return 1;

	mov	eax, 1

; 13   :  return 0;}

	ret	0
_probe_main ENDP
_TEXT	ENDS
END
