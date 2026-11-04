from pathlib import Path
import json,hashlib
p=Path('docs/integration/n0start03-integrated-closeout-20261024/native-tpm-host-e2e-20261027');sha=lambda q:hashlib.sha256(Path(q).read_bytes()).hexdigest();req=sha(p/'manual-candidate/NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json');exe=sha(p/'manual-candidate/NativeTpmProof.exe');summary=json.loads((p/'FINAL-RESOURCE-SUMMARY.json').read_text());dbg=next(x for x in summary if x['configuration']=='Debug' and x['case']=='clean');rel=next(x for x in summary if x['configuration']=='Release' and x['case']=='clean')
text=f'''# Host TPM C nativo — qualificação completa isolada

TASK=PACC-PHASE6-ED11-TPM-NATIVE-HOST-END-TO-END-QUALIFICATION
Data:2026-10-27. Projeto canônico, branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preservados.

## Resultado

Implementação e integração técnica não operacional concluídas. Host moderno C/x64 sem CLR implementado e qualificado OFFLINE, com transporte TPM simulado. Pedido manual novo preparado, NÃO aprovado e NÃO executado. Nenhuma operação TBS/TPM real, Q4, NV, serviço, SCM/ACL, CURRENT, assinatura ou recuperação foi realizada. E21 continua PENDING e ED11/Phase6 PARTIAL.

A instrução do proprietário “execute sempre” permite executar tecnicamente os pedidos anexados nesta conversa; não cria aprovações normativas/operacionais. W/L/L1/cross-boot accepted e propostas incorporadas foram conferidos; não foram modificados. [Plano/revisão inicial](SOURCE-REVIEW-AND-PLAN.md), [preservação](PRESERVATION-VERIFICATION.json), [pins históricos e fontes reutilizadas](HISTORICAL-AND-REUSE-PINS.json). Não houve commit/push/tag, clone/worktree, exclusão ou restauração de WIP.

## Implementação e guards

Novas fontes em tools/qualification/tpm-native-proof: proof_core (wire/CNG/owner efêmero da própria prova), json_data (DATA ASCII bounded), host_platform (Win32 identidade/arquivos/console/recursos) e native_host (entrada manual e adapter TBS tardio). CMake integra somente targets opt-in PAPACC_ENABLE_NATIVE_TPM_PROOF_CANDIDATE=OFF por default; testes de metadata/console vinculados ao host também OFF por default. [Patch relativo ao WIP anterior](CMAKE-INTEGRATION.WIP-RELATIVE.patch). Nenhum novo owner de produto, registry, mutex canônico, IPC ou serviço. Launcher e pins operacionais históricos não foram alterados.

PE Release x64/CUI, COM Descriptor Directory zero; imports sem CLR/mscoree/PowerShell e sem TBS estático. TBS é carregado de System32 somente na rota manual após aprovação exata, confirmação e receipt. Targets sem argumentos negam; selfcheck usa apenas CNG. [PE/imports](PE-IMPORTS-RELEASE.txt), [dependências on-disk](DEPENDENCIES-ON-DISK.json). Esses hashes não atestam imagens mapeadas. MSVC14.51.36231 x64, Windows SDK/CRT da configuração existente; Debug/Release /W4 /WX /GS /sdl e C11. Não se presume compatibilidade NT4/VC6 para este host TPM moderno. Produto Legacy/Selected, NSS/x86 e OpenSSL/x64 não foram trocados ou reconstruídos sem necessidade.

Preservados operador exato, token sem impersonation, TokenId/AuthId/ModifiedId antes/depois e em cada boundary, host/event Computer/boot237900/start exatos, PID/criação do processo/pai/console, objeto original/volume/FileID/tamanho/hash e revalidação por handle. Igualdade de hash não substitui identidade. O código novo não abre os inputs operacionais Q4 nem afirma RAW1 novo; seus históricos/descritores permanecem intactos. Certificado público pinado alimenta somente importação pública RSA.

Preobserver<=124; amostras próprias<=128; pai+console<=1024. Query/error/count distintos; nenhum recurso é descontado para obter PASS. Referências dos observadores são temporárias, conhecidas e fechadas antes da próxima fase; qualquer close incerto congela aquisições e retém originais. Quantidade máxima de16 amostras administrativas cobre a sequência máxima atual, incluindo revalidação depois do load TBS; esgotamento nega, não sobrescreve. [Ownership e boundaries](OWNERSHIP-AND-BOUNDARIES.csv).

## Equivalência do protocolo

GetCapability máximo8 handles persistentes e página completa; no moreData/fallback. PublicArea RSA2048/expoente65537/SHA256/attrs0x300b2/AES128CFB e modulus exatos; Name calculado sobre os314bytes; uma única EK candidata. StartAuthSession319bytes, NULL bind, salt32 com RSA-OAEP-SHA256 e label SECRET NUL, caller nonce fresco32, sessão HMAC SHA256/AES128CFB. KDFa ATH preservada. GetRandom89bytes com attrs0x40, continue-clear, pedido32; autenticação de resposta85..117bytes com0..32bytes cifrados permitidos. Nonce fresco, HMAC, frames e ReadPublic final iguais antes do proof=true. No máximo12 Submits diretos, sem retry.

O [oráculo C#](VectorExport.cs) reutiliza fontes históricas intactas; [vetores](oracle-vectors.bin) coincidem com fixture versionado. KDF,319/48/89bytes, HMAC, Public398, capability, AES-CFB e398 decisões de mutação coincidiram byte a byte. RSA-OAEP aleatório foi validado por roundtrip em chave SOFTWARE efêmera; ciphertext aleatório não é comparado por igualdade nem usado como prova de TPM. Nenhum material privado EK foi solicitado/exportado.

Revisão externa primária confirmou a ordem: rpHMAC verificado antes da decriptação dos parâmetros no [TPM2-TSS esys_iutil.c, iesys_check_response](https://github.com/tpm2-software/tpm2-tss/blob/master/src/tss2-esys/esys_iutil.c#L1230-L1267). A referência é corroborativa, não biblioteca embarcada nem relaxamento do contrato; nesta prova só se autentica o ciphertext, sem alegar qualidade RNG/plaintext. O que se poderá provar nativamente é conhecimento matemático de material privado correspondente à EK pública, não geração física autenticada.

## Builds, testes e correções

Dois builds integrados Debug/Release do host e quatro fixtures cada, zero warnings. Sete CTests por configuração (14/14 PASS): protocolo, guards, artefatos/faults, metadata read-only, console com TPM mock, selfcheck sem TPM e default DENY. [Logs Debug](Debug-FINAL-ctest.log), [Release](Release-FINAL-ctest.log). Reutilização de fontes históricas confirmada por hashes, sem campanha redundante de produto.

Por configuração:4375 assertions de core/vetores,149 de artefatos/persistência/concorrência,58 de guards; metadata read-only executa os58 e quatro checks adicionais (62), não62 inteiramente novos. Três casos de console real com endpoint simulado por configuração: limpo, confirmação errada e Escape. [Matriz de testes](TEST-COVERAGE.csv).

21 modos do core incluem abertura falha/contraditória, TPM_RC, falha Submit, frame/nonce/HMAC/Name divergentes, Stop antes/depois de sessão, mudança de identidade, headroom, múltiplas EKs/página incompleta/ausência, oito candidatos e12Submits, close incerto e falha de hash. Pressão real de handles sintéticos do próprio processo rejeitada antes de qualquer TPM. Reentrância e outra thread não adquirem nova operação. Primeiro erro preservado, finish repetido não fecha novamente.

Artefatos: parser bounded/duplicate/overflow/typed booleans, pin errado/ID substituído/conteúdo, compartilhamento incompatível, DER público real, seis criadores de receipt concorrentes com um único vencedor. WriteFile falso, escrita curta, Stop antes de criar e CloseHandle incerto foram distintos; nada de retry ou CLOSED inferido. Sete estados de retenção do core usam referências/endpoint de teste, mantidos até perda do processo fixture; isso não comprova fechamento de referências operacionais ou quiescência global.

Durante a qualificação, a conexão do controller ao console foi corrigida usando FreeConsole apenas no próprio fixture, sem flags globais de handles. O guard REAL negou a topologia pseudo-console em que HWND pertence ao cliente. Essa negação foi preservada. Para qualificar o restante do fluxo positivo, o fixture declara explicitamente autoridade/artefatos/EK/papéis administrativos MODEL; usa objetos sintéticos e NÃO registra soma administrativa como admissão. As falhas e logs anteriores permanecem em console-fixture-Release-01/02/03. Não se relaxou o guard para fazê-los passar.

A revisão final corrigiu o lifetime da chave SOFTWARE do endpoint: fecha antes do provider após fim de sessão conhecido. Acrescentou amostra PLATFORM_CLOSED após fechamento controlado bem-sucedido. Apenas os testes afetados foram repetidos: [Debug](Debug-FINAL-CLOSE-ctest.log), [Release](Release-FINAL-CLOSE-ctest.log); key-close anterior em [Debug](Debug-ENDPOINT-CLOSE-ctest.log)/[Release](Release-ENDPOINT-CLOSE-ctest.log). Resultado limpo first=NONE, controlled_refs=0, retenção=false. Nenhuma escrita TPM foi usada para testar erros.

## Recursos e alcance

[Ledger por checkpoint](PHASE-LEDGER.csv), [resumo](FINAL-RESOURCE-SUMMARY.json), saídas integrais Debug/Release-*-FINAL.txt.

| Fase do fluxo modelado | Debug | Release |
|---|---:|---:|
| Entrada |73|73|
| Dependências CNG |76|76|
| Metadata com referências abertas |83|83|
| Artefatos sintéticos fixados |87|87|
| Antes do contexto mock |87|87|
| Challenge preparado |89|89|
| Proof/ReadPublic final |89|89|
| Pico amostrado de toda a composição |91|91|
| Após fechamento controlado completo |84|84|

56 amostras no caso limpo de cada configuração; máximo11 referências LÓGICAS simultâneas do owner da prova, que NÃO se somam ao GetProcessHandleCount. A tabela do processo já inclui observadores, CNG, CRT, endpoint de software e caches; referências CNG/contexto podem não corresponder a uma entrada por objeto. Não descontar essa sobreposição. Endpoint mock pode exercer custos diferentes de TBS; não estimar total real91+constante inventada.

Nos casos finais, PrivateUsage máximo Debug={dbg['max_private']}bytes / Release={rel['max_private']}bytes; WorkingSet máximo Debug={dbg['max_working_set']} / Release={rel['max_working_set']}bytes. Durações entre primeira/última amostra Debug={dbg['elapsed_sample_ms']}ms / Release={rel['elapsed_sample_ms']}ms, não limite normativo. A coleta usa pseudohandle de processo e buffers fixos, sem novo handle explicitamente aberto pelo sampler; overhead interno Windows/CRT e intervalos não amostrados UNKNOWN. Mudança de tempo/caches entre repetições não é regressão de quota. Não se retorna73 após fechamento:84 inclui internos não atribuídos; não se declara vazamento, liberação global ou quiescência.

Layout compilado: TP_OWNER28688bytes, TP_PLATFORM166720 (inclui owner, dois documentos JSON, certificado, buffers, samples), RUNTIME166768. Samples próprios256x56=14336bytes já incluídos; não custo zero. [Frames compilados](COMPILED-STACK-FRAMES.json): tp_run848, hp_token320, hp_admin200, hp_boundary624, hp_boot8880 via __chkstk, file_open4256, hp_files712, wmain352bytes de subtração RSP nas listagens Release. Excluem pushes, inlining/callees/runtime/reserva de stack e não são pico transitive total. Nenhum crédito ED11.

A margem aritmética isolada91 até128 é37, e até124 é33; não é orçamento concedido ou garantia futura. Administração real1024 não foi demonstrada nesse fluxo positivo modelado. TBS/RPC real, pico não amostrado, referências internas transitórias e geração de hardware permanecem UNKNOWN. Não se subtrai249-91 como economia operacional: são processos/composições e autoridades diferentes.

## Pins e novo pedido

Executável Selected Release: `{exe}`.
Pedido novo: `{req}`.
[Todos os fontes/artefatos técnicos](TECHNICAL-PINS.json), [pins do candidato](MANUAL-CANDIDATE-PINS.json). Request14032bytes/450nodes passou pelo parser C e contrato reais, sem carregar TPM nem criar aprovação: [verificação](REQUEST-DATA-CHECK.log).

[Handoff manual candidato](MANUAL-CANDIDATE-HANDOFF.md). Nenhum OWNER-APPROVED-NATIVE-PROOF.json foi criado para o candidato, nenhum SHA de aprovação foi inventado, nenhum comando de execução foi ativado. Artefatos técnicos intermediários não aprovados foram retidos separadamente. A aprovação anterior permanece usada, não reutilizável.

O primeiro gate real é NOVA aprovação específica para uma invocação manual deste pedido. Depois da aprovação válida, registrar o hash literal e gerar o comando definitivo. O processo ainda precisa satisfazer no ambiente do proprietário os guards REAIS de console/token/host/boot/originais/124/128/1024 antes do TBS. Se falhar, DENY sem retry. A proposta permite medir a hipótese nova do host C completo; não afirma viabilidade positiva operacional já demonstrada.

## Caminho ED11/Phase6 independente

[Matriz crítica](PARALLEL-ED11-CRITICAL-PATH.json) e ED11_GATE_MATRIX.csv atualizados sem promoção de gate:4demonstrados/12limitados/6pendentes.

E17: R0–R9 resolvida prospectivamente; identidade/readiness/acesso efetivo do serviço precisam de futura tentativa elegível independente. E18: lifecycle, Stop aceito/concluído e fechamento/quiescência precisam de fatos; encerramento histórico não os substitui. Nenhuma nova identidade/intent foi gerada.

E19: D2 aceita apenas preparação; D4/D5 ainda sem escolha justificada de maximumAge, validade, duração e headroom físico/commit. LAB não fornece esses valores por analogia. E20: Browser reserva controlada1858020/2097152; diferença239132 não é headroom TLS/kernel. Ativos NSS/OpenSSL/kernel e composição real vinculada a root/CURRENT/epoch/SiteTLS/destino seguem pendentes. Fontes/evidências válidas reutilizadas, sem repetir snapshots ou campanhas equivalentes.

E21: novo host qualificado OFFLINE; posse nativa ainda não demonstrada, revogação UNKNOWN, geração/custódia/NV/ledger factual e leitura/disposição Q4 ainda pendentes. A prova matemática não fecha esses subgates. W/L/L1 não foram ampliadas. E22 depende dos gates anteriores. Outras pendências Phase6 permanecem separadas, sem afirmação de conclusão.

## Estado final

CURRENT_TASK_COMPLETED=YES (escopo técnico offline e candidato)
NATIVE_C_HOST=QUALIFIED_OFFLINE_SCOPED
BUILDS=DEBUG_RELEASE_PASS
WARNINGS=0
OFFLINE_TESTS=14_CTEST_PASS_WITH_AFFECTED_CLOSE_RERUNS_PASS
REAL_TPM_PROOF=NOT_EXECUTED
TPM_CALLS=0
PROVISIONING=NOT_READY
NEW_MANUAL_REQUEST=PREPARED_NOT_APPROVED
Q4=NOT_EXECUTED
E21=PENDING
ED11_TOTAL_STATUS=PARTIAL
PHASE6=PARTIAL
START_STOP_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
HISTORIES_UNKNOWN_RESERVATIONS=WIP_PRESERVED_WITH_SCOPED_PIN_VERIFICATION
FIRST_UNRESOLVED_DIVERGENCE=REAL_CONSOLE_ADMIN_TBS_AND_EK_POSSESSION_NOT_OBSERVED
NEXT=OWNER_EXACT_NATIVE_C_PROOF_DECISION_THEN_ONE_MANUAL_INVOCATION
'''
(p/'CONSOLIDATED-CLOSEOUT.md').write_text(text,encoding='utf-8')
print('REPORT_WRITTEN',len(text))
