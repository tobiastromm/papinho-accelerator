# UPDATE do EXE existente — decisão focal necessária

Status: CANDIDATE / NÃO ACEITA. Preparação técnica autorizada em 2026-11-01;
nenhuma aprovação normativa ou operacional inferida. Não é outra família de
ADRs. Este é um único ponto de decisão sobre um efeito ainda não coberto.

## Fundamentação e alternativas

F0 delimita NEW_INSTALL como distinto de UPDATE; F1/F3/F5 não constituem fonte
genérica de autorização de substituição. O ato UPDATE v3 U0–U7 accepted é sobre
CURRENT/base v2/CP2 ou v3/CP3 e tuplas de configuração, não sobre EXE Windows.
F8, a admissão incremental ED11 de 2026-10-12 e W4 mantêm DENY quando faltam
custos materiais exigidos. Não há extensão accepted dessas reduções a UPDATE
binário: B2 somente B0/B1; R2 somente SCM_REGISTER_CLOSED; G somente Start.

A — manter garantia integral: preservar DENY até obter uma fonte suficiente
para todos os custos materiais e qualificar o ato específico de substituição.
É a política corrente, sem alteração normativa. Medir novamente um pico não
provaria máximo de RPC, filesystem/cache, kernel ou fechamento; não se recomenda
outra coleta inconclusiva para tentar obter essa garantia.

B — recomendação para decisão: um único UPDATE limitado do EXE diagnóstico
staged, com ato independente e preservação do EXE anterior. Aceitar somente as
regras U1–U7 abaixo, indivisíveis. Não é garantia de disponibilidade/conclusão.
Escolher B exige aprovação explícita desta proposta exata antes de código
positivo dependente. Esta proposta não seleciona hashes futuros ou execução.

## U1 — Escopo e fonte

Somente PERSONAL_LOCAL, PapinhoAccelerator registrado e parado, conta virtual,
DEMAND_START e comando fechado já registrado. Atualizar apenas papacc_server.exe;
DLLs idênticas são adquiridas/validadas, nunca copiadas. Não alterar configuração
ou ACL SCM, dados, CA, CURRENT ou identidade administrativa da instalação.

Reutilizar no mesmo owner os mecanismos de identidade/token/declaração,
apresentação, CSPRNG de tentativa e confirmação atribuída de F1–F6, com propósito
explicitamente distinto BINARY_UPDATE_CLOSED. Não reutilizar os atos consumidos
de staging/registro/Start. A política é fixa fora dos pins/arquivos candidatos;
estes são DATA. O ato nomeia serviço, alvo original, bytes/pins antigos e novos,
DLLs, objetos retidos, efeitos, riscos, invocação e lifetime. Nenhum grant ou
formato de autorização persistido, root, serviço ou ledger adicional.

## U2 — Proteção que permanece obrigatória

Known folder e caminho original da instalação existente, token/admin/contexto,
ancestrais, ausência de reparse, owner/DACL efetiva e originalidade/continuidade
de cada objeto. Reter originais desde captura até disposição factual. Pins não
substituem identidade nem atestam imagem mapeada. Guardas de request, decisão,
READ, SCQ e demais perfis não mudam. Direitos mínimos para cada operação;
não ajustar proteção existente para conseguir atualizar.

SCM original deve corresponder ao mesmo comando, conta, tipo, SID type e destino,
com SERVICE_STOPPED e PID zero em gates frescos. Snapshot não impede outro
administrador de agir nem prova quiescência OS. Conflito, perda do lifetime ou
Start observado impede nova mutação. Não chamar Start/Stop para corrigir estado.

## U3 — Substituição conservadora proposta, sem atomicidade/rollback

Recomenda-se preservar o EXE antigo como objeto, em vez de sobrescrever seus
bytes. Preparar um novo arquivo exclusivo no mesmo diretório protegido, copiar
somente o novo EXE com buffer único limitado, verificar bytes/hash/descritor e
flush. Manter original antigo protegido, sem escritores/DELETE concorrentes
concedidos por compartilhamento permissivo.

Após confirmação/consumo e gates, a sequência futura qualificada poderá mover
por referência o EXE antigo para um leaf de retenção exclusivo e mover o novo
EXE verificado para o nome canônico, sem overwrite/adopção. Nomes exatos devem
estar no plano atribuído antes da primeira mutação; a captura de uma identidade
de filho só ocorre depois da criação exclusiva. Nenhum nome é autoridade.

Não presumir atomicidade entre passos, durabilidade de rename/metadata ou
rollback. Pode haver um intervalo com caminho canônico ausente e falha parcial
com arquivos anterior/novo retidos. Isso mantém Start bloqueado pelo fluxo
governado; não é fencing contra administrador externo. Não apagar o anterior,
não desfazer renames, não recriar o nome nem rearmar o ato automaticamente.
Escolha/qualificação exata da API de rename e seu readback precedem pedido
operacional; ReplaceFile/Copy-Item não são atalhos autorizados por este texto.

