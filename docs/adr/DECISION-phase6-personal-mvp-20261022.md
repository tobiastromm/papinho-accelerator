---
status: accepted
revision: 1
decision-date: 2026-10-22
scope-level: project
scope-target: PapinhoAccelerator Phase6 personal Windows MVP
supersedes: []
---
# Decisão — MVP pessoal Phase6 M0–M11

## Autoridade e incorporação delimitada

TASK=PACC-PHASE6-PERSONAL-MVP-M0-M11-ACCEPTANCE-1. O owner aprovou explicitamente M3 e M7, preservou os requisitos do MVP/histórico e autorizou aceitação exata após revisão oficial. Registro vigente somente após aprovação oficial da escrita exata e verificação dos postimages. Não autoriza implementação ou operação.

Fonte preservada [ADR-CANDIDATE-phase6-personal-mvp.md](ADR-CANDIDATE-phase6-personal-mvp.md), revisão1, SHA256 c013eaee6242ec73b17950a483edc1e61e900e3e8f0aedf482bb813a6219508e. Incorporação literal das doze cláusulas: intervalo de bytes UTF8 [507,11067), comprimento 10560, SHA256 1df5ba8cace4edc03a704aceb485b34abdf09b68d3b2c70fe02c98bca08a562b. Bytes também preservados em [M0-M11-frozen.txt](../integration/personal-mvp-m0-m11-acceptance-20261022/M0-M11-frozen.txt). Front matter, introdução e bloco final de estados da proposta não são incorporados.

As decisões explícitas do owner reproduzidas abaixo são condições normativas controladoras de M3/M7. Em eventual leitura ambígua, prevalecem sobre o texto histórico incorporado. Não se aceita whole-matrix/relatório/handoff por referência. [Revisão exata e precedência](../integration/personal-mvp-m0-m11-acceptance-20261022/conformance-review.md).

Somente o status normativo dos trechos incorporados passa a ACCEPTED. Palavras históricas PROPOSTO/PROPOSED, condicionais Se aceita e a exigência de aprovação ainda pendente em M11 registram o estado da fonte e são satisfeitas por este registro posterior. Elas NÃO mantêm a norma aqui aceita pendente nem autorizam execução. Todos os gates factuais, source qualification, hunks, layout/codec/API e ED11 continuam; proposta original e ADRs anteriores ficam byte-exatos.

## M3 — decisão explícita incorporada do owner

ACCEPT:

A primeira versão pessoal permite continuidade operacional normal enquanto a identidade e o estado protegido permanecerem íntegros.

Não oferece recuperação da CA após perda definitiva.

Não oferece garantia contra clonagem externa integral ou rollback externo não detectável.

Em caso de perda definitiva, exige nova identidade de instalação e novo pareamento.

Preservar integralmente as proteções internas accepted contra replay, rollback detectável, duplicação, substituição e reutilização indevida.

Não autorizar recuperação implícita nem promoção de estado antigo.

Não confundir ausência de recuperação com dispensa de consistência administrativa.

Aplicação delimitada: a hipótese local de M3 não permite ignorar inconsistência detectável, resetar high-water, reaproveitar serial/correlação, adotar chave antiga ou preencher provas fictícias. Fluxo normal exige fatos de continuidade e integridade da mesma root; restart não ressuscita autorização/Session. Restore/recuperação/clone não são reclassificados como operação normal. O core atual e suas guards não são alterados; representação das provas por propósito exige revisão funcional própria antes de qualquer emissão.

## M7 — decisão explícita incorporada do owner

ACCEPT:

Após publicação canônica e durável da revogação de um cliente:

- negar novas admissões pela credencial;
- invalidar admissões existentes associadas;
- solicitar Stop das sessões e operações dependentes;
- impedir novos efeitos autorizados pela credencial;
- respeitar quiescência e retenção;
- preservar UNKNOWN;
- não declarar desconexão concluída antes de confirmação factual.

A revogação não depende de o Browser estar online.

Não implementar sincronização bilateral de exclusões nesta versão.

