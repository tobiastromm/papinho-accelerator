# T0â€“T9 revisÃ£o 2 â€” aceitaÃ§Ã£o e integraÃ§Ã£o qualificada offline

TASK=PACC-PHASE6-ED11-T0T9-ACCEPTANCE-AND-INTEGRATION â€” 2026-10-25.

## Resultado

A aprovaÃ§Ã£o normativa explÃ­cita foi registrada em [DECISION T0â€“T9](../../../adr/DECISION-n0start03-noninherited-local-transport-20261025.md), incorporando integralmente a proposta aprovada SHA2565336728e06b1daeb87fe691e637e3ed2a64262967c27ad3c48fb518436b5d995. N5 original permanece byte-exato. A precedÃªncia Ã© exclusivamente a emenda Ã  primeira frase no escopo N0START-03; os demais parÃ¡grafos de N5 e B+D permanecem controladores. O [registro do owner](owner-approval-record.json) reproduz sua declaraÃ§Ã£o real; nÃ£o Ã© assinatura ou certificaÃ§Ã£o independente, e nÃ£o aprova nenhuma operaÃ§Ã£o.

O launcher canÃ´nico e o qualificador receberam o transporte stdout privado local. NÃ£o foi criado outro launcher operacional, serviÃ§o, owner de admissÃ£o ou processo helper. A DLL Ã© um recurso no MESMO supervisor; o binding .NET Framework x64 Ã© compilado offline, sem compilador novo em runtime para este transporte. O owner/mutex, A1/B1/self-token, RAW1, N7, H/P3, exactly-once e todos os limites permanecem.

## ImplementaÃ§Ã£o e limites do escopo

- CreateProcessW usa heranÃ§a FALSE. Novos handles sÃ£o nÃ£o herdÃ¡veis durante toda a sessÃ£o. Nenhum SetHandleInformation modifica handles preexistentes do PowerShell.
- Endpoint local exclusivo/uma instÃ¢ncia/REJECT_REMOTE_CLIENTS, nonce BCrypt128, DACL protegida SYSTEM+operador exato com mÃ¡scara cliente0x00120183; DACL Ã© lida e verificada do objeto original. NÃ£o hÃ¡ remediaÃ§Ã£o de ACL existente.
- AutenticaÃ§Ã£o recÃ­proca de PID/criaÃ§Ã£o/liveness/token primÃ¡rio/user/AuthId/sessÃ£o, sem impersonation, contra processo ORIGINAL no supervisor. Nome/hello/ack nÃ£o sÃ£o credenciais nem admissÃ£o. Um intruso que conectar primeiro nega a sessÃ£o sem reconexÃ£o.
- O cliente valida namespace fechado e identidade antes de conectar; stdin/stderr continuam no console. O writer FILE Ã© explÃ­cito com NOINHERIT e sem _dup2. printf/puts/fflush alcanÃ§Ã¡veis no caminho read-only sÃ£o encaminhados pelo adapter; conteÃºdo ASCII/UTF16 hexadecimal atual preservado. [InventÃ¡rio](stdout-dispatch-review.csv) distingue emissores das rotas legadas/positivas, que nÃ£o sÃ£o habilitadas por esta feature. NÃ£o se afirma compatibilidade arbitrÃ¡ria Unicode/transcript/custom host.
- stdout Ã© lido em buffer nativo persistente; nenhum buffer GC/stack de caller fica referenciado por I/O pendente. CancelIoEx nÃ£o Ã© prova de conclusÃ£o: GetOverlappedResult Ã© exigido antes de fechar/reutilizar. UNKNOWN conserva contexto/buffer/refs, sem reset/retry/double-close/kill.
- Falha de escrita no qualificador solicita Stop local, marca retenÃ§Ã£o e entra em loop sem retorno; nenhum comando de Stop Ã© transportado no pipe. O supervisor retÃ©m processo/pins/locks quando falta recibo ou tÃ©rmino factual. O caminho CreateProcess definitivamente negado pode fechar preparatÃ³rios somente com PID/criaÃ§Ã£o ausentes, contexto nÃ£o congelado e conclusÃ£o/close comprovados.
- CorreÃ§Ã£o de erro: o Win32 original disponÃ­vel em _get_doserrno Ã© preservado. O teste real de peer fechado observou232 e freeze. A primeira expectativa do fixture era109; foi corrigida por confronto com o valor efetivamente retornado, sem converter erro em PASS operacional. Se o CRT nÃ£o expuser Win32, fallback WRITE_FAULT Ã© falha incerta, nunca sucesso. Logs de diagnÃ³stico e da expectativa falha foram preservados.
- Guardas contra DLL preexistente, localizaÃ§Ã£o divergente e assembly diverso evitam adotar cÃ³digo jÃ¡ carregado. Arquivos e ancestrais permanecem sob os locks/pins existentes. Contexto Ã© de sessÃ£o Ãºnica; nÃ£o existe API de rearm/reset.

## Builds e testes

