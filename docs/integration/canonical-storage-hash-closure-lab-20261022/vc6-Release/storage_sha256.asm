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
;	COMDAT _span
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _overlap
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _clear_hash
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _terminal
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _enter
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _pacc_storage_sha_init
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _pacc_storage_sha_update
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _pacc_storage_sha_final
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _pacc_storage_hash_callback
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
FLAT	GROUP _DATA, CONST, _BSS
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
PUBLIC	_pacc_storage_sha_init
EXTRN	_br_sha256_init:NEAR
;	COMDAT _pacc_storage_sha_init
_TEXT	SEGMENT
_c$ = 8
_pacc_storage_sha_init PROC NEAR			; COMDAT

; 16   : {if(!span(c,sizeof(*c)))return PACC_HASH_INVALID;

	push	esi
	mov	esi, DWORD PTR _c$[esp]
	push	128					; 00000080H
	push	esi
	call	_span
	add	esp, 8
	test	eax, eax
	jne	SHORT $L1152
	mov	eax, 1
	pop	esi

; 18   :  br_sha256_init(&c->hash);c->version=1U;c->state=PACC_HASH_ACTIVE;return PACC_HASH_OK;}

	ret	0
$L1152:

; 17   :  if(c->version||c->state||c->busy)return PACC_HASH_STATE;

	cmp	DWORD PTR [esi], 0
	jne	SHORT $L1154
	mov	eax, DWORD PTR [esi+4]
	test	eax, eax
	jne	SHORT $L1154
	mov	eax, DWORD PTR [esi+8]
	test	eax, eax
	jne	SHORT $L1154

; 18   :  br_sha256_init(&c->hash);c->version=1U;c->state=PACC_HASH_ACTIVE;return PACC_HASH_OK;}

	lea	eax, DWORD PTR [esi+16]
	push	eax
	call	_br_sha256_init
	add	esp, 4
	mov	DWORD PTR [esi], 1
	mov	DWORD PTR [esi+4], 1
	xor	eax, eax
	pop	esi
	ret	0
$L1154:

; 17   :  if(c->version||c->state||c->busy)return PACC_HASH_STATE;

	mov	eax, 2
	pop	esi

; 18   :  br_sha256_init(&c->hash);c->version=1U;c->state=PACC_HASH_ACTIVE;return PACC_HASH_OK;}

	ret	0
_pacc_storage_sha_init ENDP
_TEXT	ENDS
;	COMDAT _span
_TEXT	SEGMENT
_p$ = 8
_n$ = 12
_span	PROC NEAR					; COMDAT

; 5    : {return !n || (p && n-1U<=(size_t)-1-(size_t)p);}

	mov	ecx, DWORD PTR _n$[esp-4]
	test	ecx, ecx
	je	SHORT $L1245
	mov	eax, DWORD PTR _p$[esp-4]
	test	eax, eax
	je	SHORT $L1244
	or	edx, -1
	sub	edx, eax
	dec	ecx
	cmp	ecx, edx
	jbe	SHORT $L1245
$L1244:
	xor	eax, eax
	ret	0
$L1245:
	mov	eax, 1
	ret	0
_span	ENDP
_TEXT	ENDS
PUBLIC	_pacc_storage_sha_update
EXTRN	_br_sha224_update:NEAR
;	COMDAT _pacc_storage_sha_update
_TEXT	SEGMENT
_c$ = 8
_data$ = 12
_n$ = 16
_stop$ = 20
_p$ = 12
_pacc_storage_sha_update PROC NEAR			; COMDAT

