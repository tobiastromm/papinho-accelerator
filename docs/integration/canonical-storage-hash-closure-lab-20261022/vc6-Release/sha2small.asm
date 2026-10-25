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
;	COMDAT _br_enc32be
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _br_enc64be
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _br_sha2small_round
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _sha2small_update
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _sha2small_out
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _br_sha224_init
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _br_sha224_update
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _br_sha224_out
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _br_sha224_state
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _br_sha224_set_state
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _br_sha256_init
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _br_sha256_out
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
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
;	COMDAT _br_sha2small_round
_TEXT	SEGMENT
_buf$ = 8
_val$ = 12
_a$ = -268
_b$ = -280
_e$ = -260
_f$ = -276
_g$ = -284
_w$ = -256
_T1$1133 = -272
_T1$1150 = -272
_T1$1167 = -272
_T1$1184 = -272
_T1$1201 = -272
_T1$1235 = -272
_T1$1252 = -272
_br_sha2small_round PROC NEAR				; COMDAT

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

	mov	eax, DWORD PTR _buf$[esp-4]
	sub	esp, 284				; 0000011cH
	lea	ecx, DWORD PTR _w$[esp+284]
	push	ebx
	push	ebp
	push	esi
	push	edi
	push	eax
	push	16					; 00000010H
	push	ecx
	call	_br_range_dec32be
	add	esp, 12					; 0000000cH
	lea	edx, DWORD PTR _w$[esp+356]
	mov	esi, 48					; 00000030H
$L1114:

; 86   : 	for (i = 16; i < 64; i ++) {
; 87   : 		w[i] = SSG2_1(w[i - 2]) + w[i - 7]
; 88   : 			+ SSG2_0(w[i - 15]) + w[i - 16];

	mov	ecx, DWORD PTR [edx]
	mov	eax, DWORD PTR [edx-52]
	mov	edi, ecx
	mov	ebx, ecx
	shr	edi, 17					; 00000011H
	shl	ebx, 15					; 0000000fH
	or	edi, ebx
	mov	ebx, ecx
	mov	ebp, ecx
	add	edx, 4
	shr	ebx, 19					; 00000013H
	shl	ebp, 13					; 0000000dH
	or	ebx, ebp
	mov	ebp, eax
	xor	edi, ebx
	mov	ebx, eax
	shr	ecx, 10					; 0000000aH
	xor	edi, ecx
	mov	ecx, eax
	shr	ecx, 18					; 00000012H
	shl	ebx, 14					; 0000000eH
	or	ecx, ebx
	mov	ebx, eax
	shl	ebx, 25					; 00000019H
	shr	ebp, 7
	or	ebx, ebp
	mov	ebp, DWORD PTR [edx-60]
	xor	ecx, ebx
	mov	ebx, DWORD PTR [edx-24]
	shr	eax, 3
	xor	ecx, eax
	add	edi, ecx
	add	edi, ebp
	add	edi, ebx
	dec	esi
	mov	DWORD PTR [edx+4], edi
	jne	SHORT $L1114

; 89   : 	}
; 90   : 	a = val[0];

	mov	ecx, DWORD PTR _val$[esp+296]

