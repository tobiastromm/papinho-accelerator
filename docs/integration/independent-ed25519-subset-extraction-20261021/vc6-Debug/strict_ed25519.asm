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
$$SYMBOLS	SEGMENT BYTE USE32 'DEBSYM'
$$SYMBOLS	ENDS
$$TYPES	SEGMENT BYTE USE32 'DEBTYP'
$$TYPES	ENDS
_TLS	SEGMENT DWORD USE32 PUBLIC 'TLS'
_TLS	ENDS
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
EXTRN	_memcmp:NEAR
_TEXT	SEGMENT
_key$ = 8
_bytes$ = 12
_point$ = -284
_multiple$ = -120
_encoded$ = -316
_i$ = -124
_papacc_time_source_ed25519_check PROC NEAR

; 1442 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 316				; 0000013cH

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

	cmp	DWORD PTR _key$[ebp], 0
	je	SHORT $L1948
	cmp	DWORD PTR _bytes$[ebp], 32		; 00000020H
	je	SHORT $L1947
$L1948:

; 1456 :         return 0;

	xor	eax, eax
	jmp	$L1935
$L1947:

; 1457 :     if (ge_frombytes_vartime(&point, key) != 0)

	mov	eax, DWORD PTR _key$[ebp]
	push	eax
	lea	ecx, DWORD PTR _point$[ebp]
	push	ecx
	call	_ge_frombytes_vartime
	add	esp, 8
	test	eax, eax
	je	SHORT $L1949

; 1458 :         return 0;

	xor	eax, eax
	jmp	$L1935
$L1949:

; 1459 :     ge_p3_tobytes(encoded, &point);

	lea	edx, DWORD PTR _point$[ebp]
	push	edx
	lea	eax, DWORD PTR _encoded$[ebp]
	push	eax
	call	_ge_p3_tobytes
	add	esp, 8

; 1460 :     if (memcmp(encoded, key, 32) != 0)

	push	32					; 00000020H
	mov	ecx, DWORD PTR _key$[ebp]
	push	ecx
	lea	edx, DWORD PTR _encoded$[ebp]
	push	edx
	call	_memcmp
	add	esp, 12					; 0000000cH
	test	eax, eax
	je	SHORT $L1950

; 1461 :         return 0;

	xor	eax, eax
	jmp	$L1935
$L1950:

; 1462 :     if (memcmp(encoded, identity, 32) == 0)

	push	32					; 00000020H
	push	OFFSET FLAT:_?identity@?1??papacc_time_source_ed25519_check@@9@9
	lea	eax, DWORD PTR _encoded$[ebp]
	push	eax
	call	_memcmp
	add	esp, 12					; 0000000cH
	test	eax, eax
	jne	SHORT $L1951

; 1463 :         return 0;

	xor	eax, eax
	jmp	$L1935
$L1951:

; 1464 :     /* Integer L, NOT x25519_sc_reduce(L), NOT cofactor-cleared input. */
; 1465 :     ge_double_scalarmult_vartime(&multiple, order, &point, zero);

	push	OFFSET FLAT:_?zero@?1??papacc_time_source_ed25519_check@@9@9
	lea	ecx, DWORD PTR _point$[ebp]
	push	ecx
	push	OFFSET FLAT:_?order@?1??papacc_time_source_ed25519_check@@9@9
	lea	edx, DWORD PTR _multiple$[ebp]
	push	edx
	call	_ge_double_scalarmult_vartime
	add	esp, 16					; 00000010H

; 1466 :     /* Projective zero denominator is an internal invariant failure. */
; 1467 :     if (fe_isnonzero(multiple.Z) == 0)

	lea	eax, DWORD PTR _multiple$[ebp+80]
	push	eax
	call	_fe_isnonzero
	add	esp, 4
	test	eax, eax
	jne	SHORT $L1952

; 1468 :         return -1;

	or	eax, -1
	jmp	SHORT $L1935
$L1952:

; 1469 :     ge_tobytes(encoded, &multiple);

	lea	ecx, DWORD PTR _multiple$[ebp]
	push	ecx
	lea	edx, DWORD PTR _encoded$[ebp]
	push	edx
	call	_ge_tobytes
	add	esp, 8

; 1470 :     for (i = 0; i < 32; ++i) {

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L1953
$L1954:
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 1
	mov	DWORD PTR _i$[ebp], eax
$L1953:
	cmp	DWORD PTR _i$[ebp], 32			; 00000020H
	jae	SHORT $L1955

; 1471 :         if (encoded[i] != identity[i])

	mov	ecx, DWORD PTR _i$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR _encoded$[ebp+ecx]
	mov	eax, DWORD PTR _i$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR _?identity@?1??papacc_time_source_ed25519_check@@9@9[eax]
	cmp	edx, ecx
	je	SHORT $L1956

; 1472 :             return 0;

	xor	eax, eax
	jmp	SHORT $L1935
$L1956:

; 1473 :     }

	jmp	SHORT $L1954
$L1955:

; 1474 :     return 1;

	mov	eax, 1
$L1935:

; 1475 : }

	mov	esp, ebp
	pop	ebp
	ret	0
_papacc_time_source_ed25519_check ENDP
_f$ = 8
_s$ = -32
_fe_isnonzero PROC NEAR

; 818  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 32					; 00000020H

; 819  :     uint8_t s[32];
; 820  :     static const uint8_t zero[32] = { 0 };
; 821  : 
; 822  :     fe_tobytes(s, f);

	mov	eax, DWORD PTR _f$[ebp]
	push	eax
	lea	ecx, DWORD PTR _s$[ebp]
	push	ecx
	call	_fe_tobytes
	add	esp, 8

; 823  : 
; 824  :     return memcmp(s, zero, sizeof(zero)) != 0;

	push	32					; 00000020H
	push	OFFSET FLAT:_?zero@?1??fe_isnonzero@@9@9
	lea	edx, DWORD PTR _s$[ebp]
	push	edx
	call	_memcmp
	add	esp, 12					; 0000000cH
	neg	eax
	sbb	eax, eax
	neg	eax

; 825  : }

	mov	esp, ebp
	pop	ebp
	ret	0
_fe_isnonzero ENDP
_s$ = 8
_h$ = 12
_h0$ = -8
_h1$ = -12
_h2$ = -16
_h3$ = -20
_h4$ = -24
_h5$ = -28
_h6$ = -32
_h7$ = -36
_h8$ = -40
_h9$ = -44
_q$ = -4
_fe_tobytes PROC NEAR

; 191  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 44					; 0000002cH

; 192  :     int32_t h0 = h[0];

	mov	eax, DWORD PTR _h$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _h0$[ebp], ecx

; 193  :     int32_t h1 = h[1];

	mov	edx, DWORD PTR _h$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR _h1$[ebp], eax

; 194  :     int32_t h2 = h[2];

	mov	ecx, DWORD PTR _h$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	mov	DWORD PTR _h2$[ebp], edx

; 195  :     int32_t h3 = h[3];

	mov	eax, DWORD PTR _h$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	DWORD PTR _h3$[ebp], ecx

; 196  :     int32_t h4 = h[4];

	mov	edx, DWORD PTR _h$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	DWORD PTR _h4$[ebp], eax

; 197  :     int32_t h5 = h[5];

	mov	ecx, DWORD PTR _h$[ebp]
	mov	edx, DWORD PTR [ecx+20]
	mov	DWORD PTR _h5$[ebp], edx

; 198  :     int32_t h6 = h[6];

	mov	eax, DWORD PTR _h$[ebp]
	mov	ecx, DWORD PTR [eax+24]
	mov	DWORD PTR _h6$[ebp], ecx

; 199  :     int32_t h7 = h[7];

	mov	edx, DWORD PTR _h$[ebp]
	mov	eax, DWORD PTR [edx+28]
	mov	DWORD PTR _h7$[ebp], eax

; 200  :     int32_t h8 = h[8];

	mov	ecx, DWORD PTR _h$[ebp]
	mov	edx, DWORD PTR [ecx+32]
	mov	DWORD PTR _h8$[ebp], edx

; 201  :     int32_t h9 = h[9];

	mov	eax, DWORD PTR _h$[ebp]
	mov	ecx, DWORD PTR [eax+36]
	mov	DWORD PTR _h9$[ebp], ecx

; 202  :     int32_t q;
; 203  : 
; 204  :     q = (19 * h9 + (((int32_t)1) << 24)) >> 25;

	mov	edx, DWORD PTR _h9$[ebp]
	imul	edx, 19					; 00000013H
	add	edx, 16777216				; 01000000H
	sar	edx, 25					; 00000019H
	mov	DWORD PTR _q$[ebp], edx

; 205  :     q = (h0 + q) >> 26;

	mov	eax, DWORD PTR _h0$[ebp]
	add	eax, DWORD PTR _q$[ebp]
	sar	eax, 26					; 0000001aH
	mov	DWORD PTR _q$[ebp], eax

; 206  :     q = (h1 + q) >> 25;

	mov	ecx, DWORD PTR _h1$[ebp]
	add	ecx, DWORD PTR _q$[ebp]
	sar	ecx, 25					; 00000019H
	mov	DWORD PTR _q$[ebp], ecx

; 207  :     q = (h2 + q) >> 26;

	mov	edx, DWORD PTR _h2$[ebp]
	add	edx, DWORD PTR _q$[ebp]
	sar	edx, 26					; 0000001aH
	mov	DWORD PTR _q$[ebp], edx

; 208  :     q = (h3 + q) >> 25;

	mov	eax, DWORD PTR _h3$[ebp]
	add	eax, DWORD PTR _q$[ebp]
	sar	eax, 25					; 00000019H
	mov	DWORD PTR _q$[ebp], eax

; 209  :     q = (h4 + q) >> 26;

	mov	ecx, DWORD PTR _h4$[ebp]
	add	ecx, DWORD PTR _q$[ebp]
	sar	ecx, 26					; 0000001aH
	mov	DWORD PTR _q$[ebp], ecx

; 210  :     q = (h5 + q) >> 25;

	mov	edx, DWORD PTR _h5$[ebp]
	add	edx, DWORD PTR _q$[ebp]
	sar	edx, 25					; 00000019H
	mov	DWORD PTR _q$[ebp], edx

; 211  :     q = (h6 + q) >> 26;

	mov	eax, DWORD PTR _h6$[ebp]
	add	eax, DWORD PTR _q$[ebp]
	sar	eax, 26					; 0000001aH
	mov	DWORD PTR _q$[ebp], eax

; 212  :     q = (h7 + q) >> 25;

	mov	ecx, DWORD PTR _h7$[ebp]
	add	ecx, DWORD PTR _q$[ebp]
	sar	ecx, 25					; 00000019H
	mov	DWORD PTR _q$[ebp], ecx

; 213  :     q = (h8 + q) >> 26;

	mov	edx, DWORD PTR _h8$[ebp]
	add	edx, DWORD PTR _q$[ebp]
	sar	edx, 26					; 0000001aH
	mov	DWORD PTR _q$[ebp], edx

; 214  :     q = (h9 + q) >> 25;

	mov	eax, DWORD PTR _h9$[ebp]
	add	eax, DWORD PTR _q$[ebp]
	sar	eax, 25					; 00000019H
	mov	DWORD PTR _q$[ebp], eax

; 215  : 
; 216  :     /* Goal: Output h-(2^255-19)q, which is between 0 and 2^255-20. */
; 217  :     h0 += 19 * q;

	mov	ecx, DWORD PTR _q$[ebp]
	imul	ecx, 19					; 00000013H
	mov	edx, DWORD PTR _h0$[ebp]
	add	edx, ecx
	mov	DWORD PTR _h0$[ebp], edx

; 218  :     /* Goal: Output h-2^255 q, which is between 0 and 2^255-20. */
; 219  : 
; 220  :     h1 += h0 >> 26;

	mov	eax, DWORD PTR _h0$[ebp]
	sar	eax, 26					; 0000001aH
	mov	ecx, DWORD PTR _h1$[ebp]
	add	ecx, eax
	mov	DWORD PTR _h1$[ebp], ecx

; 221  :     h0 &= kBottom26Bits;

	mov	edx, DWORD PTR _kBottom26Bits
	mov	eax, DWORD PTR _h0$[ebp]
	and	eax, edx
	mov	DWORD PTR _h0$[ebp], eax

; 222  :     h2 += h1 >> 25;

	mov	ecx, DWORD PTR _h1$[ebp]
	sar	ecx, 25					; 00000019H
	mov	edx, DWORD PTR _h2$[ebp]
	add	edx, ecx
	mov	DWORD PTR _h2$[ebp], edx

; 223  :     h1 &= kBottom25Bits;

	mov	eax, DWORD PTR _kBottom25Bits
	mov	ecx, DWORD PTR _h1$[ebp]
	and	ecx, eax
	mov	DWORD PTR _h1$[ebp], ecx

; 224  :     h3 += h2 >> 26;

	mov	edx, DWORD PTR _h2$[ebp]
	sar	edx, 26					; 0000001aH
	mov	eax, DWORD PTR _h3$[ebp]
	add	eax, edx
	mov	DWORD PTR _h3$[ebp], eax

; 225  :     h2 &= kBottom26Bits;

	mov	ecx, DWORD PTR _kBottom26Bits
	mov	edx, DWORD PTR _h2$[ebp]
	and	edx, ecx
	mov	DWORD PTR _h2$[ebp], edx

; 226  :     h4 += h3 >> 25;

	mov	eax, DWORD PTR _h3$[ebp]
	sar	eax, 25					; 00000019H
	mov	ecx, DWORD PTR _h4$[ebp]
	add	ecx, eax
	mov	DWORD PTR _h4$[ebp], ecx

; 227  :     h3 &= kBottom25Bits;

	mov	edx, DWORD PTR _kBottom25Bits
	mov	eax, DWORD PTR _h3$[ebp]
	and	eax, edx
	mov	DWORD PTR _h3$[ebp], eax

; 228  :     h5 += h4 >> 26;

	mov	ecx, DWORD PTR _h4$[ebp]
	sar	ecx, 26					; 0000001aH
	mov	edx, DWORD PTR _h5$[ebp]
	add	edx, ecx
	mov	DWORD PTR _h5$[ebp], edx

; 229  :     h4 &= kBottom26Bits;

	mov	eax, DWORD PTR _kBottom26Bits
	mov	ecx, DWORD PTR _h4$[ebp]
	and	ecx, eax
	mov	DWORD PTR _h4$[ebp], ecx

; 230  :     h6 += h5 >> 25;

	mov	edx, DWORD PTR _h5$[ebp]
	sar	edx, 25					; 00000019H
	mov	eax, DWORD PTR _h6$[ebp]
	add	eax, edx
	mov	DWORD PTR _h6$[ebp], eax

; 231  :     h5 &= kBottom25Bits;

	mov	ecx, DWORD PTR _kBottom25Bits
	mov	edx, DWORD PTR _h5$[ebp]
	and	edx, ecx
	mov	DWORD PTR _h5$[ebp], edx

; 232  :     h7 += h6 >> 26;

	mov	eax, DWORD PTR _h6$[ebp]
	sar	eax, 26					; 0000001aH
	mov	ecx, DWORD PTR _h7$[ebp]
	add	ecx, eax
	mov	DWORD PTR _h7$[ebp], ecx

; 233  :     h6 &= kBottom26Bits;

	mov	edx, DWORD PTR _kBottom26Bits
	mov	eax, DWORD PTR _h6$[ebp]
	and	eax, edx
	mov	DWORD PTR _h6$[ebp], eax

; 234  :     h8 += h7 >> 25;

	mov	ecx, DWORD PTR _h7$[ebp]
	sar	ecx, 25					; 00000019H
	mov	edx, DWORD PTR _h8$[ebp]
	add	edx, ecx
	mov	DWORD PTR _h8$[ebp], edx

; 235  :     h7 &= kBottom25Bits;

	mov	eax, DWORD PTR _kBottom25Bits
	mov	ecx, DWORD PTR _h7$[ebp]
	and	ecx, eax
	mov	DWORD PTR _h7$[ebp], ecx

; 236  :     h9 += h8 >> 26;

	mov	edx, DWORD PTR _h8$[ebp]
	sar	edx, 26					; 0000001aH
	mov	eax, DWORD PTR _h9$[ebp]
	add	eax, edx
	mov	DWORD PTR _h9$[ebp], eax

; 237  :     h8 &= kBottom26Bits;

	mov	ecx, DWORD PTR _kBottom26Bits
	mov	edx, DWORD PTR _h8$[ebp]
	and	edx, ecx
	mov	DWORD PTR _h8$[ebp], edx

; 238  :     h9 &= kBottom25Bits;

	mov	eax, DWORD PTR _kBottom25Bits
	mov	ecx, DWORD PTR _h9$[ebp]
	and	ecx, eax
	mov	DWORD PTR _h9$[ebp], ecx

; 239  :     /* h10 = carry9 */
; 240  : 
; 241  :     /*
; 242  :      * Goal: Output h0+...+2^255 h10-2^255 q, which is between 0 and 2^255-20.
; 243  :      * Have h0+...+2^230 h9 between 0 and 2^255-1;
; 244  :      * evidently 2^255 h10-2^255 q = 0.
; 245  :      * Goal: Output h0+...+2^230 h9.
; 246  :      */
; 247  :     s[0] = (uint8_t)(h0 >> 0);

	mov	edx, DWORD PTR _s$[ebp]
	mov	al, BYTE PTR _h0$[ebp]
	mov	BYTE PTR [edx], al

; 248  :     s[1] = (uint8_t)(h0 >> 8);

	mov	ecx, DWORD PTR _h0$[ebp]
	sar	ecx, 8
	mov	edx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [edx+1], cl

; 249  :     s[2] = (uint8_t)(h0 >> 16);

	mov	eax, DWORD PTR _h0$[ebp]
	sar	eax, 16					; 00000010H
	mov	ecx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [ecx+2], al

; 250  :     s[3] = (uint8_t)((h0 >> 24) | ((uint32_t)(h1) << 2));

	mov	edx, DWORD PTR _h0$[ebp]
	sar	edx, 24					; 00000018H
	mov	eax, DWORD PTR _h1$[ebp]
	shl	eax, 2
	or	edx, eax
	mov	ecx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [ecx+3], dl

; 251  :     s[4] = (uint8_t)(h1 >> 6);

	mov	edx, DWORD PTR _h1$[ebp]
	sar	edx, 6
	mov	eax, DWORD PTR _s$[ebp]
	mov	BYTE PTR [eax+4], dl

; 252  :     s[5] = (uint8_t)(h1 >> 14);

	mov	ecx, DWORD PTR _h1$[ebp]
	sar	ecx, 14					; 0000000eH
	mov	edx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [edx+5], cl

; 253  :     s[6] = (uint8_t)((h1 >> 22) | ((uint32_t)(h2) << 3));

	mov	eax, DWORD PTR _h1$[ebp]
	sar	eax, 22					; 00000016H
	mov	ecx, DWORD PTR _h2$[ebp]
	shl	ecx, 3
	or	eax, ecx
	mov	edx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [edx+6], al

; 254  :     s[7] = (uint8_t)(h2 >> 5);

	mov	eax, DWORD PTR _h2$[ebp]
	sar	eax, 5
	mov	ecx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [ecx+7], al

; 255  :     s[8] = (uint8_t)(h2 >> 13);

	mov	edx, DWORD PTR _h2$[ebp]
	sar	edx, 13					; 0000000dH
	mov	eax, DWORD PTR _s$[ebp]
	mov	BYTE PTR [eax+8], dl

; 256  :     s[9] = (uint8_t)((h2 >> 21) | ((uint32_t)(h3) << 5));

	mov	ecx, DWORD PTR _h2$[ebp]
	sar	ecx, 21					; 00000015H
	mov	edx, DWORD PTR _h3$[ebp]
	shl	edx, 5
	or	ecx, edx
	mov	eax, DWORD PTR _s$[ebp]
	mov	BYTE PTR [eax+9], cl

; 257  :     s[10] = (uint8_t)(h3 >> 3);

	mov	ecx, DWORD PTR _h3$[ebp]
	sar	ecx, 3
	mov	edx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [edx+10], cl

; 258  :     s[11] = (uint8_t)(h3 >> 11);

	mov	eax, DWORD PTR _h3$[ebp]
	sar	eax, 11					; 0000000bH
	mov	ecx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [ecx+11], al

; 259  :     s[12] = (uint8_t)((h3 >> 19) | ((uint32_t)(h4) << 6));

	mov	edx, DWORD PTR _h3$[ebp]
	sar	edx, 19					; 00000013H
	mov	eax, DWORD PTR _h4$[ebp]
	shl	eax, 6
	or	edx, eax
	mov	ecx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [ecx+12], dl

; 260  :     s[13] = (uint8_t)(h4 >> 2);

	mov	edx, DWORD PTR _h4$[ebp]
	sar	edx, 2
	mov	eax, DWORD PTR _s$[ebp]
	mov	BYTE PTR [eax+13], dl

; 261  :     s[14] = (uint8_t)(h4 >> 10);

	mov	ecx, DWORD PTR _h4$[ebp]
	sar	ecx, 10					; 0000000aH
	mov	edx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [edx+14], cl

; 262  :     s[15] = (uint8_t)(h4 >> 18);

	mov	eax, DWORD PTR _h4$[ebp]
	sar	eax, 18					; 00000012H
	mov	ecx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [ecx+15], al

; 263  :     s[16] = (uint8_t)(h5 >> 0);

	mov	edx, DWORD PTR _s$[ebp]
	mov	al, BYTE PTR _h5$[ebp]
	mov	BYTE PTR [edx+16], al

; 264  :     s[17] = (uint8_t)(h5 >> 8);

	mov	ecx, DWORD PTR _h5$[ebp]
	sar	ecx, 8
	mov	edx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [edx+17], cl

; 265  :     s[18] = (uint8_t)(h5 >> 16);

	mov	eax, DWORD PTR _h5$[ebp]
	sar	eax, 16					; 00000010H
	mov	ecx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [ecx+18], al

; 266  :     s[19] = (uint8_t)((h5 >> 24) | ((uint32_t)(h6) << 1));

	mov	edx, DWORD PTR _h5$[ebp]
	sar	edx, 24					; 00000018H
	mov	eax, DWORD PTR _h6$[ebp]
	shl	eax, 1
	or	edx, eax
	mov	ecx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [ecx+19], dl

; 267  :     s[20] = (uint8_t)(h6 >> 7);

	mov	edx, DWORD PTR _h6$[ebp]
	sar	edx, 7
	mov	eax, DWORD PTR _s$[ebp]
	mov	BYTE PTR [eax+20], dl

; 268  :     s[21] = (uint8_t)(h6 >> 15);

	mov	ecx, DWORD PTR _h6$[ebp]
	sar	ecx, 15					; 0000000fH
	mov	edx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [edx+21], cl

; 269  :     s[22] = (uint8_t)((h6 >> 23) | ((uint32_t)(h7) << 3));

	mov	eax, DWORD PTR _h6$[ebp]
	sar	eax, 23					; 00000017H
	mov	ecx, DWORD PTR _h7$[ebp]
	shl	ecx, 3
	or	eax, ecx
	mov	edx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [edx+22], al

; 270  :     s[23] = (uint8_t)(h7 >> 5);

	mov	eax, DWORD PTR _h7$[ebp]
	sar	eax, 5
	mov	ecx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [ecx+23], al

; 271  :     s[24] = (uint8_t)(h7 >> 13);

	mov	edx, DWORD PTR _h7$[ebp]
	sar	edx, 13					; 0000000dH
	mov	eax, DWORD PTR _s$[ebp]
	mov	BYTE PTR [eax+24], dl

; 272  :     s[25] = (uint8_t)((h7 >> 21) | ((uint32_t)(h8) << 4));

	mov	ecx, DWORD PTR _h7$[ebp]
	sar	ecx, 21					; 00000015H
	mov	edx, DWORD PTR _h8$[ebp]
	shl	edx, 4
	or	ecx, edx
	mov	eax, DWORD PTR _s$[ebp]
	mov	BYTE PTR [eax+25], cl

; 273  :     s[26] = (uint8_t)(h8 >> 4);

	mov	ecx, DWORD PTR _h8$[ebp]
	sar	ecx, 4
	mov	edx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [edx+26], cl

; 274  :     s[27] = (uint8_t)(h8 >> 12);

	mov	eax, DWORD PTR _h8$[ebp]
	sar	eax, 12					; 0000000cH
	mov	ecx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [ecx+27], al

; 275  :     s[28] = (uint8_t)((h8 >> 20) | ((uint32_t)(h9) << 6));

	mov	edx, DWORD PTR _h8$[ebp]
	sar	edx, 20					; 00000014H
	mov	eax, DWORD PTR _h9$[ebp]
	shl	eax, 6
	or	edx, eax
	mov	ecx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [ecx+28], dl

; 276  :     s[29] = (uint8_t)(h9 >> 2);

	mov	edx, DWORD PTR _h9$[ebp]
	sar	edx, 2
	mov	eax, DWORD PTR _s$[ebp]
	mov	BYTE PTR [eax+29], dl

; 277  :     s[30] = (uint8_t)(h9 >> 10);

	mov	ecx, DWORD PTR _h9$[ebp]
	sar	ecx, 10					; 0000000aH
	mov	edx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [edx+30], cl

; 278  :     s[31] = (uint8_t)(h9 >> 18);

	mov	eax, DWORD PTR _h9$[ebp]
	sar	eax, 18					; 00000012H
	mov	ecx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [ecx+31], al

; 279  : }

	mov	esp, ebp
	pop	ebp
	ret	0
_fe_tobytes ENDP
_s$ = 8
_h$ = 12
_recip$ = -120
_x$ = -40
_y$ = -80
_ge_tobytes PROC NEAR

; 1061 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 120				; 00000078H

; 1062 :     fe recip;
; 1063 :     fe x;
; 1064 :     fe y;
; 1065 : 
; 1066 :     fe_invert(recip, h->Z);

	mov	eax, DWORD PTR _h$[ebp]
	add	eax, 80					; 00000050H
	push	eax
	lea	ecx, DWORD PTR _recip$[ebp]
	push	ecx
	call	_fe_invert
	add	esp, 8

; 1067 :     fe_mul(x, h->X, recip);

	lea	edx, DWORD PTR _recip$[ebp]
	push	edx
	mov	eax, DWORD PTR _h$[ebp]
	push	eax
	lea	ecx, DWORD PTR _x$[ebp]
	push	ecx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1068 :     fe_mul(y, h->Y, recip);

	lea	edx, DWORD PTR _recip$[ebp]
	push	edx
	mov	eax, DWORD PTR _h$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	lea	ecx, DWORD PTR _y$[ebp]
	push	ecx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1069 :     fe_tobytes(s, y);

	lea	edx, DWORD PTR _y$[ebp]
	push	edx
	mov	eax, DWORD PTR _s$[ebp]
	push	eax
	call	_fe_tobytes
	add	esp, 8

; 1070 :     s[31] ^= fe_isnegative(x) << 7;

	lea	ecx, DWORD PTR _x$[ebp]
	push	ecx
	call	_fe_isnegative
	add	esp, 4
	shl	eax, 7
	mov	edx, DWORD PTR _s$[ebp]
	mov	cl, BYTE PTR [edx+31]
	xor	cl, al
	mov	edx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [edx+31], cl

; 1071 : }

	mov	esp, ebp
	pop	ebp
	ret	0
_ge_tobytes ENDP
_TEXT	ENDS
EXTRN	__allmul:NEAR
EXTRN	__allshr:NEAR
_TEXT	SEGMENT
_h$ = 8
_f$ = 12
_g$ = 16
_f0$ = -1016
_f1$ = -1096
_f2$ = -56
_f3$ = -104
_f4$ = -172
_f5$ = -244
_f6$ = -308
_f7$ = -388
_f8$ = -412
_f9$ = -464
_g0$ = -168
_g1$ = -240
_g2$ = -304
_g3$ = -384
_g4$ = -408
_g5$ = -460
_g6$ = -496
_g7$ = -552
_g8$ = -600
_g9$ = -692
_g1_19$ = -548
_g2_19$ = -596
_g3_19$ = -1092
_g4_19$ = -52
_g5_19$ = -100
_g6_19$ = -164
_g7_19$ = -456
_g8_19$ = -492
_g9_19$ = -544
_f1_2$ = -540
_f3_2$ = -1012
_f5_2$ = -1008
_f7_2$ = -1088
_f9_2$ = -1084
_f0g0$ = -452
_f0g1$ = -488
_f0g2$ = -536
_f0g3$ = -592
_f0g4$ = -688
_f0g5$ = -780
_f0g6$ = -916
_f0g7$ = -1004
_f0g8$ = -1080
_f0g9$ = -48
_f1g0$ = -444
_f1g1_2$ = -436
_f1g2$ = -528
_f1g3_2$ = -300
_f1g4$ = -680
_f1g5_2$ = -292
_f1g6$ = -908
_f1g7_2$ = -380
_f1g8$ = -1072
_f1g9_38$ = -584
_f2g0$ = -772
_f2g1$ = -900
_f2g2$ = -996
_f2g3$ = -1064
_f2g4$ = -40
_f2g5$ = -96
_f2g6$ = -160
_f2g7$ = -236
_f2g8_19$ = -764
_f2g9_19$ = -892
_f3g0$ = -756
_f3g1_2$ = -284
_f3g2$ = -988
_f3g3_2$ = -372
_f3g4$ = -32
_f3g5_2$ = -980
_f3g6$ = -152
_f3g7_38$ = -144
_f3g8_19$ = -748
_f3g9_38$ = -480
_f4g0$ = -740
_f4g1$ = -884
_f4g2$ = -972
_f4g3$ = -1056
_f4g4$ = -24
_f4g5$ = -88
_f4g6_19$ = -672
_f4g7_19$ = -732
_f4g8_19$ = -876
_f4g9_19$ = -136
_f5g0$ = -868
_f5g1_2$ = -364
_f5g2$ = -1048
_f5g3_2$ = -520
_f5g4$ = -80
_f5g5_38$ = -128
_f5g6_19$ = -724
_f5g7_38$ = -276
_f5g8_19$ = -120
_f5g9_38$ = -576
_f6g0$ = -860
_f6g1$ = -964
_f6g2$ = -1040
_f6g3$ = -16
_f6g4_19$ = -664
_f6g5_19$ = -716
_f6g6_19$ = -852
_f6g7_19$ = -956
_f6g8_19$ = -228
_f6g9_19$ = -268
_f7g0$ = -844
_f7g1_2$ = -356
_f7g2$ = -1032
_f7g3_38$ = -72
_f7g4_19$ = -656
_f7g5_38$ = -220
_f7g6_19$ = -836
_f7g7_38$ = -512
_f7g8_19$ = -212
_f7g9_38$ = -648
_f8g0$ = -828
_f8g1$ = -948
_f8g2_19$ = -348
_f8g3_19$ = -640
_f8g4_19$ = -708
_f8g5_19$ = -820
_f8g6_19$ = -940
_f8g7_19$ = -204
_f8g8_19$ = -260
_f8g9_19$ = -340
_f9g0$ = -932
_f9g1_38$ = -812
_f9g2_19$ = -632
_f9g3_38$ = -196
_f9g4_19$ = -804
_f9g5_38$ = -332
_f9g6_19$ = -188
_f9g7_38$ = -624
_f9g8_19$ = -324
_f9g9_38$ = -796
_h0$ = -404
_h1$ = -428
_h2$ = -472
_h3$ = -504
_h4$ = -568
_h5$ = -616
_h6$ = -700
_h7$ = -788
_h8$ = -924
_h9$ = -1024
_carry0$ = -112
_carry1$ = -180
_carry2$ = -8
_carry3$ = -64
_carry4$ = -396
_carry5$ = -420
_carry6$ = -252
_carry7$ = -316
_carry8$ = -560
_carry9$ = -608
_fe_mul	PROC NEAR

; 316  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 1096				; 00000448H
	push	esi

