# FCA — caller original, worker e key owner integrados offline

2026-11-03. Integração técnica offline concluída para o escopo limitado FCA. UPDATE autorizado contextualmente pelo proprietário, **execução manual pendente**. Preparação real de contêiner/chave, assinatura, CURRENT, NORMAL e Start não executados nem concedidos por este pacote.

## Autoridade e baseline

FCA0–FCA7 alternativa B aprovada pelo proprietário pelo SHA256 `365103d7f06cbe55dd0a1a260e4a274ad5db6dab40ae9dee6864edaf83803f5a`, registrada antes do código em `DECISION-personal-first-instance-preparation-assurance-fca0-fca7-20261103.md`, SHA256 `a4f8b9700c66bb8488ec78295d549596721b54f1fd31b4b430dc0fa9b7f0d1a7`. Proposta, accepted anteriores e todos os históricos preservados. Branch main, HEAD `f341985924d29870d8ff627ed37c8a0874ad6469`, WIP preservado. Sem commit/reset/stash/limpeza.

Preimagens da composição em `fca-caller-preimages/`; etapa anterior do key owner em `fca-preimages/` e `FCA-IMPLEMENTATION-BASELINE.json`. O relatório anterior `FCA-KEY-OWNER-QUALIFICATION-REPORT.md` permanece como estado intermediário histórico; sua lacuna do caller foi tratada nesta etapa.

## Percurso integrado

O entrypoint existente aceita somente a declaração adicional exata `NEW_INSTALL_CA_PREPARE`. A seleção chama um ramo específico do mesmo launcher; não é CLI administrativa universal, fallback de READ/TRANSPORT_ONLY ou autorização por argumento. Legacy e outros targets não recebem seleção positiva. O presenter antigo mantém seu handoff DATA.

O ramo FCA captura o orçamento original antes das aquisições, exige console original e chama a aquisição nativa existente de parent/token. Parent de dados e leaf devem corresponder à resolução NEW_INSTALL já existente; o contêiner permanece `Accelerator`. O novo campo de chave é storage do mesmo launcher, fora do commit para respeitar a rejeição existente de overlap; reutiliza o key owner, não outro manager/serviço.

Reutiliza resolver direto `\Device\...`, OBJ_DONT_REPARSE e a representação de custódia de oito diretórios. O tag privado FCA só seleciona guard/consulta SD nessa custódia; não é purpose SCQ válido nem fonte de autoridade. Pode reter um ramo protegido distinto para o source no mesmo volume, sem ampliar slots. Original do source é read-only, regular, link único, com owner/DACL qualificados. A associação ao parent de dados usa a identidade do original retido, não um nome ou identidade histórica.

O producer nativo captura efetivamente o parent, token e descritor. A identidade retornada do parent alimenta os campos usados pela guarda do worker. Cada produtor revalida seus próprios bytes de SD; entre GetSecurityInfo e consulta NT são comparados owner e ACL exata/proteção, sem exigir a mesma disposição de offsets do SD. Não há relaxamento de trustees/masks ou semântica de reparse. A exceção AU create-child existente continua restrita ao anchor; nenhum ancestral ProgramData é presumido admissível.

A configuração aceita para preparação é deliberadamente mínima: UTF-8 sem BOM, bytes de `format_version 1\nclient_admission DISABLED\n`. Parse/validação e conferência do source permanecem; listeners/blocos extras/admissão não são aceitos. Authority/generation e tentativa vêm do CSPRNG existente; zero/colisão observada nega. SID da conta virtual resolvida é candidato explicitamente apresentado, não token de runtime nem prova P4.

Apresentação mostra plano/configuração/IDs/efeitos/política e riscos; a confirmação atribuída nomeia conjuntamente NEW_INSTALL+CA_INITIALIZE. Arm somente encurta a janela original. Antes da submissão há revalidação e consumo irrevogável. O mesmo worker revalida plano, source, token/contexto, parent/ancestrais, Stop e recursos. Não se lê metadata de confirmação mutável no worker para restaurar aprovação invalidada.

O contêiner usa FILE_CREATE exclusivo relativo ao mesmo parent, SD final protegido na criação, owner operador e allowlist administrativa operador/SYSTEM/BA. Sem alteração de ACL existente. Referência e identidade futuras vêm só do retorno original, com readback de proteção. O worker não publica nem cria a chave. Após sinal e fechamento do worker, CA_INITIALIZE usa o mesmo key owner e plano, uma criação/finalização e readback; não obtém borrow produtivo. O setter/máscaras CA ordinários permanecem; S0–S5 da fixture não são importados.