; 91   : 	b = val[1];
; 92   : 	c = val[2];
; 93   : 	d = val[3];
; 94   : 	e = val[4];
; 95   : 	f = val[5];
; 96   : 	g = val[6];
; 97   : 	h = val[7];
; 98   : 	for (i = 0; i < 64; i += 8) {

	mov	DWORD PTR -264+[esp+300], 0
	mov	edi, DWORD PTR [ecx+20]
	mov	edx, DWORD PTR [ecx]
	mov	eax, DWORD PTR [ecx+4]
	mov	esi, DWORD PTR [ecx+16]
	mov	DWORD PTR _f$[esp+300], edi
	mov	edi, DWORD PTR [ecx+24]
	mov	DWORD PTR _a$[esp+300], edx
	mov	edx, DWORD PTR [ecx+8]
	mov	DWORD PTR _b$[esp+300], eax
	mov	eax, DWORD PTR [ecx+12]
	mov	ecx, DWORD PTR [ecx+28]
	mov	DWORD PTR _e$[esp+300], esi
	mov	DWORD PTR _g$[esp+300], edi
$L1130:

; 99   : 		SHA2_STEP(a, b, c, d, e, f, g, h, i + 0);

	mov	edi, esi
	mov	ebx, esi
	shl	edi, 21					; 00000015H
	shr	ebx, 11					; 0000000bH
	or	edi, ebx
	mov	ebx, esi
	mov	ebp, esi
	shl	ebx, 26					; 0000001aH
	shr	ebp, 6
	or	ebx, ebp
	mov	ebp, esi
	xor	edi, ebx
	mov	ebx, esi
	shr	ebx, 25					; 00000019H
	shl	ebp, 7
	or	ebx, ebp
	xor	edi, ebx
	mov	ebx, DWORD PTR _g$[esp+300]
	mov	ebp, ebx
	xor	ebp, DWORD PTR _f$[esp+300]
	and	ebp, esi
	xor	ebp, ebx
	mov	ebx, DWORD PTR -264+[esp+300]
	add	edi, ebp
	mov	ebp, DWORD PTR _K[ebx]
	add	edi, ebp
	mov	ebp, DWORD PTR _w$[esp+ebx+300]
	add	edi, ebp
	add	edi, ecx
	mov	ecx, DWORD PTR _a$[esp+300]
	mov	DWORD PTR _T1$1133[esp+300], edi
	add	eax, edi
	mov	ebx, ecx
	mov	edi, ecx
	shl	ebx, 19					; 00000013H
	shr	edi, 13					; 0000000dH
	or	ebx, edi
	mov	edi, ecx
	mov	ebp, ecx
	shr	edi, 22					; 00000016H
	shl	ebp, 10					; 0000000aH
	or	edi, ebp
	mov	ebp, ecx

; 100  : 		SHA2_STEP(h, a, b, c, d, e, f, g, i + 1);
; 101  : 		SHA2_STEP(g, h, a, b, c, d, e, f, i + 2);
; 102  : 		SHA2_STEP(f, g, h, a, b, c, d, e, i + 3);
; 103  : 		SHA2_STEP(e, f, g, h, a, b, c, d, i + 4);
; 104  : 		SHA2_STEP(d, e, f, g, h, a, b, c, i + 5);
; 105  : 		SHA2_STEP(c, d, e, f, g, h, a, b, i + 6);
; 106  : 		SHA2_STEP(b, c, d, e, f, g, h, a, i + 7);
; 107  : 	}
; 108  : 	val[0] += a;
; 109  : 	val[1] += b;
; 110  : 	val[2] += c;
; 111  : 	val[3] += d;
; 112  : 	val[4] += e;
; 113  : 	val[5] += f;
; 114  : 	val[6] += g;
; 115  : 	val[7] += h;

	xor	ebx, edi
	mov	edi, ecx
	shl	edi, 30					; 0000001eH
	shr	ebp, 2
	or	edi, ebp
	mov	ebp, edx
	xor	ebx, edi
	mov	edi, DWORD PTR _b$[esp+300]
	or	ebp, edi
	and	ebp, ecx
	mov	ecx, edx
	and	ecx, edi
	mov	edi, eax
	or	ebp, ecx
	mov	ecx, DWORD PTR _T1$1133[esp+300]
	add	ecx, ebx
	mov	ebx, eax
	shl	edi, 21					; 00000015H
	shr	ebx, 11					; 0000000bH
	add	ecx, ebp
	or	edi, ebx
	mov	ebx, eax
	mov	ebp, eax
	shl	ebx, 26					; 0000001aH
	shr	ebp, 6
	or	ebx, ebp
	mov	ebp, eax
	xor	edi, ebx
	mov	ebx, eax
	shr	ebx, 25					; 00000019H
	shl	ebp, 7
	or	ebx, ebp
	xor	edi, ebx
	mov	ebx, DWORD PTR _f$[esp+300]
	mov	ebp, ebx
	xor	ebp, esi
	and	ebp, eax
	xor	ebp, ebx
	mov	esi, DWORD PTR -264+[esp+300]
	add	edi, ebp
	mov	ebx, DWORD PTR _K[esi+4]
	mov	ebp, DWORD PTR _w$[esp+esi+304]
	add	edi, ebx
	mov	ebx, DWORD PTR _g$[esp+300]
	add	edi, ebp
	mov	esi, ecx
	add	edi, ebx
	mov	ebx, ecx
	shl	ebx, 19					; 00000013H
	shr	esi, 13					; 0000000dH
	or	ebx, esi
	mov	DWORD PTR _T1$1150[esp+300], edi
	add	edx, edi
	mov	esi, ecx
	mov	edi, ecx
	shr	esi, 22					; 00000016H
	shl	edi, 10					; 0000000aH
	or	esi, edi
	mov	edi, ecx
	xor	ebx, esi
	mov	esi, ecx
	shl	esi, 30					; 0000001eH
	shr	edi, 2
	or	esi, edi
	mov	edi, DWORD PTR _a$[esp+300]
	xor	ebx, esi
	mov	esi, DWORD PTR _b$[esp+300]
	mov	ebp, esi
	and	esi, edi
	or	ebp, edi
	and	ebp, ecx
	or	ebp, esi
	mov	esi, DWORD PTR _T1$1150[esp+300]
	add	esi, ebx
	mov	ebx, edx
	add	ebp, esi
	mov	esi, edx
	shl	esi, 21					; 00000015H
	shr	ebx, 11					; 0000000bH
	mov	DWORD PTR _g$[esp+300], ebp
	or	esi, ebx
	mov	ebx, edx
	mov	ebp, edx
	shl	ebx, 26					; 0000001aH
	shr	ebp, 6
	or	ebx, ebp
	mov	ebp, edx
	xor	esi, ebx
	mov	ebx, edx
	shr	ebx, 25					; 00000019H
	shl	ebp, 7
	or	ebx, ebp
	xor	esi, ebx
	mov	ebx, DWORD PTR _e$[esp+300]
	mov	ebp, ebx
	xor	ebp, eax
	and	ebp, edx
	xor	ebp, ebx
	mov	ebx, DWORD PTR -264+[esp+300]
	add	esi, ebp
	mov	ebp, DWORD PTR _K[ebx+8]
	add	esi, ebp
	mov	ebp, DWORD PTR _w$[esp+ebx+308]
	mov	ebx, DWORD PTR _b$[esp+300]
	add	esi, ebp
	add	esi, DWORD PTR _f$[esp+300]
	mov	DWORD PTR _T1$1167[esp+300], esi
	add	ebx, esi
	mov	esi, DWORD PTR _g$[esp+300]
	mov	DWORD PTR _b$[esp+300], ebx
	mov	ebx, esi
	mov	ebp, esi
	shl	ebx, 19					; 00000013H
	shr	ebp, 13					; 0000000dH
	or	ebx, ebp
	mov	ebp, esi
	shr	ebp, 22					; 00000016H
	shl	esi, 10					; 0000000aH
	or	ebp, esi
	mov	esi, DWORD PTR _g$[esp+300]
	xor	ebx, ebp
	mov	ebp, esi
	shl	ebp, 30					; 0000001eH
	shr	esi, 2
	or	ebp, esi
	mov	esi, DWORD PTR _g$[esp+300]
	xor	ebx, ebp
	mov	ebp, ecx
	or	ebp, edi
	and	ebp, esi
	mov	esi, ecx
	and	esi, edi
	or	ebp, esi
	mov	esi, DWORD PTR _T1$1167[esp+300]
	add	esi, ebx
	mov	ebx, DWORD PTR _b$[esp+300]
	add	ebp, esi
	mov	esi, ebx
	mov	DWORD PTR _f$[esp+300], ebp
	mov	ebp, ebx
	shl	esi, 21					; 00000015H
	shr	ebp, 11					; 0000000bH
	or	esi, ebp
	mov	ebp, ebx
	shl	ebp, 26					; 0000001aH
	shr	ebx, 6
	or	ebp, ebx
	mov	ebx, DWORD PTR _b$[esp+300]
	xor	esi, ebp
	mov	ebp, ebx
	shr	ebp, 25					; 00000019H
	shl	ebx, 7
	or	ebp, ebx
	mov	ebx, eax
	xor	esi, ebp
	mov	ebp, DWORD PTR _b$[esp+300]
	xor	ebx, edx
	and	ebx, ebp
	xor	ebx, eax
	add	esi, ebx
	mov	ebx, DWORD PTR -264+[esp+300]
	mov	ebp, DWORD PTR _K[ebx+12]
	add	esi, ebp
	mov	ebp, DWORD PTR _w$[esp+ebx+312]
	add	esi, ebp
	mov	ebp, DWORD PTR _e$[esp+300]
	add	esi, ebp
	mov	DWORD PTR _T1$1184[esp+300], esi
	add	edi, esi
	mov	esi, DWORD PTR _f$[esp+300]
	mov	DWORD PTR _a$[esp+300], edi
	mov	ebx, esi
	mov	edi, esi
	shl	ebx, 19					; 00000013H
	shr	edi, 13					; 0000000dH
	or	ebx, edi
	mov	edi, esi
	mov	ebp, esi
	shr	edi, 22					; 00000016H
	shl	ebp, 10					; 0000000aH
	or	edi, ebp
	mov	ebp, esi
	xor	ebx, edi
	mov	edi, esi
	shl	edi, 30					; 0000001eH
	shr	ebp, 2
	or	edi, ebp
	mov	ebp, ecx
	xor	ebx, edi
	mov	edi, DWORD PTR _g$[esp+300]
	or	ebp, edi
	and	ebp, esi
	mov	esi, ecx
	and	esi, edi
	or	ebp, esi
	mov	esi, DWORD PTR _T1$1184[esp+300]
	add	esi, ebx
	mov	ebx, DWORD PTR _a$[esp+300]
	add	esi, ebp
	mov	edi, ebx
	mov	ebp, ebx
	shl	edi, 21					; 00000015H
	shr	ebp, 11					; 0000000bH
	or	edi, ebp
	mov	ebp, ebx
	shl	ebp, 26					; 0000001aH
	shr	ebx, 6
	or	ebp, ebx
	mov	ebx, DWORD PTR _a$[esp+300]
	xor	edi, ebp
	mov	ebp, ebx
	shr	ebp, 25					; 00000019H
	shl	ebx, 7
	or	ebp, ebx
	mov	ebx, edx
	xor	edi, ebp
	mov	ebp, DWORD PTR _b$[esp+300]
	xor	ebx, ebp
	mov	ebp, DWORD PTR _a$[esp+300]
	and	ebx, ebp
	xor	ebx, edx
	add	edi, ebx
	mov	ebx, DWORD PTR -264+[esp+300]
	mov	ebp, DWORD PTR _w$[esp+ebx+316]
	add	edi, ebp
	mov	ebp, DWORD PTR _K[ebx+16]
	add	edi, ebp
	mov	ebx, esi
	add	edi, eax
	mov	eax, esi
	shl	ebx, 19					; 00000013H
	shr	eax, 13					; 0000000dH
	mov	DWORD PTR _T1$1201[esp+300], edi
	add	ecx, edi
	or	ebx, eax
	mov	eax, esi
	mov	edi, esi
	shr	eax, 22					; 00000016H
	shl	edi, 10					; 0000000aH
	or	eax, edi
	mov	edi, esi
	xor	ebx, eax
	mov	eax, esi
	shl	eax, 30					; 0000001eH
	shr	edi, 2
	or	eax, edi
	mov	edi, DWORD PTR _g$[esp+300]
	xor	ebx, eax
	mov	eax, DWORD PTR _f$[esp+300]
	mov	ebp, edi
	and	edi, eax
	or	ebp, eax
	mov	eax, DWORD PTR _T1$1201[esp+300]
	and	ebp, esi
	add	eax, ebx
	or	ebp, edi
	mov	edi, ecx
	mov	ebx, ecx
	add	eax, ebp
	shl	edi, 21					; 00000015H
	shr	ebx, 11					; 0000000bH
	or	edi, ebx
	mov	ebx, ecx
	mov	ebp, ecx
	shl	ebx, 26					; 0000001aH
	shr	ebp, 6
	or	ebx, ebp
	mov	ebp, ecx
	xor	edi, ebx
	mov	ebx, ecx
	shr	ebx, 25					; 00000019H
	shl	ebp, 7
	or	ebx, ebp
	xor	edi, ebx
	mov	ebx, DWORD PTR _b$[esp+300]
	mov	ebp, ebx
	xor	ebp, DWORD PTR _a$[esp+300]
	and	ebp, ecx
	xor	ebp, ebx
	add	edi, ebp
	mov	ebx, DWORD PTR -264+[esp+300]
	mov	ebp, DWORD PTR _w$[esp+ebx+320]
	add	edi, ebp
	mov	ebp, DWORD PTR _K[ebx+20]
	mov	ebx, DWORD PTR _g$[esp+300]
	add	edi, ebp
	add	edi, edx
	mov	edx, eax
	add	ebx, edi
	mov	ebp, eax
	mov	DWORD PTR _g$[esp+300], ebx
	mov	ebx, eax
	shl	ebx, 19					; 00000013H
	shr	edx, 13					; 0000000dH
	or	ebx, edx
	mov	edx, eax
	shr	edx, 22					; 00000016H
	shl	ebp, 10					; 0000000aH
	or	edx, ebp
	mov	ebp, eax
	xor	ebx, edx
	mov	edx, eax
	shl	edx, 30					; 0000001eH
	shr	ebp, 2
	or	edx, ebp
	xor	ebx, edx
	mov	edx, DWORD PTR _f$[esp+300]
	mov	ebp, edx
	and	edx, esi
	or	ebp, esi
	add	edi, ebx
	mov	ebx, DWORD PTR _g$[esp+300]
	and	ebp, eax
	or	ebp, edx
	add	ebp, edi
	mov	edi, ebx
	mov	edx, ebp
	mov	ebp, ebx
	shl	edi, 21					; 00000015H
	shr	ebp, 11					; 0000000bH
	or	edi, ebp
	mov	ebp, ebx
	shl	ebp, 26					; 0000001aH
	shr	ebx, 6
	or	ebp, ebx
	mov	ebx, DWORD PTR _g$[esp+300]
	xor	edi, ebp
	mov	ebp, ebx
	shr	ebp, 25					; 00000019H
	shl	ebx, 7
	or	ebp, ebx
	mov	ebx, ecx
	xor	edi, ebp
	mov	ebp, DWORD PTR _a$[esp+300]
	xor	ebx, ebp
	mov	ebp, DWORD PTR _g$[esp+300]
	and	ebx, ebp
	mov	ebp, DWORD PTR _a$[esp+300]
	xor	ebx, ebp
	add	edi, ebx
	mov	ebx, DWORD PTR -264+[esp+300]
	mov	ebp, DWORD PTR _w$[esp+ebx+324]
	add	edi, ebp
	mov	ebp, DWORD PTR _K[ebx+24]
	mov	ebx, DWORD PTR _f$[esp+300]
	add	edi, ebp
	mov	ebp, DWORD PTR _b$[esp+300]
	add	edi, ebp
	mov	ebp, edx
	add	ebx, edi
	mov	DWORD PTR _T1$1235[esp+300], edi
	mov	DWORD PTR _f$[esp+300], ebx
	mov	ebx, edx
	mov	edi, edx
	shl	ebx, 19					; 00000013H
	shr	edi, 13					; 0000000dH
	or	ebx, edi
	mov	edi, edx
	shr	edi, 22					; 00000016H
	shl	ebp, 10					; 0000000aH
	or	edi, ebp
	xor	ebx, edi
	mov	edi, edx
	mov	ebp, edx
	shr	ebp, 2
	shl	edi, 30					; 0000001eH
	or	edi, ebp
	mov	ebp, esi
	xor	ebx, edi
	or	ebp, eax
	mov	edi, esi
	and	ebp, edx
	and	edi, eax
	or	ebp, edi
	mov	edi, DWORD PTR _T1$1235[esp+300]
	add	edi, ebx
	mov	ebx, DWORD PTR _f$[esp+300]
	add	ebp, edi
	mov	edi, ebx
	mov	DWORD PTR _b$[esp+300], ebp
	mov	ebp, ebx
	shl	edi, 21					; 00000015H
	shr	ebp, 11					; 0000000bH
	or	edi, ebp
	mov	ebp, ebx
	shl	ebp, 26					; 0000001aH
	shr	ebx, 6
	or	ebp, ebx
	mov	ebx, DWORD PTR _f$[esp+300]
	xor	edi, ebp
	mov	ebp, ebx
	shr	ebp, 25					; 00000019H
	shl	ebx, 7
	or	ebp, ebx
	mov	ebx, ecx
	xor	edi, ebp
	mov	ebp, DWORD PTR _g$[esp+300]
	xor	ebx, ebp
	mov	ebp, DWORD PTR _f$[esp+300]
	and	ebx, ebp
	xor	ebx, ecx
	add	edi, ebx
	mov	ebx, DWORD PTR -264+[esp+300]
	mov	ebp, DWORD PTR _w$[esp+ebx+328]
	add	edi, ebp
	mov	ebp, DWORD PTR _K[ebx+28]
	add	edi, ebp
	mov	ebp, DWORD PTR _a$[esp+300]
	add	edi, ebp
	mov	DWORD PTR _T1$1252[esp+300], edi
	add	esi, edi
	mov	edi, DWORD PTR _b$[esp+300]
	mov	DWORD PTR _e$[esp+300], esi
	mov	ebx, edi
	mov	ebp, edi
	shl	ebx, 19					; 00000013H
	shr	ebp, 13					; 0000000dH
	or	ebx, ebp
	mov	ebp, edi
	shr	ebp, 22					; 00000016H
	shl	edi, 10					; 0000000aH
	or	ebp, edi
	mov	edi, DWORD PTR _b$[esp+300]
	xor	ebx, ebp
	mov	ebp, edi
	shl	ebp, 30					; 0000001eH
	shr	edi, 2
	or	ebp, edi
	mov	edi, DWORD PTR _b$[esp+300]
	xor	ebx, ebp
	mov	ebp, eax
	or	ebp, edx
	and	ebp, edi
	mov	edi, eax
	and	edi, edx
	or	ebp, edi
	mov	edi, DWORD PTR _T1$1252[esp+300]
	add	edi, ebx
	add	ebp, edi
	mov	edi, DWORD PTR -264+[esp+300]
	mov	ebx, ebp
	mov	DWORD PTR _a$[esp+300], ebx
	add	edi, 32					; 00000020H
	cmp	edi, 256				; 00000100H
	mov	DWORD PTR -264+[esp+300], edi
	jl	$L1130
	mov	edi, DWORD PTR _val$[esp+296]
	mov	ebp, DWORD PTR [edi]
	add	ebp, ebx
	mov	ebx, DWORD PTR [edi+4]
	mov	DWORD PTR [edi], ebp
	mov	ebp, DWORD PTR _b$[esp+300]
	add	ebx, ebp
	mov	DWORD PTR [edi+4], ebx
	mov	ebx, DWORD PTR [edi+8]
	add	ebx, edx
	mov	edx, DWORD PTR [edi+12]
	add	edx, eax
	mov	eax, DWORD PTR [edi+16]
	add	eax, esi
	mov	DWORD PTR [edi+12], edx
	mov	edx, DWORD PTR _f$[esp+300]
	mov	DWORD PTR [edi+16], eax
	mov	eax, DWORD PTR [edi+20]
	mov	DWORD PTR [edi+8], ebx
	add	eax, edx
	mov	edx, DWORD PTR _g$[esp+300]
	mov	DWORD PTR [edi+20], eax
	mov	eax, DWORD PTR [edi+24]
	add	eax, edx
	mov	DWORD PTR [edi+24], eax
	mov	eax, DWORD PTR [edi+28]
	add	eax, ecx
	mov	DWORD PTR [edi+28], eax
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

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

	add	esp, 284				; 0000011cH
	ret	0