; 20   : {

	push	ebx

; 21   :  const unsigned char *p=data;size_t k;int r=enter(c);uint64_t maxbytes=(((uint64_t)1U)<<61)-1U;

	mov	ebx, DWORD PTR _c$[esp]
	push	ebp
	push	esi
	mov	esi, DWORD PTR _data$[esp+8]
	push	edi
	push	ebx
	mov	DWORD PTR _p$[esp+16], esi
	call	_enter
	add	esp, 4

; 22   :  (void)sc;if(r)return r;

	test	eax, eax
	jne	$L1165

; 23   :  if((stop&&!span((const void *)stop,sizeof(*stop)))||!span(data,n)||overlap(data,n,c,sizeof(*c)))return terminal(c,PACC_HASH_INVALID);

	mov	ebp, DWORD PTR _stop$[esp+12]
	test	ebp, ebp
	je	SHORT $L1176
	push	4
	push	ebp
	call	_span
	add	esp, 8
	test	eax, eax
	je	$L1175
$L1176:
	mov	edi, DWORD PTR _n$[esp+12]
	push	edi
	push	esi
	call	_span
	add	esp, 8
	test	eax, eax
	je	$L1175
	push	128					; 00000080H
	push	ebx
	push	edi
	push	esi
	call	_overlap
	add	esp, 16					; 00000010H
	test	eax, eax
	jne	$L1175

; 24   :  if(c->hash.count>maxbytes||(uint64_t)n>maxbytes-c->hash.count)return terminal(c,PACC_HASH_LIMIT);

	mov	edx, DWORD PTR [ebx+92]
	mov	eax, DWORD PTR [ebx+88]
	cmp	edx, 536870911				; 1fffffffH
	ja	$L1179
	jb	SHORT $L1251
	cmp	eax, -1
	ja	$L1179
$L1251:
	or	ecx, -1
	sub	ecx, eax
	mov	eax, 536870911				; 1fffffffH
	sbb	eax, edx
	xor	edx, edx
	cmp	edx, eax
	ja	$L1179
	jb	SHORT $L1252
	cmp	edi, ecx
	ja	SHORT $L1179
$L1252:

; 25   :  if(stop&&*stop)return terminal(c,PACC_HASH_STOPPED);

	test	ebp, ebp
	je	SHORT $L1249
	cmp	DWORD PTR [ebp], 0
	je	SHORT $L1249
	push	3
	push	ebx
	call	_terminal
	add	esp, 8
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 31   : }

	ret	0
$L1249:

; 26   :  while(n){k=n;if(k>PACC_STORAGE_HASH_CHUNK)k=PACC_STORAGE_HASH_CHUNK;

	test	edi, edi
	je	SHORT $L1183

; 27   :   br_sha256_update(&c->hash,p,k);p+=k;n-=k;

	add	ebx, 16					; 00000010H
$L1182:

; 26   :  while(n){k=n;if(k>PACC_STORAGE_HASH_CHUNK)k=PACC_STORAGE_HASH_CHUNK;

	cmp	edi, 1024				; 00000400H
	mov	esi, edi
	jbe	SHORT $L1184
	mov	esi, 1024				; 00000400H
$L1184:

; 27   :   br_sha256_update(&c->hash,p,k);p+=k;n-=k;

	mov	eax, DWORD PTR _p$[esp+12]
	push	esi
	push	eax
	push	ebx
	call	_br_sha224_update
	mov	edx, DWORD PTR _p$[esp+24]
	add	esp, 12					; 0000000cH
	add	edx, esi
	sub	edi, esi

; 28   :   if(stop&&*stop)return terminal(c,PACC_HASH_STOPPED);

	test	ebp, ebp
	mov	DWORD PTR _p$[esp+12], edx
	je	SHORT $L1185
	cmp	DWORD PTR [ebp], 0
	jne	SHORT $L1250
$L1185:

; 26   :  while(n){k=n;if(k>PACC_STORAGE_HASH_CHUNK)k=PACC_STORAGE_HASH_CHUNK;

	test	edi, edi
	jne	SHORT $L1182

; 28   :   if(stop&&*stop)return terminal(c,PACC_HASH_STOPPED);

	mov	ebx, DWORD PTR _c$[esp+12]
$L1183:
	pop	edi
	pop	esi

; 29   :  }
; 30   :  c->busy=0;return PACC_HASH_OK;

	mov	DWORD PTR [ebx+8], 0
	pop	ebp
	xor	eax, eax
	pop	ebx

; 31   : }

	ret	0
$L1250:

; 28   :   if(stop&&*stop)return terminal(c,PACC_HASH_STOPPED);

	mov	ecx, DWORD PTR _c$[esp+12]
	push	3
	push	ecx
	call	_terminal
	add	esp, 8
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 31   : }

	ret	0