Aplicação delimitada: invalidar admissão é retirar elegibilidade/autorização operacional das admissões associadas à credencial revogada, não destruir seu snapshot imutável ou liberar pins/buffers/leases. Mesmo snapshot retido não autoriza novos efeitos. O escopo factual cliente/credencial da revogação é revalidado; não estender a outra credencial do Principal por inferência. Solicitação de Stop, consumo de bytes, invalidação de autorização, quiescência e fechamento são fatos separados. Propagação/fechamento incerto não permite resposta de desconexão concluída nem novo efeito pela credencial; preservar referências/primeira causa e UNKNOWN sob Model E/ED08. Não se define novo wire, deadline ou mecanismo de teardown aqui.

## Precedência normativa exata no perfil pessoal

1. Especializar CA technical profile20261018,3.5–3.6 e Owner conditions (procedimento/backup/restore pré-inicialização/emissão), e personal-ca-preinitialization20261022 P1/P2/parteP5 e procedimento A/D/E/F, somente para adiar backup completo/restore da identidade como requisito de disponibilidade do MVP pessoal NÃO recuperável. Obrigações de proteção, separação de identidade, emissão, serial, status e rotação/aposentadoria permanecem. Procedimento recuperável anterior e ferramentas continuam candidatos futuros, não desqualificados nem executados. Fora desse perfil não há dispensa.
2. CA2.4, issuer-C2 P6.1–P6.3 e declarações gerais de checkpoint/fencing/backup são especializados apenas quanto à dependência de prova independente de recuperação para continuidade NORMAL do perfil pessoal M3. História original íntegra/validada, CURRENT/predecessor/CAS, reserva durável/serial/high-water, exclusividade e revalidação permanecem. RECOVERY continua DENY, com requisitos de latest-state/fencing intactos para qualquer futura recuperação. Sem continuidade demonstrada, nenhuma assinatura.
3. Custódia issuer cláusula6 adia backup/restore como capacidade pessoal, preservando init/rotation explícitos, correspondência chave/certificado, recusa de startup regeneration, de overwrite e de retries após incerteza. M2/M4 delimitam sucessor de identidade perdida: novo ato/alvo/identidades e novo pareamento, sem adoção de estado antigo ou dispensa de P1/P3/P8 no NEW_INSTALL normal.
4. M7 fecha o gate R-ACTIVE para este perfil: especializa somente o POLICY_PENDING de issuer cláusula8/Delimited precedence, CA-profile parágrafo Active Session revocation remains POLICY_PENDING, issuer-C2 P5.2 e correspondentes declarações de não expansão em issuer-live-session-coexistence e ED11-WindowsW5. Os contratos de memória/snapshot/CAS/epoch/pins/Stop/retenção dessas autoridades não mudam. Não escolher grandfathering ou mutação retrospectiva de grants; reter memória não é manter autorização revogada. Outros perfis não recebem política por inferência.
5. Gramática papacc.conf F1–F6/N1–N8 recebe a extensão semântica M6 de UMA diretiva global revoked_history_retention_days N, ausente/default0. Convenção diretivas/blocos preservada, sem INI ou parser paralelo. Catálogo/layout/projeções/hashing e migrações binárias requerem revisão própria; nenhum formato histórico é reinterpretado por esta aceitação. Retenção visível não apaga registros protegidos nem cria crédito.

Essas são especializações expressamente aprovadas, não conflitos residuais sem solução nem supersession integral. Todos os originais permanecem históricos. Fora dos pontos delimitados continuam vigentes F0–F9/P1, CA1.4/2.1–2.3/3.1–3.4/3.7, issuer, CURRENT/R/A/P/I/CAS, admissão, SDK/provisão, TLS1.3/mTLS operacional, Model E, ED08 e ED11. Nenhuma quota, limiar de host, algoritmo, backend ou obrigação de segurança adicional é dispensada.

## Cláusulas M0–M11 — bytes normativos incorporados

## M0 — Escopo e precedência proposta

