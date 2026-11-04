# Phase 6 — contexto nativo e provisão

**Primeira falha resolvida factualmente: ADMIN_ENABLED = falso.** Não é erro de API. O contexto observado é inelegível; não foi relaxado. Phase 6/ED11 PARTIAL; instalação e autoridades produtivas DENY.

## Diagnóstico nativo

A instrumentação preserva exatamente a ordem dos guards de token e parent. Separou retorno de API, valor observado e NTSTATUS, com primeira falha latched no contexto existente. Query error não se transforma em ausência, e GetLastError não é usado como erro de um predicado booleano. O close continua factual e independente da primeira causa.

O preflight real nas quatro configurações parou no predicado 3, `ADMIN_ENABLED`, kind=2 (predicado falso), native_error=0. Antes dele foram satisfeitos os requisitos anteriores da mesma chamada; os posteriores não foram declarados PASS.

Observação Selected Debug:

- TokenUser: `S-1-5-21-896877160-3262526087-2538948522-1003`.
- TokenElevation consultado: não elevado. TokenElevationType=Default (1), sem inferência de token administrativo limitado.
- TokenGroups consultado independentemente **depois** da primeira falha: SID Administrators ausente. Não foi chamado de grupo desabilitado ou deny-only.
- TokenSession=1, ProcessSession=1, ActiveConsoleSession=1; observação independente, não dispensa a ordem do guard.
- Host de metadados `TCORP-TOBIAS`. Boot System/Kernel-General12 RecordId=237900; StartTime XML `2026-10-26T21:35:16.5000000Z`. TimeCreated do evento é separado (`2026-10-26T21:35:17.6469098Z`). Nome/boot metadata não são identidade física autenticada nem autorização.
- Parent: não alcançado/aberto. O teste apenas escolhe TEMP como probe; nenhum alvo de instalação foi designado.
- Native close=OK, token do observador fechado, observer_error=0. Não houve worker, child, CA/key, credencial ou publicação. Pico total de handles e custo provider/kernel não foram qualificados; nenhum crédito concedido.

O owner é o pacote transitório do teste read-only, com writer da thread corrente. Não se provou uma root operacional publicada; NEW_INSTALL sob F0 não exige uma CA/CURRENT anterior. Não se retoma descoberta/bootstrap C8 por inferência.

`NATIVE-CONTEXT-RESULT.json` e `HOST-BOOT-METADATA.json` preservam os dados. O SID observado pertence ao processo de execução desta coleta e não deve ser substituído pelo SID histórico do proprietário ou do serviço.

## Contexto necessário e correção legítima

Não há correção de código legítima que faça esse token satisfazer Administrators habilitado. O caminho é obter evidência do contexto administrativo legítimo do proprietário, interativo, primário, elevado/não limitado, sem impersonação e na sessão ativa; parent/source e declaração/confirmacão continuam exigidos depois. Elevação sozinha não autoriza NEW_INSTALL ou CA_INITIALIZE.

Foi preparado `collect-owner-context.MANUAL.ps1` para **uma coleta manual somente de metadados** em Windows PowerShell 5.1 x64 já elevado. Confere hash do próprio script e do coletor antes da execução. Não eleva, não altera execução/permissões, não cria target ou CA e não publica CURRENT. O teste abre somente o parent TEMP existente se os guards anteriores permitirem, para leitura de identidade/segurança e consulta dos direitos exigidos; não cria conteúdo. Não é validação de um alvo operacional.

O script foi validado sintaticamente, não executado automaticamente. Comando completo em `MANUAL-COMMAND.txt`. Após a saída, reconciliar a primeira causa e fechamento; não presumir que passar ADMIN_ENABLED satisfaz os demais gates.

## Correções e testes

- Extração do código dos mesmos guards para `server_personal_token_checks_win32.inc`, sem novos critérios permissivos.
- Primeira falha observável nos guards de token e nos passos principais de abertura/proteção do parent; causas de consulta e valor separado. Falhas não instrumentadas permanecem explicitamente NONE_OR_UNINSTRUMENTED, nunca PASS por ausência de código.
- 88 verificações/configuração de fault injection dos mesmos guards, incluindo impersonação, membership, falhas GetTokenInformation, elevação, sessão, SID, continuidade, primeira causa e retenção de falha de close. APIs de token simuladas apenas no teste.
- Selected/Legacy Debug/Release: quatro builds PASS, zero warnings.
- 19 CTests/configuração: **76 execuções PASS**. Oito fixtures nativos v1/v2: PASS após a mudança de layout. Campanhas inalteradas CAS/TPM e criação persistente não repetidas.
- Layout: Selected 1.972.064/3.145.728; Legacy 1.803.496/2.097.152. Diagnóstico privado aumentou o layout em oito bytes após padding; limites inalterados, custo opaco UNKNOWN.

