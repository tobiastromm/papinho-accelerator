# Escolha focal — piso de commit do primeiro Start P1

Status: PROPOSED / NOT_ACCEPTED / NO_OPERATION_AUTHORIZED_BY_THIS_DOCUMENT.
2026-11-02. Um único campo, somente PERSONAL_LOCAL P1 TRANSPORT_ONLY,
primeiro diagnóstico fechado. Não é nova arquitetura, canal ou ADR genérica.

## Evidência e razão da escolha

G3-P1-ADMISSION-SESSION-RESULT.json e sua saída preservada demonstram
ORIGINAL_ERROR0 e RESOURCE_ERROR8: commit9081241600 < piso9823326208 bytes.
Handles140<188/192, inicial113<=128 e físico8314572800>=7239172096 não rejeitaram.
Não houve endpoint/Start. Ready/growth/bilateral/receipt/close runtime não foram
alcançados. Autorizações anteriores utilizadas; causa histórica de outras
sessões não provada. Retenção histórica e quiescência OS UNKNOWN.

O piso atual deriva da menor observação histórica de commit no registro SCM,
conforme INITIAL-QUERY-AND-G3.CANDIDATE.md/tabela G3, SHA256
5b77ddae089ba496c511a562575e22642a29555e536fd4563e5e5ebb8faefeca.
É tolerância administrativa aceita, não prova de que o Papinho consome9.8GB
ou de que abaixo disso Windows falhará. G1 continua admitindo risco opaco,
sem afastar a obrigação de margem fresca concretamente selecionada.

## A — manter o contrato atual (menor intervenção)

Manter9823326208 bytes disponíveis como piso comum dos participantes.
A condição do host deve satisfazê-lo numa leitura fresca da nova sessão.
Diferença histórica observada742084608bytes não é medição atual nem garantia
de suficiência futura. Não fechar processos/handles ou alterar paginação/
configuração automaticamente. Não exige código, build ou UPDATE. Exige nova
sessão operacional independente quando houver fundamento de condição alterada.
Não repetir a sessão consumida. Esta é a opção de menor mudança técnica.

## B — selecionar tolerância administrativa de 8 GiB para P1

Novo piso EXATO8589934592 bytes (8 GiB) disponíveis, tanto controlador quanto
runtime, exclusivamente P1 PERSONAL_LOCAL TRANSPORT_ONLY. Não é8GB decimal.
Apenas esse campo é especializado. Escolha redonda explícita de tolerância a
risco, NÃO derivada de um máximo nativo, censo, prova de reserva ou necessidade
do aplicativo. Não ajustar automaticamente ao valor observado para obter PASS.
Na amostra preservada restariam491307008bytes acima desse novo piso; isso só
compara números históricos, não prova headroom presente ou sucesso do Start.

Redução do piso atual1233391616bytes. Aceitar B significa admitir operação
com menor commit disponível no host, ainda sob risco opaco G0–G9. Não pressupor
pico, latência, término de APIs ou cancelamento por Stop. O orçamento privado/WS
não é previsão de loader/RPC/kernel/retidos nem garante a suficiência de8GiB.
Recursos podem cair após uma leitura; revalidar por efeito/freshness obrigatório.
Não aceitar B com base apenas em a amostra passar seu comparador.

## Especialização normativa necessária em B

Precedência apenas sobre commit9823326208 da tabela G3 incorporada pela
DECISION-personal-scq-initial-query-g3-20261031.md e a preservação desse valor
na DECISION-personal-scq-p1-handle-envelope-20261102.md, no escopo P1 citado.
Os documentos accepted e candidatos históricos permanecem imutáveis; registrar
decisão focal aprovada antes de implementar. Não reabrir G0–G9/G1, P0–P7,
QUERY/SQB2, OHI/SSO-B ou o binding condicional9637246a, salvo essa precedência
numérica delimitada. A aceitação operacional genérica não seleciona B.

READ/NEW_INSTALL/TO sem P1/UPDATE/SCM e outros perfis não recebem redução.
Se uma arquitetura de build não puder isolar isso, DENY e reportar a lacuna;
sem fallback por argumento CLI ou sugestão do peer. Revalidar bindings G3
exatos/policy hash nas duas pontas, mesmo owner/seletores já existentes.
Não alterar formatos públicos/persistidos ou criar novo mecanismo administrativo.

## Garantias intactas e implementação dependente

Manter físico7239172096, entrada128/guard188/cap192, private e WS1994944bytes
por participante, freshness100ms até efeito, janela240000ms/global60000ms/
menor prazo sem renovação, memória managed3MiB e todos os demais limites.
Identidade/proveniência/proteção/ancestrais/reparse/continuidade, originais,
três confirmações/consumos independentes antes dos efeitos, Stop, replay,
QUERY candidata distinta de bilateral/empréstimo, receipt/close antes de
RUNNING apenas diagnóstico, retenção UNKNOWN e produtivo DENY inalterados.

Somente após aceitação explícita de B: especializar produtor e verificador G3
P1 existentes/controlador/runtime e seus bindings, testar fronteiras
8589934591/8589934592/8589934593 bytes, isolamento dos perfis e rejeição de
policy divergente/fallback, Stop/UNKNOWN/freshness/consumo. Quatro builds e
regressões afetadas. Depois congelar EXE e preparar UPDATE U1–U7/U2 governado
se necessário, e uma sessão E/Q/S nova. Não executar efeitos automaticamente.
Sem loops de retries, redução adicional automática, limpeza/rollback/refund.

## Decisão concreta pendente

Escolher A, ou aprovar exclusivamente B com piso8589934592bytes neste escopo.
Outro número exige sua seleção explícita e conferência dos mesmos bindings;
não é selecionado por silêncio/continuidade. Publicação não é ACCEPTED.
Aprovar B permite registro normativo e código/qualificação offline dependente;
não autoriza por si só UPDATE/endpoint/SCQ/Start/Stop reais. Esses atos continuam
específicos e vinculados a pedidos/hashes/IDs finais e confirmações nativas.

Preservar HEAD/WIP, todos os objetos/EXEs/pins/históricos/retidos/atos consumidos,
reservas, PST0.6.4 e UNKNOWN. Não executar coleta, mudança de processos/ACL/SCM,
paginação/CA/CURRENT/rede/TPM/NV/Q4/limpeza/refund/recuperação nesta análise.
