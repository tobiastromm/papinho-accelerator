# Matriz focal de precedência normativa

GOVERNANCE CONFLICT: dispensar backup/restore AGORA contraria os contratos accepted. A solução está apenas PROPOSED em M0–M11, sem aplicação antecipada. Originais e hashes preservados.

| Autoridade/cláusula exata | Menor especialização proposta | Preservado |
| --- | --- | --- |
| CA technical profile20261018,3.5/3.6 e Owner conditions Before PRODUCTIVE CA initialization | M0/M2 adiam imagem/duas gerações/restore pré-inicialização/emissão para pessoal não recuperável | Proteção MACHINE/export0; perda não recuperada, backup não declarado qualificado |
| personal-ca-preinitialization20261022 P1/P2/P5, procedimento A/D/E/F | Não exigir gates B/C/ensaio wbadmin como disponibilidade pessoal; procedimento fica direção recuperável futura | P3/P4, contas distintas e binding factual; nenhum backup executado |
| CA2.4, issuer-C2 P6.1–P6.3 e parágrafo High-water…backup continuam obrigatórios | M3 distingue continuidade NORMAL em armazenamento original de restore; dispensa prova independente de recuperação só para NORMAL sob fault model explícito | Root/CURRENT/chain original/CAS/high-water/serial reservado/exclusividade; clone/rollback/restore e recuperação autoritativa DENY |
| issuer-live-session-coexistence20261019 Recovery remains denied | Preservado; restart normal não restaura Session/snapshot vivo | Slots/pins/retenção e contexto factual |
| internal-client-issuer-and-manual-custody20261018 cláusula6 | Obrigação backup/restore fica futura; init/rotation continuam atos distintos | Sem startup regeneration, overwriteUNKNOWN ou nome como custódia |
| CA1.4/2.1–2.3/3.1–3.4/3.7 | Preservar, adiar só backup de gerações | Propriedades/DACL/token/SPKI/UTC/serial, rotação/história/borrows |
| pessoal F0–F9 e bootstrap P1/P3/P8 | Manter NEW_INSTALL normal; M2 permite sucessor após abandono explícito, não reset automático | Ato/operador/alvo/config/tentativa, exclusivo/consumo único/UNKNOWNretido |
| production-client-provisioning20261018 itens1,3–8; SDK S0–S8; pairing P0–P9/T0–T7 | Não reduzir MANUAL/AUTOMATIC nem refazer protocolo | TLS1.3/mTLS operacional, enrollment separado, PoP/replay/status/root, shared-owner-first |
| issuer8 e issuer-C2 P5.2/R-ACTIVE/Delimited precedence POLICY_PENDING | M7 propõe Stop de Sessions/operações da credencial após revogação publicada, exige aprovação | Bloqueio novas admissões; sem mutar snapshots ou fabricar destruição |
| issuer-C2 P4.2/P4.5/P4.7, codec/status/história persistentes | M8 só oculta view, não remove record | Tombstones/serial/revogação/DER/pins/UNKNOWN; ocultar não libera quota |
| papacc-conf-textual-grammar F1–F6 e comments-acls-listeners N1–N8 | M6 adiciona uma diretiva global, não INI ou codec implícito | UTF8/end/limites8192/96/8/64, desconhecido/unsupported falha, texto não autoriza |
| ADR0006/0012/0028, CURRENT/R/A/P/I/CAS | Preservados | Credential≠Principal≠grant; ocultar≠revogar; sem ampliação de Session |
| Model E/ED08 e ED11 preserve-incremental/Windows3MiB | Preservados integralmente | Stop≠destruição, retenção e custos opacos/host, sem quotas maiores |
| G1–G5 LAB | Não promover ao produtivo | LAB não autoriza identidade real nem remove gate produtivo |

Código concreto: issuer_core.c:view_ok exige PAPACC_ISSUER_ROOT_PROOFS31, incluindo COMPLETE_CHAIN/FENCED_HIGH_WATER. Não retirar check ou preencher bits fictícios. Será necessário contrato exato de provas por propósito e patch revisado; não nesta tarefa. Inspeção DATA do issuer zera epoch/proofs, não supre esses fatos.

M3 não resiste a replay/clones feitos por administrador comprometido. Deployment que exija tal garantia não seleciona perfil simplificado e mantém gates atuais. Tradeoff e M7 são NOVAS escolhas aguardando owner/revisão oficial. Sem troca de biblioteca/wire ou CA LAB. Layout/API para ocultação/retention não aceito por esta matriz.