; 317  :     int32_t f0 = f[0];

	mov	eax, DWORD PTR _f$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _f0$[ebp], ecx

; 318  :     int32_t f1 = f[1];

	mov	edx, DWORD PTR _f$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR _f1$[ebp], eax

; 319  :     int32_t f2 = f[2];

	mov	ecx, DWORD PTR _f$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	mov	DWORD PTR _f2$[ebp], edx

; 320  :     int32_t f3 = f[3];

	mov	eax, DWORD PTR _f$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	DWORD PTR _f3$[ebp], ecx

; 321  :     int32_t f4 = f[4];

	mov	edx, DWORD PTR _f$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	DWORD PTR _f4$[ebp], eax

; 322  :     int32_t f5 = f[5];

	mov	ecx, DWORD PTR _f$[ebp]
	mov	edx, DWORD PTR [ecx+20]
	mov	DWORD PTR _f5$[ebp], edx

; 323  :     int32_t f6 = f[6];

	mov	eax, DWORD PTR _f$[ebp]
	mov	ecx, DWORD PTR [eax+24]
	mov	DWORD PTR _f6$[ebp], ecx

; 324  :     int32_t f7 = f[7];

	mov	edx, DWORD PTR _f$[ebp]
	mov	eax, DWORD PTR [edx+28]
	mov	DWORD PTR _f7$[ebp], eax

; 325  :     int32_t f8 = f[8];

	mov	ecx, DWORD PTR _f$[ebp]
	mov	edx, DWORD PTR [ecx+32]
	mov	DWORD PTR _f8$[ebp], edx

; 326  :     int32_t f9 = f[9];

	mov	eax, DWORD PTR _f$[ebp]
	mov	ecx, DWORD PTR [eax+36]
	mov	DWORD PTR _f9$[ebp], ecx

; 327  :     int32_t g0 = g[0];

	mov	edx, DWORD PTR _g$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR _g0$[ebp], eax

; 328  :     int32_t g1 = g[1];

	mov	ecx, DWORD PTR _g$[ebp]
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR _g1$[ebp], edx

; 329  :     int32_t g2 = g[2];

	mov	eax, DWORD PTR _g$[ebp]
	mov	ecx, DWORD PTR [eax+8]
	mov	DWORD PTR _g2$[ebp], ecx

; 330  :     int32_t g3 = g[3];

	mov	edx, DWORD PTR _g$[ebp]
	mov	eax, DWORD PTR [edx+12]
	mov	DWORD PTR _g3$[ebp], eax

; 331  :     int32_t g4 = g[4];

	mov	ecx, DWORD PTR _g$[ebp]
	mov	edx, DWORD PTR [ecx+16]
	mov	DWORD PTR _g4$[ebp], edx

; 332  :     int32_t g5 = g[5];

	mov	eax, DWORD PTR _g$[ebp]
	mov	ecx, DWORD PTR [eax+20]
	mov	DWORD PTR _g5$[ebp], ecx

; 333  :     int32_t g6 = g[6];

	mov	edx, DWORD PTR _g$[ebp]
	mov	eax, DWORD PTR [edx+24]
	mov	DWORD PTR _g6$[ebp], eax

; 334  :     int32_t g7 = g[7];

	mov	ecx, DWORD PTR _g$[ebp]
	mov	edx, DWORD PTR [ecx+28]
	mov	DWORD PTR _g7$[ebp], edx

; 335  :     int32_t g8 = g[8];

	mov	eax, DWORD PTR _g$[ebp]
	mov	ecx, DWORD PTR [eax+32]
	mov	DWORD PTR _g8$[ebp], ecx

; 336  :     int32_t g9 = g[9];

	mov	edx, DWORD PTR _g$[ebp]
	mov	eax, DWORD PTR [edx+36]
	mov	DWORD PTR _g9$[ebp], eax

; 337  :     int32_t g1_19 = 19 * g1; /* 1.959375*2^29 */

	mov	ecx, DWORD PTR _g1$[ebp]
	imul	ecx, 19					; 00000013H
	mov	DWORD PTR _g1_19$[ebp], ecx

; 338  :     int32_t g2_19 = 19 * g2; /* 1.959375*2^30; still ok */

	mov	edx, DWORD PTR _g2$[ebp]
	imul	edx, 19					; 00000013H
	mov	DWORD PTR _g2_19$[ebp], edx

; 339  :     int32_t g3_19 = 19 * g3;

	mov	eax, DWORD PTR _g3$[ebp]
	imul	eax, 19					; 00000013H
	mov	DWORD PTR _g3_19$[ebp], eax

; 340  :     int32_t g4_19 = 19 * g4;

	mov	ecx, DWORD PTR _g4$[ebp]
	imul	ecx, 19					; 00000013H
	mov	DWORD PTR _g4_19$[ebp], ecx

; 341  :     int32_t g5_19 = 19 * g5;

	mov	edx, DWORD PTR _g5$[ebp]
	imul	edx, 19					; 00000013H
	mov	DWORD PTR _g5_19$[ebp], edx

; 342  :     int32_t g6_19 = 19 * g6;

	mov	eax, DWORD PTR _g6$[ebp]
	imul	eax, 19					; 00000013H
	mov	DWORD PTR _g6_19$[ebp], eax

; 343  :     int32_t g7_19 = 19 * g7;

	mov	ecx, DWORD PTR _g7$[ebp]
	imul	ecx, 19					; 00000013H
	mov	DWORD PTR _g7_19$[ebp], ecx

; 344  :     int32_t g8_19 = 19 * g8;

	mov	edx, DWORD PTR _g8$[ebp]
	imul	edx, 19					; 00000013H
	mov	DWORD PTR _g8_19$[ebp], edx

; 345  :     int32_t g9_19 = 19 * g9;

	mov	eax, DWORD PTR _g9$[ebp]
	imul	eax, 19					; 00000013H
	mov	DWORD PTR _g9_19$[ebp], eax

; 346  :     int32_t f1_2 = 2 * f1;

	mov	ecx, DWORD PTR _f1$[ebp]
	shl	ecx, 1
	mov	DWORD PTR _f1_2$[ebp], ecx

; 347  :     int32_t f3_2 = 2 * f3;

	mov	edx, DWORD PTR _f3$[ebp]
	shl	edx, 1
	mov	DWORD PTR _f3_2$[ebp], edx

; 348  :     int32_t f5_2 = 2 * f5;

	mov	eax, DWORD PTR _f5$[ebp]
	shl	eax, 1
	mov	DWORD PTR _f5_2$[ebp], eax

; 349  :     int32_t f7_2 = 2 * f7;

	mov	ecx, DWORD PTR _f7$[ebp]
	shl	ecx, 1
	mov	DWORD PTR _f7_2$[ebp], ecx

; 350  :     int32_t f9_2 = 2 * f9;

	mov	edx, DWORD PTR _f9$[ebp]
	shl	edx, 1
	mov	DWORD PTR _f9_2$[ebp], edx

; 351  :     int64_t f0g0 = f0 * (int64_t)g0;

	mov	eax, DWORD PTR _f0$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g0$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0g0$[ebp], eax
	mov	DWORD PTR _f0g0$[ebp+4], edx

; 352  :     int64_t f0g1 = f0 * (int64_t)g1;

	mov	eax, DWORD PTR _f0$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g1$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0g1$[ebp], eax
	mov	DWORD PTR _f0g1$[ebp+4], edx

; 353  :     int64_t f0g2 = f0 * (int64_t)g2;

	mov	eax, DWORD PTR _f0$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0g2$[ebp], eax
	mov	DWORD PTR _f0g2$[ebp+4], edx

; 354  :     int64_t f0g3 = f0 * (int64_t)g3;

	mov	eax, DWORD PTR _f0$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g3$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0g3$[ebp], eax
	mov	DWORD PTR _f0g3$[ebp+4], edx

; 355  :     int64_t f0g4 = f0 * (int64_t)g4;

	mov	eax, DWORD PTR _f0$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g4$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0g4$[ebp], eax
	mov	DWORD PTR _f0g4$[ebp+4], edx

; 356  :     int64_t f0g5 = f0 * (int64_t)g5;

	mov	eax, DWORD PTR _f0$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g5$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0g5$[ebp], eax
	mov	DWORD PTR _f0g5$[ebp+4], edx

; 357  :     int64_t f0g6 = f0 * (int64_t)g6;

	mov	eax, DWORD PTR _f0$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g6$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0g6$[ebp], eax
	mov	DWORD PTR _f0g6$[ebp+4], edx

; 358  :     int64_t f0g7 = f0 * (int64_t)g7;

	mov	eax, DWORD PTR _f0$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g7$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0g7$[ebp], eax
	mov	DWORD PTR _f0g7$[ebp+4], edx

; 359  :     int64_t f0g8 = f0 * (int64_t)g8;

	mov	eax, DWORD PTR _f0$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g8$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0g8$[ebp], eax
	mov	DWORD PTR _f0g8$[ebp+4], edx

; 360  :     int64_t f0g9 = f0 * (int64_t)g9;

	mov	eax, DWORD PTR _f0$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g9$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0g9$[ebp], eax
	mov	DWORD PTR _f0g9$[ebp+4], edx

; 361  :     int64_t f1g0 = f1 * (int64_t)g0;

	mov	eax, DWORD PTR _f1$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g0$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1g0$[ebp], eax
	mov	DWORD PTR _f1g0$[ebp+4], edx

; 362  :     int64_t f1g1_2 = f1_2 * (int64_t)g1;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g1$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1g1_2$[ebp], eax
	mov	DWORD PTR _f1g1_2$[ebp+4], edx

; 363  :     int64_t f1g2 = f1 * (int64_t)g2;

	mov	eax, DWORD PTR _f1$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1g2$[ebp], eax
	mov	DWORD PTR _f1g2$[ebp+4], edx

; 364  :     int64_t f1g3_2 = f1_2 * (int64_t)g3;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g3$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1g3_2$[ebp], eax
	mov	DWORD PTR _f1g3_2$[ebp+4], edx

; 365  :     int64_t f1g4 = f1 * (int64_t)g4;

	mov	eax, DWORD PTR _f1$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g4$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1g4$[ebp], eax
	mov	DWORD PTR _f1g4$[ebp+4], edx

; 366  :     int64_t f1g5_2 = f1_2 * (int64_t)g5;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g5$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1g5_2$[ebp], eax
	mov	DWORD PTR _f1g5_2$[ebp+4], edx

; 367  :     int64_t f1g6 = f1 * (int64_t)g6;

	mov	eax, DWORD PTR _f1$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g6$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1g6$[ebp], eax
	mov	DWORD PTR _f1g6$[ebp+4], edx

; 368  :     int64_t f1g7_2 = f1_2 * (int64_t)g7;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g7$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1g7_2$[ebp], eax
	mov	DWORD PTR _f1g7_2$[ebp+4], edx

; 369  :     int64_t f1g8 = f1 * (int64_t)g8;

	mov	eax, DWORD PTR _f1$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g8$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1g8$[ebp], eax
	mov	DWORD PTR _f1g8$[ebp+4], edx

; 370  :     int64_t f1g9_38 = f1_2 * (int64_t)g9_19;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g9_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1g9_38$[ebp], eax
	mov	DWORD PTR _f1g9_38$[ebp+4], edx

; 371  :     int64_t f2g0 = f2 * (int64_t)g0;

	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g0$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2g0$[ebp], eax
	mov	DWORD PTR _f2g0$[ebp+4], edx

; 372  :     int64_t f2g1 = f2 * (int64_t)g1;

	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g1$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2g1$[ebp], eax
	mov	DWORD PTR _f2g1$[ebp+4], edx

; 373  :     int64_t f2g2 = f2 * (int64_t)g2;

	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2g2$[ebp], eax
	mov	DWORD PTR _f2g2$[ebp+4], edx

; 374  :     int64_t f2g3 = f2 * (int64_t)g3;

	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g3$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2g3$[ebp], eax
	mov	DWORD PTR _f2g3$[ebp+4], edx

; 375  :     int64_t f2g4 = f2 * (int64_t)g4;

	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g4$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2g4$[ebp], eax
	mov	DWORD PTR _f2g4$[ebp+4], edx

; 376  :     int64_t f2g5 = f2 * (int64_t)g5;

	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g5$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2g5$[ebp], eax
	mov	DWORD PTR _f2g5$[ebp+4], edx

; 377  :     int64_t f2g6 = f2 * (int64_t)g6;

	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g6$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2g6$[ebp], eax
	mov	DWORD PTR _f2g6$[ebp+4], edx

; 378  :     int64_t f2g7 = f2 * (int64_t)g7;

	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g7$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2g7$[ebp], eax
	mov	DWORD PTR _f2g7$[ebp+4], edx

; 379  :     int64_t f2g8_19 = f2 * (int64_t)g8_19;

	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2g8_19$[ebp], eax
	mov	DWORD PTR _f2g8_19$[ebp+4], edx

; 380  :     int64_t f2g9_19 = f2 * (int64_t)g9_19;

	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g9_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2g9_19$[ebp], eax
	mov	DWORD PTR _f2g9_19$[ebp+4], edx

; 381  :     int64_t f3g0 = f3 * (int64_t)g0;

	mov	eax, DWORD PTR _f3$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g0$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3g0$[ebp], eax
	mov	DWORD PTR _f3g0$[ebp+4], edx

; 382  :     int64_t f3g1_2 = f3_2 * (int64_t)g1;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g1$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3g1_2$[ebp], eax
	mov	DWORD PTR _f3g1_2$[ebp+4], edx

; 383  :     int64_t f3g2 = f3 * (int64_t)g2;

	mov	eax, DWORD PTR _f3$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3g2$[ebp], eax
	mov	DWORD PTR _f3g2$[ebp+4], edx

; 384  :     int64_t f3g3_2 = f3_2 * (int64_t)g3;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g3$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3g3_2$[ebp], eax
	mov	DWORD PTR _f3g3_2$[ebp+4], edx

; 385  :     int64_t f3g4 = f3 * (int64_t)g4;

	mov	eax, DWORD PTR _f3$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g4$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3g4$[ebp], eax
	mov	DWORD PTR _f3g4$[ebp+4], edx

; 386  :     int64_t f3g5_2 = f3_2 * (int64_t)g5;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g5$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3g5_2$[ebp], eax
	mov	DWORD PTR _f3g5_2$[ebp+4], edx

; 387  :     int64_t f3g6 = f3 * (int64_t)g6;

	mov	eax, DWORD PTR _f3$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g6$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3g6$[ebp], eax
	mov	DWORD PTR _f3g6$[ebp+4], edx

; 388  :     int64_t f3g7_38 = f3_2 * (int64_t)g7_19;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g7_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3g7_38$[ebp], eax
	mov	DWORD PTR _f3g7_38$[ebp+4], edx

; 389  :     int64_t f3g8_19 = f3 * (int64_t)g8_19;

	mov	eax, DWORD PTR _f3$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3g8_19$[ebp], eax
	mov	DWORD PTR _f3g8_19$[ebp+4], edx

; 390  :     int64_t f3g9_38 = f3_2 * (int64_t)g9_19;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g9_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3g9_38$[ebp], eax
	mov	DWORD PTR _f3g9_38$[ebp+4], edx

; 391  :     int64_t f4g0 = f4 * (int64_t)g0;

	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g0$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4g0$[ebp], eax
	mov	DWORD PTR _f4g0$[ebp+4], edx

; 392  :     int64_t f4g1 = f4 * (int64_t)g1;

	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g1$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4g1$[ebp], eax
	mov	DWORD PTR _f4g1$[ebp+4], edx

; 393  :     int64_t f4g2 = f4 * (int64_t)g2;

	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4g2$[ebp], eax
	mov	DWORD PTR _f4g2$[ebp+4], edx

; 394  :     int64_t f4g3 = f4 * (int64_t)g3;

	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g3$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4g3$[ebp], eax
	mov	DWORD PTR _f4g3$[ebp+4], edx

; 395  :     int64_t f4g4 = f4 * (int64_t)g4;

	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g4$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4g4$[ebp], eax
	mov	DWORD PTR _f4g4$[ebp+4], edx

; 396  :     int64_t f4g5 = f4 * (int64_t)g5;

	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g5$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4g5$[ebp], eax
	mov	DWORD PTR _f4g5$[ebp+4], edx

; 397  :     int64_t f4g6_19 = f4 * (int64_t)g6_19;

	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g6_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4g6_19$[ebp], eax
	mov	DWORD PTR _f4g6_19$[ebp+4], edx

; 398  :     int64_t f4g7_19 = f4 * (int64_t)g7_19;

	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g7_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4g7_19$[ebp], eax
	mov	DWORD PTR _f4g7_19$[ebp+4], edx

; 399  :     int64_t f4g8_19 = f4 * (int64_t)g8_19;

	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4g8_19$[ebp], eax
	mov	DWORD PTR _f4g8_19$[ebp+4], edx

; 400  :     int64_t f4g9_19 = f4 * (int64_t)g9_19;

	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g9_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4g9_19$[ebp], eax
	mov	DWORD PTR _f4g9_19$[ebp+4], edx

; 401  :     int64_t f5g0 = f5 * (int64_t)g0;

	mov	eax, DWORD PTR _f5$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g0$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5g0$[ebp], eax
	mov	DWORD PTR _f5g0$[ebp+4], edx

; 402  :     int64_t f5g1_2 = f5_2 * (int64_t)g1;

	mov	eax, DWORD PTR _f5_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g1$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5g1_2$[ebp], eax
	mov	DWORD PTR _f5g1_2$[ebp+4], edx

; 403  :     int64_t f5g2 = f5 * (int64_t)g2;

	mov	eax, DWORD PTR _f5$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5g2$[ebp], eax
	mov	DWORD PTR _f5g2$[ebp+4], edx

; 404  :     int64_t f5g3_2 = f5_2 * (int64_t)g3;

	mov	eax, DWORD PTR _f5_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g3$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5g3_2$[ebp], eax
	mov	DWORD PTR _f5g3_2$[ebp+4], edx

; 405  :     int64_t f5g4 = f5 * (int64_t)g4;

	mov	eax, DWORD PTR _f5$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g4$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5g4$[ebp], eax
	mov	DWORD PTR _f5g4$[ebp+4], edx

; 406  :     int64_t f5g5_38 = f5_2 * (int64_t)g5_19;

	mov	eax, DWORD PTR _f5_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g5_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5g5_38$[ebp], eax
	mov	DWORD PTR _f5g5_38$[ebp+4], edx

; 407  :     int64_t f5g6_19 = f5 * (int64_t)g6_19;

	mov	eax, DWORD PTR _f5$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g6_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5g6_19$[ebp], eax
	mov	DWORD PTR _f5g6_19$[ebp+4], edx

; 408  :     int64_t f5g7_38 = f5_2 * (int64_t)g7_19;

	mov	eax, DWORD PTR _f5_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g7_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5g7_38$[ebp], eax
	mov	DWORD PTR _f5g7_38$[ebp+4], edx

; 409  :     int64_t f5g8_19 = f5 * (int64_t)g8_19;

	mov	eax, DWORD PTR _f5$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5g8_19$[ebp], eax
	mov	DWORD PTR _f5g8_19$[ebp+4], edx

; 410  :     int64_t f5g9_38 = f5_2 * (int64_t)g9_19;

	mov	eax, DWORD PTR _f5_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g9_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5g9_38$[ebp], eax
	mov	DWORD PTR _f5g9_38$[ebp+4], edx

; 411  :     int64_t f6g0 = f6 * (int64_t)g0;

	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g0$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6g0$[ebp], eax
	mov	DWORD PTR _f6g0$[ebp+4], edx

; 412  :     int64_t f6g1 = f6 * (int64_t)g1;

	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g1$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6g1$[ebp], eax
	mov	DWORD PTR _f6g1$[ebp+4], edx

; 413  :     int64_t f6g2 = f6 * (int64_t)g2;

	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6g2$[ebp], eax
	mov	DWORD PTR _f6g2$[ebp+4], edx

; 414  :     int64_t f6g3 = f6 * (int64_t)g3;

	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g3$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6g3$[ebp], eax
	mov	DWORD PTR _f6g3$[ebp+4], edx

; 415  :     int64_t f6g4_19 = f6 * (int64_t)g4_19;

	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g4_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6g4_19$[ebp], eax
	mov	DWORD PTR _f6g4_19$[ebp+4], edx

; 416  :     int64_t f6g5_19 = f6 * (int64_t)g5_19;

	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g5_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6g5_19$[ebp], eax
	mov	DWORD PTR _f6g5_19$[ebp+4], edx

; 417  :     int64_t f6g6_19 = f6 * (int64_t)g6_19;

	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g6_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6g6_19$[ebp], eax
	mov	DWORD PTR _f6g6_19$[ebp+4], edx

; 418  :     int64_t f6g7_19 = f6 * (int64_t)g7_19;

	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g7_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6g7_19$[ebp], eax
	mov	DWORD PTR _f6g7_19$[ebp+4], edx

; 419  :     int64_t f6g8_19 = f6 * (int64_t)g8_19;

	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6g8_19$[ebp], eax
	mov	DWORD PTR _f6g8_19$[ebp+4], edx

; 420  :     int64_t f6g9_19 = f6 * (int64_t)g9_19;

	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g9_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6g9_19$[ebp], eax
	mov	DWORD PTR _f6g9_19$[ebp+4], edx

; 421  :     int64_t f7g0 = f7 * (int64_t)g0;

	mov	eax, DWORD PTR _f7$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g0$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f7g0$[ebp], eax
	mov	DWORD PTR _f7g0$[ebp+4], edx

; 422  :     int64_t f7g1_2 = f7_2 * (int64_t)g1;

	mov	eax, DWORD PTR _f7_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g1$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f7g1_2$[ebp], eax
	mov	DWORD PTR _f7g1_2$[ebp+4], edx

; 423  :     int64_t f7g2 = f7 * (int64_t)g2;

	mov	eax, DWORD PTR _f7$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f7g2$[ebp], eax
	mov	DWORD PTR _f7g2$[ebp+4], edx

; 424  :     int64_t f7g3_38 = f7_2 * (int64_t)g3_19;

	mov	eax, DWORD PTR _f7_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g3_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f7g3_38$[ebp], eax
	mov	DWORD PTR _f7g3_38$[ebp+4], edx

; 425  :     int64_t f7g4_19 = f7 * (int64_t)g4_19;

	mov	eax, DWORD PTR _f7$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g4_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f7g4_19$[ebp], eax
	mov	DWORD PTR _f7g4_19$[ebp+4], edx

; 426  :     int64_t f7g5_38 = f7_2 * (int64_t)g5_19;

	mov	eax, DWORD PTR _f7_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g5_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f7g5_38$[ebp], eax
	mov	DWORD PTR _f7g5_38$[ebp+4], edx

; 427  :     int64_t f7g6_19 = f7 * (int64_t)g6_19;

	mov	eax, DWORD PTR _f7$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g6_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f7g6_19$[ebp], eax
	mov	DWORD PTR _f7g6_19$[ebp+4], edx

; 428  :     int64_t f7g7_38 = f7_2 * (int64_t)g7_19;

	mov	eax, DWORD PTR _f7_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g7_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f7g7_38$[ebp], eax
	mov	DWORD PTR _f7g7_38$[ebp+4], edx

; 429  :     int64_t f7g8_19 = f7 * (int64_t)g8_19;

	mov	eax, DWORD PTR _f7$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f7g8_19$[ebp], eax
	mov	DWORD PTR _f7g8_19$[ebp+4], edx

; 430  :     int64_t f7g9_38 = f7_2 * (int64_t)g9_19;

	mov	eax, DWORD PTR _f7_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g9_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f7g9_38$[ebp], eax
	mov	DWORD PTR _f7g9_38$[ebp+4], edx

; 431  :     int64_t f8g0 = f8 * (int64_t)g0;

	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g0$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f8g0$[ebp], eax
	mov	DWORD PTR _f8g0$[ebp+4], edx

; 432  :     int64_t f8g1 = f8 * (int64_t)g1;

	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g1$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f8g1$[ebp], eax
	mov	DWORD PTR _f8g1$[ebp+4], edx

; 433  :     int64_t f8g2_19 = f8 * (int64_t)g2_19;

	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g2_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f8g2_19$[ebp], eax
	mov	DWORD PTR _f8g2_19$[ebp+4], edx

; 434  :     int64_t f8g3_19 = f8 * (int64_t)g3_19;

	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g3_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f8g3_19$[ebp], eax
	mov	DWORD PTR _f8g3_19$[ebp+4], edx

; 435  :     int64_t f8g4_19 = f8 * (int64_t)g4_19;

	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g4_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f8g4_19$[ebp], eax
	mov	DWORD PTR _f8g4_19$[ebp+4], edx

; 436  :     int64_t f8g5_19 = f8 * (int64_t)g5_19;

	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g5_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f8g5_19$[ebp], eax
	mov	DWORD PTR _f8g5_19$[ebp+4], edx

; 437  :     int64_t f8g6_19 = f8 * (int64_t)g6_19;

	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g6_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f8g6_19$[ebp], eax
	mov	DWORD PTR _f8g6_19$[ebp+4], edx

; 438  :     int64_t f8g7_19 = f8 * (int64_t)g7_19;

	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g7_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f8g7_19$[ebp], eax
	mov	DWORD PTR _f8g7_19$[ebp+4], edx

; 439  :     int64_t f8g8_19 = f8 * (int64_t)g8_19;

	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f8g8_19$[ebp], eax
	mov	DWORD PTR _f8g8_19$[ebp+4], edx

; 440  :     int64_t f8g9_19 = f8 * (int64_t)g9_19;

	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g9_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f8g9_19$[ebp], eax
	mov	DWORD PTR _f8g9_19$[ebp+4], edx

; 441  :     int64_t f9g0 = f9 * (int64_t)g0;

	mov	eax, DWORD PTR _f9$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g0$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f9g0$[ebp], eax
	mov	DWORD PTR _f9g0$[ebp+4], edx

; 442  :     int64_t f9g1_38 = f9_2 * (int64_t)g1_19;

	mov	eax, DWORD PTR _f9_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g1_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f9g1_38$[ebp], eax
	mov	DWORD PTR _f9g1_38$[ebp+4], edx

; 443  :     int64_t f9g2_19 = f9 * (int64_t)g2_19;

	mov	eax, DWORD PTR _f9$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g2_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f9g2_19$[ebp], eax
	mov	DWORD PTR _f9g2_19$[ebp+4], edx

; 444  :     int64_t f9g3_38 = f9_2 * (int64_t)g3_19;

	mov	eax, DWORD PTR _f9_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g3_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f9g3_38$[ebp], eax
	mov	DWORD PTR _f9g3_38$[ebp+4], edx

; 445  :     int64_t f9g4_19 = f9 * (int64_t)g4_19;

	mov	eax, DWORD PTR _f9$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g4_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f9g4_19$[ebp], eax
	mov	DWORD PTR _f9g4_19$[ebp+4], edx

; 446  :     int64_t f9g5_38 = f9_2 * (int64_t)g5_19;

	mov	eax, DWORD PTR _f9_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g5_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f9g5_38$[ebp], eax
	mov	DWORD PTR _f9g5_38$[ebp+4], edx

; 447  :     int64_t f9g6_19 = f9 * (int64_t)g6_19;

	mov	eax, DWORD PTR _f9$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g6_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f9g6_19$[ebp], eax
	mov	DWORD PTR _f9g6_19$[ebp+4], edx

; 448  :     int64_t f9g7_38 = f9_2 * (int64_t)g7_19;

	mov	eax, DWORD PTR _f9_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g7_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f9g7_38$[ebp], eax
	mov	DWORD PTR _f9g7_38$[ebp+4], edx

; 449  :     int64_t f9g8_19 = f9 * (int64_t)g8_19;

	mov	eax, DWORD PTR _f9$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f9g8_19$[ebp], eax
	mov	DWORD PTR _f9g8_19$[ebp+4], edx

; 450  :     int64_t f9g9_38 = f9_2 * (int64_t)g9_19;

	mov	eax, DWORD PTR _f9_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _g9_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f9g9_38$[ebp], eax
	mov	DWORD PTR _f9g9_38$[ebp+4], edx

; 451  :     int64_t h0 = f0g0 + f1g9_38 + f2g8_19 + f3g7_38 + f4g6_19 + f5g5_38 + f6g4_19 + f7g3_38 + f8g2_19 + f9g1_38;

	mov	edx, DWORD PTR _f0g0$[ebp]
	add	edx, DWORD PTR _f1g9_38$[ebp]
	mov	eax, DWORD PTR _f0g0$[ebp+4]
	adc	eax, DWORD PTR _f1g9_38$[ebp+4]
	add	edx, DWORD PTR _f2g8_19$[ebp]
	adc	eax, DWORD PTR _f2g8_19$[ebp+4]
	add	edx, DWORD PTR _f3g7_38$[ebp]
	adc	eax, DWORD PTR _f3g7_38$[ebp+4]
	add	edx, DWORD PTR _f4g6_19$[ebp]
	adc	eax, DWORD PTR _f4g6_19$[ebp+4]
	add	edx, DWORD PTR _f5g5_38$[ebp]
	adc	eax, DWORD PTR _f5g5_38$[ebp+4]
	add	edx, DWORD PTR _f6g4_19$[ebp]
	adc	eax, DWORD PTR _f6g4_19$[ebp+4]
	add	edx, DWORD PTR _f7g3_38$[ebp]
	adc	eax, DWORD PTR _f7g3_38$[ebp+4]
	add	edx, DWORD PTR _f8g2_19$[ebp]
	adc	eax, DWORD PTR _f8g2_19$[ebp+4]
	add	edx, DWORD PTR _f9g1_38$[ebp]
	adc	eax, DWORD PTR _f9g1_38$[ebp+4]
	mov	DWORD PTR _h0$[ebp], edx
	mov	DWORD PTR _h0$[ebp+4], eax

; 452  :     int64_t h1 = f0g1 + f1g0 + f2g9_19 + f3g8_19 + f4g7_19 + f5g6_19 + f6g5_19 + f7g4_19 + f8g3_19 + f9g2_19;

	mov	ecx, DWORD PTR _f0g1$[ebp]
	add	ecx, DWORD PTR _f1g0$[ebp]
	mov	edx, DWORD PTR _f0g1$[ebp+4]
	adc	edx, DWORD PTR _f1g0$[ebp+4]
	add	ecx, DWORD PTR _f2g9_19$[ebp]
	adc	edx, DWORD PTR _f2g9_19$[ebp+4]
	add	ecx, DWORD PTR _f3g8_19$[ebp]
	adc	edx, DWORD PTR _f3g8_19$[ebp+4]
	add	ecx, DWORD PTR _f4g7_19$[ebp]
	adc	edx, DWORD PTR _f4g7_19$[ebp+4]
	add	ecx, DWORD PTR _f5g6_19$[ebp]
	adc	edx, DWORD PTR _f5g6_19$[ebp+4]
	add	ecx, DWORD PTR _f6g5_19$[ebp]
	adc	edx, DWORD PTR _f6g5_19$[ebp+4]
	add	ecx, DWORD PTR _f7g4_19$[ebp]
	adc	edx, DWORD PTR _f7g4_19$[ebp+4]
	add	ecx, DWORD PTR _f8g3_19$[ebp]
	adc	edx, DWORD PTR _f8g3_19$[ebp+4]
	add	ecx, DWORD PTR _f9g2_19$[ebp]
	adc	edx, DWORD PTR _f9g2_19$[ebp+4]
	mov	DWORD PTR _h1$[ebp], ecx
	mov	DWORD PTR _h1$[ebp+4], edx

