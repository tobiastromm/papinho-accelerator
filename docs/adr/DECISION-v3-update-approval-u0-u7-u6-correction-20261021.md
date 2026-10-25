---
status: accepted
revision: 1
decision-date: 2026-10-21
scope-level: project
scope-target: canonical dormant V3 administrative UPDATE act and corrected restart semantics
decision-makers: [Tobias Tromm]
supersedes: []
---
# Aprovação administrativa UPDATE — U0-U7 com correção exata U6

## Autoridade e incorporação byte-exata

TASK=PACC-PHASE6-UPDATE-APPROVAL-U6-CORRECTION-ACCEPTANCE-2. O owner autoriza condicionalmente a aceitação da correção exata U6 e de U0-U7 corrigido, mediante conformidade integral, aprovação oficial do executor e verificação pós-gravação. Este registro somente entra em vigor após esses fatos. Não é autorização de implementação ou qualificação operacional.

Aceitar SOMENTE U0-U7 corrigido pelo único hunk em `docs/integration/v3-update-approval-u0-u7-acceptance-20261021/exact-u6-correction-PROPOSED.patch`, SHA256 `c4097f01d294368a078a7c10c101a0ed49d6afae0ff11199cbac138ef9b72f66`, 1742bytes. Fonte original preservada: `docs/integration/v3-candidate-approval-status-producer-20261021/CANDIDATE-APPROVAL-PRODUCER-PROPOSED.md`, SHA256 `6a11985d2889c30d2f6de37e7c5c10cd779ba3326c54c0689042669c5b0d8d4f`, 10468bytes. Hunk reconstruído apenas em memória, sem editar original ou Revision2 PROPOSED. Anexo normativo incorporado [U0-U7-corrected-frozen-clause-bytes.txt](../integration/update-approval-u6-correction-acceptance2-20261021/U0-U7-corrected-frozen-clause-bytes.txt), 9786bytes, SHA256 `4597b5e3ddda96db039025dc0c4eb898e08a1a6fa799e7f4e0d444e39f2842ec`, intervalo[497,10283) da reconstrução corrigida. U0-U5 e U7 permanecem byte-exatos; somente o trecho U6 autorizado é substituído. Frontmatter, introdução e cauda de resultados da proposta não são incorporados. [Intervalos/preimagens](../integration/update-approval-u6-correction-acceptance2-20261021/incorporation-and-preimages.json) fixam a reconstrução e os hashes, sem normalização de encoding ou quebras de linha.

O vocabulário histórico PROPOSED/necessita aceitação nas cláusulas congeladas é superado apenas quanto à aceitação normativa desses bytes e ao escopo de ato administrativo aqui delimitado. Proibições de implementação, publicação, assinatura, confiança, recuperação, admissão e reservas continuam integrais. Propostas originais e STOP R1 anterior permanecem história; não há aceitação de APIs, tipos, hunks funcionais ou artefatos explicativos por referência.

## Precedência delimitada e ato administrativo

Aceitação especializa somente a origem e o vínculo do ato explícito UPDATE do administrador local Windows no owner/cohort/policy/issuer/root existentes. U1 identifica o papel apto a praticar U2; autenticação/token/grupo isolados não aprovam a candidata. O ato U2 exige a operação e imagem completa imutável, predecessor/pin/revisão e todas as tuplas SNT/PDG/issuer/policy pertinentes, com status, validade e revalidação. Bootstrap e C8 não são convertidos em autorização UPDATE. Não cria provider geral, registry, journal, worker, resolver, mecanismo de pins ou autoridade paralela. Outros targets não herdam o perfil Windows.

D2/D3/D4, S1-S5, F1-F5/G0-G7, E0-E8, issuer/policy/root/C8, Model E e ED08/ED11 permanecem autoridades. UPDATE v3 continua exclusivamente sobre base v2/CP2 ou v3/CP3 factual completa, com CURRENT real, predecessor, pin/lifetime e CAS. Bootstrap v1 e recuperação mantêm seus contratos separados; não se autoriza bootstrap v3 ou migração automática. PACROOT8/v3, PDG1, CP3, SNT1, ROOT08/v2, ROOT06/BODY0.5, D3/LST1 e todos os formatos/hash domains históricos permanecem byte-exatos.