Selected/Legacy Ã— Debug/Release: PASS, zero warnings finais (/W4 /WX). Legacy Ã© o perfil moderno MSVC com Windows3MiB desabilitado, nÃ£o migraÃ§Ã£o para NT4/VC6. Toolchain e SDK seguem os scripts do projeto. Binding Framework compilado x64 com warn-as-error.

- 60 regressÃµes: A1, B1/owner, self-token, RAW1, freshness, Stop/UNKNOWN/retenÃ§Ã£o, modelos de runtime/ciclo, perfil default/opt-in. [Resultados](native-and-regression-results.json): considerar as60 entradas de regressÃ£o; suas48 entradas preliminares do core foram sucedidas pelo relatÃ³rio final abaixo, nÃ£o somadas como cenÃ¡rios novos.
- 52 resultados esperados do cÃ³digo nativo integrado,13 por configuraÃ§Ã£o: caminho limpo, reentrÃ¢ncia/busy, CreateProcess negado e cleanup, peer/token divergentes por fault injection, intruso REAL com replay, criadores globais concorrentes, erro de escrita real, namespace/criaÃ§Ã£o divergente, impersonation real no prÃ³prio thread, close e fclose incertos. [Resultados finais](native-core-final-results.json). Dez concorrentes adicionais por caso race, comparados aos objetos privados originais por PSS completo/CompareObjectHandles, zero correspondÃªncias nos testes. AutenticaÃ§Ã£o incorreta nÃ£o recebe ack/payload; referÃªncias prÃ³prias normais fechadas. Casos UNKNOWN mantÃªm metadados sem segunda tentativa.
- 28 casos PowerShell5.1 nas quatro configuraÃ§Ãµes: protocolo/linhas vazias/hex, saÃ­da extensa e truncamento com drain+recibo, console interativo prÃ³prio (Read-Host, ReadConsoleW, stderr, Escape, F10), falha de forwarding, recibo ausente, perda do console prÃ³prio e DLL previamente carregada. [Resultados](powershell-qualified-results.json). O bloco de forwarding/recibo/Wait Ã© extraÃ­do byte-exato do launcher canÃ´nico, sem executar seu dispatcher Prepare. Native client e adapter sÃ£o os mesmos arquivos da integraÃ§Ã£o. Teclado sintÃ©tico Ã© usado SOMENTE nos consoles privados de teste.
- Review estÃ¡tico do launcher final/AST/pins: PASS, sem owner/preflight. ConfiguraÃ§Ãµes private+pinsOFF e private+positiveON sÃ£o negadas pelo CMake. [Log final](static-source-gates-final.log).

Limites da prova: credenciais de outro usuÃ¡rio real nÃ£o foram usadas; divergÃªncia de token primÃ¡rio Ã© fault injection, enquanto ImpersonateSelf/denial e APIs de identidade corretas sÃ£o reais. Fixtures UNKNOWN contÃªm seu prÃ³prio processo de teste, sem reproduzir cleanup operacional nem declarar quiescÃªncia global. A guarda do launcher real para manter operador/refs apÃ³s erro Ã© preservada; nÃ£o se matou host operacional para testar notificaÃ§Ãµes. N7 permanece a aceitaÃ§Ã£o especÃ­fica existente. Nenhum dos testes executa main Prepare/N0 operacional, SCM LAB, intent, Start ou Stop.

## Recursos e economia observada

ComparaÃ§Ã£o emparelhada PowerShell com o MESMO executÃ¡vel sintÃ©tico e oito eventos sentinela privados novos: rota anterior64 handles iniciais no filho e8 correspondÃªncias por identidade; rota privada52 antes da conexÃ£o e53 com stream conectado, zero correspondÃªncias dos oito objetos. Portanto diferenÃ§a observada12 no ponto inicial e11 no ponto conectado deste fixture. Quatro handles da diferenÃ§a inicial nÃ£o receberam atribuiÃ§Ã£o causal nova; nÃ£o declarar que todos sÃ£o dispensÃ¡veis. [Rota anterior](comparison-final-Legacy.json) / [rota privada](comparison-final-Private.json).

Supervisor observado separadamente: rota anterior584â†’588â†’578 e rota privada584â†’597â†’586. SÃ£o amostras, nÃ£o prova de leak nem custo estÃ¡vel universal. A nova composiÃ§Ã£o tem custo de mÃ³dulo/CLR/evento/autenticaÃ§Ã£o e deve debitÃ¡-lo no administrativo, sem transferir custo do qualificador ou conceder crÃ©dito. O cenÃ¡rio C limpo Release observou52â†’82â†’79; sua baseline/custos de inicializaÃ§Ã£o diferem do PowerShell e nÃ£o devem ser somados a ele.

[Ledger de ownership/recursos](ownership-and-resource-ledger.csv): contexto nativo4464 bytes medido nas quatro configuraÃ§Ãµes, inclusive buffer persistente4096; nÃ£o Ã© limite de memÃ³ria total. Buffer managed4096 e char storage atÃ©4096 UTF16, FILE/CRT, DLL/assembly, heap/stack/kernel adicionais sÃ£o separados. Disassembly do objeto Release registra subrsp8432 em Create,1520 em Open,80 em Client,48 em Read; pushes, callers, bibliotecas e multiplicidade sÃ£o extras, nÃ£o um teto transitivo. PSS/stack-VA reais continuam gates futuros. Os slots A1 nÃ£o foram ampliados. Tokens transitÃ³rios do bootstrap tÃªm ownership prÃ³prio explÃ­cito e sÃ£o cobrados no processo correspondente.