## U4 — Redução exata de garantia e recursos

Somente neste ato, especializar a suficiência prévia de custos opacos de
loader/CRT, descritores/hash, alocador, consultas SCM/RPC, filesystem/cache,
metadados/flush/rename/kernel e retenção opaca sob ED11 incremental, W4 e F8.
Continuam UNKNOWN, não zero ou máximos demonstrados. Aceita-se risco de exaustão,
bloqueio, chamadas síncronas longas e resultado parcial. Sem aceitação de U1–U7,
essa exceção não existe e a admissão permanece estrita/DENY.

Custos controlados, censo, reservas, aritmética, multiplicidades e retenção
continuam obrigatórios no orçamento compartilhado selecionado (Selected
3.145.728; Legacy 2.097.152 bytes), sem novas quotas. Limites 124/128/1024
continuam nos domínios accepted; não são copiados para UPDATE nem aumentados.
Os valores a seguir são propostas de tolerância de risco, não capacidade:

- K é a reserva conhecida completa do caminho UPDATE final, incluindo packet,
  caller, buffers, frames nomeados, referências e coexistência/retidos, censada
  depois da implementação. Não usar só o tamanho do novo EXE como K.
- Recomenda-se física e commit disponíveis >= K em observação fresca antes de
  cada submissão mutante. É margem nominal fraca, não reserva de host nem limite
  de opacos. Falha de amostra, insuficiência ou incoerência nega o próximo efeito.
- Contar handles totais, inclusive herdados, e inventariar referências próprias.
  A composição terá originais/ancestrais/DLLs, fonte, anterior, novo exclusivo e
  dois contextos SCM de consulta. Não supor SC_HANDLE igual a um handle kernel
  nem transformar contagem observada em ceiling. Nenhum total inventado.
- R é o payload novo arredondado pela geometria factual do mesmo volume, com
  aritmética verificada. Antes da criação, caller-free e actual-free >= R;
  antes de cada escrita, verificar o payload restante arredondado. O anterior
  já alocado é retido e não é contado como nova cópia. Metadados/alocação/flush
  permanecem risco opaco explícito, não coberto por R.
- Freshness por evento/contexto: amostra e revalidação imediatamente antes de
  cada efeito, depois de confirmação humana e de qualquer chamada anterior.
  Não propõe TTL ou duração máxima numéricos sem fonte; ausência de limite de
  latência/cancelamento imediato é parte explícita do risco, não valor zero.

Essa proposta é própria de UPDATE, não importação automática de B3/R3/G3.
Monitoramento não garante ausência de pressão/exaustão entre amostras.

## U5 — Consumo, Stop e número de tentativas

Uma tentativa viva por owner, confirmação específica e consumo irrevogável
antes da primeira criação do arquivo novo. Revalidar plano/contexto/originais,
recursos e Stop antes de criação, cada escrita, flush e cada rename. Uma criação
exclusiva nova e no máximo dois renames previstos; cópia finita pelo tamanho
congelado, com suporte a I/O parcial sem repetir bytes já aceitos.

Falha, conflito, replay, substituição, mudança de serviço ou Stop nega novas
mutações. Stop não cancela IO já submetido. Nenhum retry automático, nova
tentativa por restart, troca de leaf para contornar conflito ou refund.

## U6 — Readback e retenção

Conferir o novo nome canônico por referência relativa ao parent original,
identidade esperada do objeto novo, tamanho/hash/descritor e serviço ainda
parado/configuração inalterada. Conferir a disposição do anterior retido.
Fechamento controlado conhecido é necessário para sucesso limitado; quiescência
OS universal continua UNKNOWN. Falha parcial/fechamento incerto retém packet,
originais, objetos e créditos pelo owner, sem delete/rollback ou liberação por
timeout. Não promove um EXE possivelmente parcial a imagem apta a Start.

## U7 — Aceitação, implementação e operações separadas

A aceitação desta especialização não autoriza mutações reais. Após aprovação
normativa explícita: implementar ato/protocolo e qualificar offline falhas antes
e depois de cada passo, substituição parcial, Stop, concorrência, replay,
hash/proteção divergentes, readback/close UNKNOWN e rejeição de rollback.
Congelar artefato final somente depois; preparar pedido byte-exato independente
para uma única atualização. Autorizações de endpoint e Start/Stop permanecem
separadas. Sem atualização nesta task, sem comandos com placeholders.

Preservar históricos, pins, autorizações consumidas, fixture, reservas,
OS_QUIESCENCE UNKNOWN, PST0.6.4, HEAD/WIP. E21 PENDING; ED11/Phase6 PARTIAL.