Pessoal/local Windows: instalar/configurar, host e lifecycle SCM, CA protegida, MANUAL e AUTOMATIC, Browser primeiro consumidor, cadastro/listagem/revogação/histórico, desinstalação/reinstalação previsíveis e restauração somente de opções não secretas. Dois modos de provisão accepted continuam obrigatórios; MVP não redefine Phase6 como LAB, serviço ou só um modo de pareamento. Restantes obrigações Phase6 não são fechadas por este texto.

Se aceita, esta especialização prevalecerá SOMENTE no perfil pessoal explicitamente selecionado sobre: CA technical profile20261018,3.5–3.6 e Owner conditions que exigem procedimento completo/backup/restore antes de inicialização/emissão; dependência de recuperação/fencing do2.4 aplicada à operação NORMAL; personal-ca-preinitialization20261022 P1/P2 e parte deP5, procedimento incorporado A/D/E/F correspondente. Procedimento recuperável anterior fica futuro, não requisito de disponibilidade pessoal. RECOVERY continua proibida. A matriz correlaciona exatamente essas mudanças, sem supersession integral.

## M1 — Segurança que não é adiada

KSP MACHINE/RSA2048/e65537/export0/signing-only, key/cert/SPKI, CA/TLS/chaves de clientes separados, token admin versus conta operacional local padrão dedicada, ACL mínima e testes factuais. CA_INITIALIZE explícito e distinto na cerimônia F0–F9 quando plano nomeia ambos os efeitos. Sem geração em startup, LAB produtivo, conta privilegiada default, key name como fato ou fallback. Proteger serial/reserva durável ANTES da assinatura, CSR/PoP, finalidade, validade/UTC qualificados, revogação, CURRENT/CAS, exclusividade nativa e retenção UNKNOWN. Não retirar guards para conseguir emitir.

## M2 — Perda irreversível e nova instalação

O MVP não promete backup/restore da identidade antiga nem migração de CA. Informar ANTES da confirmação: perda definitiva da identidade exige outra instalação/CA e novo pareamento de TODOS os clientes; .conf não restaura confiança. Falha de provider/ACL, ausência, corrupção ou UNKNOWN não provam perda definitiva e não autorizam substituir.

Sucessor exige ato administrativo distinto de abandono irreversível, confirmação específica, contenção factual dos emissores antigos conhecidos, recursos antigos retidos/owners respeitados, novo alvo exclusivo e NOVAS identidades root/CA/geração. Não reutilizar nomes/keys/correlações incertas nem sobrescrever instalação antiga. Não transportar antigos Principal/serial/revogação para a nova root. Preservar evidência necessária conforme aposentadoria. Abandono não atesta revogação nos clientes nem destruição de clones; clientes retiram confiança antiga e pareiam explicitamente. Suspeita de comprometimento exige tratamento de incidente, não reset silencioso. Sem novo issuer/journal de recuperação.

## M3 — Operação normal não é recuperação: tradeoff explícito

Fault model PROPOSTO: OS local e administrador legítimo mantêm íntegro o armazenamento ORIGINAL, sem rollback administrativo, clonagem ou restore externo de identidade. Essa hipótese não é provada por hash/DACL/SID. O perfil NÃO garante detecção geral de replay do disco nem fencing global após clone/comprometimento. É redução explícita de garantia a aprovar, não garantia preservada por rótulo.

Sob esse limite, emissão/revogação e restart NORMAL poderão usar história canônica original completa/validada, CURRENT/predecessor, reservas/burned serials/high-water monotônicos, key/policy binding, exclusividade entre processos e revalidação antes do efeito; sem exigir prova independente de backup/restore. Estado incompleto, origem incerta, continuidade contraditória, rollback observado, concorrência de writer, commit/assinatura UNKNOWN ou suspeita de restore continuam DENY e reconciliação query-only. Nenhuma autorização ou Session viva é restaurada do disco; restart reautentica/revalida, nunca gera CA.

Mudança funcional deve separar provas por propósito/perfil no core/adapter. PROIBIDO setar PAPACC_ISSUER_ROOT_PROOFS=31 ou fingir COMPLETE_CHAIN/FENCED_HIGH_WATER de recuperação para satisfazer view_ok. Definir e revisar representação exata das provas de continuidade NORMAL, sem que sirvam para papacc_issuer_recover. RECOVERY/restore de geração antiga permanecem AUTHORITATIVE_RECOVERY=DENY; fencing/high-water independente continuam obrigatórios nesse caso. Sem provas normais suficientes, assinatura permanece DENY. Não é opção de desligar validação.

