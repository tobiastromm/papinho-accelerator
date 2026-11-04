# QOC — quantitativos e originais SCQ: fechamento autorizado e prova mínima

TASK PACC-PHASE6-TSP-QUANTITATIVE-AND-ORIGINAL-PLAN-CLOSURE, 2026-10-30.
Branch main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preservados.

## Resultado

Foram obtidas medidas novas de Windows real e preparada a menor prova nativa
seguinte com código/artefato/procedimento exatos. Não foi possível completar
legitimamente o envelope ou os originais operacionais SCQ dentro das restrições:
serviço/processo, pedido e decisão originais não foram adquiridos; SCM não foi
consultado. Não fabricar aprovação ou transformar observações em concessão de
capacidade é parte explícita do resultado. Pedido SCQ NOT_READY/DENY.

O pedido M1 atual está pronto para decisão independente, não aprovado:
[QOC-M1-RESOURCE-OBSERVATION.CANDIDATE.md](QOC-M1-RESOURCE-OBSERVATION.CANDIDATE.md),
SHA256 `edfbe8428089699dffbaafb4e0292a20d8eb46732bf1fe1d82d1e113f72647a3`.
É observação de metadados/recursos, sem sessão SCQ ou efeito de controle.
O rascunho anterior de queryex fica histórico e não é outro pedido a aprovar.

## Código efetivamente alterado

O censo existente tests/phase6_memory_census_test.c preserva o modo padrão de
sizeof/limites e acrescenta duas modalidades explícitas de observação:

- --self-observation: apenas processo coletor e contadores globais públicos.
  Executado em Windows real nesta tarefa; nenhum objeto SCM é aberto.
- --service-resource-observation: modalidade M1 futura, compilada e testada com
  APIs simuladas. Abre somente o SCM local CONNECT, serviço exato QUERY_STATUS e
  referência de processo QUERY_LIMITED_INFORMATION|SYNCHRONIZE. Não tem Start,
  controle/configuração/criação, acesso VM_READ, key/CA, endpoint ou autoridade.
  Revalida PID/criação/liveness/status antes/depois das8 amostras; falha na
  primeira consulta/divergência. Sem processo disponível, encerra sem aguardar
  ou criar participante. Fecha referências uma vez; close incerto mantém as
  restantes e registra UNKNOWN sem retry/rearm.

Somente links/targets de qualificação foram alterados no CMake: psapi/advapi32
para o censo e um teste de APIs simuladas. Nenhum owner/runtime/produtor SCQ de
produto, decisão accepted, N0, protocolo/ROOT/CURRENT ou formato persistido foi
alterado. Não se criou serviço/owner/root/endpoint ou infraestrutura paralela.
O contexto estático de3 referências é custódia do coletor de metadados, não uma
autoridade/ledger ou um novo owner do produto.

## Medições demonstradas, não limites operacionais

Campanha final:4 coletores,8 amostras cada,32 observações reais. Todos os headers
registram COLLECTOR_SELF_NOT_SCQ, SCM NOT_QUERIED, KSP NOT_EXECUTED e DENY.
A campanha inicial, anterior à integração M1, fica separadamente preservada em
QOC-INITIAL-* e QOC-PRE-FINAL-*; não misturar versões nem tratar a repetição como teto estatístico.

| Grandeza | Campanha final Windows real | Aplicabilidade |
|---|---|---|
| Handles do coletor |59 em todas as32 amostras | Não são handles do serviço, controller SCQ ou agregado administrativo |
| Commit privado do coletor |860160 a1429504 bytes | Memória observada, não custo fixo/ceil de SCQ |
| Working set do coletor |4136960 a4743168 bytes | RSS observado, não propriedade exclusiva de RAM |
| Grupo de consultas |2,7926 a4,6586 ms | Inclui instrumentação, não TTL nem duração global |
| Intervalo total de coleta |Máximo observado4,6593 ms | Não é max age garantido ou limite universal |
| Físico disponível global |Mínimo observado6667358208 bytes | Não reservado; pode mudar após a consulta |
| Commit disponível global |Mínimo observado9419870208 bytes | Não reservado; limite do sistema pode variar |
| Frequência QPC |10000000 ticks/s | Relógio observado e domínio preservado, não autoridade |

