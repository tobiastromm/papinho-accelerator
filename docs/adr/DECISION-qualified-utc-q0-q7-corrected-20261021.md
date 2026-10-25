---
status: accepted
revision: 1
decision-date: 2026-10-21
scope-level: project
scope-target: qualified UTC observation and issuer UPDATE status normative contract
decision-makers: [Tobias Tromm]
supersedes: []
---
# UTC qualificada — Q0-Q7 com correções exatas Q2/Q3

## Autoridade e incorporação byte-exata

TASK=PACC-PHASE6-QUALIFIED-UTC-Q2-Q3-CORRECTION-ACCEPTANCE-2. Owner autoriza condicionalmente a aceitação da correção exata e de Q0-Q7 corrigido, mediante conformidade integral, aprovação oficial do executor e pós-hashes. Este registro entra em vigor somente depois desses fatos. Aceitação normativa, implementação, qualificação da fonte e ativação operacional permanecem decisões separadas.

Aceitar SOMENTE as cláusulas Q0-Q7 corrigidas pelo patch `docs/integration/qualified-utc-status-q0-q7-acceptance-20261021/exact-q1-q3-corrections-PROPOSED.patch`, SHA256 `f9fa559afd4438f1ac4969d1ea4b5411f0114f35a38827228365e86a45822a11`, 7771bytes, três hunks. Q1 recebe apenas a preservação dos parâmetros rho/E exigidos pela correção Q2/Q3; não há nova fonte, aprovação ou formato implícito. Fonte original preservada `docs/integration/qualified-issuer-utc-status-binding-20261021/UTC-STATUS-SOURCE-CONTRACT-PROPOSED.md`, SHA256 `44b1bedb55047b92feea828eb188d2161ba49610a1d687dc478c741bc22d4d3b`, 12719bytes. Patch reconstruído somente em memória; nenhuma edição da proposta original ou Revision2 PROPOSED.

Incorporação pelo anexo normativo [Q0-Q7-corrected-frozen-clause-bytes.txt](../integration/qualified-utc-q2-q3-correction-acceptance2-20261021/Q0-Q7-corrected-frozen-clause-bytes.txt), 13076bytes, SHA256 `82c97ae14c82622b86095b701f0af9de191cfe6b39e371e63501dfe02f26b746`, intervalo[702,13778) da reconstrução corrigida. Q0/Q4/Q5/Q6/Q7 permanecem byte-exatos. Fontes, intervalos e três substituições em [incorporation-and-preimages.json](../integration/qualified-utc-q2-q3-correction-acceptance2-20261021/incorporation-and-preimages.json). Não normalizar encoding/quebras de linha. Frontmatter, introdução e cauda de resultados da proposta são excluídos; explicações/handoff e patches funcionais não são aceitos por referência.

Palavras históricas PROPOSED/necessita aceitação nas cláusulas congeladas são superadas somente quanto ao status normativo dos bytes incorporados. Todas as proibições de implementação, publicação, confiança, recuperação, reserva/admissão, fornecedor externo ou serviço novo continuam válidas. Originais e STOP anterior permanecem história; nenhuma aceitação automática de futuras revisões.

## Correções e precedência delimitada

R1 e R2 da revisão anterior ficam resolvidos SOMENTE no contrato. rho=rho_num/rho_den,0<=rho_num<rho_den e rho_den>0, limita avanço do UTC real por unidade PAL, não a razão inversa. Para qualquer par de cortes coberto, com d>=0 medido PAL e delta>=0 UTC decorrido, a qualificação deverá demonstrar max(0,(1-rho)*d-E)<=delta<=(1+rho)*d+E. E é limite aditivo factual/qualificado em ns para resolução/quantização/leitura/conversão dos endpoints PAL não cobertas pela taxa, inclusive d=0. Não deduzir E=0 da unidade de API; orientação inversa requer conversão conservadora demonstrada.

Q3 define a=floor(d*(rho_den-rho_num)/rho_den), b=ceil(d*(rho_den+rho_num)/rho_den), dl=max(0,a-E), du=b+E, Lm=L+dl e Um=U+du. O max0 é limite físico inferior de tempo decorrido, implementado sem underflow, não saturação permissiva. Floor inferior/ceil superior conservam inclusão do UTC real sob a desigualdade qualificada. Toda conversão/soma/multiplicação/divisão requer checagem; overflow, limites desconhecidos ou origem/vigência ausentes negam. Incerteza inicial UTC e duração PAL não são deduzidas uma da outra sem prova. As fórmulas do anexo, não valores aproximados ou exemplos, são normativas.

A observação permanece restrita à mesma operação/fronteira, fonte e owner; exigir m0<=m1<=m<D e qualidade/contexto/pin CURRENT ainda aplicáveis. D é término exclusivo restrito pela qualificação factual, não TTL/grace escolhido pelo caller; consultas não o renovam. rho/E/D não recebem defaults permissivos. Continuidade dos limites deve abranger todo o intervalo até o efeito; suspensão/resume ou alteração de condições sem essa prova invalidam a observação e impedem propagação através do evento.