; 453  :     int64_t h2 = f0g2 + f1g1_2 + f2g0 + f3g9_38 + f4g8_19 + f5g7_38 + f6g6_19 + f7g5_38 + f8g4_19 + f9g3_38;

	mov	eax, DWORD PTR _f0g2$[ebp]
	add	eax, DWORD PTR _f1g1_2$[ebp]
	mov	ecx, DWORD PTR _f0g2$[ebp+4]
	adc	ecx, DWORD PTR _f1g1_2$[ebp+4]
	add	eax, DWORD PTR _f2g0$[ebp]
	adc	ecx, DWORD PTR _f2g0$[ebp+4]
	add	eax, DWORD PTR _f3g9_38$[ebp]
	adc	ecx, DWORD PTR _f3g9_38$[ebp+4]
	add	eax, DWORD PTR _f4g8_19$[ebp]
	adc	ecx, DWORD PTR _f4g8_19$[ebp+4]
	add	eax, DWORD PTR _f5g7_38$[ebp]
	adc	ecx, DWORD PTR _f5g7_38$[ebp+4]
	add	eax, DWORD PTR _f6g6_19$[ebp]
	adc	ecx, DWORD PTR _f6g6_19$[ebp+4]
	add	eax, DWORD PTR _f7g5_38$[ebp]
	adc	ecx, DWORD PTR _f7g5_38$[ebp+4]
	add	eax, DWORD PTR _f8g4_19$[ebp]
	adc	ecx, DWORD PTR _f8g4_19$[ebp+4]
	add	eax, DWORD PTR _f9g3_38$[ebp]
	adc	ecx, DWORD PTR _f9g3_38$[ebp+4]
	mov	DWORD PTR _h2$[ebp], eax
	mov	DWORD PTR _h2$[ebp+4], ecx

; 454  :     int64_t h3 = f0g3 + f1g2 + f2g1 + f3g0 + f4g9_19 + f5g8_19 + f6g7_19 + f7g6_19 + f8g5_19 + f9g4_19;

	mov	edx, DWORD PTR _f0g3$[ebp]
	add	edx, DWORD PTR _f1g2$[ebp]
	mov	eax, DWORD PTR _f0g3$[ebp+4]
	adc	eax, DWORD PTR _f1g2$[ebp+4]
	add	edx, DWORD PTR _f2g1$[ebp]
	adc	eax, DWORD PTR _f2g1$[ebp+4]
	add	edx, DWORD PTR _f3g0$[ebp]
	adc	eax, DWORD PTR _f3g0$[ebp+4]
	add	edx, DWORD PTR _f4g9_19$[ebp]
	adc	eax, DWORD PTR _f4g9_19$[ebp+4]
	add	edx, DWORD PTR _f5g8_19$[ebp]
	adc	eax, DWORD PTR _f5g8_19$[ebp+4]
	add	edx, DWORD PTR _f6g7_19$[ebp]
	adc	eax, DWORD PTR _f6g7_19$[ebp+4]
	add	edx, DWORD PTR _f7g6_19$[ebp]
	adc	eax, DWORD PTR _f7g6_19$[ebp+4]
	add	edx, DWORD PTR _f8g5_19$[ebp]
	adc	eax, DWORD PTR _f8g5_19$[ebp+4]
	add	edx, DWORD PTR _f9g4_19$[ebp]
	adc	eax, DWORD PTR _f9g4_19$[ebp+4]
	mov	DWORD PTR _h3$[ebp], edx
	mov	DWORD PTR _h3$[ebp+4], eax

; 455  :     int64_t h4 = f0g4 + f1g3_2 + f2g2 + f3g1_2 + f4g0 + f5g9_38 + f6g8_19 + f7g7_38 + f8g6_19 + f9g5_38;

	mov	ecx, DWORD PTR _f0g4$[ebp]
	add	ecx, DWORD PTR _f1g3_2$[ebp]
	mov	edx, DWORD PTR _f0g4$[ebp+4]
	adc	edx, DWORD PTR _f1g3_2$[ebp+4]
	add	ecx, DWORD PTR _f2g2$[ebp]
	adc	edx, DWORD PTR _f2g2$[ebp+4]
	add	ecx, DWORD PTR _f3g1_2$[ebp]
	adc	edx, DWORD PTR _f3g1_2$[ebp+4]
	add	ecx, DWORD PTR _f4g0$[ebp]
	adc	edx, DWORD PTR _f4g0$[ebp+4]
	add	ecx, DWORD PTR _f5g9_38$[ebp]
	adc	edx, DWORD PTR _f5g9_38$[ebp+4]
	add	ecx, DWORD PTR _f6g8_19$[ebp]
	adc	edx, DWORD PTR _f6g8_19$[ebp+4]
	add	ecx, DWORD PTR _f7g7_38$[ebp]
	adc	edx, DWORD PTR _f7g7_38$[ebp+4]
	add	ecx, DWORD PTR _f8g6_19$[ebp]
	adc	edx, DWORD PTR _f8g6_19$[ebp+4]
	add	ecx, DWORD PTR _f9g5_38$[ebp]
	adc	edx, DWORD PTR _f9g5_38$[ebp+4]
	mov	DWORD PTR _h4$[ebp], ecx
	mov	DWORD PTR _h4$[ebp+4], edx

; 456  :     int64_t h5 = f0g5 + f1g4 + f2g3 + f3g2 + f4g1 + f5g0 + f6g9_19 + f7g8_19 + f8g7_19 + f9g6_19;

	mov	eax, DWORD PTR _f0g5$[ebp]
	add	eax, DWORD PTR _f1g4$[ebp]
	mov	ecx, DWORD PTR _f0g5$[ebp+4]
	adc	ecx, DWORD PTR _f1g4$[ebp+4]
	add	eax, DWORD PTR _f2g3$[ebp]
	adc	ecx, DWORD PTR _f2g3$[ebp+4]
	add	eax, DWORD PTR _f3g2$[ebp]
	adc	ecx, DWORD PTR _f3g2$[ebp+4]
	add	eax, DWORD PTR _f4g1$[ebp]
	adc	ecx, DWORD PTR _f4g1$[ebp+4]
	add	eax, DWORD PTR _f5g0$[ebp]
	adc	ecx, DWORD PTR _f5g0$[ebp+4]
	add	eax, DWORD PTR _f6g9_19$[ebp]
	adc	ecx, DWORD PTR _f6g9_19$[ebp+4]
	add	eax, DWORD PTR _f7g8_19$[ebp]
	adc	ecx, DWORD PTR _f7g8_19$[ebp+4]
	add	eax, DWORD PTR _f8g7_19$[ebp]
	adc	ecx, DWORD PTR _f8g7_19$[ebp+4]
	add	eax, DWORD PTR _f9g6_19$[ebp]
	adc	ecx, DWORD PTR _f9g6_19$[ebp+4]
	mov	DWORD PTR _h5$[ebp], eax
	mov	DWORD PTR _h5$[ebp+4], ecx

; 457  :     int64_t h6 = f0g6 + f1g5_2 + f2g4 + f3g3_2 + f4g2 + f5g1_2 + f6g0 + f7g9_38 + f8g8_19 + f9g7_38;

	mov	edx, DWORD PTR _f0g6$[ebp]
	add	edx, DWORD PTR _f1g5_2$[ebp]
	mov	eax, DWORD PTR _f0g6$[ebp+4]
	adc	eax, DWORD PTR _f1g5_2$[ebp+4]
	add	edx, DWORD PTR _f2g4$[ebp]
	adc	eax, DWORD PTR _f2g4$[ebp+4]
	add	edx, DWORD PTR _f3g3_2$[ebp]
	adc	eax, DWORD PTR _f3g3_2$[ebp+4]
	add	edx, DWORD PTR _f4g2$[ebp]
	adc	eax, DWORD PTR _f4g2$[ebp+4]
	add	edx, DWORD PTR _f5g1_2$[ebp]
	adc	eax, DWORD PTR _f5g1_2$[ebp+4]
	add	edx, DWORD PTR _f6g0$[ebp]
	adc	eax, DWORD PTR _f6g0$[ebp+4]
	add	edx, DWORD PTR _f7g9_38$[ebp]
	adc	eax, DWORD PTR _f7g9_38$[ebp+4]
	add	edx, DWORD PTR _f8g8_19$[ebp]
	adc	eax, DWORD PTR _f8g8_19$[ebp+4]
	add	edx, DWORD PTR _f9g7_38$[ebp]
	adc	eax, DWORD PTR _f9g7_38$[ebp+4]
	mov	DWORD PTR _h6$[ebp], edx
	mov	DWORD PTR _h6$[ebp+4], eax

; 458  :     int64_t h7 = f0g7 + f1g6 + f2g5 + f3g4 + f4g3 + f5g2 + f6g1 + f7g0 + f8g9_19 + f9g8_19;

	mov	ecx, DWORD PTR _f0g7$[ebp]
	add	ecx, DWORD PTR _f1g6$[ebp]
	mov	edx, DWORD PTR _f0g7$[ebp+4]
	adc	edx, DWORD PTR _f1g6$[ebp+4]
	add	ecx, DWORD PTR _f2g5$[ebp]
	adc	edx, DWORD PTR _f2g5$[ebp+4]
	add	ecx, DWORD PTR _f3g4$[ebp]
	adc	edx, DWORD PTR _f3g4$[ebp+4]
	add	ecx, DWORD PTR _f4g3$[ebp]
	adc	edx, DWORD PTR _f4g3$[ebp+4]
	add	ecx, DWORD PTR _f5g2$[ebp]
	adc	edx, DWORD PTR _f5g2$[ebp+4]
	add	ecx, DWORD PTR _f6g1$[ebp]
	adc	edx, DWORD PTR _f6g1$[ebp+4]
	add	ecx, DWORD PTR _f7g0$[ebp]
	adc	edx, DWORD PTR _f7g0$[ebp+4]
	add	ecx, DWORD PTR _f8g9_19$[ebp]
	adc	edx, DWORD PTR _f8g9_19$[ebp+4]
	add	ecx, DWORD PTR _f9g8_19$[ebp]
	adc	edx, DWORD PTR _f9g8_19$[ebp+4]
	mov	DWORD PTR _h7$[ebp], ecx
	mov	DWORD PTR _h7$[ebp+4], edx

; 459  :     int64_t h8 = f0g8 + f1g7_2 + f2g6 + f3g5_2 + f4g4 + f5g3_2 + f6g2 + f7g1_2 + f8g0 + f9g9_38;

	mov	eax, DWORD PTR _f0g8$[ebp]
	add	eax, DWORD PTR _f1g7_2$[ebp]
	mov	ecx, DWORD PTR _f0g8$[ebp+4]
	adc	ecx, DWORD PTR _f1g7_2$[ebp+4]
	add	eax, DWORD PTR _f2g6$[ebp]
	adc	ecx, DWORD PTR _f2g6$[ebp+4]
	add	eax, DWORD PTR _f3g5_2$[ebp]
	adc	ecx, DWORD PTR _f3g5_2$[ebp+4]
	add	eax, DWORD PTR _f4g4$[ebp]
	adc	ecx, DWORD PTR _f4g4$[ebp+4]
	add	eax, DWORD PTR _f5g3_2$[ebp]
	adc	ecx, DWORD PTR _f5g3_2$[ebp+4]
	add	eax, DWORD PTR _f6g2$[ebp]
	adc	ecx, DWORD PTR _f6g2$[ebp+4]
	add	eax, DWORD PTR _f7g1_2$[ebp]
	adc	ecx, DWORD PTR _f7g1_2$[ebp+4]
	add	eax, DWORD PTR _f8g0$[ebp]
	adc	ecx, DWORD PTR _f8g0$[ebp+4]
	add	eax, DWORD PTR _f9g9_38$[ebp]
	adc	ecx, DWORD PTR _f9g9_38$[ebp+4]
	mov	DWORD PTR _h8$[ebp], eax
	mov	DWORD PTR _h8$[ebp+4], ecx

; 460  :     int64_t h9 = f0g9 + f1g8 + f2g7 + f3g6 + f4g5 + f5g4 + f6g3 + f7g2 + f8g1 + f9g0;

	mov	edx, DWORD PTR _f0g9$[ebp]
	add	edx, DWORD PTR _f1g8$[ebp]
	mov	eax, DWORD PTR _f0g9$[ebp+4]
	adc	eax, DWORD PTR _f1g8$[ebp+4]
	add	edx, DWORD PTR _f2g7$[ebp]
	adc	eax, DWORD PTR _f2g7$[ebp+4]
	add	edx, DWORD PTR _f3g6$[ebp]
	adc	eax, DWORD PTR _f3g6$[ebp+4]
	add	edx, DWORD PTR _f4g5$[ebp]
	adc	eax, DWORD PTR _f4g5$[ebp+4]
	add	edx, DWORD PTR _f5g4$[ebp]
	adc	eax, DWORD PTR _f5g4$[ebp+4]
	add	edx, DWORD PTR _f6g3$[ebp]
	adc	eax, DWORD PTR _f6g3$[ebp+4]
	add	edx, DWORD PTR _f7g2$[ebp]
	adc	eax, DWORD PTR _f7g2$[ebp+4]
	add	edx, DWORD PTR _f8g1$[ebp]
	adc	eax, DWORD PTR _f8g1$[ebp+4]
	add	edx, DWORD PTR _f9g0$[ebp]
	adc	eax, DWORD PTR _f9g0$[ebp+4]
	mov	DWORD PTR _h9$[ebp], edx
	mov	DWORD PTR _h9$[ebp+4], eax

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

	mov	ecx, DWORD PTR _h0$[ebp]
	add	ecx, 33554432				; 02000000H
	mov	edx, DWORD PTR _h0$[ebp+4]
	adc	edx, 0
	mov	DWORD PTR _carry0$[ebp], ecx
	mov	DWORD PTR _carry0$[ebp+4], edx

; 478  :     h1 += carry0 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry0$[ebp]
	mov	edx, DWORD PTR _carry0$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h1$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h1$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h1$[ebp], ecx
	mov	DWORD PTR _h1$[ebp+4], eax

; 479  :     h0 -= carry0 & kTop38Bits;

	mov	ecx, DWORD PTR _carry0$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry0$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h0$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h0$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h0$[ebp], eax
	mov	DWORD PTR _h0$[ebp+4], ecx

; 480  :     carry4 = h4 + (1 << 25);

	mov	edx, DWORD PTR _h4$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h4$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry4$[ebp], edx
	mov	DWORD PTR _carry4$[ebp+4], eax

; 481  :     h5 += carry4 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry4$[ebp]
	mov	edx, DWORD PTR _carry4$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h5$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h5$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h5$[ebp], ecx
	mov	DWORD PTR _h5$[ebp+4], eax

; 482  :     h4 -= carry4 & kTop38Bits;

	mov	ecx, DWORD PTR _carry4$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry4$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h4$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h4$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h4$[ebp], eax
	mov	DWORD PTR _h4$[ebp+4], ecx

; 483  :     /* |h0| <= 2^25 */
; 484  :     /* |h4| <= 2^25 */
; 485  :     /* |h1| <= 1.71*2^59 */
; 486  :     /* |h5| <= 1.71*2^59 */
; 487  : 
; 488  :     carry1 = h1 + (1 << 24);

	mov	edx, DWORD PTR _h1$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h1$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry1$[ebp], edx
	mov	DWORD PTR _carry1$[ebp+4], eax

; 489  :     h2 += carry1 >> 25;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry1$[ebp]
	mov	edx, DWORD PTR _carry1$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h2$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h2$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h2$[ebp], ecx
	mov	DWORD PTR _h2$[ebp+4], eax

; 490  :     h1 -= carry1 & kTop39Bits;

	mov	ecx, DWORD PTR _carry1$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry1$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h1$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h1$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h1$[ebp], eax
	mov	DWORD PTR _h1$[ebp+4], ecx

; 491  :     carry5 = h5 + (1 << 24);

	mov	edx, DWORD PTR _h5$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h5$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry5$[ebp], edx
	mov	DWORD PTR _carry5$[ebp+4], eax

; 492  :     h6 += carry5 >> 25;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry5$[ebp]
	mov	edx, DWORD PTR _carry5$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h6$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h6$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h6$[ebp], ecx
	mov	DWORD PTR _h6$[ebp+4], eax

; 493  :     h5 -= carry5 & kTop39Bits;

	mov	ecx, DWORD PTR _carry5$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry5$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h5$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h5$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h5$[ebp], eax
	mov	DWORD PTR _h5$[ebp+4], ecx

; 494  :     /* |h1| <= 2^24; from now on fits into int32 */
; 495  :     /* |h5| <= 2^24; from now on fits into int32 */
; 496  :     /* |h2| <= 1.41*2^60 */
; 497  :     /* |h6| <= 1.41*2^60 */
; 498  : 
; 499  :     carry2 = h2 + (1 << 25);

	mov	edx, DWORD PTR _h2$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h2$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry2$[ebp], edx
	mov	DWORD PTR _carry2$[ebp+4], eax

; 500  :     h3 += carry2 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry2$[ebp]
	mov	edx, DWORD PTR _carry2$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h3$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h3$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h3$[ebp], ecx
	mov	DWORD PTR _h3$[ebp+4], eax

; 501  :     h2 -= carry2 & kTop38Bits;

	mov	ecx, DWORD PTR _carry2$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry2$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h2$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h2$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h2$[ebp], eax
	mov	DWORD PTR _h2$[ebp+4], ecx

; 502  :     carry6 = h6 + (1 << 25);

	mov	edx, DWORD PTR _h6$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h6$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry6$[ebp], edx
	mov	DWORD PTR _carry6$[ebp+4], eax

; 503  :     h7 += carry6 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry6$[ebp]
	mov	edx, DWORD PTR _carry6$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h7$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h7$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h7$[ebp], ecx
	mov	DWORD PTR _h7$[ebp+4], eax

; 504  :     h6 -= carry6 & kTop38Bits;

	mov	ecx, DWORD PTR _carry6$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry6$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h6$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h6$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h6$[ebp], eax
	mov	DWORD PTR _h6$[ebp+4], ecx

; 505  :     /* |h2| <= 2^25; from now on fits into int32 unchanged */
; 506  :     /* |h6| <= 2^25; from now on fits into int32 unchanged */
; 507  :     /* |h3| <= 1.71*2^59 */
; 508  :     /* |h7| <= 1.71*2^59 */
; 509  : 
; 510  :     carry3 = h3 + (1 << 24);

	mov	edx, DWORD PTR _h3$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h3$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry3$[ebp], edx
	mov	DWORD PTR _carry3$[ebp+4], eax

; 511  :     h4 += carry3 >> 25;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry3$[ebp]
	mov	edx, DWORD PTR _carry3$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h4$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h4$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h4$[ebp], ecx
	mov	DWORD PTR _h4$[ebp+4], eax

; 512  :     h3 -= carry3 & kTop39Bits;

	mov	ecx, DWORD PTR _carry3$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry3$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h3$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h3$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h3$[ebp], eax
	mov	DWORD PTR _h3$[ebp+4], ecx

; 513  :     carry7 = h7 + (1 << 24);

	mov	edx, DWORD PTR _h7$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h7$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry7$[ebp], edx
	mov	DWORD PTR _carry7$[ebp+4], eax

; 514  :     h8 += carry7 >> 25;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry7$[ebp]
	mov	edx, DWORD PTR _carry7$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h8$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h8$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h8$[ebp], ecx
	mov	DWORD PTR _h8$[ebp+4], eax

; 515  :     h7 -= carry7 & kTop39Bits;

	mov	ecx, DWORD PTR _carry7$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry7$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h7$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h7$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h7$[ebp], eax
	mov	DWORD PTR _h7$[ebp+4], ecx

; 516  :     /* |h3| <= 2^24; from now on fits into int32 unchanged */
; 517  :     /* |h7| <= 2^24; from now on fits into int32 unchanged */
; 518  :     /* |h4| <= 1.72*2^34 */
; 519  :     /* |h8| <= 1.41*2^60 */
; 520  : 
; 521  :     carry4 = h4 + (1 << 25);

	mov	edx, DWORD PTR _h4$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h4$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry4$[ebp], edx
	mov	DWORD PTR _carry4$[ebp+4], eax

; 522  :     h5 += carry4 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry4$[ebp]
	mov	edx, DWORD PTR _carry4$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h5$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h5$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h5$[ebp], ecx
	mov	DWORD PTR _h5$[ebp+4], eax

; 523  :     h4 -= carry4 & kTop38Bits;

	mov	ecx, DWORD PTR _carry4$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry4$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h4$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h4$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h4$[ebp], eax
	mov	DWORD PTR _h4$[ebp+4], ecx

; 524  :     carry8 = h8 + (1 << 25);

	mov	edx, DWORD PTR _h8$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h8$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry8$[ebp], edx
	mov	DWORD PTR _carry8$[ebp+4], eax

; 525  :     h9 += carry8 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry8$[ebp]
	mov	edx, DWORD PTR _carry8$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h9$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h9$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h9$[ebp], ecx
	mov	DWORD PTR _h9$[ebp+4], eax

; 526  :     h8 -= carry8 & kTop38Bits;

	mov	ecx, DWORD PTR _carry8$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry8$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h8$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h8$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h8$[ebp], eax
	mov	DWORD PTR _h8$[ebp+4], ecx

; 527  :     /* |h4| <= 2^25; from now on fits into int32 unchanged */
; 528  :     /* |h8| <= 2^25; from now on fits into int32 unchanged */
; 529  :     /* |h5| <= 1.01*2^24 */
; 530  :     /* |h9| <= 1.71*2^59 */
; 531  : 
; 532  :     carry9 = h9 + (1 << 24);

	mov	edx, DWORD PTR _h9$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h9$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry9$[ebp], edx
	mov	DWORD PTR _carry9$[ebp+4], eax

; 533  :     h0 += (carry9 >> 25) * 19;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry9$[ebp]
	mov	edx, DWORD PTR _carry9$[ebp+4]
	call	__allshr
	push	0
	push	19					; 00000013H
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR _h0$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h0$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h0$[ebp], ecx
	mov	DWORD PTR _h0$[ebp+4], eax

; 534  :     h9 -= carry9 & kTop39Bits;

	mov	ecx, DWORD PTR _carry9$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry9$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h9$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h9$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h9$[ebp], eax
	mov	DWORD PTR _h9$[ebp+4], ecx

; 535  :     /* |h9| <= 2^24; from now on fits into int32 unchanged */
; 536  :     /* |h0| <= 1.1*2^39 */
; 537  : 
; 538  :     carry0 = h0 + (1 << 25);

	mov	edx, DWORD PTR _h0$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h0$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry0$[ebp], edx
	mov	DWORD PTR _carry0$[ebp+4], eax

; 539  :     h1 += carry0 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry0$[ebp]
	mov	edx, DWORD PTR _carry0$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h1$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h1$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h1$[ebp], ecx
	mov	DWORD PTR _h1$[ebp+4], eax

; 540  :     h0 -= carry0 & kTop38Bits;

	mov	ecx, DWORD PTR _carry0$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry0$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h0$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h0$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h0$[ebp], eax
	mov	DWORD PTR _h0$[ebp+4], ecx

; 541  :     /* |h0| <= 2^25; from now on fits into int32 unchanged */
; 542  :     /* |h1| <= 1.01*2^24 */
; 543  : 
; 544  :     h[0] = (int32_t)h0;

	mov	edx, DWORD PTR _h0$[ebp]
	mov	eax, DWORD PTR _h$[ebp]
	mov	DWORD PTR [eax], edx

; 545  :     h[1] = (int32_t)h1;

	mov	ecx, DWORD PTR _h1$[ebp]
	mov	edx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [edx+4], ecx

; 546  :     h[2] = (int32_t)h2;

	mov	eax, DWORD PTR _h2$[ebp]
	mov	ecx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [ecx+8], eax

; 547  :     h[3] = (int32_t)h3;

	mov	edx, DWORD PTR _h3$[ebp]
	mov	eax, DWORD PTR _h$[ebp]
	mov	DWORD PTR [eax+12], edx

; 548  :     h[4] = (int32_t)h4;

	mov	ecx, DWORD PTR _h4$[ebp]
	mov	edx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [edx+16], ecx

; 549  :     h[5] = (int32_t)h5;

	mov	eax, DWORD PTR _h5$[ebp]
	mov	ecx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [ecx+20], eax

; 550  :     h[6] = (int32_t)h6;

	mov	edx, DWORD PTR _h6$[ebp]
	mov	eax, DWORD PTR _h$[ebp]
	mov	DWORD PTR [eax+24], edx

; 551  :     h[7] = (int32_t)h7;

	mov	ecx, DWORD PTR _h7$[ebp]
	mov	edx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [edx+28], ecx

; 552  :     h[8] = (int32_t)h8;

	mov	eax, DWORD PTR _h8$[ebp]
	mov	ecx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [ecx+32], eax

; 553  :     h[9] = (int32_t)h9;

	mov	edx, DWORD PTR _h9$[ebp]
	mov	eax, DWORD PTR _h$[ebp]
	mov	DWORD PTR [eax+36], edx

; 554  : }

	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	0
_fe_mul	ENDP
_out$ = 8
_z$ = 12
_t0$ = -40
_t1$ = -80
_t2$ = -124
_t3$ = -164
_i$ = -84
_fe_invert PROC NEAR

; 713  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 164				; 000000a4H

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

	mov	eax, DWORD PTR _z$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 727  : 
; 728  :     /* t1 = t0 ** (2 ** 2) = z ** 8 */
; 729  :     fe_sq(t1, t0);

	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	call	_fe_sq
	add	esp, 8

; 730  :     fe_sq(t1, t1);

	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t1$[ebp]
	push	edx
	call	_fe_sq
	add	esp, 8

; 731  : 
; 732  :     /* t1 = z * t1 = z ** 9 */
; 733  :     fe_mul(t1, z, t1);

	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	mov	ecx, DWORD PTR _z$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t1$[ebp]
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 734  :     /* t0 = t0 * t1 = z ** 11 -- stash t0 away for the end. */
; 735  :     fe_mul(t0, t0, t1);

	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 736  : 
; 737  :     /* t2 = t0 ** 2 = z ** 22 */
; 738  :     fe_sq(t2, t0);

	lea	eax, DWORD PTR _t0$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t2$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 739  : 
; 740  :     /* t1 = t1 * t2 = z ** (2 ** 5 - 1) */
; 741  :     fe_mul(t1, t1, t2);

	lea	edx, DWORD PTR _t2$[ebp]
	push	edx
	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 742  : 
; 743  :     /* t2 = t1 ** (2 ** 5) = z ** ((2 ** 5) * (2 ** 5 - 1)) */
; 744  :     fe_sq(t2, t1);

	lea	edx, DWORD PTR _t1$[ebp]
	push	edx
	lea	eax, DWORD PTR _t2$[ebp]
	push	eax
	call	_fe_sq
	add	esp, 8

; 745  :     for (i = 1; i < 5; ++i) {

	mov	DWORD PTR _i$[ebp], 1
	jmp	SHORT $L1524
$L1525:
	mov	ecx, DWORD PTR _i$[ebp]
	add	ecx, 1
	mov	DWORD PTR _i$[ebp], ecx
$L1524:
	cmp	DWORD PTR _i$[ebp], 5
	jge	SHORT $L1526

; 746  :         fe_sq(t2, t2);

	lea	edx, DWORD PTR _t2$[ebp]
	push	edx
	lea	eax, DWORD PTR _t2$[ebp]
	push	eax
	call	_fe_sq
	add	esp, 8

; 747  :     }

	jmp	SHORT $L1525
$L1526:

; 748  : 
; 749  :     /* t1 = t1 * t2 = z ** ((2 ** 5 + 1) * (2 ** 5 - 1)) = z ** (2 ** 10 - 1) */
; 750  :     fe_mul(t1, t2, t1);

	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t2$[ebp]
	push	edx
	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 751  : 
; 752  :     /* Continuing similarly... */
; 753  : 
; 754  :     /* t2 = z ** (2 ** 20 - 1) */
; 755  :     fe_sq(t2, t1);

	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t2$[ebp]
	push	edx
	call	_fe_sq
	add	esp, 8

; 756  :     for (i = 1; i < 10; ++i) {

	mov	DWORD PTR _i$[ebp], 1
	jmp	SHORT $L1527
$L1528:
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 1
	mov	DWORD PTR _i$[ebp], eax
$L1527:
	cmp	DWORD PTR _i$[ebp], 10			; 0000000aH
	jge	SHORT $L1529

; 757  :         fe_sq(t2, t2);

	lea	ecx, DWORD PTR _t2$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t2$[ebp]
	push	edx
	call	_fe_sq
	add	esp, 8

; 758  :     }

	jmp	SHORT $L1528
$L1529:

; 759  :     fe_mul(t2, t2, t1);

	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t2$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t2$[ebp]
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 760  : 
; 761  :     /* t2 = z ** (2 ** 40 - 1) */
; 762  :     fe_sq(t3, t2);

	lea	eax, DWORD PTR _t2$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t3$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 763  :     for (i = 1; i < 20; ++i) {

	mov	DWORD PTR _i$[ebp], 1
	jmp	SHORT $L1530
$L1531:
	mov	edx, DWORD PTR _i$[ebp]
	add	edx, 1
	mov	DWORD PTR _i$[ebp], edx
$L1530:
	cmp	DWORD PTR _i$[ebp], 20			; 00000014H
	jge	SHORT $L1532

; 764  :         fe_sq(t3, t3);

	lea	eax, DWORD PTR _t3$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t3$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 765  :     }

	jmp	SHORT $L1531
$L1532:

; 766  :     fe_mul(t2, t3, t2);

	lea	edx, DWORD PTR _t2$[ebp]
	push	edx
	lea	eax, DWORD PTR _t3$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t2$[ebp]
	push	ecx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 767  : 
; 768  :     /* t2 = z ** (2 ** 10) * (2 ** 40 - 1) */
; 769  :     for (i = 0; i < 10; ++i) {

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L1533
$L1534:
	mov	edx, DWORD PTR _i$[ebp]
	add	edx, 1
	mov	DWORD PTR _i$[ebp], edx
$L1533:
	cmp	DWORD PTR _i$[ebp], 10			; 0000000aH
	jge	SHORT $L1535

; 770  :         fe_sq(t2, t2);

	lea	eax, DWORD PTR _t2$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t2$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 771  :     }

	jmp	SHORT $L1534
$L1535:

; 772  :     /* t1 = z ** (2 ** 50 - 1) */
; 773  :     fe_mul(t1, t2, t1);

	lea	edx, DWORD PTR _t1$[ebp]
	push	edx
	lea	eax, DWORD PTR _t2$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 774  : 
; 775  :     /* t2 = z ** (2 ** 100 - 1) */
; 776  :     fe_sq(t2, t1);

	lea	edx, DWORD PTR _t1$[ebp]
	push	edx
	lea	eax, DWORD PTR _t2$[ebp]
	push	eax
	call	_fe_sq
	add	esp, 8

; 777  :     for (i = 1; i < 50; ++i) {

	mov	DWORD PTR _i$[ebp], 1
	jmp	SHORT $L1536
$L1537:
	mov	ecx, DWORD PTR _i$[ebp]
	add	ecx, 1
	mov	DWORD PTR _i$[ebp], ecx
$L1536:
	cmp	DWORD PTR _i$[ebp], 50			; 00000032H
	jge	SHORT $L1538

; 778  :         fe_sq(t2, t2);

	lea	edx, DWORD PTR _t2$[ebp]
	push	edx
	lea	eax, DWORD PTR _t2$[ebp]
	push	eax
	call	_fe_sq
	add	esp, 8

; 779  :     }

	jmp	SHORT $L1537
$L1538:

; 780  :     fe_mul(t2, t2, t1);

	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t2$[ebp]
	push	edx
	lea	eax, DWORD PTR _t2$[ebp]
	push	eax
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 781  : 
; 782  :     /* t2 = z ** (2 ** 200 - 1) */
; 783  :     fe_sq(t3, t2);

	lea	ecx, DWORD PTR _t2$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t3$[ebp]
	push	edx
	call	_fe_sq
	add	esp, 8