O commit privado aumentou dentro de cada coletor durante as amostras. Não se
atribuiu essa diferença a um provider/kernel específico nem se presumiu coleta
sem custo. A versão inicial observou47 handles e a final59; isso não autoriza
transportar qualquer uma dessas contagens ao servidor. O observador faz parte
do custo e seus componentes opacos continuam a exigir tratamento próprio.

Censo TSP do produto e limites accepted continuam: managed Selected1979784 e
Legacy1811216, quotas3145728/2097152; transporte728,driver1888,buffer1076;
SCM separado12924; caps124/128/1024. Wire4252 e soma read/write8504 são bounds
algébricos do protocolo, não medições de latência ou custos nativos. Nenhuma
margem/quota foi aumentada ou crédito concedido.

## Valores que continuam UNKNOWN

QOC-QUANTITATIVE-RECONCILIATION.json conserva null para validade/TTL,
max age/freshness, validade/duração de coleta SCQ, duração global/encerramento,
headroom físico/commit e custo ativo provider/kernel. Permanecem não demonstrados:

- Preparação completa dos originais/token/SCM/imagem e seu tempo no contexto
  legítimo; a coleta self não executou esses passos.
- Endpoint/IO parcial/pending/RNG/hash/retidos, concorrência e participantes
  reais, compartilhamento/aliases e custos transitórios Windows do caminho SCQ.
- Stop/CancelIo/fechamento e tempo de retenção nativos. Modelos provam tratamento
  lógico, não latência/garantia física ou devolução de reservas.
- Headroom de todos os atores e composição administrativa. Snapshot global e
  contagem do coletor não são reserva, fencing ou fonte de capacidade.

A validade por evento/contexto e janela finita permanece no código; não se
inventou valor. A expiração nega efeitos e pede Stop, não prova conclusão.
Nenhum número LAB/Browser/D4–D5/TPM foi copiado. M1 também não é uma exceção a
envelope SCQ: observa metadados públicos e não entra na admissão SCQ.

## Originais e bindings reconciliados

QOC-ORIGINAL-BINDINGS.csv descreve o estado de cada um dos16 bindings e suas
fontes/gates. A resolução Windows x64 DoNotVerify observou, sem criar diretórios:

- Binários: `C:\Program Files\Papinho\Accelerator`.
- Parent: `C:\ProgramData\Papinho`; contêiner Accelerator.
- Dados: `C:\ProgramData\Papinho\Accelerator`.

São DATA observados, não alvo congelado, existência/publicação, owner, RAW1,
identidade de parent ou direitos sobre objetos. O resolver canônico existente
papacc_personal_paths_new/current deve revalidar no packet governado antes da
confirmação/efeitos; nenhum parent, root C8 ou imagem protegida foi adquirido.

Propósito2/key vazio e nomes de serviço/conta/layout são regras conhecidas.
Pedido/decisão operacionais específicos ainda não foram emitidos. O presente
prompt é autorização técnica e a proposta M1 é outra observação, não a decisão
SCQ. Nenhum documento de teste/norma foi promovido a pedido/decisão original.
Os3 getters operacionais continuam NULL, exatamente como antes.

O producer/receptor existentes exigem arquivos por handles originais, identidade,
conteúdo/descritor/proveniência e correspondência da política/plano, mais
confirmação atribuída. Conteúdo CNG dos originais não foi lido/hasheado nesta
tarefa. Metadados/hash/path/SID resolvido não completam essa autoridade.
PID/token/grupos/criação/imagem/serviço são gates frescos na sessão, não valores
pré-fabricados. A prova M1 não consulta TokenUser/grupos/imagem nem autentica o
runtime: o PID reportado pelo SCM é apenas candidato. Ela limita continuidade
pelo handle retido, mas não substitui o binding original de admissão.

## Menor prova factual necessária e alcance

M1 consolida em UMA invocação do censo a disponibilidade/status, aquisição de
referência, preparação,8 intervalos/contadores do processo candidato e do coletor,
memória global e fechamento. Não muda o serviço. O procedimento, hash e efeitos
estão preenchidos no pedido; não há comando com placeholders ou execução agora.

