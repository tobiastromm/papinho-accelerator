	TITLE	C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\storage_sha256.c
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
PUBLIC	_pacc_storage_sha_init
EXTRN	_br_sha256_init:NEAR
_TEXT	SEGMENT
_c$ = 8
_pacc_storage_sha_init PROC NEAR

; 16   : {if(!span(c,sizeof(*c)))return PACC_HASH_INVALID;

	push	ebp
	mov	ebp, esp
	push	128					; 00000080H
	mov	eax, DWORD PTR _c$[ebp]
	push	eax
	call	_span
	add	esp, 8
	test	eax, eax
	jne	SHORT $L1127
	mov	eax, 1
	jmp	SHORT $L1126
$L1127:

; 17   :  if(c->version||c->state||c->busy)return PACC_HASH_STATE;

	mov	ecx, DWORD PTR _c$[ebp]
	cmp	DWORD PTR [ecx], 0
	jne	SHORT $L1129
	mov	edx, DWORD PTR _c$[ebp]
	cmp	DWORD PTR [edx+4], 0
	jne	SHORT $L1129
	mov	eax, DWORD PTR _c$[ebp]
	cmp	DWORD PTR [eax+8], 0
	je	SHORT $L1128
$L1129:
	mov	eax, 2
	jmp	SHORT $L1126
$L1128:

; 18   :  br_sha256_init(&c->hash);c->version=1U;c->state=PACC_HASH_ACTIVE;return PACC_HASH_OK;}

	mov	ecx, DWORD PTR _c$[ebp]
	add	ecx, 16					; 00000010H
	push	ecx
	call	_br_sha256_init
	add	esp, 4
	mov	edx, DWORD PTR _c$[ebp]
	mov	DWORD PTR [edx], 1
	mov	eax, DWORD PTR _c$[ebp]
	mov	DWORD PTR [eax+4], 1
	xor	eax, eax
$L1126:
	pop	ebp
	ret	0
_pacc_storage_sha_init ENDP
_p$ = 8
_n$ = 12
_span	PROC NEAR

; 5    : {return !n || (p && n-1U<=(size_t)-1-(size_t)p);}

	push	ebp
	mov	ebp, esp
	push	ecx
	cmp	DWORD PTR _n$[ebp], 0
	je	SHORT $L1210
	cmp	DWORD PTR _p$[ebp], 0
	je	SHORT $L1209
	mov	eax, DWORD PTR _n$[ebp]
	sub	eax, 1
	or	ecx, -1
	sub	ecx, DWORD PTR _p$[ebp]
	cmp	eax, ecx
	jbe	SHORT $L1210
$L1209:
	mov	DWORD PTR -4+[ebp], 0
	jmp	SHORT $L1211
$L1210:
	mov	DWORD PTR -4+[ebp], 1
$L1211:
	mov	eax, DWORD PTR -4+[ebp]
	mov	esp, ebp
	pop	ebp
	ret	0
_span	ENDP
_TEXT	ENDS
PUBLIC	_pacc_storage_sha_update
EXTRN	_br_sha224_update:NEAR
_TEXT	SEGMENT
_c$ = 8
_data$ = 12
_n$ = 16
_stop$ = 20
_p$ = -4
_k$ = -20
_r$ = -8
_maxbytes$ = -16
_pacc_storage_sha_update PROC NEAR

; 20   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 40					; 00000028H
	push	esi
	push	edi

; 21   :  const unsigned char *p=data;size_t k;int r=enter(c);uint64_t maxbytes=(((uint64_t)1U)<<61)-1U;

	mov	eax, DWORD PTR _data$[ebp]
	mov	DWORD PTR _p$[ebp], eax
	mov	ecx, DWORD PTR _c$[ebp]
	push	ecx
	call	_enter
	add	esp, 4
	mov	DWORD PTR _r$[ebp], eax
	mov	DWORD PTR _maxbytes$[ebp], -1
	mov	DWORD PTR _maxbytes$[ebp+4], 536870911	; 1fffffffH

; 22   :  (void)sc;if(r)return r;

	cmp	DWORD PTR _r$[ebp], 0
	je	SHORT $L1147
	mov	eax, DWORD PTR _r$[ebp]
	jmp	$L1140