; 784  :     for (i = 1; i < 100; ++i) {

	mov	DWORD PTR _i$[ebp], 1
	jmp	SHORT $L1539
$L1540:
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 1
	mov	DWORD PTR _i$[ebp], eax
$L1539:
	cmp	DWORD PTR _i$[ebp], 100			; 00000064H
	jge	SHORT $L1541

; 785  :         fe_sq(t3, t3);

	lea	ecx, DWORD PTR _t3$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t3$[ebp]
	push	edx
	call	_fe_sq
	add	esp, 8

; 786  :     }

	jmp	SHORT $L1540
$L1541:

; 787  :     fe_mul(t2, t3, t2);

	lea	eax, DWORD PTR _t2$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t3$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t2$[ebp]
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 788  : 
; 789  :     /* t2 = z ** ((2 ** 50) * (2 ** 200 - 1) */
; 790  :     fe_sq(t2, t2);

	lea	eax, DWORD PTR _t2$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t2$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 791  :     for (i = 1; i < 50; ++i) {

	mov	DWORD PTR _i$[ebp], 1
	jmp	SHORT $L1542
$L1543:
	mov	edx, DWORD PTR _i$[ebp]
	add	edx, 1
	mov	DWORD PTR _i$[ebp], edx
$L1542:
	cmp	DWORD PTR _i$[ebp], 50			; 00000032H
	jge	SHORT $L1544

; 792  :         fe_sq(t2, t2);

	lea	eax, DWORD PTR _t2$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t2$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 793  :     }

	jmp	SHORT $L1543
$L1544:

; 794  : 
; 795  :     /* t1 = z ** (2 ** 250 - 1) */
; 796  :     fe_mul(t1, t2, t1);

	lea	edx, DWORD PTR _t1$[ebp]
	push	edx
	lea	eax, DWORD PTR _t2$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 797  : 
; 798  :     /* t1 = z ** ((2 ** 5) * (2 ** 250 - 1)) */
; 799  :     fe_sq(t1, t1);

	lea	edx, DWORD PTR _t1$[ebp]
	push	edx
	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	call	_fe_sq
	add	esp, 8

; 800  :     for (i = 1; i < 5; ++i) {

	mov	DWORD PTR _i$[ebp], 1
	jmp	SHORT $L1545
$L1546:
	mov	ecx, DWORD PTR _i$[ebp]
	add	ecx, 1
	mov	DWORD PTR _i$[ebp], ecx
$L1545:
	cmp	DWORD PTR _i$[ebp], 5
	jge	SHORT $L1547

; 801  :         fe_sq(t1, t1);

	lea	edx, DWORD PTR _t1$[ebp]
	push	edx
	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	call	_fe_sq
	add	esp, 8

; 802  :     }

	jmp	SHORT $L1546
$L1547:

; 803  : 
; 804  :     /* Recall t0 = z ** 11; out = z ** (2 ** 255 - 21) */
; 805  :     fe_mul(out, t1, t0);

	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t1$[ebp]
	push	edx
	mov	eax, DWORD PTR _out$[ebp]
	push	eax
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 806  : }

	mov	esp, ebp
	pop	ebp
	ret	0
_fe_invert ENDP
_h$ = 8
_f$ = 12
_f0$ = -636
_f1$ = -692
_f2$ = -36
_f3$ = -64
_f4$ = -100
_f5$ = -128
_f6$ = -188
_f7$ = -224
_f8$ = -260
_f9$ = -300
_f0_2$ = -352
_f1_2$ = -348
_f2_2$ = -592
_f3_2$ = -632
_f4_2$ = -628
_f5_2$ = -624
_f6_2$ = -620
_f7_2$ = -688
_f5_38$ = -32
_f6_19$ = -412
_f7_38$ = -296
_f8_19$ = -28
_f9_38$ = -344
_f0f0$ = -124
_f0f1_2$ = -616
_f0f2_2$ = -608
_f0f3_2$ = -684
_f0f4_2$ = -676
_f0f5_2$ = -668
_f0f6_2$ = -660
_f0f7_2$ = -548
_f0f8_2$ = -540
_f0f9_2$ = -532
_f1f1_2$ = -256
_f1f2_2$ = -248
_f1f3_4$ = -220
_f1f4_2$ = -292
_f1f5_4$ = -212
_f1f6_2$ = -284
_f1f7_4$ = -408
_f1f8_2$ = -184
_f1f9_76$ = -24
_f2f2$ = -400
_f2f3_2$ = -524
_f2f4_2$ = -516
_f2f5_2$ = -508
_f2f6_2$ = -588
_f2f7_2$ = -580
_f2f8_38$ = -452
_f2f9_38$ = -500
_f3f3_2$ = -176
_f3f4_2$ = -168
_f3f5_4$ = -392
_f3f6_2$ = -204
_f3f7_76$ = -316
_f3f8_38$ = -444
_f3f9_76$ = -652
_f4f4$ = -492
_f4f5_2$ = -572
_f4f6_38$ = -384
_f4f7_38$ = -436
_f4f8_38$ = -484
_f4f9_38$ = -60
_f5f5_38$ = -340
_f5f6_38$ = -376
_f5f7_76$ = -332
_f5f8_38$ = -476
_f5f9_76$ = -16
_f6f6_19$ = -160
_f6f7_38$ = -564
_f6f8_38$ = -96
_f6f9_38$ = -116
_f7f7_38$ = -52
_f7f8_38$ = -88
_f7f9_76$ = -80
_f8f8_19$ = -468
_f8f9_38$ = -152
_f9f9_38$ = -144
_h0$ = -240
_h1$ = -276
_h2$ = -308
_h3$ = -324
_h4$ = -368
_h5$ = -428
_h6$ = -460
_h7$ = -556
_h8$ = -600
_h9$ = -644
_carry0$ = -72
_carry1$ = -108
_carry2$ = -8
_carry3$ = -44
_carry4$ = -232
_carry5$ = -268
_carry6$ = -136
_carry7$ = -196
_carry8$ = -360
_carry9$ = -420
_fe_sq	PROC NEAR

; 557  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 692				; 000002b4H
	push	esi

; 558  :     int32_t f0 = f[0];

	mov	eax, DWORD PTR _f$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _f0$[ebp], ecx

; 559  :     int32_t f1 = f[1];

	mov	edx, DWORD PTR _f$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR _f1$[ebp], eax

; 560  :     int32_t f2 = f[2];

	mov	ecx, DWORD PTR _f$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	mov	DWORD PTR _f2$[ebp], edx

; 561  :     int32_t f3 = f[3];

	mov	eax, DWORD PTR _f$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	DWORD PTR _f3$[ebp], ecx

; 562  :     int32_t f4 = f[4];

	mov	edx, DWORD PTR _f$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	DWORD PTR _f4$[ebp], eax

; 563  :     int32_t f5 = f[5];

	mov	ecx, DWORD PTR _f$[ebp]
	mov	edx, DWORD PTR [ecx+20]
	mov	DWORD PTR _f5$[ebp], edx

; 564  :     int32_t f6 = f[6];

	mov	eax, DWORD PTR _f$[ebp]
	mov	ecx, DWORD PTR [eax+24]
	mov	DWORD PTR _f6$[ebp], ecx

; 565  :     int32_t f7 = f[7];

	mov	edx, DWORD PTR _f$[ebp]
	mov	eax, DWORD PTR [edx+28]
	mov	DWORD PTR _f7$[ebp], eax

; 566  :     int32_t f8 = f[8];

	mov	ecx, DWORD PTR _f$[ebp]
	mov	edx, DWORD PTR [ecx+32]
	mov	DWORD PTR _f8$[ebp], edx

; 567  :     int32_t f9 = f[9];

	mov	eax, DWORD PTR _f$[ebp]
	mov	ecx, DWORD PTR [eax+36]
	mov	DWORD PTR _f9$[ebp], ecx

; 568  :     int32_t f0_2 = 2 * f0;

	mov	edx, DWORD PTR _f0$[ebp]
	shl	edx, 1
	mov	DWORD PTR _f0_2$[ebp], edx

; 569  :     int32_t f1_2 = 2 * f1;

	mov	eax, DWORD PTR _f1$[ebp]
	shl	eax, 1
	mov	DWORD PTR _f1_2$[ebp], eax

; 570  :     int32_t f2_2 = 2 * f2;

	mov	ecx, DWORD PTR _f2$[ebp]
	shl	ecx, 1
	mov	DWORD PTR _f2_2$[ebp], ecx

; 571  :     int32_t f3_2 = 2 * f3;

	mov	edx, DWORD PTR _f3$[ebp]
	shl	edx, 1
	mov	DWORD PTR _f3_2$[ebp], edx

; 572  :     int32_t f4_2 = 2 * f4;

	mov	eax, DWORD PTR _f4$[ebp]
	shl	eax, 1
	mov	DWORD PTR _f4_2$[ebp], eax

; 573  :     int32_t f5_2 = 2 * f5;

	mov	ecx, DWORD PTR _f5$[ebp]
	shl	ecx, 1
	mov	DWORD PTR _f5_2$[ebp], ecx

; 574  :     int32_t f6_2 = 2 * f6;

	mov	edx, DWORD PTR _f6$[ebp]
	shl	edx, 1
	mov	DWORD PTR _f6_2$[ebp], edx

; 575  :     int32_t f7_2 = 2 * f7;

	mov	eax, DWORD PTR _f7$[ebp]
	shl	eax, 1
	mov	DWORD PTR _f7_2$[ebp], eax

; 576  :     int32_t f5_38 = 38 * f5; /* 1.959375*2^30 */

	mov	ecx, DWORD PTR _f5$[ebp]
	imul	ecx, 38					; 00000026H
	mov	DWORD PTR _f5_38$[ebp], ecx

; 577  :     int32_t f6_19 = 19 * f6; /* 1.959375*2^30 */

	mov	edx, DWORD PTR _f6$[ebp]
	imul	edx, 19					; 00000013H
	mov	DWORD PTR _f6_19$[ebp], edx

; 578  :     int32_t f7_38 = 38 * f7; /* 1.959375*2^30 */

	mov	eax, DWORD PTR _f7$[ebp]
	imul	eax, 38					; 00000026H
	mov	DWORD PTR _f7_38$[ebp], eax

; 579  :     int32_t f8_19 = 19 * f8; /* 1.959375*2^30 */

	mov	ecx, DWORD PTR _f8$[ebp]
	imul	ecx, 19					; 00000013H
	mov	DWORD PTR _f8_19$[ebp], ecx

; 580  :     int32_t f9_38 = 38 * f9; /* 1.959375*2^30 */

	mov	edx, DWORD PTR _f9$[ebp]
	imul	edx, 38					; 00000026H
	mov	DWORD PTR _f9_38$[ebp], edx

; 581  :     int64_t f0f0 = f0 * (int64_t)f0;

	mov	eax, DWORD PTR _f0$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f0$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f0$[ebp], eax
	mov	DWORD PTR _f0f0$[ebp+4], edx

; 582  :     int64_t f0f1_2 = f0_2 * (int64_t)f1;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f1$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f1_2$[ebp], eax
	mov	DWORD PTR _f0f1_2$[ebp+4], edx

; 583  :     int64_t f0f2_2 = f0_2 * (int64_t)f2;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f2_2$[ebp], eax
	mov	DWORD PTR _f0f2_2$[ebp+4], edx

; 584  :     int64_t f0f3_2 = f0_2 * (int64_t)f3;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f3$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f3_2$[ebp], eax
	mov	DWORD PTR _f0f3_2$[ebp+4], edx

; 585  :     int64_t f0f4_2 = f0_2 * (int64_t)f4;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f4_2$[ebp], eax
	mov	DWORD PTR _f0f4_2$[ebp+4], edx

; 586  :     int64_t f0f5_2 = f0_2 * (int64_t)f5;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f5$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f5_2$[ebp], eax
	mov	DWORD PTR _f0f5_2$[ebp+4], edx

; 587  :     int64_t f0f6_2 = f0_2 * (int64_t)f6;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f6_2$[ebp], eax
	mov	DWORD PTR _f0f6_2$[ebp+4], edx

; 588  :     int64_t f0f7_2 = f0_2 * (int64_t)f7;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f7$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f7_2$[ebp], eax
	mov	DWORD PTR _f0f7_2$[ebp+4], edx

; 589  :     int64_t f0f8_2 = f0_2 * (int64_t)f8;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f8_2$[ebp], eax
	mov	DWORD PTR _f0f8_2$[ebp+4], edx

; 590  :     int64_t f0f9_2 = f0_2 * (int64_t)f9;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f9_2$[ebp], eax
	mov	DWORD PTR _f0f9_2$[ebp+4], edx

; 591  :     int64_t f1f1_2 = f1_2 * (int64_t)f1;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f1$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f1_2$[ebp], eax
	mov	DWORD PTR _f1f1_2$[ebp+4], edx

; 592  :     int64_t f1f2_2 = f1_2 * (int64_t)f2;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f2_2$[ebp], eax
	mov	DWORD PTR _f1f2_2$[ebp+4], edx

; 593  :     int64_t f1f3_4 = f1_2 * (int64_t)f3_2;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f3_4$[ebp], eax
	mov	DWORD PTR _f1f3_4$[ebp+4], edx

; 594  :     int64_t f1f4_2 = f1_2 * (int64_t)f4;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f4_2$[ebp], eax
	mov	DWORD PTR _f1f4_2$[ebp+4], edx

; 595  :     int64_t f1f5_4 = f1_2 * (int64_t)f5_2;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f5_2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f5_4$[ebp], eax
	mov	DWORD PTR _f1f5_4$[ebp+4], edx

; 596  :     int64_t f1f6_2 = f1_2 * (int64_t)f6;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f6_2$[ebp], eax
	mov	DWORD PTR _f1f6_2$[ebp+4], edx

; 597  :     int64_t f1f7_4 = f1_2 * (int64_t)f7_2;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f7_2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f7_4$[ebp], eax
	mov	DWORD PTR _f1f7_4$[ebp+4], edx

; 598  :     int64_t f1f8_2 = f1_2 * (int64_t)f8;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f8_2$[ebp], eax
	mov	DWORD PTR _f1f8_2$[ebp+4], edx

; 599  :     int64_t f1f9_76 = f1_2 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f9_76$[ebp], eax
	mov	DWORD PTR _f1f9_76$[ebp+4], edx

; 600  :     int64_t f2f2 = f2 * (int64_t)f2;

	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2f2$[ebp], eax
	mov	DWORD PTR _f2f2$[ebp+4], edx

; 601  :     int64_t f2f3_2 = f2_2 * (int64_t)f3;

	mov	eax, DWORD PTR _f2_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f3$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2f3_2$[ebp], eax
	mov	DWORD PTR _f2f3_2$[ebp+4], edx

; 602  :     int64_t f2f4_2 = f2_2 * (int64_t)f4;

	mov	eax, DWORD PTR _f2_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2f4_2$[ebp], eax
	mov	DWORD PTR _f2f4_2$[ebp+4], edx

; 603  :     int64_t f2f5_2 = f2_2 * (int64_t)f5;

	mov	eax, DWORD PTR _f2_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f5$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2f5_2$[ebp], eax
	mov	DWORD PTR _f2f5_2$[ebp+4], edx

; 604  :     int64_t f2f6_2 = f2_2 * (int64_t)f6;

	mov	eax, DWORD PTR _f2_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2f6_2$[ebp], eax
	mov	DWORD PTR _f2f6_2$[ebp+4], edx

; 605  :     int64_t f2f7_2 = f2_2 * (int64_t)f7;

	mov	eax, DWORD PTR _f2_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f7$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2f7_2$[ebp], eax
	mov	DWORD PTR _f2f7_2$[ebp+4], edx

; 606  :     int64_t f2f8_38 = f2_2 * (int64_t)f8_19;

	mov	eax, DWORD PTR _f2_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2f8_38$[ebp], eax
	mov	DWORD PTR _f2f8_38$[ebp+4], edx

; 607  :     int64_t f2f9_38 = f2 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2f9_38$[ebp], eax
	mov	DWORD PTR _f2f9_38$[ebp+4], edx

; 608  :     int64_t f3f3_2 = f3_2 * (int64_t)f3;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f3$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3f3_2$[ebp], eax
	mov	DWORD PTR _f3f3_2$[ebp+4], edx

; 609  :     int64_t f3f4_2 = f3_2 * (int64_t)f4;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3f4_2$[ebp], eax
	mov	DWORD PTR _f3f4_2$[ebp+4], edx

; 610  :     int64_t f3f5_4 = f3_2 * (int64_t)f5_2;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f5_2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3f5_4$[ebp], eax
	mov	DWORD PTR _f3f5_4$[ebp+4], edx

; 611  :     int64_t f3f6_2 = f3_2 * (int64_t)f6;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3f6_2$[ebp], eax
	mov	DWORD PTR _f3f6_2$[ebp+4], edx

; 612  :     int64_t f3f7_76 = f3_2 * (int64_t)f7_38;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f7_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3f7_76$[ebp], eax
	mov	DWORD PTR _f3f7_76$[ebp+4], edx

; 613  :     int64_t f3f8_38 = f3_2 * (int64_t)f8_19;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3f8_38$[ebp], eax
	mov	DWORD PTR _f3f8_38$[ebp+4], edx

; 614  :     int64_t f3f9_76 = f3_2 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3f9_76$[ebp], eax
	mov	DWORD PTR _f3f9_76$[ebp+4], edx

; 615  :     int64_t f4f4 = f4 * (int64_t)f4;

	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4f4$[ebp], eax
	mov	DWORD PTR _f4f4$[ebp+4], edx

; 616  :     int64_t f4f5_2 = f4_2 * (int64_t)f5;

	mov	eax, DWORD PTR _f4_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f5$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4f5_2$[ebp], eax
	mov	DWORD PTR _f4f5_2$[ebp+4], edx

; 617  :     int64_t f4f6_38 = f4_2 * (int64_t)f6_19;

	mov	eax, DWORD PTR _f4_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f6_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4f6_38$[ebp], eax
	mov	DWORD PTR _f4f6_38$[ebp+4], edx

; 618  :     int64_t f4f7_38 = f4 * (int64_t)f7_38;

	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f7_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4f7_38$[ebp], eax
	mov	DWORD PTR _f4f7_38$[ebp+4], edx

; 619  :     int64_t f4f8_38 = f4_2 * (int64_t)f8_19;

	mov	eax, DWORD PTR _f4_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4f8_38$[ebp], eax
	mov	DWORD PTR _f4f8_38$[ebp+4], edx

; 620  :     int64_t f4f9_38 = f4 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4f9_38$[ebp], eax
	mov	DWORD PTR _f4f9_38$[ebp+4], edx

; 621  :     int64_t f5f5_38 = f5 * (int64_t)f5_38;

	mov	eax, DWORD PTR _f5$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f5_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5f5_38$[ebp], eax
	mov	DWORD PTR _f5f5_38$[ebp+4], edx

; 622  :     int64_t f5f6_38 = f5_2 * (int64_t)f6_19;

	mov	eax, DWORD PTR _f5_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f6_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5f6_38$[ebp], eax
	mov	DWORD PTR _f5f6_38$[ebp+4], edx

; 623  :     int64_t f5f7_76 = f5_2 * (int64_t)f7_38;

	mov	eax, DWORD PTR _f5_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f7_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5f7_76$[ebp], eax
	mov	DWORD PTR _f5f7_76$[ebp+4], edx

; 624  :     int64_t f5f8_38 = f5_2 * (int64_t)f8_19;

	mov	eax, DWORD PTR _f5_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5f8_38$[ebp], eax
	mov	DWORD PTR _f5f8_38$[ebp+4], edx

; 625  :     int64_t f5f9_76 = f5_2 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f5_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5f9_76$[ebp], eax
	mov	DWORD PTR _f5f9_76$[ebp+4], edx

; 626  :     int64_t f6f6_19 = f6 * (int64_t)f6_19;

	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f6_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6f6_19$[ebp], eax
	mov	DWORD PTR _f6f6_19$[ebp+4], edx

; 627  :     int64_t f6f7_38 = f6 * (int64_t)f7_38;

	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f7_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6f7_38$[ebp], eax
	mov	DWORD PTR _f6f7_38$[ebp+4], edx

; 628  :     int64_t f6f8_38 = f6_2 * (int64_t)f8_19;

	mov	eax, DWORD PTR _f6_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6f8_38$[ebp], eax
	mov	DWORD PTR _f6f8_38$[ebp+4], edx

; 629  :     int64_t f6f9_38 = f6 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6f9_38$[ebp], eax
	mov	DWORD PTR _f6f9_38$[ebp+4], edx

; 630  :     int64_t f7f7_38 = f7 * (int64_t)f7_38;

	mov	eax, DWORD PTR _f7$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f7_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f7f7_38$[ebp], eax
	mov	DWORD PTR _f7f7_38$[ebp+4], edx

; 631  :     int64_t f7f8_38 = f7_2 * (int64_t)f8_19;

	mov	eax, DWORD PTR _f7_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f7f8_38$[ebp], eax
	mov	DWORD PTR _f7f8_38$[ebp+4], edx

; 632  :     int64_t f7f9_76 = f7_2 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f7_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f7f9_76$[ebp], eax
	mov	DWORD PTR _f7f9_76$[ebp+4], edx

; 633  :     int64_t f8f8_19 = f8 * (int64_t)f8_19;

	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f8f8_19$[ebp], eax
	mov	DWORD PTR _f8f8_19$[ebp+4], edx

; 634  :     int64_t f8f9_38 = f8 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f8f9_38$[ebp], eax
	mov	DWORD PTR _f8f9_38$[ebp+4], edx

; 635  :     int64_t f9f9_38 = f9 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f9$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f9f9_38$[ebp], eax
	mov	DWORD PTR _f9f9_38$[ebp+4], edx

; 636  :     int64_t h0 = f0f0 + f1f9_76 + f2f8_38 + f3f7_76 + f4f6_38 + f5f5_38;

	mov	edx, DWORD PTR _f0f0$[ebp]
	add	edx, DWORD PTR _f1f9_76$[ebp]
	mov	eax, DWORD PTR _f0f0$[ebp+4]
	adc	eax, DWORD PTR _f1f9_76$[ebp+4]
	add	edx, DWORD PTR _f2f8_38$[ebp]
	adc	eax, DWORD PTR _f2f8_38$[ebp+4]
	add	edx, DWORD PTR _f3f7_76$[ebp]
	adc	eax, DWORD PTR _f3f7_76$[ebp+4]
	add	edx, DWORD PTR _f4f6_38$[ebp]
	adc	eax, DWORD PTR _f4f6_38$[ebp+4]
	add	edx, DWORD PTR _f5f5_38$[ebp]
	adc	eax, DWORD PTR _f5f5_38$[ebp+4]
	mov	DWORD PTR _h0$[ebp], edx
	mov	DWORD PTR _h0$[ebp+4], eax

; 637  :     int64_t h1 = f0f1_2 + f2f9_38 + f3f8_38 + f4f7_38 + f5f6_38;

	mov	ecx, DWORD PTR _f0f1_2$[ebp]
	add	ecx, DWORD PTR _f2f9_38$[ebp]
	mov	edx, DWORD PTR _f0f1_2$[ebp+4]
	adc	edx, DWORD PTR _f2f9_38$[ebp+4]
	add	ecx, DWORD PTR _f3f8_38$[ebp]
	adc	edx, DWORD PTR _f3f8_38$[ebp+4]
	add	ecx, DWORD PTR _f4f7_38$[ebp]
	adc	edx, DWORD PTR _f4f7_38$[ebp+4]
	add	ecx, DWORD PTR _f5f6_38$[ebp]
	adc	edx, DWORD PTR _f5f6_38$[ebp+4]
	mov	DWORD PTR _h1$[ebp], ecx
	mov	DWORD PTR _h1$[ebp+4], edx

; 638  :     int64_t h2 = f0f2_2 + f1f1_2 + f3f9_76 + f4f8_38 + f5f7_76 + f6f6_19;

	mov	eax, DWORD PTR _f0f2_2$[ebp]
	add	eax, DWORD PTR _f1f1_2$[ebp]
	mov	ecx, DWORD PTR _f0f2_2$[ebp+4]
	adc	ecx, DWORD PTR _f1f1_2$[ebp+4]
	add	eax, DWORD PTR _f3f9_76$[ebp]
	adc	ecx, DWORD PTR _f3f9_76$[ebp+4]
	add	eax, DWORD PTR _f4f8_38$[ebp]
	adc	ecx, DWORD PTR _f4f8_38$[ebp+4]
	add	eax, DWORD PTR _f5f7_76$[ebp]
	adc	ecx, DWORD PTR _f5f7_76$[ebp+4]
	add	eax, DWORD PTR _f6f6_19$[ebp]
	adc	ecx, DWORD PTR _f6f6_19$[ebp+4]
	mov	DWORD PTR _h2$[ebp], eax
	mov	DWORD PTR _h2$[ebp+4], ecx

; 639  :     int64_t h3 = f0f3_2 + f1f2_2 + f4f9_38 + f5f8_38 + f6f7_38;

	mov	edx, DWORD PTR _f0f3_2$[ebp]
	add	edx, DWORD PTR _f1f2_2$[ebp]
	mov	eax, DWORD PTR _f0f3_2$[ebp+4]
	adc	eax, DWORD PTR _f1f2_2$[ebp+4]
	add	edx, DWORD PTR _f4f9_38$[ebp]
	adc	eax, DWORD PTR _f4f9_38$[ebp+4]
	add	edx, DWORD PTR _f5f8_38$[ebp]
	adc	eax, DWORD PTR _f5f8_38$[ebp+4]
	add	edx, DWORD PTR _f6f7_38$[ebp]
	adc	eax, DWORD PTR _f6f7_38$[ebp+4]
	mov	DWORD PTR _h3$[ebp], edx
	mov	DWORD PTR _h3$[ebp+4], eax

; 640  :     int64_t h4 = f0f4_2 + f1f3_4 + f2f2 + f5f9_76 + f6f8_38 + f7f7_38;

	mov	ecx, DWORD PTR _f0f4_2$[ebp]
	add	ecx, DWORD PTR _f1f3_4$[ebp]
	mov	edx, DWORD PTR _f0f4_2$[ebp+4]
	adc	edx, DWORD PTR _f1f3_4$[ebp+4]
	add	ecx, DWORD PTR _f2f2$[ebp]
	adc	edx, DWORD PTR _f2f2$[ebp+4]
	add	ecx, DWORD PTR _f5f9_76$[ebp]
	adc	edx, DWORD PTR _f5f9_76$[ebp+4]
	add	ecx, DWORD PTR _f6f8_38$[ebp]
	adc	edx, DWORD PTR _f6f8_38$[ebp+4]
	add	ecx, DWORD PTR _f7f7_38$[ebp]
	adc	edx, DWORD PTR _f7f7_38$[ebp+4]
	mov	DWORD PTR _h4$[ebp], ecx
	mov	DWORD PTR _h4$[ebp+4], edx

; 641  :     int64_t h5 = f0f5_2 + f1f4_2 + f2f3_2 + f6f9_38 + f7f8_38;

	mov	eax, DWORD PTR _f0f5_2$[ebp]
	add	eax, DWORD PTR _f1f4_2$[ebp]
	mov	ecx, DWORD PTR _f0f5_2$[ebp+4]
	adc	ecx, DWORD PTR _f1f4_2$[ebp+4]
	add	eax, DWORD PTR _f2f3_2$[ebp]
	adc	ecx, DWORD PTR _f2f3_2$[ebp+4]
	add	eax, DWORD PTR _f6f9_38$[ebp]
	adc	ecx, DWORD PTR _f6f9_38$[ebp+4]
	add	eax, DWORD PTR _f7f8_38$[ebp]
	adc	ecx, DWORD PTR _f7f8_38$[ebp+4]
	mov	DWORD PTR _h5$[ebp], eax
	mov	DWORD PTR _h5$[ebp+4], ecx

; 642  :     int64_t h6 = f0f6_2 + f1f5_4 + f2f4_2 + f3f3_2 + f7f9_76 + f8f8_19;

	mov	edx, DWORD PTR _f0f6_2$[ebp]
	add	edx, DWORD PTR _f1f5_4$[ebp]
	mov	eax, DWORD PTR _f0f6_2$[ebp+4]
	adc	eax, DWORD PTR _f1f5_4$[ebp+4]
	add	edx, DWORD PTR _f2f4_2$[ebp]
	adc	eax, DWORD PTR _f2f4_2$[ebp+4]
	add	edx, DWORD PTR _f3f3_2$[ebp]
	adc	eax, DWORD PTR _f3f3_2$[ebp+4]
	add	edx, DWORD PTR _f7f9_76$[ebp]
	adc	eax, DWORD PTR _f7f9_76$[ebp+4]
	add	edx, DWORD PTR _f8f8_19$[ebp]
	adc	eax, DWORD PTR _f8f8_19$[ebp+4]
	mov	DWORD PTR _h6$[ebp], edx
	mov	DWORD PTR _h6$[ebp+4], eax

; 643  :     int64_t h7 = f0f7_2 + f1f6_2 + f2f5_2 + f3f4_2 + f8f9_38;

	mov	ecx, DWORD PTR _f0f7_2$[ebp]
	add	ecx, DWORD PTR _f1f6_2$[ebp]
	mov	edx, DWORD PTR _f0f7_2$[ebp+4]
	adc	edx, DWORD PTR _f1f6_2$[ebp+4]
	add	ecx, DWORD PTR _f2f5_2$[ebp]
	adc	edx, DWORD PTR _f2f5_2$[ebp+4]
	add	ecx, DWORD PTR _f3f4_2$[ebp]
	adc	edx, DWORD PTR _f3f4_2$[ebp+4]
	add	ecx, DWORD PTR _f8f9_38$[ebp]
	adc	edx, DWORD PTR _f8f9_38$[ebp+4]
	mov	DWORD PTR _h7$[ebp], ecx
	mov	DWORD PTR _h7$[ebp+4], edx

; 644  :     int64_t h8 = f0f8_2 + f1f7_4 + f2f6_2 + f3f5_4 + f4f4 + f9f9_38;

	mov	eax, DWORD PTR _f0f8_2$[ebp]
	add	eax, DWORD PTR _f1f7_4$[ebp]
	mov	ecx, DWORD PTR _f0f8_2$[ebp+4]
	adc	ecx, DWORD PTR _f1f7_4$[ebp+4]
	add	eax, DWORD PTR _f2f6_2$[ebp]
	adc	ecx, DWORD PTR _f2f6_2$[ebp+4]
	add	eax, DWORD PTR _f3f5_4$[ebp]
	adc	ecx, DWORD PTR _f3f5_4$[ebp+4]
	add	eax, DWORD PTR _f4f4$[ebp]
	adc	ecx, DWORD PTR _f4f4$[ebp+4]
	add	eax, DWORD PTR _f9f9_38$[ebp]
	adc	ecx, DWORD PTR _f9f9_38$[ebp+4]
	mov	DWORD PTR _h8$[ebp], eax
	mov	DWORD PTR _h8$[ebp+4], ecx

; 645  :     int64_t h9 = f0f9_2 + f1f8_2 + f2f7_2 + f3f6_2 + f4f5_2;

	mov	edx, DWORD PTR _f0f9_2$[ebp]
	add	edx, DWORD PTR _f1f8_2$[ebp]
	mov	eax, DWORD PTR _f0f9_2$[ebp+4]
	adc	eax, DWORD PTR _f1f8_2$[ebp+4]
	add	edx, DWORD PTR _f2f7_2$[ebp]
	adc	eax, DWORD PTR _f2f7_2$[ebp+4]
	add	edx, DWORD PTR _f3f6_2$[ebp]
	adc	eax, DWORD PTR _f3f6_2$[ebp+4]
	add	edx, DWORD PTR _f4f5_2$[ebp]
	adc	eax, DWORD PTR _f4f5_2$[ebp+4]
	mov	DWORD PTR _h9$[ebp], edx
	mov	DWORD PTR _h9$[ebp+4], eax

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

	mov	ecx, DWORD PTR _h0$[ebp]
	add	ecx, 33554432				; 02000000H
	mov	edx, DWORD PTR _h0$[ebp+4]
	adc	edx, 0
	mov	DWORD PTR _carry0$[ebp], ecx
	mov	DWORD PTR _carry0$[ebp+4], edx