Se o serviço estiver ausente/parado/inacessível, a primeira evidência é essa
indisponibilidade, não autorização para install/Start/conta/fixture. Disponibilizar
um runtime legítimo exigiria decisão operacional própria; não é pré-requisito
inventado de CA/root histórica nem efeito implícito. Reutilizou-se o histórico
1060 apenas como histórico; não se executou consulta redundante nesta tarefa.

Se houver processo observável, M1 fornece baseline e custo de preparação/coleta.
Não basta para a sessão toda: limites/margens e termos ativos endpoint/IO/Stop/
retidos ainda exigem evidência ou garantia aplicável. Uma amostra não define teto.
Não se promete que essa única saída completará todos os UNKNOWN; ela resolve o
primeiro fato de disponibilidade e as contagens diretamente acessíveis sem
executar sessão ou criar infraestrutura.

## Qualificação e estado do artefato

Quatro builds DO QUALIFICADOR Selected/Legacy Debug/Release PASS, zero warnings
finais. Oito CTests afetados PASS (censo padrão e15 falhas/modelos em cada perfil).
Cobre acesso negado, serviço ausente/sem processo, tipo compartilhado, PID/criação
substituídos, fim do processo, mudança de estado, falha de clock/counter e close
incerto sem retry. O helper não aumenta direitos para obter PASS. Ordem de
includes Winsock2/Windows do fixture foi corrigida antes dos resultados finais.

As32 medidas self são Windows real; os15 casos SCM de cada configuração são
MODELOS. SCM nativo NOT_EXECUTED, autenticação bilateral NOT_DEMONSTRATED.
A campanha TSP de transporte/readiness permanece válida e não foi repetida por
não haver mudança no produto. Não se usou contagem de testes como capacidade.

O servidor Selected/Release permanece candidato de trabalho, SHA256
676071a5b7db08048e9ae491896788d0a739389898ca883f0755d9e08c060c02.
Seus bytes e os outros3 servidores compilados não foram alterados. Nenhum
congelamento/seleção operacional foi solicitado. O coletor M1 é outro artefato,
qualificado exclusivamente para a observação candidata, não a imagem do servidor.

## Caminho restante

Primeiro gate: aprovação específica da M1 somente se o proprietário quiser essa
observação SCM, respeitando a proibição atual. Após saída manual, reconciliar
primeira falha/continuidade/close e apenas os valores efetivamente demonstrados.
Completar evidências/limites que faltarem, o plano byte-exato e seus originais;
só então congelar artefato final, apresentar UMA seleção e um pedido SCQ com
endpoint/lifecycle/SCM/imagem/parent separados conforme necessidade. KSP/CNG não
faz parte de TRANSPORT_ONLY. NEW_INSTALL/CA_INITIALIZE/publicação mantêm gates
próprios posteriores; não retomar TPM/Q4 ou criar uma CA prévia para transporte.

Próximo prompt: NEXT-QOC-M1-OWNER-DECISION-AND-RECONCILIATION.md.
Nenhuma nova ADR, aprovação, intent, serviço, diretório operacional, conta,
CA/CURRENT, endpoint, sessão, Start/Stop, ACL, rede produtiva, TPM/Q4, cleanup/
refund ou recuperação ocorreu. Históricos, chave fixture, UNKNOWN/reservas e
PST0.6.4 preservados. Pedido SCQ NOT_READY; E21 PENDING; ED11/Phase6 PARTIAL;
autoridade DENY.

## Referências primárias dos contadores/direitos

[GetProcessMemoryInfo](https://learn.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-getprocessmemoryinfo)
e [GetProcessHandleCount](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getprocesshandlecount)
permitem query limitada em Windows moderno; não se requisita VM_READ.
[PERFORMANCE_INFORMATION](https://learn.microsoft.com/en-us/windows/win32/api/psapi/ns-psapi-performance_information)
distingue páginas físicas e commit e informa PageSize; disponível não é reserva.
[SCM access rights](https://learn.microsoft.com/en-us/windows/win32/services/service-security-and-access-rights)
e [CloseServiceHandle](https://learn.microsoft.com/en-us/windows/win32/services/scm-handles)
fundamentam direitos de consulta e fechamento, não identidade/custódia SCQ.
