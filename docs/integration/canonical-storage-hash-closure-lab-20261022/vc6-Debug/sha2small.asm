	TITLE	C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\vendor\bearssl-0.6\src\hash\sha2small.c
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
PUBLIC	_br_sha224_IV
PUBLIC	_br_sha256_IV
PUBLIC	_br_sha224_vtable
PUBLIC	_br_sha224_init
PUBLIC	_br_sha224_update
PUBLIC	_br_sha224_out
PUBLIC	_br_sha224_state
PUBLIC	_br_sha224_set_state
PUBLIC	_br_sha256_vtable
PUBLIC	_br_sha256_init
PUBLIC	_br_sha256_out
CONST	SEGMENT
_br_sha224_IV DD 0c1059ed8H
	DD	0367cd507H
	DD	03070dd17H
	DD	0f70e5939H
	DD	0ffc00b31H
	DD	068581511H
	DD	064f98fa7H
	DD	0befa4fa4H
_br_sha256_IV DD 06a09e667H
	DD	0bb67ae85H
	DD	03c6ef372H
	DD	0a54ff53aH
	DD	0510e527fH
	DD	09b05688cH
	DD	01f83d9abH
	DD	05be0cd19H
_K	DD	0428a2f98H
	DD	071374491H
	DD	0b5c0fbcfH
	DD	0e9b5dba5H
	DD	03956c25bH
	DD	059f111f1H
	DD	0923f82a4H
	DD	0ab1c5ed5H
	DD	0d807aa98H
	DD	012835b01H
	DD	0243185beH
	DD	0550c7dc3H
	DD	072be5d74H
	DD	080deb1feH
	DD	09bdc06a7H
	DD	0c19bf174H
	DD	0e49b69c1H
	DD	0efbe4786H
	DD	0fc19dc6H
	DD	0240ca1ccH
	DD	02de92c6fH
	DD	04a7484aaH
	DD	05cb0a9dcH
	DD	076f988daH
	DD	0983e5152H
	DD	0a831c66dH
	DD	0b00327c8H
	DD	0bf597fc7H
	DD	0c6e00bf3H
	DD	0d5a79147H
	DD	06ca6351H
	DD	014292967H
	DD	027b70a85H
	DD	02e1b2138H
	DD	04d2c6dfcH
	DD	053380d13H
	DD	0650a7354H
	DD	0766a0abbH
	DD	081c2c92eH
	DD	092722c85H
	DD	0a2bfe8a1H
	DD	0a81a664bH
	DD	0c24b8b70H
	DD	0c76c51a3H
	DD	0d192e819H
	DD	0d6990624H
	DD	0f40e3585H
	DD	0106aa070H
	DD	019a4c116H
	DD	01e376c08H
	DD	02748774cH
	DD	034b0bcb5H
	DD	0391c0cb3H
	DD	04ed8aa4aH
	DD	05b9cca4fH
	DD	0682e6ff3H
	DD	0748f82eeH
	DD	078a5636fH
	DD	084c87814H
	DD	08cc70208H
	DD	090befffaH
	DD	0a4506cebH
	DD	0bef9a3f7H
	DD	0c67178f2H
_br_sha224_vtable DD 070H
	DD	053101c03H
	DD	FLAT:_br_sha224_init
	DD	FLAT:_br_sha224_update
	DD	FLAT:_br_sha224_out
	DD	FLAT:_br_sha224_state
	DD	FLAT:_br_sha224_set_state
	ORG $+4
_br_sha256_vtable DD 070H
	DD	053102004H
	DD	FLAT:_br_sha256_init
	DD	FLAT:_br_sha224_update
	DD	FLAT:_br_sha256_out
	DD	FLAT:_br_sha224_state
	DD	FLAT:_br_sha224_set_state
CONST	ENDS
PUBLIC	_br_sha2small_round
EXTRN	_br_range_dec32be:NEAR
_TEXT	SEGMENT
_buf$ = 8
_val$ = 12
_i$ = -292
_a$ = -4
_b$ = -8
_c$ = -12
_d$ = -16
_e$ = -20
_f$ = -24
_g$ = -284
_h$ = -288
_w$ = -280
_T1$1108 = -296
_T2$1109 = -300
_T1$1125 = -304
_T2$1126 = -308
_T1$1142 = -312
_T2$1143 = -316
_T1$1159 = -320
_T2$1160 = -324
_T1$1176 = -328
_T2$1177 = -332
_T1$1193 = -336
_T2$1194 = -340
_T1$1210 = -344
_T2$1211 = -348
_T1$1227 = -352
_T2$1228 = -356
_br_sha2small_round PROC NEAR

; 71   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 356				; 00000164H
	push	esi

; 72   : 
; 73   : #define SHA2_STEP(A, B, C, D, E, F, G, H, j)   do { \
; 74   : 		uint32_t T1, T2; \
; 75   : 		T1 = H + BSG2_1(E) + CH(E, F, G) + K[j] + w[j]; \
; 76   : 		T2 = BSG2_0(A) + MAJ(A, B, C); \
; 77   : 		D += T1; \
; 78   : 		H = T1 + T2; \
; 79   : 	} while (0)
; 80   : 
; 81   : 	int i;
; 82   : 	uint32_t a, b, c, d, e, f, g, h;
; 83   : 	uint32_t w[64];
; 84   : 
; 85   : 	br_range_dec32be(w, 16, buf);

	mov	eax, DWORD PTR _buf$[ebp]
	push	eax
	push	16					; 00000010H
	lea	ecx, DWORD PTR _w$[ebp]
	push	ecx
	call	_br_range_dec32be
	add	esp, 12					; 0000000cH

; 86   : 	for (i = 16; i < 64; i ++) {

	mov	DWORD PTR _i$[ebp], 16			; 00000010H
	jmp	SHORT $L1089
$L1090:
	mov	edx, DWORD PTR _i$[ebp]
	add	edx, 1
	mov	DWORD PTR _i$[ebp], edx
$L1089:
	cmp	DWORD PTR _i$[ebp], 64			; 00000040H
	jge	$L1091

; 87   : 		w[i] = SSG2_1(w[i - 2]) + w[i - 7]
; 88   : 			+ SSG2_0(w[i - 15]) + w[i - 16];

	mov	eax, DWORD PTR _i$[ebp]
	mov	ecx, DWORD PTR _w$[ebp+eax*4-8]
	shl	ecx, 15					; 0000000fH
	mov	edx, DWORD PTR _i$[ebp]
	mov	eax, DWORD PTR _w$[ebp+edx*4-8]
	shr	eax, 17					; 00000011H
	or	ecx, eax
	mov	edx, DWORD PTR _i$[ebp]
	mov	eax, DWORD PTR _w$[ebp+edx*4-8]
	shl	eax, 13					; 0000000dH
	mov	edx, DWORD PTR _i$[ebp]
	mov	edx, DWORD PTR _w$[ebp+edx*4-8]
	shr	edx, 19					; 00000013H
	or	eax, edx
	xor	ecx, eax
	mov	eax, DWORD PTR _i$[ebp]
	mov	edx, DWORD PTR _w$[ebp+eax*4-8]
	shr	edx, 10					; 0000000aH
	xor	ecx, edx
	mov	eax, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _w$[ebp+eax*4-28]
	mov	edx, DWORD PTR _i$[ebp]
	mov	eax, DWORD PTR _w$[ebp+edx*4-60]
	shl	eax, 25					; 00000019H
	mov	edx, DWORD PTR _i$[ebp]
	mov	edx, DWORD PTR _w$[ebp+edx*4-60]
	shr	edx, 7
	or	eax, edx
	mov	edx, DWORD PTR _i$[ebp]
	mov	edx, DWORD PTR _w$[ebp+edx*4-60]
	shl	edx, 14					; 0000000eH
	mov	esi, DWORD PTR _i$[ebp]
	mov	esi, DWORD PTR _w$[ebp+esi*4-60]
	shr	esi, 18					; 00000012H
	or	edx, esi
	xor	eax, edx
	mov	edx, DWORD PTR _i$[ebp]
	mov	edx, DWORD PTR _w$[ebp+edx*4-60]
	shr	edx, 3
	xor	eax, edx
	add	ecx, eax
	mov	eax, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _w$[ebp+eax*4-64]
	mov	edx, DWORD PTR _i$[ebp]
	mov	DWORD PTR _w$[ebp+edx*4], ecx

; 89   : 	}

	jmp	$L1090