; 658  :     h1 += carry0 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry0$[ebp]
	mov	edx, DWORD PTR _carry0$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h1$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h1$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h1$[ebp], ecx
	mov	DWORD PTR _h1$[ebp+4], eax

; 659  :     h0 -= carry0 & kTop38Bits;

	mov	ecx, DWORD PTR _carry0$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry0$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h0$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h0$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h0$[ebp], eax
	mov	DWORD PTR _h0$[ebp+4], ecx

; 660  :     carry4 = h4 + (1 << 25);

	mov	edx, DWORD PTR _h4$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h4$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry4$[ebp], edx
	mov	DWORD PTR _carry4$[ebp+4], eax

; 661  :     h5 += carry4 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry4$[ebp]
	mov	edx, DWORD PTR _carry4$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h5$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h5$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h5$[ebp], ecx
	mov	DWORD PTR _h5$[ebp+4], eax

; 662  :     h4 -= carry4 & kTop38Bits;

	mov	ecx, DWORD PTR _carry4$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry4$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h4$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h4$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h4$[ebp], eax
	mov	DWORD PTR _h4$[ebp+4], ecx

; 663  : 
; 664  :     carry1 = h1 + (1 << 24);

	mov	edx, DWORD PTR _h1$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h1$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry1$[ebp], edx
	mov	DWORD PTR _carry1$[ebp+4], eax

; 665  :     h2 += carry1 >> 25;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry1$[ebp]
	mov	edx, DWORD PTR _carry1$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h2$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h2$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h2$[ebp], ecx
	mov	DWORD PTR _h2$[ebp+4], eax

; 666  :     h1 -= carry1 & kTop39Bits;

	mov	ecx, DWORD PTR _carry1$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry1$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h1$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h1$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h1$[ebp], eax
	mov	DWORD PTR _h1$[ebp+4], ecx

; 667  :     carry5 = h5 + (1 << 24);

	mov	edx, DWORD PTR _h5$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h5$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry5$[ebp], edx
	mov	DWORD PTR _carry5$[ebp+4], eax

; 668  :     h6 += carry5 >> 25;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry5$[ebp]
	mov	edx, DWORD PTR _carry5$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h6$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h6$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h6$[ebp], ecx
	mov	DWORD PTR _h6$[ebp+4], eax

; 669  :     h5 -= carry5 & kTop39Bits;

	mov	ecx, DWORD PTR _carry5$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry5$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h5$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h5$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h5$[ebp], eax
	mov	DWORD PTR _h5$[ebp+4], ecx

; 670  : 
; 671  :     carry2 = h2 + (1 << 25);

	mov	edx, DWORD PTR _h2$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h2$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry2$[ebp], edx
	mov	DWORD PTR _carry2$[ebp+4], eax

; 672  :     h3 += carry2 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry2$[ebp]
	mov	edx, DWORD PTR _carry2$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h3$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h3$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h3$[ebp], ecx
	mov	DWORD PTR _h3$[ebp+4], eax

; 673  :     h2 -= carry2 & kTop38Bits;

	mov	ecx, DWORD PTR _carry2$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry2$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h2$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h2$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h2$[ebp], eax
	mov	DWORD PTR _h2$[ebp+4], ecx

; 674  :     carry6 = h6 + (1 << 25);

	mov	edx, DWORD PTR _h6$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h6$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry6$[ebp], edx
	mov	DWORD PTR _carry6$[ebp+4], eax

; 675  :     h7 += carry6 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry6$[ebp]
	mov	edx, DWORD PTR _carry6$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h7$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h7$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h7$[ebp], ecx
	mov	DWORD PTR _h7$[ebp+4], eax

; 676  :     h6 -= carry6 & kTop38Bits;

	mov	ecx, DWORD PTR _carry6$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry6$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h6$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h6$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h6$[ebp], eax
	mov	DWORD PTR _h6$[ebp+4], ecx

; 677  : 
; 678  :     carry3 = h3 + (1 << 24);

	mov	edx, DWORD PTR _h3$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h3$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry3$[ebp], edx
	mov	DWORD PTR _carry3$[ebp+4], eax

; 679  :     h4 += carry3 >> 25;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry3$[ebp]
	mov	edx, DWORD PTR _carry3$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h4$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h4$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h4$[ebp], ecx
	mov	DWORD PTR _h4$[ebp+4], eax

; 680  :     h3 -= carry3 & kTop39Bits;

	mov	ecx, DWORD PTR _carry3$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry3$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h3$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h3$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h3$[ebp], eax
	mov	DWORD PTR _h3$[ebp+4], ecx

; 681  :     carry7 = h7 + (1 << 24);

	mov	edx, DWORD PTR _h7$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h7$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry7$[ebp], edx
	mov	DWORD PTR _carry7$[ebp+4], eax

; 682  :     h8 += carry7 >> 25;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry7$[ebp]
	mov	edx, DWORD PTR _carry7$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h8$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h8$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h8$[ebp], ecx
	mov	DWORD PTR _h8$[ebp+4], eax

; 683  :     h7 -= carry7 & kTop39Bits;

	mov	ecx, DWORD PTR _carry7$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry7$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h7$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h7$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h7$[ebp], eax
	mov	DWORD PTR _h7$[ebp+4], ecx

; 684  : 
; 685  :     carry4 = h4 + (1 << 25);

	mov	edx, DWORD PTR _h4$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h4$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry4$[ebp], edx
	mov	DWORD PTR _carry4$[ebp+4], eax

; 686  :     h5 += carry4 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry4$[ebp]
	mov	edx, DWORD PTR _carry4$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h5$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h5$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h5$[ebp], ecx
	mov	DWORD PTR _h5$[ebp+4], eax

; 687  :     h4 -= carry4 & kTop38Bits;

	mov	ecx, DWORD PTR _carry4$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry4$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h4$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h4$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h4$[ebp], eax
	mov	DWORD PTR _h4$[ebp+4], ecx

; 688  :     carry8 = h8 + (1 << 25);

	mov	edx, DWORD PTR _h8$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h8$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry8$[ebp], edx
	mov	DWORD PTR _carry8$[ebp+4], eax

; 689  :     h9 += carry8 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry8$[ebp]
	mov	edx, DWORD PTR _carry8$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h9$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h9$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h9$[ebp], ecx
	mov	DWORD PTR _h9$[ebp+4], eax

; 690  :     h8 -= carry8 & kTop38Bits;

	mov	ecx, DWORD PTR _carry8$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry8$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h8$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h8$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h8$[ebp], eax
	mov	DWORD PTR _h8$[ebp+4], ecx

; 691  : 
; 692  :     carry9 = h9 + (1 << 24);

	mov	edx, DWORD PTR _h9$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h9$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry9$[ebp], edx
	mov	DWORD PTR _carry9$[ebp+4], eax

; 693  :     h0 += (carry9 >> 25) * 19;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry9$[ebp]
	mov	edx, DWORD PTR _carry9$[ebp+4]
	call	__allshr
	push	0
	push	19					; 00000013H
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR _h0$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h0$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h0$[ebp], ecx
	mov	DWORD PTR _h0$[ebp+4], eax

; 694  :     h9 -= carry9 & kTop39Bits;

	mov	ecx, DWORD PTR _carry9$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry9$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h9$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h9$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h9$[ebp], eax
	mov	DWORD PTR _h9$[ebp+4], ecx

; 695  : 
; 696  :     carry0 = h0 + (1 << 25);

	mov	edx, DWORD PTR _h0$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h0$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry0$[ebp], edx
	mov	DWORD PTR _carry0$[ebp+4], eax

; 697  :     h1 += carry0 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry0$[ebp]
	mov	edx, DWORD PTR _carry0$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h1$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h1$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h1$[ebp], ecx
	mov	DWORD PTR _h1$[ebp+4], eax

; 698  :     h0 -= carry0 & kTop38Bits;

	mov	ecx, DWORD PTR _carry0$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry0$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h0$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h0$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h0$[ebp], eax
	mov	DWORD PTR _h0$[ebp+4], ecx

; 699  : 
; 700  :     h[0] = (int32_t)h0;

	mov	edx, DWORD PTR _h0$[ebp]
	mov	eax, DWORD PTR _h$[ebp]
	mov	DWORD PTR [eax], edx

; 701  :     h[1] = (int32_t)h1;

	mov	ecx, DWORD PTR _h1$[ebp]
	mov	edx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [edx+4], ecx

; 702  :     h[2] = (int32_t)h2;

	mov	eax, DWORD PTR _h2$[ebp]
	mov	ecx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [ecx+8], eax

; 703  :     h[3] = (int32_t)h3;

	mov	edx, DWORD PTR _h3$[ebp]
	mov	eax, DWORD PTR _h$[ebp]
	mov	DWORD PTR [eax+12], edx

; 704  :     h[4] = (int32_t)h4;

	mov	ecx, DWORD PTR _h4$[ebp]
	mov	edx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [edx+16], ecx

; 705  :     h[5] = (int32_t)h5;

	mov	eax, DWORD PTR _h5$[ebp]
	mov	ecx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [ecx+20], eax

; 706  :     h[6] = (int32_t)h6;

	mov	edx, DWORD PTR _h6$[ebp]
	mov	eax, DWORD PTR _h$[ebp]
	mov	DWORD PTR [eax+24], edx

; 707  :     h[7] = (int32_t)h7;

	mov	ecx, DWORD PTR _h7$[ebp]
	mov	edx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [edx+28], ecx

; 708  :     h[8] = (int32_t)h8;

	mov	eax, DWORD PTR _h8$[ebp]
	mov	ecx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [ecx+32], eax

; 709  :     h[9] = (int32_t)h9;

	mov	edx, DWORD PTR _h9$[ebp]
	mov	eax, DWORD PTR _h$[ebp]
	mov	DWORD PTR [eax+36], edx

; 710  : }

	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	0
_fe_sq	ENDP
_f$ = 8
_s$ = -32
_fe_isnegative PROC NEAR

; 828  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 32					; 00000020H

; 829  :     uint8_t s[32];
; 830  : 
; 831  :     fe_tobytes(s, f);

	mov	eax, DWORD PTR _f$[ebp]
	push	eax
	lea	ecx, DWORD PTR _s$[ebp]
	push	ecx
	call	_fe_tobytes
	add	esp, 8

; 832  :     return s[0] & 1;

	mov	eax, DWORD PTR _s$[ebp]
	and	eax, 255				; 000000ffH
	and	eax, 1

; 833  : }

	mov	esp, ebp
	pop	ebp
	ret	0
_fe_isnegative ENDP
_s$ = 8
_h$ = 12
_recip$ = -120
_x$ = -40
_y$ = -80
_ge_p3_tobytes PROC NEAR

; 1074 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 120				; 00000078H

; 1075 :     fe recip;
; 1076 :     fe x;
; 1077 :     fe y;
; 1078 : 
; 1079 :     fe_invert(recip, h->Z);

	mov	eax, DWORD PTR _h$[ebp]
	add	eax, 80					; 00000050H
	push	eax
	lea	ecx, DWORD PTR _recip$[ebp]
	push	ecx
	call	_fe_invert
	add	esp, 8

; 1080 :     fe_mul(x, h->X, recip);

	lea	edx, DWORD PTR _recip$[ebp]
	push	edx
	mov	eax, DWORD PTR _h$[ebp]
	push	eax
	lea	ecx, DWORD PTR _x$[ebp]
	push	ecx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1081 :     fe_mul(y, h->Y, recip);

	lea	edx, DWORD PTR _recip$[ebp]
	push	edx
	mov	eax, DWORD PTR _h$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	lea	ecx, DWORD PTR _y$[ebp]
	push	ecx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1082 :     fe_tobytes(s, y);

	lea	edx, DWORD PTR _y$[ebp]
	push	edx
	mov	eax, DWORD PTR _s$[ebp]
	push	eax
	call	_fe_tobytes
	add	esp, 8

; 1083 :     s[31] ^= fe_isnegative(x) << 7;

	lea	ecx, DWORD PTR _x$[ebp]
	push	ecx
	call	_fe_isnegative
	add	esp, 4
	shl	eax, 7
	mov	edx, DWORD PTR _s$[ebp]
	mov	cl, BYTE PTR [edx+31]
	xor	cl, al
	mov	edx, DWORD PTR _s$[ebp]
	mov	BYTE PTR [edx+31], cl

; 1084 : }

	mov	esp, ebp
	pop	ebp
	ret	0
_ge_p3_tobytes ENDP
_h$ = 8
_s$ = 12
_u$ = -40
_v$ = -80
_w$ = -120
_vxx$ = -200
_check$ = -160
_ge_frombytes_vartime PROC NEAR

; 1097 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 200				; 000000c8H

; 1098 :     fe u;
; 1099 :     fe v;
; 1100 :     fe w;
; 1101 :     fe vxx;
; 1102 :     fe check;
; 1103 : 
; 1104 :     fe_frombytes(h->Y, s);

	mov	eax, DWORD PTR _s$[ebp]
	push	eax
	mov	ecx, DWORD PTR _h$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	call	_fe_frombytes
	add	esp, 8

; 1105 :     fe_1(h->Z);

	mov	edx, DWORD PTR _h$[ebp]
	add	edx, 80					; 00000050H
	push	edx
	call	_fe_1
	add	esp, 4

; 1106 :     fe_sq(u, h->Y);

	mov	eax, DWORD PTR _h$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	lea	ecx, DWORD PTR _u$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 1107 :     fe_mul(v, u, d);

	push	OFFSET FLAT:_d
	lea	edx, DWORD PTR _u$[ebp]
	push	edx
	lea	eax, DWORD PTR _v$[ebp]
	push	eax
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1108 :     fe_sub(u, u, h->Z); /* u = y^2-1 */

	mov	ecx, DWORD PTR _h$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	lea	edx, DWORD PTR _u$[ebp]
	push	edx
	lea	eax, DWORD PTR _u$[ebp]
	push	eax
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1109 :     fe_add(v, v, h->Z); /* v = dy^2+1 */

	mov	ecx, DWORD PTR _h$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	lea	edx, DWORD PTR _v$[ebp]
	push	edx
	lea	eax, DWORD PTR _v$[ebp]
	push	eax
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1110 : 
; 1111 :     fe_mul(w, u, v); /* w = u*v */

	lea	ecx, DWORD PTR _v$[ebp]
	push	ecx
	lea	edx, DWORD PTR _u$[ebp]
	push	edx
	lea	eax, DWORD PTR _w$[ebp]
	push	eax
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1112 : 
; 1113 :     fe_pow22523(h->X, w); /* x = w^((q-5)/8) */

	lea	ecx, DWORD PTR _w$[ebp]
	push	ecx
	mov	edx, DWORD PTR _h$[ebp]
	push	edx
	call	_fe_pow22523
	add	esp, 8

; 1114 :     fe_mul(h->X, h->X, u); /* x = u * w^((q-5)/8) */

	lea	eax, DWORD PTR _u$[ebp]
	push	eax
	mov	ecx, DWORD PTR _h$[ebp]
	push	ecx
	mov	edx, DWORD PTR _h$[ebp]
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1115 : 
; 1116 :     fe_sq(vxx, h->X);

	mov	eax, DWORD PTR _h$[ebp]
	push	eax
	lea	ecx, DWORD PTR _vxx$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 1117 :     fe_mul(vxx, vxx, v);

	lea	edx, DWORD PTR _v$[ebp]
	push	edx
	lea	eax, DWORD PTR _vxx$[ebp]
	push	eax
	lea	ecx, DWORD PTR _vxx$[ebp]
	push	ecx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1118 :     fe_sub(check, vxx, u); /* vx^2-u */

	lea	edx, DWORD PTR _u$[ebp]
	push	edx
	lea	eax, DWORD PTR _vxx$[ebp]
	push	eax
	lea	ecx, DWORD PTR _check$[ebp]
	push	ecx
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1119 :     if (fe_isnonzero(check)) {

	lea	edx, DWORD PTR _check$[ebp]
	push	edx
	call	_fe_isnonzero
	add	esp, 4
	test	eax, eax
	je	SHORT $L1799

; 1120 :         fe_add(check, vxx, u); /* vx^2+u */

	lea	eax, DWORD PTR _u$[ebp]
	push	eax
	lea	ecx, DWORD PTR _vxx$[ebp]
	push	ecx
	lea	edx, DWORD PTR _check$[ebp]
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1121 :         if (fe_isnonzero(check)) {

	lea	eax, DWORD PTR _check$[ebp]
	push	eax
	call	_fe_isnonzero
	add	esp, 4
	test	eax, eax
	je	SHORT $L1800

; 1122 :             return -1;

	or	eax, -1
	jmp	SHORT $L1793
$L1800:

; 1124 :         fe_mul(h->X, h->X, sqrtm1);

	push	OFFSET FLAT:_sqrtm1
	mov	ecx, DWORD PTR _h$[ebp]
	push	ecx
	mov	edx, DWORD PTR _h$[ebp]
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH
$L1799:

; 1126 : 
; 1127 :     if (fe_isnegative(h->X) != (s[31] >> 7)) {

	mov	eax, DWORD PTR _h$[ebp]
	push	eax
	call	_fe_isnegative
	add	esp, 4
	mov	ecx, DWORD PTR _s$[ebp]
	xor	edx, edx
	mov	dl, BYTE PTR [ecx+31]
	sar	edx, 7
	cmp	eax, edx
	je	SHORT $L1801

; 1128 :         fe_neg(h->X, h->X);

	mov	eax, DWORD PTR _h$[ebp]
	push	eax
	mov	ecx, DWORD PTR _h$[ebp]
	push	ecx
	call	_fe_neg
	add	esp, 8
$L1801:

; 1130 : 
; 1131 :     fe_mul(h->T, h->X, h->Y);

	mov	edx, DWORD PTR _h$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	mov	eax, DWORD PTR _h$[ebp]
	push	eax
	mov	ecx, DWORD PTR _h$[ebp]
	add	ecx, 120				; 00000078H
	push	ecx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1132 :     return 0;

	xor	eax, eax
$L1793:

; 1133 : }

	mov	esp, ebp
	pop	ebp
	ret	0
_ge_frombytes_vartime ENDP
_TEXT	ENDS
EXTRN	__allshl:NEAR
_TEXT	SEGMENT
_h$ = 8
_s$ = 12
_h0$ = -64
_h1$ = -80
_h2$ = -88
_h3$ = -96
_h4$ = -112
_h5$ = -128
_h6$ = -136
_h7$ = -144
_h8$ = -152
_h9$ = -160
_carry0$ = -24
_carry1$ = -32
_carry2$ = -8
_carry3$ = -16
_carry4$ = -56
_carry5$ = -72
_carry6$ = -40
_carry7$ = -48
_carry8$ = -104
_carry9$ = -120
_fe_frombytes PROC NEAR

; 123  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 160				; 000000a0H

; 124  :     /* Ignores top bit of h. */
; 125  :     int64_t h0 = load_4(s);

	mov	eax, DWORD PTR _s$[ebp]
	push	eax
	call	_load_4
	add	esp, 4
	mov	DWORD PTR _h0$[ebp], eax
	mov	DWORD PTR _h0$[ebp+4], edx

; 126  :     int64_t h1 = load_3(s + 4) << 6;

	mov	ecx, DWORD PTR _s$[ebp]
	add	ecx, 4
	push	ecx
	call	_load_3
	add	esp, 4
	mov	ecx, 6
	call	__allshl
	mov	DWORD PTR _h1$[ebp], eax
	mov	DWORD PTR _h1$[ebp+4], edx

; 127  :     int64_t h2 = load_3(s + 7) << 5;

	mov	edx, DWORD PTR _s$[ebp]
	add	edx, 7
	push	edx
	call	_load_3
	add	esp, 4
	mov	ecx, 5
	call	__allshl
	mov	DWORD PTR _h2$[ebp], eax
	mov	DWORD PTR _h2$[ebp+4], edx

; 128  :     int64_t h3 = load_3(s + 10) << 3;

	mov	eax, DWORD PTR _s$[ebp]
	add	eax, 10					; 0000000aH
	push	eax
	call	_load_3
	add	esp, 4
	mov	ecx, 3
	call	__allshl
	mov	DWORD PTR _h3$[ebp], eax
	mov	DWORD PTR _h3$[ebp+4], edx

; 129  :     int64_t h4 = load_3(s + 13) << 2;

	mov	ecx, DWORD PTR _s$[ebp]
	add	ecx, 13					; 0000000dH
	push	ecx
	call	_load_3
	add	esp, 4
	mov	ecx, 2
	call	__allshl
	mov	DWORD PTR _h4$[ebp], eax
	mov	DWORD PTR _h4$[ebp+4], edx

; 130  :     int64_t h5 = load_4(s + 16);

	mov	edx, DWORD PTR _s$[ebp]
	add	edx, 16					; 00000010H
	push	edx
	call	_load_4
	add	esp, 4
	mov	DWORD PTR _h5$[ebp], eax
	mov	DWORD PTR _h5$[ebp+4], edx

; 131  :     int64_t h6 = load_3(s + 20) << 7;

	mov	eax, DWORD PTR _s$[ebp]
	add	eax, 20					; 00000014H
	push	eax
	call	_load_3
	add	esp, 4
	mov	ecx, 7
	call	__allshl
	mov	DWORD PTR _h6$[ebp], eax
	mov	DWORD PTR _h6$[ebp+4], edx

; 132  :     int64_t h7 = load_3(s + 23) << 5;

	mov	ecx, DWORD PTR _s$[ebp]
	add	ecx, 23					; 00000017H
	push	ecx
	call	_load_3
	add	esp, 4
	mov	ecx, 5
	call	__allshl
	mov	DWORD PTR _h7$[ebp], eax
	mov	DWORD PTR _h7$[ebp+4], edx

; 133  :     int64_t h8 = load_3(s + 26) << 4;

	mov	edx, DWORD PTR _s$[ebp]
	add	edx, 26					; 0000001aH
	push	edx
	call	_load_3
	add	esp, 4
	mov	ecx, 4
	call	__allshl
	mov	DWORD PTR _h8$[ebp], eax
	mov	DWORD PTR _h8$[ebp+4], edx

; 134  :     int64_t h9 = (load_3(s + 29) & 0x7fffff) << 2;

	mov	eax, DWORD PTR _s$[ebp]
	add	eax, 29					; 0000001dH
	push	eax
	call	_load_3
	add	esp, 4
	and	eax, 8388607				; 007fffffH
	and	edx, 0
	mov	ecx, 2
	call	__allshl
	mov	DWORD PTR _h9$[ebp], eax
	mov	DWORD PTR _h9$[ebp+4], edx

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

	mov	ecx, DWORD PTR _h9$[ebp]
	add	ecx, 16777216				; 01000000H
	mov	edx, DWORD PTR _h9$[ebp+4]
	adc	edx, 0
	mov	DWORD PTR _carry9$[ebp], ecx
	mov	DWORD PTR _carry9$[ebp+4], edx

; 147  :     h0 += (carry9 >> 25) * 19;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry9$[ebp]
	mov	edx, DWORD PTR _carry9$[ebp+4]
	call	__allshr
	push	0
	push	19					; 00000013H
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR _h0$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h0$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h0$[ebp], ecx
	mov	DWORD PTR _h0$[ebp+4], eax

; 148  :     h9 -= carry9 & kTop39Bits;

	mov	ecx, DWORD PTR _carry9$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry9$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h9$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h9$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h9$[ebp], eax
	mov	DWORD PTR _h9$[ebp+4], ecx

; 149  :     carry1 = h1 + (1 << 24);

	mov	edx, DWORD PTR _h1$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h1$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry1$[ebp], edx
	mov	DWORD PTR _carry1$[ebp+4], eax

; 150  :     h2 += carry1 >> 25;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry1$[ebp]
	mov	edx, DWORD PTR _carry1$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h2$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h2$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h2$[ebp], ecx
	mov	DWORD PTR _h2$[ebp+4], eax

; 151  :     h1 -= carry1 & kTop39Bits;

	mov	ecx, DWORD PTR _carry1$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry1$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h1$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h1$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h1$[ebp], eax
	mov	DWORD PTR _h1$[ebp+4], ecx

; 152  :     carry3 = h3 + (1 << 24);

	mov	edx, DWORD PTR _h3$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h3$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry3$[ebp], edx
	mov	DWORD PTR _carry3$[ebp+4], eax

; 153  :     h4 += carry3 >> 25;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry3$[ebp]
	mov	edx, DWORD PTR _carry3$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h4$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h4$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h4$[ebp], ecx
	mov	DWORD PTR _h4$[ebp+4], eax

; 154  :     h3 -= carry3 & kTop39Bits;

	mov	ecx, DWORD PTR _carry3$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry3$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h3$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h3$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h3$[ebp], eax
	mov	DWORD PTR _h3$[ebp+4], ecx

; 155  :     carry5 = h5 + (1 << 24);

	mov	edx, DWORD PTR _h5$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h5$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry5$[ebp], edx
	mov	DWORD PTR _carry5$[ebp+4], eax

; 156  :     h6 += carry5 >> 25;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry5$[ebp]
	mov	edx, DWORD PTR _carry5$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h6$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h6$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h6$[ebp], ecx
	mov	DWORD PTR _h6$[ebp+4], eax

; 157  :     h5 -= carry5 & kTop39Bits;

	mov	ecx, DWORD PTR _carry5$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry5$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h5$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h5$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h5$[ebp], eax
	mov	DWORD PTR _h5$[ebp+4], ecx

; 158  :     carry7 = h7 + (1 << 24);

	mov	edx, DWORD PTR _h7$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h7$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry7$[ebp], edx
	mov	DWORD PTR _carry7$[ebp+4], eax

; 159  :     h8 += carry7 >> 25;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry7$[ebp]
	mov	edx, DWORD PTR _carry7$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h8$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h8$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h8$[ebp], ecx
	mov	DWORD PTR _h8$[ebp+4], eax

; 160  :     h7 -= carry7 & kTop39Bits;

	mov	ecx, DWORD PTR _carry7$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry7$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h7$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h7$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h7$[ebp], eax
	mov	DWORD PTR _h7$[ebp+4], ecx

; 161  : 
; 162  :     carry0 = h0 + (1 << 25);

	mov	edx, DWORD PTR _h0$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h0$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry0$[ebp], edx
	mov	DWORD PTR _carry0$[ebp+4], eax

; 163  :     h1 += carry0 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry0$[ebp]
	mov	edx, DWORD PTR _carry0$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h1$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h1$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h1$[ebp], ecx
	mov	DWORD PTR _h1$[ebp+4], eax

; 164  :     h0 -= carry0 & kTop38Bits;

	mov	ecx, DWORD PTR _carry0$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry0$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h0$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h0$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h0$[ebp], eax
	mov	DWORD PTR _h0$[ebp+4], ecx

; 165  :     carry2 = h2 + (1 << 25);

	mov	edx, DWORD PTR _h2$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h2$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry2$[ebp], edx
	mov	DWORD PTR _carry2$[ebp+4], eax

; 166  :     h3 += carry2 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry2$[ebp]
	mov	edx, DWORD PTR _carry2$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h3$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h3$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h3$[ebp], ecx
	mov	DWORD PTR _h3$[ebp+4], eax

; 167  :     h2 -= carry2 & kTop38Bits;

	mov	ecx, DWORD PTR _carry2$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry2$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h2$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h2$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h2$[ebp], eax
	mov	DWORD PTR _h2$[ebp+4], ecx

; 168  :     carry4 = h4 + (1 << 25);

	mov	edx, DWORD PTR _h4$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h4$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry4$[ebp], edx
	mov	DWORD PTR _carry4$[ebp+4], eax

; 169  :     h5 += carry4 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry4$[ebp]
	mov	edx, DWORD PTR _carry4$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h5$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h5$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h5$[ebp], ecx
	mov	DWORD PTR _h5$[ebp+4], eax

; 170  :     h4 -= carry4 & kTop38Bits;

	mov	ecx, DWORD PTR _carry4$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry4$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h4$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h4$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h4$[ebp], eax
	mov	DWORD PTR _h4$[ebp+4], ecx

; 171  :     carry6 = h6 + (1 << 25);

	mov	edx, DWORD PTR _h6$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h6$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry6$[ebp], edx
	mov	DWORD PTR _carry6$[ebp+4], eax

; 172  :     h7 += carry6 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry6$[ebp]
	mov	edx, DWORD PTR _carry6$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h7$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h7$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h7$[ebp], ecx
	mov	DWORD PTR _h7$[ebp+4], eax

; 173  :     h6 -= carry6 & kTop38Bits;

	mov	ecx, DWORD PTR _carry6$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry6$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h6$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h6$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h6$[ebp], eax
	mov	DWORD PTR _h6$[ebp+4], ecx

; 174  :     carry8 = h8 + (1 << 25);

	mov	edx, DWORD PTR _h8$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h8$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry8$[ebp], edx
	mov	DWORD PTR _carry8$[ebp+4], eax

; 175  :     h9 += carry8 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry8$[ebp]
	mov	edx, DWORD PTR _carry8$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h9$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h9$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h9$[ebp], ecx
	mov	DWORD PTR _h9$[ebp+4], eax

; 176  :     h8 -= carry8 & kTop38Bits;

	mov	ecx, DWORD PTR _carry8$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry8$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h8$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h8$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h8$[ebp], eax
	mov	DWORD PTR _h8$[ebp+4], ecx

; 177  : 
; 178  :     h[0] = (int32_t)h0;

	mov	edx, DWORD PTR _h0$[ebp]
	mov	eax, DWORD PTR _h$[ebp]
	mov	DWORD PTR [eax], edx

; 179  :     h[1] = (int32_t)h1;

	mov	ecx, DWORD PTR _h1$[ebp]
	mov	edx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [edx+4], ecx

; 180  :     h[2] = (int32_t)h2;

	mov	eax, DWORD PTR _h2$[ebp]
	mov	ecx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [ecx+8], eax

; 181  :     h[3] = (int32_t)h3;

	mov	edx, DWORD PTR _h3$[ebp]
	mov	eax, DWORD PTR _h$[ebp]
	mov	DWORD PTR [eax+12], edx

; 182  :     h[4] = (int32_t)h4;

	mov	ecx, DWORD PTR _h4$[ebp]
	mov	edx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [edx+16], ecx

; 183  :     h[5] = (int32_t)h5;

	mov	eax, DWORD PTR _h5$[ebp]
	mov	ecx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [ecx+20], eax

; 184  :     h[6] = (int32_t)h6;

	mov	edx, DWORD PTR _h6$[ebp]
	mov	eax, DWORD PTR _h$[ebp]
	mov	DWORD PTR [eax+24], edx

; 185  :     h[7] = (int32_t)h7;

	mov	ecx, DWORD PTR _h7$[ebp]
	mov	edx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [edx+28], ecx

; 186  :     h[8] = (int32_t)h8;

	mov	eax, DWORD PTR _h8$[ebp]
	mov	ecx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [ecx+32], eax

; 187  :     h[9] = (int32_t)h9;

	mov	edx, DWORD PTR _h9$[ebp]
	mov	eax, DWORD PTR _h$[ebp]
	mov	DWORD PTR [eax+36], edx

; 188  : }

	mov	esp, ebp
	pop	ebp
	ret	0
_fe_frombytes ENDP
_in$ = 8
_result$ = -8
_load_3	PROC NEAR

; 102  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8

; 103  :     uint64_t result;
; 104  : 
; 105  :     result = ((uint64_t)in[0]);

	mov	eax, DWORD PTR _in$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax]
	mov	eax, ecx
	cdq
	mov	DWORD PTR _result$[ebp], eax
	mov	DWORD PTR _result$[ebp+4], edx