As condições [notBefore,notAfter) accepted continuam intactas: todo intervalo UTC conservador deve estar dentro delas, sem clock-skew/backdating ou validade adicional. A nova definição não altera formato/assinatura de certificado, pacote, store, backend.utc ou wire. É complemento normativo de ADR-0006/perfil CA1 §1.4 e U4/F2, não substituição de U0-U7, F1-F5/G0-G7, E0-E8, root/C8, Model E ou ED08/ED11.

## Fonte, status e autoridade

UTC deve vir do backend administrativo issuer já existente, com origem/instância/integridade/qualidade/erro/vigência factuais. O PAL existente mede duração, não cria UTC. Relógio local, callback escalar, configuração textual, documento, hash ou hora plausível isolados não qualificam uma fonte. A fonte OS Windows é somente candidato dependente de qualificação; não presumir Windows Time/sincronização/autenticação externos. Não criar relógio, serviço, peer/protocolo ou autoridade de tempo paralelos.

Status issuer deve vir da root CURRENT/pin com identidade/revisão/lifetime e histórico aplicáveis; certificado e delegação têm validade/revogação/propósito separados. PUBLISHED, X509 válido ou PDG APPROVED não aprovam o UPDATE. UTC qualificada não concede ato administrativo, grants, TRUST/E0/M1, assinatura ou commit v3. UNKNOWN, rollback, inconsistência, fonte/revisão divergentes e expiração negam, sem fallback favorável. Restart não restaura observação/ato/CURRENT/epoch/fencing; recuperação autoritativa segue DENY.

## Lifecycle, portabilidade e ED11

Manter grafo protegido no mesmo owner e referências externas sob seus owners/cópias contabilizadas. E0 acquire continua síncrono borrow-only, sem aquisição nativa/I/O ou lease fabricado. Stop não bloqueia por clock/provider, não prova cancelamento e não libera retidos. Quiescência/fechamento factuais e Model E/exactly-once regem liberação/crédito; RELEASE_INVALID permanece sem tentativa/efeito. Falha UTC não é destruição.

Contrato comum usa dados/unidades e aritmética C89 sem Win32/CNG/SID obrigatórios. Conversão FILETIME pertence ao adaptador Windows; box/ISA e outros targets exigem equivalência/qualificação próprias. Nenhuma plataforma/PST é qualificada por aceitação textual; não se modifica PST ou PACC wire.

Custo runtime desta decisão=0. Estruturas/observações anterior-nova, rho/E/D/proveniência, snapshots/pins, DER/workspaces/buffers/caller/stack/handles e retidos precisam censo compilado/lifetime antes de hunks. Não aumentar quotas, omitir memória ou presumir economia. Windows3145728 somente seleção explícita, Legacy2097152 e quotas individuais intactos. Censo fixo anterior1948880/1790872 não é pico/crédito; cenário completo3747825 excede3MiB em602097, sem autorização de ativação. Provider/kernel/stack transitivo UNKNOWN e ED11 totalPARTIAL.

## Resultado e limites

QUALIFIED_UTC_CONTRACT=ACCEPTED
Q2_RHO_DIRECTION=REAL_UTC_ADVANCE_PER_PAL_ADVANCE
Q3_PAL_ERROR=QUALIFIED_ADDITIVE_E_OUTWARD_ROUNDING_CHECKED_ARITHMETIC
UTC_SOURCE=NOT_QUALIFIED_UNTIL_IMPLEMENTED
ROLLBACK_PROTECTION=NORMATIVE_INVALIDATION_NOT_IMPLEMENTED_OR_QUALIFIED
IMPLEMENTATION_AUTHORIZED=NO
ISSUER_STATUS_BINDING=DENY
UPDATE_U0_U7_BINDING=DENY
V3_COMMIT=DENY
CURRENT_PUBLICATION=DENY
E0_E8_FACTUAL_SOURCE=DENY
M1_TRUST=DENY
PAPACC_INSTALL=DENY
AUTOMATIC_ENROLLMENT=DENY
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

Nenhum código, build, teste, rede, CA/chave/certificado, relógio/token/ACL, clone/worktree ou commit/push/tag/release é alterado/executado por esta aceitação. WIP dos dois projetos, .gitignore Browser e quatro LAB preservados. Implementação futura depende de autorização própria, hunks exatos aprovados e qualificação factual/nativa separada. [Conformidade](../integration/qualified-utc-q2-q3-correction-acceptance2-20261021/conformance-matrix.md) · [Closeout](../integration/qualified-utc-q2-q3-correction-acceptance2-20261021/closeout.md).