Não se tornou verdadeiro o `personal_native_ceremony_ready` genérico. Publicação, assinatura, CURRENT, startup NORMAL e readiness produtiva continuam negados. P4/K5/I0–I6 e os caminhos READ/TO existentes não recebem uma dispensa.

## Lifetimes e correções demonstradas

- Identidade do parent: o modelo com a aquisição real mostrou que a captura usada na apresentação não preenchia o slot de identidade do worker. A ligação FCA agora copia somente o retorno original para esse slot; não remove a guarda.
- IOSB do contêiner: a passagem ao backing durável inicialmente ainda conferia o local antigo. A checagem agora lê o mesmo IOSB usado na chamada. Status/Information/pending são testados pelo primitive real com API mutante.
- Backings: parent e contêiner FCA usam OBJECT_ATTRIBUTES/UNICODE_STRING/nome/IOSB no packet retido. Um teste de limite corrigiu a capacidade do nome durável, menor que o antigo local. Retorno pending/inconclusivo não permite reutilizar ou liberar esse storage.
- Consumo/guard: nova consulta completa no worker imediatamente antes do efeito; conferência final de freshness/deadline antes do NtCreateFile, sem rebasing. Begin do orçamento também valida freshness e Stop depois da amostra inicial.
- Close: source é emprestado do chain owner, sem segundo CloseHandle. Candidate de chave só é removido do packet após fechamento factual do exato key owner. Falha de close retém a coorte e não tenta liberar de novo. Source/parent/SID/SD só saem após worker quieto. A contenção retida já existente, com Sleep(INFINITE), impede que o retorno do processo descarte backings incertos; não é retry, timeout de cancelamento ou close SCQ. Handler cooperativo de Stop permanece quando a coorte é incerta.

Nenhuma dessas correções demonstra retroativamente a causa histórica de 1816, BADKEYSET ou timeout de sessões anteriores. Os fatos antigos e UNKNOWN permanecem.

## Qualificação final

Mesma toolchain MSVC18/vcvars64 e mesmos diretórios Selected/Legacy Debug/Release. Quatro builds finais sem warnings; 30 testes por configuração, **120 pares PASS**, logs `fca-final4-*-build.log` e `fca-final4-*-tests.log`. Não foram repetidas campanhas sem relação: funções compartilhadas de token, namespace/SD, custódia, CA, READ/QUERY e layout receberam regressões por causa da composição/extrações e alterações de header. Tentativas intermediárias foram preservadas; os resultados finais não são substituídos por logs de falha antigos.

Modelo FCA Selected: **161 checks**, percorrendo as funções de produção de aquisição do parent, captura FCA, consumo, dispatch, criação exclusiva, worker, CA_INITIALIZE e close, com efeitos nativos substituídos. Inclui deny/reparse/pending originais, buffer, autoridade/source/identidade substituídos, perfis ausentes/divergentes, hardlink, token, SD, replay, recursos, colisão, falha de submissão, deadline, primeira falha CNG e close/retention. Legacy tem dois checks de recusa, sem ramo positivo. Entry model tem **12 casos**, testa roteamento e rejeições, não fabrica confirmação ou autoridade. Os modelos existentes exercitam o token helper, o key owner real com CNG substituído e as guardas estritas de SD/namespace/READ.

Desvio de isolamento registrado: no teste intermediário `fca-caller-model2`, o adapter native_current não estava substituído. A chamada consultou elegibilidade local de token/membership e negou antes de possuir o token original. O código do teste não abriu arquivo operacional nem submeteu CNG/SCM ou alteração persistente. O adapter foi substituído e os testes finais não usam esse caminho nativo. Não se apresenta aquela consulta como coleta autorizada ou prova de elegibilidade. Helpers reais de SID/SD e identificação da thread do modelo operam sobre memória/contexto do próprio teste; nenhuma coleta operacional de recursos/serviço foi executada.

Manifesto final com fontes e hashes dos logs: `FCA-FIRST-PREPARATION-OFFLINE-QUALIFICATION.json`, SHA256 `72dc2e5e2e8161917836e89187d0c42b3c73a7b75adb4baed320f760f89e669e`.

## Recursos e isolamento