$L1091:

; 90   : 	a = val[0];

	mov	eax, DWORD PTR _val$[ebp]
	mov	ecx, DWORD PTR [eax]
	mov	DWORD PTR _a$[ebp], ecx

; 91   : 	b = val[1];

	mov	edx, DWORD PTR _val$[ebp]
	mov	eax, DWORD PTR [edx+4]
	mov	DWORD PTR _b$[ebp], eax

; 92   : 	c = val[2];

	mov	ecx, DWORD PTR _val$[ebp]
	mov	edx, DWORD PTR [ecx+8]
	mov	DWORD PTR _c$[ebp], edx

; 93   : 	d = val[3];

	mov	eax, DWORD PTR _val$[ebp]
	mov	ecx, DWORD PTR [eax+12]
	mov	DWORD PTR _d$[ebp], ecx

; 94   : 	e = val[4];

	mov	edx, DWORD PTR _val$[ebp]
	mov	eax, DWORD PTR [edx+16]
	mov	DWORD PTR _e$[ebp], eax

; 95   : 	f = val[5];

	mov	ecx, DWORD PTR _val$[ebp]
	mov	edx, DWORD PTR [ecx+20]
	mov	DWORD PTR _f$[ebp], edx

; 96   : 	g = val[6];

	mov	eax, DWORD PTR _val$[ebp]
	mov	ecx, DWORD PTR [eax+24]
	mov	DWORD PTR _g$[ebp], ecx

; 97   : 	h = val[7];

	mov	edx, DWORD PTR _val$[ebp]
	mov	eax, DWORD PTR [edx+28]
	mov	DWORD PTR _h$[ebp], eax

