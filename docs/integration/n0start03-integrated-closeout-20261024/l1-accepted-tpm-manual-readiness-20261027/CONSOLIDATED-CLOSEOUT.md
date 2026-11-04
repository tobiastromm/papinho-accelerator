# L1 ACCEPTED e prova TPM manual pronta — closeout consolidado

TASK PACC-PHASE6-ED11-L1-ACCEPTED-TPM-MANUAL-READINESS, 2026-10-27. HEAD main `f341985924d29870d8ff627ed37c8a0874ad6469` preservado; preimagens do WIP e patch relativo ao WIP estão neste pacote. Nenhum commit/push/tag/worktree. As autorizações A e B foram conferidas e registradas separadamente. Nenhuma execução TPM foi iniciada pelo agente.

## Decisões e pins

- L1 original SHA256 `335dd1f829d959220ab1e9e0c2038cae9209ec5b9c205e52a15952d55686d5e3`, preservada byte-exata. [Decisão ACCEPTED](../../../adr/DECISION-q4-l1-single-root-component-20261027.md) registrada antes de implementar, SHA256 `40a746438fb3dfb75ac447964227aff25293dd49f4f1c08ad1d65cea90e3a1cd`.
- Pedido TPM original SHA256 `357c33ceafb7e5a33c69c87f7a70e0db685a16547e0e7fe8741b33e0d07d77a1`, preservado.
- [Aprovação operacional independente](../tpm-l1-native-proof-and-parallel-closure-20261026/OWNER-APPROVED-NATIVE-PROOF.json), SHA256 `d7417145a73a2b8ec03fd4b57df23ecc10b045178d02eac543557be3fa76a146`.
- Executável manual imutável `NativeProofRelease.exe`, SHA256 `ffd02de63a9b8e95741707532b6e81b33307f7745741232ae91f4c35d4148334`.
- Dez pins de decisão/pedido/artefatos/aprovação e sete fontes E17/E18 conferidos: [17 checks PASS](MANUAL-PACKAGE-VERIFICATION.json). Recibo de uso único ausente na revisão, sem reserva implícita. Nenhum desafio gerado/reutilizado.

## Implementação efetiva e limites

Novos `src/config/config_root8_v5.c/.h`: carrier21216..32352bytes, componente512, endian/shape/bounds/checksums, callback DATA10 obrigatório, CP5, fragmentos e vínculo completo v4/v5. A validação matemática Ed25519 para v5 é independente; o adaptador antigo v4 e os validadores/formatos v3/v4 permaneceram byte-exatos.

Q4 vincula o SHA256 de TODOS os bytes da preimagem original, incluindo seu digest final. Isso é distinto do digest interno prefixado em header208. Nas transições posteriores o componente conserva o predecessor original PREPARED e os nove manifestos; a linhagem externa aponta ao predecessor imediato. Referências/hash não substituem handles originais, File IDs, RAW1 ou autenticação. O codec não resolve autoridade nem declara antirollback.

A relação aceita somente criação PREPARED a partir de v4 e transições da mesma invocação PREPARED->CONSUMED/UNKNOWN e CONSUMED->RESULT/UNKNOWN. Identidade PID/criação, baseline/expected e manifestos permanecem iguais. Replay, retomada de terminal, substituição e salto de fase negam. Os callbacks de testes usam manifestos sintéticos explicitamente, não evidência protegida real.

Transições documentais preservam descritores/configuração/issuer/SNT/PDG/TSC e epoch operacional. BODY0.5 precisa atualizar seus próprios campos revision/predecessor e checksums para acompanhar a linhagem; os segmentos semânticos de configuração são comparados byte-exatos. Uma mudança TLS válida como DATA mas indevida para Q4 é rejeitada pelo vínculo documental. Nenhuma fonte/capacidade é alterada por esses bytes.