$L1179:

; 24   :  if(c->hash.count>maxbytes||(uint64_t)n>maxbytes-c->hash.count)return terminal(c,PACC_HASH_LIMIT);

	push	4
	push	ebx
	call	_terminal
	add	esp, 8
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 31   : }

	ret	0
$L1175:

; 23   :  if((stop&&!span((const void *)stop,sizeof(*stop)))||!span(data,n)||overlap(data,n,c,sizeof(*c)))return terminal(c,PACC_HASH_INVALID);

	push	1
	push	ebx
	call	_terminal
	add	esp, 8
$L1165:
	pop	edi
	pop	esi
	pop	ebp
	pop	ebx

; 31   : }

	ret	0
_pacc_storage_sha_update ENDP
_TEXT	ENDS
;	COMDAT _overlap
_TEXT	SEGMENT
_p$ = 8
_n$ = 12
_q$ = 16
_m$ = 20
_overlap PROC NEAR					; COMDAT

; 7    : {size_t a=(size_t)p,b=(size_t)q;return n&&m&&(a<=b?b-a<n:a-b<m);}

	push	esi
	mov	esi, DWORD PTR _n$[esp]
	test	esi, esi
	je	SHORT $L1257
	mov	edx, DWORD PTR _m$[esp]
	test	edx, edx
	je	SHORT $L1257
	mov	eax, DWORD PTR _p$[esp]
	mov	ecx, DWORD PTR _q$[esp]
	cmp	eax, ecx
	ja	SHORT $L1258
	sub	ecx, eax
	cmp	ecx, esi
	jmp	SHORT $L1261
$L1258:
	sub	eax, ecx
	cmp	eax, edx
$L1261:
	sbb	eax, eax
	neg	eax
	test	eax, eax
	je	SHORT $L1257
	mov	eax, 1
	pop	esi
	ret	0
$L1257:
	xor	eax, eax
	pop	esi
	ret	0
_overlap ENDP
_TEXT	ENDS
;	COMDAT _terminal
_TEXT	SEGMENT
_c$ = 8
_r$ = 12
_terminal PROC NEAR					; COMDAT

; 11   : {clear_hash(c);c->state=r==PACC_HASH_STOPPED?PACC_HASH_CANCELLED:PACC_HASH_FAILED;c->busy=0;return r;}

	push	esi
	mov	esi, DWORD PTR _c$[esp]
	push	esi
	call	_clear_hash
	mov	eax, DWORD PTR _r$[esp+4]
	add	esp, 4
	xor	ecx, ecx
	cmp	eax, 3
	sete	cl
	add	ecx, 3
	mov	DWORD PTR [esi+8], 0
	mov	DWORD PTR [esi+4], ecx
	pop	esi
	ret	0
_terminal ENDP
_TEXT	ENDS
;	COMDAT _clear_hash
_TEXT	SEGMENT
_c$ = 8
_clear_hash PROC NEAR					; COMDAT

; 9    : {volatile unsigned char *p=(volatile unsigned char *)&c->hash;size_t n=sizeof(c->hash);while(n--)*p++=0;}

	mov	eax, DWORD PTR _c$[esp-4]
	mov	ecx, 112				; 00000070H
	add	eax, 16					; 00000010H
$L1133:
	mov	BYTE PTR [eax], 0
	inc	eax
	dec	ecx
	jne	SHORT $L1133
	ret	0
_clear_hash ENDP
_TEXT	ENDS
;	COMDAT _enter
_TEXT	SEGMENT
_c$ = 8
_enter	PROC NEAR					; COMDAT

; 13   : {if(!span(c,sizeof(*c)))return PACC_HASH_INVALID;if(c->version!=1U||c->state!=PACC_HASH_ACTIVE)return PACC_HASH_STATE;

	push	esi
	mov	esi, DWORD PTR _c$[esp]
	push	128					; 00000080H
	push	esi
	call	_span
	add	esp, 8
	test	eax, eax
	mov	eax, 1
	je	SHORT $L1144
	cmp	DWORD PTR [esi], eax
	jne	SHORT $L1147
	cmp	DWORD PTR [esi+4], eax
	jne	SHORT $L1147

; 14   :  if(c->busy)return PACC_HASH_BUSY;c->busy=1;return PACC_HASH_OK;}

	mov	ecx, DWORD PTR [esi+8]
	test	ecx, ecx
	je	SHORT $L1148
	mov	eax, 5
	pop	esi
	ret	0
