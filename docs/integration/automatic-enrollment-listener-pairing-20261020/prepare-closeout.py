from pathlib import Path
import json,hashlib,difflib
r=Path.cwd();d=r/'docs/integration/automatic-enrollment-listener-pairing-20261020'
proof='''# Prova delimitada e handoff

## Estado factual

HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a, main. O patch de oito arquivos foi submetido ao executor e aprovado antes da aplicação; source-operations.json registra preimagens e hashes. baseline-verification.json verifica 645 arquivos anteriores: somente CMakeLists.txt mudou, por acréscimo aprovado. Nenhum formato persistente, fonte de produção, Browser, PST ou wire operacional mudou.

## Implementação por função e lifetime

- ep_frame_encode/decode: PAE1/v1.0, oito tipos, direções, tamanhos, UTF-8 e campos reservados. Decode retorna uma view emprestada do buffer do caller, sem autoridade.
- ep_reader_push: montagem incremental limitada a 16400 bytes, deadline absoluto de frame 5s, sem pipeline oculto. Caller mantém RX estável enquanto houver view; reader pronto não pode ser reutilizado. Stop invalida sem liberar storage.
- ep_trace_observe: sequência/correlação, até oito frames e 32768 bytes por direção; OPEN/CLOSED e AUTO/MANUAL representam somente estados declarados. APPROVED observado não comprova autorização. CSR é copiado para 4096 bytes próprios, mantidos em Stop; nenhuma emissão é chamada. CSR shape não comprova assinatura/PoP.
- ep_pair_prepare_data/claim_data: dados próprios, código Crockford de 50 bits, TTL 300s, colisões negadas, comparação constante existente, uma submissão/claim por conexão/geração local. Não equivale a PAIRING_CONSUMED, policy, Principal ou autorização. Owner futuro mantém contextos inválidos/retidos e limpa segredos somente após quiescência factual.
- ep_pair_generate_windows: BCryptGenRandom para código/correlação/nonce, scratch 55 bytes limpo com SecureZeroMemory. Prova nativa restrita ao RNG, sem chave ou certificado.
- ep_limiter_admit: seis admissões globais/minuto, três por origem/minuto, dezesseis entradas, dois lifetimes inclusive retidos. ep_origin_set normaliza IPv4-mapped. Caller deve usar origem canônica da PAL, nunca headers remotos. ep_limiter_release_data contabiliza ticket local após fechamento factual pelo owner futuro; não fecha handles nem devolve créditos nativos. Stop não libera tickets.
- ep_tls_profile_prepare: projeção imutável de configuração PST com credencial emprestada, TLS1.3/server-only/papacc-enroll/1. Caller mantém credencial, lease e endereço do perfil durante todo uso/retenção. Nenhuma chamada PST ou listener foi ativado. Teste do builder operacional mantém certificado de cliente REQUIRED e papacc/1.
- ep_enrollment_quote: soma conservadora, UNKNOWN nega; não reserva créditos ou qualifica memória nativa.

## Limites da entrega e gates

O propósito ENROLLMENT ainda não tem representação persistente versionada aceita no modelo/root. Não usar enum secure/legacy nem bytes reservados. Runtime singular não pode truncar listeners[]. Ligação produtiva fica DENY. Fonte factual de CURRENT, identidade do servidor e autorização administrativa não estão disponíveis; provider CLOSED permanece PENDING. Não se cria autoridade sintética para suprir essas fontes.

Ainda faltam limites globais P4 de geração 2/min e submissão 10/min, aplicação dos deadlines de conexão/handshake/primeira mensagem/escrita/vida total pelo owner nativo, continuidade factual de aprovação/consumo/CSR até issuer e aquisição supervisionada dos créditos. O limiter atual implementa somente T6 pré-TLS. Não registrar cobertura integral de P0–P9 ou T0–T7.

## Validação

Quatro configurações: Selected e Legacy, Debug e Release, MSVC /W4 /WX, dez suítes por configuração, zero warnings. Logs preservados e validation-results.json. São 1369 assertions DATA_MODEL e 13 PROFILE_DATA por configuração; CSR/frames/approval são fixtures sintéticas sem autoridade. BCrypt RNG é a única evidência nativa nova. Não há handshake, socket, rede, assinatura privada ou prova positiva CURRENT. NT4/VC6, box e ISA não foram qualificados.

## ED11

ed11-census.json separa tamanhos compilados de reserva operacional. Delta fixo no servidor zero: bibliotecas opt-in não ligadas a papacc_server. Exemplo de dois conjuntos reader/trace/pair/perfil, RX/TX e limiter: 76280 bytes; RNG scratch 55 bytes. Essa soma NÃO inclui toda stack transitiva, buffers de encode/decode adicionais, PST/handshake, issuer, retenção criptográfica e custos opacos. Não é pico nem reserva aprovada.

Subteto enrollment 262144 permanece dentro do limite selecionado. Cenário normativo base Windows 2103847 + subteto = 2365991, margem nominal 779737; Legacy 2067655 + subteto = 2329799, excede 2097152 em 232647: ativação negada. UNKNOWN de provider/kernel e composição de ativação impedem qualificação total. Quotas individuais e seleção explícita Windows3MiB preservadas.

## Handoff Browser e integração

Browser mantém chave automática exclusivamente local. Confirmar identidade do servidor por canal independente autenticado antes dos segredos; código não autentica servidor. Usar endpoint ENROLLMENT separado, TLS1.3 e ALPN exato; rejeitar downgrade/papacc/1 e mensagens operacionais. Mesma conexão/owner/geração/request/nonces até CSR; sem resume por token. CLOSED exige provider factual; código, autenticação, aprovação e emissão são gates distintos. Cancelamento/Stop não provam cancelamento de I/O ou liberação de recursos. Não modificar Browser nesta entrega.

Próximo patch depende primeiro do contrato persistente do propósito, owner/runtime de coleção e censo completo. Qualificação positiva externa permanece necessária. CURRENT, assinatura, recuperação e admissão continuam DENY.
'''
(d/'implementation-proof-and-browser-handoff.md').write_text(proof,encoding='utf-8')
close='''# Closeout — implementação parcial offline

Implementados helpers privados de framing, código/claim, limiter pré-TLS e perfil PST candidato. O executor aprovou os oito arquivos exatos antes da aplicação. Não foi registrado listener no servidor: o propósito persistente e o binding factual do owner permanecem gates. Ver [prova e pendências](implementation-proof-and-browser-handoff.md), [validação](validation-results.json), [ED11](ed11-census.json) e [preimagens](source-operations.json).

```ini
CURRENT_TASK_COMPLETED=NO
ENROLLMENT_LISTENER=OFFLINE_PST_PROFILE_ONLY_NOT_REGISTERED
ENROLLMENT_ALPN=papacc-enroll/1
PAIRING_CODE=OFFLINE_DATA_AND_NATIVE_CSPRNG
CLAIM_ONCE=LOCAL_DATA_GUARDS_TESTED
OPEN_CLOSED=DATA_ORDER_ONLY_PROVIDER_PENDING
APPROVAL_WORKFLOW=FACTUAL_SOURCES_UNBOUND_DENY
CSR_CONTINUITY=OWNED_DATA_COPY_ONLY
BUILDS=PASS_SELECTED_LEGACY_DEBUG_RELEASE
WARNINGS=0
OFFLINE_TESTS=10_OF_10_PER_CONFIGURATION
NETWORK_TESTS=NOT_RUN
AUTOMATIC_ENROLLMENT=DENY_UNTIL_QUALIFIED
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=NO_ACCEPTED_PERSISTENT_ENROLLMENT_PURPOSE_LAYOUT
NEXT=VERSIONED_LISTENER_PURPOSE_AND_CANONICAL_OWNER_INTEGRATION
```

Limites globais P4 e deadlines nativos completos também permanecem pendentes; os helpers não constituem implementação integral dos contratos. Nenhum commit, push, tag, release, chave, certificado ou publicação foi executado.
'''
(d/'closeout.md').write_text(close,encoding='utf-8')
p=r/'docs/capabilities/client-pairing.md';old=p.read_bytes();(d/'preimages/client-pairing.md').write_bytes(old)
addition='''\n\n### Implementação parcial offline de enrollment — 2026-10-20\n\nBibliotecas privadas opt-in implementam projeção do perfil PST ENROLLMENT, framing PAE1, trace de dados, CSPRNG Windows para código de 50 bits, claim único local e limiter pré-TLS. Não estão ligadas ao servidor nem registram listener. APPROVED/claim não constituem autorização ou emissão; operacional mantém mTLS/papacc/1. Selected/Legacy Debug/Release passaram 10/10 suítes por configuração, zero warnings; prova nativa nova limitada ao RNG. Permanecem gates de propósito persistente, owner/runtime de coleção, fontes factuais, provider CLOSED, limites globais P4 e deadlines nativos. Delta fixo no servidor zero, pico de ativação UNKNOWN; ED11 PARTIAL. CURRENT/assinatura/admissão DENY. Ver [closeout](../integration/automatic-enrollment-listener-pairing-20261020/closeout.md) e [prova/handoff](../integration/automatic-enrollment-listener-pairing-20261020/implementation-proof-and-browser-handoff.md).\n'''
new=old+addition.replace('\n','\r\n').encode();(d/'staged/client-pairing.md').write_bytes(new)
sha=lambda x:hashlib.sha256(x).hexdigest()
(d/'capability-operation.json').write_text(json.dumps({'preimage':sha(old),'after':sha(new)},indent=2))
(d/'exact-capability.patch').write_text(''.join(difflib.unified_diff(old.decode().splitlines(True),new.decode().splitlines(True),fromfile='a/docs/capabilities/client-pairing.md',tofile='b/docs/capabilities/client-pairing.md')),encoding='utf-8')
print('Closeout, proof, ED11 and exact capability appendix prepared')
