# Phase 6 — fechamento offline READ/MACHINE

2026-11-03. TASK PACC-PHASE6-READ-MACHINE-END-TO-END-CLOSURE.
Estado: **caller frio READ/QUERY e lifecycle integrados e qualificados offline**. UPDATE preparado, aguardando decisão específica. Nenhuma operação protegida executada. Branch main/HEADf341985924d29870d8ff627ed37c8a0874ad6469 e WIP preservados.

## Decisão e compatibilidade

K0–K9 accepted permanece byte-exata: proposta8489c009dfa62df0835415d4544c0632abd850924a35217b2ec451718147ad38; registroe4c9fdaff1c4f4ddedbe8c7373be81eab31fb594c6683ba87ea823ac154b82c0. Não há GOVERNANCE CONFLICT identificado nem nova decisão arquitetural necessária para esta ligação técnica. Nenhum accepted foi modificado.

K3: entrypoint exclusivo MACHINE_READ/seleção console original, token/proveniência/paths/SD/cadeia retidos; PXR1 vincula imagem/instalação/key/publicSHA, nunca autoridade por DATA. K4: startup genérico/QUERY permanecem sem KSP; quatro atos E/Q/S/P separados, P consumido somente após admissão e validações originais. K2: controle positivo administrativo fresco da MESMA fixture, com referência original retida e identidade pública. K5: probe único no próprio runtime virtual sem impersonação, retorno DATA privado bounded no mesmo pipe. K6: dois novos originais K6/five-query read-only e custódia/continuidade até terminal. K7: seleção READ explícita e pisos/guards próprios exatos; nenhum fallback. K8: receipt/close originais e lifecycle finito, UNKNOWN/Stop sem retry/rollback. U1–U7/U2 continuam únicos mecanismos de UPDATE; nenhum owner/serviço/root/canal/distribuição adicional.

READordinary/TRANSPORT_ONLY/NEW_INSTALL/SAM/outros perfis mantêm suas regras. Getter genérico CNG continua NULL; fixture, token, imagem, keyname, publicSHA, carrier e resultado não recebem autoridade. Future Directions de listener/recovery não foram implementadas ou alteradas: não têm delta factual nesta integração.

## Código integrado e cadeia

- `server_launcher_scq_cold_start_win32.inc` + `personal_scq_cold_start_win32.inc` ligam entrypoint estreito17argv ao owner/plano/originais READ e quatro confirmações atribuídas. Nome/pin público são DATA, aquisição/proveniência/consumo continuam nativos. Parser rejeita TO/READordinary/NEW_INSTALL, ausência ou divergência de seleção.
- `personal_scq_ingress_win32.inc`, bootstrap, inputs/image-chain/profile/recipient/transport reutilizam QUERY candidata, runtime tipado, leitura/hash/SD/cadeia/SCM e cinco referências originais; ambos os peers e continuidade são independentes do conteúdo recebido. Nome nativo direto/OBJ_DONT_REPARSE/access/share/ancestrais preservados.
- `personal_scq_machine_read_continue_win32.inc` compõe no mesmo owner: admissão QUERY → watermarkSKA1 → consumo P/controle positivo administrativo → SKP1 → probe runtime → SKR1 → receipt/SQC3/retorno de close. SKA1 só ordena, não autentica/concede autoridade.
- `server_ca_key_win32.c` reutiliza owner CA e APIs de abertura/validação/controle público; READ não cria fixture, assina ou exporta privado. Antes de cada API protegida aplica o MESMO baseline/observador G3 e prazo, sem renovação/rebase. Rejeição indevida introduzida na entrada fixture durante integração foi corrigida e ambas as regressões fixture passaram; guardas ordinárias continuam intactas.
- Probe mantém token original e faz resource/identity/no-impersonation antes de provider/OpenKey. Falha de consulta de handles, inclusive last-error sem código útil, é DENY/ERROR_GEN_FAILURE; jamais PASS. Amostra ainda fresca≤100ms exigida, latência nativa pode impedir a próxima chamada. NTSTATUS/CNG originais não são substituídos por evidência inventada.
- `scqr_closed_ready_current` exige PERMISSION_OBSERVED fechado, além de toda prova original; BAD_KEYSET, provider/local failure, chave inesperadamente aberta ou close inconclusivo não qualificam RUNNING diagnóstico. Produção continua DENY.

Layout/censo/ownership detalhados em [READ-MACHINE-CLOSED-PRIVATE-PROTOCOL-AND-LIFETIMES.md](READ-MACHINE-CLOSED-PRIVATE-PROTOCOL-AND-LIFETIMES.md). SQK1=1132,SKA1=100,SKP1=144,SKR1=128; PS_SCQ308/registro1048/receipt1052/SQC31092 preservados. Buffer de sessão+32bytes, nenhum pipe/event/OVERLAPPED ou handle de transporte adicional. Recursos CNG reais não são equiparados a número provado de kernel handles.

## Qualificação final

| Configuração | Build | Testes afetados | Warnings | ROOT | KNOWN | Quota |
|---|---|---:|---:|---:|---:|---:|
| Selected Debug | PASS |39/39|0|380408|2007456|3145728|
| Selected Release | PASS |39/39|0|380408|2007456|3145728|
| Legacy Debug | PASS |39/39|0|211840|1838888|2097152|
| Legacy Release | PASS |39/39|0|211840|1838888|2097152|

**156 pares PASS**. MSVC/Ninja e targets/configurações documentados preservados; /W4 /WX. Não repetir campanhas inalteradas. A última requalificação foi justificada pelo guard novo de consulta indisponível; artefatos finais correspondem a esse código.