_br_sha2small_round ENDP
_TEXT	ENDS
EXTRN	_pacc_hash_memcpy:NEAR
;	COMDAT _br_sha224_init
_TEXT	SEGMENT
_cc$ = 8
_br_sha224_init PROC NEAR				; COMDAT

; 255  : {

	push	esi

; 256  : 	cc->vtable = &br_sha224_vtable;

	mov	esi, DWORD PTR _cc$[esp]

; 257  : 	memcpy(cc->val, br_sha224_IV, sizeof cc->val);

	push	32					; 00000020H
	push	OFFSET FLAT:_br_sha224_IV
	lea	eax, DWORD PTR [esi+80]
	mov	DWORD PTR [esi], OFFSET FLAT:_br_sha224_vtable
	push	eax
	call	_pacc_hash_memcpy

; 258  : 	cc->count = 0;

	xor	eax, eax
	add	esp, 12					; 0000000cH
	mov	DWORD PTR [esi+72], eax
	mov	DWORD PTR [esi+76], eax
	pop	esi

; 259  : }

	ret	0
_br_sha224_init ENDP
_TEXT	ENDS
;	COMDAT _br_sha224_update
_TEXT	SEGMENT
_cc$ = 8
_data$ = 12
_len$ = 16
_br_sha224_update PROC NEAR				; COMDAT