; 106  :     result |= ((uint64_t)in[1]) << 8;

	mov	edx, DWORD PTR _in$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+1]
	cdq
	mov	ecx, 8
	call	__allshl
	mov	ecx, DWORD PTR _result$[ebp]
	or	ecx, eax
	mov	eax, DWORD PTR _result$[ebp+4]
	or	eax, edx
	mov	DWORD PTR _result$[ebp], ecx
	mov	DWORD PTR _result$[ebp+4], eax

; 107  :     result |= ((uint64_t)in[2]) << 16;

	mov	ecx, DWORD PTR _in$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [ecx+2]
	cdq
	mov	ecx, 16					; 00000010H
	call	__allshl
	mov	ecx, DWORD PTR _result$[ebp]
	or	ecx, eax
	mov	eax, DWORD PTR _result$[ebp+4]
	or	eax, edx
	mov	DWORD PTR _result$[ebp], ecx
	mov	DWORD PTR _result$[ebp+4], eax

; 108  :     return result;

	mov	eax, DWORD PTR _result$[ebp]
	mov	edx, DWORD PTR _result$[ebp+4]

; 109  : }

	mov	esp, ebp
	pop	ebp
	ret	0
_load_3	ENDP
_in$ = 8
_result$ = -8
_load_4	PROC NEAR

; 112  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 8

; 113  :     uint64_t result;
; 114  : 
; 115  :     result = ((uint64_t)in[0]);

	mov	eax, DWORD PTR _in$[ebp]
	xor	ecx, ecx
	mov	cl, BYTE PTR [eax]
	mov	eax, ecx
	cdq
	mov	DWORD PTR _result$[ebp], eax
	mov	DWORD PTR _result$[ebp+4], edx

; 116  :     result |= ((uint64_t)in[1]) << 8;

	mov	edx, DWORD PTR _in$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+1]
	cdq
	mov	ecx, 8
	call	__allshl
	mov	ecx, DWORD PTR _result$[ebp]
	or	ecx, eax
	mov	eax, DWORD PTR _result$[ebp+4]
	or	eax, edx
	mov	DWORD PTR _result$[ebp], ecx
	mov	DWORD PTR _result$[ebp+4], eax

; 117  :     result |= ((uint64_t)in[2]) << 16;

	mov	ecx, DWORD PTR _in$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [ecx+2]
	cdq
	mov	ecx, 16					; 00000010H
	call	__allshl
	mov	ecx, DWORD PTR _result$[ebp]
	or	ecx, eax
	mov	eax, DWORD PTR _result$[ebp+4]
	or	eax, edx
	mov	DWORD PTR _result$[ebp], ecx
	mov	DWORD PTR _result$[ebp+4], eax

; 118  :     result |= ((uint64_t)in[3]) << 24;

	mov	ecx, DWORD PTR _in$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [ecx+3]
	cdq
	mov	ecx, 24					; 00000018H
	call	__allshl
	mov	ecx, DWORD PTR _result$[ebp]
	or	ecx, eax
	mov	eax, DWORD PTR _result$[ebp+4]
	or	eax, edx
	mov	DWORD PTR _result$[ebp], ecx
	mov	DWORD PTR _result$[ebp+4], eax

; 119  :     return result;

	mov	eax, DWORD PTR _result$[ebp]
	mov	edx, DWORD PTR _result$[ebp+4]

; 120  : }

	mov	esp, ebp
	pop	ebp
	ret	0
_load_4	ENDP
_TEXT	ENDS
EXTRN	_memset:NEAR
_TEXT	SEGMENT
_h$ = 8
_fe_1	PROC NEAR

; 292  : {

	push	ebp
	mov	ebp, esp

; 293  :     memset(h, 0, sizeof(int32_t) * 10);

	push	40					; 00000028H
	push	0
	mov	eax, DWORD PTR _h$[ebp]
	push	eax
	call	_memset
	add	esp, 12					; 0000000cH

; 294  :     h[0] = 1;

	mov	ecx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [ecx], 1

; 295  : }

	pop	ebp
	ret	0
_fe_1	ENDP
_h$ = 8
_f$ = 12
_g$ = 16
_i$ = -4
_fe_add	PROC NEAR

; 298  : {

	push	ebp
	mov	ebp, esp
	push	ecx

; 299  :     unsigned i;
; 300  : 
; 301  :     for (i = 0; i < 10; i++) {

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L1061
$L1062:
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 1
	mov	DWORD PTR _i$[ebp], eax
$L1061:
	cmp	DWORD PTR _i$[ebp], 10			; 0000000aH
	jae	SHORT $L1063

; 302  :         h[i] = f[i] + g[i];

	mov	ecx, DWORD PTR _i$[ebp]
	mov	edx, DWORD PTR _f$[ebp]
	mov	eax, DWORD PTR [edx+ecx*4]
	mov	ecx, DWORD PTR _i$[ebp]
	mov	edx, DWORD PTR _g$[ebp]
	add	eax, DWORD PTR [edx+ecx*4]
	mov	ecx, DWORD PTR _i$[ebp]
	mov	edx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [edx+ecx*4], eax

; 303  :     }

	jmp	SHORT $L1062
$L1063:

; 304  : }

	mov	esp, ebp
	pop	ebp
	ret	0
_fe_add	ENDP
_h$ = 8
_f$ = 12
_g$ = 16
_i$ = -4
_fe_sub	PROC NEAR

; 307  : {

	push	ebp
	mov	ebp, esp
	push	ecx
	push	esi

; 308  :     unsigned i;
; 309  : 
; 310  :     for (i = 0; i < 10; i++) {

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L1072
$L1073:
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 1
	mov	DWORD PTR _i$[ebp], eax
$L1072:
	cmp	DWORD PTR _i$[ebp], 10			; 0000000aH
	jae	SHORT $L1074

; 311  :         h[i] = f[i] - g[i];

	mov	ecx, DWORD PTR _i$[ebp]
	mov	edx, DWORD PTR _f$[ebp]
	mov	eax, DWORD PTR _i$[ebp]
	mov	esi, DWORD PTR _g$[ebp]
	mov	ecx, DWORD PTR [edx+ecx*4]
	sub	ecx, DWORD PTR [esi+eax*4]
	mov	edx, DWORD PTR _i$[ebp]
	mov	eax, DWORD PTR _h$[ebp]
	mov	DWORD PTR [eax+edx*4], ecx

; 312  :     }

	jmp	SHORT $L1073
$L1074:

; 313  : }

	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	0
_fe_sub	ENDP
_h$ = 8
_f$ = 12
_i$ = -4
_fe_neg	PROC NEAR

; 809  : {

	push	ebp
	mov	ebp, esp
	push	ecx

; 810  :     unsigned i;
; 811  : 
; 812  :     for (i = 0; i < 10; i++) {

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L1554
$L1555:
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 1
	mov	DWORD PTR _i$[ebp], eax
$L1554:
	cmp	DWORD PTR _i$[ebp], 10			; 0000000aH
	jae	SHORT $L1556

; 813  :         h[i] = -f[i];

	mov	ecx, DWORD PTR _i$[ebp]
	mov	edx, DWORD PTR _f$[ebp]
	mov	eax, DWORD PTR [edx+ecx*4]
	neg	eax
	mov	ecx, DWORD PTR _i$[ebp]
	mov	edx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [edx+ecx*4], eax

; 814  :     }

	jmp	SHORT $L1555
$L1556:

; 815  : }

	mov	esp, ebp
	pop	ebp
	ret	0
_fe_neg	ENDP
_out$ = 8
_z$ = 12
_t0$ = -40
_t1$ = -80
_t2$ = -124
_i$ = -84
_fe_pow22523 PROC NEAR

; 1003 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 124				; 0000007cH

; 1004 :     fe t0;
; 1005 :     fe t1;
; 1006 :     fe t2;
; 1007 :     int i;
; 1008 : 
; 1009 :     fe_sq(t0, z);

	mov	eax, DWORD PTR _z$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 1010 :     fe_sq(t1, t0);

	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	call	_fe_sq
	add	esp, 8

; 1011 :     for (i = 1; i < 2; ++i) {

	mov	DWORD PTR _i$[ebp], 1
	jmp	SHORT $L1744
$L1745:
	mov	ecx, DWORD PTR _i$[ebp]
	add	ecx, 1
	mov	DWORD PTR _i$[ebp], ecx
$L1744:
	cmp	DWORD PTR _i$[ebp], 2
	jge	SHORT $L1746

; 1012 :         fe_sq(t1, t1);

	lea	edx, DWORD PTR _t1$[ebp]
	push	edx
	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	call	_fe_sq
	add	esp, 8

; 1013 :     }

	jmp	SHORT $L1745
$L1746:

; 1014 :     fe_mul(t1, z, t1);

	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	mov	edx, DWORD PTR _z$[ebp]
	push	edx
	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1015 :     fe_mul(t0, t0, t1);

	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	lea	eax, DWORD PTR _t0$[ebp]
	push	eax
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1016 :     fe_sq(t0, t0);

	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	call	_fe_sq
	add	esp, 8

; 1017 :     fe_mul(t0, t1, t0);

	lea	eax, DWORD PTR _t0$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1018 :     fe_sq(t1, t0);

	lea	eax, DWORD PTR _t0$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 1019 :     for (i = 1; i < 5; ++i) {

	mov	DWORD PTR _i$[ebp], 1
	jmp	SHORT $L1747
$L1748:
	mov	edx, DWORD PTR _i$[ebp]
	add	edx, 1
	mov	DWORD PTR _i$[ebp], edx
$L1747:
	cmp	DWORD PTR _i$[ebp], 5
	jge	SHORT $L1749

; 1020 :         fe_sq(t1, t1);

	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 1021 :     }

	jmp	SHORT $L1748
$L1749:

; 1022 :     fe_mul(t0, t1, t0);

	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1023 :     fe_sq(t1, t0);

	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	call	_fe_sq
	add	esp, 8

; 1024 :     for (i = 1; i < 10; ++i) {

	mov	DWORD PTR _i$[ebp], 1
	jmp	SHORT $L1750
$L1751:
	mov	ecx, DWORD PTR _i$[ebp]
	add	ecx, 1
	mov	DWORD PTR _i$[ebp], ecx
$L1750:
	cmp	DWORD PTR _i$[ebp], 10			; 0000000aH
	jge	SHORT $L1752

; 1025 :         fe_sq(t1, t1);

	lea	edx, DWORD PTR _t1$[ebp]
	push	edx
	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	call	_fe_sq
	add	esp, 8

; 1026 :     }

	jmp	SHORT $L1751
$L1752:

; 1027 :     fe_mul(t1, t1, t0);

	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t1$[ebp]
	push	edx
	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1028 :     fe_sq(t2, t1);

	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t2$[ebp]
	push	edx
	call	_fe_sq
	add	esp, 8

; 1029 :     for (i = 1; i < 20; ++i) {

	mov	DWORD PTR _i$[ebp], 1
	jmp	SHORT $L1753
$L1754:
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 1
	mov	DWORD PTR _i$[ebp], eax
$L1753:
	cmp	DWORD PTR _i$[ebp], 20			; 00000014H
	jge	SHORT $L1755

; 1030 :         fe_sq(t2, t2);

	lea	ecx, DWORD PTR _t2$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t2$[ebp]
	push	edx
	call	_fe_sq
	add	esp, 8

; 1031 :     }

	jmp	SHORT $L1754
$L1755:

; 1032 :     fe_mul(t1, t2, t1);

	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t2$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t1$[ebp]
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1033 :     fe_sq(t1, t1);

	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 1034 :     for (i = 1; i < 10; ++i) {

	mov	DWORD PTR _i$[ebp], 1
	jmp	SHORT $L1756
$L1757:
	mov	edx, DWORD PTR _i$[ebp]
	add	edx, 1
	mov	DWORD PTR _i$[ebp], edx
$L1756:
	cmp	DWORD PTR _i$[ebp], 10			; 0000000aH
	jge	SHORT $L1758

; 1035 :         fe_sq(t1, t1);

	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 1036 :     }

	jmp	SHORT $L1757
$L1758:

; 1037 :     fe_mul(t0, t1, t0);

	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1038 :     fe_sq(t1, t0);

	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	call	_fe_sq
	add	esp, 8

; 1039 :     for (i = 1; i < 50; ++i) {

	mov	DWORD PTR _i$[ebp], 1
	jmp	SHORT $L1759
$L1760:
	mov	ecx, DWORD PTR _i$[ebp]
	add	ecx, 1
	mov	DWORD PTR _i$[ebp], ecx
$L1759:
	cmp	DWORD PTR _i$[ebp], 50			; 00000032H
	jge	SHORT $L1761

; 1040 :         fe_sq(t1, t1);

	lea	edx, DWORD PTR _t1$[ebp]
	push	edx
	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	call	_fe_sq
	add	esp, 8

; 1041 :     }

	jmp	SHORT $L1760
$L1761:

; 1042 :     fe_mul(t1, t1, t0);

	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t1$[ebp]
	push	edx
	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1043 :     fe_sq(t2, t1);

	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t2$[ebp]
	push	edx
	call	_fe_sq
	add	esp, 8

; 1044 :     for (i = 1; i < 100; ++i) {

	mov	DWORD PTR _i$[ebp], 1
	jmp	SHORT $L1762
$L1763:
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 1
	mov	DWORD PTR _i$[ebp], eax
$L1762:
	cmp	DWORD PTR _i$[ebp], 100			; 00000064H
	jge	SHORT $L1764

; 1045 :         fe_sq(t2, t2);

	lea	ecx, DWORD PTR _t2$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t2$[ebp]
	push	edx
	call	_fe_sq
	add	esp, 8

; 1046 :     }

	jmp	SHORT $L1763
$L1764:

; 1047 :     fe_mul(t1, t2, t1);

	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t2$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t1$[ebp]
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1048 :     fe_sq(t1, t1);

	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 1049 :     for (i = 1; i < 50; ++i) {

	mov	DWORD PTR _i$[ebp], 1
	jmp	SHORT $L1765
$L1766:
	mov	edx, DWORD PTR _i$[ebp]
	add	edx, 1
	mov	DWORD PTR _i$[ebp], edx
$L1765:
	cmp	DWORD PTR _i$[ebp], 50			; 00000032H
	jge	SHORT $L1767

; 1050 :         fe_sq(t1, t1);

	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t1$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 1051 :     }

	jmp	SHORT $L1766
$L1767:

; 1052 :     fe_mul(t0, t1, t0);

	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	lea	eax, DWORD PTR _t1$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1053 :     fe_sq(t0, t0);

	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	lea	eax, DWORD PTR _t0$[ebp]
	push	eax
	call	_fe_sq
	add	esp, 8

; 1054 :     for (i = 1; i < 2; ++i) {

	mov	DWORD PTR _i$[ebp], 1
	jmp	SHORT $L1768
$L1769:
	mov	ecx, DWORD PTR _i$[ebp]
	add	ecx, 1
	mov	DWORD PTR _i$[ebp], ecx
$L1768:
	cmp	DWORD PTR _i$[ebp], 2
	jge	SHORT $L1770

; 1055 :         fe_sq(t0, t0);

	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	lea	eax, DWORD PTR _t0$[ebp]
	push	eax
	call	_fe_sq
	add	esp, 8

; 1056 :     }

	jmp	SHORT $L1769
$L1770:

; 1057 :     fe_mul(out, t0, z);

	mov	ecx, DWORD PTR _z$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	mov	eax, DWORD PTR _out$[ebp]
	push	eax
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1058 : }

	mov	esp, ebp
	pop	ebp
	ret	0
_fe_pow22523 ENDP
_r$ = 8
_a$ = 12
_A$ = 16
_b$ = 20
_aslide$ = -1536
_bslide$ = -1952
_Ai$ = -1280
_t$ = -1696
_u$ = -2112
_A2$ = -2276
_i$ = -2116
_ge_double_scalarmult_vartime PROC NEAR

; 1369 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 2276				; 000008e4H

; 1370 :     signed char aslide[256];
; 1371 :     signed char bslide[256];
; 1372 :     ge_cached Ai[8]; /* A,3A,5A,7A,9A,11A,13A,15A */
; 1373 :     ge_p1p1 t;
; 1374 :     ge_p3 u;
; 1375 :     ge_p3 A2;
; 1376 :     int i;
; 1377 : 
; 1378 :     slide(aslide, a);

	mov	eax, DWORD PTR _a$[ebp]
	push	eax
	lea	ecx, DWORD PTR _aslide$[ebp]
	push	ecx
	call	_slide
	add	esp, 8

; 1379 :     slide(bslide, b);

	mov	edx, DWORD PTR _b$[ebp]
	push	edx
	lea	eax, DWORD PTR _bslide$[ebp]
	push	eax
	call	_slide
	add	esp, 8

; 1380 : 
; 1381 :     ge_p3_to_cached(&Ai[0], A);

	mov	ecx, DWORD PTR _A$[ebp]
	push	ecx
	lea	edx, DWORD PTR _Ai$[ebp]
	push	edx
	call	_ge_p3_to_cached
	add	esp, 8

; 1382 :     ge_p3_dbl(&t, A);

	mov	eax, DWORD PTR _A$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t$[ebp]
	push	ecx
	call	_ge_p3_dbl
	add	esp, 8

; 1383 :     ge_p1p1_to_p3(&A2, &t);

	lea	edx, DWORD PTR _t$[ebp]
	push	edx
	lea	eax, DWORD PTR _A2$[ebp]
	push	eax
	call	_ge_p1p1_to_p3
	add	esp, 8

; 1384 :     ge_add(&t, &A2, &Ai[0]);

	lea	ecx, DWORD PTR _Ai$[ebp]
	push	ecx
	lea	edx, DWORD PTR _A2$[ebp]
	push	edx
	lea	eax, DWORD PTR _t$[ebp]
	push	eax
	call	_ge_add
	add	esp, 12					; 0000000cH

; 1385 :     ge_p1p1_to_p3(&u, &t);

	lea	ecx, DWORD PTR _t$[ebp]
	push	ecx
	lea	edx, DWORD PTR _u$[ebp]
	push	edx
	call	_ge_p1p1_to_p3
	add	esp, 8

; 1386 :     ge_p3_to_cached(&Ai[1], &u);

	lea	eax, DWORD PTR _u$[ebp]
	push	eax
	lea	ecx, DWORD PTR _Ai$[ebp+160]
	push	ecx
	call	_ge_p3_to_cached
	add	esp, 8

; 1387 :     ge_add(&t, &A2, &Ai[1]);

	lea	edx, DWORD PTR _Ai$[ebp+160]
	push	edx
	lea	eax, DWORD PTR _A2$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t$[ebp]
	push	ecx
	call	_ge_add
	add	esp, 12					; 0000000cH

; 1388 :     ge_p1p1_to_p3(&u, &t);

	lea	edx, DWORD PTR _t$[ebp]
	push	edx
	lea	eax, DWORD PTR _u$[ebp]
	push	eax
	call	_ge_p1p1_to_p3
	add	esp, 8

; 1389 :     ge_p3_to_cached(&Ai[2], &u);

	lea	ecx, DWORD PTR _u$[ebp]
	push	ecx
	lea	edx, DWORD PTR _Ai$[ebp+320]
	push	edx
	call	_ge_p3_to_cached
	add	esp, 8

; 1390 :     ge_add(&t, &A2, &Ai[2]);

	lea	eax, DWORD PTR _Ai$[ebp+320]
	push	eax
	lea	ecx, DWORD PTR _A2$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t$[ebp]
	push	edx
	call	_ge_add
	add	esp, 12					; 0000000cH

; 1391 :     ge_p1p1_to_p3(&u, &t);

	lea	eax, DWORD PTR _t$[ebp]
	push	eax
	lea	ecx, DWORD PTR _u$[ebp]
	push	ecx
	call	_ge_p1p1_to_p3
	add	esp, 8

; 1392 :     ge_p3_to_cached(&Ai[3], &u);

	lea	edx, DWORD PTR _u$[ebp]
	push	edx
	lea	eax, DWORD PTR _Ai$[ebp+480]
	push	eax
	call	_ge_p3_to_cached
	add	esp, 8

; 1393 :     ge_add(&t, &A2, &Ai[3]);

	lea	ecx, DWORD PTR _Ai$[ebp+480]
	push	ecx
	lea	edx, DWORD PTR _A2$[ebp]
	push	edx
	lea	eax, DWORD PTR _t$[ebp]
	push	eax
	call	_ge_add
	add	esp, 12					; 0000000cH

; 1394 :     ge_p1p1_to_p3(&u, &t);

	lea	ecx, DWORD PTR _t$[ebp]
	push	ecx
	lea	edx, DWORD PTR _u$[ebp]
	push	edx
	call	_ge_p1p1_to_p3
	add	esp, 8

; 1395 :     ge_p3_to_cached(&Ai[4], &u);

	lea	eax, DWORD PTR _u$[ebp]
	push	eax
	lea	ecx, DWORD PTR _Ai$[ebp+640]
	push	ecx
	call	_ge_p3_to_cached
	add	esp, 8

; 1396 :     ge_add(&t, &A2, &Ai[4]);

	lea	edx, DWORD PTR _Ai$[ebp+640]
	push	edx
	lea	eax, DWORD PTR _A2$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t$[ebp]
	push	ecx
	call	_ge_add
	add	esp, 12					; 0000000cH

; 1397 :     ge_p1p1_to_p3(&u, &t);

	lea	edx, DWORD PTR _t$[ebp]
	push	edx
	lea	eax, DWORD PTR _u$[ebp]
	push	eax
	call	_ge_p1p1_to_p3
	add	esp, 8

; 1398 :     ge_p3_to_cached(&Ai[5], &u);

	lea	ecx, DWORD PTR _u$[ebp]
	push	ecx
	lea	edx, DWORD PTR _Ai$[ebp+800]
	push	edx
	call	_ge_p3_to_cached
	add	esp, 8

; 1399 :     ge_add(&t, &A2, &Ai[5]);

	lea	eax, DWORD PTR _Ai$[ebp+800]
	push	eax
	lea	ecx, DWORD PTR _A2$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t$[ebp]
	push	edx
	call	_ge_add
	add	esp, 12					; 0000000cH

; 1400 :     ge_p1p1_to_p3(&u, &t);

	lea	eax, DWORD PTR _t$[ebp]
	push	eax
	lea	ecx, DWORD PTR _u$[ebp]
	push	ecx
	call	_ge_p1p1_to_p3
	add	esp, 8

; 1401 :     ge_p3_to_cached(&Ai[6], &u);

	lea	edx, DWORD PTR _u$[ebp]
	push	edx
	lea	eax, DWORD PTR _Ai$[ebp+960]
	push	eax
	call	_ge_p3_to_cached
	add	esp, 8

; 1402 :     ge_add(&t, &A2, &Ai[6]);

	lea	ecx, DWORD PTR _Ai$[ebp+960]
	push	ecx
	lea	edx, DWORD PTR _A2$[ebp]
	push	edx
	lea	eax, DWORD PTR _t$[ebp]
	push	eax
	call	_ge_add
	add	esp, 12					; 0000000cH

; 1403 :     ge_p1p1_to_p3(&u, &t);

	lea	ecx, DWORD PTR _t$[ebp]
	push	ecx
	lea	edx, DWORD PTR _u$[ebp]
	push	edx
	call	_ge_p1p1_to_p3
	add	esp, 8

; 1404 :     ge_p3_to_cached(&Ai[7], &u);

	lea	eax, DWORD PTR _u$[ebp]
	push	eax
	lea	ecx, DWORD PTR _Ai$[ebp+1120]
	push	ecx
	call	_ge_p3_to_cached
	add	esp, 8

; 1405 : 
; 1406 :     ge_p2_0(r);

	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	_ge_p2_0
	add	esp, 4