Novo teste source-linked `scq_machine_read_chain_test.c` e `scq_machine_read_ca_model.c`:27 cenários/1473 checks, quatro caminhos positivos fechados, APIs de token/arquivo/QUERY-dup/pipe/KSP/SCM mutantes. Executa corpos reais da entrega das cinco referências, validação/admissão, owner CA/controle positivo, P/probe/resultado e receipt/close recíprocos; cenário positivo também ServiceMain modelo START_PENDING→RUNNING diagnóstico→STOPPING→STOPPED, com readiness produtiva falsa. Entry/console/plano frio são qualificados separadamente pelos seus próprios corpos/modelos; isso não é execução do binário instalado ou prova de capacidade/Windows/KSP real.

Cobertura: originais ausentes/substituídos, token/imagem runtime divergente, QUERY sem autoridade, bilateral incompleta, P ausente, replay/duplo consumo, deadline expirado, Stop antes/durante, receipt/close ausentes, closeUNKNOWN/retenção, recursos/entrada129/guard188, controle positivo/hash/identidade divergentes, provider falho, BAD_KEYSET, abertura inesperada, watermark adulterado e readiness prematura. READruntime21cenários/927checks inclui count indisponível/errozero,188 e freshness; entryREAD7 e seleção fria8 casos sem operação nativa. Regressões anteriores de TO/READordinary/NEW_INSTALL/limites/SD/identidade/lifetime também PASS.

Censo ROOT/KNOWN +272bytes sobre snapshot anterior, quotas intactas. Preparação fixture Selected K3071004<3145728 (margem74724 somente aritmética); Legacy K2902436>2097152, preparação permanece DENY antes dos efeitos. Isso não é suficiência nativa nem aplica K7 à criação. Anexo próprio de assurance da fixture continua obrigatório; custos Windows/provider/alocador/stack permanecem opacos.

K7 READ preservado: entrada128/guard188/cap192/private1994944/working3145728/físico7239172096/commit3221225472/fresh100/janela240000/global60000, sem renovação. Nenhuma exclusão de handles, aumento de quota ou fallback. Piso READ3GiB não foi transportado a READordinary, outros perfis ou criação fixture.

Manifesto [READ-MACHINE-END-TO-END-OFFLINE-QUALIFICATION.json](READ-MACHINE-END-TO-END-OFFLINE-QUALIFICATION.json), SHA256 **d3622a25bc080ffe5326b20da7cd50b4ba052fe4e8ef03316efcec12fc292a40**, contém fontes, oito logs finais, contratos, wrapper e censos. Snapshot anterior152pares/históricos preservados; oito pins anteriores reconferidos. Ver também MACHINE-FIXTURE-FIRST-SNAPSHOT-EVIDENCE-RETENTION.json quanto à retenção documental intermediária já registrada; nenhuma evidência histórica foi reconstruída.

## Delta, imagem instalada e UPDATE

Última imagem INSTALADA reconciliada (histórico, sem consulta nova):809984bytes,ID417614100:1638400:439388,SHA25678886f53119af1875dd7f326221b8f154e9528ef02f2f61f6a7bd9d23fab3de5. SCM/configuração/STOPPED históricos não são lease fresco; novas observações só sob autoridade própria. Nenhuma imagem foi instalada aqui.

Candidato final Selected/Release: **871424bytes**, SHA256 **6063d3692f22cfd3baa7e5128b5b787ce6ce33ddb362e805897323f39884599a**; delta+61440 sobre instalada/+26112 sobre snapshot offline845312. Cópia congelada papacc_server.read-machine-closed-871424.exe (DATA, sem autoridade), hash idêntico. DLLs do build permanecem nos pins históricos; updater protegido104448/084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a não foi recompilado ou trocado.

Pedido [ONE-READ-MACHINE-CLOSED-IMAGE-UPDATE-REQUEST.CANDIDATE.md](ONE-READ-MACHINE-CLOSED-IMAGE-UPDATE-REQUEST.CANDIDATE.md), SHA256 **d6f10549a207a39527cea032e37882c8995d9168f912e02cbf932b5e12bd3837**. Procedimento manual candidato preparado, AST/bindings PASS, helpers/16argv reutilizados. **UPDATE pronto para decisão específica, não autorizado/executado.** Nenhuma decisão do proprietário foi fabricada e nenhum comando com hash de aprovação fictício é emitido.

## Primeiro gate e caminho mínimo

Primeiro gate operacional: seleção/decisão específica deste pedido UPDATE e posterior execução MANUAL única com identidade/SD/continuidade/SCM STOPPED/recursos frescos. Estado impeditivo/UNKNOWN => STOP; nenhum retry/limpeza. Reconciliar saída antes de alegar instalação.

Depois: preparar a decisão operacional independente da fixture TEST_ONLY com seu próprio anexo/custos/assurance/novos originais. Nome e SHA público só existirão após criação original factual; não inventar. O limite Legacy de preparação permanece bloqueante nesse perfil. Somente com fixture conhecida preparar originais READ K6 e nova sessão E/Q/S/P independente, com tuple/imagem atual, key/publicSHA, lifetime e quatro confirmações; PID/token do runtime/bilateral/probe/receipt/close/finite exit são provas posteriores, nunca PASS prévio.

TRANSPORT_ONLY já qualificado não será repetido; infra/registro/updater/three-acts não são reimplementados. Histórico/application-close/retention/reservas/quiescência OS UNKNOWN preservados, sem limpeza/refund/recovery. Nenhum KSP/coleta/endpoint/SCQ/StartStop/SCM-ACL/UPDATE/CA-CURRENT/rede/TPM-NV-Q4/produtivo executado; PST0.6.4 e EXEs anteriores preservados.

Próximo prompt: [NEXT-READ-MACHINE-CLOSED-IMAGE-UPDATE-DECISION.md](NEXT-READ-MACHINE-CLOSED-IMAGE-UPDATE-DECISION.md).