## M4 — Rotação, aposentadoria e uninstall

Manter CA profile1.4/3.7: rotação explícita da mesma autoridade íntegra/autorizada, nova geração, retenção certs/DER/revogações/serial/história/dependências até quiescência. Adiar só backup/restore da geração; não migrar chave. Irrecuperabilidade segue M2, não rotação automática. Nenhuma eliminação com borrow/worker/UNKNOWN.

Remover serviço/binários não remove CA/CURRENT/root/config/dados. Preservar dados é padrão seguro; reinstalar sobre dados existentes exige validação/fluxo correspondente, nunca inferir NEW_INSTALL. Purge definitivo exige operação distinta especificamente confirmada/autorizada/contida, sem atestar revogação de certificados distribuídos. P1/P3/P8 especializam somente o sucessor explicitamente abandonado M2, não exceção para contornar incerteza. Não apagar LAB.

## M5 — Windows SCM

SCM hospeda o MESMO runtime/owner; sem segundo worker/root/listener administrativo. Conta padrão dedicada selecionada e token observado; accountSID não equivale a SCM serviceSID. Instalar/iniciar/parar/status/manual-auto/remover; sem SYSTEM/LocalService/Administrador default, senha em argv/log/config ou autoelevação runtime. CLI canônica administra SCM; serviço não confirma cerimônia interativa nem cria CA.

Callbacks apenas sinalizam owner; STOP_PENDING/retenção e statusSCM são distintos de destruição. Sem liberação por timeout, TerminateProcess como cleanup, retry infinito ou DESTROYED fictício. Sem config/autoridade/ED11, startup operacional nega com erro factual, não abre rede por estar no SCM.

## M6 — .conf settings-only

Preservar gramática diretivas/blocos, sem INI/parser paralelo. Restauração em instalação NOVA importa allowlist suportada de opções não secretas: portas/binds revalidados localmente, opções/preferências/rede/limites válidos e retenção visível. Referências de autoridade/security/provider anterior, certificados, CA, root identity/CURRENT/serial/revogações/credenciais/pins/autorizações consumidas não são restaurados. Texto não concede confiança, cadastro ou quotas maiores.

Diretiva GLOBAL proposta revoked_history_retention_days N: decimal unsigned32 0..4294967295, ausente=0;0=nunca ocultar por idade. Sem [Pairing]. Catálogo/projeção/campo persistido precisam revisão própria de patch/layout; sem padding ou reinterpretar BODY/ROOT históricos. Especificação única: ../integration/personal-mvp-consolidation-20261022/config-and-retention.md. Conta/segredo SCM não vai para .conf.

## M7 — Clientes e R-ACTIVE

Uma coleção canônica com views ativa e histórico revogado. Campos só com proveniência factual; data desconhecida aparece como tal. Revogar exige confirmação exata/autorização e commit/status/CURRENT factuais; só então sai da lista ativa. Filtro visual não revoga.

Escolha NOVA PROPOSED para R-ACTIVE: após publicação factual da revogação, negar novas autenticações/admissões/OPEN/BIND e sinalizar Stop aos owners de TODAS as Sessions/operações daquela credencial, antes de novos efeitos autorizáveis. Não ampliar a outra credencial do Principal sem ato correspondente. Não mutar snapshot/grants nem desfazer bytes aceitos; CANCEL/FINAL/fences/cleanup seguem contratos. Quiescência/close factuais, UNKNOWN retido. Não afirmar desconexão/revogação ativa concluída sem prova de cada fase; propagação incerta bloqueia novos efeitos e resultado positivo da fase. Especializa somente POLICY_PENDING de issuer8/issuer-C2 P5.2, não é consequência implícita de status/TLS close.

## M8 — Histórico visível e protegido