; 265  : 	sha2small_update(cc, data, len);

	mov	eax, DWORD PTR _len$[esp-4]
	mov	ecx, DWORD PTR _data$[esp-4]
	mov	edx, DWORD PTR _cc$[esp-4]
	push	eax
	push	ecx
	push	edx
	call	_sha2small_update
	add	esp, 12					; 0000000cH

; 266  : }

	ret	0
_br_sha224_update ENDP
_TEXT	ENDS
;	COMDAT _sha2small_update
_TEXT	SEGMENT
_cc$ = 8
_data$ = 12
_len$ = 16
_buf$ = 12
_sha2small_update PROC NEAR				; COMDAT

; 204  : {

	push	ebx

; 205  : 	const unsigned char *buf;
; 206  : 	size_t ptr;
; 207  : 
; 208  : 	buf = data;
; 209  : 	ptr = (size_t)cc->count & 63;
; 210  : 	cc->count += (uint64_t)len;

	mov	ebx, DWORD PTR _len$[esp]
	push	ebp
	mov	ebp, DWORD PTR _cc$[esp+4]
	push	edi
	mov	eax, DWORD PTR _data$[esp+8]
	mov	edi, DWORD PTR [ebp+72]
	mov	edx, DWORD PTR [ebp+72]
	and	edi, 63					; 0000003fH
	add	edx, ebx
	mov	DWORD PTR [ebp+72], edx
	mov	ecx, DWORD PTR [ebp+76]
	adc	ecx, 0
	mov	DWORD PTR _buf$[esp+8], eax

; 211  : 	while (len > 0) {

	test	ebx, ebx
	mov	DWORD PTR [ebp+76], ecx
	jbe	SHORT $L1280
	push	esi
$L1279:

; 212  : 		size_t clen;
; 213  : 
; 214  : 		clen = 64 - ptr;

	mov	esi, 64					; 00000040H
	sub	esi, edi

; 215  : 		if (clen > len) {

	cmp	esi, ebx
	jbe	SHORT $L1282

; 216  : 			clen = len;

	mov	esi, ebx
$L1282:

; 217  : 		}
; 218  : 		memcpy(cc->buf + ptr, buf, clen);

	mov	ecx, DWORD PTR _buf$[esp+12]
	push	esi
	lea	edx, DWORD PTR [edi+ebp+4]
	push	ecx
	push	edx
	call	_pacc_hash_memcpy

; 219  : 		ptr += clen;
; 220  : 		buf += clen;

	mov	edx, DWORD PTR _buf$[esp+24]
	add	edi, esi
	add	esp, 12					; 0000000cH
	add	edx, esi

; 221  : 		len -= clen;

	sub	ebx, esi

; 222  : 		if (ptr == 64) {

	cmp	edi, 64					; 00000040H
	mov	DWORD PTR _buf$[esp+12], edx
	jne	SHORT $L1283

; 223  : 			br_sha2small_round(cc->buf, cc->val);

	lea	eax, DWORD PTR [ebp+80]
	push	eax
	lea	eax, DWORD PTR [ebp+4]
	push	eax
	call	_br_sha2small_round
	add	esp, 8

; 224  : 			ptr = 0;

	xor	edi, edi
$L1283:

; 211  : 	while (len > 0) {

	test	ebx, ebx
	ja	SHORT $L1279
	pop	esi
$L1280:
	pop	edi
	pop	ebp
	pop	ebx

; 225  : 		}
; 226  : 	}
; 227  : }

	ret	0
_sha2small_update ENDP
_TEXT	ENDS
;	COMDAT _br_sha224_out
_TEXT	SEGMENT
_cc$ = 8
_dst$ = 12
_br_sha224_out PROC NEAR				; COMDAT

; 272  : 	sha2small_out(cc, dst, 7);

	mov	eax, DWORD PTR _dst$[esp-4]
	mov	ecx, DWORD PTR _cc$[esp-4]
	push	7
	push	eax
	push	ecx
	call	_sha2small_out
	add	esp, 12					; 0000000cH

; 273  : }

	ret	0