O mesmo owner e worker canônicos receberam discriminador privado14, sem colidir com personal13. O guard verifica os carriers/vínculo/BODY/chaves e congela pares completos; usa a mesma identidade nativa, comparação de TODA a preimagem, TxF, flush/readback, Stop e retenção. Não há novo owner, registry, thread mechanism ou ledger paralelo. A entrada de fixture permanece compile-isolated; **não foi instalado produtor de autoridade operacional nem resolvedor factual dos manifestos**. O guard de pacote congelado não autentica manifestos/issuer por si só, como o guard11 anterior; tal qualificação e live pin/RAW1/budget continuam pré-condições do futuro binding autorizado. Não houve migração/adoption, escrita da root real, publicação ou NV.

Essa é integração técnica qualificada do formato e persistência pelo worker existente sobre arquivos sintéticos. Não é prova de persistência real ledger+TPM ou autorização para um participante Q4 ativo. DPAPI/custódia é referência DATA; nenhum segredo/backing operacional foi criado. Witness real, custódia, manifests/provisionamento e readback ainda são gates.

## Builds, regressões e faults

| Evidência final | Resultado e alcance |
|---|---|
| Selected/Legacy Debug/Release, MSVC x64 |4 builds PASS, zero warnings, incluindo `papacc_server`, biblioteca canônica e codec |
| Teste DATA v5 |69.241 verificações por configuração, 4/4 PASS; builders reutilizados, sem autoridade |
| Regressão DATA v4 |97.663 verificações por configuração, 4/4 PASS; inclui v3 histórico, não somado duas vezes |
| Worker v5 nativo sobre arquivos sintéticos |20/20 PASS: Debug/Release x predecessor4/5 x sucesso/Stop/UNKNOWN/conteúdo divergente/identidade divergente |
| Worker v4 regressão |10/10 PASS: mesmas cinco falhas/caminhos nas duas configurações Selected |
| Binding da root atual |132 por configuração, 528 verificações PASS; nenhuma store/TPM/publicação |
| Perfis/créditos ED11 |16 invocações PASS; modelos de crédito não concedem créditos reais |
| Artefatos manuais e fontes de serviço |17 pins PASS; executável TPM não invocado |
| Protocolo/faults anteriores |17 testes worker, 1.092 checks simulados e 728 criptográficos reutilizados no escopo anterior; não recontados como evidência nativa nova |

[Resultados finais v5](FINAL-DATA-RESULTS.json), [20 casos finais do worker](FINAL-QUALIFICATION-RESULTS.json), [regressões v4 e campanha inicial](QUALIFICATION-RESULTS.json), [binding/perfis](BINDING-RESOURCE-REGRESSIONS.json). Falhas iniciais de harness (path relativo; expectativa de que versão-base válida isolada negaria no encode em vez do link) foram corrigidas. Link do fixture v4 dependia do módulo personal DATA já exigido pelo worker atual; CMake agora inclui esse módulo canônico em ambos os fixtures. Não relaxamos validators para satisfazer testes. Os registros iniciais e arquivos sintéticos ficam preservados, sem limpeza.

Os casos de UNKNOWN conservam as referências originais no estado observado do processo fixture. A saída do fixture não demonstra fechamento global nem resistência física a rollback. Witness/crash/reboot/rollback anteriores são modelos; os casos nativos aqui provam somente o caminho TxF/CAS sobre arquivos sintéticos. Nenhum TPM foi consultado ou modificado nesta campanha.

## Storage, lifetimes e stack

Cada carrier aumenta512bytes. No Selected crescem seis buffers existentes: committed, previous/next pendentes e três backings, total3072bytes. Owner263584->266656; pending74344. Com os campos de fault injection o fixture tem owner266680/pending74368,24bytes extras de teste. Legacy owner98088/pending43432 inalterado; codec DATA disponível, worker v5 sem capacidade nativa suportada nesse perfil. Workspace32768 comporta o novo máximo32352 sem ser aumentado.

[Layout e frames compilados](STORAGE-AND-FRAMES.json): `sub rsp` Release base_link344, encode96, validate56; Debug valores separados no JSON e listings. Esses números excluem pushes, callees, pilha transitiva, provider/kernel/runtime/TPM e não são memória total ou pico observado. Nenhuma nova aquisição/handle pelo codec, retenção de ponteiro, quota, crédito ou reserva concedida.