Excluir do histórico só após revogação confirmada, com ato/commit canônico; não remover ativo. Decisão durável de ocultação na MESMA root, sem segundo cadastro. Manter tombstones/IDs/DER/hash/revogação/serial/high-water/evidência e leases obrigatórias. Ocultar não libera slot protegido, reativa credencial ou cria crédito. Layout/backing/versionamento exatos ainda exigem revisão. Coleção cheia nega novos registros, não purga tombstone para ganhar quota.

Auto mede N dias de86400000ms desde revogação factual. Checked arithmetic e tempo qualificado: só ocultar se menor instante atual admissível >= maior instante de revogação + duração. Ausência/rollback/descontinuidade/overflow/UNKNOWN => manter visível. Default0 preserva view; sem depender de Browser/UTC escalar não qualificado. Ação manual independe de idade, não de revogação/autorização.

## M9 — SDK e independência

Preservar composite shared-owner-first, formatos/transportes de provisão accepted e chave automática no cliente; Browser primeiro, não único. Excluir configuração Browser não revoga servidor; revogar servidor independe de Browser online. Sync bilateral adiada. Nenhum contrato/código Browser/PST muda aqui. MANUAL/AUTOMATIC convergem no issuer/root/cadastro, não são qualificados por decoder.

## M10 — Futuro e limites

Backup integral autenticado/criptografado, recuperação antiga, migração, institucional/MDM e sync bilateral ficam futuros. .papaccbak é só nome/extensão candidata, sem bytes/crypto/API/compatibilidade; não é .papacc de provisão. Ver ../future/personal-backup-and-identity-recovery.md. Após aceitação o futuro não bloqueia MVP por existir; gates essenciais continuam bloqueando.

## M11 — Aceitação e implementação

Revisão oficial e aprovação específica do owner, sobretudo M3/M7, precedem novo registro accepted. Até lá TODAS normas anteriores vigem, inclusive backup/restore. Aprovação documental da proposta não autoriza código. Após aceitação, exigir autorização funcional específica, hunks, builds/testes e ED11. Não encerrar Phase6 sem capacidades reais/demais obrigações vigentes. Nenhuma alteração de números/quotas/wire/formatos históricos/produção0.6.4 ou TLS1.3/mTLS operacional.


## Escopo e gates atuais

Histórico: ativos separados de revogados; ocultação manual somente após revogação confirmada; retenção automática desde a revogação, revoked_history_retention_days ausente/default0, sem limpeza por idade quando tempo não qualificado/UNKNOWN; tombstones/evidências obrigatórias preservados. Não reativar credenciais ou liberar quota por ocultação.

Obrigatórios: instalação pessoalWindows/lifecycleSCM/config.conf/CAprotegida/provisãoMANUALeAUTOMATIC/pareamento/certificados/listagem/revogação/histórico/desinstalação previsível. Backup completo/recuperação antiga/institucional/syncbilateral ficamfuturos. .conf restaura só opções não secretas, não identidade/pareamentos. .papaccbak continua extensão candidata não especificada, distinta de .papacc.

Aceitação normativa não qualifica código/LAB como produção, isolamento de conta/serviço, fontesUTC/status, provas normais de issuer, recursos nativos ou capacidade. Nenhum build/test/rede/conta/serviço/key/cert/bootstrap/CURRENT é executado ou autorizado. Autorizações funcionais/hunks ainda são separados. Preservar PST/Browser/wire/pin0.6.4 e WIP. Phase6 não é declarada concluída.

CURRENT_TASK_COMPLETED=YES
PERSONAL_MVP_CONTRACT=ACCEPTED
M3_DECISION=ACCEPTED_WITH_OWNER_INTERNAL_PROTECTIONS_PRESERVED
M7_DECISION=ACCEPTED_INVALIDATE_ADMISSIONS_STOP_AND_FACTUAL_QUIESCENCE
IMPLEMENTATION_AUTHORIZED=NO
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=WINDOWS_SCM_LIFECYCLE_IMPLEMENTATION

## Histórico

Revisão1,2026-10-22: aprovação explícita do owner de M3/M7, revisão exata, precedência pessoal delimitada e registro sujeito à aprovação oficial dos postimages. Originais preservados; nenhum efeito operacional.
