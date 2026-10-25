	TITLE	C:\Projetos\PapinhoAccelerator\src\issuer\strict_ed25519.c
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
;	COMDAT _load_3
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _load_4
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _fe_frombytes
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _fe_tobytes
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _fe_copy
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _fe_0
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _fe_1
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _fe_add
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _fe_sub
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _fe_mul
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _fe_sq
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _fe_invert
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _fe_neg
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _fe_isnonzero
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _fe_isnegative
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _fe_sq2
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _fe_pow22523
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _ge_tobytes
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _ge_p3_tobytes
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _ge_frombytes_vartime
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _ge_p2_0
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _ge_p3_to_p2
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _ge_p3_to_cached
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _ge_p1p1_to_p2
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _ge_p1p1_to_p3
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _ge_p2_dbl
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _ge_p3_dbl
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _ge_madd
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _ge_msub
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _ge_add
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _ge_sub
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _slide
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _ge_double_scalarmult_vartime
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _papacc_time_source_ed25519_check
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
CONST	SEGMENT
_kBottom25Bits DQ 0000000001ffffffH
_kBottom26Bits DQ 0000000003ffffffH
_kTop39Bits DQ	fffffffffe000000H
_kTop38Bits DQ	fffffffffc000000H
_?zero@?1??fe_isnonzero@@9@9 DB 00H
	ORG $+31
_d	DD	0ff5978b6H
	DD	0d37285H
	DD	0ff156ebdH
	DD	06a0a0fH
	DD	01c029H
	DD	0ff79e898H
	DD	0ffa03cbcH
	DD	0ffce7199H
	DD	0fee2b700H
	DD	0ff480db4H
_sqrtm1	DD	0fe0ea0b0H
	DD	0ff86c9d3H
	DD	08f189eH
	DD	035697fH
	DD	0bd0c60H
	DD	0fffbd7a7H
	DD	0fe804c9fH
	DD	0ffe1656aH
	DD	04fc1eH
	DD	0ae0c92H
_d2	DD	0feb2f159H
	DD	0ffa6e50aH
	DD	0fe2add7bH
	DD	0d4141eH
	DD	038052H
	DD	0f3d130H
	DD	0ff407977H
	DD	0ff9ce332H
	DD	01c56e00H
	DD	0901b67H
_Bi	DD	018c3b85H
	DD	0ff24f1bdH
	DD	01c325f8H
	DD	037dc60H
	DD	0ff3e4cb7H
	DD	03d42c3H
	DD	01a44c32H
	DD	0ff4ca4e1H
	DD	0ffa33d4cH
	DD	01f3e75H
	DD	0ff409151H
	DD	0e4176H
	DD	0ffd673a2H
	DD	02e8a06H
	DD	0fff4e67cH
	DD	08f8a0aH
	DD	0c21a34H
	DD	04cf4b8H
	DD	01298f81H
	DD	0ff13f4beH
	DD	0ff7aaa7bH
	DD	0448162H
	DD	093d579H
	DD	0ff1e6556H
	DD	09b67a1H
	DD	0ff43598cH
	DD	01bee5efH
	DD	0b50b43H
	DD	0fe89f0c6H
	DD	0ffbc45eeH
	DD	0ee9743H
	DD	0ff6c2a13H
	DD	017155e5H
	DD	0ff874432H
	DD	096a11H
	DD	0ff016732H
	DD	01a80150H
	DD	0ff1e9823H
	DD	01b9a810H
	DD	0ffe85938H
	DD	0fcd265H
	DD	047fa29H
	DD	0ff4faaccH
	DD	0ffef2e0eH
	DD	0ef4d50H
	DD	0ff4bd6bdH
	DD	0f98d11H
	DD	0ff4c5026H
	DD	07555beH
	DD	0aae456H
	DD	01d0d89cH
	DD	0ffa4cfc3H
	DD	0ff4c4296H
	DD	0ff10e1afH
	DD	0162508dH
	DD	0f2db4cH
	DD	072a2c6H
	DD	098da2eH
	DD	0fef12b9bH
	DD	0ff68a09bH
	DD	0a5bb33H
	DD	0af1102H
	DD	01a05442H
	DD	01e3af7H
	DD	0fe354123H
	DD	0bfec45H
	DD	01f5862dH
	DD	0dd7ba3H
	DD	0ff146e20H
	DD	0a51734H
	DD	047d6baH
	DD	060b0e9H
	DD	0136eff2H
	DD	08a5939H
	DD	0ff540053H
	DD	064a088H
	DD	0fe788e5cH
	DD	0be7c68H
	DD	0ff3eb1b5H
	DD	05529faH
	DD	012a8298H
	DD	0f6fc60H
	DD	0fe3f9797H
	DD	03e85efH
	DD	09c3820H
	DD	0ffbda72dH
	DD	01b3858eH
	DD	0d35683H
	DD	0fe96b3bbH
	DD	0ff0eaafaH
	DD	04ea3d2H
	DD	0973425H
	DD	01a4d63H
	DD	0ffd59ceeH
	DD	01d1c0d5H
	DD	0542e49H
	DD	01294114H
	DD	04fce36H
	DD	0fe9283c9H
	DD	0ff186faaH
	DD	0fe3221b1H
	DD	0ffcb26abH
	DD	074f74eH
	DD	099ddd1H
	DD	01b28085H
	DD	0192c3aH
	DD	013b27c9H
	DD	0fc13bdH
	DD	01d2e531H
	DD	075bb75H
	DD	01b8b3b5H
	DD	0db7200H
	DD	0935e30H
	DD	03829f5H
	DD	0fecc0d7dH
	DD	077adf4H
	DD	0fe20dd2cH
	DD	014ea54H
	DD	01c6a0f9H
	DD	0ffea7eecH
	DD	0fea8632fH
	DD	0ff99e2aaH
	DD	0d8b366H
	DD	0ff7a8de2H
	DD	0fe99427aH
	DD	086f5b6H
	DD	0119e4e3H
	DD	0ffeb39d6H
	DD	0ff38add8H
	DD	0d2e7b5H
	DD	0ff9d8077H
	DD	0ff885f81H
	DD	0337e6eH
	DD	0ffb7a902H
	DD	0fe628207H
	DD	0ff5eb045H
	DD	01e30474H
	DD	0ff91f2d9H
	DD	011fadcaH
	DD	0ff270169H
	DD	045af2eH
	DD	0ff3a2fe4H
	DD	0fe45e0d7H
	DD	0ff4538cfH
	DD	0ff8bfe10H
	DD	0ffd4cf17H
	DD	0ff7e14caH
	DD	0ff60d55fH
	DD	021b009H
	DD	0ffcf05c8H
	DD	0fe802af1H
	DD	0ffc02123H
	DD	01c4e5f8H
	DD	0781181H
	DD	0ff9767fbH
	DD	0ff703407H
	DD	0ff42388cH
	DD	0fff5e228H
	DD	0fe2546d9H
	DD	0ff09d6acH
	DD	01864348H
	DD	0ffd6c092H
	DD	070262cH
	DD	0ff4bb844H
	DD	0fb5aceH
	DD	08deb95H
	DD	03aaab5H
	DD	0eff474H
	DD	029d5cH
	DD	062ad66H
	DD	016089e9H
	DD	0cb317fH
	DD	0949b05H
	DD	0ff099417H
	DD	0c7ad3H
	DD	0ff1a8622H
	DD	088ccdbH
	DD	0ff290886H
	DD	0fe2b53e0H
	DD	0f71955H
	DD	0fe007f6dH
	DD	03088a9H
	DD	0ffdb77eeH
	DD	0d5ade7H
	DD	0fefe12ceH
	DD	0ff07ba08H
	DD	0107097eH
	DD	0482a6fH
	DD	0feec346fH
	DD	08d3f60H
	DD	0fe7fbf93H
	DD	0ffc04ecdH
	DD	01ed6a0eH
	DD	04cdbbbH
	DD	0febbf3afH
	DD	0ad5969H
	DD	01591955H
	DD	094f3a2H
	DD	0fed17602H
	DD	099e21H
	DD	0ff2ea38bH
	DD	028465dH
	DD	0fe8e2a6cH
	DD	0ff8efc6fH
	DD	090df9bH
	DD	0ffa7e533H
	DD	0ff9bfc49H
	DD	0ff0c745eH
	DD	0ffdaa098H
	DD	0ff25ee9cH
	DD	0ffcfeab3H
	DD	0ffb300c5H
	DD	08da49aH
	DD	068c4e1H
	DD	0fe19230aH
	DD	0fff2d4d1H
	DD	0fedefd61H
	DD	0e565b8H
	DD	017f12deH
	DD	0ff8788a4H
	DD	0fe8ccf1eH
	DD	0f36192H
	DD	0fe1ac081H
	DD	0ff2154c9H
	DD	0ff4e0a6fH
	DD	0ffb25193H
	DD	0180404H
	DD	0ffd7eea1H
	DD	0218d06H
	DD	0ff0ed735H
	DD	0ffd0b529H
	DD	09d8be7H
	DD	0ffddcbb3H
	DD	071b9ffH
	DD	0fface2bdH
	DD	0ffd64271H
	DD	0ff2d3ecaH
	DD	0ff084066H
	DD	0fe10ae4eH
	DD	0ff447585H
_?order@?1??papacc_time_source_ed25519_check@@9@9 DB 0edH
	DB	0d3H
	DB	0f5H
	DB	05cH
	DB	01aH
	DB	063H
	DB	012H
	DB	058H
	DB	0d6H
	DB	09cH
	DB	0f7H
	DB	0a2H
	DB	0deH
	DB	0f9H
	DB	0deH
	DB	014H
	DB	00H
	DB	00H
	DB	00H
	DB	00H
	DB	00H
	DB	00H
	DB	00H
	DB	00H
	DB	00H
	DB	00H
	DB	00H
	DB	00H
	DB	00H
	DB	00H
	DB	00H
	DB	010H
_?zero@?1??papacc_time_source_ed25519_check@@9@9 DB 00H
	ORG $+31
_?identity@?1??papacc_time_source_ed25519_check@@9@9 DB 01H
	ORG $+31
CONST	ENDS
PUBLIC	_papacc_time_source_ed25519_check
;	COMDAT _papacc_time_source_ed25519_check
_TEXT	SEGMENT
_key$ = 8
_bytes$ = 12
_point$ = -160
_multiple$ = -280
_encoded$ = -312
_papacc_time_source_ed25519_check PROC NEAR		; COMDAT

; 1442 : {

	sub	esp, 312				; 00000138H
	push	esi
	push	edi

; 1443 :     static const unsigned char order[32] = {
; 1444 :         0xed,0xd3,0xf5,0x5c,0x1a,0x63,0x12,0x58,
; 1445 :         0xd6,0x9c,0xf7,0xa2,0xde,0xf9,0xde,0x14,
; 1446 :         0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x10
; 1447 :     };
; 1448 :     static const unsigned char zero[32] = {0};
; 1449 :     static const unsigned char identity[32] = {1};
; 1450 :     ge_p3 point;
; 1451 :     ge_p2 multiple;
; 1452 :     unsigned char encoded[32];
; 1453 :     unsigned int i;
; 1454 : 
; 1455 :     if (key == NULL || bytes != 32)

	mov	edi, DWORD PTR _key$[esp+316]
	test	edi, edi
	je	$L2021
	cmp	DWORD PTR _bytes$[esp+316], 32		; 00000020H
	jne	$L2021

; 1457 :     if (ge_frombytes_vartime(&point, key) != 0)

	lea	eax, DWORD PTR _point$[esp+320]
	push	edi
	push	eax
	call	_ge_frombytes_vartime
	add	esp, 8
	test	eax, eax

; 1458 :         return 0;

	jne	$L2021

; 1459 :     ge_p3_tobytes(encoded, &point);

	lea	ecx, DWORD PTR _point$[esp+320]
	lea	edx, DWORD PTR _encoded$[esp+320]
	push	ecx
	push	edx
	call	_ge_p3_tobytes
	add	esp, 8

; 1460 :     if (memcmp(encoded, key, 32) != 0)

	mov	ecx, 8
	lea	esi, DWORD PTR _encoded$[esp+320]
	xor	eax, eax
	repe cmpsd

; 1461 :         return 0;

	jne	$L2021

; 1462 :     if (memcmp(encoded, identity, 32) == 0)

	mov	ecx, 8
	mov	edi, OFFSET FLAT:_?identity@?1??papacc_time_source_ed25519_check@@9@9
	lea	esi, DWORD PTR _encoded$[esp+320]
	xor	edx, edx
	repe cmpsd

; 1463 :         return 0;

	je	SHORT $L2021

; 1464 :     /* Integer L, NOT x25519_sc_reduce(L), NOT cofactor-cleared input. */
; 1465 :     ge_double_scalarmult_vartime(&multiple, order, &point, zero);

	lea	eax, DWORD PTR _point$[esp+320]
	push	OFFSET FLAT:_?zero@?1??papacc_time_source_ed25519_check@@9@9
	push	eax
	lea	ecx, DWORD PTR _multiple$[esp+328]
	push	OFFSET FLAT:_?order@?1??papacc_time_source_ed25519_check@@9@9
	push	ecx
	call	_ge_double_scalarmult_vartime

; 1466 :     /* Projective zero denominator is an internal invariant failure. */
; 1467 :     if (fe_isnonzero(multiple.Z) == 0)

	lea	edx, DWORD PTR _multiple$[esp+416]
	push	edx
	call	_fe_isnonzero
	add	esp, 20					; 00000014H
	test	eax, eax
	jne	SHORT $L1977
	pop	edi

; 1468 :         return -1;

	or	eax, -1
	pop	esi

; 1475 : }

	add	esp, 312				; 00000138H
	ret	0
$L1977:

; 1469 :     ge_tobytes(encoded, &multiple);

	lea	eax, DWORD PTR _multiple$[esp+320]
	lea	ecx, DWORD PTR _encoded$[esp+320]
	push	eax
	push	ecx
	call	_ge_tobytes
	add	esp, 8

; 1470 :     for (i = 0; i < 32; ++i) {

	xor	eax, eax
$L1978:

; 1471 :         if (encoded[i] != identity[i])

	mov	dl, BYTE PTR _encoded$[esp+eax+320]
	mov	cl, BYTE PTR _?identity@?1??papacc_time_source_ed25519_check@@9@9[eax]
	cmp	dl, cl
	jne	SHORT $L2021
	inc	eax
	cmp	eax, 32					; 00000020H
	jb	SHORT $L1978
	pop	edi

; 1472 :             return 0;
; 1473 :     }
; 1474 :     return 1;

	mov	eax, 1
	pop	esi

; 1475 : }

	add	esp, 312				; 00000138H
	ret	0
$L2021:
	pop	edi

; 1456 :         return 0;

	xor	eax, eax
	pop	esi

; 1475 : }

	add	esp, 312				; 00000138H
	ret	0
_papacc_time_source_ed25519_check ENDP
_TEXT	ENDS
;	COMDAT _fe_isnonzero
_TEXT	SEGMENT
_f$ = 8
_s$ = -32
_fe_isnonzero PROC NEAR					; COMDAT

; 819  :     uint8_t s[32];
; 820  :     static const uint8_t zero[32] = { 0 };
; 821  : 
; 822  :     fe_tobytes(s, f);

	mov	eax, DWORD PTR _f$[esp-4]
	sub	esp, 32					; 00000020H
	lea	ecx, DWORD PTR _s$[esp+32]
	push	esi
	push	edi
	push	eax
	push	ecx
	call	_fe_tobytes
	add	esp, 8

; 823  : 
; 824  :     return memcmp(s, zero, sizeof(zero)) != 0;

	mov	ecx, 8
	mov	edi, OFFSET FLAT:_?zero@?1??fe_isnonzero@@9@9
	lea	esi, DWORD PTR _s$[esp+40]
	xor	edx, edx
	repe cmpsd
	mov	eax, edx
	pop	edi
	setne	al
	pop	esi

; 825  : }

	add	esp, 32					; 00000020H
	ret	0
_fe_isnonzero ENDP
_TEXT	ENDS
;	COMDAT _fe_tobytes
_TEXT	SEGMENT
_s$ = 8
_h$ = 12
_h0$ = -12
_h1$ = -12
_h2$ = -8
_h3$ = -4
_h4$ = -20
_h5$ = -16
_h6$ = 12
_h7$ = -28
_h8$ = -24
_fe_tobytes PROC NEAR					; COMDAT

; 191  : {

	sub	esp, 28					; 0000001cH

; 192  :     int32_t h0 = h[0];

	mov	eax, DWORD PTR _h$[esp+24]
	push	ebx
	push	ebp
	push	esi
	mov	ecx, DWORD PTR [eax]

; 193  :     int32_t h1 = h[1];

	mov	esi, DWORD PTR [eax+4]
	mov	DWORD PTR _h0$[esp+40], ecx

; 194  :     int32_t h2 = h[2];
; 195  :     int32_t h3 = h[3];
; 196  :     int32_t h4 = h[4];
; 197  :     int32_t h5 = h[5];
; 198  :     int32_t h6 = h[6];

	mov	ecx, DWORD PTR [eax+24]
	mov	DWORD PTR _h6$[esp+36], ecx

; 199  :     int32_t h7 = h[7];

	mov	ecx, DWORD PTR [eax+28]
	mov	ebx, DWORD PTR [eax+12]
	mov	edx, DWORD PTR [eax+16]
	mov	ebp, DWORD PTR [eax+20]
	mov	DWORD PTR _h7$[esp+40], ecx

; 200  :     int32_t h8 = h[8];

	mov	ecx, DWORD PTR [eax+32]
	push	edi
	mov	edi, DWORD PTR [eax+8]
	mov	DWORD PTR _h8$[esp+44], ecx

; 201  :     int32_t h9 = h[9];

	mov	ecx, DWORD PTR [eax+36]
	mov	DWORD PTR _h5$[esp+44], ebp
	mov	DWORD PTR _h4$[esp+44], edx

; 202  :     int32_t q;
; 203  : 
; 204  :     q = (19 * h9 + (((int32_t)1) << 24)) >> 25;

	lea	eax, DWORD PTR [ecx+ecx*8]
	lea	eax, DWORD PTR [ecx+eax*2+16777216]
	sar	eax, 25					; 00000019H

; 205  :     q = (h0 + q) >> 26;

	add	eax, DWORD PTR _h0$[esp+44]
	sar	eax, 26					; 0000001aH

; 206  :     q = (h1 + q) >> 25;

	add	eax, esi
	sar	eax, 25					; 00000019H

; 207  :     q = (h2 + q) >> 26;

	add	eax, edi
	sar	eax, 26					; 0000001aH

; 208  :     q = (h3 + q) >> 25;

	add	eax, ebx
	sar	eax, 25					; 00000019H

; 209  :     q = (h4 + q) >> 26;

	add	eax, edx
	sar	eax, 26					; 0000001aH

; 210  :     q = (h5 + q) >> 25;

	add	eax, ebp

; 211  :     q = (h6 + q) >> 26;

	mov	ebp, DWORD PTR _h6$[esp+40]
	sar	eax, 25					; 00000019H
	add	eax, ebp

; 212  :     q = (h7 + q) >> 25;

	mov	ebp, DWORD PTR _h7$[esp+44]
	sar	eax, 26					; 0000001aH
	add	eax, ebp

; 213  :     q = (h8 + q) >> 26;

	mov	ebp, DWORD PTR _h8$[esp+44]
	sar	eax, 25					; 00000019H
	add	eax, ebp

; 214  :     q = (h9 + q) >> 25;
; 215  : 
; 216  :     /* Goal: Output h-(2^255-19)q, which is between 0 and 2^255-20. */
; 217  :     h0 += 19 * q;

	mov	ebp, DWORD PTR _h0$[esp+44]
	sar	eax, 26					; 0000001aH
	add	eax, ecx
	sar	eax, 25					; 00000019H
	lea	edx, DWORD PTR [eax+eax*8]
	lea	edx, DWORD PTR [ebp+edx*2]

; 218  :     /* Goal: Output h-2^255 q, which is between 0 and 2^255-20. */
; 219  : 
; 220  :     h1 += h0 >> 26;
; 221  :     h0 &= kBottom26Bits;

	mov	ebp, DWORD PTR _kBottom26Bits
	add	edx, eax
	mov	eax, edx
	and	edx, ebp
	sar	eax, 26					; 0000001aH
	add	esi, eax

; 222  :     h2 += h1 >> 25;

	mov	eax, esi
	sar	eax, 25					; 00000019H
	add	edi, eax

; 223  :     h1 &= kBottom25Bits;

	mov	eax, DWORD PTR _kBottom25Bits
	and	esi, eax

; 224  :     h3 += h2 >> 26;

	mov	eax, edi
	sar	eax, 26					; 0000001aH
	add	ebx, eax

; 225  :     h2 &= kBottom26Bits;
; 226  :     h4 += h3 >> 25;

	mov	eax, DWORD PTR _h4$[esp+44]
	and	edi, ebp
	mov	DWORD PTR _h1$[esp+44], esi
	mov	DWORD PTR _h2$[esp+44], edi

; 227  :     h3 &= kBottom25Bits;

	mov	edi, DWORD PTR _kBottom25Bits
	mov	esi, ebx
	and	ebx, edi
	sar	esi, 25					; 00000019H
	add	eax, esi

; 228  :     h5 += h4 >> 26;

	mov	esi, DWORD PTR _h5$[esp+44]
	mov	DWORD PTR _h3$[esp+44], ebx
	mov	ebx, eax
	sar	ebx, 26					; 0000001aH
	add	esi, ebx

; 229  :     h4 &= kBottom26Bits;

	and	eax, ebp
	mov	DWORD PTR _h4$[esp+44], eax

; 230  :     h6 += h5 >> 25;

	mov	ebx, esi
	mov	eax, DWORD PTR _h6$[esp+40]

; 231  :     h5 &= kBottom25Bits;

	and	esi, edi
	sar	ebx, 25					; 00000019H
	add	eax, ebx
	mov	DWORD PTR _h5$[esp+44], esi

; 232  :     h7 += h6 >> 26;

	mov	esi, DWORD PTR _h7$[esp+44]
	mov	ebx, eax
	sar	ebx, 26					; 0000001aH
	add	esi, ebx

; 233  :     h6 &= kBottom26Bits;

	and	eax, ebp

; 234  :     h8 += h7 >> 25;

	mov	ebx, esi
	mov	DWORD PTR _h6$[esp+40], eax
	mov	eax, DWORD PTR _h8$[esp+44]

; 235  :     h7 &= kBottom25Bits;

	and	esi, edi
	sar	ebx, 25					; 00000019H
	add	eax, ebx
	mov	DWORD PTR _h7$[esp+44], esi

; 236  :     h9 += h8 >> 26;

	mov	esi, eax

; 237  :     h8 &= kBottom26Bits;

	and	eax, ebp
	mov	DWORD PTR _h8$[esp+44], eax

; 238  :     h9 &= kBottom25Bits;
; 239  :     /* h10 = carry9 */
; 240  : 
; 241  :     /*
; 242  :      * Goal: Output h0+...+2^255 h10-2^255 q, which is between 0 and 2^255-20.
; 243  :      * Have h0+...+2^230 h9 between 0 and 2^255-1;
; 244  :      * evidently 2^255 h10-2^255 q = 0.
; 245  :      * Goal: Output h0+...+2^230 h9.
; 246  :      */
; 247  :     s[0] = (uint8_t)(h0 >> 0);

	mov	eax, DWORD PTR _s$[esp+40]

; 248  :     s[1] = (uint8_t)(h0 >> 8);

	mov	ebx, edx
	sar	ebx, 8
	mov	BYTE PTR [eax+1], bl

; 249  :     s[2] = (uint8_t)(h0 >> 16);

	mov	ebx, edx
	sar	ebx, 16					; 00000010H
	mov	BYTE PTR [eax+2], bl

; 250  :     s[3] = (uint8_t)((h0 >> 24) | ((uint32_t)(h1) << 2));

	mov	bl, BYTE PTR _h1$[esp+44]
	mov	BYTE PTR [eax], dl
	sar	edx, 24					; 00000018H
	shl	bl, 2
	or	dl, bl
	mov	BYTE PTR [eax+3], dl

; 251  :     s[4] = (uint8_t)(h1 >> 6);

	mov	edx, DWORD PTR _h1$[esp+44]
	mov	ebx, edx
	sar	ebx, 6
	mov	BYTE PTR [eax+4], bl

; 252  :     s[5] = (uint8_t)(h1 >> 14);

	mov	ebx, edx
	sar	ebx, 14					; 0000000eH
	mov	BYTE PTR [eax+5], bl

; 253  :     s[6] = (uint8_t)((h1 >> 22) | ((uint32_t)(h2) << 3));

	mov	bl, BYTE PTR _h2$[esp+44]
	sar	edx, 22					; 00000016H
	shl	bl, 3
	or	dl, bl
	mov	BYTE PTR [eax+6], dl

; 254  :     s[7] = (uint8_t)(h2 >> 5);

	mov	edx, DWORD PTR _h2$[esp+44]
	mov	ebx, edx
	sar	ebx, 5
	mov	BYTE PTR [eax+7], bl

; 255  :     s[8] = (uint8_t)(h2 >> 13);

	mov	ebx, edx
	sar	ebx, 13					; 0000000dH
	mov	BYTE PTR [eax+8], bl

; 256  :     s[9] = (uint8_t)((h2 >> 21) | ((uint32_t)(h3) << 5));

	mov	bl, BYTE PTR _h3$[esp+44]
	sar	edx, 21					; 00000015H
	shl	bl, 5
	or	dl, bl
	mov	BYTE PTR [eax+9], dl

; 257  :     s[10] = (uint8_t)(h3 >> 3);

	mov	edx, DWORD PTR _h3$[esp+44]
	mov	ebx, edx
	sar	ebx, 3
	mov	BYTE PTR [eax+10], bl

; 258  :     s[11] = (uint8_t)(h3 >> 11);

	mov	ebx, edx
	sar	ebx, 11					; 0000000bH
	mov	BYTE PTR [eax+11], bl

; 259  :     s[12] = (uint8_t)((h3 >> 19) | ((uint32_t)(h4) << 6));

	mov	bl, BYTE PTR _h4$[esp+44]
	sar	edx, 19					; 00000013H
	shl	bl, 6
	or	dl, bl
	mov	BYTE PTR [eax+12], dl

; 260  :     s[13] = (uint8_t)(h4 >> 2);

	mov	edx, DWORD PTR _h4$[esp+44]
	mov	ebx, edx
	sar	ebx, 2
	sar	esi, 26					; 0000001aH
	mov	BYTE PTR [eax+13], bl
	add	ecx, esi

; 261  :     s[14] = (uint8_t)(h4 >> 10);

	mov	ebx, edx
	and	ecx, edi
	sar	ebx, 10					; 0000000aH

; 262  :     s[15] = (uint8_t)(h4 >> 18);

	sar	edx, 18					; 00000012H
	mov	BYTE PTR [eax+14], bl
	mov	BYTE PTR [eax+15], dl

; 263  :     s[16] = (uint8_t)(h5 >> 0);

	mov	edx, DWORD PTR _h5$[esp+44]
	pop	edi

; 264  :     s[17] = (uint8_t)(h5 >> 8);

	mov	ebx, edx
	mov	BYTE PTR [eax+16], dl
	sar	ebx, 8
	mov	BYTE PTR [eax+17], bl

; 265  :     s[18] = (uint8_t)(h5 >> 16);

	mov	ebx, edx
	sar	ebx, 16					; 00000010H
	mov	BYTE PTR [eax+18], bl

; 266  :     s[19] = (uint8_t)((h5 >> 24) | ((uint32_t)(h6) << 1));

	mov	bl, BYTE PTR _h6$[esp+36]
	sar	edx, 24					; 00000018H
	shl	bl, 1
	or	dl, bl
	pop	esi
	mov	BYTE PTR [eax+19], dl

; 267  :     s[20] = (uint8_t)(h6 >> 7);

	mov	edx, DWORD PTR _h6$[esp+32]
	mov	ebx, edx
	pop	ebp
	sar	ebx, 7
	mov	BYTE PTR [eax+20], bl

; 268  :     s[21] = (uint8_t)(h6 >> 15);

	mov	ebx, edx
	sar	ebx, 15					; 0000000fH
	mov	BYTE PTR [eax+21], bl

; 269  :     s[22] = (uint8_t)((h6 >> 23) | ((uint32_t)(h7) << 3));

	mov	bl, BYTE PTR _h7$[esp+32]
	sar	edx, 23					; 00000017H
	shl	bl, 3
	or	dl, bl
	mov	BYTE PTR [eax+22], dl

; 270  :     s[23] = (uint8_t)(h7 >> 5);

	mov	edx, DWORD PTR _h7$[esp+32]
	mov	ebx, edx
	sar	ebx, 5
	mov	BYTE PTR [eax+23], bl

; 271  :     s[24] = (uint8_t)(h7 >> 13);

	mov	ebx, edx
	sar	ebx, 13					; 0000000dH
	mov	BYTE PTR [eax+24], bl

; 272  :     s[25] = (uint8_t)((h7 >> 21) | ((uint32_t)(h8) << 4));

	mov	bl, BYTE PTR _h8$[esp+32]
	sar	edx, 21					; 00000015H
	shl	bl, 4
	or	dl, bl
	mov	BYTE PTR [eax+25], dl

; 273  :     s[26] = (uint8_t)(h8 >> 4);

	mov	edx, DWORD PTR _h8$[esp+32]
	mov	ebx, edx
	sar	ebx, 4
	mov	BYTE PTR [eax+26], bl

; 274  :     s[27] = (uint8_t)(h8 >> 12);

	mov	ebx, edx
	sar	ebx, 12					; 0000000cH
	mov	BYTE PTR [eax+27], bl

; 275  :     s[28] = (uint8_t)((h8 >> 20) | ((uint32_t)(h9) << 6));

	mov	bl, cl
	sar	edx, 20					; 00000014H
	shl	bl, 6
	or	dl, bl
	pop	ebx
	mov	BYTE PTR [eax+28], dl

; 276  :     s[29] = (uint8_t)(h9 >> 2);

	mov	edx, ecx
	sar	edx, 2
	mov	BYTE PTR [eax+29], dl

; 277  :     s[30] = (uint8_t)(h9 >> 10);

	mov	edx, ecx
	sar	edx, 10					; 0000000aH

; 278  :     s[31] = (uint8_t)(h9 >> 18);

	sar	ecx, 18					; 00000012H
	mov	BYTE PTR [eax+30], dl
	mov	BYTE PTR [eax+31], cl

; 279  : }

	add	esp, 28					; 0000001cH
	ret	0
_fe_tobytes ENDP
_TEXT	ENDS
;	COMDAT _ge_tobytes
_TEXT	SEGMENT
_s$ = 8
_h$ = 12
_recip$ = -120
_x$ = -40
_y$ = -80
_ge_tobytes PROC NEAR					; COMDAT

; 1061 : {

	sub	esp, 120				; 00000078H

; 1062 :     fe recip;
; 1063 :     fe x;
; 1064 :     fe y;
; 1065 : 
; 1066 :     fe_invert(recip, h->Z);

	lea	ecx, DWORD PTR _recip$[esp+120]
	push	esi
	mov	esi, DWORD PTR _h$[esp+120]
	lea	eax, DWORD PTR [esi+80]
	push	eax
	push	ecx
	call	_fe_invert

; 1067 :     fe_mul(x, h->X, recip);

	lea	edx, DWORD PTR _recip$[esp+132]
	lea	eax, DWORD PTR _x$[esp+132]
	push	edx
	push	esi
	push	eax
	call	_fe_mul

; 1068 :     fe_mul(y, h->Y, recip);

	lea	ecx, DWORD PTR _recip$[esp+144]
	add	esi, 40					; 00000028H
	push	ecx
	lea	edx, DWORD PTR _y$[esp+148]
	push	esi
	push	edx
	call	_fe_mul

; 1069 :     fe_tobytes(s, y);

	mov	esi, DWORD PTR _s$[esp+152]
	lea	eax, DWORD PTR _y$[esp+156]
	push	eax
	push	esi
	call	_fe_tobytes

; 1070 :     s[31] ^= fe_isnegative(x) << 7;

	lea	ecx, DWORD PTR _x$[esp+164]
	push	ecx
	call	_fe_isnegative
	mov	cl, BYTE PTR [esi+31]
	add	esp, 44					; 0000002cH
	shl	al, 7
	xor	cl, al
	mov	BYTE PTR [esi+31], cl
	pop	esi

; 1071 : }

	add	esp, 120				; 00000078H
	ret	0
_ge_tobytes ENDP
_TEXT	ENDS
EXTRN	__allmul:NEAR
EXTRN	__allshr:NEAR
;	COMDAT _fe_mul
_TEXT	SEGMENT
_h$ = 8
_f$ = 12
_g$ = 16
_f0$ = -256
_f1$ = -88
_f2$ = -272
_f3$ = -104
_f4$ = -280
_f5$ = -120
_f6$ = -264
_f7$ = -136
_f8$ = -248
_f9$ = -112
_g0$ = -288
_g1$ = -224
_g2$ = -320
_g3$ = -128
_g4$ = -144
_g5$ = -336
_g6$ = -328
_g7$ = -312
_g8$ = -304
_g9$ = -240
_g2_19$ = -216
_g3_19$ = -168
_g4_19$ = -208
_g5_19$ = -96
_g7_19$ = -192
_g9_19$ = -232
_f1_2$ = -144
_f3_2$ = -80
_f5_2$ = -64
_f7_2$ = -152
_h1$ = -8
_h2$ = -16
_h3$ = -336
_h5$ = -32
_h6$ = -72
_h7$ = -328
_h8$ = -176
_h9$ = -200
_carry0$ = -48
_carry1$ = -24
_carry2$ = -296
_carry3$ = -296
_carry4$ = -88
_carry5$ = -160
_carry6$ = -296
_carry7$ = -296
_carry8$ = -160
_carry9$ = -160
_fe_mul	PROC NEAR					; COMDAT