## Comando manual e reconciliação

[Procedimento definitivo](OWNER-MANUAL-HANDOFF.md) e [comando literal](OWNER-MANUAL-COMMAND.txt). Usar console Windows PowerShell5.1x64 já elevado e operador exato; não executar automaticamente. Todos os guards de boot/host/token/console/budget permanecem ativos. Boot fixado237900/início2026-10-26T21:35:16.5000000Z; divergência significa STOP, não atualizar pin. Uma tentativa, sem retry/remoção do recibo. O EXE cria somente seu recibo administrativo exclusivo e sessão transitória previstos no pedido. Não NV/AK/hierarquia/Q4/Start/Stop.

[Template de reconciliação](NATIVE-RESULT-RECONCILIATION.TEMPLATE.json) exige saída original completa, first/stage, proof, Name, live binding, calls, limites/amostras, session_end, context/observer close e retained. Falha de fechamento retém dependências; nenhum FlushContext adivinhado/kill/refund. Posse matemática no endpoint não demonstra dispositivo físico íntegro, geração NV, não revogação ou orçamento; revogação segue UNKNOWN. Nenhuma disposição Q4 decorre do experimento.

## Caminho crítico atualizado E17–E22

[Plano único paralelo](ED11-CRITICAL-PATH.json). E17 precisa identidade/acesso efetivo/readiness reais em futuro ciclo independente elegível; R0–R9 e sete fontes/testes conferidos sem regressão. E18 precisa Stop solicitado/aceito/concluído, lifecycle e fechamento factual; não inferir de processo encerrado. Essas frentes não esperam TPM para preparação.

E19/E20 mantêm coletores e contratos já implementados; não houve hipótese nova que justificasse repetir as campanhas. D2 é accepted somente para preparação. Permanecem idade/validade/duração/headroom físico/commit sem valores justificados e bindings root/CURRENT/epoch/SiteTLS/destino/módulos/host. Custos ativos provider/kernel/shared/retained seguem UNKNOWN. Browser1858020/2097152 é reserva gerenciada de uma operação;239132 não é margem TLS/kernel. NSS/x86 e OpenSSL/x64 não foram equiparados. Nenhuma rede/identidade/intent operacional foi criada.

E21 permanece PENDING: próximo fato é a única prova manual agora autorizada, depois reconciliação. Não basta para designar geração NV, custodiar/provisionar índice ou executar Q4. Hardware/ledger durável, readback e disposição documental exigem suas condições e autorizações próprias. E22 depende dos demais gates. Matriz mantém4 demonstrados/12 limitados/6 pendentes; ED11 e Phase6 PARTIAL.

## Hashes finais e preservação

[35 hashes finais de fontes/artefatos](FINAL-ARTIFACT-HASHES.json), [registro das decisões](APPROVAL-REGISTRATION.json), preimagens e [patch relativo ao WIP](L1-IMPLEMENTATION-RELATIVE-TO-WIP.patch). Codec C SHA256 `ac764b1ffa36b022125178130dfe552ccaeb997ade3ac95a6473b43e56b568e6`; header SHA256 `d63c0c50965d2739d3959d65020cce6ee07461e48f2dff08b4e7b4a691c77853`. Históricos/pins operacionais/inputs protegidos/UNKNOWN/reservas preservados. Os únicos pins novos são derivados dos artefatos desta integração e da aprovação específica; não ativam admissão.

CURRENT_TASK_COMPLETED=YES (integração técnica e preparação manual autorizadas). L1=ACCEPTED; TECHNICAL_INTEGRATION=QUALIFIED_DATA_AND_FROZEN_SAME_OWNER_WORKER; NATIVE_TPM_PROOF=NOT_EXECUTED; NV_MUTATIONS=NONE; Q4=NOT_EXECUTED; START_STOP=NONE; SCM_ACL_MUTATIONS=NONE; ED11_PHASE6=PARTIAL; CURRENT_SIGNATURE_AUTHORITATIVE_RECOVERY=DENY. NEXT=OWNER_ONE_MANUAL_TRANSIENT_TPM_PROOF_THEN_RECONCILIATION.
