# Phase 6 — continuação funcional do root/issuer e fanout

Continuar a autorização técnica de PACC-PHASE6-ISSUER-TRUST-ENROLLMENT-FUNCTIONAL-INTEGRATION. Ler CONSOLIDATED-CLOSEOUT.md deste pacote, seu delta e fontes atuais. Não reiniciar auditoria, pedir novamente M3/M7/M0–M8/F/G/P/T, refazer campanhas inalteradas ou retomar TPM por default.

1. Integrar no MESMO root producer nativo de continuidade NORMAL: current/pin original, CA/key/generation, história/serial/high-water e exclusividade reais. Usar papacc_issuer_init_normal e o validador compartilhado core/CNG, nunca bits31 ou dados de fixture como autoridade. Recuperação permanece separada e negada no perfil NORMAL.
2. Resolver a ligação entre issuer_root_port_cas e issuer_prepare_live/begin/poll/publish_live sem bloquear/repetir CAS/assinatura: conservar contexto/inputs/der/pins/quotas e resultado pendente/UNKNOWN; sucesso somente após confirmação da seleção/publicação/epochs e re-read exatos. Qualificar em fixtures novos exclusivos no build, com falhas antes/depois de commit e sem efeitos produtivos.
3. Implementar o port revoke_dependents no root real usando a identidade de credencial exata e os owners de Sessions/operações. Nunca ampliar a outro cliente/credencial do mesmo Principal por inferência. Sucesso do port não é fechamento: invalidation, Stop solicitado/aceito, quiescência e release são fatos distintos. Falha conserva o grafo e bloqueia, sem retry automático.
4. Avançar E8 apenas quando aquisições/produtores factuais exigidos estiverem demonstrados. Preservar binding SOURCE_ABSENT se faltarem. Não criar fábrica que aceite bool, PDG, digest ou callback arbitrário como trust. Browser deve manter o mesmo M1/store/lifetime; edição fora da raiz permitida precisa de permissão própria para o patch concreto.
5. Não persistir ENROLLMENT até existir representação versionada aceita conforme L2. Preparar somente a extensão exata necessária no modelo/codec existente, sem nova família de ADRs ou reaprovar o protocolo inteiro; continuar outras frentes se esse gate permanecer pendente.
6. Manter D4/D5 UNKNOWN onde não fundamentado; reunir evidência sem instalar setters/quotas ou copiar LAB. Preservar NSS/x86, OpenSSL/x64, Selected/Legacy e API2/API3/R2.

Rodar builds/regressões afetados pela continuação. Registrar mudanças contra preimagens do WIP atual e atualizar capability/matriz com escopos precisos. Produzir relatório único de integração efetiva e lacunas restantes.

Não executar operação TPM/NV/ownerAuth/Q4, serviço real, rede operacional/nova admissão, bootstrap/root/CURRENT produtivos, SCM/ACL, cleanup/refund, assinatura/recuperação autoritativas ou publicação PST. Preservar reservas, UNKNOWN, HEAD/WIP/históricos/pin0.6.4. A continuação não é aprovação de layout novo nem de operação real.
