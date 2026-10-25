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
_DATA	SEGMENT
$SG1079	DB	'a', 00H
	ORG $+2
$SG1080	DB	'abc', 00H
$SG1081	DB	'bc', 00H
	ORG $+1
$SG1091	DB	'abc', 00H
_DATA	ENDS
_TEXT	SEGMENT
_i$ = -4
_probe_main PROC NEAR

; 6    : int probe_main(void){unsigned i;

	push	ebp
	mov	ebp, esp
	push	ecx

; 7    :  if(pacc_storage_sha_init(&a)||pacc_storage_sha_init(&b))return 1;

	push	OFFSET FLAT:_a
	call	_pacc_storage_sha_init
	add	esp, 4
	test	eax, eax
	jne	SHORT $L1076
	push	OFFSET FLAT:_b
	call	_pacc_storage_sha_init
	add	esp, 4
	test	eax, eax
	je	SHORT $L1075
$L1076:
	mov	eax, 1
	jmp	$L1073
$L1075:

; 8    :  if(pacc_storage_sha_update(&a,"a",1,0,0)||pacc_storage_sha_update(&b,"abc",3,0,0)||pacc_storage_sha_update(&a,"bc",2,0,0))return 2;

	push	0
	push	0
	push	1
	push	OFFSET FLAT:$SG1079
	push	OFFSET FLAT:_a
	call	_pacc_storage_sha_update
	add	esp, 20					; 00000014H
	test	eax, eax
	jne	SHORT $L1078
	push	0
	push	0
	push	3
	push	OFFSET FLAT:$SG1080
	push	OFFSET FLAT:_b
	call	_pacc_storage_sha_update
	add	esp, 20					; 00000014H
	test	eax, eax
	jne	SHORT $L1078
	push	0
	push	0
	push	2
	push	OFFSET FLAT:$SG1081
	push	OFFSET FLAT:_a
	call	_pacc_storage_sha_update
	add	esp, 20					; 00000014H
	test	eax, eax
	je	SHORT $L1077
$L1078:
	mov	eax, 2
	jmp	$L1073
$L1077:

; 9    :  if(pacc_storage_sha_final(&a,x,32,0,0)||pacc_storage_sha_final(&b,y,32,0,0))return 3;

	push	0
	push	0
	push	32					; 00000020H
	push	OFFSET FLAT:_x
	push	OFFSET FLAT:_a
	call	_pacc_storage_sha_final
	add	esp, 20					; 00000014H
	test	eax, eax
	jne	SHORT $L1083
	push	0
	push	0
	push	32					; 00000020H
	push	OFFSET FLAT:_y
	push	OFFSET FLAT:_b
	call	_pacc_storage_sha_final
	add	esp, 20					; 00000014H
	test	eax, eax
	je	SHORT $L1082
$L1083:
	mov	eax, 3
	jmp	$L1073
$L1082:

; 10   :  for(i=0;i<32;i++)if(x[i]!=expected[i]||y[i]!=expected[i])return 4;

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L1084
$L1085:
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 1
	mov	DWORD PTR _i$[ebp], eax
$L1084:
	cmp	DWORD PTR _i$[ebp], 32			; 00000020H
	jae	SHORT $L1086
	mov	ecx, DWORD PTR _i$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR _x[ecx]
	mov	eax, DWORD PTR _i$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR _expected[eax]
	cmp	edx, ecx
	jne	SHORT $L1088
	mov	edx, DWORD PTR _i$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR _y[edx]
	mov	ecx, DWORD PTR _i$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR _expected[ecx]
	cmp	eax, edx
	je	SHORT $L1087
$L1088:
	mov	eax, 4
	jmp	SHORT $L1073
$L1087:

; 11   :  if(!pacc_storage_hash_callback(0,(const unsigned char *)"abc",3,x))return 5;

	jmp	SHORT $L1085
$L1086:
	push	OFFSET FLAT:_x
	push	3
	push	OFFSET FLAT:$SG1091
	push	0
	call	_pacc_storage_hash_callback
	add	esp, 16					; 00000010H
	test	eax, eax
	jne	SHORT $L1090
	mov	eax, 5
	jmp	SHORT $L1073
$L1090:

; 12   :  for(i=0;i<32;i++)if(x[i]!=expected[i])return 6;

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L1092
$L1093:
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 1
	mov	DWORD PTR _i$[ebp], eax
$L1092:
	cmp	DWORD PTR _i$[ebp], 32			; 00000020H
	jae	SHORT $L1094
	mov	ecx, DWORD PTR _i$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR _x[ecx]
	mov	eax, DWORD PTR _i$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR _expected[eax]
	cmp	edx, ecx
	je	SHORT $L1095
	mov	eax, 6
	jmp	SHORT $L1073
$L1095:

; 13   :  return 0;}

	jmp	SHORT $L1093
$L1094:
	xor	eax, eax
$L1073:
	mov	esp, ebp
	pop	ebp
	ret	0
_probe_main ENDP
_TEXT	ENDS
END
