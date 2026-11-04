# Suficiência nativa/filesystem — garantias por fase

Task PACC-PHASE6-NATIVE-FS-SUFFICIENCY. Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469. Nenhuma operação protegida executada. D0–D7 accepted preservado, sem novas decisões normativas.

## Conclusão de aplicabilidade

As três classes ainda pendentes representam SUFICIÊNCIA de recursos, não obrigação de provar previamente o sucesso de Create/Write/Flush/close. D3/D4 exigem critério fundamentado antes da admissão e distinguem observado/bounded/opaco; não exigem teto absoluto inventado de todo provider/kernel. A decisão incremental exige capacidade factual além da quota e não aceita host headroom adivinhado ou bound opaco. F3/F5 exigem originais/consumo/revalidação antes do efeito; F6 e W5 regem falha/retidos; F8/W4 exigem transientes e multiplicidade.

Portanto: não exigir que preflight read-only demonstre sucesso futuro, reserva física por free-space query ou inexistência futura de ENOSPC/falha de provider. Tampouco considerar que tratamento seguro de falha prova capacidade suficiente para iniciar. Política de suficiência e protocolo de execução são obrigações diferentes. Nenhuma norma encontrada exige que o OS jamais falhe; nenhuma autoriza iniciar com margem desconhecida apenas porque falhas seriam retidas.

## Matriz antes/durante/depois

| Requisito | Antes de qualquer efeito / preflight fresco | Durante operação autorizada | Fechamento posterior |
|---|---|---|---|
| Autoridade | Ato específico atribuído, plano/pins/alvo/política imutáveis, operador elegível, exclusividade; não reutilizar aprovações | Consumo único antes da primeira submissão; revalidar Stop/ownership antes de efeitos dependentes | Disposição separada, sem rearme ou aprovação retroativa |
| Ancestrais/volume | Known folder, referências originais retidas, identidades/SD/direitos, no reparse, proteção e unidade/quota caller/global | Revalidar originais e identidade de cada filho criado; colisão não é adoção | Manter originais enquanto necessários, close conhecido ou retenção |
| Memória/handles | Quotas geridas e bounds controlados; medidas totais por processo/fase, custo a adquirir e critério de margem aplicável fundamentado | Checagens de aquisição e recursos quando aplicáveis; erro/insuficiência suprime efeitos seguintes | Recursos retidos continuam cobrados; não crédito por término presumido |
| Disco | Tamanhos/hashes exatos, unidades/arredondamento com overflow, espaço caller/global e política que considere metadata/concorrência | Create exclusivo, Write parcial contabilizado; erro de espaço/falha não é autorização de retry | Efeito parcial preservado; não apagar para fingir NEW_INSTALL |
| Escrita/flush | Contrato e caminho de falha qualificados; não resultado futuro | Verificar cada retorno, bytes/progresso, erro, flush por arquivo e identidade; não avançar com resultado incerto | Recibo distingue efeitos/flush verificados de pendência |
| Readback | Inputs originais/pins/hash e lifetime protegidos | Hash/tamanho/identidade/descritores dos destinos comparados após flush | Não implica prova universal de reboot/power-loss ou publicação CURRENT |
| Stop/duração | Critérios por evento/contexto; nenhuma janela numérica arbitrária | Stop entre submissões; chamada síncrona em andamento não é cancelada por flag | Não declarar latência máxima/timeout cleanup sem mecanismo comprovado |
| Retenção/close | Single cohort/buffers/backings e falhas parciais qualificados; custo retido no envelope | Falha após efeitos bloqueia novas operações conflitantes | Um close por referência adquirida; close incerto mantém UNKNOWN, sem double-close/refund |

Exclusividade de criação resolve concorrência pelo MESMO nome; não reserva disco contra outros consumidores. Hash/re-read resolve correspondência de bytes observados; não converte cache/dispositivo em garantia física mais forte que a API.

## Menor envelope demonstrável, sem teto opaco inventado

- Packet fixo7360 bytes no root; cópia4096 e hash1024/contexto existentes aninhados/serializados, sem alocar três arquivos inteiros em memória. Descritores/SID/alocação transitória continuam separados; comprimento aceito após alocação não limita custo antes dela.
- Até16 slots de handles do staging: A ancestrais (A<=8),3fontes,2diretórios,3destinos = A+8. Token/volume/contexto externos, STD emprestados, herança e internos OS não desaparecem dessa conta;16 não é total do processo. A política precisa relacionar custo a adquirir ao processo já existente, sem desconto de baseline.
- Até5 criações do plano,3arquivos sequenciais, um flush e uma verificação final de conteúdo por arquivo no caminho saudável. Escritas parciais exigem progresso positivo e bytes restantes; número de chamadas não é constante. Não prova duração ou custo kernel máximo.
- Censo gerido Selected1.988.704/Legacy1.820.136, mesmos campos e limites; SCM separado quando realmente composto. Não usar PrivateUsage/WS como memória gerida nem somar suas métricas sobrepostas.
- Sum(ceil(file_bytes/unit)*unit) é projeção de payload. Não limita metadata/cache/diretórios nem reserva disco. Produto/soma e espaço caller/global são gates negativos necessários, não prova de suficiência completa.
- O estado retido conserva o mesmo cohort, referências e buffers; falha não cria uma segunda operação para recuperar automaticamente. Custo opaco retido continua material no envelope nativo/FS.