$L1147:

; 23   :  if((stop&&!span((const void *)stop,sizeof(*stop)))||!span(data,n)||overlap(data,n,c,sizeof(*c)))return terminal(c,PACC_HASH_INVALID);

	cmp	DWORD PTR _stop$[ebp], 0
	je	SHORT $L1151
	push	4
	mov	edx, DWORD PTR _stop$[ebp]
	push	edx
	call	_span
	add	esp, 8
	test	eax, eax
	je	SHORT $L1150
$L1151:
	mov	eax, DWORD PTR _n$[ebp]
	push	eax
	mov	ecx, DWORD PTR _data$[ebp]
	push	ecx
	call	_span
	add	esp, 8
	test	eax, eax
	je	SHORT $L1150
	push	128					; 00000080H
	mov	edx, DWORD PTR _c$[ebp]
	push	edx
	mov	eax, DWORD PTR _n$[ebp]
	push	eax
	mov	ecx, DWORD PTR _data$[ebp]
	push	ecx
	call	_overlap
	add	esp, 16					; 00000010H
	test	eax, eax
	je	SHORT $L1149
$L1150:
	push	1
	mov	edx, DWORD PTR _c$[ebp]
	push	edx
	call	_terminal
	add	esp, 8
	jmp	$L1140
$L1149:

; 24   :  if(c->hash.count>maxbytes||(uint64_t)n>maxbytes-c->hash.count)return terminal(c,PACC_HASH_LIMIT);

	mov	eax, DWORD PTR _c$[ebp]
	add	eax, 88					; 00000058H
	mov	DWORD PTR -24+[ebp], eax
	mov	ecx, DWORD PTR -24+[ebp]
	mov	edx, DWORD PTR [ecx+4]
	cmp	edx, DWORD PTR _maxbytes$[ebp+4]
	ja	SHORT $L1154
	jb	SHORT $L1213
	mov	eax, DWORD PTR -24+[ebp]
	mov	ecx, DWORD PTR [eax]
	cmp	ecx, DWORD PTR _maxbytes$[ebp]
	ja	SHORT $L1154
$L1213:
	mov	edx, DWORD PTR _n$[ebp]
	xor	eax, eax
	mov	ecx, DWORD PTR _c$[ebp]
	mov	esi, DWORD PTR _maxbytes$[ebp]
	sub	esi, DWORD PTR [ecx+88]
	mov	edi, DWORD PTR _maxbytes$[ebp+4]
	sbb	edi, DWORD PTR [ecx+92]
	mov	DWORD PTR -32+[ebp], edx
	mov	DWORD PTR -32+[ebp+4], eax
	mov	DWORD PTR -40+[ebp], esi
	mov	DWORD PTR -40+[ebp+4], edi
	mov	edx, DWORD PTR -32+[ebp+4]
	cmp	edx, DWORD PTR -40+[ebp+4]
	jb	SHORT $L1153
	ja	SHORT $L1154
	mov	eax, DWORD PTR -32+[ebp]
	cmp	eax, DWORD PTR -40+[ebp]
	jbe	SHORT $L1153
$L1154:
	push	4
	mov	ecx, DWORD PTR _c$[ebp]
	push	ecx
	call	_terminal
	add	esp, 8
	jmp	$L1140
$L1153:

; 25   :  if(stop&&*stop)return terminal(c,PACC_HASH_STOPPED);

	cmp	DWORD PTR _stop$[ebp], 0
	je	SHORT $L1155
	mov	edx, DWORD PTR _stop$[ebp]
	cmp	DWORD PTR [edx], 0
	je	SHORT $L1155
	push	3
	mov	eax, DWORD PTR _c$[ebp]
	push	eax
	call	_terminal
	add	esp, 8
	jmp	SHORT $L1140
$L1155:

; 26   :  while(n){k=n;if(k>PACC_STORAGE_HASH_CHUNK)k=PACC_STORAGE_HASH_CHUNK;

	cmp	DWORD PTR _n$[ebp], 0
	je	SHORT $L1158
	mov	ecx, DWORD PTR _n$[ebp]
	mov	DWORD PTR _k$[ebp], ecx
	cmp	DWORD PTR _k$[ebp], 1024		; 00000400H
	jbe	SHORT $L1159
	mov	DWORD PTR _k$[ebp], 1024		; 00000400H
$L1159:

; 27   :   br_sha256_update(&c->hash,p,k);p+=k;n-=k;

	mov	edx, DWORD PTR _k$[ebp]
	push	edx
	mov	eax, DWORD PTR _p$[ebp]
	push	eax
	mov	ecx, DWORD PTR _c$[ebp]
	add	ecx, 16					; 00000010H
	push	ecx
	call	_br_sha224_update
	add	esp, 12					; 0000000cH
	mov	edx, DWORD PTR _p$[ebp]
	add	edx, DWORD PTR _k$[ebp]
	mov	DWORD PTR _p$[ebp], edx
	mov	eax, DWORD PTR _n$[ebp]
	sub	eax, DWORD PTR _k$[ebp]
	mov	DWORD PTR _n$[ebp], eax

; 28   :   if(stop&&*stop)return terminal(c,PACC_HASH_STOPPED);

	cmp	DWORD PTR _stop$[ebp], 0
	je	SHORT $L1160
	mov	ecx, DWORD PTR _stop$[ebp]
	cmp	DWORD PTR [ecx], 0
	je	SHORT $L1160
	push	3
	mov	edx, DWORD PTR _c$[ebp]
	push	edx
	call	_terminal
	add	esp, 8
	jmp	SHORT $L1140
$L1160:

; 29   :  }

	jmp	SHORT $L1155
$L1158:

; 30   :  c->busy=0;return PACC_HASH_OK;

	mov	eax, DWORD PTR _c$[ebp]
	mov	DWORD PTR [eax+8], 0
	xor	eax, eax
$L1140:

; 31   : }

	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	0
_pacc_storage_sha_update ENDP
_p$ = 8
_n$ = 12
_q$ = 16
_m$ = 20
_a$ = -4
_b$ = -8
_overlap PROC NEAR

; 7    : {size_t a=(size_t)p,b=(size_t)q;return n&&m&&(a<=b?b-a<n:a-b<m);}

	push	ebp
	mov	ebp, esp
	sub	esp, 16					; 00000010H
	mov	eax, DWORD PTR _p$[ebp]
	mov	DWORD PTR _a$[ebp], eax
	mov	ecx, DWORD PTR _q$[ebp]
	mov	DWORD PTR _b$[ebp], ecx
	cmp	DWORD PTR _n$[ebp], 0
	je	SHORT $L1216
	cmp	DWORD PTR _m$[ebp], 0
	je	SHORT $L1216
	mov	edx, DWORD PTR _a$[ebp]
	cmp	edx, DWORD PTR _b$[ebp]
	ja	SHORT $L1217
	mov	eax, DWORD PTR _b$[ebp]
	sub	eax, DWORD PTR _a$[ebp]
	cmp	eax, DWORD PTR _n$[ebp]
	sbb	ecx, ecx
	neg	ecx
	mov	DWORD PTR -12+[ebp], ecx
	jmp	SHORT $L1218
$L1217:
	mov	edx, DWORD PTR _a$[ebp]
	sub	edx, DWORD PTR _b$[ebp]
	cmp	edx, DWORD PTR _m$[ebp]
	sbb	eax, eax
	neg	eax
	mov	DWORD PTR -12+[ebp], eax
$L1218:
	cmp	DWORD PTR -12+[ebp], 0
	je	SHORT $L1216
	mov	DWORD PTR -16+[ebp], 1
	jmp	SHORT $L1219
$L1216:
	mov	DWORD PTR -16+[ebp], 0
$L1219:
	mov	eax, DWORD PTR -16+[ebp]
	mov	esp, ebp
	pop	ebp
	ret	0
_overlap ENDP
_c$ = 8
_r$ = 12
_terminal PROC NEAR