; 98   : 	for (i = 0; i < 64; i += 8) {

	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L1102
$L1103:
	mov	ecx, DWORD PTR _i$[ebp]
	add	ecx, 8
	mov	DWORD PTR _i$[ebp], ecx
$L1102:
	cmp	DWORD PTR _i$[ebp], 64			; 00000040H
	jge	$L1104
$L1105:

; 99   : 		SHA2_STEP(a, b, c, d, e, f, g, h, i + 0);

	mov	edx, DWORD PTR _e$[ebp]
	shl	edx, 26					; 0000001aH
	mov	eax, DWORD PTR _e$[ebp]
	shr	eax, 6
	or	edx, eax
	mov	ecx, DWORD PTR _e$[ebp]
	shl	ecx, 21					; 00000015H
	mov	eax, DWORD PTR _e$[ebp]
	shr	eax, 11					; 0000000bH
	or	ecx, eax
	xor	edx, ecx
	mov	ecx, DWORD PTR _e$[ebp]
	shl	ecx, 7
	mov	eax, DWORD PTR _e$[ebp]
	shr	eax, 25					; 00000019H
	or	ecx, eax
	xor	edx, ecx
	mov	ecx, DWORD PTR _h$[ebp]
	add	ecx, edx
	mov	edx, DWORD PTR _f$[ebp]
	xor	edx, DWORD PTR _g$[ebp]
	and	edx, DWORD PTR _e$[ebp]
	xor	edx, DWORD PTR _g$[ebp]
	add	ecx, edx
	mov	eax, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _K[eax*4]
	mov	edx, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _w$[ebp+edx*4]
	mov	DWORD PTR _T1$1108[ebp], ecx
	mov	eax, DWORD PTR _a$[ebp]
	shl	eax, 30					; 0000001eH
	mov	ecx, DWORD PTR _a$[ebp]
	shr	ecx, 2
	or	eax, ecx
	mov	edx, DWORD PTR _a$[ebp]
	shl	edx, 19					; 00000013H
	mov	ecx, DWORD PTR _a$[ebp]
	shr	ecx, 13					; 0000000dH
	or	edx, ecx
	xor	eax, edx
	mov	edx, DWORD PTR _a$[ebp]
	shl	edx, 10					; 0000000aH
	mov	ecx, DWORD PTR _a$[ebp]
	shr	ecx, 22					; 00000016H
	or	edx, ecx
	xor	eax, edx
	mov	edx, DWORD PTR _b$[ebp]
	and	edx, DWORD PTR _c$[ebp]
	mov	ecx, DWORD PTR _b$[ebp]
	or	ecx, DWORD PTR _c$[ebp]
	and	ecx, DWORD PTR _a$[ebp]
	or	edx, ecx
	add	eax, edx
	mov	DWORD PTR _T2$1109[ebp], eax
	mov	edx, DWORD PTR _d$[ebp]
	add	edx, DWORD PTR _T1$1108[ebp]
	mov	DWORD PTR _d$[ebp], edx
	mov	eax, DWORD PTR _T1$1108[ebp]
	add	eax, DWORD PTR _T2$1109[ebp]
	mov	DWORD PTR _h$[ebp], eax
	xor	ecx, ecx
	test	ecx, ecx
	jne	$L1105
$L1107:

; 100  : 		SHA2_STEP(h, a, b, c, d, e, f, g, i + 1);

	mov	edx, DWORD PTR _d$[ebp]
	shl	edx, 26					; 0000001aH
	mov	eax, DWORD PTR _d$[ebp]
	shr	eax, 6
	or	edx, eax
	mov	ecx, DWORD PTR _d$[ebp]
	shl	ecx, 21					; 00000015H
	mov	eax, DWORD PTR _d$[ebp]
	shr	eax, 11					; 0000000bH
	or	ecx, eax
	xor	edx, ecx
	mov	ecx, DWORD PTR _d$[ebp]
	shl	ecx, 7
	mov	eax, DWORD PTR _d$[ebp]
	shr	eax, 25					; 00000019H
	or	ecx, eax
	xor	edx, ecx
	mov	ecx, DWORD PTR _g$[ebp]
	add	ecx, edx
	mov	edx, DWORD PTR _e$[ebp]
	xor	edx, DWORD PTR _f$[ebp]
	and	edx, DWORD PTR _d$[ebp]
	xor	edx, DWORD PTR _f$[ebp]
	add	ecx, edx
	mov	eax, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _K[eax*4+4]
	mov	edx, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _w$[ebp+edx*4+4]
	mov	DWORD PTR _T1$1125[ebp], ecx
	mov	eax, DWORD PTR _h$[ebp]
	shl	eax, 30					; 0000001eH
	mov	ecx, DWORD PTR _h$[ebp]
	shr	ecx, 2
	or	eax, ecx
	mov	edx, DWORD PTR _h$[ebp]
	shl	edx, 19					; 00000013H
	mov	ecx, DWORD PTR _h$[ebp]
	shr	ecx, 13					; 0000000dH
	or	edx, ecx
	xor	eax, edx
	mov	edx, DWORD PTR _h$[ebp]
	shl	edx, 10					; 0000000aH
	mov	ecx, DWORD PTR _h$[ebp]
	shr	ecx, 22					; 00000016H
	or	edx, ecx
	xor	eax, edx
	mov	edx, DWORD PTR _a$[ebp]
	and	edx, DWORD PTR _b$[ebp]
	mov	ecx, DWORD PTR _a$[ebp]
	or	ecx, DWORD PTR _b$[ebp]
	and	ecx, DWORD PTR _h$[ebp]
	or	edx, ecx
	add	eax, edx
	mov	DWORD PTR _T2$1126[ebp], eax
	mov	edx, DWORD PTR _c$[ebp]
	add	edx, DWORD PTR _T1$1125[ebp]
	mov	DWORD PTR _c$[ebp], edx
	mov	eax, DWORD PTR _T1$1125[ebp]
	add	eax, DWORD PTR _T2$1126[ebp]
	mov	DWORD PTR _g$[ebp], eax
	xor	ecx, ecx
	test	ecx, ecx
	jne	$L1107
$L1124:

; 101  : 		SHA2_STEP(g, h, a, b, c, d, e, f, i + 2);

	mov	edx, DWORD PTR _c$[ebp]
	shl	edx, 26					; 0000001aH
	mov	eax, DWORD PTR _c$[ebp]
	shr	eax, 6
	or	edx, eax
	mov	ecx, DWORD PTR _c$[ebp]
	shl	ecx, 21					; 00000015H
	mov	eax, DWORD PTR _c$[ebp]
	shr	eax, 11					; 0000000bH
	or	ecx, eax
	xor	edx, ecx
	mov	ecx, DWORD PTR _c$[ebp]
	shl	ecx, 7
	mov	eax, DWORD PTR _c$[ebp]
	shr	eax, 25					; 00000019H
	or	ecx, eax
	xor	edx, ecx
	mov	ecx, DWORD PTR _f$[ebp]
	add	ecx, edx
	mov	edx, DWORD PTR _d$[ebp]
	xor	edx, DWORD PTR _e$[ebp]
	and	edx, DWORD PTR _c$[ebp]
	xor	edx, DWORD PTR _e$[ebp]
	add	ecx, edx
	mov	eax, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _K[eax*4+8]
	mov	edx, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _w$[ebp+edx*4+8]
	mov	DWORD PTR _T1$1142[ebp], ecx
	mov	eax, DWORD PTR _g$[ebp]
	shl	eax, 30					; 0000001eH
	mov	ecx, DWORD PTR _g$[ebp]
	shr	ecx, 2
	or	eax, ecx
	mov	edx, DWORD PTR _g$[ebp]
	shl	edx, 19					; 00000013H
	mov	ecx, DWORD PTR _g$[ebp]
	shr	ecx, 13					; 0000000dH
	or	edx, ecx
	xor	eax, edx
	mov	edx, DWORD PTR _g$[ebp]
	shl	edx, 10					; 0000000aH
	mov	ecx, DWORD PTR _g$[ebp]
	shr	ecx, 22					; 00000016H
	or	edx, ecx
	xor	eax, edx
	mov	edx, DWORD PTR _h$[ebp]
	and	edx, DWORD PTR _a$[ebp]
	mov	ecx, DWORD PTR _h$[ebp]
	or	ecx, DWORD PTR _a$[ebp]
	and	ecx, DWORD PTR _g$[ebp]
	or	edx, ecx
	add	eax, edx
	mov	DWORD PTR _T2$1143[ebp], eax
	mov	edx, DWORD PTR _b$[ebp]
	add	edx, DWORD PTR _T1$1142[ebp]
	mov	DWORD PTR _b$[ebp], edx
	mov	eax, DWORD PTR _T1$1142[ebp]
	add	eax, DWORD PTR _T2$1143[ebp]
	mov	DWORD PTR _f$[ebp], eax
	xor	ecx, ecx
	test	ecx, ecx
	jne	$L1124
$L1141:

; 102  : 		SHA2_STEP(f, g, h, a, b, c, d, e, i + 3);

	mov	edx, DWORD PTR _b$[ebp]
	shl	edx, 26					; 0000001aH
	mov	eax, DWORD PTR _b$[ebp]
	shr	eax, 6
	or	edx, eax
	mov	ecx, DWORD PTR _b$[ebp]
	shl	ecx, 21					; 00000015H
	mov	eax, DWORD PTR _b$[ebp]
	shr	eax, 11					; 0000000bH
	or	ecx, eax
	xor	edx, ecx
	mov	ecx, DWORD PTR _b$[ebp]
	shl	ecx, 7
	mov	eax, DWORD PTR _b$[ebp]
	shr	eax, 25					; 00000019H
	or	ecx, eax
	xor	edx, ecx
	mov	ecx, DWORD PTR _e$[ebp]
	add	ecx, edx
	mov	edx, DWORD PTR _c$[ebp]
	xor	edx, DWORD PTR _d$[ebp]
	and	edx, DWORD PTR _b$[ebp]
	xor	edx, DWORD PTR _d$[ebp]
	add	ecx, edx
	mov	eax, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _K[eax*4+12]
	mov	edx, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _w$[ebp+edx*4+12]
	mov	DWORD PTR _T1$1159[ebp], ecx
	mov	eax, DWORD PTR _f$[ebp]
	shl	eax, 30					; 0000001eH
	mov	ecx, DWORD PTR _f$[ebp]
	shr	ecx, 2
	or	eax, ecx
	mov	edx, DWORD PTR _f$[ebp]
	shl	edx, 19					; 00000013H
	mov	ecx, DWORD PTR _f$[ebp]
	shr	ecx, 13					; 0000000dH
	or	edx, ecx
	xor	eax, edx
	mov	edx, DWORD PTR _f$[ebp]
	shl	edx, 10					; 0000000aH
	mov	ecx, DWORD PTR _f$[ebp]
	shr	ecx, 22					; 00000016H
	or	edx, ecx
	xor	eax, edx
	mov	edx, DWORD PTR _g$[ebp]
	and	edx, DWORD PTR _h$[ebp]
	mov	ecx, DWORD PTR _g$[ebp]
	or	ecx, DWORD PTR _h$[ebp]
	and	ecx, DWORD PTR _f$[ebp]
	or	edx, ecx
	add	eax, edx
	mov	DWORD PTR _T2$1160[ebp], eax
	mov	edx, DWORD PTR _a$[ebp]
	add	edx, DWORD PTR _T1$1159[ebp]
	mov	DWORD PTR _a$[ebp], edx
	mov	eax, DWORD PTR _T1$1159[ebp]
	add	eax, DWORD PTR _T2$1160[ebp]
	mov	DWORD PTR _e$[ebp], eax
	xor	ecx, ecx
	test	ecx, ecx
	jne	$L1141
$L1158:

; 103  : 		SHA2_STEP(e, f, g, h, a, b, c, d, i + 4);

	mov	edx, DWORD PTR _a$[ebp]
	shl	edx, 26					; 0000001aH
	mov	eax, DWORD PTR _a$[ebp]
	shr	eax, 6
	or	edx, eax
	mov	ecx, DWORD PTR _a$[ebp]
	shl	ecx, 21					; 00000015H
	mov	eax, DWORD PTR _a$[ebp]
	shr	eax, 11					; 0000000bH
	or	ecx, eax
	xor	edx, ecx
	mov	ecx, DWORD PTR _a$[ebp]
	shl	ecx, 7
	mov	eax, DWORD PTR _a$[ebp]
	shr	eax, 25					; 00000019H
	or	ecx, eax
	xor	edx, ecx
	mov	ecx, DWORD PTR _d$[ebp]
	add	ecx, edx
	mov	edx, DWORD PTR _b$[ebp]
	xor	edx, DWORD PTR _c$[ebp]
	and	edx, DWORD PTR _a$[ebp]
	xor	edx, DWORD PTR _c$[ebp]
	add	ecx, edx
	mov	eax, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _K[eax*4+16]
	mov	edx, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _w$[ebp+edx*4+16]
	mov	DWORD PTR _T1$1176[ebp], ecx
	mov	eax, DWORD PTR _e$[ebp]
	shl	eax, 30					; 0000001eH
	mov	ecx, DWORD PTR _e$[ebp]
	shr	ecx, 2
	or	eax, ecx
	mov	edx, DWORD PTR _e$[ebp]
	shl	edx, 19					; 00000013H
	mov	ecx, DWORD PTR _e$[ebp]
	shr	ecx, 13					; 0000000dH
	or	edx, ecx
	xor	eax, edx
	mov	edx, DWORD PTR _e$[ebp]
	shl	edx, 10					; 0000000aH
	mov	ecx, DWORD PTR _e$[ebp]
	shr	ecx, 22					; 00000016H
	or	edx, ecx
	xor	eax, edx
	mov	edx, DWORD PTR _f$[ebp]
	and	edx, DWORD PTR _g$[ebp]
	mov	ecx, DWORD PTR _f$[ebp]
	or	ecx, DWORD PTR _g$[ebp]
	and	ecx, DWORD PTR _e$[ebp]
	or	edx, ecx
	add	eax, edx
	mov	DWORD PTR _T2$1177[ebp], eax
	mov	edx, DWORD PTR _h$[ebp]
	add	edx, DWORD PTR _T1$1176[ebp]
	mov	DWORD PTR _h$[ebp], edx
	mov	eax, DWORD PTR _T1$1176[ebp]
	add	eax, DWORD PTR _T2$1177[ebp]
	mov	DWORD PTR _d$[ebp], eax
	xor	ecx, ecx
	test	ecx, ecx
	jne	$L1158
$L1175:

; 104  : 		SHA2_STEP(d, e, f, g, h, a, b, c, i + 5);

	mov	edx, DWORD PTR _h$[ebp]
	shl	edx, 26					; 0000001aH
	mov	eax, DWORD PTR _h$[ebp]
	shr	eax, 6
	or	edx, eax
	mov	ecx, DWORD PTR _h$[ebp]
	shl	ecx, 21					; 00000015H
	mov	eax, DWORD PTR _h$[ebp]
	shr	eax, 11					; 0000000bH
	or	ecx, eax
	xor	edx, ecx
	mov	ecx, DWORD PTR _h$[ebp]
	shl	ecx, 7
	mov	eax, DWORD PTR _h$[ebp]
	shr	eax, 25					; 00000019H
	or	ecx, eax
	xor	edx, ecx
	mov	ecx, DWORD PTR _c$[ebp]
	add	ecx, edx
	mov	edx, DWORD PTR _a$[ebp]
	xor	edx, DWORD PTR _b$[ebp]
	and	edx, DWORD PTR _h$[ebp]
	xor	edx, DWORD PTR _b$[ebp]
	add	ecx, edx
	mov	eax, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _K[eax*4+20]
	mov	edx, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _w$[ebp+edx*4+20]
	mov	DWORD PTR _T1$1193[ebp], ecx
	mov	eax, DWORD PTR _d$[ebp]
	shl	eax, 30					; 0000001eH
	mov	ecx, DWORD PTR _d$[ebp]
	shr	ecx, 2
	or	eax, ecx
	mov	edx, DWORD PTR _d$[ebp]
	shl	edx, 19					; 00000013H
	mov	ecx, DWORD PTR _d$[ebp]
	shr	ecx, 13					; 0000000dH
	or	edx, ecx
	xor	eax, edx
	mov	edx, DWORD PTR _d$[ebp]
	shl	edx, 10					; 0000000aH
	mov	ecx, DWORD PTR _d$[ebp]
	shr	ecx, 22					; 00000016H
	or	edx, ecx
	xor	eax, edx
	mov	edx, DWORD PTR _e$[ebp]
	and	edx, DWORD PTR _f$[ebp]
	mov	ecx, DWORD PTR _e$[ebp]
	or	ecx, DWORD PTR _f$[ebp]
	and	ecx, DWORD PTR _d$[ebp]
	or	edx, ecx
	add	eax, edx
	mov	DWORD PTR _T2$1194[ebp], eax
	mov	edx, DWORD PTR _g$[ebp]
	add	edx, DWORD PTR _T1$1193[ebp]
	mov	DWORD PTR _g$[ebp], edx
	mov	eax, DWORD PTR _T1$1193[ebp]
	add	eax, DWORD PTR _T2$1194[ebp]
	mov	DWORD PTR _c$[ebp], eax
	xor	ecx, ecx
	test	ecx, ecx
	jne	$L1175
$L1192:

; 105  : 		SHA2_STEP(c, d, e, f, g, h, a, b, i + 6);

	mov	edx, DWORD PTR _g$[ebp]
	shl	edx, 26					; 0000001aH
	mov	eax, DWORD PTR _g$[ebp]
	shr	eax, 6
	or	edx, eax
	mov	ecx, DWORD PTR _g$[ebp]
	shl	ecx, 21					; 00000015H
	mov	eax, DWORD PTR _g$[ebp]
	shr	eax, 11					; 0000000bH
	or	ecx, eax
	xor	edx, ecx
	mov	ecx, DWORD PTR _g$[ebp]
	shl	ecx, 7
	mov	eax, DWORD PTR _g$[ebp]
	shr	eax, 25					; 00000019H
	or	ecx, eax
	xor	edx, ecx
	mov	ecx, DWORD PTR _b$[ebp]
	add	ecx, edx
	mov	edx, DWORD PTR _h$[ebp]
	xor	edx, DWORD PTR _a$[ebp]
	and	edx, DWORD PTR _g$[ebp]
	xor	edx, DWORD PTR _a$[ebp]
	add	ecx, edx
	mov	eax, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _K[eax*4+24]
	mov	edx, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _w$[ebp+edx*4+24]
	mov	DWORD PTR _T1$1210[ebp], ecx
	mov	eax, DWORD PTR _c$[ebp]
	shl	eax, 30					; 0000001eH
	mov	ecx, DWORD PTR _c$[ebp]
	shr	ecx, 2
	or	eax, ecx
	mov	edx, DWORD PTR _c$[ebp]
	shl	edx, 19					; 00000013H
	mov	ecx, DWORD PTR _c$[ebp]
	shr	ecx, 13					; 0000000dH
	or	edx, ecx
	xor	eax, edx
	mov	edx, DWORD PTR _c$[ebp]
	shl	edx, 10					; 0000000aH
	mov	ecx, DWORD PTR _c$[ebp]
	shr	ecx, 22					; 00000016H
	or	edx, ecx
	xor	eax, edx
	mov	edx, DWORD PTR _d$[ebp]
	and	edx, DWORD PTR _e$[ebp]
	mov	ecx, DWORD PTR _d$[ebp]
	or	ecx, DWORD PTR _e$[ebp]
	and	ecx, DWORD PTR _c$[ebp]
	or	edx, ecx
	add	eax, edx
	mov	DWORD PTR _T2$1211[ebp], eax
	mov	edx, DWORD PTR _f$[ebp]
	add	edx, DWORD PTR _T1$1210[ebp]
	mov	DWORD PTR _f$[ebp], edx
	mov	eax, DWORD PTR _T1$1210[ebp]
	add	eax, DWORD PTR _T2$1211[ebp]
	mov	DWORD PTR _b$[ebp], eax
	xor	ecx, ecx
	test	ecx, ecx
	jne	$L1192
$L1209:

; 106  : 		SHA2_STEP(b, c, d, e, f, g, h, a, i + 7);

	mov	edx, DWORD PTR _f$[ebp]
	shl	edx, 26					; 0000001aH
	mov	eax, DWORD PTR _f$[ebp]
	shr	eax, 6
	or	edx, eax
	mov	ecx, DWORD PTR _f$[ebp]
	shl	ecx, 21					; 00000015H
	mov	eax, DWORD PTR _f$[ebp]
	shr	eax, 11					; 0000000bH
	or	ecx, eax
	xor	edx, ecx
	mov	ecx, DWORD PTR _f$[ebp]
	shl	ecx, 7
	mov	eax, DWORD PTR _f$[ebp]
	shr	eax, 25					; 00000019H
	or	ecx, eax
	xor	edx, ecx
	mov	ecx, DWORD PTR _a$[ebp]
	add	ecx, edx
	mov	edx, DWORD PTR _g$[ebp]
	xor	edx, DWORD PTR _h$[ebp]
	and	edx, DWORD PTR _f$[ebp]
	xor	edx, DWORD PTR _h$[ebp]
	add	ecx, edx
	mov	eax, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _K[eax*4+28]
	mov	edx, DWORD PTR _i$[ebp]
	add	ecx, DWORD PTR _w$[ebp+edx*4+28]
	mov	DWORD PTR _T1$1227[ebp], ecx
	mov	eax, DWORD PTR _b$[ebp]
	shl	eax, 30					; 0000001eH
	mov	ecx, DWORD PTR _b$[ebp]
	shr	ecx, 2
	or	eax, ecx
	mov	edx, DWORD PTR _b$[ebp]
	shl	edx, 19					; 00000013H
	mov	ecx, DWORD PTR _b$[ebp]
	shr	ecx, 13					; 0000000dH
	or	edx, ecx
	xor	eax, edx
	mov	edx, DWORD PTR _b$[ebp]
	shl	edx, 10					; 0000000aH
	mov	ecx, DWORD PTR _b$[ebp]
	shr	ecx, 22					; 00000016H
	or	edx, ecx
	xor	eax, edx
	mov	edx, DWORD PTR _c$[ebp]
	and	edx, DWORD PTR _d$[ebp]
	mov	ecx, DWORD PTR _c$[ebp]
	or	ecx, DWORD PTR _d$[ebp]
	and	ecx, DWORD PTR _b$[ebp]
	or	edx, ecx
	add	eax, edx
	mov	DWORD PTR _T2$1228[ebp], eax
	mov	edx, DWORD PTR _e$[ebp]
	add	edx, DWORD PTR _T1$1227[ebp]
	mov	DWORD PTR _e$[ebp], edx
	mov	eax, DWORD PTR _T1$1227[ebp]
	add	eax, DWORD PTR _T2$1228[ebp]
	mov	DWORD PTR _a$[ebp], eax
	xor	ecx, ecx
	test	ecx, ecx
	jne	$L1209

; 107  : 	}

	jmp	$L1103