_br_sha224_out ENDP
_TEXT	ENDS
EXTRN	_br_range_enc32be:NEAR
EXTRN	__allshl:NEAR
EXTRN	_pacc_hash_memset:NEAR
;	COMDAT _sha2small_out
_TEXT	SEGMENT
_cc$ = 8
_dst$ = 12
_num$ = 16
_buf$ = -64
_val$ = -96
_sha2small_out PROC NEAR				; COMDAT

; 231  : {

	sub	esp, 96					; 00000060H

; 232  : 	unsigned char buf[64];
; 233  : 	uint32_t val[8];
; 234  : 	size_t ptr;
; 235  : 
; 236  : 	ptr = (size_t)cc->count & 63;
; 237  : 	memcpy(buf, cc->buf, ptr);

	lea	ecx, DWORD PTR _buf$[esp+96]
	push	esi
	push	edi
	mov	edi, DWORD PTR _cc$[esp+100]
	mov	esi, DWORD PTR [edi+72]
	lea	eax, DWORD PTR [edi+4]
	and	esi, 63					; 0000003fH
	push	esi
	push	eax
	push	ecx
	call	_pacc_hash_memcpy

; 238  : 	memcpy(val, cc->val, sizeof val);

	lea	edx, DWORD PTR [edi+80]
	push	32					; 00000020H
	lea	eax, DWORD PTR _val$[esp+120]
	push	edx
	push	eax
	call	_pacc_hash_memcpy
	add	esp, 24					; 00000018H

; 239  : 	buf[ptr ++] = 0x80;

	mov	BYTE PTR _buf$[esp+esi+104], 128	; 00000080H
	inc	esi

; 240  : 	if (ptr > 56) {

	cmp	esi, 56					; 00000038H
	jbe	SHORT $L1296

; 241  : 		memset(buf + ptr, 0, 64 - ptr);

	mov	ecx, 64					; 00000040H
	lea	edx, DWORD PTR _buf$[esp+esi+104]
	sub	ecx, esi
	push	ecx
	push	0
	push	edx
	call	_pacc_hash_memset

; 242  : 		br_sha2small_round(buf, val);

	lea	eax, DWORD PTR _val$[esp+116]
	lea	ecx, DWORD PTR _buf$[esp+116]
	push	eax
	push	ecx
	call	_br_sha2small_round

; 243  : 		memset(buf, 0, 56);

	push	56					; 00000038H
	lea	edx, DWORD PTR _buf$[esp+128]
	push	0
	push	edx
	call	_pacc_hash_memset
	add	esp, 32					; 00000020H

; 244  : 	} else {

	jmp	SHORT $L1297
$L1296:

; 245  : 		memset(buf + ptr, 0, 56 - ptr);

	mov	eax, 56					; 00000038H
	lea	ecx, DWORD PTR _buf$[esp+esi+104]
	sub	eax, esi
	push	eax
	push	0
	push	ecx
	call	_pacc_hash_memset
	add	esp, 12					; 0000000cH
$L1297:

; 246  : 	}
; 247  : 	br_enc64be(buf + 56, cc->count << 3);

	mov	eax, DWORD PTR [edi+72]
	mov	edx, DWORD PTR [edi+76]
	mov	ecx, 3
	call	__allshl
	mov	ecx, edx
	mov	BYTE PTR _buf$[esp+163], dl
	shr	ecx, 24					; 00000018H
	mov	BYTE PTR _buf$[esp+160], cl
	mov	ecx, edx
	shr	ecx, 16					; 00000010H
	mov	BYTE PTR _buf$[esp+161], cl
	mov	ecx, edx
	shr	ecx, 8
	mov	edx, eax
	mov	BYTE PTR _buf$[esp+162], cl
	shr	edx, 24					; 00000018H
	mov	ecx, eax
	mov	BYTE PTR _buf$[esp+164], dl
	shr	ecx, 16					; 00000010H
	mov	edx, eax
	mov	BYTE PTR _buf$[esp+167], al
	mov	BYTE PTR _buf$[esp+165], cl

; 248  : 	br_sha2small_round(buf, val);

	lea	eax, DWORD PTR _val$[esp+104]
	lea	ecx, DWORD PTR _buf$[esp+104]
	push	eax
	shr	edx, 8
	push	ecx
	mov	BYTE PTR _buf$[esp+174], dl
	call	_br_sha2small_round

; 249  : 	br_range_enc32be(dst, val, num);

	mov	edx, DWORD PTR _num$[esp+108]
	mov	ecx, DWORD PTR _dst$[esp+108]
	lea	eax, DWORD PTR _val$[esp+112]
	push	edx
	push	eax
	push	ecx
	call	_br_range_enc32be
	add	esp, 20					; 00000014H
	pop	edi
	pop	esi

; 250  : }

	add	esp, 96					; 00000060H
	ret	0