| Termo | Bytes/limite |
|---|---:|
| Root Selected | 381504 |
| C8 conhecido Selected, incluindo stacks/pools já cobrados | 2008552 |
| Charge FCA de payloads dinâmicos: 12 SDs limitados + 8 KiB | 794624 |
| Conhecido total do percurso FCA Selected | 2880828 |
| Limite Selected | 3145728 |
| FCA hipotético Legacy, não habilitado | 2712260 |
| Limite Legacy, inalterado | 2097152 |

Os nove SDs originais da chain, parent original/check e scratch compartilhado são cobrados; SID/SD de criação e alocações locais limitadas da chave têm charge separado. Não é limite de overhead do alocador, provider, kernel, latência ou host. Censo não é suficiência universal. Base/caller antigos não recebem o charge/envelope FCA por fallback.

Entrada 128, guard pré-aquisição 188, máximo observado 192; private 1994944, working 3145728, physical 7239172096, commit 3221225472; freshness 100 ms, janela 240000 ms, global 60000 ms sem renovação/rebase. Contagem inclui Windows, herdados e referências duplicadas legitimamente retidas. READ e outros perfis preservam os seus próprios contratos. Só o ramo FCA aprovado pode usar esse envelope.

## Imagem e UPDATE

Candidato final Selected/Release: **905728 bytes**, SHA256 `1ee37a465e6820a5d5967e25630673aadb81d1972a734fb9bf4046ef3b19b2e6`. Congelado em `papacc_server.fca-first-preparation-905728.exe`; build e cópia conferidos. Delta de tamanho: **+23040 bytes** sobre o último instalado reconciliado.

Instalado histórico: 882688 bytes, ID `417614100:1966080:412413`, SHA256 `e3a433c4a6edb565c783fee672bfc82a5794ba44988b123e7a38eb4ec5c9d6b7`. Não houve consulta atual ao objeto protegido ou SCM para converter esse histórico em lease. Novo EXE ainda não instalado por este trabalho.

Pedido `ONE-FCA-FIRST-PREPARATION-IMAGE-UPDATE-REQUEST.CANDIDATE.md`, SHA256 `9f856436cfbd90d857540f6bebfd6cf16765469822491ce6bd2d094e7b8458a7`. Reutiliza papacc_update.exe protegido 104448/id417614100:30670848:57859/SHA084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a e U1–U7/U2. Helper definitions idênticas ao procedimento anterior; AST PASS, uma chamada/16 argumentos; não invocado.

O proprietário respondeu **“autorizo o update”** durante esta preparação. A decisão contextual foi registrada com transparência sobre a emissão posterior do pedido final; não se inventou que ele citou/revisou anteriormente esse hash. Decisão efetiva SHA256 `15dcd9bff58a47565bc6d1e914e3198bfb015b28d763e4a8b6aa074e0c29240e`; procedimento SHA256 `0a31f85c452585c79ccffee949208dc0d6966fc79dfd0e917435ee957aa3fc10`. Comando exato em `FCA-FIRST-PREPARATION-UPDATE-MANUAL-COMMAND.md`. Execução manual única pendente; não solicitar aprovação redundante nem executar automaticamente.

## Primeiro gate e caminho mínimo

Primeiro gate: única chamada manual com pins e guards frescos do executor, antigos/candidato/DLLs/ancestrais, operador e SCM STOPPED/configuração original. Depois: reconciliar consumo/dois renames/anterior/novo ID-hash-SD/readback/close, sem retry. Próximo prompt: `NEXT-FCA-FIRST-PREPARATION-UPDATE-RECONCILIATION.md`.

Somente com esse resultado preparar o pedido operacional FCA próprio e os originais mínimos de configuração/parent efetivamente qualificados. Não presumir proteção de ProgramData, disponibilidade do source, recursos, confirmação, ausência de objeto anterior ou sucesso de execução. Modelo não fornece essas provas reais. UPDATE não concede criação FCA. Preparação FCA, por sua vez, não concede assinatura/CURRENT/NORMAL/Start ou dispensa P4/K5. Não há motivo para repetir a antiga investigação READ ou sessões consumidas para esse UPDATE.

Estado: FCA_CALLER_WORKER_KEY_OWNER=OFFLINE_QUALIFIED; UPDATE=AUTHORIZED_MANUAL_PENDING; REAL_FCA=NOT_EXECUTED; PRODUCTION=DENY; retenção histórica/quiescência OS=UNKNOWN; PST=0.6.4. Nenhuma operação real de UPDATE, criação de contêiner/chave, endpoint, Start/Stop, alteração SCM/ACL, rede, TPM/NV/Q4, cleanup/refund ou autoridade produtiva foi realizada.