$L1104:

; 108  : 	val[0] += a;

	mov	edx, DWORD PTR _val$[ebp]
	mov	eax, DWORD PTR [edx]
	add	eax, DWORD PTR _a$[ebp]
	mov	ecx, DWORD PTR _val$[ebp]
	mov	DWORD PTR [ecx], eax

; 109  : 	val[1] += b;

	mov	edx, DWORD PTR _val$[ebp]
	mov	eax, DWORD PTR [edx+4]
	add	eax, DWORD PTR _b$[ebp]
	mov	ecx, DWORD PTR _val$[ebp]
	mov	DWORD PTR [ecx+4], eax

; 110  : 	val[2] += c;

	mov	edx, DWORD PTR _val$[ebp]
	mov	eax, DWORD PTR [edx+8]
	add	eax, DWORD PTR _c$[ebp]
	mov	ecx, DWORD PTR _val$[ebp]
	mov	DWORD PTR [ecx+8], eax

; 111  : 	val[3] += d;

	mov	edx, DWORD PTR _val$[ebp]
	mov	eax, DWORD PTR [edx+12]
	add	eax, DWORD PTR _d$[ebp]
	mov	ecx, DWORD PTR _val$[ebp]
	mov	DWORD PTR [ecx+12], eax

; 112  : 	val[4] += e;

	mov	edx, DWORD PTR _val$[ebp]
	mov	eax, DWORD PTR [edx+16]
	add	eax, DWORD PTR _e$[ebp]
	mov	ecx, DWORD PTR _val$[ebp]
	mov	DWORD PTR [ecx+16], eax