Economia no transporte integrado foi medida em fixture PowerShell; economia no launcher completo com N0/SCM/dossiÃª reais NÃƒO FOI MEDIDA. Os126 histÃ³ricos e as projeÃ§Ãµes129/132 nÃ£o viraram amostra fresca ou grant. 128/1024/quatro papÃ©is/60s1s/headroom5/9GiB/NONE_GRANTED permanecem; obrigaÃ§Ãµes histÃ³ricas e reservas reais continuam retidas. A tabela antiga de owners canÃ´nicos nÃ£o deve ser confundida com uma lista exaustiva do novo contexto: o novo stdout pode ser correlacionado pelo alias STD_OUTPUT_HANDLE e pelo ledger, sem atribuir zero aos demais unmapped.

## Pins finais

| Artefato | SHA256 |
|---|---|
| Qualificador Selected Release | `6f58cd11624360413d63480933a9ce01385525989381f5fd0b5af675061d5b5f` |
| Launcher canÃ´nico | `b65571abc3b5335b79f60ab0cca055d08c71dff749b1b947c5dcf55ca03cd6b3` |
| DLL nativa no supervisor | `0d3f611010254082673a170e81118728b303d92f3ac74e7b86a21a2aee906abf` |
| Binding Framework x64 | `513107355c46b8282ac17eb9763a1278bd201c0d4f9f9a925950676fba14d9f4` |
| DecisÃ£o T0â€“T9 accepted | `c9ac85be53fc0df9c356b6a0856c520dcf25d212593e2ec78ee05ef949f7c330` |

Os arquivos de aprovaÃ§Ã£o histÃ³ricos, o binÃ¡rio histÃ³rico2bf1746aâ€¦ e a proposta original foram preservados; a preimagem do launcher3451d4d5â€¦ estÃ¡ no pacote. [Patch prÃ³prio](applied-integration.patch) compara a baseline desta tarefa, preservando WIP anterior em CMake e demais arquivos. Nenhum commit/push/tag/worktree/clone foi feito.

## Gate operacional Ãºnico restante

Novo [pedido read-only candidato](READONLY-OPERATION-REQUEST.DRAFT.json), SHA256 `67c312608207eaf4e7dcff863a8c1253d4558b9c8e4d2e7d54dc5a416e90db16`. [Procedimento](manual-readonly-procedure.md) e [redaÃ§Ã£o candidata](OWNER-APPROVAL-CANDIDATE.txt) preparados, sem aprovaÃ§Ã£o executÃ¡vel.

HipÃ³tese discriminante: medir a composiÃ§Ã£o REAL sem heranÃ§a ampla, nos trÃªs checkpoints jÃ¡ existentes e janelas PSS, separando qualificador e PowerShell+auxiliar, recursos novos e primeiro erro. NecessÃ¡ria porque fixtures nÃ£o incluem todo N0/SCM/RAW1/Windows initialization; nÃ£o repetir a observaÃ§Ã£o anterior sem mudanÃ§a. Depois do aceite especÃ­fico desse pedido, registrar a declaraÃ§Ã£o real, calcular seu hash literal e entregar o comando final para execuÃ§Ã£o manual Ãºnica. NÃ£o usar o hash do pedido como se fosse o hash do registro aprovado. Valores runtime permanecem null/desafios; nÃ£o inventar PID/token/mÃ©trica.

CURRENT_TASK_COMPLETED=YES â€” escopo tÃ©cnico e pacote concluÃ­dos.
NORMATIVE_T0_T9=ACCEPTED
CANONICAL_TRANSPORT=INTEGRATED_OFFLINE_QUALIFIED_READONLY_OPT_IN
DEFAULT_PROFILE=EMPTY_DENY
POSITIVE_ROUTE=DENY
OPERATIONAL_APPROVAL=ABSENT
FRESH_NATIVE_OBSERVATION=NOT_EXECUTED
POSITIVE_CAPACITY=NOT_DEMONSTRATED
INDEPENDENT_BUDGET=NOT_GRANTED
PREPARE_EXECUTED=NO
START_EXECUTED=NO
STOP_EXECUTED=NO
SCM_MUTATIONS=NONE
EXISTING_ACL_MUTATIONS=NONE
HISTORICAL_UNKNOWN=RETAINED
ED11_TOTAL_STATUS=PARTIAL
PHASE6=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
FIRST_UNRESOLVED_DIVERGENCE=ACTUAL_INTEGRATED_NATIVE_RESOURCE_COMPOSITION_NOT_OBSERVED
NEXT=OWNER_APPROVES_EXACT_PRIVATE_TRANSPORT_READONLY_OBSERVATION

NETWORK_TESTS=NOT_RUN — local pipe/console tests only; no remote communication.
WIP_PRESERVED=YES_WITH_SCOPED_EDITS_AND_PREIMAGES