; 11   : {clear_hash(c);c->state=r==PACC_HASH_STOPPED?PACC_HASH_CANCELLED:PACC_HASH_FAILED;c->busy=0;return r;}

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR _c$[ebp]
	push	eax
	call	_clear_hash
	add	esp, 4
	xor	ecx, ecx
	cmp	DWORD PTR _r$[ebp], 3
	sete	cl
	add	ecx, 3
	mov	edx, DWORD PTR _c$[ebp]
	mov	DWORD PTR [edx+4], ecx
	mov	eax, DWORD PTR _c$[ebp]
	mov	DWORD PTR [eax+8], 0
	mov	eax, DWORD PTR _r$[ebp]
	pop	ebp
	ret	0
_terminal ENDP
_c$ = 8
_p$ = -4
_n$ = -8
_clear_hash PROC NEAR

; 9    : {volatile unsigned char *p=(volatile unsigned char *)&c->hash;size_t n=sizeof(c->hash);while(n--)*p++=0;}

	push	ebp
	mov	ebp, esp
	sub	esp, 8
	mov	eax, DWORD PTR _c$[ebp]
	add	eax, 16					; 00000010H
	mov	DWORD PTR _p$[ebp], eax
	mov	DWORD PTR _n$[ebp], 112			; 00000070H
$L1108:
	mov	ecx, DWORD PTR _n$[ebp]
	mov	edx, DWORD PTR _n$[ebp]
	sub	edx, 1
	mov	DWORD PTR _n$[ebp], edx
	test	ecx, ecx
	je	SHORT $L1109
	mov	eax, DWORD PTR _p$[ebp]
	mov	BYTE PTR [eax], 0
	mov	ecx, DWORD PTR _p$[ebp]
	add	ecx, 1
	mov	DWORD PTR _p$[ebp], ecx
	jmp	SHORT $L1108
$L1109:
	mov	esp, ebp
	pop	ebp
	ret	0
_clear_hash ENDP
_c$ = 8
_enter	PROC NEAR

; 13   : {if(!span(c,sizeof(*c)))return PACC_HASH_INVALID;if(c->version!=1U||c->state!=PACC_HASH_ACTIVE)return PACC_HASH_STATE;

	push	ebp
	mov	ebp, esp
	push	128					; 00000080H
	mov	eax, DWORD PTR _c$[ebp]
	push	eax
	call	_span
	add	esp, 8
	test	eax, eax
	jne	SHORT $L1120
	mov	eax, 1
	jmp	SHORT $L1119
$L1120:
	mov	ecx, DWORD PTR _c$[ebp]
	cmp	DWORD PTR [ecx], 1
	jne	SHORT $L1122
	mov	edx, DWORD PTR _c$[ebp]
	cmp	DWORD PTR [edx+4], 1
	je	SHORT $L1121
$L1122:
	mov	eax, 2
	jmp	SHORT $L1119
$L1121:

; 14   :  if(c->busy)return PACC_HASH_BUSY;c->busy=1;return PACC_HASH_OK;}

	mov	eax, DWORD PTR _c$[ebp]
	cmp	DWORD PTR [eax+8], 0
	je	SHORT $L1123
	mov	eax, 5
	jmp	SHORT $L1119
$L1123:
	mov	ecx, DWORD PTR _c$[ebp]
	mov	DWORD PTR [ecx+8], 1
	xor	eax, eax
$L1119:
	pop	ebp
	ret	0
_enter	ENDP
_TEXT	ENDS
PUBLIC	_pacc_storage_sha_final
EXTRN	_pacc_hash_memcpy:NEAR
EXTRN	_br_sha256_out:NEAR
_TEXT	SEGMENT
_c$ = 8
_out$ = 12
_cap$ = 16
_stop$ = 20
_staged$ = -36
_r$ = -4
_i$ = -40
_v$1183 = -44
_pacc_storage_sha_final PROC NEAR

; 33   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 44					; 0000002cH

; 34   :  unsigned char staged[32];int r=enter(c);unsigned i;

	mov	eax, DWORD PTR _c$[ebp]
	push	eax
	call	_enter
	add	esp, 4
	mov	DWORD PTR _r$[ebp], eax

; 35   :  (void)sc;if(r)return r;

	cmp	DWORD PTR _r$[ebp], 0
	je	SHORT $L1176
	mov	eax, DWORD PTR _r$[ebp]
	jmp	$L1171
$L1176:

; 36   :  if((stop&&!span((const void *)stop,sizeof(*stop)))||cap<32U||!span(out,32U)||overlap(out,32U,c,sizeof(*c)))return terminal(c,PACC_HASH_INVALID);

	cmp	DWORD PTR _stop$[ebp], 0
	je	SHORT $L1180
	push	4
	mov	ecx, DWORD PTR _stop$[ebp]
	push	ecx
	call	_span
	add	esp, 8
	test	eax, eax
	je	SHORT $L1179
$L1180:
	cmp	DWORD PTR _cap$[ebp], 32		; 00000020H
	jb	SHORT $L1179
	push	32					; 00000020H
	mov	edx, DWORD PTR _out$[ebp]
	push	edx
	call	_span
	add	esp, 8
	test	eax, eax
	je	SHORT $L1179
	push	128					; 00000080H
	mov	eax, DWORD PTR _c$[ebp]
	push	eax
	push	32					; 00000020H
	mov	ecx, DWORD PTR _out$[ebp]
	push	ecx
	call	_overlap
	add	esp, 16					; 00000010H
	test	eax, eax
	je	SHORT $L1178
$L1179:
	push	1
	mov	edx, DWORD PTR _c$[ebp]
	push	edx
	call	_terminal
	add	esp, 8
	jmp	$L1171
$L1178:

; 37   :  if(stop&&*stop)return terminal(c,PACC_HASH_STOPPED);

	cmp	DWORD PTR _stop$[ebp], 0
	je	SHORT $L1181
	mov	eax, DWORD PTR _stop$[ebp]
	cmp	DWORD PTR [eax], 0
	je	SHORT $L1181
	push	3
	mov	ecx, DWORD PTR _c$[ebp]
	push	ecx
	call	_terminal
	add	esp, 8
	jmp	$L1171
$L1181:

; 38   :  br_sha256_out(&c->hash,staged);

	lea	edx, DWORD PTR _staged$[ebp]
	push	edx
	mov	eax, DWORD PTR _c$[ebp]
	add	eax, 16					; 00000010H
	push	eax
	call	_br_sha256_out
	add	esp, 8

; 39   :  if(stop&&*stop){volatile unsigned char *v=staged;for(i=0;i<32U;i++)v[i]=0;return terminal(c,PACC_HASH_STOPPED);}

	cmp	DWORD PTR _stop$[ebp], 0
	je	SHORT $L1182
	mov	ecx, DWORD PTR _stop$[ebp]
	cmp	DWORD PTR [ecx], 0
	je	SHORT $L1182
	lea	edx, DWORD PTR _staged$[ebp]
	mov	DWORD PTR _v$1183[ebp], edx
	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $L1184
$L1185:
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 1
	mov	DWORD PTR _i$[ebp], eax
$L1184:
	cmp	DWORD PTR _i$[ebp], 32			; 00000020H
	jae	SHORT $L1186
	mov	ecx, DWORD PTR _v$1183[ebp]
	add	ecx, DWORD PTR _i$[ebp]
	mov	BYTE PTR [ecx], 0
	jmp	SHORT $L1185
$L1186:
	push	3
	mov	edx, DWORD PTR _c$[ebp]
	push	edx
	call	_terminal
	add	esp, 8
	jmp	SHORT $L1171
$L1182:

; 40   :  memcpy(out,staged,32U);clear_hash(c);c->state=PACC_HASH_DONE;c->busy=0;return PACC_HASH_OK;

	push	32					; 00000020H
	lea	eax, DWORD PTR _staged$[ebp]
	push	eax
	mov	ecx, DWORD PTR _out$[ebp]
	push	ecx
	call	_pacc_hash_memcpy
	add	esp, 12					; 0000000cH
	mov	edx, DWORD PTR _c$[ebp]
	push	edx
	call	_clear_hash
	add	esp, 4
	mov	eax, DWORD PTR _c$[ebp]
	mov	DWORD PTR [eax+4], 2
	mov	ecx, DWORD PTR _c$[ebp]
	mov	DWORD PTR [ecx+8], 0
	xor	eax, eax
$L1171:

; 41   : }

	mov	esp, ebp
	pop	ebp
	ret	0