; 113  : 	val[5] += f;

	mov	edx, DWORD PTR _val$[ebp]
	mov	eax, DWORD PTR [edx+20]
	add	eax, DWORD PTR _f$[ebp]
	mov	ecx, DWORD PTR _val$[ebp]
	mov	DWORD PTR [ecx+20], eax

; 114  : 	val[6] += g;

	mov	edx, DWORD PTR _val$[ebp]
	mov	eax, DWORD PTR [edx+24]
	add	eax, DWORD PTR _g$[ebp]
	mov	ecx, DWORD PTR _val$[ebp]
	mov	DWORD PTR [ecx+24], eax

; 115  : 	val[7] += h;

	mov	edx, DWORD PTR _val$[ebp]
	mov	eax, DWORD PTR [edx+28]
	add	eax, DWORD PTR _h$[ebp]
	mov	ecx, DWORD PTR _val$[ebp]
	mov	DWORD PTR [ecx+28], eax

; 116  : 
; 117  : #if 0
; 118  : /* obsolete */
; 119  : #define SHA2_MEXP1(pc)   do { \
; 120  : 		W[pc] = br_dec32be(buf + ((pc) << 2)); \
; 121  : 	} while (0)
; 122  : 
; 123  : #define SHA2_MEXP2(pc)   do { \
; 124  : 		W[(pc) & 0x0F] = SSG2_1(W[((pc) - 2) & 0x0F]) \
; 125  : 			+ W[((pc) - 7) & 0x0F] \
; 126  : 			+ SSG2_0(W[((pc) - 15) & 0x0F]) + W[(pc) & 0x0F]; \
; 127  : 	} while (0)
; 128  : 
; 129  : #define SHA2_STEPn(n, a, b, c, d, e, f, g, h, pc)   do { \
; 130  : 		uint32_t t1, t2; \
; 131  : 		SHA2_MEXP ## n(pc); \
; 132  : 		t1 = h + BSG2_1(e) + CH(e, f, g) \
; 133  : 			+ K[pcount + (pc)] + W[(pc) & 0x0F]; \
; 134  : 		t2 = BSG2_0(a) + MAJ(a, b, c); \
; 135  : 		d += t1; \
; 136  : 		h = t1 + t2; \
; 137  : 	} while (0)
; 138  : 
; 139  : #define SHA2_STEP1(a, b, c, d, e, f, g, h, pc) \
; 140  : 	SHA2_STEPn(1, a, b, c, d, e, f, g, h, pc)
; 141  : #define SHA2_STEP2(a, b, c, d, e, f, g, h, pc) \
; 142  : 	SHA2_STEPn(2, a, b, c, d, e, f, g, h, pc)
; 143  : 
; 144  : 	uint32_t A, B, C, D, E, F, G, H;
; 145  : 	uint32_t W[16];
; 146  : 	unsigned pcount;
; 147  : 
; 148  : 	A = val[0];
; 149  : 	B = val[1];
; 150  : 	C = val[2];
; 151  : 	D = val[3];
; 152  : 	E = val[4];
; 153  : 	F = val[5];
; 154  : 	G = val[6];
; 155  : 	H = val[7];
; 156  : 	pcount = 0;
; 157  : 	SHA2_STEP1(A, B, C, D, E, F, G, H,  0);
; 158  : 	SHA2_STEP1(H, A, B, C, D, E, F, G,  1);
; 159  : 	SHA2_STEP1(G, H, A, B, C, D, E, F,  2);
; 160  : 	SHA2_STEP1(F, G, H, A, B, C, D, E,  3);
; 161  : 	SHA2_STEP1(E, F, G, H, A, B, C, D,  4);
; 162  : 	SHA2_STEP1(D, E, F, G, H, A, B, C,  5);
; 163  : 	SHA2_STEP1(C, D, E, F, G, H, A, B,  6);
; 164  : 	SHA2_STEP1(B, C, D, E, F, G, H, A,  7);
; 165  : 	SHA2_STEP1(A, B, C, D, E, F, G, H,  8);
; 166  : 	SHA2_STEP1(H, A, B, C, D, E, F, G,  9);
; 167  : 	SHA2_STEP1(G, H, A, B, C, D, E, F, 10);
; 168  : 	SHA2_STEP1(F, G, H, A, B, C, D, E, 11);
; 169  : 	SHA2_STEP1(E, F, G, H, A, B, C, D, 12);
; 170  : 	SHA2_STEP1(D, E, F, G, H, A, B, C, 13);
; 171  : 	SHA2_STEP1(C, D, E, F, G, H, A, B, 14);
; 172  : 	SHA2_STEP1(B, C, D, E, F, G, H, A, 15);
; 173  : 	for (pcount = 16; pcount < 64; pcount += 16) {
; 174  : 		SHA2_STEP2(A, B, C, D, E, F, G, H,  0);
; 175  : 		SHA2_STEP2(H, A, B, C, D, E, F, G,  1);
; 176  : 		SHA2_STEP2(G, H, A, B, C, D, E, F,  2);
; 177  : 		SHA2_STEP2(F, G, H, A, B, C, D, E,  3);
; 178  : 		SHA2_STEP2(E, F, G, H, A, B, C, D,  4);
; 179  : 		SHA2_STEP2(D, E, F, G, H, A, B, C,  5);
; 180  : 		SHA2_STEP2(C, D, E, F, G, H, A, B,  6);
; 181  : 		SHA2_STEP2(B, C, D, E, F, G, H, A,  7);
; 182  : 		SHA2_STEP2(A, B, C, D, E, F, G, H,  8);
; 183  : 		SHA2_STEP2(H, A, B, C, D, E, F, G,  9);
; 184  : 		SHA2_STEP2(G, H, A, B, C, D, E, F, 10);
; 185  : 		SHA2_STEP2(F, G, H, A, B, C, D, E, 11);
; 186  : 		SHA2_STEP2(E, F, G, H, A, B, C, D, 12);
; 187  : 		SHA2_STEP2(D, E, F, G, H, A, B, C, 13);
; 188  : 		SHA2_STEP2(C, D, E, F, G, H, A, B, 14);
; 189  : 		SHA2_STEP2(B, C, D, E, F, G, H, A, 15);
; 190  : 	}
; 191  : 	val[0] += A;
; 192  : 	val[1] += B;
; 193  : 	val[2] += C;
; 194  : 	val[3] += D;
; 195  : 	val[4] += E;
; 196  : 	val[5] += F;
; 197  : 	val[6] += G;
; 198  : 	val[7] += H;
; 199  : #endif
; 200  : }

	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	0
