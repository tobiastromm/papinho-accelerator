# Fontes da política própria de staging — consolidação

Task PACC-PHASE6-STAGING-POLICY-SOURCES-AND-ADMISSION. Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469. WIP anterior preservado; sem commit/reset/clean. D0–D7 permanece ACCEPTED, sem nova aprovação necessária para esta correção técnica.

## Aplicabilidade por pendência

| Termo anterior | Fonte accepted | Materialidade e tratamento |
|---|---|---|
| Envelope nativo/transitório | D3/D4; F8; W4 do perfil3MiB; decisão de envelopes incrementais | Material: custo do próprio controlador, loader/CRT/OS, aquisições, descritores, stack e operações FS não se limita ao tamanho do packet. Não há ceiling/fonte suficiente. Mantido UNKNOWN. Custos retidos pertencem também a este envelope. |
| Margem do host | D3/D4 memória; decisão incremental: capacidade factual além da quota | Material: disponibilidade após preparação precisa suportar próximos custos, concorrência e retenção. Amostra histórica ou valor positivo não prova suficiência; margem sem fundamento não pode ser inventada. Mantido UNKNOWN. |
| Retenção | D4 retenção/fechamento; F6/F8; W5 | Ownership/single-cohort e conservação do packet já demonstrados por código/modelos. Não é sempre uma fonte externa ausente: falha retida/owner unresolved é condição concreta de DENY. Custo nativo/FS retido não foi provado nem retirado do orçamento; permanece nas respectivas classes. |
| Cohort administrativo | D0/D1/D3 e D4 handles; decisão incremental de atribuição/host | Não existe exigência de peer SCQ, auxiliar TPM ou serviço futuro neste efeito independente. Há um controlador canônico amostrado e um contexto externo de execução; seus custos não são gratuitos. Corrigida a pendência incondicional de cohort: identidade/continuidade do owner precisa ser válida; custos externos não isolados permanecem na margem do host e atribuição nativa. Não se aplica franquia1024 por herança. |
| Metadados/efeitos FS | D4 volume/transitórios; F8; W4 | Material: dois diretórios e três arquivos criados, alocação/cópia/flush/hash/re-read e retenção parcial. Tamanhos de conteúdo não limitam metadados/cache/IO. Mantido UNKNOWN. |
| Observação | D4 handles/memória/tempo | Fato fresco: samples conhecidos, clock consistente, ausência de erro e valores de memória válidos são necessários. Não suficientes para capacidade. |
| Volume | D4 volume; D5 originais/proteção | Fato fresco: volume retido, unidades/caller quota/global e projeção do pacote. Ainda não observado nas coletas reais. Acima do payload arredondado não é admissão. |

Nenhuma norma exige inventário global de cada handle do OS como condição autônoma de staging. Atribuição detalhada continua útil apenas se uma redução específica depender dela; não deve substituir a definição de uma política de suficiência. Mantidos guards conservadores124 enquanto a política completa não os puder substituir; LAB/TPM/SCQ intactos.

## Correção integrada

server_staging_resources_win32.inc: staging_policy_missing passa de cinco bits obrigatoriamente UNKNOWN para três classes materiais obrigatoriamente pendentes (native/host/FS). Retenção e ownership administrativo agora são pendências condicionais derivadas do estado factual do owner (unresolved/RETAINED/phase8; thread/invocation/state). A máscara saudável parcial passa de31 para19. Observação/volume ainda acrescentam bits32/64 quando insuficientes. missing inicial115 exclui condições4/8 ainda não observadas; os números são bits privados, não quotas nem autoridade. Mensagens históricas/mask95 não são reescritas.

Nenhuma pendência de custo foi convertida em PASS: a conservação estrutural dos recursos é distinguida de seu custo, ainda UNKNOWN nos envelopes nativo/FS. Não se supõe custo zero do pai/console: sua interferência está na margem factual do host, não em uma topologia LAB transplantada. staging_admit mantém ownership/phase/consumo/Stop/primeira falha e nega política material incompleta. Caller zerar bits não concede capacidade. Não há setter/grant positivo, novo owner/pool/ledger ou API operacional.

## Evidência suficiente no seu alcance