_sha2small_out ENDP
_TEXT	ENDS
;	COMDAT _br_sha224_state
_TEXT	SEGMENT
_cc$ = 8
_dst$ = 12
_br_sha224_state PROC NEAR				; COMDAT

; 279  : 	br_range_enc32be(dst, cc->val, 8);

	mov	ecx, DWORD PTR _dst$[esp-4]
	push	esi
	mov	esi, DWORD PTR _cc$[esp]
	push	8
	lea	eax, DWORD PTR [esi+80]
	push	eax
	push	ecx
	call	_br_range_enc32be

; 280  : 	return cc->count;

	mov	eax, DWORD PTR [esi+72]
	mov	edx, DWORD PTR [esi+76]
	add	esp, 12					; 0000000cH
	pop	esi

; 281  : }

	ret	0
_br_sha224_state ENDP
_TEXT	ENDS
;	COMDAT _br_sha224_set_state
_TEXT	SEGMENT
_cc$ = 8
_stb$ = 12
_count$ = 16
_br_sha224_set_state PROC NEAR				; COMDAT

; 287  : 	br_range_dec32be(cc->val, 8, stb);

	mov	eax, DWORD PTR _stb$[esp-4]
	push	esi
	mov	esi, DWORD PTR _cc$[esp]
	push	eax
	push	8
	lea	ecx, DWORD PTR [esi+80]
	push	ecx
	call	_br_range_dec32be