_br_sha2small_round ENDP
_TEXT	ENDS
EXTRN	_pacc_hash_memcpy:NEAR
_TEXT	SEGMENT
_cc$ = 8
_br_sha224_init PROC NEAR

; 255  : {

	push	ebp
	mov	ebp, esp

; 256  : 	cc->vtable = &br_sha224_vtable;

	mov	eax, DWORD PTR _cc$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:_br_sha224_vtable

; 257  : 	memcpy(cc->val, br_sha224_IV, sizeof cc->val);

	push	32					; 00000020H
	push	OFFSET FLAT:_br_sha224_IV
	mov	ecx, DWORD PTR _cc$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	call	_pacc_hash_memcpy
	add	esp, 12					; 0000000cH

; 258  : 	cc->count = 0;

	mov	edx, DWORD PTR _cc$[ebp]
	mov	DWORD PTR [edx+72], 0
	mov	DWORD PTR [edx+76], 0

; 259  : }

	pop	ebp
	ret	0
_br_sha224_init ENDP
_cc$ = 8
_data$ = 12
_len$ = 16
_br_sha224_update PROC NEAR

; 264  : {

	push	ebp
	mov	ebp, esp

; 265  : 	sha2small_update(cc, data, len);

	mov	eax, DWORD PTR _len$[ebp]
	push	eax
	mov	ecx, DWORD PTR _data$[ebp]
	push	ecx
	mov	edx, DWORD PTR _cc$[ebp]
	push	edx
	call	_sha2small_update
	add	esp, 12					; 0000000cH

; 266  : }

	pop	ebp
	ret	0
