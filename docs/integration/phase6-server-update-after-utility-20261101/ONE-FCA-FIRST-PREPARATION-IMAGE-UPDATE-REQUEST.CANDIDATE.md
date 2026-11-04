# Pedido operacional — UPDATE da imagem com preparação inicial FCA

CANDIDATE / MANUAL_ONLY / NOT_EXECUTED. 2026-11-03.

Objeto: UMA execução manual de `BINARY_UPDATE_CLOSED` PERSONAL_LOCAL U1–U7/U2, pelo executor protegido já qualificado, para substituir exclusivamente `papacc_server.exe` pelo candidato abaixo. Sem retry, execução automática ou rollback. A decisão operacional é registrada separadamente; este documento, seu hash e os arquivos do workspace não constituem autoridade nativa.

## Código e qualificação selecionados

Caller específico `config first-install --declare NEW_INSTALL_CA_PREPARE`, somente Selected AMD64, integrado aos originais, pending/worker e key owner existentes. A aquisição do parent real alimenta o slot de identidade usado pelo worker; originais de parent/ancestrais/source são retidos e revalidados. Resolver nativo existente, OBJ_DONT_REPARSE, criação exclusiva com proteção na criação, consumo único, Stop, deadlines, readback e close/retention preservados. O orçamento contempla payloads dos descritores e não exclui handles do Windows. Nenhum gate genérico de publicação foi habilitado.

A aceitação FCA0–FCA7 autoriza implementação offline, não a criação real da primeira instância. Este UPDATE também não autoriza essa criação. A funcionalidade instalada exigirá um pedido próprio, originais legítimos, declaração/confirmação atribuída e todos os gates FCA/F0 antes de qualquer efeito preparatório. Nenhuma assinatura/certificado/CSR, CURRENT, NORMAL ou readiness produtiva é concedida. P4/K5/I0–I6 permanecem; BADKEYSET não vira isolamento PASS.

Qualificação final: quatro builds Selected/Legacy Debug/Release, zero warnings; 30 testes por configuração, 120 pares PASS. Modelo FCA Selected com 161 checks; entrypoint com 12 casos. Manifesto `FCA-FIRST-PREPARATION-OFFLINE-QUALIFICATION.json`, SHA256 `72dc2e5e2e8161917836e89187d0c42b3c73a7b75adb4baed320f760f89e669e`. Não é qualificação nativa da preparação nem prova de suficiência universal.

Normas: U1–U7 incorporado na decisão accepted de UPDATE; proposta original SHA256 `9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234`. U2 executor/DATA original SHA256 `58493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da`. FCA aprovado SHA256 `365103d7f06cbe55dd0a1a260e4a274ad5db6dab40ae9dee6864edaf83803f5a`; registro accepted SHA256 `a4f8b9700c66bb8488ec78295d549596721b54f1fd31b4b430dc0fa9b7f0d1a7`. Nenhum accepted anterior foi alterado.

## Objetos selecionados

Operador esperado: TCORP-TOBIAS, SID `S-1-5-21-785699550-1440458426-1682014700-1108`, contexto administrativo legítimo, Windows PowerShell 5.1 Desktop AMD64. Metadata/elevação isoladas não autorizam. Exigir declaração e confirmação nativa `BINARY_UPDATE_CLOSED` do plano efetivamente apresentado, sem resposta automática.

Parent: `C:\Program Files\Papinho\Accelerator`, identidade esperada `417614100:8257536:71209`; âncora `417614100:131072:226316`. Owner/DACL efetiva, proteção dos ancestrais, ausência de reparse, referências originais e continuidade devem passar em gates frescos.

Executor independente: `C:\Program Files\Papinho\Accelerator\papacc_update.exe`, 104448 bytes, identidade `417614100:30670848:57859`, SHA256 `084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a`. Preservar esse objeto e suas dependências; não atualizá-lo nem usar o payload como executor. Sua instalação/qualificação histórica não dispensa aquisição e proteção atuais. Hash de disco não atesta páginas mapeadas.

Canônico anterior esperado: `C:\Program Files\Papinho\Accelerator\papacc_server.exe`, 882688 bytes, identidade `417614100:1966080:412413`, SHA256 `e3a433c4a6edb565c783fee672bfc82a5794ba44988b123e7a38eb4ec5c9d6b7`. Fonte histórica: `READ-FINAL-IO-COMPLETION-IMAGE-UPDATE-RESULT.json`, `LIMITED_UPDATE_VERIFIED_CLOSED`. Não houve nova consulta ao instalado nesta preparação; esse registro não é lease atual.

Candidato exclusivamente DATA:

`C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe`

**905728 bytes; SHA256 `1ee37a465e6820a5d5967e25630673aadb81d1972a734fb9bf4046ef3b19b2e6`.**

Cópia offline congelada: `papacc_server.fca-first-preparation-905728.exe`, mesmos bytes/hash. O caminho build é passado ao updater existente. Rebuild ou substituição divergentes implicam STOP, nunca atualizar o pin por conveniência. Identidade do DATA será adquirida pelo original nativo; identidade do novo canônico só existirá após criação/readback, não será inventada. Rejeitar alias/hardlink entre executor, DATA, anterior, DLLs e novo candidato.

DLLs apenas conferir e preservar:

- `libssl-3-x64.dll`: 1321472 bytes, SHA256 `3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed`.
- `libcrypto-3-x64.dll`: 7217664 bytes, SHA256 `09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044`.

## Procedimento e gates frescos

Procedimento: `update-fca-first-preparation-image.MANUAL.CANDIDATE.ps1`, SHA256 `0a31f85c452585c79ccffee949208dc0d6966fc79dfd0e917435ee957aa3fc10`. AST sem erros, não invocado; definições dos helpers de read-lock idênticas ao procedimento qualificado anterior. Mantém uma chamada do executor e seus 16 argumentos. Alterações: pacote/pins, tamanho DATA, identidade anterior e vínculo à norma FCA. Não reimplementa updater, staging, SCM, transporte ou namespace.

Antes da chamada, verificar hashes reais do script, pedido e decisão emitida, além dos pins de normas, executor, DATA, anterior e DLLs. Nenhum comando com decisão fictícia ou hash-placeholder deve ser emitido. A aprovação contextual do owner deve ser registrada literalmente, sem alegar que ele forneceu um hash que não forneceu.

SCM original obrigatório: PapinhoAccelerator, OWN_PROCESS 16, DEMAND_START 3, ERROR_NORMAL 1; conta `NT SERVICE\PapinhoAccelerator`; SID_UNRESTRICTED 1; comando exato `"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed`. Exigir STOPPED/PID0/configuração inalterada antes dos efeitos e depois do readback. Não executar Stop para produzir esse estado. Snapshot não impede outro administrador nem prova quiescência OS.

Revalidar identidade/token/contexto, executor original e dependências, parent/ancestrais, proteção, ausência de reparse, DATA finito, hashes, plano, recursos e Stop. Reter referências. O wrapper libera somente sua referência preflight OLD antes da aquisição nativa com DELETE, conforme procedimento já qualificado; não ampliar sharing. O native owner readquire OLD e conserva os originais.

K e R continuam no domínio U4 do executor protegido. O censo FCA de 2880828 bytes não é importado para UPDATE. O executor não foi recompilado/substituído: buffers, argv tipado e composição próprios permanecem os qualificados; o novo payload modifica o número finito de bytes e R, calculado pela geometria factual do volume. Exigir observações frescas e aritmética original, inclusive antes de cada escrita/rename. Não escolher R/K históricos por conveniência ou usar floors FCA como fallback.

## Efeitos permitidos e riscos

Depois da confirmação do plano e consumo irrevogável: UMA criação exclusiva de novo candidato protegido no mesmo parent, cópia finita de 905728 bytes com buffer existente, verificação independente de conteúdo/hash/descritor, flush e readback. No máximo dois renames sem overwrite: anterior para leaf `.previous` exclusivo; candidato verificado para nome canônico. Nomes concretos vêm do CSPRNG/owner da tentativa e são apresentados antes dos efeitos; não são preenchidos antecipadamente. Preservar `RootDirectory=NULL` do protocolo de rename qualificado.

Conferir novo canônico por referência, identidade/tamanho/hash/proteção, anterior preservado, SCM ainda parado e fechamento. Preservar todos os `.previous`, candidatos, originais e fontes históricos. Não remover a retenção anterior nem reutilizar autorizações consumidas.

Riscos residuais: pressão/exaustão de recursos opacos, chamadas longas, falha parcial, intervalo sem canônico, retenção persistente e ausência de rollback automático. Não há atomicidade ou durabilidade universal demonstradas. Stop nega novas submissões, não cancela I/O já submetido. Falha/colisão/substituição/SCM divergente/close inconclusivo implica STOP e UNKNOWN, sem retry, delete, completar automaticamente, restauração ou refund. Fechamento conhecido do UPDATE não resolve histórico ou quiescência global.

Proibido nesta operação: executar o candidato do workspace, criar contêiner/chave/configuração/originais FCA, fixture, READ/E/Q/S/P/endpoint, Start/Stop, UPDATE do executor/DLLs, alterações de SCM/ACL existentes, ProgramData, CA/CURRENT/rootstore, assinatura/exportação privada, rede operacional, TPM/NV/Q4, limpeza, rollback ou autoridades produtivas. PST 0.6.4, HEAD/WIP, históricos, pins e UNKNOWN preservados.

## Reconciliação e marco seguinte

Depois da única execução manual: reconciliar saída integral, primeira falha, consumo, criação, dois renames, anterior, novo ID/hash/SD/readback, SCM, close e retenção. Não repetir se falhar ou ficar inconclusivo. Não registrar sucesso por exit code isolado.

Somente após reconciliação suficiente preparar o pedido próprio FCA, com configuração mínima original protegida, parent de dados original e declarações específicas, sem recriar fixture ou repetir READ consumido. Configuração exigida pelo caller: bytes UTF-8 sem BOM de `format_version 1` e `client_admission DISABLED`, cada linha terminada em LF. O código aceita source protegido no mesmo volume, retido pela custódia de até oito diretórios, inclusive ramo distinto do parent de dados; não confia no path. Identidades e proteção futuras continuam pendentes. Este UPDATE não concede essa execução nem assinatura/CURRENT/Start. P4/K5 e prontidão produtiva seguem não qualificados/DENY.
