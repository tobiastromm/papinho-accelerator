# Registro parado — alvo e observação de recursos do controlador

Data: 2026-10-31. Continuação autorizada de
`phase6-scm-original-producer-20261031/NEXT-REGISTRATION-TARGET-AND-RESOURCE-ADMISSION.md`.
Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469; WIP preservado.

## Resultado

Integrada a observação nativa por fases do controlador, no mesmo packet/owner
do ato SCM_REGISTER_CLOSED. As APIs de observação foram qualificadas com falhas
simuladas; **não foram executadas sobre uma preparação nativa real**. Os dados
observados não se tornam política ou autorização. A admissão positiva continua
negada por lacunas explícitas, em vez de inferir capacidade de um contador ou
de um tamanho de struct.

O alvo não foi novamente consultado. Reutilizada a observação anterior de
NOT_FOUND_REPORTED_BY_PROVIDER, sem convertê-la em ausência atual/histórica
comprovada. Arquivo de build não é imagem legitimamente implantada.

Foi delimitado o pacote que precisaria de staging: executável e duas DLLs
app-local, com imports PE, tamanhos e hashes reais. São candidatos de
qualificação API3/PST0.6.5, não distribuição produtiva. Pin de produção0.6.4
inalterado. Nenhum arquivo foi copiado para o destino de instalação.

## Aplicabilidade: evitar outras dependências circulares

| Questão | Resultado e fundamento |
|---|---|
| Processo SCM existente/M1 antes do registro | Não exigido; o ator a observar aqui é o controlador já vivo |
| CA, CURRENT, epoch ou root C8 publicada | Não são fontes da autorização de registro M5/F1–F6; o adapter C8 operacional exige essas provas para outro propósito |
| Parent de dados ProgramData para staging de binários | Não é efeito nem pré-condição dessa implantação de arquivos públicos |
| Token do serviço antes de sua criação | Binding tardio, na fase que efetivamente cria/inicia o processo; não fabricar PID/SID factual |
| TTL numérico do ato de registro | Não identificado como obrigação nesta operação; F4 exige mesma invocação/contexto e invalidações |
| Prazos quantitativos da troca SCQ/TSP | Permanecem no perfil correspondente; não são importados como gate de registro |
| Capacidade factual do host | Obrigatória por ED11 incremental e W2/W4; sizeof ou bytes em disco não a comprovam |
| Custo opaco/retidos sem evidência suficiente | UNKNOWN/DENY; não transformar falta de evidência em custo zero |

Normas: F0–F9, M5/P3, conta virtual e destino accepted, ED11 incremental de
20261012, Windows3MiB W1–W6, OHI/SSO-B/TO0–TO9 nos respectivos escopos. Nenhum
accepted foi alterado ou reaceito. Future Directions de listeners e backup
foram revistas e permanecem independentes/inalteradas. Não há nova ADR.

## Código integrado

`server_scm_registration_resources_win32.inc` consulta somente o próprio
controlador por pseudohandle: GetProcessHandleCount, GetProcessMemoryInfo,
GlobalMemoryStatusEx, GetPerformanceInfo e QueryPerformanceCounter/Frequency.
Não consulta SCM, peers ou credenciais e não abre novos handles controlados.
Não se afirma que chamadas internas dos providers/OS não tenham custos.

O subestado de observação está no ato existente, em
`personal_scm_registration_win32.h`. Conserva primeiro/último sample, máximos
amostrados, mínimos de disponibilidade, frequência/domínio do relógio, primeira
falha e sink não secreto da mesma invocação síncrona. Não é ledger, pool ou
formato persistido. Não aceita valores de capacidade de caller/arquivo/IPC.

Fases no producer existente: ENTRY, ORIGINALS_HELD, CONFIRMED, PRE_CREATE,
BEFORE_CLOSE e CLOSED. ENTRY ocorre **depois** da aquisição/hash inicial do
módulo e do zeroing do root; não é baseline total/pre-C2. O caminho da CLI que
nega antes de SCM tem cinco samples saudáveis; a primitiva positiva modelada
pode chegar a seis. Cardinalidade de queries não é limite de duração.