_br_sha224_update ENDP
_cc$ = 8
_data$ = 12
_len$ = 16
_buf$ = -8
_ptr$ = -4
_clen$1256 = -12
_sha2small_update PROC NEAR

; 204  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	esi

; 205  : 	const unsigned char *buf;
; 206  : 	size_t ptr;
; 207  : 
; 208  : 	buf = data;

	mov	eax, DWORD PTR _data$[ebp]
	mov	DWORD PTR _buf$[ebp], eax

; 209  : 	ptr = (size_t)cc->count & 63;

	mov	ecx, DWORD PTR _cc$[ebp]
	mov	edx, DWORD PTR [ecx+72]
	and	edx, 63					; 0000003fH
	mov	DWORD PTR _ptr$[ebp], edx

; 210  : 	cc->count += (uint64_t)len;

	mov	eax, DWORD PTR _cc$[ebp]
	mov	ecx, DWORD PTR _len$[ebp]
	xor	edx, edx
	mov	esi, DWORD PTR [eax+72]
	add	esi, ecx
	mov	eax, DWORD PTR [eax+76]
	adc	eax, edx
	mov	ecx, DWORD PTR _cc$[ebp]
	mov	DWORD PTR [ecx+72], esi
	mov	DWORD PTR [ecx+76], eax
$L1254:

; 211  : 	while (len > 0) {

	cmp	DWORD PTR _len$[ebp], 0
	jbe	SHORT $L1255

; 212  : 		size_t clen;
; 213  : 
; 214  : 		clen = 64 - ptr;

	mov	edx, 64					; 00000040H
	sub	edx, DWORD PTR _ptr$[ebp]
	mov	DWORD PTR _clen$1256[ebp], edx

; 215  : 		if (clen > len) {

	mov	eax, DWORD PTR _clen$1256[ebp]
	cmp	eax, DWORD PTR _len$[ebp]
	jbe	SHORT $L1257

; 216  : 			clen = len;

	mov	ecx, DWORD PTR _len$[ebp]
	mov	DWORD PTR _clen$1256[ebp], ecx
$L1257:

; 218  : 		memcpy(cc->buf + ptr, buf, clen);

	mov	edx, DWORD PTR _clen$1256[ebp]
	push	edx
	mov	eax, DWORD PTR _buf$[ebp]
	push	eax
	mov	ecx, DWORD PTR _ptr$[ebp]
	mov	edx, DWORD PTR _cc$[ebp]
	lea	eax, DWORD PTR [edx+ecx+4]
	push	eax
	call	_pacc_hash_memcpy
	add	esp, 12					; 0000000cH

; 219  : 		ptr += clen;

	mov	ecx, DWORD PTR _ptr$[ebp]
	add	ecx, DWORD PTR _clen$1256[ebp]
	mov	DWORD PTR _ptr$[ebp], ecx

; 220  : 		buf += clen;

	mov	edx, DWORD PTR _buf$[ebp]
	add	edx, DWORD PTR _clen$1256[ebp]
	mov	DWORD PTR _buf$[ebp], edx

; 221  : 		len -= clen;

	mov	eax, DWORD PTR _len$[ebp]
	sub	eax, DWORD PTR _clen$1256[ebp]
	mov	DWORD PTR _len$[ebp], eax

; 222  : 		if (ptr == 64) {

	cmp	DWORD PTR _ptr$[ebp], 64		; 00000040H
	jne	SHORT $L1258

; 223  : 			br_sha2small_round(cc->buf, cc->val);

	mov	ecx, DWORD PTR _cc$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	mov	edx, DWORD PTR _cc$[ebp]
	add	edx, 4
	push	edx
	call	_br_sha2small_round
	add	esp, 8

; 224  : 			ptr = 0;

	mov	DWORD PTR _ptr$[ebp], 0
$L1258:

; 226  : 	}

	jmp	SHORT $L1254
$L1255:

; 227  : }

	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	0
_sha2small_update ENDP
_cc$ = 8
_dst$ = 12
_br_sha224_out PROC NEAR

; 271  : {

	push	ebp
	mov	ebp, esp

; 272  : 	sha2small_out(cc, dst, 7);

	push	7
	mov	eax, DWORD PTR _dst$[ebp]
	push	eax
	mov	ecx, DWORD PTR _cc$[ebp]
	push	ecx
	call	_sha2small_out
	add	esp, 12					; 0000000cH

; 273  : }

	pop	ebp
	ret	0
_br_sha224_out ENDP
_TEXT	ENDS
EXTRN	_br_range_enc32be:NEAR
EXTRN	__allshl:NEAR
EXTRN	_pacc_hash_memset:NEAR
_TEXT	SEGMENT
_cc$ = 8
_dst$ = 12
_num$ = 16
_buf$ = -100
_val$ = -32
_ptr$ = -36
_sha2small_out PROC NEAR

; 231  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 100				; 00000064H

; 232  : 	unsigned char buf[64];
; 233  : 	uint32_t val[8];
; 234  : 	size_t ptr;
; 235  : 
; 236  : 	ptr = (size_t)cc->count & 63;

	mov	eax, DWORD PTR _cc$[ebp]
	mov	ecx, DWORD PTR [eax+72]
	and	ecx, 63					; 0000003fH
	mov	DWORD PTR _ptr$[ebp], ecx

; 237  : 	memcpy(buf, cc->buf, ptr);

	mov	edx, DWORD PTR _ptr$[ebp]
	push	edx
	mov	eax, DWORD PTR _cc$[ebp]
	add	eax, 4
	push	eax
	lea	ecx, DWORD PTR _buf$[ebp]
	push	ecx
	call	_pacc_hash_memcpy
	add	esp, 12					; 0000000cH

; 238  : 	memcpy(val, cc->val, sizeof val);

	push	32					; 00000020H
	mov	edx, DWORD PTR _cc$[ebp]
	add	edx, 80					; 00000050H
	push	edx
	lea	eax, DWORD PTR _val$[ebp]
	push	eax
	call	_pacc_hash_memcpy
	add	esp, 12					; 0000000cH

; 239  : 	buf[ptr ++] = 0x80;

	mov	ecx, DWORD PTR _ptr$[ebp]
	mov	BYTE PTR _buf$[ebp+ecx], 128		; 00000080H
	mov	edx, DWORD PTR _ptr$[ebp]
	add	edx, 1
	mov	DWORD PTR _ptr$[ebp], edx

; 240  : 	if (ptr > 56) {

	cmp	DWORD PTR _ptr$[ebp], 56		; 00000038H
	jbe	SHORT $L1271

; 241  : 		memset(buf + ptr, 0, 64 - ptr);

	mov	eax, 64					; 00000040H
	sub	eax, DWORD PTR _ptr$[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR _ptr$[ebp]
	lea	edx, DWORD PTR _buf$[ebp+ecx]
	push	edx
	call	_pacc_hash_memset
	add	esp, 12					; 0000000cH

; 242  : 		br_sha2small_round(buf, val);

	lea	eax, DWORD PTR _val$[ebp]
	push	eax
	lea	ecx, DWORD PTR _buf$[ebp]
	push	ecx
	call	_br_sha2small_round
	add	esp, 8

; 243  : 		memset(buf, 0, 56);

	push	56					; 00000038H
	push	0
	lea	edx, DWORD PTR _buf$[ebp]
	push	edx
	call	_pacc_hash_memset
	add	esp, 12					; 0000000cH

; 244  : 	} else {

	jmp	SHORT $L1272
$L1271:

; 245  : 		memset(buf + ptr, 0, 56 - ptr);

	mov	eax, 56					; 00000038H
	sub	eax, DWORD PTR _ptr$[ebp]
	push	eax
	push	0
	mov	ecx, DWORD PTR _ptr$[ebp]
	lea	edx, DWORD PTR _buf$[ebp+ecx]
	push	edx
	call	_pacc_hash_memset
	add	esp, 12					; 0000000cH
$L1272:

; 247  : 	br_enc64be(buf + 56, cc->count << 3);

	mov	edx, DWORD PTR _cc$[ebp]
	mov	ecx, 3
	mov	eax, DWORD PTR [edx+72]
	mov	edx, DWORD PTR [edx+76]
	call	__allshl
	push	edx
	push	eax
	lea	eax, DWORD PTR _buf$[ebp+56]
	push	eax
	call	_br_enc64be
	add	esp, 12					; 0000000cH

; 248  : 	br_sha2small_round(buf, val);

	lea	ecx, DWORD PTR _val$[ebp]
	push	ecx
	lea	edx, DWORD PTR _buf$[ebp]
	push	edx
	call	_br_sha2small_round
	add	esp, 8

; 249  : 	br_range_enc32be(dst, val, num);

	mov	eax, DWORD PTR _num$[ebp]
	push	eax
	lea	ecx, DWORD PTR _val$[ebp]
	push	ecx
	mov	edx, DWORD PTR _dst$[ebp]
	push	edx
	call	_br_range_enc32be
	add	esp, 12					; 0000000cH

; 250  : }

	mov	esp, ebp
	pop	ebp
	ret	0
_sha2small_out ENDP
_TEXT	ENDS
EXTRN	__aullshr:NEAR
_TEXT	SEGMENT
_dst$ = 8
_x$ = 12
_buf$ = -4
_br_enc64be PROC NEAR

; 66   : {

	push	ebp
	mov	ebp, esp
	push	ecx

; 67   : #if BR_BE_UNALIGNED
; 68   : 	((br_union_u64 *)dst)->u = x;
; 69   : #else
; 70   : 	unsigned char *buf;
; 71   : 
; 72   : 	buf = dst;

	mov	eax, DWORD PTR _dst$[ebp]
	mov	DWORD PTR _buf$[ebp], eax

; 73   : 	br_enc32be(buf, (uint32_t)(x >> 32));

	mov	ecx, 32					; 00000020H
	mov	eax, DWORD PTR _x$[ebp]
	mov	edx, DWORD PTR _x$[ebp+4]
	call	__aullshr
	push	eax
	mov	ecx, DWORD PTR _buf$[ebp]
	push	ecx
	call	_br_enc32be
	add	esp, 8

; 74   : 	br_enc32be(buf + 4, (uint32_t)x);

	mov	edx, DWORD PTR _x$[ebp]
	push	edx
	mov	eax, DWORD PTR _buf$[ebp]
	add	eax, 4
	push	eax
	call	_br_enc32be
	add	esp, 8

; 75   : #endif
; 76   : }

	mov	esp, ebp
	pop	ebp
	ret	0
_br_enc64be ENDP
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
_cc$ = 8
_dst$ = 12
_br_sha224_state PROC NEAR

; 278  : {

	push	ebp
	mov	ebp, esp

; 279  : 	br_range_enc32be(dst, cc->val, 8);

	push	8
	mov	eax, DWORD PTR _cc$[ebp]
	add	eax, 80					; 00000050H
	push	eax
	mov	ecx, DWORD PTR _dst$[ebp]
	push	ecx
	call	_br_range_enc32be
	add	esp, 12					; 0000000cH

; 280  : 	return cc->count;

	mov	edx, DWORD PTR _cc$[ebp]
	mov	eax, DWORD PTR [edx+72]
	mov	edx, DWORD PTR [edx+76]

; 281  : }

	pop	ebp
	ret	0
_br_sha224_state ENDP
_cc$ = 8
_stb$ = 12
_count$ = 16
_br_sha224_set_state PROC NEAR

; 286  : {

	push	ebp
	mov	ebp, esp

; 287  : 	br_range_dec32be(cc->val, 8, stb);

	mov	eax, DWORD PTR _stb$[ebp]
	push	eax
	push	8
	mov	ecx, DWORD PTR _cc$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	call	_br_range_dec32be
	add	esp, 12					; 0000000cH

; 288  : 	cc->count = count;

	mov	edx, DWORD PTR _cc$[ebp]
	mov	eax, DWORD PTR _count$[ebp]
	mov	DWORD PTR [edx+72], eax
	mov	ecx, DWORD PTR _count$[ebp+4]
	mov	DWORD PTR [edx+76], ecx

; 289  : }

	pop	ebp
	ret	0
_br_sha224_set_state ENDP
_cc$ = 8
_br_sha256_init PROC NEAR

; 294  : {

	push	ebp
	mov	ebp, esp

; 295  : 	cc->vtable = &br_sha256_vtable;

	mov	eax, DWORD PTR _cc$[ebp]
	mov	DWORD PTR [eax], OFFSET FLAT:_br_sha256_vtable

; 296  : 	memcpy(cc->val, br_sha256_IV, sizeof cc->val);

	push	32					; 00000020H
	push	OFFSET FLAT:_br_sha256_IV
	mov	ecx, DWORD PTR _cc$[ebp]
	add	ecx, 80					; 00000050H
	push	ecx
	call	_pacc_hash_memcpy
	add	esp, 12					; 0000000cH

; 297  : 	cc->count = 0;

	mov	edx, DWORD PTR _cc$[ebp]
	mov	DWORD PTR [edx+72], 0
	mov	DWORD PTR [edx+76], 0

; 298  : }

	pop	ebp
	ret	0
_br_sha256_init ENDP
_cc$ = 8
_dst$ = 12
_br_sha256_out PROC NEAR

; 303  : {

	push	ebp
	mov	ebp, esp

; 304  : 	sha2small_out(cc, dst, 8);

	push	8
	mov	eax, DWORD PTR _dst$[ebp]
	push	eax
	mov	ecx, DWORD PTR _cc$[ebp]
	push	ecx
	call	_sha2small_out
	add	esp, 12					; 0000000cH

; 305  : }

	pop	ebp
	ret	0
_br_sha256_out ENDP
_TEXT	ENDS
END