; 288  : 	cc->count = count;

	mov	edx, DWORD PTR _count$[esp+12]
	mov	eax, DWORD PTR _count$[esp+16]
	add	esp, 12					; 0000000cH
	mov	DWORD PTR [esi+72], edx
	mov	DWORD PTR [esi+76], eax
	pop	esi

; 289  : }

	ret	0
_br_sha224_set_state ENDP
_TEXT	ENDS
;	COMDAT _br_sha256_init
_TEXT	SEGMENT
_cc$ = 8
_br_sha256_init PROC NEAR				; COMDAT

; 294  : {

	push	esi

; 295  : 	cc->vtable = &br_sha256_vtable;

	mov	esi, DWORD PTR _cc$[esp]

; 296  : 	memcpy(cc->val, br_sha256_IV, sizeof cc->val);

	push	32					; 00000020H
	push	OFFSET FLAT:_br_sha256_IV
	lea	eax, DWORD PTR [esi+80]
	mov	DWORD PTR [esi], OFFSET FLAT:_br_sha256_vtable
	push	eax
	call	_pacc_hash_memcpy

; 297  : 	cc->count = 0;

	xor	eax, eax
	add	esp, 12					; 0000000cH
	mov	DWORD PTR [esi+72], eax
	mov	DWORD PTR [esi+76], eax
	pop	esi

; 298  : }

	ret	0
_br_sha256_init ENDP
_TEXT	ENDS
;	COMDAT _br_sha256_out
_TEXT	SEGMENT
_cc$ = 8
_dst$ = 12
_br_sha256_out PROC NEAR				; COMDAT

; 304  : 	sha2small_out(cc, dst, 8);

	mov	eax, DWORD PTR _dst$[esp-4]
	mov	ecx, DWORD PTR _cc$[esp-4]
	push	8
	push	eax
	push	ecx
	call	_sha2small_out
	add	esp, 12					; 0000000cH

; 305  : }

	ret	0
_br_sha256_out ENDP
_TEXT	ENDS
END