## Avanço independente AUTOMATIC

Foi implementado `ep_trace_csr_candidate`: handoff DATA borrow-only da CSR originalmente copiada para o trace, após claim e correlação com request/nonces/conexão/geração. Rejeita fase incompleta, pending/terminal, pair inválido ou sem claim, Stop, expiração, mismatch e output alias. Falhas deixam a saída intacta. O owner mantém trace/pair e backing imutáveis pelo lifetime do borrow; não é um novo owner, lease de autoridade ou alocação.

A composição CONFIRM/claim→estado observado→CSR→candidate foi qualificada. Alterar o RX não altera a CSR do owner. A suíte enrollment passou de 1.386 para **1.405 verificações DATA/configuração**.

A função não valida PoP, não transforma APPROVED recebido em autorização, não emite e não persiste L2. O issuer ainda exige fonte/autoridade/custódia originais. Não houve listener, rede, novo formato ou setter de aprovação. Este componente independe de NEW_INSTALL e foi implementado sem esperar o contexto administrativo.

## MANUAL/E8, Browser e M7

Foram confrontados os bindings reais de ambos os projetos. `mp_auth_pdg_candidate` valida os DATA disponíveis e continua SOURCE_ABSENT quando falta fonte factual. O adapter e Browser M1 chamam o binding indisponível `sp_trust_acquire(NULL,NULL,NULL,...)`: não existem produtores de pacote factual/observação administrativa/confirmação independente, nem lease positiva. A negação ocorre em TRUST antes de KDF; um receipt do presenter ou o novo borrow de CSR não pode satisfazer E8.

Browser HEAD observado: `96f6cd2f9ec4f834999828fa13bb7d170db66a98`. Nenhum arquivo Browser/PST foi alterado; repetir seus builds inalterados não produziria a fonte faltante. NSS/x86 e OpenSSL/x64 permanecem distintos. Não foi criado callback positivo ou autoridade compartilhada artificial.

M7 permanece na composição isolada já qualificada: digest exato, credencial não relacionada preservada e Stop solicitado separado de fechamento/quiescência. Sua regressão passou; nenhum encerramento produtivo foi inferido.

## Custódia e primeiro gate real

O perfil pessoal accepted P0/P3/P4 mantém CA Microsoft Software KSP MACHINE e identidade operacional distinta legitimamente selecionada, com TokenUser real e negativa efetiva de uso privado pelo runtime. CurrentUser do fixture não equivale a MACHINE. Custodiante produtivo, conta/SID operacional e alvo efetivo ainda não foram demonstrados; decisões STORAGE_20/Q4 não os concedem.

Primeiro gate externo: saída da coleta manual no contexto legítimo já elevado. Depois, qualificar o primeiro predicado que realmente permanecer pendente, parent/source, identidade operacional e custódia MACHINE. Não há pedido pronto para criação de CA ou instalação real. Não é necessária nova ADR para reafirmar esses requisitos.

O próximo prompt está em `NEXT-OWNER-CONTEXT-AND-CSR-CONSUMER.md`. Mantém a continuação independente do consumidor CSR/L2 e fontes E8, sem reabrir o presenter/criptografia. Nenhuma operação NEW_INSTALL, MACHINE CA, CURRENT, SCM/ACL, Start/Stop, rede, TPM/NV/Q4, limpeza/refund ou recuperação foi executada. Chave retida, históricos, pins, HEAD/WIP e PST 0.6.4 preservados.

## Evidências

`QUALIFICATION.json`, logs, preimages, `SOURCE-CHANGES.json`, delta e `ARTIFACT-HASHES.json` contêm os hashes finais. A matriz única continua com 60 requisitos e separa o resultado de contexto dos gates de provisão e Phase 6.