$L1148:
	mov	DWORD PTR [esi+8], eax
	xor	eax, eax
	pop	esi
	ret	0
$L1147:

; 13   : {if(!span(c,sizeof(*c)))return PACC_HASH_INVALID;if(c->version!=1U||c->state!=PACC_HASH_ACTIVE)return PACC_HASH_STATE;

	mov	eax, 2
$L1144:
	pop	esi

; 14   :  if(c->busy)return PACC_HASH_BUSY;c->busy=1;return PACC_HASH_OK;}

	ret	0
_enter	ENDP
_TEXT	ENDS
PUBLIC	_pacc_storage_sha_final
EXTRN	_pacc_hash_memcpy:NEAR
EXTRN	_br_sha256_out:NEAR
;	COMDAT _pacc_storage_sha_final
_TEXT	SEGMENT
_c$ = 8
_out$ = 12
_cap$ = 16
_stop$ = 20
_staged$ = -32
_pacc_storage_sha_final PROC NEAR			; COMDAT

; 33   : {

	sub	esp, 32					; 00000020H
	push	ebp
	push	esi

; 34   :  unsigned char staged[32];int r=enter(c);unsigned i;

	mov	esi, DWORD PTR _c$[esp+36]
	push	edi
	push	esi
	call	_enter
	add	esp, 4

; 35   :  (void)sc;if(r)return r;

	test	eax, eax
	jne	$L1196

; 36   :  if((stop&&!span((const void *)stop,sizeof(*stop)))||cap<32U||!span(out,32U)||overlap(out,32U,c,sizeof(*c)))return terminal(c,PACC_HASH_INVALID);

	mov	edi, DWORD PTR _stop$[esp+40]
	test	edi, edi
	je	SHORT $L1205
	push	4
	push	edi
	call	_span
	add	esp, 8
	test	eax, eax
	je	$L1204
$L1205:
	cmp	DWORD PTR _cap$[esp+40], 32		; 00000020H
	jb	$L1204
	mov	ebp, DWORD PTR _out$[esp+40]
	push	32					; 00000020H
	push	ebp
	call	_span
	add	esp, 8
	test	eax, eax
	je	$L1204
	push	128					; 00000080H
	push	esi
	push	32					; 00000020H
	push	ebp
	call	_overlap
	add	esp, 16					; 00000010H
	test	eax, eax
	jne	$L1204

; 37   :  if(stop&&*stop)return terminal(c,PACC_HASH_STOPPED);

	test	edi, edi
	je	SHORT $L1206
	cmp	DWORD PTR [edi], 0
	je	SHORT $L1206
	push	3
	push	esi
	call	_terminal
	add	esp, 8
	pop	edi
	pop	esi
	pop	ebp

; 41   : }

	add	esp, 32					; 00000020H
	ret	0
$L1206:

; 38   :  br_sha256_out(&c->hash,staged);

	lea	eax, DWORD PTR _staged$[esp+44]
	lea	ecx, DWORD PTR [esi+16]
	push	eax
	push	ecx
	call	_br_sha256_out
	add	esp, 8

; 39   :  if(stop&&*stop){volatile unsigned char *v=staged;for(i=0;i<32U;i++)v[i]=0;return terminal(c,PACC_HASH_STOPPED);}

	test	edi, edi
	je	SHORT $L1207
	cmp	DWORD PTR [edi], 0
	je	SHORT $L1207
	xor	eax, eax
$L1209:
	mov	BYTE PTR _staged$[esp+eax+44], 0
	inc	eax
	cmp	eax, 32					; 00000020H
	jb	SHORT $L1209
	push	3
	push	esi
	call	_terminal
	add	esp, 8
	pop	edi
	pop	esi
	pop	ebp

; 41   : }

	add	esp, 32					; 00000020H
	ret	0
$L1207:

; 40   :  memcpy(out,staged,32U);clear_hash(c);c->state=PACC_HASH_DONE;c->busy=0;return PACC_HASH_OK;

	lea	edx, DWORD PTR _staged$[esp+44]
	push	32					; 00000020H
	push	edx
	push	ebp
	call	_pacc_hash_memcpy
	push	esi
	call	_clear_hash
	add	esp, 16					; 00000010H
	mov	DWORD PTR [esi+4], 2
	mov	DWORD PTR [esi+8], 0
	xor	eax, eax
	pop	edi
	pop	esi
	pop	ebp

; 41   : }

	add	esp, 32					; 00000020H
	ret	0
$L1204:

; 36   :  if((stop&&!span((const void *)stop,sizeof(*stop)))||cap<32U||!span(out,32U)||overlap(out,32U,c,sizeof(*c)))return terminal(c,PACC_HASH_INVALID);

	push	1
	push	esi
	call	_terminal
	add	esp, 8
$L1196:
	pop	edi
	pop	esi
	pop	ebp

; 41   : }

	add	esp, 32					; 00000020H
	ret	0
_pacc_storage_sha_final ENDP
_TEXT	ENDS
PUBLIC	_pacc_storage_hash_callback
EXTRN	_pacc_hash_memset:NEAR
;	COMDAT _pacc_storage_hash_callback
_TEXT	SEGMENT
_ctx$ = 8
_p$ = 12
_n$ = 16
_out$ = 20
_c$ = -128
_pacc_storage_hash_callback PROC NEAR			; COMDAT

; 43   : {

	sub	esp, 128				; 00000080H

; 44   :  PACC_STORAGE_SHA256 c;PACC_STORAGE_HASH_CALL *call=ctx;int r;
; 45   :  memset(&c,0,sizeof(c));r=pacc_storage_sha_init(&c);

	lea	eax, DWORD PTR _c$[esp+128]
	push	esi
	push	128					; 00000080H
	push	0
	push	eax
	call	_pacc_hash_memset
	lea	ecx, DWORD PTR _c$[esp+144]
	push	ecx
	call	_pacc_storage_sha_init
	add	esp, 16					; 00000010H

; 46   :  if(!r)r=pacc_storage_sha_update(&c,p,n,call?call->stop:NULL,call?call->context:NULL);

	test	eax, eax
	jne	SHORT $L1227
	mov	esi, DWORD PTR _ctx$[esp+128]
	test	esi, esi
	je	SHORT $L1277
	mov	ecx, DWORD PTR [esi+4]
	mov	eax, DWORD PTR [esi]
	jmp	SHORT $L1280
$L1277:
	xor	ecx, ecx
	xor	eax, eax
$L1280:
	mov	edx, DWORD PTR _n$[esp+128]
	push	ecx
	push	eax
	mov	eax, DWORD PTR _p$[esp+136]
	push	edx
	lea	ecx, DWORD PTR _c$[esp+144]
	push	eax
	push	ecx
	call	_pacc_storage_sha_update
	add	esp, 20					; 00000014H

; 47   :  if(!r)r=pacc_storage_sha_final(&c,out,32U,call?call->stop:NULL,call?call->context:NULL);

	test	eax, eax
	jne	SHORT $L1227
	test	esi, esi
	je	SHORT $L1281
	mov	eax, DWORD PTR [esi+4]
	mov	esi, DWORD PTR [esi]
	jmp	SHORT $L1284
$L1281:
	xor	eax, eax
	xor	esi, esi
$L1284:
	mov	edx, DWORD PTR _out$[esp+128]
	push	eax
	push	esi
	push	32					; 00000020H
	lea	eax, DWORD PTR _c$[esp+144]
	push	edx
	push	eax
	call	_pacc_storage_sha_final
	add	esp, 20					; 00000014H
$L1227:

; 48   :  return r==PACC_HASH_OK;

	xor	ecx, ecx
	pop	esi
	test	eax, eax
	sete	cl
	mov	eax, ecx

; 49   : }

	add	esp, 128				; 00000080H
	ret	0
_pacc_storage_hash_callback ENDP
_TEXT	ENDS
END