Esses bounds são aproveitáveis. Não existe nos dados atuais um número justificável para teto total de handles, native heap/stack/commit ou margem ativa do FS. Histórico132 ou RAM disponível naquela coleta não fornecem esse número. A tarefa não seleciona ceiling nem eleva quota.

## Correção técnica integrada

Foi encontrada lacuna de Stop imediatamente antes de IO mutante: inner loop verificava apenas console Stop e o flush podia ocorrer após packet Stop recebido na leitura que retorna EOF. Introduzida staging_effect_barrier no include existente: valida owner/invocação/state, phase4/consumed, primeira falha e ambos Stop (console e packet). Chamada antes de CADA WriteFile e imediatamente antes de FlushFileBuffers. Não admite operação nem restaura consumo, não chama serviço e não altera política de capacidade. Corrida após a checagem e chamada síncrona em andamento continuam distintas de cancelamento nativo; não se promete atomicidade com Stop.

Testes de componentes modelam Stop no EOF (flush suprimido), após primeira escrita parcial (segunda escrita suprimida) e após leitura (primeira escrita suprimida). Já havendo criações/efeito parcial, fechamento permanece RETAINED_UNKNOWN; sem fechamento destrutivo. Falhas de flush/conteúdo/exclusividade/IO parcial anteriores continuam exercitadas. Não se considera esses modelos prova de capacidade real.

## Contratos de API consultados

Documentação oficial Microsoft, consultada sem executar APIs do produto:
- [WriteFile](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-writefile): buffer precisa permanecer válido até completar IO; retornos/bytes verificam a submissão. Não fornece teto de memória interna ou duração.
- [FlushFileBuffers](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-flushfilebuffers): retorno e erro são verificados durante a chamada; descreve flush dos buffers, não reserva antecipada ou infalibilidade.
- [GetDiskFreeSpaceEx](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getdiskfreespaceexa): distingue espaço disponível ao caller por quota do espaço livre total. Consulta documental comparativa; código continua consultando volume original por NtQueryVolumeInformationFile, sem troca por consulta de path.

Essas APIs não demonstram por si o envelope opaco; nenhum backend/target foi trocado.

## Qualificação e artefatos

Quatro builds Selected/Legacy Debug/Release PASS, /W4/WX, sem warnings MSVC.17 CTests/configuração,68execuções PASS. Staging51cenários/152checks por configuração; recursos31casos/117checks; demais regressores preservados. Logs/preimages/HASHES.json neste diretório. APIs mutantes/counters/RNG/known folders modelados; SHA/parsing SD em memória reais. Nenhum endpoint, staging, coleta, conta, chave ou diretório operacional criado.

Selected/Release555008bytes SHA256eee02c930d2a59c8b80e43e544b6d71d34b1a3add91f3b4bed46f41c662791d6; não selecionado para operação. Artefato anteriorb835... preservado/conferido em build/staging-observation-preserved-20261031/pre-native-fs-b83581a1 antes do rebuild. Pins/históricos/aprovações consumidas não alterados.

## Primeiro gate real e caminho mínimo

A primeira operação factual útil, depois de política aplicável capaz de alcançá-la, seria aquisição/read-only dos ancestrais e volume por referências originais, incluindo proteção, caller quota/global, recursos e close. Não exige SCM futuro ou prova de escrita bem-sucedida. Não pode ser solicitada como repetição do coletor antigo: guard conservador ainda impede alcançar essa fase e não está removido por esta tarefa.

Antes de uma execução protegida falta fundamentar a SUFICIÊNCIA do envelope/margem para iniciar. Os fatos atuais e contratos de API não a completam; não há pedido operacional byte-exato pronto. Caso uma política de ensaio proponha aceitar risco residual de custos opacos com monitoramento/Stop/falha segura, ela deve explicitar alcance, critérios e residual não observado, com decisão específica quando necessária. Não decorre automaticamente de D0–D7, não se registra nova aceitação nem se cria ADR aqui. Não é necessário provar previamente o sucesso futuro de flush ou fechamento; esses são outcomes a medir sob autoridade.

Caminho: fundamento de suficiência/risco aplicável → qualificação offline da política/gates e eventual revisão do guard conservador → aprovação específica de preflight mínimo se necessário → pedido próprio de staging com gates antes/durante/depois → execução manual autorizada/reconciliação → readback da imagem staged e pedido independente SCM_REGISTER_CLOSED parado. Nenhuma publicação CA/CURRENT, Start/SCQ/KSP ou garantia ED11 completa decorre disso.

Staging NOT_READY/DENY; E21 PENDING; ED11/Phase6 PARTIAL. HEAD/WIP, históricos, pins, UNKNOWN, reservas, chave fixture e PST0.6.4 preservados.