; 1407 : 
; 1408 :     for (i = 255; i >= 0; --i) {

	mov	DWORD PTR _i$[ebp], 255			; 000000ffH
	jmp	SHORT $L1917
$L1918:
	mov	eax, DWORD PTR _i$[ebp]
	sub	eax, 1
	mov	DWORD PTR _i$[ebp], eax
$L1917:
	cmp	DWORD PTR _i$[ebp], 0
	jl	SHORT $L1919

; 1409 :         if (aslide[i] || bslide[i]) {

	mov	ecx, DWORD PTR _i$[ebp]
	movsx	edx, BYTE PTR _aslide$[ebp+ecx]
	test	edx, edx
	jne	SHORT $L1921
	mov	eax, DWORD PTR _i$[ebp]
	movsx	ecx, BYTE PTR _bslide$[ebp+eax]
	test	ecx, ecx
	je	SHORT $L1920
$L1921:

; 1410 :             break;

	jmp	SHORT $L1919
$L1920:

; 1412 :     }

	jmp	SHORT $L1918
$L1919:

; 1413 : 
; 1414 :     for (; i >= 0; --i) {

	jmp	SHORT $L1922
$L1923:
	mov	edx, DWORD PTR _i$[ebp]
	sub	edx, 1
	mov	DWORD PTR _i$[ebp], edx
$L1922:
	cmp	DWORD PTR _i$[ebp], 0
	jl	$L1924

; 1415 :         ge_p2_dbl(&t, r);

	mov	eax, DWORD PTR _r$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t$[ebp]
	push	ecx
	call	_ge_p2_dbl
	add	esp, 8

; 1416 : 
; 1417 :         if (aslide[i] > 0) {

	mov	edx, DWORD PTR _i$[ebp]
	movsx	eax, BYTE PTR _aslide$[ebp+edx]
	test	eax, eax
	jle	SHORT $L1925

; 1418 :             ge_p1p1_to_p3(&u, &t);

	lea	ecx, DWORD PTR _t$[ebp]
	push	ecx
	lea	edx, DWORD PTR _u$[ebp]
	push	edx
	call	_ge_p1p1_to_p3
	add	esp, 8

; 1419 :             ge_add(&t, &u, &Ai[aslide[i] / 2]);

	mov	eax, DWORD PTR _i$[ebp]
	movsx	eax, BYTE PTR _aslide$[ebp+eax]
	cdq
	sub	eax, edx
	sar	eax, 1
	imul	eax, 160				; 000000a0H
	lea	ecx, DWORD PTR _Ai$[ebp+eax]
	push	ecx
	lea	edx, DWORD PTR _u$[ebp]
	push	edx
	lea	eax, DWORD PTR _t$[ebp]
	push	eax
	call	_ge_add
	add	esp, 12					; 0000000cH

; 1420 :         } else if (aslide[i] < 0) {

	jmp	SHORT $L1927
$L1925:
	mov	ecx, DWORD PTR _i$[ebp]
	movsx	edx, BYTE PTR _aslide$[ebp+ecx]
	test	edx, edx
	jge	SHORT $L1927

; 1421 :             ge_p1p1_to_p3(&u, &t);

	lea	eax, DWORD PTR _t$[ebp]
	push	eax
	lea	ecx, DWORD PTR _u$[ebp]
	push	ecx
	call	_ge_p1p1_to_p3
	add	esp, 8

; 1422 :             ge_sub(&t, &u, &Ai[(-aslide[i]) / 2]);

	mov	edx, DWORD PTR _i$[ebp]
	movsx	eax, BYTE PTR _aslide$[ebp+edx]
	neg	eax
	cdq
	sub	eax, edx
	sar	eax, 1
	imul	eax, 160				; 000000a0H
	lea	eax, DWORD PTR _Ai$[ebp+eax]
	push	eax
	lea	ecx, DWORD PTR _u$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t$[ebp]
	push	edx
	call	_ge_sub
	add	esp, 12					; 0000000cH
$L1927:

; 1424 : 
; 1425 :         if (bslide[i] > 0) {

	mov	eax, DWORD PTR _i$[ebp]
	movsx	ecx, BYTE PTR _bslide$[ebp+eax]
	test	ecx, ecx
	jle	SHORT $L1928

; 1426 :             ge_p1p1_to_p3(&u, &t);

	lea	edx, DWORD PTR _t$[ebp]
	push	edx
	lea	eax, DWORD PTR _u$[ebp]
	push	eax
	call	_ge_p1p1_to_p3
	add	esp, 8

; 1427 :             ge_madd(&t, &u, &Bi[bslide[i] / 2]);

	mov	ecx, DWORD PTR _i$[ebp]
	movsx	eax, BYTE PTR _bslide$[ebp+ecx]
	cdq
	sub	eax, edx
	sar	eax, 1
	imul	eax, 120				; 00000078H
	add	eax, OFFSET FLAT:_Bi
	push	eax
	lea	edx, DWORD PTR _u$[ebp]
	push	edx
	lea	eax, DWORD PTR _t$[ebp]
	push	eax
	call	_ge_madd
	add	esp, 12					; 0000000cH

; 1428 :         } else if (bslide[i] < 0) {

	jmp	SHORT $L1930
$L1928:
	mov	ecx, DWORD PTR _i$[ebp]
	movsx	edx, BYTE PTR _bslide$[ebp+ecx]
	test	edx, edx
	jge	SHORT $L1930

; 1429 :             ge_p1p1_to_p3(&u, &t);

	lea	eax, DWORD PTR _t$[ebp]
	push	eax
	lea	ecx, DWORD PTR _u$[ebp]
	push	ecx
	call	_ge_p1p1_to_p3
	add	esp, 8

; 1430 :             ge_msub(&t, &u, &Bi[(-bslide[i]) / 2]);

	mov	edx, DWORD PTR _i$[ebp]
	movsx	eax, BYTE PTR _bslide$[ebp+edx]
	neg	eax
	cdq
	sub	eax, edx
	sar	eax, 1
	imul	eax, 120				; 00000078H
	add	eax, OFFSET FLAT:_Bi
	push	eax
	lea	eax, DWORD PTR _u$[ebp]
	push	eax
	lea	ecx, DWORD PTR _t$[ebp]
	push	ecx
	call	_ge_msub
	add	esp, 12					; 0000000cH
$L1930:

; 1432 : 
; 1433 :         ge_p1p1_to_p2(r, &t);

	lea	edx, DWORD PTR _t$[ebp]
	push	edx
	mov	eax, DWORD PTR _r$[ebp]
	push	eax
	call	_ge_p1p1_to_p2
	add	esp, 8

; 1434 :     }

	jmp	$L1923
$L1924:

; 1435 : }

	mov	esp, ebp
	pop	ebp
	ret	0
_ge_double_scalarmult_vartime ENDP
_h$ = 8
_ge_p2_0 PROC NEAR

; 1136 : {

	push	ebp
	mov	ebp, esp

; 1137 :     fe_0(h->X);

	mov	eax, DWORD PTR _h$[ebp]
	push	eax
	call	_fe_0
	add	esp, 4

; 1138 :     fe_1(h->Y);

	mov	ecx, DWORD PTR _h$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	call	_fe_1
	add	esp, 4

; 1139 :     fe_1(h->Z);

	mov	edx, DWORD PTR _h$[ebp]
	add	edx, 80					; 00000050H
	push	edx
	call	_fe_1
	add	esp, 4

; 1140 : }

	pop	ebp
	ret	0
_ge_p2_0 ENDP
_h$ = 8
_fe_0	PROC NEAR

; 287  : {

	push	ebp
	mov	ebp, esp

; 288  :     memset(h, 0, sizeof(int32_t) * 10);

	push	40					; 00000028H
	push	0
	mov	eax, DWORD PTR _h$[ebp]
	push	eax
	call	_memset
	add	esp, 12					; 0000000cH

; 289  : }

	pop	ebp
	ret	0
_fe_0	ENDP
_r$ = 8
_p$ = 12
_ge_p3_to_cached PROC NEAR

; 1155 : {

	push	ebp
	mov	ebp, esp

; 1156 :     fe_add(r->YplusX, p->Y, p->X);

	mov	eax, DWORD PTR _p$[ebp]
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1157 :     fe_sub(r->YminusX, p->Y, p->X);

	mov	eax, DWORD PTR _p$[ebp]
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1158 :     fe_copy(r->Z, p->Z);

	mov	eax, DWORD PTR _p$[ebp]
	add	eax, 80					; 00000050H
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	call	_fe_copy
	add	esp, 8

; 1159 :     fe_mul(r->T2d, p->T, d2);

	push	OFFSET FLAT:_d2
	mov	edx, DWORD PTR _p$[ebp]
	add	edx, 120				; 00000078H
	push	edx
	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1160 : }

	pop	ebp
	ret	0
_ge_p3_to_cached ENDP
_TEXT	ENDS
EXTRN	__imp__memmove:NEAR
_TEXT	SEGMENT
_h$ = 8
_f$ = 12
_fe_copy PROC NEAR

; 282  : {

	push	ebp
	mov	ebp, esp

; 283  :     memmove(h, f, sizeof(int32_t) * 10);

	push	40					; 00000028H
	mov	eax, DWORD PTR _f$[ebp]
	push	eax
	mov	ecx, DWORD PTR _h$[ebp]
	push	ecx
	call	DWORD PTR __imp__memmove
	add	esp, 12					; 0000000cH

; 284  : }

	pop	ebp
	ret	0
_fe_copy ENDP
_r$ = 8
_p$ = 12
_ge_p1p1_to_p2 PROC NEAR

; 1163 : {

	push	ebp
	mov	ebp, esp

; 1164 :     fe_mul(r->X, p->X, p->T);

	mov	eax, DWORD PTR _p$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1165 :     fe_mul(r->Y, p->Y, p->Z);

	mov	eax, DWORD PTR _p$[ebp]
	add	eax, 80					; 00000050H
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1166 :     fe_mul(r->Z, p->Z, p->T);

	mov	eax, DWORD PTR _p$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 80					; 00000050H
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1167 : }

	pop	ebp
	ret	0
_ge_p1p1_to_p2 ENDP
_r$ = 8
_p$ = 12
_ge_p1p1_to_p3 PROC NEAR

; 1170 : {

	push	ebp
	mov	ebp, esp

; 1171 :     fe_mul(r->X, p->X, p->T);

	mov	eax, DWORD PTR _p$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1172 :     fe_mul(r->Y, p->Y, p->Z);

	mov	eax, DWORD PTR _p$[ebp]
	add	eax, 80					; 00000050H
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1173 :     fe_mul(r->Z, p->Z, p->T);

	mov	eax, DWORD PTR _p$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 80					; 00000050H
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1174 :     fe_mul(r->T, p->X, p->Y);

	mov	eax, DWORD PTR _p$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 120				; 00000078H
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1175 : }

	pop	ebp
	ret	0
_ge_p1p1_to_p3 ENDP
_r$ = 8
_p$ = 12
_t0$ = -40
_ge_p2_dbl PROC NEAR

; 1178 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 40					; 00000028H

; 1179 :     fe t0;
; 1180 : 
; 1181 :     fe_sq(r->X, p->X);

	mov	eax, DWORD PTR _p$[ebp]
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 1182 :     fe_sq(r->Z, p->Y);

	mov	edx, DWORD PTR _p$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 80					; 00000050H
	push	eax
	call	_fe_sq
	add	esp, 8

; 1183 :     fe_sq2(r->T, p->Z);

	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 120				; 00000078H
	push	edx
	call	_fe_sq2
	add	esp, 8

; 1184 :     fe_add(r->Y, p->X, p->Y);

	mov	eax, DWORD PTR _p$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1185 :     fe_sq(t0, r->Y);

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	call	_fe_sq
	add	esp, 8

; 1186 :     fe_add(r->Y, r->Z, r->X);

	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 80					; 00000050H
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1187 :     fe_sub(r->Z, r->Z, r->X);

	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 80					; 00000050H
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1188 :     fe_sub(r->X, t0, r->Y);

	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	lea	eax, DWORD PTR _t0$[ebp]
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1189 :     fe_sub(r->T, r->T, r->Z);

	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 80					; 00000050H
	push	edx
	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, 120				; 00000078H
	push	ecx
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1190 : }

	mov	esp, ebp
	pop	ebp
	ret	0
_ge_p2_dbl ENDP
_h$ = 8
_f$ = 12
_f0$ = -636
_f1$ = -692
_f2$ = -36
_f3$ = -64
_f4$ = -100
_f5$ = -128
_f6$ = -188
_f7$ = -224
_f8$ = -260
_f9$ = -300
_f0_2$ = -352
_f1_2$ = -348
_f2_2$ = -592
_f3_2$ = -632
_f4_2$ = -628
_f5_2$ = -624
_f6_2$ = -620
_f7_2$ = -688
_f5_38$ = -32
_f6_19$ = -412
_f7_38$ = -296
_f8_19$ = -28
_f9_38$ = -344
_f0f0$ = -124
_f0f1_2$ = -616
_f0f2_2$ = -608
_f0f3_2$ = -684
_f0f4_2$ = -676
_f0f5_2$ = -668
_f0f6_2$ = -660
_f0f7_2$ = -548
_f0f8_2$ = -540
_f0f9_2$ = -532
_f1f1_2$ = -256
_f1f2_2$ = -248
_f1f3_4$ = -220
_f1f4_2$ = -292
_f1f5_4$ = -212
_f1f6_2$ = -284
_f1f7_4$ = -408
_f1f8_2$ = -184
_f1f9_76$ = -24
_f2f2$ = -400
_f2f3_2$ = -524
_f2f4_2$ = -516
_f2f5_2$ = -508
_f2f6_2$ = -588
_f2f7_2$ = -580
_f2f8_38$ = -452
_f2f9_38$ = -500
_f3f3_2$ = -176
_f3f4_2$ = -168
_f3f5_4$ = -392
_f3f6_2$ = -204
_f3f7_76$ = -316
_f3f8_38$ = -444
_f3f9_76$ = -652
_f4f4$ = -492
_f4f5_2$ = -572
_f4f6_38$ = -384
_f4f7_38$ = -436
_f4f8_38$ = -484
_f4f9_38$ = -60
_f5f5_38$ = -340
_f5f6_38$ = -376
_f5f7_76$ = -332
_f5f8_38$ = -476
_f5f9_76$ = -16
_f6f6_19$ = -160
_f6f7_38$ = -564
_f6f8_38$ = -96
_f6f9_38$ = -116
_f7f7_38$ = -52
_f7f8_38$ = -88
_f7f9_76$ = -80
_f8f8_19$ = -468
_f8f9_38$ = -152
_f9f9_38$ = -144
_h0$ = -240
_h1$ = -276
_h2$ = -308
_h3$ = -324
_h4$ = -368
_h5$ = -428
_h6$ = -460
_h7$ = -556
_h8$ = -600
_h9$ = -644
_carry0$ = -72
_carry1$ = -108
_carry2$ = -8
_carry3$ = -44
_carry4$ = -232
_carry5$ = -268
_carry6$ = -136
_carry7$ = -196
_carry8$ = -360
_carry9$ = -420
_fe_sq2	PROC NEAR

; 836  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 692				; 000002b4H
	push	esi

; 837  :     int32_t f0 = f[0];

	mov	eax, DWORD PTR _f$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _f0$[ebp], ecx

; 838  :     int32_t f1 = f[1];

	mov	edx, DWORD PTR _f$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR _f1$[ebp], eax

; 839  :     int32_t f2 = f[2];

	mov	ecx, DWORD PTR _f$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	mov	DWORD PTR _f2$[ebp], edx

; 840  :     int32_t f3 = f[3];

	mov	eax, DWORD PTR _f$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	DWORD PTR _f3$[ebp], ecx

; 841  :     int32_t f4 = f[4];

	mov	edx, DWORD PTR _f$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	DWORD PTR _f4$[ebp], eax

; 842  :     int32_t f5 = f[5];

	mov	ecx, DWORD PTR _f$[ebp]
	mov	edx, DWORD PTR [ecx+20]
	mov	DWORD PTR _f5$[ebp], edx

; 843  :     int32_t f6 = f[6];

	mov	eax, DWORD PTR _f$[ebp]
	mov	ecx, DWORD PTR [eax+24]
	mov	DWORD PTR _f6$[ebp], ecx

; 844  :     int32_t f7 = f[7];

	mov	edx, DWORD PTR _f$[ebp]
	mov	eax, DWORD PTR [edx+28]
	mov	DWORD PTR _f7$[ebp], eax

; 845  :     int32_t f8 = f[8];

	mov	ecx, DWORD PTR _f$[ebp]
	mov	edx, DWORD PTR [ecx+32]
	mov	DWORD PTR _f8$[ebp], edx

; 846  :     int32_t f9 = f[9];

	mov	eax, DWORD PTR _f$[ebp]
	mov	ecx, DWORD PTR [eax+36]
	mov	DWORD PTR _f9$[ebp], ecx

; 847  :     int32_t f0_2 = 2 * f0;

	mov	edx, DWORD PTR _f0$[ebp]
	shl	edx, 1
	mov	DWORD PTR _f0_2$[ebp], edx

; 848  :     int32_t f1_2 = 2 * f1;

	mov	eax, DWORD PTR _f1$[ebp]
	shl	eax, 1
	mov	DWORD PTR _f1_2$[ebp], eax

; 849  :     int32_t f2_2 = 2 * f2;

	mov	ecx, DWORD PTR _f2$[ebp]
	shl	ecx, 1
	mov	DWORD PTR _f2_2$[ebp], ecx

; 850  :     int32_t f3_2 = 2 * f3;

	mov	edx, DWORD PTR _f3$[ebp]
	shl	edx, 1
	mov	DWORD PTR _f3_2$[ebp], edx

; 851  :     int32_t f4_2 = 2 * f4;

	mov	eax, DWORD PTR _f4$[ebp]
	shl	eax, 1
	mov	DWORD PTR _f4_2$[ebp], eax

; 852  :     int32_t f5_2 = 2 * f5;

	mov	ecx, DWORD PTR _f5$[ebp]
	shl	ecx, 1
	mov	DWORD PTR _f5_2$[ebp], ecx

; 853  :     int32_t f6_2 = 2 * f6;

	mov	edx, DWORD PTR _f6$[ebp]
	shl	edx, 1
	mov	DWORD PTR _f6_2$[ebp], edx

; 854  :     int32_t f7_2 = 2 * f7;

	mov	eax, DWORD PTR _f7$[ebp]
	shl	eax, 1
	mov	DWORD PTR _f7_2$[ebp], eax

; 855  :     int32_t f5_38 = 38 * f5; /* 1.959375*2^30 */

	mov	ecx, DWORD PTR _f5$[ebp]
	imul	ecx, 38					; 00000026H
	mov	DWORD PTR _f5_38$[ebp], ecx

; 856  :     int32_t f6_19 = 19 * f6; /* 1.959375*2^30 */

	mov	edx, DWORD PTR _f6$[ebp]
	imul	edx, 19					; 00000013H
	mov	DWORD PTR _f6_19$[ebp], edx

; 857  :     int32_t f7_38 = 38 * f7; /* 1.959375*2^30 */

	mov	eax, DWORD PTR _f7$[ebp]
	imul	eax, 38					; 00000026H
	mov	DWORD PTR _f7_38$[ebp], eax

; 858  :     int32_t f8_19 = 19 * f8; /* 1.959375*2^30 */

	mov	ecx, DWORD PTR _f8$[ebp]
	imul	ecx, 19					; 00000013H
	mov	DWORD PTR _f8_19$[ebp], ecx

; 859  :     int32_t f9_38 = 38 * f9; /* 1.959375*2^30 */

	mov	edx, DWORD PTR _f9$[ebp]
	imul	edx, 38					; 00000026H
	mov	DWORD PTR _f9_38$[ebp], edx

; 860  :     int64_t f0f0 = f0 * (int64_t)f0;

	mov	eax, DWORD PTR _f0$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f0$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f0$[ebp], eax
	mov	DWORD PTR _f0f0$[ebp+4], edx

; 861  :     int64_t f0f1_2 = f0_2 * (int64_t)f1;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f1$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f1_2$[ebp], eax
	mov	DWORD PTR _f0f1_2$[ebp+4], edx

; 862  :     int64_t f0f2_2 = f0_2 * (int64_t)f2;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f2_2$[ebp], eax
	mov	DWORD PTR _f0f2_2$[ebp+4], edx

; 863  :     int64_t f0f3_2 = f0_2 * (int64_t)f3;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f3$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f3_2$[ebp], eax
	mov	DWORD PTR _f0f3_2$[ebp+4], edx

; 864  :     int64_t f0f4_2 = f0_2 * (int64_t)f4;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f4_2$[ebp], eax
	mov	DWORD PTR _f0f4_2$[ebp+4], edx

; 865  :     int64_t f0f5_2 = f0_2 * (int64_t)f5;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f5$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f5_2$[ebp], eax
	mov	DWORD PTR _f0f5_2$[ebp+4], edx

; 866  :     int64_t f0f6_2 = f0_2 * (int64_t)f6;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f6_2$[ebp], eax
	mov	DWORD PTR _f0f6_2$[ebp+4], edx

; 867  :     int64_t f0f7_2 = f0_2 * (int64_t)f7;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f7$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f7_2$[ebp], eax
	mov	DWORD PTR _f0f7_2$[ebp+4], edx

; 868  :     int64_t f0f8_2 = f0_2 * (int64_t)f8;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f8_2$[ebp], eax
	mov	DWORD PTR _f0f8_2$[ebp+4], edx

; 869  :     int64_t f0f9_2 = f0_2 * (int64_t)f9;

	mov	eax, DWORD PTR _f0_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f0f9_2$[ebp], eax
	mov	DWORD PTR _f0f9_2$[ebp+4], edx

; 870  :     int64_t f1f1_2 = f1_2 * (int64_t)f1;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f1$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f1_2$[ebp], eax
	mov	DWORD PTR _f1f1_2$[ebp+4], edx

; 871  :     int64_t f1f2_2 = f1_2 * (int64_t)f2;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f2_2$[ebp], eax
	mov	DWORD PTR _f1f2_2$[ebp+4], edx

; 872  :     int64_t f1f3_4 = f1_2 * (int64_t)f3_2;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f3_4$[ebp], eax
	mov	DWORD PTR _f1f3_4$[ebp+4], edx

; 873  :     int64_t f1f4_2 = f1_2 * (int64_t)f4;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f4_2$[ebp], eax
	mov	DWORD PTR _f1f4_2$[ebp+4], edx

; 874  :     int64_t f1f5_4 = f1_2 * (int64_t)f5_2;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f5_2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f5_4$[ebp], eax
	mov	DWORD PTR _f1f5_4$[ebp+4], edx

; 875  :     int64_t f1f6_2 = f1_2 * (int64_t)f6;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f6_2$[ebp], eax
	mov	DWORD PTR _f1f6_2$[ebp+4], edx

; 876  :     int64_t f1f7_4 = f1_2 * (int64_t)f7_2;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f7_2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f7_4$[ebp], eax
	mov	DWORD PTR _f1f7_4$[ebp+4], edx

; 877  :     int64_t f1f8_2 = f1_2 * (int64_t)f8;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f8_2$[ebp], eax
	mov	DWORD PTR _f1f8_2$[ebp+4], edx

; 878  :     int64_t f1f9_76 = f1_2 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f1_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f1f9_76$[ebp], eax
	mov	DWORD PTR _f1f9_76$[ebp+4], edx

; 879  :     int64_t f2f2 = f2 * (int64_t)f2;

	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2f2$[ebp], eax
	mov	DWORD PTR _f2f2$[ebp+4], edx

; 880  :     int64_t f2f3_2 = f2_2 * (int64_t)f3;

	mov	eax, DWORD PTR _f2_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f3$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2f3_2$[ebp], eax
	mov	DWORD PTR _f2f3_2$[ebp+4], edx

; 881  :     int64_t f2f4_2 = f2_2 * (int64_t)f4;

	mov	eax, DWORD PTR _f2_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2f4_2$[ebp], eax
	mov	DWORD PTR _f2f4_2$[ebp+4], edx

; 882  :     int64_t f2f5_2 = f2_2 * (int64_t)f5;

	mov	eax, DWORD PTR _f2_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f5$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2f5_2$[ebp], eax
	mov	DWORD PTR _f2f5_2$[ebp+4], edx

; 883  :     int64_t f2f6_2 = f2_2 * (int64_t)f6;

	mov	eax, DWORD PTR _f2_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2f6_2$[ebp], eax
	mov	DWORD PTR _f2f6_2$[ebp+4], edx

; 884  :     int64_t f2f7_2 = f2_2 * (int64_t)f7;

	mov	eax, DWORD PTR _f2_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f7$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2f7_2$[ebp], eax
	mov	DWORD PTR _f2f7_2$[ebp+4], edx

; 885  :     int64_t f2f8_38 = f2_2 * (int64_t)f8_19;

	mov	eax, DWORD PTR _f2_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2f8_38$[ebp], eax
	mov	DWORD PTR _f2f8_38$[ebp+4], edx

; 886  :     int64_t f2f9_38 = f2 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f2f9_38$[ebp], eax
	mov	DWORD PTR _f2f9_38$[ebp+4], edx

; 887  :     int64_t f3f3_2 = f3_2 * (int64_t)f3;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f3$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3f3_2$[ebp], eax
	mov	DWORD PTR _f3f3_2$[ebp+4], edx

; 888  :     int64_t f3f4_2 = f3_2 * (int64_t)f4;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3f4_2$[ebp], eax
	mov	DWORD PTR _f3f4_2$[ebp+4], edx

; 889  :     int64_t f3f5_4 = f3_2 * (int64_t)f5_2;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f5_2$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3f5_4$[ebp], eax
	mov	DWORD PTR _f3f5_4$[ebp+4], edx

; 890  :     int64_t f3f6_2 = f3_2 * (int64_t)f6;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3f6_2$[ebp], eax
	mov	DWORD PTR _f3f6_2$[ebp+4], edx

; 891  :     int64_t f3f7_76 = f3_2 * (int64_t)f7_38;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f7_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3f7_76$[ebp], eax
	mov	DWORD PTR _f3f7_76$[ebp+4], edx

; 892  :     int64_t f3f8_38 = f3_2 * (int64_t)f8_19;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3f8_38$[ebp], eax
	mov	DWORD PTR _f3f8_38$[ebp+4], edx

; 893  :     int64_t f3f9_76 = f3_2 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f3_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f3f9_76$[ebp], eax
	mov	DWORD PTR _f3f9_76$[ebp+4], edx

; 894  :     int64_t f4f4 = f4 * (int64_t)f4;

	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4f4$[ebp], eax
	mov	DWORD PTR _f4f4$[ebp+4], edx

; 895  :     int64_t f4f5_2 = f4_2 * (int64_t)f5;

	mov	eax, DWORD PTR _f4_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f5$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4f5_2$[ebp], eax
	mov	DWORD PTR _f4f5_2$[ebp+4], edx

; 896  :     int64_t f4f6_38 = f4_2 * (int64_t)f6_19;

	mov	eax, DWORD PTR _f4_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f6_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4f6_38$[ebp], eax
	mov	DWORD PTR _f4f6_38$[ebp+4], edx

; 897  :     int64_t f4f7_38 = f4 * (int64_t)f7_38;

	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f7_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4f7_38$[ebp], eax
	mov	DWORD PTR _f4f7_38$[ebp+4], edx

; 898  :     int64_t f4f8_38 = f4_2 * (int64_t)f8_19;

	mov	eax, DWORD PTR _f4_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4f8_38$[ebp], eax
	mov	DWORD PTR _f4f8_38$[ebp+4], edx

; 899  :     int64_t f4f9_38 = f4 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f4$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f4f9_38$[ebp], eax
	mov	DWORD PTR _f4f9_38$[ebp+4], edx

; 900  :     int64_t f5f5_38 = f5 * (int64_t)f5_38;

	mov	eax, DWORD PTR _f5$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f5_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5f5_38$[ebp], eax
	mov	DWORD PTR _f5f5_38$[ebp+4], edx

; 901  :     int64_t f5f6_38 = f5_2 * (int64_t)f6_19;

	mov	eax, DWORD PTR _f5_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f6_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5f6_38$[ebp], eax
	mov	DWORD PTR _f5f6_38$[ebp+4], edx

; 902  :     int64_t f5f7_76 = f5_2 * (int64_t)f7_38;

	mov	eax, DWORD PTR _f5_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f7_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5f7_76$[ebp], eax
	mov	DWORD PTR _f5f7_76$[ebp+4], edx

; 903  :     int64_t f5f8_38 = f5_2 * (int64_t)f8_19;

	mov	eax, DWORD PTR _f5_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5f8_38$[ebp], eax
	mov	DWORD PTR _f5f8_38$[ebp+4], edx

; 904  :     int64_t f5f9_76 = f5_2 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f5_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f5f9_76$[ebp], eax
	mov	DWORD PTR _f5f9_76$[ebp+4], edx

; 905  :     int64_t f6f6_19 = f6 * (int64_t)f6_19;

	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f6_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6f6_19$[ebp], eax
	mov	DWORD PTR _f6f6_19$[ebp+4], edx

; 906  :     int64_t f6f7_38 = f6 * (int64_t)f7_38;

	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f7_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6f7_38$[ebp], eax
	mov	DWORD PTR _f6f7_38$[ebp+4], edx

; 907  :     int64_t f6f8_38 = f6_2 * (int64_t)f8_19;

	mov	eax, DWORD PTR _f6_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6f8_38$[ebp], eax
	mov	DWORD PTR _f6f8_38$[ebp+4], edx

; 908  :     int64_t f6f9_38 = f6 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f6$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f6f9_38$[ebp], eax
	mov	DWORD PTR _f6f9_38$[ebp+4], edx

; 909  :     int64_t f7f7_38 = f7 * (int64_t)f7_38;

	mov	eax, DWORD PTR _f7$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f7_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f7f7_38$[ebp], eax
	mov	DWORD PTR _f7f7_38$[ebp+4], edx

; 910  :     int64_t f7f8_38 = f7_2 * (int64_t)f8_19;

	mov	eax, DWORD PTR _f7_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f7f8_38$[ebp], eax
	mov	DWORD PTR _f7f8_38$[ebp+4], edx

; 911  :     int64_t f7f9_76 = f7_2 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f7_2$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f7f9_76$[ebp], eax
	mov	DWORD PTR _f7f9_76$[ebp+4], edx

; 912  :     int64_t f8f8_19 = f8 * (int64_t)f8_19;

	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f8_19$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f8f8_19$[ebp], eax
	mov	DWORD PTR _f8f8_19$[ebp+4], edx

; 913  :     int64_t f8f9_38 = f8 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f8$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f8f9_38$[ebp], eax
	mov	DWORD PTR _f8f9_38$[ebp+4], edx

; 914  :     int64_t f9f9_38 = f9 * (int64_t)f9_38;

	mov	eax, DWORD PTR _f9$[ebp]
	cdq
	mov	ecx, eax
	mov	esi, edx
	mov	eax, DWORD PTR _f9_38$[ebp]
	cdq
	push	edx
	push	eax
	push	esi
	push	ecx
	call	__allmul
	mov	DWORD PTR _f9f9_38$[ebp], eax
	mov	DWORD PTR _f9f9_38$[ebp+4], edx

; 915  :     int64_t h0 = f0f0 + f1f9_76 + f2f8_38 + f3f7_76 + f4f6_38 + f5f5_38;

	mov	edx, DWORD PTR _f0f0$[ebp]
	add	edx, DWORD PTR _f1f9_76$[ebp]
	mov	eax, DWORD PTR _f0f0$[ebp+4]
	adc	eax, DWORD PTR _f1f9_76$[ebp+4]
	add	edx, DWORD PTR _f2f8_38$[ebp]
	adc	eax, DWORD PTR _f2f8_38$[ebp+4]
	add	edx, DWORD PTR _f3f7_76$[ebp]
	adc	eax, DWORD PTR _f3f7_76$[ebp+4]
	add	edx, DWORD PTR _f4f6_38$[ebp]
	adc	eax, DWORD PTR _f4f6_38$[ebp+4]
	add	edx, DWORD PTR _f5f5_38$[ebp]
	adc	eax, DWORD PTR _f5f5_38$[ebp+4]
	mov	DWORD PTR _h0$[ebp], edx
	mov	DWORD PTR _h0$[ebp+4], eax

; 916  :     int64_t h1 = f0f1_2 + f2f9_38 + f3f8_38 + f4f7_38 + f5f6_38;

	mov	ecx, DWORD PTR _f0f1_2$[ebp]
	add	ecx, DWORD PTR _f2f9_38$[ebp]
	mov	edx, DWORD PTR _f0f1_2$[ebp+4]
	adc	edx, DWORD PTR _f2f9_38$[ebp+4]
	add	ecx, DWORD PTR _f3f8_38$[ebp]
	adc	edx, DWORD PTR _f3f8_38$[ebp+4]
	add	ecx, DWORD PTR _f4f7_38$[ebp]
	adc	edx, DWORD PTR _f4f7_38$[ebp+4]
	add	ecx, DWORD PTR _f5f6_38$[ebp]
	adc	edx, DWORD PTR _f5f6_38$[ebp+4]
	mov	DWORD PTR _h1$[ebp], ecx
	mov	DWORD PTR _h1$[ebp+4], edx

; 917  :     int64_t h2 = f0f2_2 + f1f1_2 + f3f9_76 + f4f8_38 + f5f7_76 + f6f6_19;

	mov	eax, DWORD PTR _f0f2_2$[ebp]
	add	eax, DWORD PTR _f1f1_2$[ebp]
	mov	ecx, DWORD PTR _f0f2_2$[ebp+4]
	adc	ecx, DWORD PTR _f1f1_2$[ebp+4]
	add	eax, DWORD PTR _f3f9_76$[ebp]
	adc	ecx, DWORD PTR _f3f9_76$[ebp+4]
	add	eax, DWORD PTR _f4f8_38$[ebp]
	adc	ecx, DWORD PTR _f4f8_38$[ebp+4]
	add	eax, DWORD PTR _f5f7_76$[ebp]
	adc	ecx, DWORD PTR _f5f7_76$[ebp+4]
	add	eax, DWORD PTR _f6f6_19$[ebp]
	adc	ecx, DWORD PTR _f6f6_19$[ebp+4]
	mov	DWORD PTR _h2$[ebp], eax
	mov	DWORD PTR _h2$[ebp+4], ecx

; 918  :     int64_t h3 = f0f3_2 + f1f2_2 + f4f9_38 + f5f8_38 + f6f7_38;

	mov	edx, DWORD PTR _f0f3_2$[ebp]
	add	edx, DWORD PTR _f1f2_2$[ebp]
	mov	eax, DWORD PTR _f0f3_2$[ebp+4]
	adc	eax, DWORD PTR _f1f2_2$[ebp+4]
	add	edx, DWORD PTR _f4f9_38$[ebp]
	adc	eax, DWORD PTR _f4f9_38$[ebp+4]
	add	edx, DWORD PTR _f5f8_38$[ebp]
	adc	eax, DWORD PTR _f5f8_38$[ebp+4]
	add	edx, DWORD PTR _f6f7_38$[ebp]
	adc	eax, DWORD PTR _f6f7_38$[ebp+4]
	mov	DWORD PTR _h3$[ebp], edx
	mov	DWORD PTR _h3$[ebp+4], eax

; 919  :     int64_t h4 = f0f4_2 + f1f3_4 + f2f2 + f5f9_76 + f6f8_38 + f7f7_38;

	mov	ecx, DWORD PTR _f0f4_2$[ebp]
	add	ecx, DWORD PTR _f1f3_4$[ebp]
	mov	edx, DWORD PTR _f0f4_2$[ebp+4]
	adc	edx, DWORD PTR _f1f3_4$[ebp+4]
	add	ecx, DWORD PTR _f2f2$[ebp]
	adc	edx, DWORD PTR _f2f2$[ebp+4]
	add	ecx, DWORD PTR _f5f9_76$[ebp]
	adc	edx, DWORD PTR _f5f9_76$[ebp+4]
	add	ecx, DWORD PTR _f6f8_38$[ebp]
	adc	edx, DWORD PTR _f6f8_38$[ebp+4]
	add	ecx, DWORD PTR _f7f7_38$[ebp]
	adc	edx, DWORD PTR _f7f7_38$[ebp+4]
	mov	DWORD PTR _h4$[ebp], ecx
	mov	DWORD PTR _h4$[ebp+4], edx

; 920  :     int64_t h5 = f0f5_2 + f1f4_2 + f2f3_2 + f6f9_38 + f7f8_38;

	mov	eax, DWORD PTR _f0f5_2$[ebp]
	add	eax, DWORD PTR _f1f4_2$[ebp]
	mov	ecx, DWORD PTR _f0f5_2$[ebp+4]
	adc	ecx, DWORD PTR _f1f4_2$[ebp+4]
	add	eax, DWORD PTR _f2f3_2$[ebp]
	adc	ecx, DWORD PTR _f2f3_2$[ebp+4]
	add	eax, DWORD PTR _f6f9_38$[ebp]
	adc	ecx, DWORD PTR _f6f9_38$[ebp+4]
	add	eax, DWORD PTR _f7f8_38$[ebp]
	adc	ecx, DWORD PTR _f7f8_38$[ebp+4]
	mov	DWORD PTR _h5$[ebp], eax
	mov	DWORD PTR _h5$[ebp+4], ecx

; 921  :     int64_t h6 = f0f6_2 + f1f5_4 + f2f4_2 + f3f3_2 + f7f9_76 + f8f8_19;

	mov	edx, DWORD PTR _f0f6_2$[ebp]
	add	edx, DWORD PTR _f1f5_4$[ebp]
	mov	eax, DWORD PTR _f0f6_2$[ebp+4]
	adc	eax, DWORD PTR _f1f5_4$[ebp+4]
	add	edx, DWORD PTR _f2f4_2$[ebp]
	adc	eax, DWORD PTR _f2f4_2$[ebp+4]
	add	edx, DWORD PTR _f3f3_2$[ebp]
	adc	eax, DWORD PTR _f3f3_2$[ebp+4]
	add	edx, DWORD PTR _f7f9_76$[ebp]
	adc	eax, DWORD PTR _f7f9_76$[ebp+4]
	add	edx, DWORD PTR _f8f8_19$[ebp]
	adc	eax, DWORD PTR _f8f8_19$[ebp+4]
	mov	DWORD PTR _h6$[ebp], edx
	mov	DWORD PTR _h6$[ebp+4], eax

; 922  :     int64_t h7 = f0f7_2 + f1f6_2 + f2f5_2 + f3f4_2 + f8f9_38;

	mov	ecx, DWORD PTR _f0f7_2$[ebp]
	add	ecx, DWORD PTR _f1f6_2$[ebp]
	mov	edx, DWORD PTR _f0f7_2$[ebp+4]
	adc	edx, DWORD PTR _f1f6_2$[ebp+4]
	add	ecx, DWORD PTR _f2f5_2$[ebp]
	adc	edx, DWORD PTR _f2f5_2$[ebp+4]
	add	ecx, DWORD PTR _f3f4_2$[ebp]
	adc	edx, DWORD PTR _f3f4_2$[ebp+4]
	add	ecx, DWORD PTR _f8f9_38$[ebp]
	adc	edx, DWORD PTR _f8f9_38$[ebp+4]
	mov	DWORD PTR _h7$[ebp], ecx
	mov	DWORD PTR _h7$[ebp+4], edx

; 923  :     int64_t h8 = f0f8_2 + f1f7_4 + f2f6_2 + f3f5_4 + f4f4 + f9f9_38;

	mov	eax, DWORD PTR _f0f8_2$[ebp]
	add	eax, DWORD PTR _f1f7_4$[ebp]
	mov	ecx, DWORD PTR _f0f8_2$[ebp+4]
	adc	ecx, DWORD PTR _f1f7_4$[ebp+4]
	add	eax, DWORD PTR _f2f6_2$[ebp]
	adc	ecx, DWORD PTR _f2f6_2$[ebp+4]
	add	eax, DWORD PTR _f3f5_4$[ebp]
	adc	ecx, DWORD PTR _f3f5_4$[ebp+4]
	add	eax, DWORD PTR _f4f4$[ebp]
	adc	ecx, DWORD PTR _f4f4$[ebp+4]
	add	eax, DWORD PTR _f9f9_38$[ebp]
	adc	ecx, DWORD PTR _f9f9_38$[ebp+4]
	mov	DWORD PTR _h8$[ebp], eax
	mov	DWORD PTR _h8$[ebp+4], ecx

; 924  :     int64_t h9 = f0f9_2 + f1f8_2 + f2f7_2 + f3f6_2 + f4f5_2;

	mov	edx, DWORD PTR _f0f9_2$[ebp]
	add	edx, DWORD PTR _f1f8_2$[ebp]
	mov	eax, DWORD PTR _f0f9_2$[ebp+4]
	adc	eax, DWORD PTR _f1f8_2$[ebp+4]
	add	edx, DWORD PTR _f2f7_2$[ebp]
	adc	eax, DWORD PTR _f2f7_2$[ebp+4]
	add	edx, DWORD PTR _f3f6_2$[ebp]
	adc	eax, DWORD PTR _f3f6_2$[ebp+4]
	add	edx, DWORD PTR _f4f5_2$[ebp]
	adc	eax, DWORD PTR _f4f5_2$[ebp+4]
	mov	DWORD PTR _h9$[ebp], edx
	mov	DWORD PTR _h9$[ebp+4], eax

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

	mov	ecx, DWORD PTR _h0$[ebp]
	add	ecx, DWORD PTR _h0$[ebp]
	mov	edx, DWORD PTR _h0$[ebp+4]
	adc	edx, DWORD PTR _h0$[ebp+4]
	mov	DWORD PTR _h0$[ebp], ecx
	mov	DWORD PTR _h0$[ebp+4], edx

; 937  :     h1 += h1;

	mov	eax, DWORD PTR _h1$[ebp]
	add	eax, DWORD PTR _h1$[ebp]
	mov	ecx, DWORD PTR _h1$[ebp+4]
	adc	ecx, DWORD PTR _h1$[ebp+4]
	mov	DWORD PTR _h1$[ebp], eax
	mov	DWORD PTR _h1$[ebp+4], ecx

; 938  :     h2 += h2;

	mov	edx, DWORD PTR _h2$[ebp]
	add	edx, DWORD PTR _h2$[ebp]
	mov	eax, DWORD PTR _h2$[ebp+4]
	adc	eax, DWORD PTR _h2$[ebp+4]
	mov	DWORD PTR _h2$[ebp], edx
	mov	DWORD PTR _h2$[ebp+4], eax

; 939  :     h3 += h3;

	mov	ecx, DWORD PTR _h3$[ebp]
	add	ecx, DWORD PTR _h3$[ebp]
	mov	edx, DWORD PTR _h3$[ebp+4]
	adc	edx, DWORD PTR _h3$[ebp+4]
	mov	DWORD PTR _h3$[ebp], ecx
	mov	DWORD PTR _h3$[ebp+4], edx

; 940  :     h4 += h4;

	mov	eax, DWORD PTR _h4$[ebp]
	add	eax, DWORD PTR _h4$[ebp]
	mov	ecx, DWORD PTR _h4$[ebp+4]
	adc	ecx, DWORD PTR _h4$[ebp+4]
	mov	DWORD PTR _h4$[ebp], eax
	mov	DWORD PTR _h4$[ebp+4], ecx

; 941  :     h5 += h5;

	mov	edx, DWORD PTR _h5$[ebp]
	add	edx, DWORD PTR _h5$[ebp]
	mov	eax, DWORD PTR _h5$[ebp+4]
	adc	eax, DWORD PTR _h5$[ebp+4]
	mov	DWORD PTR _h5$[ebp], edx
	mov	DWORD PTR _h5$[ebp+4], eax

; 942  :     h6 += h6;

	mov	ecx, DWORD PTR _h6$[ebp]
	add	ecx, DWORD PTR _h6$[ebp]
	mov	edx, DWORD PTR _h6$[ebp+4]
	adc	edx, DWORD PTR _h6$[ebp+4]
	mov	DWORD PTR _h6$[ebp], ecx
	mov	DWORD PTR _h6$[ebp+4], edx

; 943  :     h7 += h7;

	mov	eax, DWORD PTR _h7$[ebp]
	add	eax, DWORD PTR _h7$[ebp]
	mov	ecx, DWORD PTR _h7$[ebp+4]
	adc	ecx, DWORD PTR _h7$[ebp+4]
	mov	DWORD PTR _h7$[ebp], eax
	mov	DWORD PTR _h7$[ebp+4], ecx

; 944  :     h8 += h8;

	mov	edx, DWORD PTR _h8$[ebp]
	add	edx, DWORD PTR _h8$[ebp]
	mov	eax, DWORD PTR _h8$[ebp+4]
	adc	eax, DWORD PTR _h8$[ebp+4]
	mov	DWORD PTR _h8$[ebp], edx
	mov	DWORD PTR _h8$[ebp+4], eax

; 945  :     h9 += h9;

	mov	ecx, DWORD PTR _h9$[ebp]
	add	ecx, DWORD PTR _h9$[ebp]
	mov	edx, DWORD PTR _h9$[ebp+4]
	adc	edx, DWORD PTR _h9$[ebp+4]
	mov	DWORD PTR _h9$[ebp], ecx
	mov	DWORD PTR _h9$[ebp+4], edx

; 946  : 
; 947  :     carry0 = h0 + (1 << 25);

	mov	eax, DWORD PTR _h0$[ebp]
	add	eax, 33554432				; 02000000H
	mov	ecx, DWORD PTR _h0$[ebp+4]
	adc	ecx, 0
	mov	DWORD PTR _carry0$[ebp], eax
	mov	DWORD PTR _carry0$[ebp+4], ecx

; 948  :     h1 += carry0 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry0$[ebp]
	mov	edx, DWORD PTR _carry0$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h1$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h1$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h1$[ebp], ecx
	mov	DWORD PTR _h1$[ebp+4], eax

; 949  :     h0 -= carry0 & kTop38Bits;

	mov	ecx, DWORD PTR _carry0$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry0$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h0$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h0$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h0$[ebp], eax
	mov	DWORD PTR _h0$[ebp+4], ecx

; 950  :     carry4 = h4 + (1 << 25);

	mov	edx, DWORD PTR _h4$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h4$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry4$[ebp], edx
	mov	DWORD PTR _carry4$[ebp+4], eax

; 951  :     h5 += carry4 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry4$[ebp]
	mov	edx, DWORD PTR _carry4$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h5$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h5$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h5$[ebp], ecx
	mov	DWORD PTR _h5$[ebp+4], eax

; 952  :     h4 -= carry4 & kTop38Bits;

	mov	ecx, DWORD PTR _carry4$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry4$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h4$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h4$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h4$[ebp], eax
	mov	DWORD PTR _h4$[ebp+4], ecx

; 953  : 
; 954  :     carry1 = h1 + (1 << 24);

	mov	edx, DWORD PTR _h1$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h1$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry1$[ebp], edx
	mov	DWORD PTR _carry1$[ebp+4], eax

; 955  :     h2 += carry1 >> 25;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry1$[ebp]
	mov	edx, DWORD PTR _carry1$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h2$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h2$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h2$[ebp], ecx
	mov	DWORD PTR _h2$[ebp+4], eax

; 956  :     h1 -= carry1 & kTop39Bits;

	mov	ecx, DWORD PTR _carry1$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry1$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h1$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h1$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h1$[ebp], eax
	mov	DWORD PTR _h1$[ebp+4], ecx

; 957  :     carry5 = h5 + (1 << 24);

	mov	edx, DWORD PTR _h5$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h5$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry5$[ebp], edx
	mov	DWORD PTR _carry5$[ebp+4], eax

; 958  :     h6 += carry5 >> 25;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry5$[ebp]
	mov	edx, DWORD PTR _carry5$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h6$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h6$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h6$[ebp], ecx
	mov	DWORD PTR _h6$[ebp+4], eax

; 959  :     h5 -= carry5 & kTop39Bits;

	mov	ecx, DWORD PTR _carry5$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry5$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h5$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h5$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h5$[ebp], eax
	mov	DWORD PTR _h5$[ebp+4], ecx

; 960  : 
; 961  :     carry2 = h2 + (1 << 25);

	mov	edx, DWORD PTR _h2$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h2$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry2$[ebp], edx
	mov	DWORD PTR _carry2$[ebp+4], eax

; 962  :     h3 += carry2 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry2$[ebp]
	mov	edx, DWORD PTR _carry2$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h3$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h3$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h3$[ebp], ecx
	mov	DWORD PTR _h3$[ebp+4], eax

; 963  :     h2 -= carry2 & kTop38Bits;

	mov	ecx, DWORD PTR _carry2$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry2$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h2$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h2$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h2$[ebp], eax
	mov	DWORD PTR _h2$[ebp+4], ecx

; 964  :     carry6 = h6 + (1 << 25);

	mov	edx, DWORD PTR _h6$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h6$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry6$[ebp], edx
	mov	DWORD PTR _carry6$[ebp+4], eax

; 965  :     h7 += carry6 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry6$[ebp]
	mov	edx, DWORD PTR _carry6$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h7$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h7$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h7$[ebp], ecx
	mov	DWORD PTR _h7$[ebp+4], eax

; 966  :     h6 -= carry6 & kTop38Bits;

	mov	ecx, DWORD PTR _carry6$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry6$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h6$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h6$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h6$[ebp], eax
	mov	DWORD PTR _h6$[ebp+4], ecx

; 967  : 
; 968  :     carry3 = h3 + (1 << 24);

	mov	edx, DWORD PTR _h3$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h3$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry3$[ebp], edx
	mov	DWORD PTR _carry3$[ebp+4], eax

; 969  :     h4 += carry3 >> 25;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry3$[ebp]
	mov	edx, DWORD PTR _carry3$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h4$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h4$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h4$[ebp], ecx
	mov	DWORD PTR _h4$[ebp+4], eax

; 970  :     h3 -= carry3 & kTop39Bits;

	mov	ecx, DWORD PTR _carry3$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry3$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h3$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h3$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h3$[ebp], eax
	mov	DWORD PTR _h3$[ebp+4], ecx

; 971  :     carry7 = h7 + (1 << 24);

	mov	edx, DWORD PTR _h7$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h7$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry7$[ebp], edx
	mov	DWORD PTR _carry7$[ebp+4], eax

; 972  :     h8 += carry7 >> 25;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry7$[ebp]
	mov	edx, DWORD PTR _carry7$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h8$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h8$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h8$[ebp], ecx
	mov	DWORD PTR _h8$[ebp+4], eax

; 973  :     h7 -= carry7 & kTop39Bits;

	mov	ecx, DWORD PTR _carry7$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry7$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h7$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h7$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h7$[ebp], eax
	mov	DWORD PTR _h7$[ebp+4], ecx

; 974  : 
; 975  :     carry4 = h4 + (1 << 25);

	mov	edx, DWORD PTR _h4$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h4$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry4$[ebp], edx
	mov	DWORD PTR _carry4$[ebp+4], eax

; 976  :     h5 += carry4 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry4$[ebp]
	mov	edx, DWORD PTR _carry4$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h5$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h5$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h5$[ebp], ecx
	mov	DWORD PTR _h5$[ebp+4], eax

; 977  :     h4 -= carry4 & kTop38Bits;

	mov	ecx, DWORD PTR _carry4$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry4$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h4$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h4$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h4$[ebp], eax
	mov	DWORD PTR _h4$[ebp+4], ecx

; 978  :     carry8 = h8 + (1 << 25);

	mov	edx, DWORD PTR _h8$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h8$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry8$[ebp], edx
	mov	DWORD PTR _carry8$[ebp+4], eax

; 979  :     h9 += carry8 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry8$[ebp]
	mov	edx, DWORD PTR _carry8$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h9$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h9$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h9$[ebp], ecx
	mov	DWORD PTR _h9$[ebp+4], eax

; 980  :     h8 -= carry8 & kTop38Bits;

	mov	ecx, DWORD PTR _carry8$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry8$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h8$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h8$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h8$[ebp], eax
	mov	DWORD PTR _h8$[ebp+4], ecx

; 981  : 
; 982  :     carry9 = h9 + (1 << 24);

	mov	edx, DWORD PTR _h9$[ebp]
	add	edx, 16777216				; 01000000H
	mov	eax, DWORD PTR _h9$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry9$[ebp], edx
	mov	DWORD PTR _carry9$[ebp+4], eax

; 983  :     h0 += (carry9 >> 25) * 19;

	mov	ecx, 25					; 00000019H
	mov	eax, DWORD PTR _carry9$[ebp]
	mov	edx, DWORD PTR _carry9$[ebp+4]
	call	__allshr
	push	0
	push	19					; 00000013H
	push	edx
	push	eax
	call	__allmul
	mov	ecx, DWORD PTR _h0$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h0$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h0$[ebp], ecx
	mov	DWORD PTR _h0$[ebp+4], eax

; 984  :     h9 -= carry9 & kTop39Bits;

	mov	ecx, DWORD PTR _carry9$[ebp]
	and	ecx, DWORD PTR _kTop39Bits
	mov	edx, DWORD PTR _carry9$[ebp+4]
	and	edx, DWORD PTR _kTop39Bits+4
	mov	eax, DWORD PTR _h9$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h9$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h9$[ebp], eax
	mov	DWORD PTR _h9$[ebp+4], ecx

; 985  : 
; 986  :     carry0 = h0 + (1 << 25);

	mov	edx, DWORD PTR _h0$[ebp]
	add	edx, 33554432				; 02000000H
	mov	eax, DWORD PTR _h0$[ebp+4]
	adc	eax, 0
	mov	DWORD PTR _carry0$[ebp], edx
	mov	DWORD PTR _carry0$[ebp+4], eax

; 987  :     h1 += carry0 >> 26;

	mov	ecx, 26					; 0000001aH
	mov	eax, DWORD PTR _carry0$[ebp]
	mov	edx, DWORD PTR _carry0$[ebp+4]
	call	__allshr
	mov	ecx, DWORD PTR _h1$[ebp]
	add	ecx, eax
	mov	eax, DWORD PTR _h1$[ebp+4]
	adc	eax, edx
	mov	DWORD PTR _h1$[ebp], ecx
	mov	DWORD PTR _h1$[ebp+4], eax

; 988  :     h0 -= carry0 & kTop38Bits;

	mov	ecx, DWORD PTR _carry0$[ebp]
	and	ecx, DWORD PTR _kTop38Bits
	mov	edx, DWORD PTR _carry0$[ebp+4]
	and	edx, DWORD PTR _kTop38Bits+4
	mov	eax, DWORD PTR _h0$[ebp]
	sub	eax, ecx
	mov	ecx, DWORD PTR _h0$[ebp+4]
	sbb	ecx, edx
	mov	DWORD PTR _h0$[ebp], eax
	mov	DWORD PTR _h0$[ebp+4], ecx

; 989  : 
; 990  :     h[0] = (int32_t)h0;

	mov	edx, DWORD PTR _h0$[ebp]
	mov	eax, DWORD PTR _h$[ebp]
	mov	DWORD PTR [eax], edx

; 991  :     h[1] = (int32_t)h1;

	mov	ecx, DWORD PTR _h1$[ebp]
	mov	edx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [edx+4], ecx

; 992  :     h[2] = (int32_t)h2;

	mov	eax, DWORD PTR _h2$[ebp]
	mov	ecx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [ecx+8], eax

; 993  :     h[3] = (int32_t)h3;

	mov	edx, DWORD PTR _h3$[ebp]
	mov	eax, DWORD PTR _h$[ebp]
	mov	DWORD PTR [eax+12], edx

; 994  :     h[4] = (int32_t)h4;

	mov	ecx, DWORD PTR _h4$[ebp]
	mov	edx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [edx+16], ecx

; 995  :     h[5] = (int32_t)h5;

	mov	eax, DWORD PTR _h5$[ebp]
	mov	ecx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [ecx+20], eax

; 996  :     h[6] = (int32_t)h6;

	mov	edx, DWORD PTR _h6$[ebp]
	mov	eax, DWORD PTR _h$[ebp]
	mov	DWORD PTR [eax+24], edx

; 997  :     h[7] = (int32_t)h7;

	mov	ecx, DWORD PTR _h7$[ebp]
	mov	edx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [edx+28], ecx

; 998  :     h[8] = (int32_t)h8;

	mov	eax, DWORD PTR _h8$[ebp]
	mov	ecx, DWORD PTR _h$[ebp]
	mov	DWORD PTR [ecx+32], eax

; 999  :     h[9] = (int32_t)h9;

	mov	edx, DWORD PTR _h9$[ebp]
	mov	eax, DWORD PTR _h$[ebp]
	mov	DWORD PTR [eax+36], edx

; 1000 : }

	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	0
