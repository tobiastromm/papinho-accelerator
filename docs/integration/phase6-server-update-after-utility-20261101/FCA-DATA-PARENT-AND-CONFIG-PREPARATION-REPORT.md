# FCA — parent de dados e configuração original: preparação offline

2026-11-03. Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469; WIP preservado. Nenhum código do produto ou contrato accepted foi modificado. Sem execução nativa, UPDATE, criação protegida, SCM, CNG/key, endpoint, Start/Stop ou alteração de ACL nesta etapa.

## Estado factual e política

A imagem FCA reconciliada continua 905728 bytes, ID417614100:2555904:1047601, SHA2561ee37a465e6820a5d5967e25630673aadb81d1972a734fb9bf4046ef3b19b2e6. O hash do candidato congelado no workspace foi reconferido; não foi feita nova aquisição do EXE instalado. O UPDATE e sua autorização permanecem consumidos.

`personal_install_paths_win32.inc:pip_resolve` resolve ProgramFiles e ProgramData por KnownFolder DONT_VERIFY e deriva parent ProgramData/Papinho, leaf Accelerator. `personal_first_open` exige esse parent original preexistente, captura sua identidade e reabre volume/componentes relativos por OBJ_DONT_REPARSE, retendo source e ancestrais. FCA4 permite somente filho exclusivo e chave; não autoriza criar/remediar o parent desconhecido. A fonte pode estar em outro ramo protegido no mesmo volume, dentro dos oito slots totais; um futuro source sob ProgramFiles/Papinho/Accelerator consumiria três slots adicionais após a cadeia do parent e o volume compartilhado.

A busca das evidências relacionadas encontrou o SD histórico de C:/ProgramData em `../n0start03-protected-dossier-qualification-20261024/readonly-constituents-result.json`: owner SYSTEM, DACL protegida, ACE4 de BUILTIN Users mask0x116 aplicada ao objeto. A função atual `scqi_directory_sd` rejeita escrita por esse trustee: 0x116 intersecta 0x500d0156; a exceção AU self-only0x4 vale somente no volume e não nessa ACE. A projeção foi reproduzida offline, inclusive contra a função gerenciada previamente qualificada do materializador. Não se atribui isso à ACL atual, nem a uma falha nativa FCA que ainda não ocorreu. O histórico tratava PapinhoAccelerator, não demonstra o parent distinto Papinho. A falha histórica ANCESTOR_PROTECTION do workspace não é prova sobre ProgramData.

## Integração mínima

Foi preparado `fca-parent-readonly.cs`, adapter de observação DATA pelo mesmo padrão existente de leitores nativos e wrapper CodeDom manual. Não é owner, serviço, canal, transporte, seletor ou autoridade novos. Usa somente KnownFolder ProgramData, resolução nativa direta e aberturas relativas fixas FILE_OPEN, access0x1200a0/share1/options0x200021/OBJ0x1040. Retém e revalida identidade/SD dos componentes; não segue reparse, enumera diretórios ou abre o leaf Accelerator. Query NT OWNER|DACL sizing/fill limitado a64KiB por chamada. Raw NTSTATUS preservado. Projeção exporta apenas hashes e metadados limitados; não dados privados ou SD bruto.

O observador admite continuar a leitura fixa depois de uma projeção de proteção REJECT para identificar o parent; isso nunca concede admissão FCA e não altera a função produtiva. Falha nativa/Stop/substituição interrompe sem retry. Custos internos CLR/Shell/kernel/hash UNKNOWN. Oito handles próprios de diretórios no máximo; demais recursos não são excluídos nem anunciados como teto nativo. Pending/retorno inconclusivo/close incerto retém o adapter pelo mesmo padrão de contenção, sem kill/limpeza. Close conhecido refere-se apenas aos handles próprios do observador.

Limitação deliberada: token/processo diferentes, sem ProgramFiles KnownFolder, sem direito ADD_SUBDIRECTORY que o owner FCA exige, sem source/config/runtime/capacidade FCA. Não é reprodução integral do futuro ato. Match da projeção não autoriza criar nem fornece continuidade futura; owner real terá que readquirir e validar todos os originais e recursos.

## Configuração e qualificação

`FCA-FIRST-CONFIG.DATA.txt`: exatamente43 bytes UTF-8 sem BOM, LF, `format_version 1` e `client_admission DISABLED`. SHA25616c4847a27d32689ad7eb2c0bd29f6f0f698df74092e9df107cc8203e07e0ae7. É apenas DATA no workspace; original protegido NOT_MATERIALIZED. Sua eventual cópia depende de pedido próprio e do gate de parent, não será criada por esta leitura.

Qualificação final: 63 verificações PASS, zero warnings, CodeDom .NET Framework/Windows PowerShell existente. Mock IO para resolve/open/inspect/revalidate/close; falhas em dez pontos, Stop, replay, close UNKNOWN, formato de caminhos, política histórica, perfis de ACE e allowlist das nove importações nativas. Nenhuma chamada do adapter nativo; os testes não são prova do comportamento factual do filesystem/NT neste host. Layout AMD64 conferido em memória. Wrapper AST sem erro e não invocado. Manifesto e log em FCA-DATA-PARENT-READONLY-QUALIFICATION.json / fca-parent-readonly-offline-qualification.log. Revisão separou rejeição de metadados de falha de API para não propagar GetLastError obsoleto. Nenhum rebuild ou campanha inalterada do produto repetido.

## Pedido e primeiro gate

Pedido pronto: ONE-FCA-DATA-PARENT-READONLY-REQUEST.CANDIDATE.md, SHA25639cf3699b037983e9bfd84959740b170eab502b2c6a1a319f27215f9490e7ae3. Procedimento: observe-fca-data-parent.MANUAL.CANDIDATE.ps1, SHA2565c27d2fdacc319d4c0b6c6d5a0b73b54ee9f4f76fdf12ec6645ac772bdd951be. Comando exato em FCA-DATA-PARENT-READONLY-MANUAL-COMMAND.md, aguardando autorização específica para a única observação. Não executado.

O primeiro gate restante é evidência atual focal do parent/ancestrais: identidade, proteção e revalidação ou primeira falha de abertura. Depois, se elegível, preparar materialização mínima legítima de source e pedido FCA; se houver rejeição, apresentar somente decisão focal indispensável, sem mudar quotas, ACL, localização ou política para PASS. Não repetir UPDATE/READ/fixture ou consumo anterior. Sem novo Start como investigação genérica. P4/K5 não qualificados, readiness DENY e retenção/quiescência histórica UNKNOWN. PST0.6.4 preservado.