Queries, overflow, inconsistência ou relógio regressivo negam; não há retry de
observação após a primeira falha. Preservado o guard próprio124; superar esse
guard não é confundido com erro de API. Zero disponibilidade factual de físico
ou commit também nega. Passar esses negativos não qualifica margem positiva.

GetPerformanceInfo é convertido de páginas com subtração/multiplicação checked.
Mudança de frequência, contador regressivo, PageSize zero, commit inconsistente
ou físico disponível maior que total não produzem um sample utilizável.

O relatório de registro agora expõe quantidade de samples, erro e lacunas.
A projeção dos dados não resampleia nem renova freshness. Com observação saudável,
somente a lacuna OBSERVATION é removida. As restantes não possuem setter de PASS:

| Bit | Evidência ainda insuficiente |
|---:|---|
| 1 | Baseline completo/pre-entry: loader, CRT, primeira aquisição/hash do módulo |
| 2 | Peak transitivo: stack, provider/kernel e sobrecusto da própria instrumentação |
| 4 | Margem/capacidade e política de admissão do host devidamente fundamentadas |
| 8 | Envelope de retenção e fechamento incerto |
| 16 | Atribuição/aplicabilidade dos participantes administrativos originais; não inserir um runtime futuro |
| 32 | Create/Change/readback SCM ativos, falhas e custos transitórios |
| 64 | Observação não utilizável/completa para o ato; samples conhecidos não são apagados |

Observação saudável deixa a máscara63; observação indisponível mantém também64.
O campo antigo NEEDS_RESOURCES permanece obrigatório e o retorno da admissão
continua DENY. Não foi introduzido um requisito de TTL numérico para F4.
Consistência de QPC não substitui as invalidações/contexto de F4 nem demonstra
por si observação de todos os eventos de suspensão/continuidade. Não se alega
qualificação nativa desses eventos por estes testes de contadores.

Fechamento conserva sua semântica: falha de contador não é falha de CloseHandle.
É possível registrar recursos UNKNOWN e referências factualmente fechadas.
Fechamento/registro parcial já incerto conserva os dois cohorts e consumo,
sem fabricar sample CLOSED, rearmar o ato ou devolver créditos.

## Qualificação

Selected/Legacy × Debug/Release: quatro builds PASS, zero warnings finais.
52 execuções de CTests PASS,13 por configuração. Incluem observações novas,
producer afetado, lifecycle/SCM modelados, hosts/owner fechado, token/plano e
layout/censo puro. Nenhuma campanha criptográfica histórica reexecutada.

Novo foco:21 cenários/121 verificações por configuração; APIs Windows de
contadores, RNG, aquisição e SCM modeladas. Exercita falhas individuais de APIs,
relógio/frequência, overflow/underflow, dados inconsistentes, guard124/125/129,
ausência de capacidade, primeiro erro sem retry, e close factual apesar de
query falhar. Um físico disponível não nulo de1 byte e um count124 são aceitos
como dados no modelo, mas não como capacidade/admissão: a máscara continua63.
Os valores do fixture não são números operacionais selecionados.

O producer afetado preservou36 cenários/164 checks por configuração. A nova
instrumentação justifica essa regressão; sua aquisição original não foi
reescrita. Não há prova nativa bilateral, SCM/token ou capacidade por esses
modelos. As 32 amostras self históricas do QOC continuam apenas do coletor e
não foram repetidas nem promovidas a evidência do controlador.

Cinco mutantes compilaram e foram rejeitados por comportamento: guard relaxado
para128, erro de query convertido em zero, overflow aceito, observação promovida
a política e admissão com recursos desconhecidos. Fontes restauradas byte-exatas;
rebuild/teste restaurado PASS. A aplicação inicial do último mutante parou por
preimagem/newline divergente antes da alteração; o recorte foi corrigido sem
alterar a implementação. Evidências em MUTANTS.json e logs.

## Quantitativos que realmente existem

| Item | Selected | Legacy |
|---|---:|---:|
| Censo managed compilado | 1981344 | 1812776 |
| Limite managed do perfil | 3145728 | 2097152 |
| Owner SCM separado | 13060 | 13060 |
| Soma estática conhecida | 1994404 | 1825836 |