## Persistência histórica versus autoridade após restart

A correção encerra somente R1 da revisão anterior. Disposições e histórico podem ser persistidos unicamente pelos campos/transações accepted; prova transitória de origem/ato não é serializada ou reconstruída de PDG. Essa separação não apaga histórico, não exige nova persistência, não recupera aprovação para outra operação e não concede autoridade pós-restart.

Arquivo/checkpoint íntegro é evidência histórica, nunca prova suficiente de CURRENT, freshness, fencing ou autorização vigente. Nenhum restart publica CURRENT, emite epochs, revalida automaticamente um ato ou restaura contexto operacional. Sem fontes e provas qualificadas de recuperação/CURRENT e autorização/revalidação aplicáveis, todos os efeitos dependentes permanecem DENY. AUTHORITATIVE_RECOVERY=DENY; não se cria mecanismo alternativo de restore/fencing. Serial/high-water/revogações/história são preservados, sem fallback para ancestral.

## Status, lifecycle e recursos

Certificado PUBLISHED não delega PACKAGE_SIGNING; status/validade/revogação do certificado e da delegação são separados. UTC precisa fonte issuer aprovada e qualificada, não relógio/campo do caller por inferência. Ausência, expiração, revisão divergente, incerteza ou fonte não qualificada nega. Este registro não estabelece CURRENT factual, ambiente administrativo elevado ou UTC operacional.

Stop continua não bloqueante e impede efeitos, sem presumir cancelamento/fechamento. Owner, borrows, pins, fontes externas, ambas gerações e créditos permanecem até quiescência e liberação factuais; timeout não libera. CONFLICT/UNKNOWN não são sucesso, rollback presumido ou autorização de retry destrutivo. E0 mantém acquire síncrono borrow-only e RELEASE_INVALID sem tentativa/efeito. M1 não executa KDF antes de confiança factual; Browser continua responsável por confirmação independente e autorização local.

Custo runtime desta aceitação=0. Nenhuma quota/reserva é ampliada ou reduzida. Windows3145728 somente opt-in explícito; Legacy2097152 e quotas individuais intactos. Censo fixo anterior1948656/1790872 não é pico/credito; cenário completo3747601 excede3MiB em601873, sem autorização de ativação. Prova/handles/fontes/arenas/snapshots/caller/stack/coexistência/retidos exigem censo compilado e lifetime antes de hunks. Provider/kernel/stack transitivo UNKNOWN, ED11 totalPARTIAL. Contrato aceito não qualifica capacidade, implementação, target ou admissão.

## Resultado e limites

UPDATE_APPROVAL_CONTRACT=ACCEPTED
U6_RESTART_SEMANTICS=NO_AUTOMATIC_OPERATIONAL_AUTHORITY_OR_ACT_RESTORATION
HISTORICAL_PERSISTENCE=PER_ACCEPTED_FORMATS_ONLY
OPERATIONAL_AUTHORITY_AFTER_RESTART=DENY
AUTHORITATIVE_RECOVERY=DENY
IMPLEMENTATION_AUTHORIZED=NO
V3_COMMIT=DENY
CURRENT_PUBLICATION=DENY
E0_E8_FACTUAL_SOURCE=DENY
M1_TRUST=DENY
PAPACC_INSTALL=DENY
AUTOMATIC_ENROLLMENT=DENY
SIGNATURE_AUTHORITY=DENY
ED11_TOTAL_STATUS=PARTIAL
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION

Sem fontes funcionais, builds/testes/rede, chaves/certificados, publicação CURRENT/assinatura, clone/worktree ou commit/push/tag/release. Preservar WIP, alteração externa do .gitignore Browser e quatro LAB. Implementação do produtor requer tarefa própria, hunks/preimagens aprovados oficialmente e provas offline/nativas separadas. [Conformidade](../integration/update-approval-u6-correction-acceptance2-20261021/conformance-matrix.md) · [Closeout](../integration/update-approval-u6-correction-acceptance2-20261021/closeout.md).