; 316  : {

	sub	esp, 336				; 00000150H

; 317  :     int32_t f0 = f[0];

	push	ebx
	push	ebp
	push	esi

; 339  :     int32_t g3_19 = 19 * g3;

	push	edi

; 317  :     int32_t f0 = f[0];

	mov	eax, DWORD PTR _f$[esp+348]
	mov	ecx, DWORD PTR [eax]

; 318  :     int32_t f1 = f[1];

	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR _f0$[esp+352], ecx

; 319  :     int32_t f2 = f[2];

	mov	ecx, DWORD PTR [eax+8]
	mov	DWORD PTR _f2$[esp+352], ecx

; 320  :     int32_t f3 = f[3];
; 321  :     int32_t f4 = f[4];

	mov	ecx, DWORD PTR [eax+16]
	mov	DWORD PTR _f1$[esp+352], edx
	mov	edx, DWORD PTR [eax+12]
	mov	DWORD PTR _f4$[esp+352], ecx

; 322  :     int32_t f5 = f[5];
; 323  :     int32_t f6 = f[6];

	mov	ecx, DWORD PTR [eax+24]
	mov	DWORD PTR _f3$[esp+352], edx
	mov	edx, DWORD PTR [eax+20]
	mov	DWORD PTR _f6$[esp+352], ecx

; 324  :     int32_t f7 = f[7];
; 325  :     int32_t f8 = f[8];

	mov	ecx, DWORD PTR [eax+32]
	mov	DWORD PTR _f5$[esp+352], edx
	mov	edx, DWORD PTR [eax+28]
	mov	DWORD PTR _f8$[esp+352], ecx

; 326  :     int32_t f9 = f[9];
; 327  :     int32_t g0 = g[0];

	mov	ecx, DWORD PTR _g$[esp+348]
	mov	DWORD PTR _f7$[esp+352], edx
	mov	edx, DWORD PTR [eax+36]
	mov	eax, DWORD PTR [ecx]
	mov	DWORD PTR _f9$[esp+352], edx

; 328  :     int32_t g1 = g[1];

	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR _g0$[esp+352], eax

; 329  :     int32_t g2 = g[2];

	mov	eax, DWORD PTR [ecx+8]
	mov	DWORD PTR _g1$[esp+352], edx

; 330  :     int32_t g3 = g[3];

	mov	edx, DWORD PTR [ecx+12]
	mov	DWORD PTR _g2$[esp+352], eax

; 331  :     int32_t g4 = g[4];
; 332  :     int32_t g5 = g[5];
; 333  :     int32_t g6 = g[6];
; 334  :     int32_t g7 = g[7];
; 335  :     int32_t g8 = g[8];
; 336  :     int32_t g9 = g[9];
; 337  :     int32_t g1_19 = 19 * g1; /* 1.959375*2^29 */
; 338  :     int32_t g2_19 = 19 * g2; /* 1.959375*2^30; still ok */

	lea	ebp, DWORD PTR [eax+eax*8]
	mov	DWORD PTR _g3$[esp+352], edx
	mov	edx, DWORD PTR [ecx+16]
	mov	esi, DWORD PTR [ecx+24]
	lea	eax, DWORD PTR [eax+ebp*2]
	mov	DWORD PTR _g4$[esp+352], edx
	mov	DWORD PTR _g2_19$[esp+352], eax

; 339  :     int32_t g3_19 = 19 * g3;

	mov	eax, DWORD PTR _g3$[esp+352]
	mov	edx, DWORD PTR [ecx+20]
	mov	ebx, DWORD PTR [ecx+32]
	lea	ebp, DWORD PTR [eax+eax*8]
	mov	edi, DWORD PTR [ecx+28]
	mov	ecx, DWORD PTR [ecx+36]
	lea	eax, DWORD PTR [eax+ebp*2]

; 340  :     int32_t g4_19 = 19 * g4;

	mov	ebp, DWORD PTR _g4$[esp+352]
	mov	DWORD PTR _g3_19$[esp+352], eax
	mov	DWORD PTR _g5$[esp+352], edx
	lea	eax, DWORD PTR [ebp+ebp*8]
	mov	DWORD PTR _g6$[esp+352], esi
	mov	DWORD PTR _g8$[esp+352], ebx
	mov	DWORD PTR _g9$[esp+352], ecx
	lea	eax, DWORD PTR [ebp+eax*2]
	mov	DWORD PTR _g7$[esp+352], edi
	mov	DWORD PTR _g4_19$[esp+352], eax

; 341  :     int32_t g5_19 = 19 * g5;

	lea	eax, DWORD PTR [edx+edx*8]
	lea	edx, DWORD PTR [edx+eax*2]

; 342  :     int32_t g6_19 = 19 * g6;

	lea	eax, DWORD PTR [esi+esi*8]
	mov	DWORD PTR _g5_19$[esp+352], edx

; 343  :     int32_t g7_19 = 19 * g7;

	lea	edx, DWORD PTR [edi+edi*8]
	lea	esi, DWORD PTR [esi+eax*2]
	lea	eax, DWORD PTR [edi+edx*2]

; 344  :     int32_t g8_19 = 19 * g8;

	lea	edx, DWORD PTR [ebx+ebx*8]
	mov	DWORD PTR _g7_19$[esp+352], eax

; 345  :     int32_t g9_19 = 19 * g9;

	lea	eax, DWORD PTR [ecx+ecx*8]
	lea	ebx, DWORD PTR [ebx+edx*2]

; 346  :     int32_t f1_2 = 2 * f1;

	mov	edx, DWORD PTR _f1$[esp+352]
	lea	ecx, DWORD PTR [ecx+eax*2]
	mov	DWORD PTR _g9_19$[esp+352], ecx

; 347  :     int32_t f3_2 = 2 * f3;

	mov	ecx, DWORD PTR _f3$[esp+352]
	lea	eax, DWORD PTR [edx+edx]
	mov	DWORD PTR _f1_2$[esp+352], eax

; 348  :     int32_t f5_2 = 2 * f5;

	mov	eax, DWORD PTR _f5$[esp+352]
	lea	edx, DWORD PTR [ecx+ecx]
	mov	DWORD PTR _f3_2$[esp+352], edx

; 349  :     int32_t f7_2 = 2 * f7;

	mov	edx, DWORD PTR _f7$[esp+352]
	lea	ecx, DWORD PTR [eax+eax]
	mov	DWORD PTR _f5_2$[esp+352], ecx
	lea	eax, DWORD PTR [edx+edx]

; 350  :     int32_t f9_2 = 2 * f9;

	mov	ecx, DWORD PTR _f9$[esp+352]
	mov	DWORD PTR _f7_2$[esp+352], eax
	lea	eax, DWORD PTR [ecx+ecx]

; 351  :     int64_t f0g0 = f0 * (int64_t)g0;
; 352  :     int64_t f0g1 = f0 * (int64_t)g1;
; 353  :     int64_t f0g2 = f0 * (int64_t)g2;
; 354  :     int64_t f0g3 = f0 * (int64_t)g3;
; 355  :     int64_t f0g4 = f0 * (int64_t)g4;
; 356  :     int64_t f0g5 = f0 * (int64_t)g5;
; 357  :     int64_t f0g6 = f0 * (int64_t)g6;
; 358  :     int64_t f0g7 = f0 * (int64_t)g7;
; 359  :     int64_t f0g8 = f0 * (int64_t)g8;
; 360  :     int64_t f0g9 = f0 * (int64_t)g9;
; 361  :     int64_t f1g0 = f1 * (int64_t)g0;
; 362  :     int64_t f1g1_2 = f1_2 * (int64_t)g1;
; 363  :     int64_t f1g2 = f1 * (int64_t)g2;
; 364  :     int64_t f1g3_2 = f1_2 * (int64_t)g3;
; 365  :     int64_t f1g4 = f1 * (int64_t)g4;
; 366  :     int64_t f1g5_2 = f1_2 * (int64_t)g5;
; 367  :     int64_t f1g6 = f1 * (int64_t)g6;
; 368  :     int64_t f1g7_2 = f1_2 * (int64_t)g7;
; 369  :     int64_t f1g8 = f1 * (int64_t)g8;
; 370  :     int64_t f1g9_38 = f1_2 * (int64_t)g9_19;
; 371  :     int64_t f2g0 = f2 * (int64_t)g0;
; 372  :     int64_t f2g1 = f2 * (int64_t)g1;
; 373  :     int64_t f2g2 = f2 * (int64_t)g2;
; 374  :     int64_t f2g3 = f2 * (int64_t)g3;
; 375  :     int64_t f2g4 = f2 * (int64_t)g4;
; 376  :     int64_t f2g5 = f2 * (int64_t)g5;
; 377  :     int64_t f2g6 = f2 * (int64_t)g6;
; 378  :     int64_t f2g7 = f2 * (int64_t)g7;
; 379  :     int64_t f2g8_19 = f2 * (int64_t)g8_19;
; 380  :     int64_t f2g9_19 = f2 * (int64_t)g9_19;
; 381  :     int64_t f3g0 = f3 * (int64_t)g0;
; 382  :     int64_t f3g1_2 = f3_2 * (int64_t)g1;
; 383  :     int64_t f3g2 = f3 * (int64_t)g2;
; 384  :     int64_t f3g3_2 = f3_2 * (int64_t)g3;
; 385  :     int64_t f3g4 = f3 * (int64_t)g4;
; 386  :     int64_t f3g5_2 = f3_2 * (int64_t)g5;
; 387  :     int64_t f3g6 = f3 * (int64_t)g6;
; 388  :     int64_t f3g7_38 = f3_2 * (int64_t)g7_19;
; 389  :     int64_t f3g8_19 = f3 * (int64_t)g8_19;
; 390  :     int64_t f3g9_38 = f3_2 * (int64_t)g9_19;
; 391  :     int64_t f4g0 = f4 * (int64_t)g0;
; 392  :     int64_t f4g1 = f4 * (int64_t)g1;
; 393  :     int64_t f4g2 = f4 * (int64_t)g2;
; 394  :     int64_t f4g3 = f4 * (int64_t)g3;
; 395  :     int64_t f4g4 = f4 * (int64_t)g4;
; 396  :     int64_t f4g5 = f4 * (int64_t)g5;
; 397  :     int64_t f4g6_19 = f4 * (int64_t)g6_19;
; 398  :     int64_t f4g7_19 = f4 * (int64_t)g7_19;
; 399  :     int64_t f4g8_19 = f4 * (int64_t)g8_19;
; 400  :     int64_t f4g9_19 = f4 * (int64_t)g9_19;
; 401  :     int64_t f5g0 = f5 * (int64_t)g0;
; 402  :     int64_t f5g1_2 = f5_2 * (int64_t)g1;
; 403  :     int64_t f5g2 = f5 * (int64_t)g2;
; 404  :     int64_t f5g3_2 = f5_2 * (int64_t)g3;
; 405  :     int64_t f5g4 = f5 * (int64_t)g4;
; 406  :     int64_t f5g5_38 = f5_2 * (int64_t)g5_19;
; 407  :     int64_t f5g6_19 = f5 * (int64_t)g6_19;
; 408  :     int64_t f5g7_38 = f5_2 * (int64_t)g7_19;
; 409  :     int64_t f5g8_19 = f5 * (int64_t)g8_19;
; 410  :     int64_t f5g9_38 = f5_2 * (int64_t)g9_19;
; 411  :     int64_t f6g0 = f6 * (int64_t)g0;
; 412  :     int64_t f6g1 = f6 * (int64_t)g1;
; 413  :     int64_t f6g2 = f6 * (int64_t)g2;
; 414  :     int64_t f6g3 = f6 * (int64_t)g3;
; 415  :     int64_t f6g4_19 = f6 * (int64_t)g4_19;
; 416  :     int64_t f6g5_19 = f6 * (int64_t)g5_19;
; 417  :     int64_t f6g6_19 = f6 * (int64_t)g6_19;
; 418  :     int64_t f6g7_19 = f6 * (int64_t)g7_19;
; 419  :     int64_t f6g8_19 = f6 * (int64_t)g8_19;
; 420  :     int64_t f6g9_19 = f6 * (int64_t)g9_19;
; 421  :     int64_t f7g0 = f7 * (int64_t)g0;
; 422  :     int64_t f7g1_2 = f7_2 * (int64_t)g1;
; 423  :     int64_t f7g2 = f7 * (int64_t)g2;
; 424  :     int64_t f7g3_38 = f7_2 * (int64_t)g3_19;
; 425  :     int64_t f7g4_19 = f7 * (int64_t)g4_19;
; 426  :     int64_t f7g5_38 = f7_2 * (int64_t)g5_19;
; 427  :     int64_t f7g6_19 = f7 * (int64_t)g6_19;
; 428  :     int64_t f7g7_38 = f7_2 * (int64_t)g7_19;
; 429  :     int64_t f7g8_19 = f7 * (int64_t)g8_19;
; 430  :     int64_t f7g9_38 = f7_2 * (int64_t)g9_19;
; 431  :     int64_t f8g0 = f8 * (int64_t)g0;
; 432  :     int64_t f8g1 = f8 * (int64_t)g1;
; 433  :     int64_t f8g2_19 = f8 * (int64_t)g2_19;
; 434  :     int64_t f8g3_19 = f8 * (int64_t)g3_19;
; 435  :     int64_t f8g4_19 = f8 * (int64_t)g4_19;
; 436  :     int64_t f8g5_19 = f8 * (int64_t)g5_19;
; 437  :     int64_t f8g6_19 = f8 * (int64_t)g6_19;
; 438  :     int64_t f8g7_19 = f8 * (int64_t)g7_19;
; 439  :     int64_t f8g8_19 = f8 * (int64_t)g8_19;
; 440  :     int64_t f8g9_19 = f8 * (int64_t)g9_19;
; 441  :     int64_t f9g0 = f9 * (int64_t)g0;
; 442  :     int64_t f9g1_38 = f9_2 * (int64_t)g1_19;
; 443  :     int64_t f9g2_19 = f9 * (int64_t)g2_19;
; 444  :     int64_t f9g3_38 = f9_2 * (int64_t)g3_19;
; 445  :     int64_t f9g4_19 = f9 * (int64_t)g4_19;
; 446  :     int64_t f9g5_38 = f9_2 * (int64_t)g5_19;
; 447  :     int64_t f9g6_19 = f9 * (int64_t)g6_19;
; 448  :     int64_t f9g7_38 = f9_2 * (int64_t)g7_19;
; 449  :     int64_t f9g8_19 = f9 * (int64_t)g8_19;
; 450  :     int64_t f9g9_38 = f9_2 * (int64_t)g9_19;
; 451  :     int64_t h0 = f0g0 + f1g9_38 + f2g8_19 + f3g7_38 + f4g6_19 + f5g5_38 + f6g4_19 + f7g3_38 + f8g2_19 + f9g1_38;
; 452  :     int64_t h1 = f0g1 + f1g0 + f2g9_19 + f3g8_19 + f4g7_19 + f5g6_19 + f6g5_19 + f7g4_19 + f8g3_19 + f9g2_19;
; 453  :     int64_t h2 = f0g2 + f1g1_2 + f2g0 + f3g9_38 + f4g8_19 + f5g7_38 + f6g6_19 + f7g5_38 + f8g4_19 + f9g3_38;
; 454  :     int64_t h3 = f0g3 + f1g2 + f2g1 + f3g0 + f4g9_19 + f5g8_19 + f6g7_19 + f7g6_19 + f8g5_19 + f9g4_19;
; 455  :     int64_t h4 = f0g4 + f1g3_2 + f2g2 + f3g1_2 + f4g0 + f5g9_38 + f6g8_19 + f7g7_38 + f8g6_19 + f9g5_38;
; 456  :     int64_t h5 = f0g5 + f1g4 + f2g3 + f3g2 + f4g1 + f5g0 + f6g9_19 + f7g8_19 + f8g7_19 + f9g6_19;
; 457  :     int64_t h6 = f0g6 + f1g5_2 + f2g4 + f3g3_2 + f4g2 + f5g1_2 + f6g0 + f7g9_38 + f8g8_19 + f9g7_38;
; 458  :     int64_t h7 = f0g7 + f1g6 + f2g5 + f3g4 + f4g3 + f5g2 + f6g1 + f7g0 + f8g9_19 + f9g8_19;
; 459  :     int64_t h8 = f0g8 + f1g7_2 + f2g6 + f3g5_2 + f4g4 + f5g3_2 + f6g2 + f7g1_2 + f8g0 + f9g9_38;
; 460  :     int64_t h9 = f0g9 + f1g8 + f2g7 + f3g6 + f4g5 + f5g4 + f6g3 + f7g2 + f8g1 + f9g0;
; 461  :     int64_t carry0;
; 462  :     int64_t carry1;
; 463  :     int64_t carry2;
; 464  :     int64_t carry3;
; 465  :     int64_t carry4;
; 466  :     int64_t carry5;
; 467  :     int64_t carry6;
; 468  :     int64_t carry7;
; 469  :     int64_t carry8;
; 470  :     int64_t carry9;
; 471  : 
; 472  :     /* |h0| <= (1.65*1.65*2^52*(1+19+19+19+19)+1.65*1.65*2^50*(38+38+38+38+38))
; 473  :      *   i.e. |h0| <= 1.4*2^60; narrower ranges for h2, h4, h6, h8
; 474  :      * |h1| <= (1.65*1.65*2^51*(1+1+19+19+19+19+19+19+19+19))
; 475  :      *   i.e. |h1| <= 1.7*2^59; narrower ranges for h3, h5, h7, h9 */
; 476  : 
; 477  :     carry0 = h0 + (1 << 25);
; 478  :     h1 += carry0 >> 26;
; 479  :     h0 -= carry0 & kTop38Bits;
; 480  :     carry4 = h4 + (1 << 25);
; 481  :     h5 += carry4 >> 26;
; 482  :     h4 -= carry4 & kTop38Bits;
; 483  :     /* |h0| <= 2^25 */
; 484  :     /* |h4| <= 2^25 */
; 485  :     /* |h1| <= 1.71*2^59 */
; 486  :     /* |h5| <= 1.71*2^59 */
; 487  : 
; 488  :     carry1 = h1 + (1 << 24);
; 489  :     h2 += carry1 >> 25;
; 490  :     h1 -= carry1 & kTop39Bits;
; 491  :     carry5 = h5 + (1 << 24);
; 492  :     h6 += carry5 >> 25;
; 493  :     h5 -= carry5 & kTop39Bits;
; 494  :     /* |h1| <= 2^24; from now on fits into int32 */
; 495  :     /* |h5| <= 2^24; from now on fits into int32 */
; 496  :     /* |h2| <= 1.41*2^60 */
; 497  :     /* |h6| <= 1.41*2^60 */
; 498  : 
; 499  :     carry2 = h2 + (1 << 25);
; 500  :     h3 += carry2 >> 26;
; 501  :     h2 -= carry2 & kTop38Bits;
; 502  :     carry6 = h6 + (1 << 25);
; 503  :     h7 += carry6 >> 26;
; 504  :     h6 -= carry6 & kTop38Bits;
; 505  :     /* |h2| <= 2^25; from now on fits into int32 unchanged */
; 506  :     /* |h6| <= 2^25; from now on fits into int32 unchanged */
; 507  :     /* |h3| <= 1.71*2^59 */
; 508  :     /* |h7| <= 1.71*2^59 */
; 509  : 
; 510  :     carry3 = h3 + (1 << 24);
; 511  :     h4 += carry3 >> 25;
; 512  :     h3 -= carry3 & kTop39Bits;
; 513  :     carry7 = h7 + (1 << 24);
; 514  :     h8 += carry7 >> 25;
; 515  :     h7 -= carry7 & kTop39Bits;
; 516  :     /* |h3| <= 2^24; from now on fits into int32 unchanged */
; 517  :     /* |h7| <= 2^24; from now on fits into int32 unchanged */
; 518  :     /* |h4| <= 1.72*2^34 */
; 519  :     /* |h8| <= 1.41*2^60 */
; 520  : 
; 521  :     carry4 = h4 + (1 << 25);
; 522  :     h5 += carry4 >> 26;
; 523  :     h4 -= carry4 & kTop38Bits;
; 524  :     carry8 = h8 + (1 << 25);
; 525  :     h9 += carry8 >> 26;
; 526  :     h8 -= carry8 & kTop38Bits;
; 527  :     /* |h4| <= 2^25; from now on fits into int32 unchanged */
; 528  :     /* |h8| <= 2^25; from now on fits into int32 unchanged */
; 529  :     /* |h5| <= 1.01*2^24 */
; 530  :     /* |h9| <= 1.71*2^59 */
; 531  : 
; 532  :     carry9 = h9 + (1 << 24);
; 533  :     h0 += (carry9 >> 25) * 19;
; 534  :     h9 -= carry9 & kTop39Bits;
; 535  :     /* |h9| <= 2^24; from now on fits into int32 unchanged */
; 536  :     /* |h0| <= 1.1*2^39 */
; 537  : 
; 538  :     carry0 = h0 + (1 << 25);
; 539  :     h1 += carry0 >> 26;
; 540  :     h0 -= carry0 & kTop38Bits;
; 541  :     /* |h0| <= 2^25; from now on fits into int32 unchanged */
; 542  :     /* |h1| <= 1.01*2^24 */
; 543  : 
; 544  :     h[0] = (int32_t)h0;
; 545  :     h[1] = (int32_t)h1;
; 546  :     h[2] = (int32_t)h2;
; 547  :     h[3] = (int32_t)h3;
; 548  :     h[4] = (int32_t)h4;
; 549  :     h[5] = (int32_t)h5;
; 550  :     h[6] = (int32_t)h6;
; 551  :     h[7] = (int32_t)h7;
; 552  :     h[8] = (int32_t)h8;
; 553  :     h[9] = (int32_t)h9;

	cdq
	mov	ecx, eax
	mov	eax, DWORD PTR _f1_2$[esp+352]
	mov	edi, edx
	mov	DWORD PTR -176+[esp+352], ecx
	cdq
	mov	DWORD PTR -144+[esp+352], eax
	mov	eax, DWORD PTR _g9_19$[esp+352]
	mov	DWORD PTR -144+[esp+356], edx
	mov	DWORD PTR -176+[esp+356], edi
	cdq
	mov	DWORD PTR -232+[esp+352], eax
	mov	eax, DWORD PTR _f3_2$[esp+352]
	mov	DWORD PTR -232+[esp+356], edx
	cdq
	mov	DWORD PTR -80+[esp+352], eax
	mov	eax, DWORD PTR _g7_19$[esp+352]
	mov	DWORD PTR -80+[esp+356], edx
	cdq
	mov	DWORD PTR -192+[esp+352], eax
	mov	eax, DWORD PTR _f5_2$[esp+352]
	mov	DWORD PTR -192+[esp+356], edx
	cdq
	mov	DWORD PTR -56+[esp+352], eax
	mov	eax, DWORD PTR _g5_19$[esp+352]
	mov	DWORD PTR -56+[esp+356], edx
	cdq
	mov	DWORD PTR -96+[esp+352], eax
	mov	eax, DWORD PTR _f7_2$[esp+352]
	mov	DWORD PTR -96+[esp+356], edx
	cdq
	mov	DWORD PTR -64+[esp+352], eax
	mov	eax, DWORD PTR _g3_19$[esp+352]
	mov	DWORD PTR -64+[esp+356], edx
	cdq
	mov	DWORD PTR -200+[esp+352], eax
	mov	eax, DWORD PTR _g2_19$[esp+352]
	mov	DWORD PTR -200+[esp+356], edx
	cdq
	mov	DWORD PTR -72+[esp+352], eax
	mov	eax, DWORD PTR _f8$[esp+352]
	mov	DWORD PTR -72+[esp+356], edx
	cdq
	mov	DWORD PTR -248+[esp+352], eax
	mov	eax, DWORD PTR _g4_19$[esp+352]
	mov	DWORD PTR -248+[esp+356], edx
	cdq
	mov	DWORD PTR -184+[esp+352], eax
	mov	eax, DWORD PTR _f6$[esp+352]
	mov	DWORD PTR -184+[esp+356], edx
	cdq
	mov	DWORD PTR -264+[esp+352], eax
	mov	eax, esi
	mov	DWORD PTR -264+[esp+356], edx
	cdq
	mov	DWORD PTR -152+[esp+352], eax
	mov	eax, DWORD PTR _f4$[esp+352]
	mov	DWORD PTR -152+[esp+356], edx
	cdq
	mov	DWORD PTR -280+[esp+352], eax
	mov	eax, ebx
	mov	DWORD PTR -280+[esp+356], edx
	mov	ebx, DWORD PTR _g1$[esp+352]
	cdq
	mov	DWORD PTR -208+[esp+352], eax
	mov	eax, DWORD PTR _f2$[esp+352]
	mov	DWORD PTR -208+[esp+356], edx
	cdq
	mov	DWORD PTR -272+[esp+352], eax
	mov	eax, DWORD PTR _g0$[esp+352]
	mov	DWORD PTR -272+[esp+356], edx
	cdq
	mov	DWORD PTR -288+[esp+352], eax
	mov	eax, DWORD PTR _f0$[esp+352]
	mov	DWORD PTR -288+[esp+356], edx
	cdq
	mov	DWORD PTR -256+[esp+352], eax
	mov	DWORD PTR -256+[esp+356], edx
	lea	edx, DWORD PTR [ebx+ebx*8]
	push	edi
	push	ecx
	lea	eax, DWORD PTR [ebx+edx*2]
	cdq
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -288+[esp+352]
	mov	esi, eax
	mov	eax, DWORD PTR -288+[esp+356]
	mov	edi, edx
	mov	edx, DWORD PTR -256+[esp+356]
	push	eax
	mov	eax, DWORD PTR -256+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -208+[esp+356]
	add	esi, eax
	mov	eax, DWORD PTR -272+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -272+[esp+356]
	adc	edi, edx
	mov	edx, DWORD PTR -208+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -280+[esp+356]
	add	esi, eax
	mov	eax, DWORD PTR -152+[esp+352]
	adc	edi, edx
	mov	edx, DWORD PTR -152+[esp+356]
	push	edx
	mov	edx, DWORD PTR -280+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -184+[esp+352]
	add	esi, eax
	mov	eax, DWORD PTR -184+[esp+356]
	adc	edi, edx
	mov	edx, DWORD PTR -264+[esp+356]
	push	eax
	mov	eax, DWORD PTR -264+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -72+[esp+356]
	add	esi, eax
	mov	eax, DWORD PTR -248+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -248+[esp+356]
	adc	edi, edx
	mov	edx, DWORD PTR -72+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -200+[esp+356]
	add	esi, eax
	mov	eax, DWORD PTR -64+[esp+352]
	adc	edi, edx
	mov	edx, DWORD PTR -64+[esp+356]
	push	edx
	mov	edx, DWORD PTR -200+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -56+[esp+352]
	add	esi, eax
	mov	eax, DWORD PTR -56+[esp+356]
	adc	edi, edx
	mov	edx, DWORD PTR -96+[esp+356]
	push	eax
	push	ecx
	mov	eax, DWORD PTR -96+[esp+360]
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -80+[esp+356]
	add	esi, eax
	mov	eax, DWORD PTR -192+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -192+[esp+356]
	adc	edi, edx
	mov	edx, DWORD PTR -80+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -232+[esp+356]
	add	esi, eax
	mov	eax, DWORD PTR -144+[esp+352]
	adc	edi, edx
	mov	edx, DWORD PTR -144+[esp+356]
	push	edx
	mov	edx, DWORD PTR -232+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	add	esi, eax
	mov	eax, DWORD PTR _g3$[esp+352]
	adc	edi, edx
	mov	ecx, DWORD PTR -256+[esp+356]
	cdq
	mov	DWORD PTR -168+[esp+352], eax
	mov	eax, ebx
	mov	DWORD PTR -168+[esp+356], edx
	push	ecx
	cdq
	mov	DWORD PTR -224+[esp+356], eax
	mov	eax, DWORD PTR _g2$[esp+356]
	mov	ecx, DWORD PTR -256+[esp+356]
	mov	DWORD PTR -224+[esp+360], edx
	cdq
	mov	DWORD PTR -216+[esp+356], eax
	mov	eax, ebp
	mov	DWORD PTR -216+[esp+360], edx
	push	ecx
	cdq
	push	edx
	push	eax
	mov	DWORD PTR -128+[esp+368], eax
	mov	DWORD PTR -128+[esp+372], edx
	call	__allmul
	mov	ecx, DWORD PTR -216+[esp+356]
	mov	ebp, edx
	mov	edx, DWORD PTR -272+[esp+356]
	mov	ebx, eax
	mov	eax, DWORD PTR -272+[esp+352]
	push	edx
	mov	edx, DWORD PTR -216+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -280+[esp+352]
	add	ebx, eax
	mov	eax, DWORD PTR -280+[esp+356]
	adc	ebp, edx
	mov	edx, DWORD PTR -288+[esp+356]
	push	eax
	mov	eax, DWORD PTR -288+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -264+[esp+356]
	add	ebx, eax
	mov	eax, DWORD PTR -208+[esp+356]
	push	ecx
	adc	ebp, edx
	mov	edx, DWORD PTR -264+[esp+356]
	push	edx
	push	eax
	mov	ecx, DWORD PTR -208+[esp+364]
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -152+[esp+356]
	add	ebx, eax
	mov	eax, DWORD PTR -248+[esp+352]
	adc	ebp, edx
	mov	edx, DWORD PTR -248+[esp+356]
	push	edx
	mov	edx, DWORD PTR -152+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -80+[esp+352]
	add	ebx, eax
	mov	eax, DWORD PTR -80+[esp+356]
	adc	ebp, edx
	mov	edx, DWORD PTR -224+[esp+356]
	push	eax
	mov	eax, DWORD PTR -224+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -144+[esp+356]
	add	ebx, eax
	mov	eax, DWORD PTR -168+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -168+[esp+356]
	adc	ebp, edx
	mov	edx, DWORD PTR -144+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -64+[esp+356]
	add	ebx, eax
	mov	eax, DWORD PTR -192+[esp+352]
	adc	ebp, edx
	mov	edx, DWORD PTR -192+[esp+356]
	push	edx
	mov	edx, DWORD PTR -64+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -232+[esp+352]
	add	ebx, eax
	mov	eax, DWORD PTR -232+[esp+356]
	adc	ebp, edx
	mov	edx, DWORD PTR -56+[esp+356]
	push	eax
	mov	eax, DWORD PTR -56+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -176+[esp+356]
	add	ebx, eax
	mov	eax, DWORD PTR -96+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -96+[esp+356]
	adc	ebp, edx
	mov	edx, DWORD PTR -176+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	add	ebx, eax
	mov	eax, esi
	adc	ebp, edx
	add	eax, 33554432				; 02000000H
	mov	DWORD PTR _carry0$[esp+352], eax
	mov	eax, edi
	adc	eax, 0
	mov	DWORD PTR _carry0$[esp+356], eax
	mov	eax, DWORD PTR _f9$[esp+352]
	cdq
	mov	DWORD PTR -112+[esp+352], eax
	mov	DWORD PTR -112+[esp+356], edx
	mov	eax, DWORD PTR _f7$[esp+352]
	mov	ecx, DWORD PTR -288+[esp+356]
	cdq
	mov	DWORD PTR -136+[esp+352], eax
	mov	eax, DWORD PTR _f5$[esp+352]
	mov	DWORD PTR -136+[esp+356], edx
	push	ecx
	cdq
	mov	DWORD PTR -120+[esp+356], eax
	mov	eax, DWORD PTR _f3$[esp+356]
	mov	ecx, DWORD PTR -288+[esp+356]
	mov	DWORD PTR -120+[esp+360], edx
	cdq
	mov	DWORD PTR -104+[esp+356], eax
	mov	eax, DWORD PTR _f1$[esp+356]
	mov	DWORD PTR -104+[esp+360], edx
	push	ecx
	cdq
	push	edx
	push	eax
	mov	DWORD PTR -40+[esp+368], eax
	mov	DWORD PTR -40+[esp+372], edx
	call	__allmul
	mov	ecx, DWORD PTR -104+[esp+356]
	mov	DWORD PTR -320+[esp+356], edx
	mov	edx, DWORD PTR -208+[esp+356]
	mov	DWORD PTR -320+[esp+352], eax
	mov	eax, DWORD PTR -208+[esp+352]
	push	edx
	mov	edx, DWORD PTR -104+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -320+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -320+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -120+[esp+356]
	mov	DWORD PTR -320+[esp+356], eax
	mov	eax, DWORD PTR -152+[esp+356]
	mov	DWORD PTR -320+[esp+352], ecx
	mov	ecx, DWORD PTR -152+[esp+352]
	push	eax
	mov	eax, DWORD PTR -120+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -320+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -320+[esp+356]
	mov	DWORD PTR -320+[esp+352], ecx
	mov	ecx, DWORD PTR -256+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -256+[esp+352]
	mov	DWORD PTR -320+[esp+356], eax
	mov	eax, DWORD PTR -224+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -224+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -320+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -184+[esp+352]
	mov	DWORD PTR -320+[esp+352], ecx
	mov	ecx, DWORD PTR -320+[esp+356]
	adc	ecx, edx
	mov	edx, DWORD PTR -184+[esp+356]
	mov	DWORD PTR -320+[esp+356], ecx
	mov	ecx, DWORD PTR -136+[esp+356]
	push	edx
	mov	edx, DWORD PTR -136+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -320+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -320+[esp+356]
	mov	DWORD PTR -320+[esp+352], ecx
	mov	ecx, DWORD PTR -72+[esp+352]
	adc	eax, edx
	mov	edx, DWORD PTR -112+[esp+356]
	mov	DWORD PTR -320+[esp+356], eax
	mov	eax, DWORD PTR -72+[esp+356]
	push	eax
	mov	eax, DWORD PTR -112+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -320+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -320+[esp+356]
	mov	DWORD PTR -320+[esp+352], ecx
	mov	ecx, DWORD PTR -200+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -200+[esp+352]
	mov	DWORD PTR -320+[esp+356], eax
	mov	eax, DWORD PTR -248+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -248+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -320+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -96+[esp+352]
	mov	DWORD PTR -320+[esp+352], ecx
	mov	ecx, DWORD PTR -320+[esp+356]
	adc	ecx, edx
	mov	edx, DWORD PTR -96+[esp+356]
	mov	DWORD PTR -320+[esp+356], ecx
	mov	ecx, DWORD PTR -264+[esp+356]
	push	edx
	mov	edx, DWORD PTR -264+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -320+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -320+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -280+[esp+356]
	mov	DWORD PTR -320+[esp+356], eax
	mov	eax, DWORD PTR -192+[esp+356]
	mov	DWORD PTR -320+[esp+352], ecx
	mov	ecx, DWORD PTR -192+[esp+352]
	push	eax
	mov	eax, DWORD PTR -280+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -320+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -320+[esp+356]
	mov	DWORD PTR -320+[esp+352], ecx
	mov	ecx, DWORD PTR -232+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -232+[esp+352]
	mov	DWORD PTR -320+[esp+356], eax
	mov	eax, DWORD PTR -272+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -272+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -320+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -320+[esp+356]
	adc	eax, edx
	mov	DWORD PTR -320+[esp+352], ecx
	mov	DWORD PTR -320+[esp+356], eax
	mov	eax, DWORD PTR _carry0$[esp+352]
	mov	ecx, 26					; 0000001aH
	mov	edx, DWORD PTR _carry0$[esp+356]
	call	__allshr
	mov	ecx, DWORD PTR -320+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -320+[esp+356]
	adc	eax, edx
	mov	DWORD PTR _h1$[esp+352], ecx
	mov	DWORD PTR _h1$[esp+356], eax
	mov	eax, ebx
	add	eax, 33554432				; 02000000H
	mov	ecx, DWORD PTR -128+[esp+356]
	mov	DWORD PTR _carry4$[esp+352], eax
	mov	eax, ebp
	adc	eax, 0
	push	ecx
	mov	DWORD PTR _carry4$[esp+360], eax
	mov	eax, DWORD PTR _g5$[esp+356]
	mov	ecx, DWORD PTR -40+[esp+356]
	cdq
	mov	DWORD PTR -320+[esp+360], edx
	mov	edx, DWORD PTR -128+[esp+356]
	mov	DWORD PTR -320+[esp+356], eax
	mov	eax, DWORD PTR -40+[esp+360]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -104+[esp+356]
	mov	DWORD PTR -336+[esp+356], edx
	mov	edx, DWORD PTR -216+[esp+356]
	mov	DWORD PTR -336+[esp+352], eax
	mov	eax, DWORD PTR -216+[esp+352]
	push	edx
	mov	edx, DWORD PTR -104+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -336+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -320+[esp+356]
	mov	DWORD PTR -336+[esp+356], eax
	mov	eax, DWORD PTR -256+[esp+356]
	mov	DWORD PTR -336+[esp+352], ecx
	mov	ecx, DWORD PTR -256+[esp+352]
	push	eax
	mov	eax, DWORD PTR -320+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -336+[esp+356]
	mov	DWORD PTR -336+[esp+352], ecx
	mov	ecx, DWORD PTR -288+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -288+[esp+352]
	mov	DWORD PTR -336+[esp+356], eax
	mov	eax, DWORD PTR -120+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -120+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -208+[esp+352]
	mov	DWORD PTR -336+[esp+352], ecx
	mov	ecx, DWORD PTR -336+[esp+356]
	adc	ecx, edx
	mov	edx, DWORD PTR -208+[esp+356]
	mov	DWORD PTR -336+[esp+356], ecx
	mov	ecx, DWORD PTR -136+[esp+356]
	push	edx
	mov	edx, DWORD PTR -136+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -336+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -112+[esp+356]
	mov	DWORD PTR -336+[esp+356], eax
	mov	eax, DWORD PTR -152+[esp+356]
	mov	DWORD PTR -336+[esp+352], ecx
	mov	ecx, DWORD PTR -152+[esp+352]
	push	eax
	mov	eax, DWORD PTR -112+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -336+[esp+356]
	mov	DWORD PTR -336+[esp+352], ecx
	mov	ecx, DWORD PTR -272+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -272+[esp+352]
	mov	DWORD PTR -336+[esp+356], eax
	mov	eax, DWORD PTR -168+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -168+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -280+[esp+352]
	mov	DWORD PTR -336+[esp+352], ecx
	mov	ecx, DWORD PTR -336+[esp+356]
	adc	ecx, edx
	mov	edx, DWORD PTR -280+[esp+356]
	mov	DWORD PTR -336+[esp+356], ecx
	mov	ecx, DWORD PTR -224+[esp+356]
	push	edx
	mov	edx, DWORD PTR -224+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -336+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -248+[esp+356]
	mov	DWORD PTR -336+[esp+356], eax
	mov	eax, DWORD PTR -192+[esp+356]
	mov	DWORD PTR -336+[esp+352], ecx
	mov	ecx, DWORD PTR -192+[esp+352]
	push	eax
	mov	eax, DWORD PTR -248+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -336+[esp+356]
	mov	DWORD PTR -336+[esp+352], ecx
	mov	ecx, DWORD PTR -232+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -232+[esp+352]
	mov	DWORD PTR -336+[esp+356], eax
	mov	eax, DWORD PTR -264+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -264+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -336+[esp+356]
	mov	DWORD PTR -336+[esp+352], ecx
	adc	eax, edx
	mov	edx, DWORD PTR _carry4$[esp+356]
	mov	DWORD PTR -336+[esp+356], eax
	mov	eax, DWORD PTR _carry4$[esp+352]
	mov	ecx, 26					; 0000001aH
	call	__allshr
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -336+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -256+[esp+352]
	mov	DWORD PTR _h5$[esp+356], eax
	mov	eax, DWORD PTR _h1$[esp+352]
	add	eax, 16777216				; 01000000H
	mov	DWORD PTR _h5$[esp+352], ecx
	mov	ecx, DWORD PTR -256+[esp+356]
	mov	DWORD PTR _carry1$[esp+352], eax
	mov	eax, DWORD PTR _h1$[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -216+[esp+356]
	push	edx
	adc	eax, 0
	mov	DWORD PTR _carry1$[esp+364], eax
	mov	eax, DWORD PTR -216+[esp+364]
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -288+[esp+356]
	mov	DWORD PTR -336+[esp+356], edx
	mov	edx, DWORD PTR -272+[esp+356]
	mov	DWORD PTR -336+[esp+352], eax
	mov	eax, DWORD PTR -272+[esp+352]
	push	edx
	mov	edx, DWORD PTR -288+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -336+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -208+[esp+356]
	mov	DWORD PTR -336+[esp+356], eax
	mov	eax, DWORD PTR -280+[esp+356]
	mov	DWORD PTR -336+[esp+352], ecx
	mov	ecx, DWORD PTR -280+[esp+352]
	push	eax
	mov	eax, DWORD PTR -208+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -336+[esp+356]
	mov	DWORD PTR -336+[esp+352], ecx
	mov	ecx, DWORD PTR -264+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -264+[esp+352]
	mov	DWORD PTR -336+[esp+356], eax
	mov	eax, DWORD PTR -152+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -152+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -248+[esp+352]
	mov	DWORD PTR -336+[esp+352], ecx
	mov	ecx, DWORD PTR -336+[esp+356]
	adc	ecx, edx
	mov	edx, DWORD PTR -248+[esp+356]
	mov	DWORD PTR -336+[esp+356], ecx
	mov	ecx, DWORD PTR -184+[esp+356]
	push	edx
	mov	edx, DWORD PTR -184+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -336+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -224+[esp+356]
	mov	DWORD PTR -336+[esp+356], eax
	mov	eax, DWORD PTR -144+[esp+356]
	mov	DWORD PTR -336+[esp+352], ecx
	mov	ecx, DWORD PTR -144+[esp+352]
	push	eax
	mov	eax, DWORD PTR -224+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -336+[esp+356]
	mov	DWORD PTR -336+[esp+352], ecx
	mov	ecx, DWORD PTR -96+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -96+[esp+352]
	mov	DWORD PTR -336+[esp+356], eax
	mov	eax, DWORD PTR -64+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -64+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -192+[esp+352]
	mov	DWORD PTR -336+[esp+352], ecx
	mov	ecx, DWORD PTR -336+[esp+356]
	adc	ecx, edx
	mov	edx, DWORD PTR -192+[esp+356]
	mov	DWORD PTR -336+[esp+356], ecx
	mov	ecx, DWORD PTR -56+[esp+356]
	push	edx
	mov	edx, DWORD PTR -56+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -336+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -200+[esp+356]
	mov	DWORD PTR -336+[esp+356], eax
	mov	eax, DWORD PTR -176+[esp+356]
	mov	DWORD PTR -336+[esp+352], ecx
	mov	ecx, DWORD PTR -176+[esp+352]
	push	eax
	mov	eax, DWORD PTR -200+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -336+[esp+356]
	mov	DWORD PTR -336+[esp+352], ecx
	mov	ecx, DWORD PTR -232+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -232+[esp+352]
	mov	DWORD PTR -336+[esp+356], eax
	mov	eax, DWORD PTR -80+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -80+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -336+[esp+356]
	mov	DWORD PTR -336+[esp+352], ecx
	adc	eax, edx
	mov	edx, DWORD PTR _carry1$[esp+356]
	mov	DWORD PTR -336+[esp+356], eax
	mov	eax, DWORD PTR _carry1$[esp+352]
	mov	ecx, 25					; 00000019H
	call	__allshr
	mov	ecx, DWORD PTR -336+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -336+[esp+356]
	adc	eax, edx
	mov	DWORD PTR _h2$[esp+352], ecx
	mov	DWORD PTR _h2$[esp+356], eax
	mov	eax, DWORD PTR _h5$[esp+352]
	mov	ecx, DWORD PTR -256+[esp+356]
	add	eax, 16777216				; 01000000H
	mov	DWORD PTR _carry5$[esp+352], eax
	mov	eax, DWORD PTR _h5$[esp+356]
	adc	eax, 0
	push	ecx
	mov	ecx, DWORD PTR -256+[esp+356]
	mov	DWORD PTR _carry5$[esp+360], eax
	mov	eax, DWORD PTR _g6$[esp+356]
	push	ecx
	cdq
	push	edx
	push	eax
	mov	DWORD PTR -200+[esp+368], eax
	mov	DWORD PTR -200+[esp+372], edx
	call	__allmul
	mov	ecx, DWORD PTR -128+[esp+356]
	mov	DWORD PTR -328+[esp+356], edx
	mov	edx, DWORD PTR -272+[esp+356]
	mov	DWORD PTR -328+[esp+352], eax
	mov	eax, DWORD PTR -272+[esp+352]
	push	edx
	mov	edx, DWORD PTR -128+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -328+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -320+[esp+356]
	mov	DWORD PTR -328+[esp+356], eax
	mov	eax, DWORD PTR -144+[esp+356]
	mov	DWORD PTR -328+[esp+352], ecx
	mov	ecx, DWORD PTR -144+[esp+352]
	push	eax
	mov	eax, DWORD PTR -320+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -328+[esp+356]
	mov	DWORD PTR -328+[esp+352], ecx
	mov	ecx, DWORD PTR -280+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -280+[esp+352]
	mov	DWORD PTR -328+[esp+356], eax
	mov	eax, DWORD PTR -216+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -216+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -264+[esp+352]
	mov	DWORD PTR -328+[esp+352], ecx
	mov	ecx, DWORD PTR -328+[esp+356]
	adc	ecx, edx
	mov	edx, DWORD PTR -264+[esp+356]
	mov	DWORD PTR -328+[esp+356], ecx
	mov	ecx, DWORD PTR -288+[esp+356]
	push	edx
	push	eax
	mov	edx, DWORD PTR -288+[esp+360]
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -328+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -208+[esp+356]
	mov	DWORD PTR -328+[esp+356], eax
	mov	eax, DWORD PTR -248+[esp+356]
	mov	DWORD PTR -328+[esp+352], ecx
	mov	ecx, DWORD PTR -248+[esp+352]
	push	eax
	mov	eax, DWORD PTR -208+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -328+[esp+356]
	mov	DWORD PTR -328+[esp+352], ecx
	mov	ecx, DWORD PTR -56+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -56+[esp+352]
	mov	DWORD PTR -328+[esp+356], eax
	mov	eax, DWORD PTR -224+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -224+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -80+[esp+352]
	mov	DWORD PTR -328+[esp+352], ecx
	mov	ecx, DWORD PTR -328+[esp+356]
	adc	ecx, edx
	mov	edx, DWORD PTR -80+[esp+356]
	mov	DWORD PTR -328+[esp+356], ecx
	mov	ecx, DWORD PTR -168+[esp+356]
	push	edx
	mov	edx, DWORD PTR -168+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -328+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -64+[esp+356]
	mov	DWORD PTR -328+[esp+356], eax
	mov	eax, DWORD PTR -232+[esp+356]
	mov	DWORD PTR -328+[esp+352], ecx
	mov	ecx, DWORD PTR -232+[esp+352]
	push	eax
	mov	eax, DWORD PTR -64+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -328+[esp+356]
	mov	DWORD PTR -328+[esp+352], ecx
	mov	ecx, DWORD PTR -176+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -176+[esp+352]
	mov	DWORD PTR -328+[esp+356], eax
	mov	eax, DWORD PTR -192+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -192+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -328+[esp+356]
	mov	DWORD PTR -328+[esp+352], ecx
	adc	eax, edx
	mov	edx, DWORD PTR _carry5$[esp+356]
	mov	DWORD PTR -328+[esp+356], eax
	mov	eax, DWORD PTR _carry5$[esp+352]
	mov	ecx, 25					; 00000019H
	call	__allshr
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -328+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -216+[esp+352]
	mov	DWORD PTR _h6$[esp+356], eax
	mov	eax, DWORD PTR _h2$[esp+352]
	add	eax, 33554432				; 02000000H
	mov	DWORD PTR _h6$[esp+352], ecx
	mov	ecx, DWORD PTR -216+[esp+356]
	mov	DWORD PTR _carry2$[esp+352], eax
	mov	eax, DWORD PTR _h2$[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -40+[esp+356]
	push	edx
	adc	eax, 0
	mov	DWORD PTR _carry2$[esp+364], eax
	mov	eax, DWORD PTR -40+[esp+364]
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -104+[esp+356]
	mov	DWORD PTR -328+[esp+356], edx
	mov	edx, DWORD PTR -288+[esp+356]
	mov	DWORD PTR -328+[esp+352], eax
	mov	eax, DWORD PTR -288+[esp+352]
	push	edx
	mov	edx, DWORD PTR -104+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -328+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -120+[esp+356]
	mov	DWORD PTR -328+[esp+356], eax
	mov	eax, DWORD PTR -208+[esp+356]
	mov	DWORD PTR -328+[esp+352], ecx
	mov	ecx, DWORD PTR -208+[esp+352]
	push	eax
	mov	eax, DWORD PTR -120+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -328+[esp+356]
	mov	DWORD PTR -328+[esp+352], ecx
	mov	ecx, DWORD PTR -152+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -152+[esp+352]
	mov	DWORD PTR -328+[esp+356], eax
	mov	eax, DWORD PTR -136+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -136+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -256+[esp+352]
	mov	DWORD PTR -328+[esp+352], ecx
	mov	ecx, DWORD PTR -328+[esp+356]
	adc	ecx, edx
	mov	edx, DWORD PTR -256+[esp+356]
	mov	DWORD PTR -328+[esp+356], ecx
	mov	ecx, DWORD PTR -168+[esp+356]
	push	edx
	push	eax
	mov	edx, DWORD PTR -168+[esp+360]
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -328+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -224+[esp+356]
	mov	DWORD PTR -328+[esp+356], eax
	mov	eax, DWORD PTR -272+[esp+356]
	mov	DWORD PTR -328+[esp+352], ecx
	mov	ecx, DWORD PTR -272+[esp+352]
	push	eax
	mov	eax, DWORD PTR -224+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -328+[esp+356]
	mov	DWORD PTR -328+[esp+352], ecx
	mov	ecx, DWORD PTR -184+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -184+[esp+352]
	mov	DWORD PTR -328+[esp+356], eax
	mov	eax, DWORD PTR -112+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -112+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -96+[esp+352]
	mov	DWORD PTR -328+[esp+352], ecx
	mov	ecx, DWORD PTR -328+[esp+356]
	adc	ecx, edx
	mov	edx, DWORD PTR -96+[esp+356]
	mov	DWORD PTR -328+[esp+356], ecx
	mov	ecx, DWORD PTR -248+[esp+356]
	push	edx
	mov	edx, DWORD PTR -248+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -328+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -264+[esp+356]
	mov	DWORD PTR -328+[esp+356], eax
	mov	eax, DWORD PTR -192+[esp+356]
	mov	DWORD PTR -328+[esp+352], ecx
	mov	ecx, DWORD PTR -192+[esp+352]
	push	eax
	mov	eax, DWORD PTR -264+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -328+[esp+356]
	mov	DWORD PTR -328+[esp+352], ecx
	mov	ecx, DWORD PTR -232+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -232+[esp+352]
	mov	DWORD PTR -328+[esp+356], eax
	mov	eax, DWORD PTR -280+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -280+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -328+[esp+356]
	mov	DWORD PTR -328+[esp+352], ecx
	adc	eax, edx
	mov	edx, DWORD PTR _carry2$[esp+356]
	mov	DWORD PTR -328+[esp+356], eax
	mov	eax, DWORD PTR _carry2$[esp+352]
	mov	ecx, 26					; 0000001aH
	call	__allshr
	mov	ecx, DWORD PTR -328+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -328+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR _kTop38Bits+4
	mov	DWORD PTR _h3$[esp+352], ecx
	mov	ecx, DWORD PTR _kTop38Bits
	mov	DWORD PTR _h3$[esp+356], eax
	mov	eax, DWORD PTR _carry2$[esp+352]
	and	eax, ecx
	mov	ecx, DWORD PTR _carry2$[esp+356]
	and	ecx, edx
	mov	edx, DWORD PTR _h2$[esp+352]
	sub	edx, eax
	mov	eax, DWORD PTR _h2$[esp+356]
	sbb	eax, ecx
	mov	ecx, DWORD PTR -40+[esp+356]
	mov	eax, DWORD PTR _h6$[esp+352]
	mov	DWORD PTR _h2$[esp+352], edx
	add	eax, 33554432				; 02000000H
	push	ecx
	mov	DWORD PTR _carry6$[esp+356], eax
	mov	eax, DWORD PTR _h6$[esp+360]
	adc	eax, 0
	mov	ecx, DWORD PTR -200+[esp+356]
	mov	DWORD PTR _carry6$[esp+360], eax
	mov	eax, DWORD PTR _g7$[esp+356]
	cdq
	mov	DWORD PTR -184+[esp+360], edx
	mov	edx, DWORD PTR -40+[esp+356]
	mov	DWORD PTR -184+[esp+356], eax
	mov	eax, DWORD PTR -200+[esp+360]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -184+[esp+356]
	mov	DWORD PTR -312+[esp+356], edx
	mov	edx, DWORD PTR -256+[esp+356]
	mov	DWORD PTR -312+[esp+352], eax
	mov	eax, DWORD PTR -256+[esp+352]
	push	edx
	mov	edx, DWORD PTR -184+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -312+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -312+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -104+[esp+356]
	mov	DWORD PTR -312+[esp+356], eax
	mov	eax, DWORD PTR -128+[esp+356]
	mov	DWORD PTR -312+[esp+352], ecx
	mov	ecx, DWORD PTR -128+[esp+352]
	push	eax
	mov	eax, DWORD PTR -104+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -312+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -312+[esp+356]
	mov	DWORD PTR -312+[esp+352], ecx
	mov	ecx, DWORD PTR -216+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -216+[esp+352]
	mov	DWORD PTR -312+[esp+356], eax
	mov	eax, DWORD PTR -120+[esp+356]
	push	ecx
	push	edx
	mov	ecx, DWORD PTR -120+[esp+360]
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -312+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -272+[esp+352]
	mov	DWORD PTR -312+[esp+352], ecx
	mov	ecx, DWORD PTR -312+[esp+356]
	adc	ecx, edx
	mov	edx, DWORD PTR -272+[esp+356]
	mov	DWORD PTR -312+[esp+356], ecx
	mov	ecx, DWORD PTR -320+[esp+356]
	push	edx
	mov	edx, DWORD PTR -320+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -312+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -312+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -136+[esp+356]
	mov	DWORD PTR -312+[esp+356], eax
	mov	eax, DWORD PTR -288+[esp+356]
	mov	DWORD PTR -312+[esp+352], ecx
	mov	ecx, DWORD PTR -288+[esp+352]
	push	eax
	mov	eax, DWORD PTR -136+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -312+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -312+[esp+356]
	mov	DWORD PTR -312+[esp+352], ecx
	mov	ecx, DWORD PTR -208+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -208+[esp+352]
	mov	DWORD PTR -312+[esp+356], eax
	mov	eax, DWORD PTR -112+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -112+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -312+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -280+[esp+352]
	mov	DWORD PTR -312+[esp+352], ecx
	mov	ecx, DWORD PTR -312+[esp+356]
	adc	ecx, edx
	mov	edx, DWORD PTR -280+[esp+356]
	mov	DWORD PTR -312+[esp+356], ecx
	mov	ecx, DWORD PTR -168+[esp+356]
	push	edx
	mov	edx, DWORD PTR -168+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -312+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -312+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -224+[esp+356]
	mov	DWORD PTR -312+[esp+356], eax
	mov	eax, DWORD PTR -264+[esp+356]
	mov	DWORD PTR -312+[esp+352], ecx
	mov	ecx, DWORD PTR -264+[esp+352]
	push	eax
	mov	eax, DWORD PTR -224+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -312+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -312+[esp+356]
	mov	DWORD PTR -312+[esp+352], ecx
	mov	ecx, DWORD PTR -232+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -232+[esp+352]
	mov	DWORD PTR -312+[esp+356], eax
	mov	eax, DWORD PTR -248+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -248+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -312+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -312+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR _carry6$[esp+356]
	mov	DWORD PTR -312+[esp+352], ecx
	mov	DWORD PTR -312+[esp+356], eax
	mov	eax, DWORD PTR _carry6$[esp+352]
	mov	ecx, 26					; 0000001aH
	call	__allshr
	mov	ecx, DWORD PTR -312+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -312+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR _kTop38Bits+4
	mov	DWORD PTR _h7$[esp+352], ecx
	mov	ecx, DWORD PTR _kTop38Bits
	mov	DWORD PTR _h7$[esp+356], eax
	mov	eax, DWORD PTR _carry6$[esp+352]
	and	eax, ecx
	mov	ecx, DWORD PTR _carry6$[esp+356]
	and	ecx, edx
	mov	edx, DWORD PTR _h6$[esp+352]
	sub	edx, eax
	mov	eax, DWORD PTR _h6$[esp+356]
	sbb	eax, ecx
	mov	ecx, DWORD PTR _h3$[esp+356]
	mov	eax, DWORD PTR _h3$[esp+352]
	mov	DWORD PTR _h6$[esp+352], edx
	mov	edx, DWORD PTR _kTop38Bits
	add	eax, 16777216				; 01000000H
	adc	ecx, 0
	mov	DWORD PTR _carry3$[esp+352], eax
	mov	DWORD PTR _carry3$[esp+356], ecx
	mov	ecx, DWORD PTR _carry4$[esp+352]
	and	ecx, edx
	mov	edx, DWORD PTR _carry4$[esp+356]
	mov	DWORD PTR -312+[esp+352], ecx
	mov	ecx, DWORD PTR _kTop38Bits+4
	and	edx, ecx
	mov	ecx, 25					; 00000019H
	mov	DWORD PTR -312+[esp+356], edx
	mov	edx, DWORD PTR _carry3$[esp+356]
	call	__allshr
	mov	ecx, DWORD PTR -312+[esp+352]
	sub	eax, ecx
	mov	ecx, DWORD PTR -312+[esp+356]
	sbb	edx, ecx
	mov	ecx, DWORD PTR _kTop39Bits
	add	ebx, eax
	mov	eax, DWORD PTR _carry3$[esp+352]
	adc	ebp, edx
	mov	edx, DWORD PTR _kTop39Bits+4
	and	eax, ecx
	mov	ecx, DWORD PTR _carry3$[esp+356]
	and	ecx, edx
	mov	edx, DWORD PTR _h3$[esp+352]
	sub	edx, eax
	mov	eax, DWORD PTR _h3$[esp+356]
	sbb	eax, ecx
	mov	DWORD PTR _h3$[esp+352], edx
	mov	eax, DWORD PTR _h7$[esp+352]
	add	eax, 16777216				; 01000000H
	mov	DWORD PTR _carry7$[esp+352], eax
	mov	eax, DWORD PTR _h7$[esp+356]
	adc	eax, 0
	mov	DWORD PTR _carry7$[esp+356], eax
	mov	eax, DWORD PTR _g8$[esp+352]
	mov	ecx, DWORD PTR -256+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -256+[esp+356]
	cdq
	push	ecx
	push	edx
	push	eax
	mov	DWORD PTR -312+[esp+368], eax
	mov	DWORD PTR -312+[esp+372], edx
	call	__allmul
	mov	ecx, DWORD PTR -200+[esp+356]
	mov	DWORD PTR -304+[esp+356], edx
	mov	edx, DWORD PTR -272+[esp+356]
	mov	DWORD PTR -304+[esp+352], eax
	mov	eax, DWORD PTR -272+[esp+352]
	push	edx
	mov	edx, DWORD PTR -200+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -304+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -304+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -184+[esp+356]
	mov	DWORD PTR -304+[esp+356], eax
	mov	eax, DWORD PTR -144+[esp+356]
	mov	DWORD PTR -304+[esp+352], ecx
	mov	ecx, DWORD PTR -144+[esp+352]
	push	eax
	mov	eax, DWORD PTR -184+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -304+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -304+[esp+356]
	mov	DWORD PTR -304+[esp+352], ecx
	mov	ecx, DWORD PTR -80+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -80+[esp+352]
	mov	DWORD PTR -304+[esp+356], eax
	mov	eax, DWORD PTR -320+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -320+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -304+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -280+[esp+352]
	mov	DWORD PTR -304+[esp+352], ecx
	mov	ecx, DWORD PTR -304+[esp+356]
	adc	ecx, edx
	mov	edx, DWORD PTR -280+[esp+356]
	mov	DWORD PTR -304+[esp+356], ecx
	mov	ecx, DWORD PTR -128+[esp+356]
	push	edx
	mov	edx, DWORD PTR -128+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -304+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -304+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -216+[esp+356]
	mov	DWORD PTR -304+[esp+356], eax
	mov	eax, DWORD PTR -264+[esp+356]
	mov	DWORD PTR -304+[esp+352], ecx
	mov	ecx, DWORD PTR -264+[esp+352]
	push	eax
	mov	eax, DWORD PTR -216+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -304+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -304+[esp+356]
	mov	DWORD PTR -304+[esp+352], ecx
	mov	ecx, DWORD PTR -64+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -64+[esp+352]
	mov	DWORD PTR -304+[esp+356], eax
	mov	eax, DWORD PTR -224+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -224+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -304+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -248+[esp+352]
	mov	DWORD PTR -304+[esp+352], ecx
	mov	ecx, DWORD PTR -304+[esp+356]
	adc	ecx, edx
	mov	edx, DWORD PTR -248+[esp+356]
	mov	DWORD PTR -304+[esp+356], ecx
	mov	ecx, DWORD PTR -288+[esp+356]
	push	edx
	mov	edx, DWORD PTR -288+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -304+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -304+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -168+[esp+356]
	mov	DWORD PTR -304+[esp+356], eax
	mov	eax, DWORD PTR -56+[esp+356]
	mov	DWORD PTR -304+[esp+352], ecx
	mov	ecx, DWORD PTR -56+[esp+352]
	push	eax
	mov	eax, DWORD PTR -168+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -304+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -304+[esp+356]
	mov	DWORD PTR -304+[esp+352], ecx
	mov	ecx, DWORD PTR -176+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -176+[esp+352]
	mov	DWORD PTR -304+[esp+356], eax
	mov	eax, DWORD PTR -232+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -232+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -304+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -304+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR _carry7$[esp+356]
	mov	DWORD PTR -304+[esp+352], ecx
	mov	DWORD PTR -304+[esp+356], eax
	mov	eax, DWORD PTR _carry7$[esp+352]
	mov	ecx, 25					; 00000019H
	call	__allshr
	mov	ecx, DWORD PTR -304+[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR -304+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR _kTop39Bits+4
	mov	DWORD PTR _h8$[esp+352], ecx
	mov	ecx, DWORD PTR _kTop39Bits
	mov	DWORD PTR _h8$[esp+356], eax
	mov	eax, DWORD PTR _carry7$[esp+352]
	and	eax, ecx
	mov	ecx, DWORD PTR _carry7$[esp+356]
	and	ecx, edx
	mov	edx, DWORD PTR _h7$[esp+352]
	sub	edx, eax
	mov	eax, DWORD PTR _h7$[esp+356]
	sbb	eax, ecx
	mov	ecx, ebp
	mov	eax, ebx
	mov	DWORD PTR _h7$[esp+352], edx
	mov	edx, DWORD PTR _kTop39Bits
	add	eax, 33554432				; 02000000H
	adc	ecx, 0
	mov	DWORD PTR _carry4$[esp+352], eax
	mov	DWORD PTR _carry4$[esp+356], ecx
	mov	ecx, DWORD PTR _carry5$[esp+352]
	and	ecx, edx
	mov	edx, DWORD PTR _carry5$[esp+356]
	mov	DWORD PTR -296+[esp+352], ecx
	mov	ecx, DWORD PTR _kTop39Bits+4
	and	edx, ecx
	mov	ecx, 26					; 0000001aH
	mov	DWORD PTR -296+[esp+356], edx
	mov	edx, DWORD PTR _carry4$[esp+356]
	call	__allshr
	mov	ecx, DWORD PTR -296+[esp+352]
	sub	eax, ecx
	mov	ecx, DWORD PTR -296+[esp+356]
	sbb	edx, ecx
	mov	ecx, DWORD PTR _h5$[esp+352]
	add	ecx, eax
	mov	eax, DWORD PTR _h5$[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _carry4$[esp+352]
	mov	DWORD PTR _h5$[esp+352], ecx
	mov	ecx, DWORD PTR _kTop38Bits
	and	eax, ecx
	mov	ecx, DWORD PTR _carry4$[esp+356]
	and	ecx, edx
	sub	ebx, eax
	mov	eax, DWORD PTR _h8$[esp+352]
	sbb	ebp, ecx
	mov	ecx, DWORD PTR -256+[esp+356]
	add	eax, 33554432				; 02000000H
	push	ecx
	mov	DWORD PTR _carry8$[esp+356], eax
	mov	eax, DWORD PTR _h8$[esp+360]
	mov	ecx, DWORD PTR -256+[esp+356]
	adc	eax, 0
	push	ecx
	mov	DWORD PTR _carry8$[esp+364], eax
	mov	eax, DWORD PTR _g9$[esp+360]
	cdq
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -312+[esp+356]
	mov	DWORD PTR -240+[esp+356], edx
	mov	edx, DWORD PTR -40+[esp+356]
	mov	ebp, eax
	mov	eax, DWORD PTR -40+[esp+352]
	push	edx
	mov	edx, DWORD PTR -312+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -104+[esp+352]
	add	ebp, eax
	mov	eax, DWORD PTR -240+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -200+[esp+356]
	mov	DWORD PTR -240+[esp+356], eax
	mov	eax, DWORD PTR -104+[esp+356]
	push	eax
	mov	eax, DWORD PTR -200+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	add	ebp, eax
	mov	eax, DWORD PTR -240+[esp+356]
	mov	ecx, DWORD PTR -272+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -272+[esp+352]
	mov	DWORD PTR -240+[esp+356], eax
	mov	eax, DWORD PTR -184+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -184+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -240+[esp+356]
	add	ebp, eax
	mov	eax, DWORD PTR -128+[esp+352]
	adc	ecx, edx
	mov	edx, DWORD PTR -128+[esp+356]
	mov	DWORD PTR -240+[esp+356], ecx
	mov	ecx, DWORD PTR -120+[esp+356]
	push	edx
	mov	edx, DWORD PTR -120+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -216+[esp+352]
	add	ebp, eax
	mov	eax, DWORD PTR -240+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -136+[esp+356]
	mov	DWORD PTR -240+[esp+356], eax
	mov	eax, DWORD PTR -216+[esp+356]
	push	eax
	mov	eax, DWORD PTR -136+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -280+[esp+356]
	add	ebp, eax
	mov	eax, DWORD PTR -240+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -320+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -280+[esp+356]
	mov	DWORD PTR -240+[esp+360], eax
	mov	eax, DWORD PTR -320+[esp+360]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -240+[esp+356]
	add	ebp, eax
	mov	eax, DWORD PTR -288+[esp+352]
	adc	ecx, edx
	mov	edx, DWORD PTR -288+[esp+356]
	mov	DWORD PTR -240+[esp+356], ecx
	mov	ecx, DWORD PTR -112+[esp+356]
	push	edx
	mov	edx, DWORD PTR -112+[esp+356]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -264+[esp+352]
	add	ebp, eax
	mov	eax, DWORD PTR -240+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR -168+[esp+356]
	mov	DWORD PTR -240+[esp+356], eax
	mov	eax, DWORD PTR -264+[esp+356]
	push	eax
	mov	eax, DWORD PTR -168+[esp+356]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -248+[esp+356]
	add	ebp, eax
	adc	DWORD PTR -240+[esp+356], edx
	mov	edx, DWORD PTR -248+[esp+352]
	mov	eax, DWORD PTR -224+[esp+356]
	push	ecx
	mov	ecx, DWORD PTR -224+[esp+356]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	add	ebp, eax
	mov	eax, DWORD PTR -240+[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR _carry8$[esp+356]
	mov	DWORD PTR -240+[esp+356], eax
	mov	eax, DWORD PTR _carry8$[esp+352]
	mov	ecx, 26					; 0000001aH
	call	__allshr
	add	ebp, eax
	mov	eax, DWORD PTR -240+[esp+356]
	mov	ecx, DWORD PTR _kTop38Bits+4
	adc	eax, edx
	mov	edx, DWORD PTR _kTop38Bits
	mov	DWORD PTR _h9$[esp+356], eax
	mov	eax, DWORD PTR _carry8$[esp+352]
	and	eax, edx
	mov	edx, DWORD PTR _carry8$[esp+356]
	and	edx, ecx
	mov	ecx, DWORD PTR _h8$[esp+352]
	sub	ecx, eax
	mov	eax, DWORD PTR _h8$[esp+356]
	sbb	eax, edx
	mov	edx, DWORD PTR _kTop38Bits
	mov	eax, ebp
	mov	DWORD PTR _h8$[esp+352], ecx
	mov	ecx, DWORD PTR _h9$[esp+356]
	add	eax, 16777216				; 01000000H
	adc	ecx, 0
	mov	DWORD PTR _carry9$[esp+352], eax
	mov	DWORD PTR _carry9$[esp+356], ecx
	mov	ecx, DWORD PTR _carry0$[esp+352]
	and	ecx, edx
	mov	edx, DWORD PTR _carry0$[esp+356]
	mov	DWORD PTR -296+[esp+352], ecx
	mov	ecx, DWORD PTR _kTop38Bits+4
	and	edx, ecx
	mov	ecx, 25					; 00000019H
	mov	DWORD PTR -296+[esp+356], edx
	mov	edx, DWORD PTR _carry9$[esp+356]
	call	__allshr
	push	0
	push	19					; 00000013H
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -296+[esp+352]
	sub	eax, ecx
	mov	ecx, DWORD PTR -296+[esp+356]
	sbb	edx, ecx
	mov	ecx, DWORD PTR _carry9$[esp+356]
	add	esi, eax
	mov	eax, DWORD PTR _carry9$[esp+352]
	adc	edi, edx
	mov	edx, DWORD PTR _kTop39Bits
	and	ecx, DWORD PTR _kTop39Bits+4
	and	eax, edx
	sub	ebp, eax
	mov	eax, DWORD PTR _h9$[esp+356]
	sbb	eax, ecx
	mov	ecx, DWORD PTR _carry1$[esp+356]
	mov	eax, esi
	add	eax, 33554432				; 02000000H
	adc	edi, 0
	mov	DWORD PTR _carry0$[esp+352], eax
	mov	DWORD PTR _carry0$[esp+356], edi
	mov	edi, DWORD PTR _carry1$[esp+352]
	and	edi, edx
	mov	edx, DWORD PTR _kTop39Bits+4
	and	ecx, edx
	mov	edx, DWORD PTR _carry0$[esp+356]
	mov	DWORD PTR -24+[esp+356], ecx
	mov	ecx, 26					; 0000001aH
	call	__allshr
	mov	ecx, DWORD PTR -24+[esp+356]
	sub	eax, edi
	sbb	edx, ecx
	mov	ecx, DWORD PTR _h1$[esp+352]
	mov	edi, DWORD PTR _kTop38Bits
	add	ecx, eax
	mov	eax, DWORD PTR _h1$[esp+356]
	adc	eax, edx
	mov	edx, DWORD PTR _h2$[esp+352]
	mov	eax, DWORD PTR _carry0$[esp+352]
	and	eax, edi
	pop	edi
	sub	esi, eax
	mov	eax, DWORD PTR _h$[esp+344]
	mov	DWORD PTR [eax+4], ecx
	mov	ecx, DWORD PTR _h3$[esp+348]
	mov	DWORD PTR [eax+8], edx
	mov	edx, DWORD PTR _h5$[esp+348]
	mov	DWORD PTR [eax+12], ecx
	mov	ecx, DWORD PTR _h6$[esp+348]
	mov	DWORD PTR [eax], esi
	mov	DWORD PTR [eax+20], edx
	mov	edx, DWORD PTR _h7$[esp+348]
	mov	DWORD PTR [eax+24], ecx
	mov	ecx, DWORD PTR _h8$[esp+348]
	mov	DWORD PTR [eax+36], ebp
	pop	esi
	mov	DWORD PTR [eax+16], ebx
	pop	ebp
	mov	DWORD PTR [eax+28], edx
	mov	DWORD PTR [eax+32], ecx
	pop	ebx

; 554  : }

	add	esp, 336				; 00000150H
	ret	0
_fe_mul	ENDP
_TEXT	ENDS
;	COMDAT _fe_invert
_TEXT	SEGMENT
_out$ = 8
_z$ = 12
_t0$ = -40
_t1$ = -120
_t2$ = -160
_t3$ = -80
_fe_invert PROC NEAR					; COMDAT

; 713  : {

	sub	esp, 160				; 000000a0H

; 714  :     fe t0;
; 715  :     fe t1;
; 716  :     fe t2;
; 717  :     fe t3;
; 718  :     int i;
; 719  : 
; 720  :     /*
; 721  :      * Compute z ** -1 = z ** (2 ** 255 - 19 - 2) with the exponent as
; 722  :      * 2 ** 255 - 21 = (2 ** 5) * (2 ** 250 - 1) + 11.
; 723  :      */
; 724  : 
; 725  :     /* t0 = z ** 2 */
; 726  :     fe_sq(t0, z);

	lea	eax, DWORD PTR _t0$[esp+160]
	push	esi
	mov	esi, DWORD PTR _z$[esp+160]
	push	esi
	push	eax
	call	_fe_sq

; 727  : 
; 728  :     /* t1 = t0 ** (2 ** 2) = z ** 8 */
; 729  :     fe_sq(t1, t0);

	lea	ecx, DWORD PTR _t0$[esp+172]
	lea	edx, DWORD PTR _t1$[esp+172]
	push	ecx
	push	edx
	call	_fe_sq

; 730  :     fe_sq(t1, t1);

	lea	eax, DWORD PTR _t1$[esp+180]
	lea	ecx, DWORD PTR _t1$[esp+180]
	push	eax
	push	ecx
	call	_fe_sq

; 731  : 
; 732  :     /* t1 = z * t1 = z ** 9 */
; 733  :     fe_mul(t1, z, t1);

	lea	edx, DWORD PTR _t1$[esp+188]
	lea	eax, DWORD PTR _t1$[esp+188]
	push	edx
	push	esi
	push	eax
	call	_fe_mul

; 734  :     /* t0 = t0 * t1 = z ** 11 -- stash t0 away for the end. */
; 735  :     fe_mul(t0, t0, t1);

	lea	ecx, DWORD PTR _t1$[esp+200]
	lea	edx, DWORD PTR _t0$[esp+200]
	push	ecx
	lea	eax, DWORD PTR _t0$[esp+204]
	push	edx
	push	eax
	call	_fe_mul

; 736  : 
; 737  :     /* t2 = t0 ** 2 = z ** 22 */
; 738  :     fe_sq(t2, t0);

	lea	ecx, DWORD PTR _t0$[esp+212]
	lea	edx, DWORD PTR _t2$[esp+212]
	push	ecx
	push	edx
	call	_fe_sq

; 739  : 
; 740  :     /* t1 = t1 * t2 = z ** (2 ** 5 - 1) */
; 741  :     fe_mul(t1, t1, t2);

	lea	eax, DWORD PTR _t2$[esp+220]
	lea	ecx, DWORD PTR _t1$[esp+220]
	push	eax
	lea	edx, DWORD PTR _t1$[esp+224]
	push	ecx
	push	edx
	call	_fe_mul
	add	esp, 68					; 00000044H

; 742  : 
; 743  :     /* t2 = t1 ** (2 ** 5) = z ** ((2 ** 5) * (2 ** 5 - 1)) */
; 744  :     fe_sq(t2, t1);

	lea	eax, DWORD PTR _t1$[esp+164]
	lea	ecx, DWORD PTR _t2$[esp+164]
	push	eax
	push	ecx
	call	_fe_sq
	add	esp, 8
	mov	esi, 4
$L1549:

; 745  :     for (i = 1; i < 5; ++i) {
; 746  :         fe_sq(t2, t2);

	lea	edx, DWORD PTR _t2$[esp+164]
	lea	eax, DWORD PTR _t2$[esp+164]
	push	edx
	push	eax
	call	_fe_sq
	add	esp, 8
	dec	esi
	jne	SHORT $L1549

; 747  :     }
; 748  : 
; 749  :     /* t1 = t1 * t2 = z ** ((2 ** 5 + 1) * (2 ** 5 - 1)) = z ** (2 ** 10 - 1) */
; 750  :     fe_mul(t1, t2, t1);

	lea	ecx, DWORD PTR _t1$[esp+164]
	lea	edx, DWORD PTR _t2$[esp+164]
	push	ecx
	lea	eax, DWORD PTR _t1$[esp+168]
	push	edx
	push	eax
	call	_fe_mul

; 751  : 
; 752  :     /* Continuing similarly... */
; 753  : 
; 754  :     /* t2 = z ** (2 ** 20 - 1) */
; 755  :     fe_sq(t2, t1);

	lea	ecx, DWORD PTR _t1$[esp+176]
	lea	edx, DWORD PTR _t2$[esp+176]
	push	ecx
	push	edx
	call	_fe_sq
	add	esp, 20					; 00000014H
	mov	esi, 9
$L1552:

; 756  :     for (i = 1; i < 10; ++i) {
; 757  :         fe_sq(t2, t2);

	lea	eax, DWORD PTR _t2$[esp+164]
	lea	ecx, DWORD PTR _t2$[esp+164]
	push	eax
	push	ecx
	call	_fe_sq
	add	esp, 8
	dec	esi
	jne	SHORT $L1552

; 758  :     }
; 759  :     fe_mul(t2, t2, t1);

	lea	edx, DWORD PTR _t1$[esp+164]
	lea	eax, DWORD PTR _t2$[esp+164]
	push	edx
	lea	ecx, DWORD PTR _t2$[esp+168]
	push	eax
	push	ecx
	call	_fe_mul

; 760  : 
; 761  :     /* t2 = z ** (2 ** 40 - 1) */
; 762  :     fe_sq(t3, t2);

	lea	edx, DWORD PTR _t2$[esp+176]
	lea	eax, DWORD PTR _t3$[esp+176]
	push	edx
	push	eax
	call	_fe_sq
	add	esp, 20					; 00000014H
	mov	esi, 19					; 00000013H
$L1555:

; 763  :     for (i = 1; i < 20; ++i) {
; 764  :         fe_sq(t3, t3);

	lea	ecx, DWORD PTR _t3$[esp+164]
	lea	edx, DWORD PTR _t3$[esp+164]
	push	ecx
	push	edx
	call	_fe_sq
	add	esp, 8
	dec	esi
	jne	SHORT $L1555

; 765  :     }
; 766  :     fe_mul(t2, t3, t2);

	lea	eax, DWORD PTR _t2$[esp+164]
	lea	ecx, DWORD PTR _t3$[esp+164]
	push	eax
	lea	edx, DWORD PTR _t2$[esp+168]
	push	ecx
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH
	mov	esi, 10					; 0000000aH
$L1558:

; 767  : 
; 768  :     /* t2 = z ** (2 ** 10) * (2 ** 40 - 1) */
; 769  :     for (i = 0; i < 10; ++i) {
; 770  :         fe_sq(t2, t2);

	lea	eax, DWORD PTR _t2$[esp+164]
	lea	ecx, DWORD PTR _t2$[esp+164]
	push	eax
	push	ecx
	call	_fe_sq
	add	esp, 8
	dec	esi
	jne	SHORT $L1558

; 771  :     }
; 772  :     /* t1 = z ** (2 ** 50 - 1) */
; 773  :     fe_mul(t1, t2, t1);

	lea	edx, DWORD PTR _t1$[esp+164]
	lea	eax, DWORD PTR _t2$[esp+164]
	push	edx
	lea	ecx, DWORD PTR _t1$[esp+168]
	push	eax
	push	ecx
	call	_fe_mul

; 774  : 
; 775  :     /* t2 = z ** (2 ** 100 - 1) */
; 776  :     fe_sq(t2, t1);

	lea	edx, DWORD PTR _t1$[esp+176]
	lea	eax, DWORD PTR _t2$[esp+176]
	push	edx
	push	eax
	call	_fe_sq
	add	esp, 20					; 00000014H
	mov	esi, 49					; 00000031H
$L1561:

; 777  :     for (i = 1; i < 50; ++i) {
; 778  :         fe_sq(t2, t2);

	lea	ecx, DWORD PTR _t2$[esp+164]
	lea	edx, DWORD PTR _t2$[esp+164]
	push	ecx
	push	edx
	call	_fe_sq
	add	esp, 8
	dec	esi
	jne	SHORT $L1561

; 779  :     }
; 780  :     fe_mul(t2, t2, t1);

	lea	eax, DWORD PTR _t1$[esp+164]
	lea	ecx, DWORD PTR _t2$[esp+164]
	push	eax
	lea	edx, DWORD PTR _t2$[esp+168]
	push	ecx
	push	edx
	call	_fe_mul

; 781  : 
; 782  :     /* t2 = z ** (2 ** 200 - 1) */
; 783  :     fe_sq(t3, t2);

	lea	eax, DWORD PTR _t2$[esp+176]
	lea	ecx, DWORD PTR _t3$[esp+176]
	push	eax
	push	ecx
	call	_fe_sq
	add	esp, 20					; 00000014H
	mov	esi, 99					; 00000063H
$L1564:

; 784  :     for (i = 1; i < 100; ++i) {
; 785  :         fe_sq(t3, t3);

	lea	edx, DWORD PTR _t3$[esp+164]
	lea	eax, DWORD PTR _t3$[esp+164]
	push	edx
	push	eax
	call	_fe_sq
	add	esp, 8
	dec	esi
	jne	SHORT $L1564

; 786  :     }
; 787  :     fe_mul(t2, t3, t2);

	lea	ecx, DWORD PTR _t2$[esp+164]
	lea	edx, DWORD PTR _t3$[esp+164]
	push	ecx
	lea	eax, DWORD PTR _t2$[esp+168]
	push	edx
	push	eax
	call	_fe_mul

; 788  : 
; 789  :     /* t2 = z ** ((2 ** 50) * (2 ** 200 - 1) */
; 790  :     fe_sq(t2, t2);

	lea	ecx, DWORD PTR _t2$[esp+176]
	lea	edx, DWORD PTR _t2$[esp+176]
	push	ecx
	push	edx
	call	_fe_sq
	add	esp, 20					; 00000014H
	mov	esi, 49					; 00000031H
$L1567:

; 791  :     for (i = 1; i < 50; ++i) {
; 792  :         fe_sq(t2, t2);

	lea	eax, DWORD PTR _t2$[esp+164]
	lea	ecx, DWORD PTR _t2$[esp+164]
	push	eax
	push	ecx
	call	_fe_sq
	add	esp, 8
	dec	esi
	jne	SHORT $L1567

; 793  :     }
; 794  : 
; 795  :     /* t1 = z ** (2 ** 250 - 1) */
; 796  :     fe_mul(t1, t2, t1);

	lea	edx, DWORD PTR _t1$[esp+164]
	lea	eax, DWORD PTR _t2$[esp+164]
	push	edx
	lea	ecx, DWORD PTR _t1$[esp+168]
	push	eax
	push	ecx
	call	_fe_mul

; 797  : 
; 798  :     /* t1 = z ** ((2 ** 5) * (2 ** 250 - 1)) */
; 799  :     fe_sq(t1, t1);

	lea	edx, DWORD PTR _t1$[esp+176]
	lea	eax, DWORD PTR _t1$[esp+176]
	push	edx
	push	eax
	call	_fe_sq
	add	esp, 20					; 00000014H
	mov	esi, 4
$L1570:

; 800  :     for (i = 1; i < 5; ++i) {
; 801  :         fe_sq(t1, t1);

	lea	ecx, DWORD PTR _t1$[esp+164]
	lea	edx, DWORD PTR _t1$[esp+164]
	push	ecx
	push	edx
	call	_fe_sq
	add	esp, 8
	dec	esi
	jne	SHORT $L1570

; 802  :     }
; 803  : 
; 804  :     /* Recall t0 = z ** 11; out = z ** (2 ** 255 - 21) */
; 805  :     fe_mul(out, t1, t0);

	mov	edx, DWORD PTR _out$[esp+160]
	lea	eax, DWORD PTR _t0$[esp+164]
	lea	ecx, DWORD PTR _t1$[esp+164]
	push	eax
	push	ecx
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH
	pop	esi

; 806  : }

	add	esp, 160				; 000000a0H
	ret	0
_fe_invert ENDP
_TEXT	ENDS
;	COMDAT _fe_sq
_TEXT	SEGMENT
_h$ = 8
_f$ = 12
_f0$ = -112
_f1$ = -56
_f2$ = -200
_f3$ = -64
_f4$ = -72
_f6$ = -216
_f8$ = -224
_f9$ = -208
_f0_2$ = -160
_f1_2$ = -176
_f2_2$ = -168
_f3_2$ = -144
_f4_2$ = -104
_f5_2$ = -120
_f7_2$ = -128
_f6_19$ = -152
_f8_19$ = -184
_h1$ = -48
_h2$ = -96
_h3$ = -184
_h5$ = -64
_h6$ = -32
_h7$ = -152
_h8$ = -224
_h9$ = -88
_carry0$ = -16
_carry1$ = -24
_carry2$ = -192
_carry3$ = -192
_carry4$ = -200
_carry5$ = -136
_carry6$ = -192
_carry7$ = -192
_carry8$ = -136
_carry9$ = -136
_fe_sq	PROC NEAR					; COMDAT

; 557  : {

	sub	esp, 224				; 000000e0H

; 558  :     int32_t f0 = f[0];

	mov	eax, DWORD PTR _f$[esp+220]
	push	ebx
	push	ebp
	push	esi

; 559  :     int32_t f1 = f[1];

	mov	ecx, DWORD PTR [eax+4]

; 560  :     int32_t f2 = f[2];

	mov	edx, DWORD PTR [eax+8]
	mov	DWORD PTR _f1$[esp+236], ecx

; 561  :     int32_t f3 = f[3];

	mov	ecx, DWORD PTR [eax+12]
	mov	DWORD PTR _f2$[esp+236], edx

; 562  :     int32_t f4 = f[4];
; 563  :     int32_t f5 = f[5];
; 564  :     int32_t f6 = f[6];
; 565  :     int32_t f7 = f[7];
; 566  :     int32_t f8 = f[8];
; 567  :     int32_t f9 = f[9];
; 568  :     int32_t f0_2 = 2 * f0;
; 569  :     int32_t f1_2 = 2 * f1;
; 570  :     int32_t f2_2 = 2 * f2;

	add	edx, edx
	mov	DWORD PTR _f3$[esp+236], ecx
	mov	ecx, DWORD PTR [eax+16]
	mov	esi, DWORD PTR [eax+20]
	mov	DWORD PTR _f2_2$[esp+236], edx

; 571  :     int32_t f3_2 = 2 * f3;

	mov	edx, DWORD PTR _f3$[esp+236]
	mov	ebp, DWORD PTR [eax+28]
	add	edx, edx
	mov	DWORD PTR _f4$[esp+236], ecx
	mov	DWORD PTR _f3_2$[esp+236], edx

; 572  :     int32_t f4_2 = 2 * f4;

	mov	edx, DWORD PTR _f4$[esp+236]
	mov	ebx, DWORD PTR [eax+24]
	add	edx, edx
	mov	DWORD PTR _f4_2$[esp+236], edx

; 573  :     int32_t f5_2 = 2 * f5;

	lea	edx, DWORD PTR [esi+esi]
	mov	DWORD PTR _f5_2$[esp+236], edx
	push	edi
	mov	edi, DWORD PTR [eax]

; 574  :     int32_t f6_2 = 2 * f6;
; 575  :     int32_t f7_2 = 2 * f7;

	lea	edx, DWORD PTR [ebp+ebp]
	mov	ecx, DWORD PTR [eax+32]
	mov	DWORD PTR _f7_2$[esp+240], edx
	mov	DWORD PTR _f0$[esp+240], edi

; 576  :     int32_t f5_38 = 38 * f5; /* 1.959375*2^30 */
; 577  :     int32_t f6_19 = 19 * f6; /* 1.959375*2^30 */

	lea	edx, DWORD PTR [ebx+ebx*8]
	add	edi, edi
	mov	eax, DWORD PTR [eax+36]
	mov	DWORD PTR _f0_2$[esp+240], edi
	mov	edi, DWORD PTR _f1$[esp+240]
	lea	edx, DWORD PTR [ebx+edx*2]
	add	edi, edi
	mov	DWORD PTR _f6_19$[esp+240], edx

; 578  :     int32_t f7_38 = 38 * f7; /* 1.959375*2^30 */

	lea	edx, DWORD PTR [ebp+ebp*8]
	mov	DWORD PTR _f1_2$[esp+240], edi
	mov	DWORD PTR _f8$[esp+240], ecx
	lea	edi, DWORD PTR [ebp+edx*2]

; 579  :     int32_t f8_19 = 19 * f8; /* 1.959375*2^30 */

	lea	edx, DWORD PTR [ecx+ecx*8]
	mov	DWORD PTR _f9$[esp+240], eax
	mov	DWORD PTR _f6$[esp+240], ebx
	lea	ecx, DWORD PTR [ecx+edx*2]

; 580  :     int32_t f9_38 = 38 * f9; /* 1.959375*2^30 */

	lea	edx, DWORD PTR [eax+eax*8]
	mov	DWORD PTR _f8_19$[esp+240], ecx
	lea	ecx, DWORD PTR [eax+edx*2]

; 581  :     int64_t f0f0 = f0 * (int64_t)f0;
; 582  :     int64_t f0f1_2 = f0_2 * (int64_t)f1;
; 583  :     int64_t f0f2_2 = f0_2 * (int64_t)f2;
; 584  :     int64_t f0f3_2 = f0_2 * (int64_t)f3;
; 585  :     int64_t f0f4_2 = f0_2 * (int64_t)f4;
; 586  :     int64_t f0f5_2 = f0_2 * (int64_t)f5;
; 587  :     int64_t f0f6_2 = f0_2 * (int64_t)f6;
; 588  :     int64_t f0f7_2 = f0_2 * (int64_t)f7;
; 589  :     int64_t f0f8_2 = f0_2 * (int64_t)f8;
; 590  :     int64_t f0f9_2 = f0_2 * (int64_t)f9;
; 591  :     int64_t f1f1_2 = f1_2 * (int64_t)f1;
; 592  :     int64_t f1f2_2 = f1_2 * (int64_t)f2;
; 593  :     int64_t f1f3_4 = f1_2 * (int64_t)f3_2;
; 594  :     int64_t f1f4_2 = f1_2 * (int64_t)f4;
; 595  :     int64_t f1f5_4 = f1_2 * (int64_t)f5_2;
; 596  :     int64_t f1f6_2 = f1_2 * (int64_t)f6;
; 597  :     int64_t f1f7_4 = f1_2 * (int64_t)f7_2;
; 598  :     int64_t f1f8_2 = f1_2 * (int64_t)f8;
; 599  :     int64_t f1f9_76 = f1_2 * (int64_t)f9_38;
; 600  :     int64_t f2f2 = f2 * (int64_t)f2;
; 601  :     int64_t f2f3_2 = f2_2 * (int64_t)f3;
; 602  :     int64_t f2f4_2 = f2_2 * (int64_t)f4;
; 603  :     int64_t f2f5_2 = f2_2 * (int64_t)f5;
; 604  :     int64_t f2f6_2 = f2_2 * (int64_t)f6;
; 605  :     int64_t f2f7_2 = f2_2 * (int64_t)f7;
; 606  :     int64_t f2f8_38 = f2_2 * (int64_t)f8_19;
; 607  :     int64_t f2f9_38 = f2 * (int64_t)f9_38;
; 608  :     int64_t f3f3_2 = f3_2 * (int64_t)f3;
; 609  :     int64_t f3f4_2 = f3_2 * (int64_t)f4;
; 610  :     int64_t f3f5_4 = f3_2 * (int64_t)f5_2;
; 611  :     int64_t f3f6_2 = f3_2 * (int64_t)f6;
; 612  :     int64_t f3f7_76 = f3_2 * (int64_t)f7_38;
; 613  :     int64_t f3f8_38 = f3_2 * (int64_t)f8_19;
; 614  :     int64_t f3f9_76 = f3_2 * (int64_t)f9_38;
; 615  :     int64_t f4f4 = f4 * (int64_t)f4;
; 616  :     int64_t f4f5_2 = f4_2 * (int64_t)f5;
; 617  :     int64_t f4f6_38 = f4_2 * (int64_t)f6_19;
; 618  :     int64_t f4f7_38 = f4 * (int64_t)f7_38;
; 619  :     int64_t f4f8_38 = f4_2 * (int64_t)f8_19;
; 620  :     int64_t f4f9_38 = f4 * (int64_t)f9_38;
; 621  :     int64_t f5f5_38 = f5 * (int64_t)f5_38;
; 622  :     int64_t f5f6_38 = f5_2 * (int64_t)f6_19;
; 623  :     int64_t f5f7_76 = f5_2 * (int64_t)f7_38;
; 624  :     int64_t f5f8_38 = f5_2 * (int64_t)f8_19;
; 625  :     int64_t f5f9_76 = f5_2 * (int64_t)f9_38;
; 626  :     int64_t f6f6_19 = f6 * (int64_t)f6_19;
; 627  :     int64_t f6f7_38 = f6 * (int64_t)f7_38;
; 628  :     int64_t f6f8_38 = f6_2 * (int64_t)f8_19;
; 629  :     int64_t f6f9_38 = f6 * (int64_t)f9_38;
; 630  :     int64_t f7f7_38 = f7 * (int64_t)f7_38;
; 631  :     int64_t f7f8_38 = f7_2 * (int64_t)f8_19;
; 632  :     int64_t f7f9_76 = f7_2 * (int64_t)f9_38;
; 633  :     int64_t f8f8_19 = f8 * (int64_t)f8_19;
; 634  :     int64_t f8f9_38 = f8 * (int64_t)f9_38;
; 635  :     int64_t f9f9_38 = f9 * (int64_t)f9_38;
; 636  :     int64_t h0 = f0f0 + f1f9_76 + f2f8_38 + f3f7_76 + f4f6_38 + f5f5_38;
; 637  :     int64_t h1 = f0f1_2 + f2f9_38 + f3f8_38 + f4f7_38 + f5f6_38;
; 638  :     int64_t h2 = f0f2_2 + f1f1_2 + f3f9_76 + f4f8_38 + f5f7_76 + f6f6_19;
; 639  :     int64_t h3 = f0f3_2 + f1f2_2 + f4f9_38 + f5f8_38 + f6f7_38;
; 640  :     int64_t h4 = f0f4_2 + f1f3_4 + f2f2 + f5f9_76 + f6f8_38 + f7f7_38;
; 641  :     int64_t h5 = f0f5_2 + f1f4_2 + f2f3_2 + f6f9_38 + f7f8_38;
; 642  :     int64_t h6 = f0f6_2 + f1f5_4 + f2f4_2 + f3f3_2 + f7f9_76 + f8f8_19;
; 643  :     int64_t h7 = f0f7_2 + f1f6_2 + f2f5_2 + f3f4_2 + f8f9_38;
; 644  :     int64_t h8 = f0f8_2 + f1f7_4 + f2f6_2 + f3f5_4 + f4f4 + f9f9_38;
; 645  :     int64_t h9 = f0f9_2 + f1f8_2 + f2f7_2 + f3f6_2 + f4f5_2;
; 646  :     int64_t carry0;
; 647  :     int64_t carry1;
; 648  :     int64_t carry2;
; 649  :     int64_t carry3;
; 650  :     int64_t carry4;
; 651  :     int64_t carry5;
; 652  :     int64_t carry6;
; 653  :     int64_t carry7;
; 654  :     int64_t carry8;
; 655  :     int64_t carry9;
; 656  : 
; 657  :     carry0 = h0 + (1 << 25);
; 658  :     h1 += carry0 >> 26;
; 659  :     h0 -= carry0 & kTop38Bits;
; 660  :     carry4 = h4 + (1 << 25);
; 661  :     h5 += carry4 >> 26;
; 662  :     h4 -= carry4 & kTop38Bits;
; 663  : 
; 664  :     carry1 = h1 + (1 << 24);
; 665  :     h2 += carry1 >> 25;
; 666  :     h1 -= carry1 & kTop39Bits;
; 667  :     carry5 = h5 + (1 << 24);
; 668  :     h6 += carry5 >> 25;
; 669  :     h5 -= carry5 & kTop39Bits;
; 670  : 
; 671  :     carry2 = h2 + (1 << 25);
; 672  :     h3 += carry2 >> 26;
; 673  :     h2 -= carry2 & kTop38Bits;
; 674  :     carry6 = h6 + (1 << 25);
; 675  :     h7 += carry6 >> 26;
; 676  :     h6 -= carry6 & kTop38Bits;
; 677  : 
; 678  :     carry3 = h3 + (1 << 24);
; 679  :     h4 += carry3 >> 25;
; 680  :     h3 -= carry3 & kTop39Bits;
; 681  :     carry7 = h7 + (1 << 24);
; 682  :     h8 += carry7 >> 25;
; 683  :     h7 -= carry7 & kTop39Bits;
; 684  : 
; 685  :     carry4 = h4 + (1 << 25);
; 686  :     h5 += carry4 >> 26;
; 687  :     h4 -= carry4 & kTop38Bits;
; 688  :     carry8 = h8 + (1 << 25);
; 689  :     h9 += carry8 >> 26;
; 690  :     h8 -= carry8 & kTop38Bits;
; 691  : 
; 692  :     carry9 = h9 + (1 << 24);
; 693  :     h0 += (carry9 >> 25) * 19;
; 694  :     h9 -= carry9 & kTop39Bits;
; 695  : 
; 696  :     carry0 = h0 + (1 << 25);
; 697  :     h1 += carry0 >> 26;
; 698  :     h0 -= carry0 & kTop38Bits;
; 699  : 
; 700  :     h[0] = (int32_t)h0;
; 701  :     h[1] = (int32_t)h1;
; 702  :     h[2] = (int32_t)h2;
; 703  :     h[3] = (int32_t)h3;
; 704  :     h[4] = (int32_t)h4;
; 705  :     h[5] = (int32_t)h5;
; 706  :     h[6] = (int32_t)h6;
; 707  :     h[7] = (int32_t)h7;
; 708  :     h[8] = (int32_t)h8;
; 709  :     h[9] = (int32_t)h9;

	mov	eax, esi
	cdq
	mov	DWORD PTR -40+[esp+240], eax
	mov	eax, DWORD PTR _f6_19$[esp+240]
	mov	DWORD PTR -40+[esp+244], edx
	cdq
	mov	DWORD PTR -96+[esp+240], eax
	mov	eax, DWORD PTR _f4_2$[esp+240]
	mov	DWORD PTR -96+[esp+244], edx
	cdq
	shl	edi, 1
	mov	DWORD PTR -88+[esp+240], eax
	mov	eax, edi
	mov	DWORD PTR -88+[esp+244], edx
	cdq
	mov	DWORD PTR -152+[esp+240], eax
	mov	eax, DWORD PTR _f3_2$[esp+240]
	mov	DWORD PTR -152+[esp+244], edx
	cdq
	mov	DWORD PTR -144+[esp+240], eax
	mov	eax, DWORD PTR _f8_19$[esp+240]
	mov	DWORD PTR -144+[esp+244], edx
	cdq
	mov	DWORD PTR -184+[esp+240], eax
	mov	eax, DWORD PTR _f2_2$[esp+240]
	mov	DWORD PTR -184+[esp+244], edx
	cdq
	shl	ecx, 1
	mov	DWORD PTR -104+[esp+240], eax
	mov	DWORD PTR -104+[esp+244], edx
	mov	eax, ecx
	cdq
	mov	DWORD PTR -168+[esp+240], eax
	mov	eax, DWORD PTR _f1_2$[esp+240]
	mov	DWORD PTR -168+[esp+244], edx
	mov	ecx, DWORD PTR -40+[esp+244]
	cdq
	mov	DWORD PTR -176+[esp+240], eax
	mov	eax, DWORD PTR _f0$[esp+240]
	mov	DWORD PTR -176+[esp+244], edx
	push	ecx
	cdq
	mov	DWORD PTR -48+[esp+244], eax
	mov	ecx, DWORD PTR -40+[esp+244]
	lea	eax, DWORD PTR [esi+esi*8]
	mov	DWORD PTR -48+[esp+248], edx
	push	ecx
	lea	eax, DWORD PTR [esi+eax*2]
	shl	eax, 1
	cdq
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -48+[esp+244]
	mov	esi, eax
	mov	eax, DWORD PTR -48+[esp+240]
	push	ecx
	push	eax
	push	ecx
	push	eax
	mov	edi, edx
	call	__allmul
	mov	ecx, DWORD PTR -176+[esp+244]
	add	esi, eax
	mov	eax, DWORD PTR -168+[esp+240]
	adc	edi, edx
	mov	edx, DWORD PTR -168+[esp+244]
	push	edx
	mov	edx, DWORD PTR -176+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -184+[esp+240]
	add	esi, eax
	mov	eax, DWORD PTR -184+[esp+244]
	adc	edi, edx
	mov	edx, DWORD PTR -104+[esp+244]
	push	eax
	mov	eax, DWORD PTR -104+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -152+[esp+244]
	add	esi, eax
	mov	eax, DWORD PTR -144+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -144+[esp+244]
	adc	edi, edx
	mov	edx, DWORD PTR -152+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -88+[esp+244]
	add	esi, eax
	mov	eax, DWORD PTR -96+[esp+240]
	adc	edi, edx
	mov	edx, DWORD PTR -96+[esp+244]
	push	edx
	mov	edx, DWORD PTR -88+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	add	esi, eax
	mov	eax, DWORD PTR _f5_2$[esp+240]
	adc	edi, edx
	cdq
	mov	DWORD PTR -112+[esp+240], eax
	mov	DWORD PTR -112+[esp+244], edx
	mov	eax, ebp
	mov	ecx, DWORD PTR -184+[esp+244]
	cdq
	mov	DWORD PTR -8+[esp+240], eax
	mov	eax, DWORD PTR _f0_2$[esp+240]
	mov	DWORD PTR -8+[esp+244], edx
	push	ecx
	cdq
	mov	DWORD PTR -160+[esp+244], eax
	mov	eax, DWORD PTR _f4$[esp+244]
	mov	DWORD PTR -160+[esp+248], edx
	mov	ecx, DWORD PTR -184+[esp+244]
	cdq
	mov	DWORD PTR -120+[esp+244], eax
	mov	eax, DWORD PTR _f2$[esp+244]
	mov	DWORD PTR -120+[esp+248], edx
	push	ecx
	cdq
	mov	DWORD PTR -72+[esp+248], eax
	lea	eax, DWORD PTR [ebx+ebx]
	mov	DWORD PTR -72+[esp+252], edx
	cdq
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -72+[esp+244]
	mov	ebx, eax
	mov	eax, DWORD PTR -72+[esp+240]
	push	ecx
	push	eax
	push	ecx
	push	eax
	mov	ebp, edx
	call	__allmul
	mov	ecx, DWORD PTR -120+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -160+[esp+240]
	adc	ebp, edx
	mov	edx, DWORD PTR -160+[esp+244]
	push	edx
	mov	edx, DWORD PTR -120+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -168+[esp+240]
	add	ebx, eax
	mov	eax, DWORD PTR -168+[esp+244]
	adc	ebp, edx
	mov	edx, DWORD PTR -112+[esp+244]
	push	eax
	mov	eax, DWORD PTR -112+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -152+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -8+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -8+[esp+244]
	adc	ebp, edx
	mov	edx, DWORD PTR -152+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -176+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -144+[esp+240]
	adc	ebp, edx
	mov	edx, DWORD PTR -144+[esp+244]
	push	edx
	mov	edx, DWORD PTR -176+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	add	ebx, eax
	mov	eax, esi
	adc	ebp, edx
	mov	ecx, DWORD PTR -160+[esp+244]
	add	eax, 33554432				; 02000000H
	mov	DWORD PTR _carry0$[esp+240], eax
	mov	eax, edi
	adc	eax, 0
	push	ecx
	mov	ecx, DWORD PTR -160+[esp+244]
	mov	DWORD PTR _carry0$[esp+248], eax
	mov	eax, DWORD PTR _f1$[esp+244]
	push	ecx
	cdq
	push	edx
	push	eax
	mov	DWORD PTR -136+[esp+256], eax
	mov	DWORD PTR -136+[esp+260], edx
	call	__allmul
	mov	ecx, DWORD PTR -72+[esp+244]
	mov	DWORD PTR -200+[esp+244], edx
	mov	edx, DWORD PTR -168+[esp+244]
	mov	DWORD PTR -200+[esp+240], eax
	mov	eax, DWORD PTR -168+[esp+240]
	push	edx
	mov	edx, DWORD PTR -72+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -200+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -200+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -120+[esp+244]
	mov	DWORD PTR -200+[esp+244], eax
	mov	eax, DWORD PTR -152+[esp+244]
	mov	DWORD PTR -200+[esp+240], ecx
	mov	ecx, DWORD PTR -152+[esp+240]
	push	eax
	mov	eax, DWORD PTR -120+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -200+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -200+[esp+244]
	mov	DWORD PTR -200+[esp+240], ecx
	mov	ecx, DWORD PTR -96+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -96+[esp+240]
	mov	DWORD PTR -200+[esp+244], eax
	mov	eax, DWORD PTR -112+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -112+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -200+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -144+[esp+240]
	mov	DWORD PTR -200+[esp+240], ecx
	mov	ecx, DWORD PTR -200+[esp+244]
	adc	ecx, edx
	mov	edx, DWORD PTR -144+[esp+244]
	mov	DWORD PTR -200+[esp+244], ecx
	mov	ecx, DWORD PTR -184+[esp+244]
	push	edx
	mov	edx, DWORD PTR -184+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -200+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -200+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR _carry0$[esp+244]
	mov	DWORD PTR -200+[esp+240], ecx
	mov	DWORD PTR -200+[esp+244], eax
	mov	eax, DWORD PTR _carry0$[esp+240]
	mov	ecx, 26					; 0000001aH
	call	__allshr
	mov	ecx, DWORD PTR -200+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -200+[esp+244]
	adc	eax, edx
	mov	DWORD PTR _h1$[esp+240], ecx
	mov	DWORD PTR _h1$[esp+244], eax
	mov	eax, ebx
	add	eax, 33554432				; 02000000H
	mov	ecx, DWORD PTR -104+[esp+244]
	mov	DWORD PTR _carry4$[esp+240], eax
	mov	eax, ebp
	adc	eax, 0
	push	ecx
	mov	DWORD PTR _carry4$[esp+248], eax
	mov	eax, DWORD PTR _f7_2$[esp+244]
	cdq
	mov	DWORD PTR -56+[esp+244], eax
	mov	eax, DWORD PTR _f6$[esp+244]
	mov	ecx, DWORD PTR -104+[esp+244]
	mov	DWORD PTR -56+[esp+248], edx
	cdq
	mov	DWORD PTR -128+[esp+244], eax
	mov	eax, DWORD PTR _f3$[esp+244]
	mov	DWORD PTR -128+[esp+248], edx
	push	ecx
	cdq
	push	edx
	push	eax
	mov	DWORD PTR -80+[esp+256], eax
	mov	DWORD PTR -80+[esp+260], edx
	call	__allmul
	mov	ecx, DWORD PTR -128+[esp+244]
	mov	DWORD PTR -216+[esp+244], edx
	mov	edx, DWORD PTR -168+[esp+244]
	mov	DWORD PTR -216+[esp+240], eax
	mov	eax, DWORD PTR -168+[esp+240]
	push	edx
	mov	edx, DWORD PTR -128+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -216+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -216+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -56+[esp+244]
	mov	DWORD PTR -216+[esp+244], eax
	mov	eax, DWORD PTR -184+[esp+244]
	mov	DWORD PTR -216+[esp+240], ecx
	mov	ecx, DWORD PTR -184+[esp+240]
	push	eax
	mov	eax, DWORD PTR -56+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -216+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -216+[esp+244]
	mov	DWORD PTR -216+[esp+240], ecx
	mov	ecx, DWORD PTR -176+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -176+[esp+240]
	mov	DWORD PTR -216+[esp+244], eax
	mov	eax, DWORD PTR -120+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -120+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -216+[esp+240]
	add	ecx, eax
	mov	DWORD PTR -216+[esp+240], ecx
	mov	ecx, DWORD PTR -216+[esp+244]
	adc	ecx, edx
	mov	edx, DWORD PTR -40+[esp+244]
	mov	DWORD PTR -216+[esp+244], ecx
	push	edx
	mov	eax, DWORD PTR -40+[esp+244]
	mov	ecx, DWORD PTR -160+[esp+248]
	mov	edx, DWORD PTR -160+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -216+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -216+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR _carry4$[esp+244]
	mov	DWORD PTR -216+[esp+240], ecx
	mov	DWORD PTR -216+[esp+244], eax
	mov	eax, DWORD PTR _carry4$[esp+240]
	mov	ecx, 26					; 0000001aH
	call	__allshr
	mov	ecx, DWORD PTR -216+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -216+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -160+[esp+240]
	mov	DWORD PTR _h5$[esp+244], eax
	mov	eax, DWORD PTR _h1$[esp+240]
	add	eax, 16777216				; 01000000H
	mov	DWORD PTR _h5$[esp+240], ecx
	mov	ecx, DWORD PTR -160+[esp+244]
	mov	DWORD PTR _carry1$[esp+240], eax
	mov	eax, DWORD PTR _h1$[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -72+[esp+244]
	push	edx
	adc	eax, 0
	mov	DWORD PTR _carry1$[esp+252], eax
	mov	eax, DWORD PTR -72+[esp+252]
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -136+[esp+244]
	mov	DWORD PTR -216+[esp+244], edx
	mov	edx, DWORD PTR -176+[esp+244]
	mov	DWORD PTR -216+[esp+240], eax
	mov	eax, DWORD PTR -176+[esp+240]
	push	edx
	mov	edx, DWORD PTR -136+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -216+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -216+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -128+[esp+244]
	mov	DWORD PTR -216+[esp+244], eax
	mov	eax, DWORD PTR -96+[esp+244]
	mov	DWORD PTR -216+[esp+240], ecx
	mov	ecx, DWORD PTR -96+[esp+240]
	push	eax
	mov	eax, DWORD PTR -128+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -216+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -216+[esp+244]
	mov	DWORD PTR -216+[esp+240], ecx
	mov	ecx, DWORD PTR -152+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -152+[esp+240]
	mov	DWORD PTR -216+[esp+244], eax
	mov	eax, DWORD PTR -112+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -112+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -216+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -144+[esp+240]
	mov	DWORD PTR -216+[esp+240], ecx
	mov	ecx, DWORD PTR -216+[esp+244]
	adc	ecx, edx
	mov	edx, DWORD PTR -144+[esp+244]
	mov	DWORD PTR -216+[esp+244], ecx
	mov	ecx, DWORD PTR -168+[esp+244]
	push	edx
	mov	edx, DWORD PTR -168+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -216+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -216+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -184+[esp+244]
	mov	DWORD PTR -216+[esp+244], eax
	mov	eax, DWORD PTR -88+[esp+244]
	mov	DWORD PTR -216+[esp+240], ecx
	mov	ecx, DWORD PTR -88+[esp+240]
	push	eax
	mov	eax, DWORD PTR -184+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -216+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -216+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR _carry1$[esp+244]
	mov	DWORD PTR -216+[esp+240], ecx
	mov	DWORD PTR -216+[esp+244], eax
	mov	eax, DWORD PTR _carry1$[esp+240]
	mov	ecx, 25					; 00000019H
	call	__allshr
	mov	ecx, DWORD PTR -216+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -216+[esp+244]
	adc	eax, edx
	mov	DWORD PTR _h2$[esp+240], ecx
	mov	DWORD PTR _h2$[esp+244], eax
	mov	eax, DWORD PTR _h5$[esp+240]
	add	eax, 16777216				; 01000000H
	mov	ecx, DWORD PTR -160+[esp+244]
	mov	DWORD PTR _carry5$[esp+240], eax
	mov	eax, DWORD PTR _h5$[esp+244]
	adc	eax, 0
	push	ecx
	mov	DWORD PTR _carry5$[esp+248], eax
	mov	eax, DWORD PTR _f8$[esp+244]
	mov	ecx, DWORD PTR -128+[esp+244]
	cdq
	mov	DWORD PTR -216+[esp+248], edx
	mov	edx, DWORD PTR -160+[esp+244]
	mov	DWORD PTR -216+[esp+244], eax
	mov	eax, DWORD PTR -128+[esp+248]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -216+[esp+244]
	mov	DWORD PTR -224+[esp+244], edx
	mov	edx, DWORD PTR -184+[esp+244]
	mov	DWORD PTR -224+[esp+240], eax
	mov	eax, DWORD PTR -184+[esp+240]
	push	edx
	mov	edx, DWORD PTR -216+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -224+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -224+[esp+244]
	adc	eax, edx
	mov	DWORD PTR -224+[esp+240], ecx
	mov	DWORD PTR -224+[esp+244], eax
	mov	eax, DWORD PTR -168+[esp+244]
	mov	ecx, DWORD PTR -168+[esp+240]
	mov	edx, DWORD PTR -56+[esp+244]
	push	eax
	mov	eax, DWORD PTR -56+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -224+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -224+[esp+244]
	mov	DWORD PTR -224+[esp+240], ecx
	mov	ecx, DWORD PTR -144+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -144+[esp+240]
	mov	DWORD PTR -224+[esp+244], eax
	mov	eax, DWORD PTR -80+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -80+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -224+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -104+[esp+240]
	mov	DWORD PTR -224+[esp+240], ecx
	mov	ecx, DWORD PTR -224+[esp+244]
	adc	ecx, edx
	mov	edx, DWORD PTR -104+[esp+244]
	mov	DWORD PTR -224+[esp+244], ecx
	mov	ecx, DWORD PTR -120+[esp+244]
	push	edx
	mov	edx, DWORD PTR -120+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -224+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -224+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -112+[esp+244]
	mov	DWORD PTR -224+[esp+244], eax
	mov	eax, DWORD PTR -176+[esp+244]
	mov	DWORD PTR -224+[esp+240], ecx
	mov	ecx, DWORD PTR -176+[esp+240]
	push	eax
	mov	eax, DWORD PTR -112+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -224+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -224+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR _carry5$[esp+244]
	mov	DWORD PTR -224+[esp+240], ecx
	mov	DWORD PTR -224+[esp+244], eax
	mov	eax, DWORD PTR _carry5$[esp+240]
	mov	ecx, 25					; 00000019H
	call	__allshr
	mov	ecx, DWORD PTR -224+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -224+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -160+[esp+240]
	mov	DWORD PTR _h6$[esp+244], eax
	mov	eax, DWORD PTR _h2$[esp+240]
	add	eax, 33554432				; 02000000H
	mov	DWORD PTR _h6$[esp+240], ecx
	mov	ecx, DWORD PTR -160+[esp+244]
	mov	DWORD PTR _carry2$[esp+240], eax
	mov	eax, DWORD PTR _h2$[esp+244]
	push	ecx
	adc	eax, 0
	push	edx
	mov	DWORD PTR _carry2$[esp+252], eax
	mov	eax, DWORD PTR -80+[esp+252]
	push	eax
	mov	ecx, DWORD PTR -80+[esp+252]
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -128+[esp+244]
	mov	DWORD PTR -224+[esp+244], edx
	mov	edx, DWORD PTR -152+[esp+244]
	mov	DWORD PTR -224+[esp+240], eax
	mov	eax, DWORD PTR -152+[esp+240]
	push	edx
	mov	edx, DWORD PTR -128+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -224+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -224+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -72+[esp+244]
	mov	DWORD PTR -224+[esp+244], eax
	mov	eax, DWORD PTR -176+[esp+244]
	mov	DWORD PTR -224+[esp+240], ecx
	mov	ecx, DWORD PTR -176+[esp+240]
	push	eax
	mov	eax, DWORD PTR -72+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -224+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -224+[esp+244]
	mov	DWORD PTR -224+[esp+240], ecx
	mov	ecx, DWORD PTR -168+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -168+[esp+240]
	mov	DWORD PTR -224+[esp+244], eax
	mov	eax, DWORD PTR -120+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -120+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -224+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -184+[esp+240]
	mov	DWORD PTR -224+[esp+240], ecx
	mov	ecx, DWORD PTR -224+[esp+244]
	adc	ecx, edx
	mov	edx, DWORD PTR -184+[esp+244]
	mov	DWORD PTR -224+[esp+244], ecx
	mov	ecx, DWORD PTR -112+[esp+244]
	push	edx
	mov	edx, DWORD PTR -112+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -224+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -224+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR _carry2$[esp+244]
	mov	DWORD PTR -224+[esp+240], ecx
	mov	DWORD PTR -224+[esp+244], eax
	mov	eax, DWORD PTR _carry2$[esp+240]
	mov	ecx, 26					; 0000001aH
	call	__allshr
	mov	ecx, DWORD PTR -224+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -224+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR _kTop38Bits+4
	mov	DWORD PTR _h3$[esp+240], ecx
	mov	ecx, DWORD PTR _kTop38Bits
	mov	DWORD PTR _h3$[esp+244], eax
	mov	eax, DWORD PTR _carry2$[esp+240]
	and	eax, ecx
	mov	ecx, DWORD PTR _carry2$[esp+244]
	and	ecx, edx
	mov	edx, DWORD PTR _h2$[esp+240]
	sub	edx, eax
	mov	eax, DWORD PTR _h2$[esp+244]
	sbb	eax, ecx
	mov	ecx, DWORD PTR -168+[esp+244]
	mov	eax, DWORD PTR _h6$[esp+240]
	mov	DWORD PTR _h2$[esp+240], edx
	mov	edx, DWORD PTR -168+[esp+240]
	add	eax, 33554432				; 02000000H
	mov	DWORD PTR _carry6$[esp+240], eax
	mov	eax, DWORD PTR _h6$[esp+244]
	adc	eax, 0
	push	ecx
	mov	ecx, DWORD PTR -216+[esp+244]
	mov	DWORD PTR _carry6$[esp+248], eax
	mov	eax, DWORD PTR -216+[esp+248]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -160+[esp+244]
	mov	DWORD PTR -224+[esp+244], edx
	mov	edx, DWORD PTR -8+[esp+244]
	mov	DWORD PTR -224+[esp+240], eax
	mov	eax, DWORD PTR -8+[esp+240]
	push	edx
	mov	edx, DWORD PTR -160+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -224+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -224+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -128+[esp+244]
	mov	DWORD PTR -224+[esp+244], eax
	mov	eax, DWORD PTR -176+[esp+244]
	mov	DWORD PTR -224+[esp+240], ecx
	mov	ecx, DWORD PTR -176+[esp+240]
	push	eax
	mov	eax, DWORD PTR -128+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -224+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -224+[esp+244]
	mov	DWORD PTR -224+[esp+240], ecx
	mov	ecx, DWORD PTR -144+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -144+[esp+240]
	mov	DWORD PTR -224+[esp+244], eax
	mov	eax, DWORD PTR -120+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -120+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -224+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -40+[esp+240]
	mov	DWORD PTR -224+[esp+240], ecx
	mov	ecx, DWORD PTR -224+[esp+244]
	adc	ecx, edx
	mov	edx, DWORD PTR -40+[esp+244]
	mov	DWORD PTR -224+[esp+244], ecx
	mov	ecx, DWORD PTR -104+[esp+244]
	push	edx
	mov	edx, DWORD PTR -104+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -224+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -224+[esp+244]
	mov	DWORD PTR -224+[esp+240], ecx
	adc	eax, edx
	mov	edx, DWORD PTR _carry6$[esp+244]
	mov	DWORD PTR -224+[esp+244], eax
	mov	eax, DWORD PTR _carry6$[esp+240]
	mov	ecx, 26					; 0000001aH
	call	__allshr
	mov	ecx, DWORD PTR -224+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -224+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR _kTop38Bits+4
	mov	DWORD PTR _h7$[esp+240], ecx
	mov	ecx, DWORD PTR _kTop38Bits
	mov	DWORD PTR _h7$[esp+244], eax
	mov	eax, DWORD PTR _carry6$[esp+240]
	and	eax, ecx
	mov	ecx, DWORD PTR _carry6$[esp+244]
	and	ecx, edx
	mov	edx, DWORD PTR _h6$[esp+240]
	sub	edx, eax
	mov	eax, DWORD PTR _h6$[esp+244]
	sbb	eax, ecx
	mov	ecx, DWORD PTR _h3$[esp+244]
	mov	eax, DWORD PTR _h3$[esp+240]
	mov	DWORD PTR _h6$[esp+240], edx
	mov	edx, DWORD PTR _kTop38Bits
	add	eax, 16777216				; 01000000H
	adc	ecx, 0
	mov	DWORD PTR _carry3$[esp+240], eax
	mov	DWORD PTR _carry3$[esp+244], ecx
	mov	ecx, DWORD PTR _carry4$[esp+240]
	and	ecx, edx
	mov	edx, DWORD PTR _carry4$[esp+244]
	mov	DWORD PTR -80+[esp+240], ecx
	mov	ecx, DWORD PTR _kTop38Bits+4
	and	edx, ecx
	mov	ecx, 25					; 00000019H
	mov	DWORD PTR -80+[esp+244], edx
	mov	edx, DWORD PTR _carry3$[esp+244]
	call	__allshr
	mov	ecx, DWORD PTR -80+[esp+240]
	sub	eax, ecx
	mov	ecx, DWORD PTR -80+[esp+244]
	sbb	edx, ecx
	mov	ecx, DWORD PTR _kTop39Bits
	add	ebx, eax
	mov	eax, DWORD PTR _carry3$[esp+240]
	adc	ebp, edx
	mov	edx, DWORD PTR _kTop39Bits+4
	and	eax, ecx
	mov	ecx, DWORD PTR _carry3$[esp+244]
	and	ecx, edx
	mov	edx, DWORD PTR _h3$[esp+240]
	sub	edx, eax
	mov	eax, DWORD PTR _h3$[esp+244]
	sbb	eax, ecx
	mov	ecx, DWORD PTR -168+[esp+244]
	mov	eax, DWORD PTR _h7$[esp+240]
	mov	DWORD PTR _h3$[esp+240], edx
	add	eax, 16777216				; 01000000H
	push	ecx
	mov	DWORD PTR _carry7$[esp+244], eax
	mov	eax, DWORD PTR _h7$[esp+248]
	mov	ecx, DWORD PTR -168+[esp+244]
	adc	eax, 0
	push	ecx
	mov	DWORD PTR _carry7$[esp+252], eax
	mov	eax, DWORD PTR _f9$[esp+248]
	cdq
	push	edx
	push	eax
	mov	DWORD PTR -80+[esp+256], eax
	mov	DWORD PTR -80+[esp+260], edx
	call	__allmul
	mov	ecx, DWORD PTR -216+[esp+244]
	mov	DWORD PTR -208+[esp+244], edx
	mov	edx, DWORD PTR -160+[esp+244]
	mov	DWORD PTR -208+[esp+240], eax
	mov	eax, DWORD PTR -160+[esp+240]
	push	edx
	push	eax
	push	ecx
	mov	edx, DWORD PTR -216+[esp+252]
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -208+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -120+[esp+240]
	mov	DWORD PTR -208+[esp+240], ecx
	mov	ecx, DWORD PTR -208+[esp+244]
	adc	ecx, edx
	mov	DWORD PTR -208+[esp+244], ecx
	mov	ecx, DWORD PTR -120+[esp+244]
	push	ecx
	push	eax
	push	ecx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -208+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -208+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -56+[esp+244]
	mov	DWORD PTR -208+[esp+244], eax
	mov	eax, DWORD PTR -176+[esp+244]
	mov	DWORD PTR -208+[esp+240], ecx
	mov	ecx, DWORD PTR -176+[esp+240]
	push	eax
	mov	eax, DWORD PTR -56+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -208+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -208+[esp+244]
	mov	DWORD PTR -208+[esp+240], ecx
	mov	ecx, DWORD PTR -104+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -104+[esp+240]
	mov	DWORD PTR -208+[esp+244], eax
	mov	eax, DWORD PTR -128+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -128+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -208+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -144+[esp+240]
	mov	DWORD PTR -208+[esp+240], ecx
	mov	ecx, DWORD PTR -208+[esp+244]
	adc	ecx, edx
	mov	edx, DWORD PTR -144+[esp+244]
	mov	DWORD PTR -208+[esp+244], ecx
	mov	ecx, DWORD PTR -112+[esp+244]
	push	edx
	mov	edx, DWORD PTR -112+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -208+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -208+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR _carry7$[esp+244]
	mov	DWORD PTR -208+[esp+240], ecx
	mov	DWORD PTR -208+[esp+244], eax
	mov	eax, DWORD PTR _carry7$[esp+240]
	mov	ecx, 25					; 00000019H
	call	__allshr
	mov	ecx, DWORD PTR -208+[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR -208+[esp+244]
	adc	eax, edx
	mov	DWORD PTR _h8$[esp+240], ecx
	mov	ecx, DWORD PTR _kTop39Bits
	mov	DWORD PTR _h8$[esp+244], eax
	mov	eax, DWORD PTR _carry7$[esp+240]
	and	eax, ecx
	mov	ecx, DWORD PTR _carry7$[esp+244]
	mov	edx, DWORD PTR _kTop39Bits+4
	and	ecx, edx
	mov	edx, DWORD PTR _h7$[esp+240]
	sub	edx, eax
	mov	eax, DWORD PTR _h7$[esp+244]
	sbb	eax, ecx
	mov	ecx, ebp
	mov	eax, ebx
	mov	DWORD PTR _h7$[esp+240], edx
	mov	edx, DWORD PTR _kTop39Bits
	add	eax, 33554432				; 02000000H
	adc	ecx, 0
	mov	DWORD PTR _carry4$[esp+240], eax
	mov	DWORD PTR _carry4$[esp+244], ecx
	mov	ecx, DWORD PTR _carry5$[esp+240]
	and	ecx, edx
	mov	edx, DWORD PTR _carry5$[esp+244]
	mov	DWORD PTR -192+[esp+240], ecx
	mov	ecx, DWORD PTR _kTop39Bits+4
	and	edx, ecx
	mov	ecx, 26					; 0000001aH
	mov	DWORD PTR -192+[esp+244], edx
	mov	edx, DWORD PTR _carry4$[esp+244]
	call	__allshr
	mov	ecx, DWORD PTR -192+[esp+240]
	sub	eax, ecx
	mov	ecx, DWORD PTR -192+[esp+244]
	sbb	edx, ecx
	mov	ecx, DWORD PTR _h5$[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR _h5$[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _carry4$[esp+240]
	mov	DWORD PTR _h5$[esp+240], ecx
	mov	ecx, DWORD PTR _kTop38Bits
	and	eax, ecx
	mov	ecx, DWORD PTR _carry4$[esp+244]
	and	ecx, edx
	sub	ebx, eax
	mov	eax, DWORD PTR _h8$[esp+240]
	mov	edx, DWORD PTR -80+[esp+244]
	sbb	ebp, ecx
	add	eax, 33554432				; 02000000H
	mov	ecx, DWORD PTR -160+[esp+240]
	mov	DWORD PTR _carry8$[esp+240], eax
	mov	eax, DWORD PTR _h8$[esp+244]
	adc	eax, 0
	mov	DWORD PTR _carry8$[esp+244], eax
	mov	eax, DWORD PTR -160+[esp+244]
	push	eax
	mov	eax, DWORD PTR -80+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -176+[esp+244]
	mov	DWORD PTR -208+[esp+244], edx
	mov	edx, DWORD PTR -176+[esp+240]
	mov	ebp, eax
	mov	eax, DWORD PTR -216+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -216+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -208+[esp+244]
	add	ebp, eax
	mov	eax, DWORD PTR -144+[esp+240]
	adc	ecx, edx
	mov	edx, DWORD PTR -144+[esp+244]
	mov	DWORD PTR -208+[esp+244], ecx
	mov	ecx, DWORD PTR -128+[esp+244]
	push	edx
	mov	edx, DWORD PTR -128+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	add	ebp, eax
	mov	eax, DWORD PTR -208+[esp+244]
	mov	ecx, DWORD PTR -104+[esp+240]
	adc	eax, edx
	mov	edx, DWORD PTR -8+[esp+244]
	mov	DWORD PTR -208+[esp+244], eax
	mov	eax, DWORD PTR -104+[esp+244]
	push	eax
	mov	eax, DWORD PTR -8+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -40+[esp+244]
	add	ebp, eax
	mov	eax, DWORD PTR -208+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -88+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR -40+[esp+244]
	mov	DWORD PTR -208+[esp+248], eax
	mov	eax, DWORD PTR -88+[esp+248]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	add	ebp, eax
	mov	eax, DWORD PTR -208+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR _carry8$[esp+244]
	mov	DWORD PTR -208+[esp+244], eax
	mov	eax, DWORD PTR _carry8$[esp+240]
	mov	ecx, 26					; 0000001aH
	call	__allshr
	add	ebp, eax
	mov	eax, DWORD PTR -208+[esp+244]
	mov	ecx, DWORD PTR _kTop38Bits+4
	mov	DWORD PTR _h9$[esp+240], ebp
	adc	eax, edx
	mov	edx, DWORD PTR _kTop38Bits
	mov	DWORD PTR _h9$[esp+244], eax
	mov	eax, DWORD PTR _carry8$[esp+240]
	and	eax, edx
	mov	edx, DWORD PTR _carry8$[esp+244]
	and	edx, ecx
	mov	ecx, DWORD PTR _h8$[esp+240]
	sub	ecx, eax
	mov	eax, DWORD PTR _h8$[esp+244]
	sbb	eax, edx
	mov	edx, DWORD PTR _h9$[esp+244]
	add	ebp, 16777216				; 01000000H
	mov	DWORD PTR _h8$[esp+240], ecx
	adc	edx, 0
	mov	ecx, 25					; 00000019H
	mov	eax, ebp
	mov	DWORD PTR _carry9$[esp+240], ebp
	mov	DWORD PTR _carry9$[esp+244], edx
	call	__allshr
	push	0
	push	19					; 00000013H
	push	edx
	push	eax
	call	__allmul
	mov	ebp, DWORD PTR _kTop38Bits
	mov	ecx, eax
	mov	eax, DWORD PTR _carry0$[esp+240]
	and	eax, ebp
	mov	ebp, DWORD PTR _kTop38Bits+4
	mov	DWORD PTR -192+[esp+240], eax
	mov	eax, DWORD PTR _carry0$[esp+244]
	and	eax, ebp
	mov	ebp, DWORD PTR -192+[esp+240]
	sub	ecx, ebp
	mov	ebp, DWORD PTR _kTop39Bits+4
	sbb	edx, eax
	mov	eax, DWORD PTR _carry9$[esp+240]
	add	esi, ecx
	mov	ecx, DWORD PTR _carry9$[esp+244]
	adc	edi, edx
	mov	edx, DWORD PTR _kTop39Bits
	and	eax, edx
	and	ecx, ebp
	mov	ebp, DWORD PTR _h9$[esp+240]
	sub	ebp, eax
	mov	eax, DWORD PTR _h9$[esp+244]
	mov	DWORD PTR _h9$[esp+240], ebp
	mov	ebp, esi
	sbb	eax, ecx
	add	ebp, 33554432				; 02000000H
	mov	ecx, DWORD PTR _carry1$[esp+244]
	mov	eax, ebp
	adc	edi, 0
	mov	DWORD PTR _carry0$[esp+244], edi
	mov	edi, DWORD PTR _carry1$[esp+240]
	and	edi, edx
	mov	edx, DWORD PTR _kTop39Bits+4
	and	ecx, edx
	mov	edx, DWORD PTR _carry0$[esp+244]
	mov	DWORD PTR -24+[esp+244], ecx
	mov	ecx, 26					; 0000001aH
	call	__allshr
	mov	ecx, DWORD PTR -24+[esp+244]
	sub	eax, edi
	sbb	edx, ecx
	mov	ecx, DWORD PTR _h1$[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR _h1$[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR _kTop38Bits
	mov	eax, DWORD PTR _h$[esp+236]
	and	ebp, edx
	mov	edx, DWORD PTR _h2$[esp+240]
	sub	esi, ebp
	mov	DWORD PTR [eax+4], ecx
	mov	ecx, DWORD PTR _h3$[esp+240]
	mov	DWORD PTR [eax+8], edx
	mov	edx, DWORD PTR _h5$[esp+240]
	mov	DWORD PTR [eax+12], ecx
	mov	ecx, DWORD PTR _h6$[esp+240]
	mov	DWORD PTR [eax+20], edx
	mov	edx, DWORD PTR _h7$[esp+240]
	mov	DWORD PTR [eax], esi
	mov	DWORD PTR [eax+24], ecx
	mov	ecx, DWORD PTR _h8$[esp+240]
	mov	DWORD PTR [eax+28], edx
	mov	edx, DWORD PTR _h9$[esp+240]
	pop	edi
	pop	esi
	mov	DWORD PTR [eax+16], ebx
	pop	ebp
	mov	DWORD PTR [eax+32], ecx
	mov	DWORD PTR [eax+36], edx
	pop	ebx

; 710  : }

	add	esp, 224				; 000000e0H
	ret	0
_fe_sq	ENDP
_TEXT	ENDS
;	COMDAT _fe_isnegative
_TEXT	SEGMENT
_f$ = 8
_s$ = -32
_fe_isnegative PROC NEAR				; COMDAT

; 829  :     uint8_t s[32];
; 830  : 
; 831  :     fe_tobytes(s, f);

	mov	eax, DWORD PTR _f$[esp-4]
	sub	esp, 32					; 00000020H
	lea	ecx, DWORD PTR _s$[esp+32]
	push	eax
	push	ecx
	call	_fe_tobytes

; 832  :     return s[0] & 1;

	mov	al, BYTE PTR _s$[esp+40]
	and	eax, 1

; 833  : }

	add	esp, 40					; 00000028H
	ret	0
_fe_isnegative ENDP
_TEXT	ENDS
;	COMDAT _ge_p3_tobytes
_TEXT	SEGMENT
_s$ = 8
_h$ = 12
_recip$ = -120
_x$ = -40
_y$ = -80
_ge_p3_tobytes PROC NEAR				; COMDAT

; 1074 : {

	sub	esp, 120				; 00000078H

; 1075 :     fe recip;
; 1076 :     fe x;
; 1077 :     fe y;
; 1078 : 
; 1079 :     fe_invert(recip, h->Z);

	lea	ecx, DWORD PTR _recip$[esp+120]
	push	esi
	mov	esi, DWORD PTR _h$[esp+120]
	lea	eax, DWORD PTR [esi+80]
	push	eax
	push	ecx
	call	_fe_invert

; 1080 :     fe_mul(x, h->X, recip);

	lea	edx, DWORD PTR _recip$[esp+132]
	lea	eax, DWORD PTR _x$[esp+132]
	push	edx
	push	esi
	push	eax
	call	_fe_mul

; 1081 :     fe_mul(y, h->Y, recip);

	lea	ecx, DWORD PTR _recip$[esp+144]
	add	esi, 40					; 00000028H
	push	ecx
	lea	edx, DWORD PTR _y$[esp+148]
	push	esi
	push	edx
	call	_fe_mul

; 1082 :     fe_tobytes(s, y);

	mov	esi, DWORD PTR _s$[esp+152]
	lea	eax, DWORD PTR _y$[esp+156]
	push	eax
	push	esi
	call	_fe_tobytes

; 1083 :     s[31] ^= fe_isnegative(x) << 7;

	lea	ecx, DWORD PTR _x$[esp+164]
	push	ecx
	call	_fe_isnegative
	mov	cl, BYTE PTR [esi+31]
	add	esp, 44					; 0000002cH
	shl	al, 7
	xor	cl, al
	mov	BYTE PTR [esi+31], cl
	pop	esi

; 1084 : }

	add	esp, 120				; 00000078H
	ret	0
_ge_p3_tobytes ENDP
_TEXT	ENDS
;	COMDAT _ge_frombytes_vartime
_TEXT	SEGMENT
_h$ = 8
_s$ = 12
_u$ = -200
_v$ = -120
_w$ = -40
_vxx$ = -160
_check$ = -80
_ge_frombytes_vartime PROC NEAR				; COMDAT

; 1097 : {

	sub	esp, 200				; 000000c8H
	push	ebx
	push	ebp

; 1098 :     fe u;
; 1099 :     fe v;
; 1100 :     fe w;
; 1101 :     fe vxx;
; 1102 :     fe check;
; 1103 : 
; 1104 :     fe_frombytes(h->Y, s);

	mov	ebp, DWORD PTR _s$[esp+204]
	push	esi
	mov	esi, DWORD PTR _h$[esp+208]
	push	edi
	push	ebp
	lea	ebx, DWORD PTR [esi+40]
	push	ebx
	call	_fe_frombytes

; 1105 :     fe_1(h->Z);

	lea	edi, DWORD PTR [esi+80]
	push	edi
	call	_fe_1

; 1106 :     fe_sq(u, h->Y);

	lea	eax, DWORD PTR _u$[esp+228]
	push	ebx
	push	eax
	call	_fe_sq

; 1107 :     fe_mul(v, u, d);

	lea	ecx, DWORD PTR _u$[esp+236]
	push	OFFSET FLAT:_d
	lea	edx, DWORD PTR _v$[esp+240]
	push	ecx
	push	edx
	call	_fe_mul

; 1108 :     fe_sub(u, u, h->Z); /* u = y^2-1 */

	lea	eax, DWORD PTR _u$[esp+248]
	push	edi
	lea	ecx, DWORD PTR _u$[esp+252]
	push	eax
	push	ecx
	call	_fe_sub

; 1109 :     fe_add(v, v, h->Z); /* v = dy^2+1 */

	lea	edx, DWORD PTR _v$[esp+260]
	push	edi
	lea	eax, DWORD PTR _v$[esp+264]
	push	edx
	push	eax
	call	_fe_add

; 1110 : 
; 1111 :     fe_mul(w, u, v); /* w = u*v */

	lea	ecx, DWORD PTR _v$[esp+272]
	lea	edx, DWORD PTR _u$[esp+272]
	push	ecx
	lea	eax, DWORD PTR _w$[esp+276]
	push	edx
	push	eax
	call	_fe_mul
	add	esp, 68					; 00000044H

; 1112 : 
; 1113 :     fe_pow22523(h->X, w); /* x = w^((q-5)/8) */

	lea	ecx, DWORD PTR _w$[esp+216]
	push	ecx
	push	esi
	call	_fe_pow22523

; 1114 :     fe_mul(h->X, h->X, u); /* x = u * w^((q-5)/8) */

	lea	edx, DWORD PTR _u$[esp+224]
	push	edx
	push	esi
	push	esi
	call	_fe_mul

; 1115 : 
; 1116 :     fe_sq(vxx, h->X);

	lea	eax, DWORD PTR _vxx$[esp+236]
	push	esi
	push	eax
	call	_fe_sq

; 1117 :     fe_mul(vxx, vxx, v);

	lea	ecx, DWORD PTR _v$[esp+244]
	lea	edx, DWORD PTR _vxx$[esp+244]
	push	ecx
	lea	eax, DWORD PTR _vxx$[esp+248]
	push	edx
	push	eax
	call	_fe_mul

; 1118 :     fe_sub(check, vxx, u); /* vx^2-u */

	lea	ecx, DWORD PTR _u$[esp+256]
	lea	edx, DWORD PTR _vxx$[esp+256]
	push	ecx
	lea	eax, DWORD PTR _check$[esp+260]
	push	edx
	push	eax
	call	_fe_sub

; 1119 :     if (fe_isnonzero(check)) {

	lea	ecx, DWORD PTR _check$[esp+268]
	push	ecx
	call	_fe_isnonzero
	add	esp, 56					; 00000038H
	test	eax, eax
	je	SHORT $L1824

; 1120 :         fe_add(check, vxx, u); /* vx^2+u */

	lea	edx, DWORD PTR _u$[esp+216]
	lea	eax, DWORD PTR _vxx$[esp+216]
	push	edx
	lea	ecx, DWORD PTR _check$[esp+220]
	push	eax
	push	ecx
	call	_fe_add

; 1121 :         if (fe_isnonzero(check)) {

	lea	edx, DWORD PTR _check$[esp+228]
	push	edx
	call	_fe_isnonzero
	add	esp, 16					; 00000010H
	test	eax, eax
	je	SHORT $L1825
	pop	edi
	pop	esi
	pop	ebp

; 1122 :             return -1;

	or	eax, -1
	pop	ebx

; 1133 : }

	add	esp, 200				; 000000c8H
	ret	0
$L1825:

; 1123 :         }
; 1124 :         fe_mul(h->X, h->X, sqrtm1);

	push	OFFSET FLAT:_sqrtm1
	push	esi
	push	esi
	call	_fe_mul
	add	esp, 12					; 0000000cH
$L1824:

; 1125 :     }
; 1126 : 
; 1127 :     if (fe_isnegative(h->X) != (s[31] >> 7)) {

	push	esi
	call	_fe_isnegative
	xor	ecx, ecx
	add	esp, 4
	mov	cl, BYTE PTR [ebp+31]
	shr	ecx, 7
	cmp	eax, ecx
	je	SHORT $L1826

; 1128 :         fe_neg(h->X, h->X);

	push	esi
	push	esi
	call	_fe_neg
	add	esp, 8
$L1826:

; 1129 :     }
; 1130 : 
; 1131 :     fe_mul(h->T, h->X, h->Y);

	push	ebx
	push	esi
	add	esi, 120				; 00000078H
	push	esi
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1132 :     return 0;

	xor	eax, eax
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 1133 : }

	add	esp, 200				; 000000c8H
	ret	0
_ge_frombytes_vartime ENDP
_TEXT	ENDS
EXTRN	__allshl:NEAR
;	COMDAT _fe_frombytes
_TEXT	SEGMENT
_h$ = 8
_s$ = 12
_h0$ = -48
_h1$ = -56
_h2$ = -112
_h3$ = -80
_h4$ = -104
_h5$ = -72
_h6$ = -96
_h7$ = -64
_h8$ = -88
_h9$ = -8
_carry2$ = -120
_carry3$ = -40
_carry4$ = -40
_carry5$ = -32
_carry6$ = -32
_carry7$ = -24
_carry8$ = -24
_carry9$ = -16
_fe_frombytes PROC NEAR					; COMDAT

; 123  : {

	sub	esp, 120				; 00000078H
	push	ebx
	push	ebp
	push	esi

; 124  :     /* Ignores top bit of h. */
; 125  :     int64_t h0 = load_4(s);

	mov	esi, DWORD PTR _s$[esp+128]
	push	edi
	push	esi
	call	_load_4
	mov	DWORD PTR _h0$[esp+140], eax

; 126  :     int64_t h1 = load_3(s + 4) << 6;

	lea	eax, DWORD PTR [esi+4]
	push	eax
	mov	edi, edx
	call	_load_3
	add	esp, 8
	mov	ecx, 6
	call	__allshl

; 127  :     int64_t h2 = load_3(s + 7) << 5;

	lea	ecx, DWORD PTR [esi+7]
	mov	ebx, eax
	mov	ebp, edx
	push	ecx
	mov	DWORD PTR _h1$[esp+140], ebx
	mov	DWORD PTR _h1$[esp+144], ebp
	call	_load_3
	add	esp, 4
	mov	ecx, 5
	call	__allshl
	mov	DWORD PTR _h2$[esp+140], edx

; 128  :     int64_t h3 = load_3(s + 10) << 3;

	lea	edx, DWORD PTR [esi+10]
	push	edx
	mov	DWORD PTR _h2$[esp+140], eax
	call	_load_3
	add	esp, 4
	mov	ecx, 3
	call	__allshl
	mov	DWORD PTR _h3$[esp+136], eax

; 129  :     int64_t h4 = load_3(s + 13) << 2;

	lea	eax, DWORD PTR [esi+13]
	push	eax
	mov	DWORD PTR _h3$[esp+144], edx
	call	_load_3
	add	esp, 4
	mov	ecx, 2
	call	__allshl

; 130  :     int64_t h5 = load_4(s + 16);

	lea	ecx, DWORD PTR [esi+16]
	mov	DWORD PTR _h4$[esp+136], eax
	push	ecx
	mov	DWORD PTR _h4$[esp+144], edx
	call	_load_4
	mov	DWORD PTR _h5$[esp+144], edx

; 131  :     int64_t h6 = load_3(s + 20) << 7;

	lea	edx, DWORD PTR [esi+20]
	push	edx
	mov	DWORD PTR _h5$[esp+144], eax
	call	_load_3
	add	esp, 8
	mov	ecx, 7
	call	__allshl
	mov	DWORD PTR _h6$[esp+136], eax

; 132  :     int64_t h7 = load_3(s + 23) << 5;

	lea	eax, DWORD PTR [esi+23]
	push	eax
	mov	DWORD PTR _h6$[esp+144], edx
	call	_load_3
	add	esp, 4
	mov	ecx, 5
	call	__allshl

; 133  :     int64_t h8 = load_3(s + 26) << 4;

	lea	ecx, DWORD PTR [esi+26]
	mov	DWORD PTR _h7$[esp+136], eax
	push	ecx
	mov	DWORD PTR _h7$[esp+144], edx
	call	_load_3
	add	esp, 4
	mov	ecx, 4
	call	__allshl

; 134  :     int64_t h9 = (load_3(s + 29) & 0x7fffff) << 2;

	add	esi, 29					; 0000001dH
	mov	DWORD PTR _h8$[esp+136], eax
	push	esi
	mov	DWORD PTR _h8$[esp+144], edx
	call	_load_3
	add	esp, 4
	and	eax, 8388607				; 007fffffH
	mov	ecx, 2
	xor	edx, edx
	call	__allshl
	mov	DWORD PTR _h9$[esp+136], eax

; 135  :     int64_t carry0;
; 136  :     int64_t carry1;
; 137  :     int64_t carry2;
; 138  :     int64_t carry3;
; 139  :     int64_t carry4;
; 140  :     int64_t carry5;
; 141  :     int64_t carry6;
; 142  :     int64_t carry7;
; 143  :     int64_t carry8;
; 144  :     int64_t carry9;
; 145  : 
; 146  :     carry9 = h9 + (1 << 24);

	add	eax, 16777216				; 01000000H
	mov	DWORD PTR _h9$[esp+140], edx
	adc	edx, 0

; 147  :     h0 += (carry9 >> 25) * 19;

	mov	ecx, 25					; 00000019H
	mov	DWORD PTR _carry9$[esp+136], eax
	mov	DWORD PTR _carry9$[esp+140], edx
	call	__allshr
	push	0
	push	19					; 00000013H
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR _h0$[esp+136]
	add	ecx, eax
	adc	edi, edx

; 148  :     h9 -= carry9 & kTop39Bits;
; 149  :     carry1 = h1 + (1 << 24);

	add	ebx, 16777216				; 01000000H
	adc	ebp, 0
	mov	DWORD PTR _h0$[esp+136], ecx

; 150  :     h2 += carry1 >> 25;

	mov	ecx, 25					; 00000019H
	mov	eax, ebx
	mov	edx, ebp
	call	__allshr
	mov	ecx, DWORD PTR _h2$[esp+136]
	mov	esi, DWORD PTR _h2$[esp+140]
	add	ecx, eax

; 151  :     h1 -= carry1 & kTop39Bits;
; 152  :     carry3 = h3 + (1 << 24);

	mov	eax, DWORD PTR _h3$[esp+136]
	adc	esi, edx
	mov	edx, DWORD PTR _h3$[esp+140]
	add	eax, 16777216				; 01000000H
	mov	DWORD PTR _h2$[esp+136], ecx
	adc	edx, 0

; 153  :     h4 += carry3 >> 25;

	mov	ecx, 25					; 00000019H
	mov	DWORD PTR _h2$[esp+140], esi
	mov	DWORD PTR _carry3$[esp+136], eax
	mov	DWORD PTR _carry3$[esp+140], edx
	call	__allshr
	mov	ecx, DWORD PTR _h4$[esp+136]
	mov	esi, DWORD PTR _h4$[esp+140]
	add	ecx, eax

; 154  :     h3 -= carry3 & kTop39Bits;
; 155  :     carry5 = h5 + (1 << 24);

	mov	eax, DWORD PTR _h5$[esp+136]
	adc	esi, edx
	mov	edx, DWORD PTR _h5$[esp+140]
	add	eax, 16777216				; 01000000H
	mov	DWORD PTR _h4$[esp+136], ecx
	adc	edx, 0

; 156  :     h6 += carry5 >> 25;

	mov	ecx, 25					; 00000019H
	mov	DWORD PTR _h4$[esp+140], esi
	mov	DWORD PTR _carry5$[esp+136], eax
	mov	DWORD PTR _carry5$[esp+140], edx
	call	__allshr
	mov	ecx, DWORD PTR _h6$[esp+136]
	mov	esi, DWORD PTR _h6$[esp+140]
	add	ecx, eax

; 157  :     h5 -= carry5 & kTop39Bits;
; 158  :     carry7 = h7 + (1 << 24);

	mov	eax, DWORD PTR _h7$[esp+136]
	adc	esi, edx
	mov	edx, DWORD PTR _h7$[esp+140]
	add	eax, 16777216				; 01000000H
	mov	DWORD PTR _h6$[esp+136], ecx
	adc	edx, 0

; 159  :     h8 += carry7 >> 25;

	mov	ecx, 25					; 00000019H
	mov	DWORD PTR _h6$[esp+140], esi
	mov	DWORD PTR _carry7$[esp+136], eax
	mov	DWORD PTR _carry7$[esp+140], edx
	call	__allshr
	mov	esi, DWORD PTR _h8$[esp+136]
	mov	ecx, DWORD PTR _h8$[esp+140]
	add	esi, eax
	mov	DWORD PTR _h8$[esp+136], esi

; 160  :     h7 -= carry7 & kTop39Bits;
; 161  : 
; 162  :     carry0 = h0 + (1 << 25);

	mov	esi, DWORD PTR _h0$[esp+136]
	adc	ecx, edx
	add	esi, 33554432				; 02000000H
	adc	edi, 0
	mov	DWORD PTR _h8$[esp+140], ecx
	mov	edx, edi
	mov	edi, DWORD PTR _kTop39Bits
	mov	eax, edi

; 163  :     h1 += carry0 >> 26;

	mov	ecx, 26					; 0000001aH
	and	eax, ebx
	mov	ebx, DWORD PTR _kTop39Bits+4
	and	ebx, ebp
	mov	DWORD PTR -120+[esp+136], eax
	mov	eax, esi
	call	__allshr
	mov	ecx, DWORD PTR -120+[esp+136]
	mov	ebp, DWORD PTR _h1$[esp+136]
	sub	eax, ecx
	mov	ecx, DWORD PTR _carry3$[esp+140]
	sbb	edx, ebx
	add	ebp, eax
	mov	eax, DWORD PTR _h1$[esp+140]
	mov	DWORD PTR _h1$[esp+136], ebp
	mov	ebp, DWORD PTR _carry3$[esp+136]
	mov	ebx, edi
	adc	eax, edx

; 164  :     h0 -= carry0 & kTop38Bits;
; 165  :     carry2 = h2 + (1 << 25);

	mov	edx, DWORD PTR _h2$[esp+140]
	mov	eax, DWORD PTR _h2$[esp+136]
	add	eax, 33554432				; 02000000H
	adc	edx, 0
	and	ebx, ebp
	mov	ebp, DWORD PTR _kTop39Bits+4
	mov	DWORD PTR _carry2$[esp+136], eax
	and	ebp, ecx

; 166  :     h3 += carry2 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	DWORD PTR _carry2$[esp+140], edx
	call	__allshr
	sub	eax, ebx

; 167  :     h2 -= carry2 & kTop38Bits;

	mov	ecx, DWORD PTR _carry2$[esp+136]
	sbb	edx, ebp
	mov	ebp, DWORD PTR _h3$[esp+136]
	add	ebp, eax
	mov	eax, DWORD PTR _h3$[esp+140]
	adc	eax, edx
	mov	edx, DWORD PTR _kTop38Bits
	mov	eax, DWORD PTR _carry2$[esp+140]
	and	edx, ecx
	mov	ecx, DWORD PTR _kTop38Bits+4
	mov	DWORD PTR _h3$[esp+136], ebp
	mov	ebp, DWORD PTR _h2$[esp+136]
	and	ecx, eax
	mov	eax, DWORD PTR _h2$[esp+140]
	sub	ebp, edx

; 168  :     carry4 = h4 + (1 << 25);

	mov	edx, DWORD PTR _h4$[esp+140]
	mov	DWORD PTR _h2$[esp+136], ebp
	mov	ebp, DWORD PTR _carry5$[esp+136]
	mov	ebx, edi
	sbb	eax, ecx
	mov	ecx, DWORD PTR _carry5$[esp+140]
	mov	eax, DWORD PTR _h4$[esp+136]
	add	eax, 33554432				; 02000000H
	adc	edx, 0
	and	ebx, ebp
	mov	ebp, DWORD PTR _kTop39Bits+4
	mov	DWORD PTR _carry4$[esp+136], eax
	and	ebp, ecx

; 169  :     h5 += carry4 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	DWORD PTR _carry4$[esp+140], edx
	call	__allshr
	sub	eax, ebx

; 170  :     h4 -= carry4 & kTop38Bits;

	mov	ecx, DWORD PTR _carry4$[esp+136]
	sbb	edx, ebp
	mov	ebp, DWORD PTR _h5$[esp+136]
	add	ebp, eax
	mov	eax, DWORD PTR _h5$[esp+140]
	adc	eax, edx
	mov	edx, DWORD PTR _kTop38Bits
	mov	eax, DWORD PTR _carry4$[esp+140]
	and	edx, ecx
	mov	ecx, DWORD PTR _kTop38Bits+4
	mov	DWORD PTR _h5$[esp+136], ebp
	mov	ebp, DWORD PTR _h4$[esp+136]
	and	ecx, eax
	mov	eax, DWORD PTR _h4$[esp+140]
	sub	ebp, edx

; 171  :     carry6 = h6 + (1 << 25);

	mov	edx, DWORD PTR _h6$[esp+140]
	mov	DWORD PTR _h4$[esp+136], ebp
	sbb	eax, ecx
	mov	ebx, edi
	mov	eax, DWORD PTR _h6$[esp+136]
	add	eax, 33554432				; 02000000H
	adc	edx, 0
	mov	DWORD PTR _carry6$[esp+136], eax
	mov	DWORD PTR _carry6$[esp+140], edx
	mov	ebp, DWORD PTR _carry7$[esp+136]
	mov	ecx, DWORD PTR _carry7$[esp+140]
	and	ebx, ebp
	mov	ebp, DWORD PTR _kTop39Bits+4
	and	ebp, ecx

; 172  :     h7 += carry6 >> 26;

	mov	ecx, 26					; 0000001aH
	call	__allshr
	sub	eax, ebx
	mov	ebx, DWORD PTR _h7$[esp+136]
	sbb	edx, ebp

; 173  :     h6 -= carry6 & kTop38Bits;

	mov	ebp, DWORD PTR _carry6$[esp+140]
	add	ebx, eax
	mov	eax, DWORD PTR _h7$[esp+140]
	adc	eax, edx
	mov	edx, DWORD PTR _kTop38Bits
	mov	eax, DWORD PTR _carry6$[esp+136]
	mov	DWORD PTR _h7$[esp+136], ebx
	mov	ebx, DWORD PTR _kTop38Bits+4
	and	edx, eax
	mov	eax, DWORD PTR _h6$[esp+136]
	mov	ecx, ebx
	and	ecx, ebp

; 174  :     carry8 = h8 + (1 << 25);

	mov	ebp, DWORD PTR _h8$[esp+136]
	sub	eax, edx
	mov	edx, DWORD PTR _h8$[esp+140]
	mov	DWORD PTR _h6$[esp+136], eax
	mov	eax, DWORD PTR _h6$[esp+140]
	sbb	eax, ecx
	mov	ecx, DWORD PTR _carry9$[esp+140]
	mov	eax, DWORD PTR _carry9$[esp+136]
	add	ebp, 33554432				; 02000000H
	adc	edx, 0
	and	edi, eax
	mov	eax, DWORD PTR _kTop39Bits+4
	mov	DWORD PTR _carry8$[esp+140], edx
	and	eax, ecx

; 175  :     h9 += carry8 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	DWORD PTR -16+[esp+140], eax
	mov	eax, ebp
	call	__allshr
	mov	ecx, edx
	mov	edx, DWORD PTR -16+[esp+140]
	sub	eax, edi

; 176  :     h8 -= carry8 & kTop38Bits;

	mov	edi, DWORD PTR _carry8$[esp+140]
	sbb	ecx, edx
	mov	edx, DWORD PTR _h9$[esp+136]
	add	edx, eax
	mov	eax, DWORD PTR _h9$[esp+140]
	adc	eax, ecx
	mov	ecx, DWORD PTR _kTop38Bits
	mov	eax, ecx
	and	ebx, edi
	mov	edi, DWORD PTR _h8$[esp+136]
	and	eax, ebp
	sub	edi, eax
	mov	eax, DWORD PTR _h8$[esp+140]
	sbb	eax, ebx
	and	esi, ecx
	mov	ecx, DWORD PTR _h0$[esp+136]

; 177  : 
; 178  :     h[0] = (int32_t)h0;

	mov	eax, DWORD PTR _h$[esp+132]
	sub	ecx, esi
	mov	DWORD PTR [eax], ecx

; 179  :     h[1] = (int32_t)h1;

	mov	ecx, DWORD PTR _h1$[esp+136]
	mov	DWORD PTR [eax+4], ecx

; 180  :     h[2] = (int32_t)h2;

	mov	ecx, DWORD PTR _h2$[esp+136]
	mov	DWORD PTR [eax+8], ecx

; 181  :     h[3] = (int32_t)h3;

	mov	ecx, DWORD PTR _h3$[esp+136]
	mov	DWORD PTR [eax+12], ecx

; 182  :     h[4] = (int32_t)h4;

	mov	ecx, DWORD PTR _h4$[esp+136]
	mov	DWORD PTR [eax+16], ecx

; 183  :     h[5] = (int32_t)h5;

	mov	ecx, DWORD PTR _h5$[esp+136]
	mov	DWORD PTR [eax+20], ecx

; 184  :     h[6] = (int32_t)h6;

	mov	ecx, DWORD PTR _h6$[esp+136]
	mov	DWORD PTR [eax+24], ecx

; 185  :     h[7] = (int32_t)h7;

	mov	ecx, DWORD PTR _h7$[esp+136]

; 186  :     h[8] = (int32_t)h8;

	mov	DWORD PTR [eax+32], edi
	pop	edi
	pop	esi
	mov	DWORD PTR [eax+28], ecx

; 187  :     h[9] = (int32_t)h9;

	mov	DWORD PTR [eax+36], edx
	pop	ebp
	pop	ebx

; 188  : }

	add	esp, 120				; 00000078H
	ret	0
_fe_frombytes ENDP
_TEXT	ENDS
;	COMDAT _load_3
_TEXT	SEGMENT
_in$ = 8
_load_3	PROC NEAR					; COMDAT

; 102  : {

	push	ebx
	push	esi

; 103  :     uint64_t result;
; 104  : 
; 105  :     result = ((uint64_t)in[0]);
; 106  :     result |= ((uint64_t)in[1]) << 8;
; 107  :     result |= ((uint64_t)in[2]) << 16;
; 108  :     return result;

	mov	esi, DWORD PTR _in$[esp+4]
	xor	eax, eax
	push	edi
	mov	ecx, 8
	mov	al, BYTE PTR [esi+2]
	cdq
	call	__allshl
	mov	edi, eax
	xor	eax, eax
	mov	al, BYTE PTR [esi+1]
	mov	ebx, edx
	cdq
	or	edi, eax
	or	ebx, edx
	mov	ecx, 8
	mov	eax, edi
	mov	edx, ebx
	call	__allshl
	mov	ecx, eax
	xor	eax, eax
	mov	al, BYTE PTR [esi]
	mov	edi, edx
	cdq
	or	edi, edx
	or	ecx, eax
	mov	edx, edi
	pop	edi
	pop	esi
	mov	eax, ecx
	pop	ebx

; 109  : }

	ret	0
_load_3	ENDP
_TEXT	ENDS
;	COMDAT _load_4
_TEXT	SEGMENT
_in$ = 8
_load_4	PROC NEAR					; COMDAT

; 112  : {

	push	ebx
	push	esi

; 113  :     uint64_t result;
; 114  : 
; 115  :     result = ((uint64_t)in[0]);
; 116  :     result |= ((uint64_t)in[1]) << 8;
; 117  :     result |= ((uint64_t)in[2]) << 16;
; 118  :     result |= ((uint64_t)in[3]) << 24;
; 119  :     return result;

	mov	esi, DWORD PTR _in$[esp+4]
	xor	eax, eax
	push	edi
	mov	ecx, 8
	mov	al, BYTE PTR [esi+3]
	cdq
	call	__allshl
	mov	edi, eax
	xor	eax, eax
	mov	al, BYTE PTR [esi+2]
	mov	ebx, edx
	cdq
	or	edi, eax
	or	ebx, edx
	mov	ecx, 8
	mov	eax, edi
	mov	edx, ebx
	call	__allshl
	mov	edi, eax
	xor	eax, eax
	mov	al, BYTE PTR [esi+1]
	mov	ebx, edx
	cdq
	or	edi, eax
	or	ebx, edx
	mov	ecx, 8
	mov	eax, edi
	mov	edx, ebx
	call	__allshl
	mov	ecx, eax
	xor	eax, eax
	mov	al, BYTE PTR [esi]
	mov	edi, edx
	cdq
	or	edi, edx
	or	ecx, eax
	mov	edx, edi
	pop	edi
	pop	esi
	mov	eax, ecx
	pop	ebx

; 120  : }

	ret	0
_load_4	ENDP
_TEXT	ENDS
;	COMDAT _fe_1
_TEXT	SEGMENT
_h$ = 8
_fe_1	PROC NEAR					; COMDAT

; 293  :     memset(h, 0, sizeof(int32_t) * 10);

	mov	edx, DWORD PTR _h$[esp-4]
	push	edi
	mov	ecx, 10					; 0000000aH
	xor	eax, eax
	mov	edi, edx
	rep stosd

; 294  :     h[0] = 1;

	mov	DWORD PTR [edx], 1
	pop	edi

; 295  : }

	ret	0
_fe_1	ENDP
_TEXT	ENDS
;	COMDAT _fe_add
_TEXT	SEGMENT
_h$ = 8
_f$ = 12
_g$ = 16
_fe_add	PROC NEAR					; COMDAT

; 299  :     unsigned i;
; 300  : 
; 301  :     for (i = 0; i < 10; i++) {

	mov	eax, DWORD PTR _g$[esp-4]
	mov	ecx, DWORD PTR _f$[esp-4]
	mov	edx, DWORD PTR _h$[esp-4]
	push	ebx
	push	esi
	sub	ecx, eax
	push	edi
	sub	edx, eax
	mov	esi, 10					; 0000000aH
$L1086:

; 302  :         h[i] = f[i] + g[i];

	mov	edi, DWORD PTR [ecx+eax]
	mov	ebx, DWORD PTR [eax]
	add	edi, ebx
	mov	DWORD PTR [edx+eax], edi
	add	eax, 4
	dec	esi
	jne	SHORT $L1086
	pop	edi
	pop	esi
	pop	ebx

; 303  :     }
; 304  : }

	ret	0
_fe_add	ENDP
_TEXT	ENDS
;	COMDAT _fe_sub
_TEXT	SEGMENT
_h$ = 8
_f$ = 12
_g$ = 16
_fe_sub	PROC NEAR					; COMDAT

; 308  :     unsigned i;
; 309  : 
; 310  :     for (i = 0; i < 10; i++) {

	mov	eax, DWORD PTR _g$[esp-4]
	mov	ecx, DWORD PTR _f$[esp-4]
	mov	edx, DWORD PTR _h$[esp-4]
	push	ebx
	push	esi
	sub	ecx, eax
	push	edi
	sub	edx, eax
	mov	esi, 10					; 0000000aH
$L1097:

; 311  :         h[i] = f[i] - g[i];

	mov	edi, DWORD PTR [ecx+eax]
	mov	ebx, DWORD PTR [eax]
	sub	edi, ebx
	mov	DWORD PTR [edx+eax], edi
	add	eax, 4
	dec	esi
	jne	SHORT $L1097
	pop	edi
	pop	esi
	pop	ebx

; 312  :     }
; 313  : }

	ret	0
_fe_sub	ENDP
_TEXT	ENDS
;	COMDAT _fe_neg
_TEXT	SEGMENT
_h$ = 8
_f$ = 12
_fe_neg	PROC NEAR					; COMDAT

; 810  :     unsigned i;
; 811  : 
; 812  :     for (i = 0; i < 10; i++) {

	mov	eax, DWORD PTR _h$[esp-4]
	mov	ecx, DWORD PTR _f$[esp-4]
	push	esi
	sub	ecx, eax
	mov	edx, 10					; 0000000aH
$L1579:

; 813  :         h[i] = -f[i];

	mov	esi, DWORD PTR [ecx+eax]
	add	eax, 4
	neg	esi
	mov	DWORD PTR [eax-4], esi
	dec	edx
	jne	SHORT $L1579
	pop	esi

; 814  :     }
; 815  : }

	ret	0
_fe_neg	ENDP
_TEXT	ENDS
;	COMDAT _fe_pow22523
_TEXT	SEGMENT
_out$ = 8
_z$ = 12
_t0$ = -80
_t1$ = -120
_t2$ = -40
_fe_pow22523 PROC NEAR					; COMDAT

; 1003 : {

	sub	esp, 120				; 00000078H

; 1004 :     fe t0;
; 1005 :     fe t1;
; 1006 :     fe t2;
; 1007 :     int i;
; 1008 : 
; 1009 :     fe_sq(t0, z);

	lea	eax, DWORD PTR _t0$[esp+120]
	push	esi
	push	edi
	mov	edi, DWORD PTR _z$[esp+124]
	push	edi
	push	eax
	call	_fe_sq

; 1010 :     fe_sq(t1, t0);

	lea	ecx, DWORD PTR _t0$[esp+136]
	lea	edx, DWORD PTR _t1$[esp+136]
	push	ecx
	push	edx
	call	_fe_sq

; 1011 :     for (i = 1; i < 2; ++i) {
; 1012 :         fe_sq(t1, t1);

	lea	eax, DWORD PTR _t1$[esp+144]
	lea	ecx, DWORD PTR _t1$[esp+144]
	push	eax
	push	ecx
	call	_fe_sq

; 1013 :     }
; 1014 :     fe_mul(t1, z, t1);

	lea	edx, DWORD PTR _t1$[esp+152]
	lea	eax, DWORD PTR _t1$[esp+152]
	push	edx
	push	edi
	push	eax
	call	_fe_mul

; 1015 :     fe_mul(t0, t0, t1);

	lea	ecx, DWORD PTR _t1$[esp+164]
	lea	edx, DWORD PTR _t0$[esp+164]
	push	ecx
	lea	eax, DWORD PTR _t0$[esp+168]
	push	edx
	push	eax
	call	_fe_mul

; 1016 :     fe_sq(t0, t0);

	lea	ecx, DWORD PTR _t0$[esp+176]
	lea	edx, DWORD PTR _t0$[esp+176]
	push	ecx
	push	edx
	call	_fe_sq

; 1017 :     fe_mul(t0, t1, t0);

	lea	eax, DWORD PTR _t0$[esp+184]
	lea	ecx, DWORD PTR _t1$[esp+184]
	push	eax
	lea	edx, DWORD PTR _t0$[esp+188]
	push	ecx
	push	edx
	call	_fe_mul
	add	esp, 68					; 00000044H

; 1018 :     fe_sq(t1, t0);

	lea	eax, DWORD PTR _t0$[esp+128]
	lea	ecx, DWORD PTR _t1$[esp+128]
	push	eax
	push	ecx
	call	_fe_sq
	add	esp, 8
	mov	esi, 4
$L1772:

; 1019 :     for (i = 1; i < 5; ++i) {
; 1020 :         fe_sq(t1, t1);

	lea	edx, DWORD PTR _t1$[esp+128]
	lea	eax, DWORD PTR _t1$[esp+128]
	push	edx
	push	eax
	call	_fe_sq
	add	esp, 8
	dec	esi
	jne	SHORT $L1772

; 1021 :     }
; 1022 :     fe_mul(t0, t1, t0);

	lea	ecx, DWORD PTR _t0$[esp+128]
	lea	edx, DWORD PTR _t1$[esp+128]
	push	ecx
	lea	eax, DWORD PTR _t0$[esp+132]
	push	edx
	push	eax
	call	_fe_mul

; 1023 :     fe_sq(t1, t0);

	lea	ecx, DWORD PTR _t0$[esp+140]
	lea	edx, DWORD PTR _t1$[esp+140]
	push	ecx
	push	edx
	call	_fe_sq
	add	esp, 20					; 00000014H
	mov	esi, 9
$L1775:

; 1024 :     for (i = 1; i < 10; ++i) {
; 1025 :         fe_sq(t1, t1);

	lea	eax, DWORD PTR _t1$[esp+128]
	lea	ecx, DWORD PTR _t1$[esp+128]
	push	eax
	push	ecx
	call	_fe_sq
	add	esp, 8
	dec	esi
	jne	SHORT $L1775

; 1026 :     }
; 1027 :     fe_mul(t1, t1, t0);

	lea	edx, DWORD PTR _t0$[esp+128]
	lea	eax, DWORD PTR _t1$[esp+128]
	push	edx
	lea	ecx, DWORD PTR _t1$[esp+132]
	push	eax
	push	ecx
	call	_fe_mul

; 1028 :     fe_sq(t2, t1);

	lea	edx, DWORD PTR _t1$[esp+140]
	lea	eax, DWORD PTR _t2$[esp+140]
	push	edx
	push	eax
	call	_fe_sq
	add	esp, 20					; 00000014H
	mov	esi, 19					; 00000013H
$L1778:

; 1029 :     for (i = 1; i < 20; ++i) {
; 1030 :         fe_sq(t2, t2);

	lea	ecx, DWORD PTR _t2$[esp+128]
	lea	edx, DWORD PTR _t2$[esp+128]
	push	ecx
	push	edx
	call	_fe_sq
	add	esp, 8
	dec	esi
	jne	SHORT $L1778

; 1031 :     }
; 1032 :     fe_mul(t1, t2, t1);

	lea	eax, DWORD PTR _t1$[esp+128]
	lea	ecx, DWORD PTR _t2$[esp+128]
	push	eax
	lea	edx, DWORD PTR _t1$[esp+132]
	push	ecx
	push	edx
	call	_fe_mul

; 1033 :     fe_sq(t1, t1);

	lea	eax, DWORD PTR _t1$[esp+140]
	lea	ecx, DWORD PTR _t1$[esp+140]
	push	eax
	push	ecx
	call	_fe_sq
	add	esp, 20					; 00000014H
	mov	esi, 9
$L1781:

; 1034 :     for (i = 1; i < 10; ++i) {
; 1035 :         fe_sq(t1, t1);

	lea	edx, DWORD PTR _t1$[esp+128]
	lea	eax, DWORD PTR _t1$[esp+128]
	push	edx
	push	eax
	call	_fe_sq
	add	esp, 8
	dec	esi
	jne	SHORT $L1781

; 1036 :     }
; 1037 :     fe_mul(t0, t1, t0);

	lea	ecx, DWORD PTR _t0$[esp+128]
	lea	edx, DWORD PTR _t1$[esp+128]
	push	ecx
	lea	eax, DWORD PTR _t0$[esp+132]
	push	edx
	push	eax
	call	_fe_mul

; 1038 :     fe_sq(t1, t0);

	lea	ecx, DWORD PTR _t0$[esp+140]
	lea	edx, DWORD PTR _t1$[esp+140]
	push	ecx
	push	edx
	call	_fe_sq
	add	esp, 20					; 00000014H
	mov	esi, 49					; 00000031H
$L1784:

; 1039 :     for (i = 1; i < 50; ++i) {
; 1040 :         fe_sq(t1, t1);

	lea	eax, DWORD PTR _t1$[esp+128]
	lea	ecx, DWORD PTR _t1$[esp+128]
	push	eax
	push	ecx
	call	_fe_sq
	add	esp, 8
	dec	esi
	jne	SHORT $L1784

; 1041 :     }
; 1042 :     fe_mul(t1, t1, t0);

	lea	edx, DWORD PTR _t0$[esp+128]
	lea	eax, DWORD PTR _t1$[esp+128]
	push	edx
	lea	ecx, DWORD PTR _t1$[esp+132]
	push	eax
	push	ecx
	call	_fe_mul

; 1043 :     fe_sq(t2, t1);

	lea	edx, DWORD PTR _t1$[esp+140]
	lea	eax, DWORD PTR _t2$[esp+140]
	push	edx
	push	eax
	call	_fe_sq
	add	esp, 20					; 00000014H
	mov	esi, 99					; 00000063H
$L1787:

; 1044 :     for (i = 1; i < 100; ++i) {
; 1045 :         fe_sq(t2, t2);

	lea	ecx, DWORD PTR _t2$[esp+128]
	lea	edx, DWORD PTR _t2$[esp+128]
	push	ecx
	push	edx
	call	_fe_sq
	add	esp, 8
	dec	esi
	jne	SHORT $L1787

; 1046 :     }
; 1047 :     fe_mul(t1, t2, t1);

	lea	eax, DWORD PTR _t1$[esp+128]
	lea	ecx, DWORD PTR _t2$[esp+128]
	push	eax
	lea	edx, DWORD PTR _t1$[esp+132]
	push	ecx
	push	edx
	call	_fe_mul

; 1048 :     fe_sq(t1, t1);

	lea	eax, DWORD PTR _t1$[esp+140]
	lea	ecx, DWORD PTR _t1$[esp+140]
	push	eax
	push	ecx
	call	_fe_sq
	add	esp, 20					; 00000014H
	mov	esi, 49					; 00000031H
$L1790:

; 1049 :     for (i = 1; i < 50; ++i) {
; 1050 :         fe_sq(t1, t1);

	lea	edx, DWORD PTR _t1$[esp+128]
	lea	eax, DWORD PTR _t1$[esp+128]
	push	edx
	push	eax
	call	_fe_sq
	add	esp, 8
	dec	esi
	jne	SHORT $L1790

; 1051 :     }
; 1052 :     fe_mul(t0, t1, t0);

	lea	ecx, DWORD PTR _t0$[esp+128]
	lea	edx, DWORD PTR _t1$[esp+128]
	push	ecx
	lea	eax, DWORD PTR _t0$[esp+132]
	push	edx
	push	eax
	call	_fe_mul

; 1053 :     fe_sq(t0, t0);

	lea	ecx, DWORD PTR _t0$[esp+140]
	lea	edx, DWORD PTR _t0$[esp+140]
	push	ecx
	push	edx
	call	_fe_sq

; 1054 :     for (i = 1; i < 2; ++i) {
; 1055 :         fe_sq(t0, t0);

	lea	eax, DWORD PTR _t0$[esp+148]
	lea	ecx, DWORD PTR _t0$[esp+148]
	push	eax
	push	ecx
	call	_fe_sq

; 1056 :     }
; 1057 :     fe_mul(out, t0, z);

	mov	eax, DWORD PTR _out$[esp+152]
	lea	edx, DWORD PTR _t0$[esp+156]
	push	edi
	push	edx
	push	eax
	call	_fe_mul
	add	esp, 40					; 00000028H
	pop	edi
	pop	esi

; 1058 : }

	add	esp, 120				; 00000078H
	ret	0
_fe_pow22523 ENDP
_TEXT	ENDS
;	COMDAT _ge_double_scalarmult_vartime
_TEXT	SEGMENT
_r$ = 8
_a$ = 12
_A$ = 16
_b$ = 20
_aslide$ = -256
_bslide$ = -1792
_Ai$ = -1536
_t$ = -2272
_u$ = -2112
_A2$ = -1952
_ge_double_scalarmult_vartime PROC NEAR			; COMDAT

; 1370 :     signed char aslide[256];
; 1371 :     signed char bslide[256];
; 1372 :     ge_cached Ai[8]; /* A,3A,5A,7A,9A,11A,13A,15A */
; 1373 :     ge_p1p1 t;
; 1374 :     ge_p3 u;
; 1375 :     ge_p3 A2;
; 1376 :     int i;
; 1377 : 
; 1378 :     slide(aslide, a);

	mov	eax, DWORD PTR _a$[esp-4]
	sub	esp, 2272				; 000008e0H
	lea	ecx, DWORD PTR _aslide$[esp+2272]
	push	ebp
	push	esi
	push	edi
	push	eax
	push	ecx
	call	_slide

; 1379 :     slide(bslide, b);

	mov	edx, DWORD PTR _b$[esp+2288]
	lea	eax, DWORD PTR _bslide$[esp+2292]
	push	edx
	push	eax
	call	_slide

; 1380 : 
; 1381 :     ge_p3_to_cached(&Ai[0], A);

	mov	esi, DWORD PTR _A$[esp+2296]
	lea	ecx, DWORD PTR _Ai$[esp+2300]
	push	esi
	push	ecx
	call	_ge_p3_to_cached

; 1382 :     ge_p3_dbl(&t, A);

	lea	edx, DWORD PTR _t$[esp+2308]
	push	esi
	push	edx
	call	_ge_p3_dbl

; 1383 :     ge_p1p1_to_p3(&A2, &t);

	lea	eax, DWORD PTR _t$[esp+2316]
	lea	ecx, DWORD PTR _A2$[esp+2316]
	push	eax
	push	ecx
	call	_ge_p1p1_to_p3

; 1384 :     ge_add(&t, &A2, &Ai[0]);

	lea	edx, DWORD PTR _Ai$[esp+2324]
	lea	eax, DWORD PTR _A2$[esp+2324]
	push	edx
	lea	ecx, DWORD PTR _t$[esp+2328]
	push	eax
	push	ecx
	call	_ge_add

; 1385 :     ge_p1p1_to_p3(&u, &t);

	lea	edx, DWORD PTR _t$[esp+2336]
	lea	eax, DWORD PTR _u$[esp+2336]
	push	edx
	push	eax
	call	_ge_p1p1_to_p3

; 1386 :     ge_p3_to_cached(&Ai[1], &u);

	lea	ecx, DWORD PTR _u$[esp+2344]
	lea	edx, DWORD PTR _Ai$[esp+2504]
	push	ecx
	push	edx
	call	_ge_p3_to_cached
	add	esp, 68					; 00000044H

; 1387 :     ge_add(&t, &A2, &Ai[1]);

	lea	eax, DWORD PTR _Ai$[esp+2444]
	lea	ecx, DWORD PTR _A2$[esp+2284]
	lea	edx, DWORD PTR _t$[esp+2284]
	push	eax
	push	ecx
	push	edx
	call	_ge_add

; 1388 :     ge_p1p1_to_p3(&u, &t);

	lea	eax, DWORD PTR _t$[esp+2296]
	lea	ecx, DWORD PTR _u$[esp+2296]
	push	eax
	push	ecx
	call	_ge_p1p1_to_p3

; 1389 :     ge_p3_to_cached(&Ai[2], &u);

	lea	edx, DWORD PTR _u$[esp+2304]
	lea	eax, DWORD PTR _Ai$[esp+2624]
	push	edx
	push	eax
	call	_ge_p3_to_cached

; 1390 :     ge_add(&t, &A2, &Ai[2]);

	lea	ecx, DWORD PTR _Ai$[esp+2632]
	lea	edx, DWORD PTR _A2$[esp+2312]
	push	ecx
	lea	eax, DWORD PTR _t$[esp+2316]
	push	edx
	push	eax
	call	_ge_add

; 1391 :     ge_p1p1_to_p3(&u, &t);

	lea	ecx, DWORD PTR _t$[esp+2324]
	lea	edx, DWORD PTR _u$[esp+2324]
	push	ecx
	push	edx
	call	_ge_p1p1_to_p3

; 1392 :     ge_p3_to_cached(&Ai[3], &u);

	lea	eax, DWORD PTR _u$[esp+2332]
	lea	ecx, DWORD PTR _Ai$[esp+2812]
	push	eax
	push	ecx
	call	_ge_p3_to_cached

; 1393 :     ge_add(&t, &A2, &Ai[3]);

	lea	edx, DWORD PTR _Ai$[esp+2820]
	lea	eax, DWORD PTR _A2$[esp+2340]
	push	edx
	lea	ecx, DWORD PTR _t$[esp+2344]
	push	eax
	push	ecx
	call	_ge_add
	add	esp, 68					; 00000044H

; 1394 :     ge_p1p1_to_p3(&u, &t);

	lea	edx, DWORD PTR _t$[esp+2284]
	lea	eax, DWORD PTR _u$[esp+2284]
	push	edx
	push	eax
	call	_ge_p1p1_to_p3

; 1395 :     ge_p3_to_cached(&Ai[4], &u);

	lea	ecx, DWORD PTR _u$[esp+2292]
	lea	edx, DWORD PTR _Ai$[esp+2932]
	push	ecx
	push	edx
	call	_ge_p3_to_cached

; 1396 :     ge_add(&t, &A2, &Ai[4]);

	lea	eax, DWORD PTR _Ai$[esp+2940]
	lea	ecx, DWORD PTR _A2$[esp+2300]
	push	eax
	lea	edx, DWORD PTR _t$[esp+2304]
	push	ecx
	push	edx
	call	_ge_add

; 1397 :     ge_p1p1_to_p3(&u, &t);

	lea	eax, DWORD PTR _t$[esp+2312]
	lea	ecx, DWORD PTR _u$[esp+2312]
	push	eax
	push	ecx
	call	_ge_p1p1_to_p3

; 1398 :     ge_p3_to_cached(&Ai[5], &u);

	lea	edx, DWORD PTR _u$[esp+2320]
	lea	eax, DWORD PTR _Ai$[esp+3120]
	push	edx
	push	eax
	call	_ge_p3_to_cached

; 1399 :     ge_add(&t, &A2, &Ai[5]);

	lea	ecx, DWORD PTR _Ai$[esp+3128]
	lea	edx, DWORD PTR _A2$[esp+2328]
	push	ecx
	lea	eax, DWORD PTR _t$[esp+2332]
	push	edx
	push	eax
	call	_ge_add

; 1400 :     ge_p1p1_to_p3(&u, &t);

	lea	ecx, DWORD PTR _t$[esp+2340]
	lea	edx, DWORD PTR _u$[esp+2340]
	push	ecx
	push	edx
	call	_ge_p1p1_to_p3
	add	esp, 64					; 00000040H

; 1401 :     ge_p3_to_cached(&Ai[6], &u);

	lea	eax, DWORD PTR _u$[esp+2284]
	lea	ecx, DWORD PTR _Ai$[esp+3244]
	push	eax
	push	ecx
	call	_ge_p3_to_cached

; 1402 :     ge_add(&t, &A2, &Ai[6]);

	lea	edx, DWORD PTR _Ai$[esp+3252]
	lea	eax, DWORD PTR _A2$[esp+2292]
	push	edx
	lea	ecx, DWORD PTR _t$[esp+2296]
	push	eax
	push	ecx
	call	_ge_add

; 1403 :     ge_p1p1_to_p3(&u, &t);

	lea	edx, DWORD PTR _t$[esp+2304]
	lea	eax, DWORD PTR _u$[esp+2304]
	push	edx
	push	eax
	call	_ge_p1p1_to_p3

; 1404 :     ge_p3_to_cached(&Ai[7], &u);

	lea	ecx, DWORD PTR _u$[esp+2312]
	lea	edx, DWORD PTR _Ai$[esp+3432]
	push	ecx
	push	edx
	call	_ge_p3_to_cached

; 1405 : 
; 1406 :     ge_p2_0(r);

	mov	ebp, DWORD PTR _r$[esp+2316]
	push	ebp
	call	_ge_p2_0
	add	esp, 40					; 00000028H

; 1407 : 
; 1408 :     for (i = 255; i >= 0; --i) {

	mov	edi, 255				; 000000ffH
$L1942:

; 1409 :         if (aslide[i] || bslide[i]) {

	mov	al, BYTE PTR _aslide$[esp+edi+2284]
	test	al, al
	jne	SHORT $L2107
	mov	al, BYTE PTR _bslide$[esp+edi+2284]
	test	al, al
	jne	SHORT $L2107

; 1407 : 
; 1408 :     for (i = 255; i >= 0; --i) {

	dec	edi
	jns	SHORT $L1942
	pop	edi
	pop	esi
	pop	ebp

; 1434 :     }
; 1435 : }

	add	esp, 2272				; 000008e0H
	ret	0
$L2107:

; 1410 :             break;
; 1411 :         }
; 1412 :     }
; 1413 : 
; 1414 :     for (; i >= 0; --i) {

	test	edi, edi
	jl	$L1949
	push	ebx
	lea	ebx, DWORD PTR _aslide$[esp+2288]
	lea	eax, DWORD PTR _bslide$[esp+2288]
	sub	ebx, eax
$L1947:

; 1415 :         ge_p2_dbl(&t, r);

	lea	ecx, DWORD PTR _t$[esp+2288]
	push	ebp
	push	ecx
	call	_ge_p2_dbl
	lea	edx, DWORD PTR [ebx+edi]
	add	esp, 8

; 1416 : 
; 1417 :         if (aslide[i] > 0) {

	mov	al, BYTE PTR _bslide$[esp+edx+2288]
	lea	esi, DWORD PTR _bslide$[esp+edx+2288]
	test	al, al
	jle	SHORT $L2110

; 1418 :             ge_p1p1_to_p3(&u, &t);

	lea	eax, DWORD PTR _t$[esp+2288]
	lea	ecx, DWORD PTR _u$[esp+2288]
	push	eax
	push	ecx
	call	_ge_p1p1_to_p3

; 1419 :             ge_add(&t, &u, &Ai[aslide[i] / 2]);

	movsx	eax, BYTE PTR [esi]
	cdq
	sub	eax, edx
	lea	ecx, DWORD PTR _u$[esp+2296]
	sar	eax, 1
	lea	edx, DWORD PTR [eax+eax*4]
	shl	edx, 5
	lea	eax, DWORD PTR _Ai$[esp+edx+2296]
	lea	edx, DWORD PTR _t$[esp+2296]
	push	eax
	push	ecx
	push	edx
	call	_ge_add

; 1420 :         } else if (aslide[i] < 0) {

	jmp	SHORT $L2111
$L2110:
	jge	SHORT $L1952

; 1421 :             ge_p1p1_to_p3(&u, &t);

	lea	eax, DWORD PTR _t$[esp+2288]
	lea	ecx, DWORD PTR _u$[esp+2288]
	push	eax
	push	ecx
	call	_ge_p1p1_to_p3

; 1422 :             ge_sub(&t, &u, &Ai[(-aslide[i]) / 2]);

	movsx	eax, BYTE PTR [esi]
	cdq
	sub	eax, edx
	lea	ecx, DWORD PTR _u$[esp+2296]
	sar	eax, 1
	lea	edx, DWORD PTR [eax+eax*4]
	lea	eax, DWORD PTR _Ai$[esp+2296]
	shl	edx, 5
	sub	eax, edx
	lea	edx, DWORD PTR _t$[esp+2296]
	push	eax
	push	ecx
	push	edx
	call	_ge_sub
$L2111:
	add	esp, 20					; 00000014H
$L1952:

; 1423 :         }
; 1424 : 
; 1425 :         if (bslide[i] > 0) {

	mov	al, BYTE PTR _bslide$[esp+edi+2288]
	test	al, al
	jle	SHORT $L2112

; 1426 :             ge_p1p1_to_p3(&u, &t);

	lea	eax, DWORD PTR _t$[esp+2288]
	lea	ecx, DWORD PTR _u$[esp+2288]
	push	eax
	push	ecx
	call	_ge_p1p1_to_p3

; 1427 :             ge_madd(&t, &u, &Bi[bslide[i] / 2]);

	movsx	eax, BYTE PTR _bslide$[esp+edi+2296]
	cdq
	sub	eax, edx
	lea	ecx, DWORD PTR _u$[esp+2296]
	sar	eax, 1
	lea	eax, DWORD PTR [eax+eax*2]
	lea	edx, DWORD PTR [eax+eax*4]
	lea	eax, DWORD PTR _Bi[edx*8]
	lea	edx, DWORD PTR _t$[esp+2296]
	push	eax
	push	ecx
	push	edx
	call	_ge_madd

; 1428 :         } else if (bslide[i] < 0) {

	jmp	SHORT $L2113
$L2112:
	jge	SHORT $L1955

; 1429 :             ge_p1p1_to_p3(&u, &t);

	lea	eax, DWORD PTR _t$[esp+2288]
	lea	ecx, DWORD PTR _u$[esp+2288]
	push	eax
	push	ecx
	call	_ge_p1p1_to_p3

; 1430 :             ge_msub(&t, &u, &Bi[(-bslide[i]) / 2]);

	movsx	eax, BYTE PTR _bslide$[esp+edi+2296]
	cdq
	sub	eax, edx
	lea	ecx, DWORD PTR _u$[esp+2296]
	sar	eax, 1
	lea	eax, DWORD PTR [eax+eax*2]
	lea	edx, DWORD PTR [eax+eax*4]
	mov	eax, OFFSET FLAT:_Bi
	shl	edx, 3
	sub	eax, edx
	lea	edx, DWORD PTR _t$[esp+2296]
	push	eax
	push	ecx
	push	edx
	call	_ge_msub
$L2113:
	add	esp, 20					; 00000014H
$L1955:

; 1431 :         }
; 1432 : 
; 1433 :         ge_p1p1_to_p2(r, &t);

	lea	eax, DWORD PTR _t$[esp+2288]
	push	eax
	push	ebp
	call	_ge_p1p1_to_p2
	add	esp, 8
	dec	edi
	jns	$L1947
	pop	ebx
$L1949:
	pop	edi
	pop	esi
	pop	ebp

; 1434 :     }
; 1435 : }

	add	esp, 2272				; 000008e0H
	ret	0
_ge_double_scalarmult_vartime ENDP
_TEXT	ENDS
;	COMDAT _ge_p2_0
_TEXT	SEGMENT
_h$ = 8
_ge_p2_0 PROC NEAR					; COMDAT

; 1136 : {

	push	esi

; 1137 :     fe_0(h->X);

	mov	esi, DWORD PTR _h$[esp]
	push	esi
	call	_fe_0

; 1138 :     fe_1(h->Y);

	lea	eax, DWORD PTR [esi+40]
	push	eax
	call	_fe_1

; 1139 :     fe_1(h->Z);

	add	esi, 80					; 00000050H
	push	esi
	call	_fe_1
	add	esp, 12					; 0000000cH
	pop	esi

; 1140 : }

	ret	0
_ge_p2_0 ENDP
_TEXT	ENDS
;	COMDAT _fe_0
_TEXT	SEGMENT
_h$ = 8
_fe_0	PROC NEAR					; COMDAT

; 287  : {

	push	edi

; 288  :     memset(h, 0, sizeof(int32_t) * 10);

	mov	edi, DWORD PTR _h$[esp]
	mov	ecx, 10					; 0000000aH
	xor	eax, eax
	rep stosd
	pop	edi

; 289  : }

	ret	0
_fe_0	ENDP
_TEXT	ENDS
;	COMDAT _ge_p3_to_cached
_TEXT	SEGMENT
_r$ = 8
_p$ = 12
_ge_p3_to_cached PROC NEAR				; COMDAT

; 1155 : {

	push	ebx
	push	esi

; 1156 :     fe_add(r->YplusX, p->Y, p->X);

	mov	esi, DWORD PTR _p$[esp+4]
	push	edi
	mov	edi, DWORD PTR _r$[esp+8]
	push	esi
	lea	ebx, DWORD PTR [esi+40]
	push	ebx
	push	edi
	call	_fe_add

; 1157 :     fe_sub(r->YminusX, p->Y, p->X);

	push	esi
	lea	eax, DWORD PTR [edi+40]
	push	ebx
	push	eax
	call	_fe_sub

; 1158 :     fe_copy(r->Z, p->Z);

	lea	ecx, DWORD PTR [esi+80]
	lea	edx, DWORD PTR [edi+80]
	push	ecx
	push	edx
	call	_fe_copy

; 1159 :     fe_mul(r->T2d, p->T, d2);

	add	esi, 120				; 00000078H
	push	OFFSET FLAT:_d2
	add	edi, 120				; 00000078H
	push	esi
	push	edi
	call	_fe_mul
	add	esp, 44					; 0000002cH
	pop	edi
	pop	esi
	pop	ebx

; 1160 : }

	ret	0
_ge_p3_to_cached ENDP
_TEXT	ENDS
EXTRN	__imp__memmove:NEAR
;	COMDAT _fe_copy
_TEXT	SEGMENT
_h$ = 8
_f$ = 12
_fe_copy PROC NEAR					; COMDAT

; 283  :     memmove(h, f, sizeof(int32_t) * 10);

	mov	eax, DWORD PTR _f$[esp-4]
	mov	ecx, DWORD PTR _h$[esp-4]
	push	40					; 00000028H
	push	eax
	push	ecx
	call	DWORD PTR __imp__memmove
	add	esp, 12					; 0000000cH

; 284  : }

	ret	0
_fe_copy ENDP
_TEXT	ENDS
;	COMDAT _ge_p1p1_to_p2
_TEXT	SEGMENT
_r$ = 8
_p$ = 12
_ge_p1p1_to_p2 PROC NEAR				; COMDAT

; 1163 : {

	push	ebx

; 1164 :     fe_mul(r->X, p->X, p->T);

	mov	ebx, DWORD PTR _r$[esp]
	push	ebp
	push	esi
	mov	esi, DWORD PTR _p$[esp+8]
	push	edi
	lea	edi, DWORD PTR [esi+120]
	push	edi
	push	esi
	push	ebx
	call	_fe_mul

; 1165 :     fe_mul(r->Y, p->Y, p->Z);

	lea	ebp, DWORD PTR [esi+80]
	add	esi, 40					; 00000028H
	push	ebp
	lea	eax, DWORD PTR [ebx+40]
	push	esi
	push	eax
	call	_fe_mul

; 1166 :     fe_mul(r->Z, p->Z, p->T);

	push	edi
	add	ebx, 80					; 00000050H
	push	ebp
	push	ebx
	call	_fe_mul
	add	esp, 36					; 00000024H
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 1167 : }

	ret	0
_ge_p1p1_to_p2 ENDP
_TEXT	ENDS
;	COMDAT _ge_p1p1_to_p3
_TEXT	SEGMENT
_r$ = 8
_p$ = 12
_ge_p1p1_to_p3 PROC NEAR				; COMDAT

; 1170 : {

	push	ebx
	push	ebp
	push	esi

; 1171 :     fe_mul(r->X, p->X, p->T);

	mov	esi, DWORD PTR _p$[esp+8]
	push	edi
	mov	edi, DWORD PTR _r$[esp+12]
	lea	ebx, DWORD PTR [esi+120]
	push	ebx
	push	esi
	push	edi
	call	_fe_mul

; 1172 :     fe_mul(r->Y, p->Y, p->Z);

	lea	ebp, DWORD PTR [esi+80]
	lea	eax, DWORD PTR [esi+40]
	push	ebp
	push	eax
	lea	eax, DWORD PTR [edi+40]
	push	eax
	call	_fe_mul

; 1173 :     fe_mul(r->Z, p->Z, p->T);

	push	ebx
	lea	ecx, DWORD PTR [edi+80]
	push	ebp
	push	ecx
	call	_fe_mul

; 1174 :     fe_mul(r->T, p->X, p->Y);

	lea	eax, DWORD PTR [esi+40]
	add	edi, 120				; 00000078H
	push	eax
	push	esi
	push	edi
	call	_fe_mul
	add	esp, 48					; 00000030H
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 1175 : }

	ret	0
_ge_p1p1_to_p3 ENDP
_TEXT	ENDS
;	COMDAT _ge_p2_dbl
_TEXT	SEGMENT
_r$ = 8
_p$ = 12
_t0$ = -40
_ge_p2_dbl PROC NEAR					; COMDAT

; 1178 : {

	sub	esp, 40					; 00000028H
	push	ebx
	push	ebp

; 1179 :     fe t0;
; 1180 : 
; 1181 :     fe_sq(r->X, p->X);

	mov	ebp, DWORD PTR _p$[esp+44]
	push	esi
	mov	esi, DWORD PTR _r$[esp+48]
	push	edi
	push	ebp
	push	esi
	call	_fe_sq

; 1182 :     fe_sq(r->Z, p->Y);

	lea	eax, DWORD PTR [ebp+40]
	lea	edi, DWORD PTR [esi+80]
	push	eax
	push	edi
	call	_fe_sq

; 1183 :     fe_sq2(r->T, p->Z);

	lea	ecx, DWORD PTR [ebp+80]
	lea	eax, DWORD PTR [esi+120]
	push	ecx
	push	eax
	mov	DWORD PTR 12+[esp+76], eax
	call	_fe_sq2

; 1184 :     fe_add(r->Y, p->X, p->Y);

	lea	eax, DWORD PTR [ebp+40]
	lea	ebx, DWORD PTR [esi+40]
	push	eax
	push	ebp
	push	ebx
	call	_fe_add

; 1185 :     fe_sq(t0, r->Y);

	lea	edx, DWORD PTR _t0$[esp+92]
	push	ebx
	push	edx
	call	_fe_sq

; 1186 :     fe_add(r->Y, r->Z, r->X);

	push	esi
	push	edi
	push	ebx
	call	_fe_add

; 1187 :     fe_sub(r->Z, r->Z, r->X);

	push	esi
	push	edi
	push	edi
	call	_fe_sub
	add	esp, 68					; 00000044H

; 1188 :     fe_sub(r->X, t0, r->Y);

	lea	eax, DWORD PTR _t0$[esp+56]
	push	ebx
	push	eax
	push	esi
	call	_fe_sub

; 1189 :     fe_sub(r->T, r->T, r->Z);

	mov	eax, DWORD PTR 12+[esp+64]
	push	edi
	push	eax
	push	eax
	call	_fe_sub
	add	esp, 24					; 00000018H
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 1190 : }

	add	esp, 40					; 00000028H
	ret	0
_ge_p2_dbl ENDP
_TEXT	ENDS
;	COMDAT _fe_sq2
_TEXT	SEGMENT
_h$ = 8
_f$ = 12
_f1$ = -88
_f2$ = -152
_f3$ = -80
_f4$ = -72
_f6$ = -104
_f7$ = -168
_f8$ = -96
_f9$ = -56
_f0_2$ = -120
_f1_2$ = -200
_f2_2$ = -160
_f3_2$ = -216
_f4_2$ = -192
_f5_2$ = -208
_f7_2$ = -176
_f6_19$ = -224
_h0$ = -48
_h1$ = -8
_h2$ = -120
_h3$ = -184
_h4$ = -152
_h5$ = -80
_h6$ = -96
_h7$ = -176
_h8$ = -56
_h9$ = -136
_carry0$ = -32
_carry1$ = -24
_carry2$ = -176
_carry3$ = -104
_carry4$ = -136
_carry5$ = -144
_carry6$ = -224
_carry7$ = -136
_carry8$ = -144
_fe_sq2	PROC NEAR					; COMDAT

; 836  : {

	sub	esp, 224				; 000000e0H

; 837  :     int32_t f0 = f[0];

	mov	eax, DWORD PTR _f$[esp+220]
	push	ebx
	push	ebp
	push	esi
	mov	ebp, DWORD PTR [eax]

; 838  :     int32_t f1 = f[1];

	mov	ecx, DWORD PTR [eax+4]

; 839  :     int32_t f2 = f[2];

	mov	edx, DWORD PTR [eax+8]
	mov	DWORD PTR _f1$[esp+236], ecx

; 840  :     int32_t f3 = f[3];
; 841  :     int32_t f4 = f[4];
; 842  :     int32_t f5 = f[5];
; 843  :     int32_t f6 = f[6];
; 844  :     int32_t f7 = f[7];
; 845  :     int32_t f8 = f[8];
; 846  :     int32_t f9 = f[9];
; 847  :     int32_t f0_2 = 2 * f0;

	lea	ebx, DWORD PTR [ebp+ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	DWORD PTR _f0_2$[esp+236], ebx

; 848  :     int32_t f1_2 = 2 * f1;

	mov	ebx, DWORD PTR _f1$[esp+236]
	add	ebx, ebx
	mov	DWORD PTR _f2$[esp+236], edx
	mov	edx, DWORD PTR [eax+16]
	mov	DWORD PTR _f1_2$[esp+236], ebx

; 849  :     int32_t f2_2 = 2 * f2;

	mov	ebx, DWORD PTR _f2$[esp+236]
	mov	DWORD PTR _f3$[esp+236], ecx
	add	ebx, ebx
	mov	ecx, DWORD PTR [eax+20]
	mov	DWORD PTR _f2_2$[esp+236], ebx

; 850  :     int32_t f3_2 = 2 * f3;

	mov	ebx, DWORD PTR _f3$[esp+236]
	add	ebx, ebx
	mov	DWORD PTR _f4$[esp+236], edx
	mov	edx, DWORD PTR [eax+28]
	mov	DWORD PTR _f3_2$[esp+236], ebx

; 851  :     int32_t f4_2 = 2 * f4;

	mov	ebx, DWORD PTR _f4$[esp+236]
	mov	esi, DWORD PTR [eax+24]
	add	ebx, ebx
	mov	DWORD PTR _f6$[esp+236], esi
	mov	DWORD PTR _f4_2$[esp+236], ebx

; 852  :     int32_t f5_2 = 2 * f5;

	lea	ebx, DWORD PTR [ecx+ecx]
	mov	DWORD PTR _f5_2$[esp+236], ebx

; 853  :     int32_t f6_2 = 2 * f6;
; 854  :     int32_t f7_2 = 2 * f7;

	lea	ebx, DWORD PTR [edx+edx]
	mov	DWORD PTR _f7_2$[esp+236], ebx

; 855  :     int32_t f5_38 = 38 * f5; /* 1.959375*2^30 */
; 856  :     int32_t f6_19 = 19 * f6; /* 1.959375*2^30 */

	lea	ebx, DWORD PTR [esi+esi*8]
	push	edi
	mov	edi, DWORD PTR [eax+32]
	mov	eax, DWORD PTR [eax+36]
	lea	esi, DWORD PTR [esi+ebx*2]
	mov	DWORD PTR _f6_19$[esp+240], esi

; 857  :     int32_t f7_38 = 38 * f7; /* 1.959375*2^30 */

	lea	esi, DWORD PTR [edx+edx*8]
	mov	DWORD PTR _f7$[esp+240], edx
	mov	DWORD PTR _f8$[esp+240], edi
	lea	ebx, DWORD PTR [edx+esi*2]

; 858  :     int32_t f8_19 = 19 * f8; /* 1.959375*2^30 */

	lea	edx, DWORD PTR [edi+edi*8]
	mov	DWORD PTR _f9$[esp+240], eax
	lea	edi, DWORD PTR [edi+edx*2]

; 859  :     int32_t f9_38 = 38 * f9; /* 1.959375*2^30 */

	lea	edx, DWORD PTR [eax+eax*8]
	shl	ebx, 1
	lea	esi, DWORD PTR [eax+edx*2]

; 860  :     int64_t f0f0 = f0 * (int64_t)f0;
; 861  :     int64_t f0f1_2 = f0_2 * (int64_t)f1;
; 862  :     int64_t f0f2_2 = f0_2 * (int64_t)f2;
; 863  :     int64_t f0f3_2 = f0_2 * (int64_t)f3;
; 864  :     int64_t f0f4_2 = f0_2 * (int64_t)f4;
; 865  :     int64_t f0f5_2 = f0_2 * (int64_t)f5;
; 866  :     int64_t f0f6_2 = f0_2 * (int64_t)f6;
; 867  :     int64_t f0f7_2 = f0_2 * (int64_t)f7;
; 868  :     int64_t f0f8_2 = f0_2 * (int64_t)f8;
; 869  :     int64_t f0f9_2 = f0_2 * (int64_t)f9;
; 870  :     int64_t f1f1_2 = f1_2 * (int64_t)f1;
; 871  :     int64_t f1f2_2 = f1_2 * (int64_t)f2;
; 872  :     int64_t f1f3_4 = f1_2 * (int64_t)f3_2;
; 873  :     int64_t f1f4_2 = f1_2 * (int64_t)f4;
; 874  :     int64_t f1f5_4 = f1_2 * (int64_t)f5_2;
; 875  :     int64_t f1f6_2 = f1_2 * (int64_t)f6;
; 876  :     int64_t f1f7_4 = f1_2 * (int64_t)f7_2;
; 877  :     int64_t f1f8_2 = f1_2 * (int64_t)f8;
; 878  :     int64_t f1f9_76 = f1_2 * (int64_t)f9_38;
; 879  :     int64_t f2f2 = f2 * (int64_t)f2;
; 880  :     int64_t f2f3_2 = f2_2 * (int64_t)f3;
; 881  :     int64_t f2f4_2 = f2_2 * (int64_t)f4;
; 882  :     int64_t f2f5_2 = f2_2 * (int64_t)f5;
; 883  :     int64_t f2f6_2 = f2_2 * (int64_t)f6;
; 884  :     int64_t f2f7_2 = f2_2 * (int64_t)f7;
; 885  :     int64_t f2f8_38 = f2_2 * (int64_t)f8_19;
; 886  :     int64_t f2f9_38 = f2 * (int64_t)f9_38;
; 887  :     int64_t f3f3_2 = f3_2 * (int64_t)f3;
; 888  :     int64_t f3f4_2 = f3_2 * (int64_t)f4;
; 889  :     int64_t f3f5_4 = f3_2 * (int64_t)f5_2;
; 890  :     int64_t f3f6_2 = f3_2 * (int64_t)f6;
; 891  :     int64_t f3f7_76 = f3_2 * (int64_t)f7_38;
; 892  :     int64_t f3f8_38 = f3_2 * (int64_t)f8_19;
; 893  :     int64_t f3f9_76 = f3_2 * (int64_t)f9_38;
; 894  :     int64_t f4f4 = f4 * (int64_t)f4;
; 895  :     int64_t f4f5_2 = f4_2 * (int64_t)f5;
; 896  :     int64_t f4f6_38 = f4_2 * (int64_t)f6_19;
; 897  :     int64_t f4f7_38 = f4 * (int64_t)f7_38;
; 898  :     int64_t f4f8_38 = f4_2 * (int64_t)f8_19;
; 899  :     int64_t f4f9_38 = f4 * (int64_t)f9_38;
; 900  :     int64_t f5f5_38 = f5 * (int64_t)f5_38;
; 901  :     int64_t f5f6_38 = f5_2 * (int64_t)f6_19;
; 902  :     int64_t f5f7_76 = f5_2 * (int64_t)f7_38;
; 903  :     int64_t f5f8_38 = f5_2 * (int64_t)f8_19;
; 904  :     int64_t f5f9_76 = f5_2 * (int64_t)f9_38;
; 905  :     int64_t f6f6_19 = f6 * (int64_t)f6_19;
; 906  :     int64_t f6f7_38 = f6 * (int64_t)f7_38;
; 907  :     int64_t f6f8_38 = f6_2 * (int64_t)f8_19;
; 908  :     int64_t f6f9_38 = f6 * (int64_t)f9_38;
; 909  :     int64_t f7f7_38 = f7 * (int64_t)f7_38;
; 910  :     int64_t f7f8_38 = f7_2 * (int64_t)f8_19;
; 911  :     int64_t f7f9_76 = f7_2 * (int64_t)f9_38;
; 912  :     int64_t f8f8_19 = f8 * (int64_t)f8_19;
; 913  :     int64_t f8f9_38 = f8 * (int64_t)f9_38;
; 914  :     int64_t f9f9_38 = f9 * (int64_t)f9_38;
; 915  :     int64_t h0 = f0f0 + f1f9_76 + f2f8_38 + f3f7_76 + f4f6_38 + f5f5_38;
; 916  :     int64_t h1 = f0f1_2 + f2f9_38 + f3f8_38 + f4f7_38 + f5f6_38;
; 917  :     int64_t h2 = f0f2_2 + f1f1_2 + f3f9_76 + f4f8_38 + f5f7_76 + f6f6_19;
; 918  :     int64_t h3 = f0f3_2 + f1f2_2 + f4f9_38 + f5f8_38 + f6f7_38;
; 919  :     int64_t h4 = f0f4_2 + f1f3_4 + f2f2 + f5f9_76 + f6f8_38 + f7f7_38;
; 920  :     int64_t h5 = f0f5_2 + f1f4_2 + f2f3_2 + f6f9_38 + f7f8_38;
; 921  :     int64_t h6 = f0f6_2 + f1f5_4 + f2f4_2 + f3f3_2 + f7f9_76 + f8f8_19;
; 922  :     int64_t h7 = f0f7_2 + f1f6_2 + f2f5_2 + f3f4_2 + f8f9_38;
; 923  :     int64_t h8 = f0f8_2 + f1f7_4 + f2f6_2 + f3f5_4 + f4f4 + f9f9_38;
; 924  :     int64_t h9 = f0f9_2 + f1f8_2 + f2f7_2 + f3f6_2 + f4f5_2;
; 925  :     int64_t carry0;
; 926  :     int64_t carry1;
; 927  :     int64_t carry2;
; 928  :     int64_t carry3;
; 929  :     int64_t carry4;
; 930  :     int64_t carry5;
; 931  :     int64_t carry6;
; 932  :     int64_t carry7;
; 933  :     int64_t carry8;
; 934  :     int64_t carry9;
; 935  : 
; 936  :     h0 += h0;
; 937  :     h1 += h1;
; 938  :     h2 += h2;
; 939  :     h3 += h3;
; 940  :     h4 += h4;
; 941  :     h5 += h5;
; 942  :     h6 += h6;
; 943  :     h7 += h7;
; 944  :     h8 += h8;
; 945  :     h9 += h9;
; 946  : 
; 947  :     carry0 = h0 + (1 << 25);
; 948  :     h1 += carry0 >> 26;
; 949  :     h0 -= carry0 & kTop38Bits;
; 950  :     carry4 = h4 + (1 << 25);
; 951  :     h5 += carry4 >> 26;
; 952  :     h4 -= carry4 & kTop38Bits;
; 953  : 
; 954  :     carry1 = h1 + (1 << 24);
; 955  :     h2 += carry1 >> 25;
; 956  :     h1 -= carry1 & kTop39Bits;
; 957  :     carry5 = h5 + (1 << 24);
; 958  :     h6 += carry5 >> 25;
; 959  :     h5 -= carry5 & kTop39Bits;
; 960  : 
; 961  :     carry2 = h2 + (1 << 25);
; 962  :     h3 += carry2 >> 26;
; 963  :     h2 -= carry2 & kTop38Bits;
; 964  :     carry6 = h6 + (1 << 25);
; 965  :     h7 += carry6 >> 26;
; 966  :     h6 -= carry6 & kTop38Bits;
; 967  : 
; 968  :     carry3 = h3 + (1 << 24);
; 969  :     h4 += carry3 >> 25;
; 970  :     h3 -= carry3 & kTop39Bits;
; 971  :     carry7 = h7 + (1 << 24);
; 972  :     h8 += carry7 >> 25;
; 973  :     h7 -= carry7 & kTop39Bits;
; 974  : 
; 975  :     carry4 = h4 + (1 << 25);
; 976  :     h5 += carry4 >> 26;
; 977  :     h4 -= carry4 & kTop38Bits;
; 978  :     carry8 = h8 + (1 << 25);
; 979  :     h9 += carry8 >> 26;
; 980  :     h8 -= carry8 & kTop38Bits;
; 981  : 
; 982  :     carry9 = h9 + (1 << 24);
; 983  :     h0 += (carry9 >> 25) * 19;
; 984  :     h9 -= carry9 & kTop39Bits;
; 985  : 
; 986  :     carry0 = h0 + (1 << 25);
; 987  :     h1 += carry0 >> 26;
; 988  :     h0 -= carry0 & kTop38Bits;
; 989  : 
; 990  :     h[0] = (int32_t)h0;
; 991  :     h[1] = (int32_t)h1;
; 992  :     h[2] = (int32_t)h2;
; 993  :     h[3] = (int32_t)h3;
; 994  :     h[4] = (int32_t)h4;
; 995  :     h[5] = (int32_t)h5;
; 996  :     h[6] = (int32_t)h6;
; 997  :     h[7] = (int32_t)h7;
; 998  :     h[8] = (int32_t)h8;
; 999  :     h[9] = (int32_t)h9;

	mov	eax, ecx
	cdq
	mov	DWORD PTR -64+[esp+240], eax
	mov	eax, DWORD PTR _f6_19$[esp+240]
	mov	DWORD PTR -64+[esp+244], edx
	cdq
	mov	DWORD PTR -128+[esp+240], eax
	mov	eax, DWORD PTR _f4_2$[esp+240]
	mov	DWORD PTR -128+[esp+244], edx
	cdq
	mov	DWORD PTR -40+[esp+240], eax
	mov	eax, ebx
	mov	DWORD PTR -40+[esp+244], edx
	cdq
	mov	DWORD PTR -184+[esp+240], eax
	mov	eax, DWORD PTR _f3_2$[esp+240]
	mov	DWORD PTR -184+[esp+244], edx
	cdq
	mov	DWORD PTR -192+[esp+240], eax
	mov	eax, edi
	mov	DWORD PTR -192+[esp+244], edx
	cdq
	mov	DWORD PTR -224+[esp+240], eax
	mov	eax, DWORD PTR _f2_2$[esp+240]
	mov	DWORD PTR -224+[esp+244], edx
	cdq
	shl	esi, 1
	mov	DWORD PTR -112+[esp+240], eax
	mov	DWORD PTR -112+[esp+244], edx
	mov	eax, esi
	cdq
	mov	esi, eax
	mov	eax, DWORD PTR _f1_2$[esp+240]
	mov	edi, edx
	cdq
	mov	DWORD PTR -216+[esp+240], eax
	mov	eax, ebp
	mov	DWORD PTR -216+[esp+244], edx
	cdq
	mov	ebx, eax
	lea	eax, DWORD PTR [ecx+ecx*8]
	mov	ebp, edx
	lea	eax, DWORD PTR [ecx+eax*2]
	mov	ecx, DWORD PTR -64+[esp+244]
	shl	eax, 1
	push	ecx
	mov	ecx, DWORD PTR -64+[esp+244]
	cdq
	push	ecx
	push	edx
	push	eax
	call	__allmul
	push	ebp
	push	ebx
	mov	ecx, eax
	push	ebp
	push	ebx
	mov	ebx, ecx
	mov	ebp, edx
	call	__allmul
	add	ebx, eax
	mov	eax, DWORD PTR -216+[esp+240]
	adc	ebp, edx
	mov	edx, DWORD PTR -216+[esp+244]
	push	edi
	push	esi
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -224+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -112+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -112+[esp+244]
	adc	ebp, edx
	mov	edx, DWORD PTR -224+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -192+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -184+[esp+240]
	adc	ebp, edx
	mov	edx, DWORD PTR -184+[esp+244]
	push	edx
	mov	edx, DWORD PTR -192+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -128+[esp+240]
	add	ebx, eax
	mov	eax, DWORD PTR -128+[esp+244]
	adc	ebp, edx
	mov	edx, DWORD PTR -40+[esp+244]
	push	eax
	mov	eax, DWORD PTR -40+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	add	ebx, eax
	push	0
	adc	ebp, edx
	push	2
	push	ebp
	push	ebx
	call	__allmul
	mov	DWORD PTR _h0$[esp+240], eax
	mov	eax, DWORD PTR _f5_2$[esp+240]
	mov	DWORD PTR _h0$[esp+244], edx
	mov	ecx, DWORD PTR _f6$[esp+240]
	cdq
	mov	DWORD PTR -160+[esp+240], eax
	mov	eax, DWORD PTR _f7$[esp+240]
	mov	DWORD PTR -160+[esp+244], edx
	cdq
	mov	DWORD PTR -16+[esp+240], eax
	mov	eax, DWORD PTR _f0_2$[esp+240]
	mov	DWORD PTR -16+[esp+244], edx
	cdq
	mov	DWORD PTR -208+[esp+240], eax
	mov	eax, DWORD PTR _f4$[esp+240]
	mov	DWORD PTR -208+[esp+244], edx
	cdq
	mov	DWORD PTR -200+[esp+240], eax
	mov	eax, DWORD PTR _f2$[esp+240]
	mov	DWORD PTR -200+[esp+244], edx
	cdq
	mov	ebx, eax
	lea	eax, DWORD PTR [ecx+ecx]
	mov	ecx, DWORD PTR -224+[esp+244]
	mov	ebp, edx
	push	ecx
	mov	ecx, DWORD PTR -224+[esp+244]
	cdq
	push	ecx
	push	edx
	push	eax
	mov	DWORD PTR -72+[esp+256], ebx
	mov	DWORD PTR -72+[esp+260], ebp
	call	__allmul
	push	ebp
	push	ebx
	mov	ecx, eax
	push	ebp
	push	ebx
	mov	ebx, ecx
	mov	ebp, edx
	call	__allmul
	mov	ecx, DWORD PTR -200+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -208+[esp+240]
	adc	ebp, edx
	mov	edx, DWORD PTR -208+[esp+244]
	push	edx
	mov	edx, DWORD PTR -200+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -160+[esp+240]
	add	ebx, eax
	mov	eax, DWORD PTR -160+[esp+244]
	push	edi
	push	esi
	push	eax
	push	ecx
	adc	ebp, edx
	call	__allmul
	mov	ecx, DWORD PTR -16+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -184+[esp+240]
	adc	ebp, edx
	mov	edx, DWORD PTR -184+[esp+244]
	push	edx
	mov	edx, DWORD PTR -16+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -192+[esp+240]
	add	ebx, eax
	mov	eax, DWORD PTR -192+[esp+244]
	push	eax
	push	ecx
	adc	ebp, edx
	mov	edx, DWORD PTR -216+[esp+252]
	mov	eax, DWORD PTR -216+[esp+248]
	push	edx
	push	eax
	call	__allmul
	add	ebx, eax
	push	0
	adc	ebp, edx
	push	2
	push	ebp
	push	ebx
	call	__allmul
	mov	DWORD PTR _h4$[esp+240], eax
	mov	eax, DWORD PTR _h0$[esp+240]
	mov	ecx, DWORD PTR -208+[esp+244]
	add	eax, 33554432				; 02000000H
	mov	DWORD PTR _carry0$[esp+240], eax
	mov	eax, DWORD PTR _h0$[esp+244]
	adc	eax, 0
	mov	DWORD PTR _h4$[esp+244], edx
	mov	DWORD PTR _carry0$[esp+244], eax
	mov	eax, DWORD PTR _f1$[esp+240]
	push	ecx
	mov	ecx, DWORD PTR -208+[esp+244]
	cdq
	push	ecx
	push	edx
	push	eax
	mov	DWORD PTR -144+[esp+256], eax
	mov	DWORD PTR -144+[esp+260], edx
	call	__allmul
	mov	ebp, edx
	mov	edx, DWORD PTR -72+[esp+244]
	mov	ebx, eax
	mov	eax, DWORD PTR -72+[esp+240]
	push	edi
	push	esi
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -184+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -200+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -200+[esp+244]
	adc	ebp, edx
	mov	edx, DWORD PTR -184+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -160+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -128+[esp+240]
	adc	ebp, edx
	mov	edx, DWORD PTR -128+[esp+244]
	push	edx
	mov	edx, DWORD PTR -160+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -192+[esp+240]
	add	ebx, eax
	mov	eax, DWORD PTR -192+[esp+244]
	adc	ebp, edx
	mov	edx, DWORD PTR -224+[esp+244]
	push	eax
	mov	eax, DWORD PTR -224+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	add	ebx, eax
	push	0
	adc	ebp, edx
	push	2
	push	ebp
	push	ebx
	call	__allmul
	mov	ebx, eax
	mov	eax, DWORD PTR _carry0$[esp+240]
	mov	ebp, edx
	mov	edx, DWORD PTR _carry0$[esp+244]
	mov	ecx, 26					; 0000001aH
	call	__allshr
	add	ebx, eax
	mov	eax, DWORD PTR _h4$[esp+240]
	adc	ebp, edx
	add	eax, 33554432				; 02000000H
	mov	DWORD PTR _carry4$[esp+240], eax
	mov	eax, DWORD PTR _h4$[esp+244]
	adc	eax, 0
	mov	ecx, DWORD PTR -112+[esp+244]
	mov	DWORD PTR _carry4$[esp+244], eax
	mov	eax, DWORD PTR _f7_2$[esp+240]
	cdq
	mov	DWORD PTR -88+[esp+240], eax
	mov	eax, DWORD PTR _f6$[esp+240]
	mov	DWORD PTR -88+[esp+244], edx
	mov	DWORD PTR _h1$[esp+240], ebx
	cdq
	mov	ebx, eax
	mov	eax, DWORD PTR _f3$[esp+240]
	mov	DWORD PTR _h1$[esp+244], ebp
	mov	ebp, edx
	push	ecx
	mov	ecx, DWORD PTR -112+[esp+244]
	cdq
	push	ecx
	push	edx
	push	eax
	mov	DWORD PTR -168+[esp+256], ebx
	mov	DWORD PTR -168+[esp+260], ebp
	mov	DWORD PTR -104+[esp+256], eax
	mov	DWORD PTR -104+[esp+260], edx
	call	__allmul
	push	edi
	push	esi
	mov	ecx, eax
	push	ebp
	push	ebx
	mov	ebx, ecx
	mov	ebp, edx
	call	__allmul
	mov	ecx, DWORD PTR -88+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -224+[esp+240]
	adc	ebp, edx
	mov	edx, DWORD PTR -224+[esp+244]
	push	edx
	mov	edx, DWORD PTR -88+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -216+[esp+240]
	add	ebx, eax
	mov	eax, DWORD PTR -216+[esp+244]
	adc	ebp, edx
	mov	edx, DWORD PTR -200+[esp+244]
	push	eax
	mov	eax, DWORD PTR -200+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -64+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -208+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -208+[esp+244]
	adc	ebp, edx
	mov	edx, DWORD PTR -64+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	add	ebx, eax
	adc	ebp, edx
	push	0
	push	2
	push	ebp
	push	ebx
	call	__allmul
	mov	ebx, eax
	mov	eax, DWORD PTR _carry4$[esp+240]
	mov	ebp, edx
	mov	edx, DWORD PTR _carry4$[esp+244]
	mov	ecx, 26					; 0000001aH
	call	__allshr
	add	ebx, eax
	mov	eax, DWORD PTR _h1$[esp+240]
	adc	ebp, edx
	mov	edx, DWORD PTR -208+[esp+244]
	add	eax, 16777216				; 01000000H
	mov	ecx, DWORD PTR -72+[esp+244]
	mov	DWORD PTR _carry1$[esp+240], eax
	mov	eax, DWORD PTR _h1$[esp+244]
	adc	eax, 0
	push	edx
	mov	edx, DWORD PTR -72+[esp+244]
	mov	DWORD PTR _carry1$[esp+248], eax
	mov	eax, DWORD PTR -208+[esp+244]
	mov	DWORD PTR _h5$[esp+244], ebx
	push	eax
	push	ecx
	push	edx
	mov	DWORD PTR _h5$[esp+260], ebp
	call	__allmul
	mov	ecx, DWORD PTR -216+[esp+240]
	mov	ebx, eax
	mov	eax, DWORD PTR -216+[esp+244]
	mov	ebp, edx
	mov	edx, DWORD PTR -144+[esp+244]
	push	eax
	mov	eax, DWORD PTR -144+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -128+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -168+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -168+[esp+244]
	adc	ebp, edx
	mov	edx, DWORD PTR -128+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -160+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -184+[esp+240]
	adc	ebp, edx
	mov	edx, DWORD PTR -184+[esp+244]
	push	edx
	mov	edx, DWORD PTR -160+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -192+[esp+240]
	add	ebx, eax
	mov	eax, DWORD PTR -192+[esp+244]
	push	eax
	push	ecx
	push	edi
	push	esi
	adc	ebp, edx
	call	__allmul
	mov	ecx, DWORD PTR -224+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -40+[esp+240]
	adc	ebp, edx
	mov	edx, DWORD PTR -40+[esp+244]
	push	edx
	mov	edx, DWORD PTR -224+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	add	ebx, eax
	push	0
	adc	ebp, edx
	push	2
	push	ebp
	push	ebx
	call	__allmul
	mov	ebx, eax
	mov	eax, DWORD PTR _carry1$[esp+240]
	mov	ebp, edx
	mov	edx, DWORD PTR _carry1$[esp+244]
	mov	ecx, 25					; 00000019H
	call	__allshr
	add	ebx, eax
	mov	eax, DWORD PTR _h5$[esp+240]
	adc	ebp, edx
	add	eax, 16777216				; 01000000H
	mov	DWORD PTR _carry5$[esp+240], eax
	mov	eax, DWORD PTR _h5$[esp+244]
	adc	eax, 0
	mov	ecx, DWORD PTR -208+[esp+240]
	mov	DWORD PTR _carry5$[esp+244], eax
	mov	eax, DWORD PTR _f8$[esp+240]
	cdq
	mov	DWORD PTR _h2$[esp+240], ebx
	mov	ebx, eax
	mov	eax, DWORD PTR -208+[esp+244]
	mov	DWORD PTR _h2$[esp+244], ebp
	mov	ebp, edx
	mov	edx, DWORD PTR -168+[esp+244]
	push	eax
	mov	eax, DWORD PTR -168+[esp+244]
	push	ecx
	push	edx
	push	eax
	mov	DWORD PTR -128+[esp+256], ebx
	mov	DWORD PTR -128+[esp+260], ebp
	call	__allmul
	mov	ecx, eax
	mov	eax, DWORD PTR -224+[esp+244]
	push	eax
	mov	eax, DWORD PTR -224+[esp+244]
	push	eax
	push	ebp
	push	ebx
	mov	ebx, ecx
	mov	ebp, edx
	call	__allmul
	mov	ecx, DWORD PTR -88+[esp+244]
	add	ebx, eax
	adc	ebp, edx
	mov	edx, DWORD PTR -88+[esp+240]
	push	edi
	push	esi
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -192+[esp+240]
	add	ebx, eax
	mov	eax, DWORD PTR -192+[esp+244]
	adc	ebp, edx
	mov	edx, DWORD PTR -104+[esp+244]
	push	eax
	mov	eax, DWORD PTR -104+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -112+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -200+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -200+[esp+244]
	adc	ebp, edx
	mov	edx, DWORD PTR -112+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -160+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -216+[esp+240]
	adc	ebp, edx
	mov	edx, DWORD PTR -216+[esp+244]
	push	edx
	mov	edx, DWORD PTR -160+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	add	ebx, eax
	push	0
	adc	ebp, edx
	push	2
	push	ebp
	push	ebx
	call	__allmul
	mov	ebx, eax
	mov	eax, DWORD PTR _carry5$[esp+240]
	mov	ebp, edx
	mov	edx, DWORD PTR _carry5$[esp+244]
	mov	ecx, 25					; 00000019H
	call	__allshr
	add	ebx, eax
	mov	eax, DWORD PTR _h2$[esp+240]
	adc	ebp, edx
	add	eax, 33554432				; 02000000H
	mov	ecx, DWORD PTR -208+[esp+240]
	mov	DWORD PTR _carry2$[esp+240], eax
	mov	eax, DWORD PTR _h2$[esp+244]
	mov	edx, DWORD PTR -104+[esp+244]
	adc	eax, 0
	mov	DWORD PTR _h6$[esp+240], ebx
	mov	DWORD PTR _carry2$[esp+244], eax
	mov	eax, DWORD PTR -208+[esp+244]
	push	eax
	mov	eax, DWORD PTR -104+[esp+244]
	push	ecx
	push	edx
	push	eax
	mov	DWORD PTR _h6$[esp+260], ebp
	call	__allmul
	mov	ecx, DWORD PTR -184+[esp+244]
	mov	ebp, edx
	mov	edx, DWORD PTR -184+[esp+240]
	mov	ebx, eax
	mov	eax, DWORD PTR -168+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -168+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -72+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -216+[esp+240]
	adc	ebp, edx
	mov	edx, DWORD PTR -216+[esp+244]
	push	edx
	mov	edx, DWORD PTR -72+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -200+[esp+240]
	add	ebx, eax
	mov	eax, DWORD PTR -200+[esp+244]
	push	edi
	push	esi
	push	eax
	push	ecx
	adc	ebp, edx
	call	__allmul
	add	ebx, eax
	mov	eax, DWORD PTR -224+[esp+240]
	adc	ebp, edx
	mov	edx, DWORD PTR -224+[esp+244]
	push	edx
	mov	ecx, DWORD PTR -160+[esp+248]
	mov	edx, DWORD PTR -160+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	add	ebx, eax
	push	0
	adc	ebp, edx
	push	2
	push	ebp
	push	ebx
	call	__allmul
	mov	ebx, DWORD PTR _carry2$[esp+240]
	mov	ebp, DWORD PTR _carry2$[esp+244]
	mov	DWORD PTR -224+[esp+240], eax
	mov	DWORD PTR -224+[esp+244], edx
	mov	ecx, 26					; 0000001aH
	mov	eax, ebx
	mov	edx, ebp
	call	__allshr
	mov	ecx, DWORD PTR -224+[esp+240]
	push	edi
	add	ecx, eax
	mov	eax, DWORD PTR -224+[esp+248]
	adc	eax, edx
	mov	edx, DWORD PTR _h2$[esp+244]
	mov	DWORD PTR _h3$[esp+244], ecx
	mov	ecx, DWORD PTR _kTop38Bits
	mov	DWORD PTR _h3$[esp+248], eax
	mov	eax, DWORD PTR _kTop38Bits+4
	and	ebx, ecx
	and	ebp, eax
	mov	eax, DWORD PTR _h2$[esp+248]
	sub	edx, ebx
	mov	ecx, DWORD PTR -128+[esp+248]
	mov	DWORD PTR _h2$[esp+244], edx
	sbb	eax, ebp
	mov	edx, DWORD PTR -128+[esp+244]
	mov	eax, DWORD PTR _h6$[esp+244]
	push	esi
	add	eax, 33554432				; 02000000H
	push	ecx
	mov	DWORD PTR _carry6$[esp+252], eax
	mov	eax, DWORD PTR _h6$[esp+256]
	adc	eax, 0
	push	edx
	mov	DWORD PTR _carry6$[esp+260], eax
	call	__allmul
	mov	ecx, DWORD PTR -16+[esp+240]
	mov	ebx, eax
	mov	eax, DWORD PTR -16+[esp+244]
	mov	ebp, edx
	mov	edx, DWORD PTR -208+[esp+244]
	push	eax
	mov	eax, DWORD PTR -208+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -216+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -168+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -168+[esp+244]
	adc	ebp, edx
	mov	edx, DWORD PTR -216+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -200+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR -192+[esp+240]
	adc	ebp, edx
	mov	edx, DWORD PTR -192+[esp+244]
	push	edx
	mov	edx, DWORD PTR -200+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -64+[esp+240]
	add	ebx, eax
	mov	eax, DWORD PTR -64+[esp+244]
	adc	ebp, edx
	mov	edx, DWORD PTR -112+[esp+244]
	push	eax
	mov	eax, DWORD PTR -112+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	add	ebx, eax
	push	0
	adc	ebp, edx
	push	2
	push	ebp
	push	ebx
	call	__allmul
	mov	ebp, DWORD PTR _carry6$[esp+240]
	mov	ebx, eax
	mov	DWORD PTR -104+[esp+244], edx
	mov	edx, DWORD PTR _carry6$[esp+244]
	mov	ecx, 26					; 0000001aH
	mov	eax, ebp
	call	__allshr
	mov	ecx, DWORD PTR -104+[esp+244]
	add	ebx, eax
	mov	eax, DWORD PTR _carry6$[esp+244]
	mov	DWORD PTR _h7$[esp+240], ebx
	mov	ebx, DWORD PTR _kTop38Bits
	adc	ecx, edx
	mov	edx, DWORD PTR _h6$[esp+240]
	mov	DWORD PTR _h7$[esp+244], ecx
	mov	ecx, DWORD PTR _kTop38Bits+4
	and	ebp, ebx
	and	eax, ecx
	sub	edx, ebp
	mov	ebp, DWORD PTR _h3$[esp+240]
	mov	DWORD PTR _h6$[esp+240], edx
	mov	edx, DWORD PTR _h6$[esp+244]
	sbb	edx, eax
	mov	eax, DWORD PTR _carry4$[esp+240]
	mov	edx, DWORD PTR _h3$[esp+244]
	add	ebp, 16777216				; 01000000H
	adc	edx, 0
	and	eax, ebx
	mov	DWORD PTR -224+[esp+240], eax
	mov	eax, DWORD PTR _carry4$[esp+244]
	and	eax, ecx
	mov	ecx, 25					; 00000019H
	mov	DWORD PTR -224+[esp+244], eax
	mov	eax, ebp
	mov	DWORD PTR _carry3$[esp+244], edx
	call	__allshr
	mov	ecx, DWORD PTR -224+[esp+240]
	sub	eax, ecx
	mov	ecx, DWORD PTR -224+[esp+244]
	sbb	edx, ecx
	mov	ecx, DWORD PTR _h4$[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR _kTop39Bits
	mov	DWORD PTR _h4$[esp+240], ecx
	mov	ecx, DWORD PTR _h4$[esp+244]
	adc	ecx, edx
	mov	edx, DWORD PTR _kTop39Bits+4
	and	ebp, eax
	mov	eax, DWORD PTR _carry3$[esp+244]
	mov	DWORD PTR _h4$[esp+244], ecx
	mov	ecx, DWORD PTR _h3$[esp+240]
	and	eax, edx
	sub	ecx, ebp
	mov	ebp, DWORD PTR _h7$[esp+240]
	mov	DWORD PTR _h3$[esp+240], ecx
	mov	ecx, DWORD PTR _h3$[esp+244]
	sbb	ecx, eax
	mov	eax, DWORD PTR _h7$[esp+244]
	add	ebp, 16777216				; 01000000H
	adc	eax, 0
	mov	DWORD PTR _carry7$[esp+244], eax
	mov	eax, DWORD PTR _f9$[esp+240]
	cdq
	push	edi
	push	esi
	push	edx
	push	eax
	mov	DWORD PTR -224+[esp+256], eax
	mov	DWORD PTR -224+[esp+260], edx
	call	__allmul
	mov	ecx, DWORD PTR -208+[esp+244]
	mov	edi, edx
	mov	edx, DWORD PTR -208+[esp+240]
	mov	esi, eax
	mov	eax, DWORD PTR -128+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -128+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	mov	ecx, DWORD PTR -200+[esp+244]
	add	esi, eax
	mov	eax, DWORD PTR -200+[esp+240]
	push	ecx
	push	eax
	push	ecx
	push	eax
	adc	edi, edx
	call	__allmul
	mov	ecx, DWORD PTR -88+[esp+244]
	add	esi, eax
	mov	eax, DWORD PTR -216+[esp+240]
	adc	edi, edx
	mov	edx, DWORD PTR -216+[esp+244]
	push	edx
	mov	edx, DWORD PTR -88+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -112+[esp+240]
	add	esi, eax
	mov	eax, DWORD PTR -112+[esp+244]
	adc	edi, edx
	mov	edx, DWORD PTR -168+[esp+244]
	push	eax
	mov	eax, DWORD PTR -168+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -192+[esp+244]
	add	esi, eax
	mov	eax, DWORD PTR -160+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -160+[esp+244]
	adc	edi, edx
	mov	edx, DWORD PTR -192+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	add	esi, eax
	push	0
	adc	edi, edx
	push	2
	push	edi
	push	esi
	call	__allmul
	mov	esi, DWORD PTR _carry7$[esp+244]
	mov	edi, eax
	mov	DWORD PTR -200+[esp+244], edx
	mov	ecx, 25					; 00000019H
	mov	eax, ebp
	mov	edx, esi
	call	__allshr
	mov	ecx, DWORD PTR _kTop39Bits
	add	edi, eax
	mov	eax, DWORD PTR -200+[esp+244]
	mov	DWORD PTR _h8$[esp+240], edi
	adc	eax, edx
	mov	DWORD PTR _h8$[esp+244], eax
	mov	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h7$[esp+240]
	and	ebp, ecx
	and	esi, edx
	sub	eax, ebp
	mov	ebp, DWORD PTR _h4$[esp+244]
	mov	DWORD PTR _h7$[esp+240], eax
	mov	eax, DWORD PTR _h7$[esp+244]
	sbb	eax, esi
	mov	esi, DWORD PTR _h4$[esp+240]
	mov	eax, DWORD PTR _carry5$[esp+240]
	add	esi, 33554432				; 02000000H
	adc	ebp, 0
	and	eax, ecx
	mov	ecx, DWORD PTR _carry5$[esp+244]
	mov	DWORD PTR -136+[esp+240], eax
	and	ecx, edx
	mov	eax, esi
	mov	DWORD PTR -136+[esp+244], ecx
	mov	ecx, 26					; 0000001aH
	mov	edx, ebp
	call	__allshr
	mov	ecx, DWORD PTR -136+[esp+240]
	sub	eax, ecx
	mov	ecx, DWORD PTR -136+[esp+244]
	sbb	edx, ecx
	mov	ecx, DWORD PTR _h5$[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR _h5$[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR _h4$[esp+240]
	mov	eax, DWORD PTR _kTop38Bits+4
	and	esi, ebx
	and	ebp, eax
	mov	eax, DWORD PTR _h4$[esp+244]
	sub	edx, esi
	mov	esi, edi
	sbb	eax, ebp
	add	esi, 33554432				; 02000000H
	mov	eax, DWORD PTR _h8$[esp+244]
	mov	DWORD PTR _h4$[esp+240], edx
	mov	edx, DWORD PTR -208+[esp+244]
	mov	DWORD PTR _h5$[esp+240], ecx
	mov	ecx, DWORD PTR -224+[esp+244]
	push	edx
	mov	edx, DWORD PTR -224+[esp+244]
	adc	eax, 0
	mov	DWORD PTR _carry8$[esp+248], eax
	mov	eax, DWORD PTR -208+[esp+244]
	push	eax
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -216+[esp+240]
	mov	edi, eax
	mov	eax, DWORD PTR -216+[esp+244]
	mov	ebp, edx
	mov	edx, DWORD PTR -128+[esp+244]
	push	eax
	mov	eax, DWORD PTR -128+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR -192+[esp+244]
	add	edi, eax
	mov	eax, DWORD PTR -168+[esp+244]
	push	ecx
	mov	ecx, DWORD PTR -168+[esp+244]
	adc	ebp, edx
	mov	edx, DWORD PTR -192+[esp+244]
	push	edx
	push	eax
	push	ecx
	call	__allmul
	add	edi, eax
	mov	eax, DWORD PTR -112+[esp+240]
	adc	ebp, edx
	mov	edx, DWORD PTR -112+[esp+244]
	push	edx
	push	eax
	mov	ecx, DWORD PTR -16+[esp+252]
	mov	edx, DWORD PTR -16+[esp+248]
	push	ecx
	push	edx
	call	__allmul
	mov	ecx, DWORD PTR -64+[esp+240]
	add	edi, eax
	mov	eax, DWORD PTR -64+[esp+244]
	adc	ebp, edx
	mov	edx, DWORD PTR -40+[esp+244]
	push	eax
	mov	eax, DWORD PTR -40+[esp+244]
	push	ecx
	push	edx
	push	eax
	call	__allmul
	add	edi, eax
	push	0
	adc	ebp, edx
	push	2
	push	ebp
	push	edi
	call	__allmul
	mov	edi, DWORD PTR _carry8$[esp+244]
	mov	ebp, eax
	mov	DWORD PTR -136+[esp+244], edx
	mov	ecx, 26					; 0000001aH
	mov	eax, esi
	mov	edx, edi
	call	__allshr
	mov	ecx, DWORD PTR _h8$[esp+240]
	add	ebp, eax
	mov	eax, DWORD PTR -136+[esp+244]
	adc	eax, edx
	mov	edx, DWORD PTR _kTop38Bits+4
	and	esi, ebx
	and	edi, edx
	sub	ecx, esi
	mov	esi, ebp
	mov	DWORD PTR _h8$[esp+240], ecx
	mov	ecx, DWORD PTR _h8$[esp+244]
	sbb	ecx, edi
	add	esi, 16777216				; 01000000H
	mov	edi, eax
	mov	DWORD PTR _h9$[esp+244], eax
	adc	edi, 0
	mov	ecx, 25					; 00000019H
	mov	eax, esi
	mov	edx, edi
	call	__allshr
	push	0
	push	19					; 00000013H
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR _carry0$[esp+240]
	and	ecx, ebx
	mov	DWORD PTR -144+[esp+240], ecx
	mov	ecx, DWORD PTR _carry0$[esp+244]
	and	ecx, DWORD PTR _kTop38Bits+4
	mov	DWORD PTR -144+[esp+244], ecx
	mov	ecx, DWORD PTR -144+[esp+240]
	sub	eax, ecx
	mov	ecx, DWORD PTR -144+[esp+244]
	sbb	edx, ecx
	mov	ecx, DWORD PTR _h0$[esp+240]
	add	ecx, eax
	mov	eax, DWORD PTR _h0$[esp+244]
	mov	DWORD PTR _h0$[esp+240], ecx
	mov	ecx, DWORD PTR _kTop39Bits
	adc	eax, edx
	mov	edx, DWORD PTR _kTop39Bits+4
	and	esi, ecx
	and	edi, edx
	sub	ebp, esi
	mov	esi, DWORD PTR _h9$[esp+244]
	sbb	esi, edi
	mov	esi, DWORD PTR _h0$[esp+240]
	add	esi, 33554432				; 02000000H
	adc	eax, 0
	mov	DWORD PTR _carry0$[esp+244], eax
	mov	eax, DWORD PTR _carry1$[esp+244]
	mov	edi, DWORD PTR _carry1$[esp+240]
	and	eax, edx
	mov	edx, DWORD PTR _carry0$[esp+244]
	and	edi, ecx
	mov	DWORD PTR -24+[esp+244], eax
	mov	ecx, 26					; 0000001aH
	mov	eax, esi
	call	__allshr
	mov	ecx, edx
	mov	edx, DWORD PTR -24+[esp+244]
	sub	eax, edi
	pop	edi
	sbb	ecx, edx
	mov	edx, DWORD PTR _h1$[esp+236]
	add	edx, eax
	mov	eax, DWORD PTR _h1$[esp+240]
	adc	eax, ecx
	mov	ecx, DWORD PTR _h0$[esp+236]
	mov	eax, DWORD PTR _h$[esp+232]
	and	esi, ebx
	sub	ecx, esi
	pop	esi
	mov	DWORD PTR [eax+4], edx
	mov	edx, DWORD PTR _h2$[esp+232]
	mov	DWORD PTR [eax], ecx
	mov	ecx, DWORD PTR _h3$[esp+232]
	mov	DWORD PTR [eax+8], edx
	mov	edx, DWORD PTR _h4$[esp+232]
	mov	DWORD PTR [eax+12], ecx
	mov	ecx, DWORD PTR _h5$[esp+232]
	mov	DWORD PTR [eax+16], edx
	mov	edx, DWORD PTR _h6$[esp+232]
	mov	DWORD PTR [eax+20], ecx
	mov	ecx, DWORD PTR _h7$[esp+232]
	mov	DWORD PTR [eax+24], edx
	mov	edx, DWORD PTR _h8$[esp+232]
	mov	DWORD PTR [eax+36], ebp
	pop	ebp
	mov	DWORD PTR [eax+28], ecx
	mov	DWORD PTR [eax+32], edx
	pop	ebx

; 1000 : }

	add	esp, 224				; 000000e0H
	ret	0
_fe_sq2	ENDP
_TEXT	ENDS
;	COMDAT _ge_p3_dbl
_TEXT	SEGMENT
_r$ = 8
_p$ = 12
_q$ = -120
_ge_p3_dbl PROC NEAR					; COMDAT

; 1194 :     ge_p2 q;
; 1195 :     ge_p3_to_p2(&q, p);

	mov	eax, DWORD PTR _p$[esp-4]
	sub	esp, 120				; 00000078H
	lea	ecx, DWORD PTR _q$[esp+120]
	push	eax
	push	ecx
	call	_ge_p3_to_p2

; 1196 :     ge_p2_dbl(r, &q);

	mov	eax, DWORD PTR _r$[esp+124]
	lea	edx, DWORD PTR _q$[esp+128]
	push	edx
	push	eax
	call	_ge_p2_dbl

; 1197 : }

	add	esp, 136				; 00000088H
	ret	0
_ge_p3_dbl ENDP
_TEXT	ENDS
;	COMDAT _ge_p3_to_p2
_TEXT	SEGMENT
_r$ = 8
_p$ = 12
_ge_p3_to_p2 PROC NEAR					; COMDAT

; 1143 : {

	push	esi

; 1144 :     fe_copy(r->X, p->X);

	mov	esi, DWORD PTR _p$[esp]
	push	edi
	mov	edi, DWORD PTR _r$[esp+4]
	push	esi
	push	edi
	call	_fe_copy

; 1145 :     fe_copy(r->Y, p->Y);

	lea	eax, DWORD PTR [esi+40]
	lea	ecx, DWORD PTR [edi+40]
	push	eax
	push	ecx
	call	_fe_copy

; 1146 :     fe_copy(r->Z, p->Z);

	add	esi, 80					; 00000050H
	add	edi, 80					; 00000050H
	push	esi
	push	edi
	call	_fe_copy
	add	esp, 24					; 00000018H
	pop	edi
	pop	esi

; 1147 : }

	ret	0
_ge_p3_to_p2 ENDP
_TEXT	ENDS
;	COMDAT _ge_madd
_TEXT	SEGMENT
_r$ = 8
_p$ = 12
_q$ = 16
_t0$ = -40
_ge_madd PROC NEAR					; COMDAT

; 1200 : {

	sub	esp, 40					; 00000028H
	push	ebx

; 1201 :     fe t0;
; 1202 : 
; 1203 :     fe_add(r->X, p->Y, p->X);

	mov	ebx, DWORD PTR _p$[esp+40]
	push	ebp
	push	esi
	push	edi
	mov	edi, DWORD PTR _r$[esp+52]
	lea	ebp, DWORD PTR [ebx+40]
	push	ebx
	push	ebp
	push	edi
	call	_fe_add

; 1204 :     fe_sub(r->Y, p->Y, p->X);

	lea	esi, DWORD PTR [edi+40]
	push	ebx
	push	ebp
	push	esi
	call	_fe_sub

; 1205 :     fe_mul(r->Z, r->X, q->yplusx);

	mov	eax, DWORD PTR _q$[esp+76]
	lea	ebp, DWORD PTR [edi+80]
	push	eax
	push	edi
	push	ebp
	call	_fe_mul

; 1206 :     fe_mul(r->Y, r->Y, q->yminusx);

	mov	ecx, DWORD PTR _q$[esp+88]
	add	ecx, 40					; 00000028H
	push	ecx
	push	esi
	push	esi
	call	_fe_mul

; 1207 :     fe_mul(r->T, q->xy2d, p->T);

	mov	ecx, DWORD PTR _q$[esp+100]
	lea	edx, DWORD PTR [ebx+120]
	add	ecx, 80					; 00000050H
	lea	eax, DWORD PTR [edi+120]
	push	edx
	push	ecx
	push	eax
	mov	DWORD PTR 12+[esp+112], eax
	call	_fe_mul

; 1208 :     fe_add(t0, p->Z, p->Z);

	add	ebx, 80					; 00000050H
	lea	edx, DWORD PTR _t0$[esp+116]
	push	ebx
	push	ebx
	push	edx
	call	_fe_add
	add	esp, 72					; 00000048H

; 1209 :     fe_sub(r->X, r->Z, r->Y);

	push	esi
	push	ebp
	push	edi
	call	_fe_sub

; 1210 :     fe_add(r->Y, r->Z, r->Y);

	push	esi
	push	ebp
	push	esi
	call	_fe_add

; 1211 :     fe_add(r->Z, t0, r->T);

	mov	esi, DWORD PTR 12+[esp+76]
	lea	eax, DWORD PTR _t0$[esp+80]
	push	esi
	push	eax
	push	ebp
	call	_fe_add

; 1212 :     fe_sub(r->T, t0, r->T);

	lea	ecx, DWORD PTR _t0$[esp+92]
	push	esi
	push	ecx
	push	esi
	call	_fe_sub
	add	esp, 48					; 00000030H
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 1213 : }

	add	esp, 40					; 00000028H
	ret	0
_ge_madd ENDP
_TEXT	ENDS
;	COMDAT _ge_msub
_TEXT	SEGMENT
_r$ = 8
_p$ = 12
_q$ = 16
_t0$ = -40
_ge_msub PROC NEAR					; COMDAT

; 1216 : {

	sub	esp, 40					; 00000028H
	push	ebx

; 1217 :     fe t0;
; 1218 : 
; 1219 :     fe_add(r->X, p->Y, p->X);

	mov	ebx, DWORD PTR _p$[esp+40]
	push	ebp
	push	esi
	push	edi
	mov	edi, DWORD PTR _r$[esp+52]
	lea	ebp, DWORD PTR [ebx+40]
	push	ebx
	push	ebp
	push	edi
	call	_fe_add

; 1220 :     fe_sub(r->Y, p->Y, p->X);

	lea	esi, DWORD PTR [edi+40]
	push	ebx
	push	ebp
	push	esi
	call	_fe_sub

; 1221 :     fe_mul(r->Z, r->X, q->yminusx);

	mov	eax, DWORD PTR _q$[esp+76]
	lea	ebp, DWORD PTR [edi+80]
	add	eax, 40					; 00000028H
	push	eax
	push	edi
	push	ebp
	call	_fe_mul

; 1222 :     fe_mul(r->Y, r->Y, q->yplusx);

	mov	ecx, DWORD PTR _q$[esp+88]
	push	ecx
	push	esi
	push	esi
	call	_fe_mul

; 1223 :     fe_mul(r->T, q->xy2d, p->T);

	mov	ecx, DWORD PTR _q$[esp+100]
	lea	edx, DWORD PTR [ebx+120]
	add	ecx, 80					; 00000050H
	lea	eax, DWORD PTR [edi+120]
	push	edx
	push	ecx
	push	eax
	mov	DWORD PTR 12+[esp+112], eax
	call	_fe_mul

; 1224 :     fe_add(t0, p->Z, p->Z);

	add	ebx, 80					; 00000050H
	lea	edx, DWORD PTR _t0$[esp+116]
	push	ebx
	push	ebx
	push	edx
	call	_fe_add
	add	esp, 72					; 00000048H

; 1225 :     fe_sub(r->X, r->Z, r->Y);

	push	esi
	push	ebp
	push	edi
	call	_fe_sub

; 1226 :     fe_add(r->Y, r->Z, r->Y);

	push	esi
	push	ebp
	push	esi
	call	_fe_add

; 1227 :     fe_sub(r->Z, t0, r->T);

	mov	esi, DWORD PTR 12+[esp+76]
	lea	eax, DWORD PTR _t0$[esp+80]
	push	esi
	push	eax
	push	ebp
	call	_fe_sub

; 1228 :     fe_add(r->T, t0, r->T);

	lea	ecx, DWORD PTR _t0$[esp+92]
	push	esi
	push	ecx
	push	esi
	call	_fe_add
	add	esp, 48					; 00000030H
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 1229 : }

	add	esp, 40					; 00000028H
	ret	0
_ge_msub ENDP
_TEXT	ENDS
;	COMDAT _ge_add
_TEXT	SEGMENT
_r$ = 8
_p$ = 12
_q$ = 16
_t0$ = -40
_ge_add	PROC NEAR					; COMDAT

; 1232 : {

	sub	esp, 40					; 00000028H
	push	ebx

; 1233 :     fe t0;
; 1234 : 
; 1235 :     fe_add(r->X, p->Y, p->X);

	mov	ebx, DWORD PTR _p$[esp+40]
	push	ebp
	push	esi
	mov	esi, DWORD PTR _r$[esp+48]
	push	edi
	lea	ebp, DWORD PTR [ebx+40]
	push	ebx
	push	ebp
	push	esi
	call	_fe_add

; 1236 :     fe_sub(r->Y, p->Y, p->X);

	lea	edi, DWORD PTR [esi+40]
	push	ebx
	push	ebp
	push	edi
	call	_fe_sub

; 1237 :     fe_mul(r->Z, r->X, q->YplusX);

	mov	eax, DWORD PTR _q$[esp+76]
	lea	ebp, DWORD PTR [esi+80]
	push	eax
	push	esi
	push	ebp
	call	_fe_mul

; 1238 :     fe_mul(r->Y, r->Y, q->YminusX);

	mov	ecx, DWORD PTR _q$[esp+88]
	add	ecx, 40					; 00000028H
	push	ecx
	push	edi
	push	edi
	call	_fe_mul

; 1239 :     fe_mul(r->T, q->T2d, p->T);

	mov	ecx, DWORD PTR _q$[esp+100]
	lea	edx, DWORD PTR [ebx+120]
	add	ecx, 120				; 00000078H
	lea	eax, DWORD PTR [esi+120]
	push	edx
	push	ecx
	push	eax
	mov	DWORD PTR 12+[esp+112], eax
	call	_fe_mul

; 1240 :     fe_mul(r->X, p->Z, q->Z);

	mov	edx, DWORD PTR _q$[esp+112]
	add	ebx, 80					; 00000050H
	add	edx, 80					; 00000050H
	push	edx
	push	ebx
	push	esi
	call	_fe_mul
	add	esp, 72					; 00000048H

; 1241 :     fe_add(t0, r->X, r->X);

	lea	eax, DWORD PTR _t0$[esp+56]
	push	esi
	push	esi
	push	eax
	call	_fe_add

; 1242 :     fe_sub(r->X, r->Z, r->Y);

	push	edi
	push	ebp
	push	esi
	call	_fe_sub

; 1243 :     fe_add(r->Y, r->Z, r->Y);

	push	edi
	push	ebp
	push	edi
	call	_fe_add

; 1244 :     fe_add(r->Z, t0, r->T);

	mov	esi, DWORD PTR 12+[esp+88]
	lea	ecx, DWORD PTR _t0$[esp+92]
	push	esi
	push	ecx
	push	ebp
	call	_fe_add

; 1245 :     fe_sub(r->T, t0, r->T);

	lea	edx, DWORD PTR _t0$[esp+104]
	push	esi
	push	edx
	push	esi
	call	_fe_sub
	add	esp, 60					; 0000003cH
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 1246 : }

	add	esp, 40					; 00000028H
	ret	0
_ge_add	ENDP
_TEXT	ENDS
;	COMDAT _ge_sub
_TEXT	SEGMENT
_r$ = 8
_p$ = 12
_q$ = 16
_t0$ = -40
_ge_sub	PROC NEAR					; COMDAT

; 1249 : {

	sub	esp, 40					; 00000028H
	push	ebx

; 1250 :     fe t0;
; 1251 : 
; 1252 :     fe_add(r->X, p->Y, p->X);

	mov	ebx, DWORD PTR _p$[esp+40]
	push	ebp
	push	esi
	mov	esi, DWORD PTR _r$[esp+48]
	push	edi
	lea	ebp, DWORD PTR [ebx+40]
	push	ebx
	push	ebp
	push	esi
	call	_fe_add

; 1253 :     fe_sub(r->Y, p->Y, p->X);

	lea	edi, DWORD PTR [esi+40]
	push	ebx
	push	ebp
	push	edi
	call	_fe_sub

; 1254 :     fe_mul(r->Z, r->X, q->YminusX);

	mov	eax, DWORD PTR _q$[esp+76]
	lea	ebp, DWORD PTR [esi+80]
	add	eax, 40					; 00000028H
	push	eax
	push	esi
	push	ebp
	call	_fe_mul

; 1255 :     fe_mul(r->Y, r->Y, q->YplusX);

	mov	ecx, DWORD PTR _q$[esp+88]
	push	ecx
	push	edi
	push	edi
	call	_fe_mul

; 1256 :     fe_mul(r->T, q->T2d, p->T);

	mov	ecx, DWORD PTR _q$[esp+100]
	lea	edx, DWORD PTR [ebx+120]
	add	ecx, 120				; 00000078H
	lea	eax, DWORD PTR [esi+120]
	push	edx
	push	ecx
	push	eax
	mov	DWORD PTR 12+[esp+112], eax
	call	_fe_mul

; 1257 :     fe_mul(r->X, p->Z, q->Z);

	mov	edx, DWORD PTR _q$[esp+112]
	add	ebx, 80					; 00000050H
	add	edx, 80					; 00000050H
	push	edx
	push	ebx
	push	esi
	call	_fe_mul
	add	esp, 72					; 00000048H

; 1258 :     fe_add(t0, r->X, r->X);

	lea	eax, DWORD PTR _t0$[esp+56]
	push	esi
	push	esi
	push	eax
	call	_fe_add

; 1259 :     fe_sub(r->X, r->Z, r->Y);

	push	edi
	push	ebp
	push	esi
	call	_fe_sub

; 1260 :     fe_add(r->Y, r->Z, r->Y);

	push	edi
	push	ebp
	push	edi
	call	_fe_add

; 1261 :     fe_sub(r->Z, t0, r->T);

	mov	esi, DWORD PTR 12+[esp+88]
	lea	ecx, DWORD PTR _t0$[esp+92]
	push	esi
	push	ecx
	push	ebp
	call	_fe_sub

; 1262 :     fe_add(r->T, t0, r->T);

	lea	edx, DWORD PTR _t0$[esp+104]
	push	esi
	push	edx
	push	esi
	call	_fe_add
	add	esp, 60					; 0000003cH
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 1263 : }

	add	esp, 40					; 00000028H
	ret	0
_ge_sub	ENDP
_TEXT	ENDS
;	COMDAT _slide
_TEXT	SEGMENT
_r$ = 8
_a$ = 12
_i$ = 12
_slide	PROC NEAR					; COMDAT

; 1266 : {

	push	ebx
	push	ebp
	push	esi

; 1267 :     int i;
; 1268 :     int b;
; 1269 :     int k;
; 1270 : 
; 1271 :     for (i = 0; i < 256; ++i) {

	mov	esi, DWORD PTR _a$[esp+8]
	push	edi
	mov	edi, DWORD PTR _r$[esp+12]
	xor	eax, eax
$L1903:

; 1272 :         r[i] = (signed char)(1 & (a[i >> 3] >> (i & 7)));

	mov	edx, eax
	mov	cl, al
	sar	edx, 3
	and	cl, 7
	mov	dl, BYTE PTR [edx+esi]
	shr	dl, cl
	and	dl, 1
	mov	BYTE PTR [eax+edi], dl
	inc	eax
	cmp	eax, 256				; 00000100H
	jl	SHORT $L1903

; 1273 :     }
; 1274 : 
; 1275 :     for (i = 0; i < 256; ++i) {

	mov	DWORD PTR _i$[esp+12], 0
$L1907:

; 1276 :         if (r[i]) {

	cmp	BYTE PTR [edi], 0
	je	SHORT $L1908

; 1277 :             for (b = 1; b <= 6 && i + b < 256; ++b) {

	mov	ecx, 1
$L1911:
	mov	eax, DWORD PTR _i$[esp+12]
	add	eax, ecx
	cmp	eax, 256				; 00000100H
	jge	SHORT $L1908

; 1278 :                 if (r[i + b]) {

	mov	bl, BYTE PTR [edi+ecx]
	test	bl, bl
	je	SHORT $L1912

; 1279 :                     if (r[i] + (r[i + b] << b) <= 15) {

	mov	dl, BYTE PTR [edi]
	movsx	ebx, bl
	shl	ebx, cl
	movsx	esi, dl
	add	ebx, esi
	cmp	ebx, 15					; 0000000fH
	jg	SHORT $L1915

; 1280 :                         r[i] = (signed char)(r[i] + (r[i + b] << b));

	mov	al, BYTE PTR [edi+ecx]
	shl	al, cl
	add	al, dl
	mov	BYTE PTR [edi], al

; 1281 :                         r[i + b] = 0;

	mov	BYTE PTR [edi+ecx], 0

; 1282 :                     } else if (r[i] - (r[i + b] << b) >= -15) {

	jmp	SHORT $L1912
$L1915:
	mov	bl, BYTE PTR [edi+ecx]
	movsx	ebp, bl
	shl	ebp, cl
	sub	esi, ebp
	cmp	esi, -15				; fffffff1H
	jl	SHORT $L1908

; 1283 :                         r[i] = (signed char)(r[i] - (r[i + b] << b));

	shl	bl, cl
	sub	dl, bl
	mov	BYTE PTR [edi], dl

; 1284 :                         for (k = i + b; k < 256; ++k) {

	mov	edx, DWORD PTR _r$[esp+12]
$L1920:

; 1285 :                             if (!r[k]) {

	cmp	BYTE PTR [eax+edx], 0
	je	SHORT $L2148

; 1287 :                                 break;
; 1288 :                             }
; 1289 :                             r[k] = 0;

	mov	BYTE PTR [eax+edx], 0
	inc	eax
	cmp	eax, 256				; 00000100H
	jl	SHORT $L1920

; 1285 :                             if (!r[k]) {

	jmp	SHORT $L1912
$L2148:

; 1286 :                                 r[k] = 1;

	mov	BYTE PTR [eax+edx], 1
$L1912:

; 1277 :             for (b = 1; b <= 6 && i + b < 256; ++b) {

	inc	ecx
	cmp	ecx, 6
	jle	SHORT $L1911
$L1908:

; 1273 :     }
; 1274 : 
; 1275 :     for (i = 0; i < 256; ++i) {

	mov	eax, DWORD PTR _i$[esp+12]
	inc	eax
	inc	edi
	cmp	eax, 256				; 00000100H
	mov	DWORD PTR _i$[esp+12], eax
	jl	$L1907
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 1290 :                         }
; 1291 :                     } else {
; 1292 :                         break;
; 1293 :                     }
; 1294 :                 }
; 1295 :             }
; 1296 :         }
; 1297 :     }
; 1298 : }

	ret	0
_slide	ENDP
_TEXT	ENDS
END