- Bound de estrutura: packet7360, copy4096 e hash1024 já aninhados,16 posições de objetos (até8 ancestrais,3fontes,2diretórios,3destinos), contexto/token/volume externos adicionais. Até5 criações do plano, arquivos serializados; número de writes não é fixo devido a IO parcial. Não são16handles totais nem teto de RAM/FS.
- Censo Selected1.988.704/Legacy1.820.136 geridos. Sem campo físico adicionado; não é PrivateUsage ou envelope de processo completo. Preservar SCM13.060 quando a composição realmente o inclua, sem somar duas vezes arrays compartilhados.
- Modelo de ownership/retention: uma invocação canônica ativa; referências originais, ausência de adoção, consumo único, Stop, partial IO/close e RETAINED_UNKNOWN já exercitados. Não prova fechamento/quiescência do OS real.
- Medições109→132 e memória são históricas pontuais de preparação sem efeitos; não reservas ou ceilings. Primeira falha/autorizações consumidas preservadas.

## Qualificação desta alteração

Quatro builds Selected/Legacy Debug/Release PASS sem warnings MSVC (/W4 /WX);17 CTests/configuração,68 execuções PASS. Fontes/preimages/hashes/logs preservados neste diretório. Staging48cenários/141checks e recursos31casos/117checks por configuração; novos checks verificam ausência de cohort imposto a owner válido, divergência de thread, owner unresolved e phase8 retida, sempre sem creates/writes/consumo operacional. Demais negativos de máscara caller, recursos insuficientes/UNKNOWN, Stop, perda de ownership, fechamento incerto e ausência de autoridade preservados. APIs mutantes e contadores modelados; parsing SD e SHA em memória reais. Nenhuma coleta/cópia protegida executada.

Novo Selected/Release555008bytes SHA256b83581a1ea7d894b6751b4fd967465ee12f34094f2ca15f28a9af43b730369f5, não selecionado para execução. Binário anteriorfdc3... preservado e conferido antes do rebuild em build/staging-observation-preserved-20261031/pre-policy-sources-fdc3c7f0. Não atualizar pins/aprovações antigos.

## Menor prova factual e limite do read-only

Primeiro bloqueio de POLÍTICA: ainda não existe critério fundamentado de suficiência do envelope nativo/FS e da margem. Isso não se resolve coletando novamente apenas contagem ou free-space. Nenhum pico observado pode suprir a política como teto universal; não se pede nova aprovação normativa genérica.

Primeira lacuna NATIVA específica do alvo: identidade/proteção original dos ancestrais ProgramFiles e geometria/espaço caller/global do volume, durante a mesma aquisição governada. É obtível em princípio por preflight read-only com FILE_OPEN/queries no owner existente, sem criar destino ou usar serviço futuro; os mecanismos já existem no produto. Deve registrar origem/contexto/erros, descritores e alocação observada, reparse/identidades, orçamento da própria observação, contadores e fechamento/retidos. Não ler ownerAuth/chaves. Não exigir enumerar toda máquina.

Esse preflight NÃO demonstra custo ativo de Create/Write/Flush/metadados/cache nem reserva futura de disco/host. Além disso, o coletor atual ainda corta antes dessas fases com o guard conservador124; não reutilizá-lo como se pudesse cumprir o objetivo nem aprovar contorno implícito. Antes de emitir pedido executável, é necessário fundamentar o envelope/critério de risco aplicável e qualificar o caminho de observação sob essa política. Não se emite novo pedido ou comando prematuro para repetir o mesmo corte.

Se a garantia pretendida exigir limite absoluto de todo custo opaco do OS/FS, não há evidência read-only existente que o prove. Esse nível de garantia não pode ser substituído silenciosamente por uma observação ou por capacidade positiva no momento. A lacuna precisa é de fonte de suficiência para o custo opaco, não de um processo SCM inexistente, secret ou inventário LAB. Eventual evidência ativa requer autoridade própria e efeitos explicitados; não está autorizada aqui.

## Caminho mínimo

Concluir fonte/justificativa específica de envelope/margem/FS a partir do modelo por fase e garantias realmente suportadas; se não for possível, explicitar essa fronteira sem fingir que outro snapshot a resolverá. Somente depois qualificar eventual revisão do guard conservador, preparar um preflight read-only específico para originais/volume se ainda necessário e obter autorização própria. Com fatos frescos e política suficientes, emitir pedido byte-exato de staging. Após staging aprovado/executado/readback, pedir separadamente SCM_REGISTER_CLOSED parado; nada disso concede Start/SCQ/CNG/CA/CURRENT.

Pedido de staging NOT_READY. E21 PENDING; ED11/Phase6 PARTIAL; DENY, UNKNOWN, reservas, HEAD/WIP/históricos/pins/chave fixture/PST0.6.4 preservados.