_fe_sq2	ENDP
_r$ = 8
_p$ = 12
_q$ = -120
_ge_p3_dbl PROC NEAR

; 1193 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 120				; 00000078H

; 1194 :     ge_p2 q;
; 1195 :     ge_p3_to_p2(&q, p);

	mov	eax, DWORD PTR _p$[ebp]
	push	eax
	lea	ecx, DWORD PTR _q$[ebp]
	push	ecx
	call	_ge_p3_to_p2
	add	esp, 8

; 1196 :     ge_p2_dbl(r, &q);

	lea	edx, DWORD PTR _q$[ebp]
	push	edx
	mov	eax, DWORD PTR _r$[ebp]
	push	eax
	call	_ge_p2_dbl
	add	esp, 8

; 1197 : }

	mov	esp, ebp
	pop	ebp
	ret	0
_ge_p3_dbl ENDP
_r$ = 8
_p$ = 12
_ge_p3_to_p2 PROC NEAR

; 1143 : {

	push	ebp
	mov	ebp, esp

; 1144 :     fe_copy(r->X, p->X);

	mov	eax, DWORD PTR _p$[ebp]
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	call	_fe_copy
	add	esp, 8

; 1145 :     fe_copy(r->Y, p->Y);

	mov	edx, DWORD PTR _p$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	call	_fe_copy
	add	esp, 8

; 1146 :     fe_copy(r->Z, p->Z);

	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 80					; 00000050H
	push	edx
	call	_fe_copy
	add	esp, 8

; 1147 : }

	pop	ebp
	ret	0
_ge_p3_to_p2 ENDP
_r$ = 8
_p$ = 12
_q$ = 16
_t0$ = -40
_ge_madd PROC NEAR

; 1200 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 40					; 00000028H

; 1201 :     fe t0;
; 1202 : 
; 1203 :     fe_add(r->X, p->Y, p->X);

	mov	eax, DWORD PTR _p$[ebp]
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1204 :     fe_sub(r->Y, p->Y, p->X);

	mov	eax, DWORD PTR _p$[ebp]
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1205 :     fe_mul(r->Z, r->X, q->yplusx);

	mov	eax, DWORD PTR _q$[ebp]
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 80					; 00000050H
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1206 :     fe_mul(r->Y, r->Y, q->yminusx);

	mov	eax, DWORD PTR _q$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1207 :     fe_mul(r->T, q->xy2d, p->T);

	mov	eax, DWORD PTR _p$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	mov	ecx, DWORD PTR _q$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 120				; 00000078H
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1208 :     fe_add(t0, p->Z, p->Z);

	mov	eax, DWORD PTR _p$[ebp]
	add	eax, 80					; 00000050H
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1209 :     fe_sub(r->X, r->Z, r->Y);

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1210 :     fe_add(r->Y, r->Z, r->Y);

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1211 :     fe_add(r->Z, t0, r->T);

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 80					; 00000050H
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1212 :     fe_sub(r->T, t0, r->T);

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 120				; 00000078H
	push	edx
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1213 : }

	mov	esp, ebp
	pop	ebp
	ret	0
_ge_madd ENDP
_r$ = 8
_p$ = 12
_q$ = 16
_t0$ = -40
_ge_msub PROC NEAR

; 1216 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 40					; 00000028H

; 1217 :     fe t0;
; 1218 : 
; 1219 :     fe_add(r->X, p->Y, p->X);

	mov	eax, DWORD PTR _p$[ebp]
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1220 :     fe_sub(r->Y, p->Y, p->X);

	mov	eax, DWORD PTR _p$[ebp]
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1221 :     fe_mul(r->Z, r->X, q->yminusx);

	mov	eax, DWORD PTR _q$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 80					; 00000050H
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1222 :     fe_mul(r->Y, r->Y, q->yplusx);

	mov	eax, DWORD PTR _q$[ebp]
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1223 :     fe_mul(r->T, q->xy2d, p->T);

	mov	eax, DWORD PTR _p$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	mov	ecx, DWORD PTR _q$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 120				; 00000078H
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1224 :     fe_add(t0, p->Z, p->Z);

	mov	eax, DWORD PTR _p$[ebp]
	add	eax, 80					; 00000050H
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1225 :     fe_sub(r->X, r->Z, r->Y);

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1226 :     fe_add(r->Y, r->Z, r->Y);

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1227 :     fe_sub(r->Z, t0, r->T);

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 80					; 00000050H
	push	edx
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1228 :     fe_add(r->T, t0, r->T);

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 120				; 00000078H
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1229 : }

	mov	esp, ebp
	pop	ebp
	ret	0
_ge_msub ENDP
_r$ = 8
_p$ = 12
_q$ = 16
_t0$ = -40
_ge_add	PROC NEAR

; 1232 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 40					; 00000028H

; 1233 :     fe t0;
; 1234 : 
; 1235 :     fe_add(r->X, p->Y, p->X);

	mov	eax, DWORD PTR _p$[ebp]
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1236 :     fe_sub(r->Y, p->Y, p->X);

	mov	eax, DWORD PTR _p$[ebp]
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1237 :     fe_mul(r->Z, r->X, q->YplusX);

	mov	eax, DWORD PTR _q$[ebp]
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 80					; 00000050H
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1238 :     fe_mul(r->Y, r->Y, q->YminusX);

	mov	eax, DWORD PTR _q$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1239 :     fe_mul(r->T, q->T2d, p->T);

	mov	eax, DWORD PTR _p$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	mov	ecx, DWORD PTR _q$[ebp]
	add	ecx, 120				; 00000078H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 120				; 00000078H
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1240 :     fe_mul(r->X, p->Z, q->Z);

	mov	eax, DWORD PTR _q$[ebp]
	add	eax, 80					; 00000050H
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1241 :     fe_add(t0, r->X, r->X);

	mov	eax, DWORD PTR _r$[ebp]
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1242 :     fe_sub(r->X, r->Z, r->Y);

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1243 :     fe_add(r->Y, r->Z, r->Y);

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1244 :     fe_add(r->Z, t0, r->T);

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 80					; 00000050H
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1245 :     fe_sub(r->T, t0, r->T);

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 120				; 00000078H
	push	edx
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1246 : }

	mov	esp, ebp
	pop	ebp
	ret	0
_ge_add	ENDP
_r$ = 8
_p$ = 12
_q$ = 16
_t0$ = -40
_ge_sub	PROC NEAR

; 1249 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 40					; 00000028H

; 1250 :     fe t0;
; 1251 : 
; 1252 :     fe_add(r->X, p->Y, p->X);

	mov	eax, DWORD PTR _p$[ebp]
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1253 :     fe_sub(r->Y, p->Y, p->X);

	mov	eax, DWORD PTR _p$[ebp]
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1254 :     fe_mul(r->Z, r->X, q->YminusX);

	mov	eax, DWORD PTR _q$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 80					; 00000050H
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1255 :     fe_mul(r->Y, r->Y, q->YplusX);

	mov	eax, DWORD PTR _q$[ebp]
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, 40					; 00000028H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1256 :     fe_mul(r->T, q->T2d, p->T);

	mov	eax, DWORD PTR _p$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	mov	ecx, DWORD PTR _q$[ebp]
	add	ecx, 120				; 00000078H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 120				; 00000078H
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1257 :     fe_mul(r->X, p->Z, q->Z);

	mov	eax, DWORD PTR _q$[ebp]
	add	eax, 80					; 00000050H
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	_fe_mul
	add	esp, 12					; 0000000cH

; 1258 :     fe_add(t0, r->X, r->X);

	mov	eax, DWORD PTR _r$[ebp]
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	push	ecx
	lea	edx, DWORD PTR _t0$[ebp]
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1259 :     fe_sub(r->X, r->Z, r->Y);

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	push	edx
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1260 :     fe_add(r->Y, r->Z, r->Y);

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 40					; 00000028H
	push	eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 40					; 00000028H
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1261 :     fe_sub(r->Z, t0, r->T);

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 80					; 00000050H
	push	edx
	call	_fe_sub
	add	esp, 12					; 0000000cH

; 1262 :     fe_add(r->T, t0, r->T);

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, 120				; 00000078H
	push	eax
	lea	ecx, DWORD PTR _t0$[ebp]
	push	ecx
	mov	edx, DWORD PTR _r$[ebp]
	add	edx, 120				; 00000078H
	push	edx
	call	_fe_add
	add	esp, 12					; 0000000cH

; 1263 : }

	mov	esp, ebp
	pop	ebp
	ret	0
_ge_sub	ENDP
_r$ = 8
_a$ = 12
_i$ = -8
_b$ = -4
_k$ = -12
_slide	PROC NEAR

; 1266 : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH

; 1267 :     int i;
; 1268 :     int b;
; 1269 :     int k;
; 1270 : 
; 1271 :     for (i = 0; i < 256; ++i) {

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L1878
$L1879:
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 1
	mov	DWORD PTR _i$[ebp], eax
$L1878:
	cmp	DWORD PTR _i$[ebp], 256			; 00000100H
	jge	SHORT $L1880

; 1272 :         r[i] = (signed char)(1 & (a[i >> 3] >> (i & 7)));

	mov	ecx, DWORD PTR _i$[ebp]
	sar	ecx, 3
	mov	edx, DWORD PTR _a$[ebp]
	xor	eax, eax
	mov	al, BYTE PTR [edx+ecx]
	mov	ecx, DWORD PTR _i$[ebp]
	and	ecx, 7
	sar	eax, cl
	and	eax, 1
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, DWORD PTR _i$[ebp]
	mov	BYTE PTR [ecx], al

; 1273 :     }

	jmp	SHORT $L1879
$L1880:

; 1274 : 
; 1275 :     for (i = 0; i < 256; ++i) {

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L1882
$L1883:
	mov	edx, DWORD PTR _i$[ebp]
	add	edx, 1
	mov	DWORD PTR _i$[ebp], edx
$L1882:
	cmp	DWORD PTR _i$[ebp], 256			; 00000100H
	jge	$L1884

; 1276 :         if (r[i]) {

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, DWORD PTR _i$[ebp]
	movsx	ecx, BYTE PTR [eax]
	test	ecx, ecx
	je	$L1888

; 1277 :             for (b = 1; b <= 6 && i + b < 256; ++b) {

	mov	DWORD PTR _b$[ebp], 1
	jmp	SHORT $L1886
$L1887:
	mov	edx, DWORD PTR _b$[ebp]
	add	edx, 1
	mov	DWORD PTR _b$[ebp], edx
$L1886:
	cmp	DWORD PTR _b$[ebp], 6
	jg	$L1888
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, DWORD PTR _b$[ebp]
	cmp	eax, 256				; 00000100H
	jge	$L1888

; 1278 :                 if (r[i + b]) {

	mov	ecx, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _b$[ebp]
	mov	edx, DWORD PTR _r$[ebp]
	movsx	eax, BYTE PTR [edx+ecx]
	test	eax, eax
	je	$L1899

; 1279 :                     if (r[i] + (r[i + b] << b) <= 15) {

	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, DWORD PTR _i$[ebp]
	movsx	edx, BYTE PTR [ecx]
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, DWORD PTR _b$[ebp]
	mov	ecx, DWORD PTR _r$[ebp]
	movsx	eax, BYTE PTR [ecx+eax]
	mov	ecx, DWORD PTR _b$[ebp]
	shl	eax, cl
	add	edx, eax
	cmp	edx, 15					; 0000000fH
	jg	SHORT $L1890

; 1280 :                         r[i] = (signed char)(r[i] + (r[i + b] << b));

	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, DWORD PTR _i$[ebp]
	movsx	edx, BYTE PTR [ecx]
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, DWORD PTR _b$[ebp]
	mov	ecx, DWORD PTR _r$[ebp]
	movsx	eax, BYTE PTR [ecx+eax]
	mov	ecx, DWORD PTR _b$[ebp]
	shl	eax, cl
	add	edx, eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, DWORD PTR _i$[ebp]
	mov	BYTE PTR [ecx], dl

; 1281 :                         r[i + b] = 0;

	mov	edx, DWORD PTR _i$[ebp]
	add	edx, DWORD PTR _b$[ebp]
	mov	eax, DWORD PTR _r$[ebp]
	mov	BYTE PTR [eax+edx], 0

; 1282 :                     } else if (r[i] - (r[i + b] << b) >= -15) {

	jmp	$L1899
$L1890:
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, DWORD PTR _i$[ebp]
	movsx	edx, BYTE PTR [ecx]
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, DWORD PTR _b$[ebp]
	mov	ecx, DWORD PTR _r$[ebp]
	movsx	eax, BYTE PTR [ecx+eax]
	mov	ecx, DWORD PTR _b$[ebp]
	shl	eax, cl
	sub	edx, eax
	cmp	edx, -15				; fffffff1H
	jl	SHORT $L1893

; 1283 :                         r[i] = (signed char)(r[i] - (r[i + b] << b));

	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, DWORD PTR _i$[ebp]
	movsx	edx, BYTE PTR [ecx]
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, DWORD PTR _b$[ebp]
	mov	ecx, DWORD PTR _r$[ebp]
	movsx	eax, BYTE PTR [ecx+eax]
	mov	ecx, DWORD PTR _b$[ebp]
	shl	eax, cl
	sub	edx, eax
	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, DWORD PTR _i$[ebp]
	mov	BYTE PTR [ecx], dl

; 1284 :                         for (k = i + b; k < 256; ++k) {

	mov	edx, DWORD PTR _i$[ebp]
	add	edx, DWORD PTR _b$[ebp]
	mov	DWORD PTR _k$[ebp], edx
	jmp	SHORT $L1895
$L1896:
	mov	eax, DWORD PTR _k$[ebp]
	add	eax, 1
	mov	DWORD PTR _k$[ebp], eax
$L1895:
	cmp	DWORD PTR _k$[ebp], 256			; 00000100H
	jge	SHORT $L1897

; 1285 :                             if (!r[k]) {

	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, DWORD PTR _k$[ebp]
	movsx	edx, BYTE PTR [ecx]
	test	edx, edx
	jne	SHORT $L1898

; 1286 :                                 r[k] = 1;

	mov	eax, DWORD PTR _r$[ebp]
	add	eax, DWORD PTR _k$[ebp]
	mov	BYTE PTR [eax], 1

; 1287 :                                 break;

	jmp	SHORT $L1897
$L1898:

; 1289 :                             r[k] = 0;

	mov	ecx, DWORD PTR _r$[ebp]
	add	ecx, DWORD PTR _k$[ebp]
	mov	BYTE PTR [ecx], 0

; 1290 :                         }

	jmp	SHORT $L1896
$L1897:

; 1291 :                     } else {

	jmp	SHORT $L1899
$L1893:

; 1292 :                         break;

	jmp	SHORT $L1888
$L1899:

; 1295 :             }

	jmp	$L1887
$L1888:

; 1297 :     }

	jmp	$L1883
$L1884:

; 1298 : }

	mov	esp, ebp
	pop	ebp
	ret	0
_slide	ENDP
_TEXT	ENDS
END