O subestado novo acrescentou192 bytes ao packet existente, já refletidos no
censo. Arrays/relatórios locais conhecidos são contados no reserve do producer;
não são ceiling do frame compilado/CRT nem reserva física. Limites124/128/1024
preservados, sem crédito fictício ou cobrança duplicada. Não somar private usage
observado ao censo como se ambos fossem alocações independentes.

Private/working set/handles são samples, não peaks universais. Disponibilidade
global não é reserva nem snapshot atômico de todas as APIs. Handles do processo
não cobrem por si toda categoria kernel/GDI/USER, nem o agregado administrativo.
Custos desconhecidos continuam explícitos em QUANTITATIVE-PLAN.json.

## Staging delimitado e primeira prova factual

Imports PE foram inspecionados por dumpbin, sem executar o servidor ou DLLs.
QUALIFICATION-BUNDLE.json contém três arquivos,9.073.152 bytes no total:

- papacc_server.exe,534016 bytes; SHA256
  `afa312e63638c5c0ac77a210c3216f994ed210d6eb3a3708b5d9ac14b70c4dc9`;
- libssl-3-x64.dll,1321472 bytes, pin verificado contra a boundary CMake;
- libcrypto-3-x64.dll,7217664 bytes, pin igualmente verificado.

Não basta copiar somente o EXE. VCRUNTIME/UCRT e imports Windows também existem;
sua resolução efetiva/mapeada não foi autenticada. Não copiar DLLs de sistema por
inferência. O SDK normal/API3 usado é de qualificação PST0.6.5; não é release
produtiva nem atualização do pin0.6.4. Imports estáticos não são attestation.

Primeiro efeito operacional necessário para disponibilizar este candidato no
alvo: staging/proteção **independentes**, de até dois componentes de diretório
sob ProgramFiles e três arquivos, sem overwrite/adoção silenciosa. Não envolve
ProgramData/C8, conta, serviço, Start, CA, CURRENT ou rede. Exclusividade,
originalidade/proteção dos parents/fontes, método de implantação, fechamento e
disposição parcial precisam constar do pedido próprio. Não foram executados ou
aprovados. Inventário não é autorização para criar diretórios ou alterar ACLs.

A menor coleta seguinte que contribui para estes custos é uma preparação do
**controlador**, não M1 do serviço inexistente: após imagem legitimamente
disponibilizada, adquirir/revalidar originais, apresentar/confirmar, registrar
samples e fechar, permanecendo bloqueado antes de OpenSCManager. Tem que ser
separadamente autorizada e pinada; não é prova de custo ativo de CreateService.
Não há comando pronto enquanto staging/bindings/escopo dessa coleta não estiverem
qualificados. Não contornar o guard do destino para executar a partir do build.

Somente depois de fundamentar os custos/margens restantes será possível um
pedido byte-exato de registro parado. O request SCM continua NOT_READY; não
foi inventada aprovação operacional nem pedido executável com placeholders.

## Estado e continuação

Alvo original: NOT_ACQUIRED; metadados anteriores reutilizados, não reconsultados.
Contadores do controlador real: NOT_EXECUTED. Observação: IMPLEMENTED_MODEL_PASS.
Capacidade positiva: NOT_DEMONSTRATED/DENY. E21 PENDING; ED11/Phase6 PARTIAL.
ARTIFACTS.json registra os quatro candidatos de trabalho, sem seleção/freeze.

Sem instalação, diretório/conta/CA operacional, SCM/ACL, Start/Stop, endpoint,
CURRENT, rede, TPM/Q4, limpeza/refund ou recuperação. ADRs, HEAD/WIP, histórico,
baseline planejado, receipt/chave fixture retida e produção0.6.4 preservados.
Legacy compilado no Windows atual não equivale a qualificação NT4.

Próximo escopo: STAGING-SCOPE-AND-FIRST-EVIDENCE.md e
NEXT-QUALIFICATION-STAGING-REQUEST.md. Não retomar implementação do producer,
TTL fictício, TPM ou root histórica.