_pacc_storage_sha_final ENDP
_TEXT	ENDS
PUBLIC	_pacc_storage_hash_callback
EXTRN	_pacc_hash_memset:NEAR
_TEXT	SEGMENT
_ctx$ = 8
_p$ = 12
_n$ = 16
_out$ = 20
_c$ = -132
_call$ = -136
_r$ = -4
_pacc_storage_hash_callback PROC NEAR

; 43   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 152				; 00000098H

; 44   :  PACC_STORAGE_SHA256 c;PACC_STORAGE_HASH_CALL *call=ctx;int r;

	mov	eax, DWORD PTR _ctx$[ebp]
	mov	DWORD PTR _call$[ebp], eax

; 45   :  memset(&c,0,sizeof(c));r=pacc_storage_sha_init(&c);

	push	128					; 00000080H
	push	0
	lea	ecx, DWORD PTR _c$[ebp]
	push	ecx
	call	_pacc_hash_memset
	add	esp, 12					; 0000000cH
	lea	edx, DWORD PTR _c$[ebp]
	push	edx
	call	_pacc_storage_sha_init
	add	esp, 4
	mov	DWORD PTR _r$[ebp], eax

; 46   :  if(!r)r=pacc_storage_sha_update(&c,p,n,call?call->stop:NULL,call?call->context:NULL);

	cmp	DWORD PTR _r$[ebp], 0
	jne	SHORT $L1199
	cmp	DWORD PTR _call$[ebp], 0
	je	SHORT $L1225
	mov	eax, DWORD PTR _call$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR -140+[ebp], ecx
	jmp	SHORT $L1226
$L1225:
	mov	DWORD PTR -140+[ebp], 0
$L1226:
	cmp	DWORD PTR _call$[ebp], 0
	je	SHORT $L1227
	mov	edx, DWORD PTR _call$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR -144+[ebp], eax
	jmp	SHORT $L1228
$L1227:
	mov	DWORD PTR -144+[ebp], 0
$L1228:
	mov	ecx, DWORD PTR -140+[ebp]
	push	ecx
	mov	edx, DWORD PTR -144+[ebp]
	push	edx
	mov	eax, DWORD PTR _n$[ebp]
	push	eax
	mov	ecx, DWORD PTR _p$[ebp]
	push	ecx
	lea	edx, DWORD PTR _c$[ebp]
	push	edx
	call	_pacc_storage_sha_update
	add	esp, 20					; 00000014H
	mov	DWORD PTR _r$[ebp], eax
$L1199:

; 47   :  if(!r)r=pacc_storage_sha_final(&c,out,32U,call?call->stop:NULL,call?call->context:NULL);

	cmp	DWORD PTR _r$[ebp], 0
	jne	SHORT $L1202
	cmp	DWORD PTR _call$[ebp], 0
	je	SHORT $L1229
	mov	eax, DWORD PTR _call$[ebp]
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR -148+[ebp], ecx
	jmp	SHORT $L1230
$L1229:
	mov	DWORD PTR -148+[ebp], 0
$L1230:
	cmp	DWORD PTR _call$[ebp], 0
	je	SHORT $L1231
	mov	edx, DWORD PTR _call$[ebp]
	mov	eax, DWORD PTR [edx]
	mov	DWORD PTR -152+[ebp], eax
	jmp	SHORT $L1232
$L1231:
	mov	DWORD PTR -152+[ebp], 0
$L1232:
	mov	ecx, DWORD PTR -148+[ebp]
	push	ecx
	mov	edx, DWORD PTR -152+[ebp]
	push	edx
	push	32					; 00000020H
	mov	eax, DWORD PTR _out$[ebp]
	push	eax
	lea	ecx, DWORD PTR _c$[ebp]
	push	ecx
	call	_pacc_storage_sha_final
	add	esp, 20					; 00000014H
	mov	DWORD PTR _r$[ebp], eax
$L1202:

; 48   :  return r==PACC_HASH_OK;

	xor	eax, eax
	cmp	DWORD PTR _r$[ebp], 0
	sete	al

; 49   : }

	mov	esp, ebp
	pop	ebp
	ret	0
_pacc_storage_hash_callback ENDP
_TEXT	ENDS
END
