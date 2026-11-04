# Sessão P1 corrigida — primeira abertura passou; guarda antes da decisão negou

Fonte: saída enviada pelo proprietário no chat após o único comando autorizado.
Transcrição dos marcadores essenciais abaixo, sem alegar cópia byte-exata de
anexo inexistente. Resumo factual: P1-CHAIN-SESSION-RESULT.json.
Autorização608b6...4441 **USED_NO_RETRY**, não pendente/reutilizável.

## Cópias documentais e digitação corretas

P1_ORIGINALS preparedTrue/firstNONE/consumedTrue/created_mask3/verified_mask3/
close_knownTrue. Pedido novo ID417614100:1441792:542844,
decisão nova ID417614100:1179648:542846. Pins608b6...4441/f54d0...d0e1
correspondem ao pacote e à resposta real aprovados. Arquivos persistentes
devem ser preservados; não recriar, adotar ou apagar para tentar novamente.

Linha DATA P1 aprovada foi digitada no prompt nativo correto, com imagem
417614100:1638400:813992. Nenhum erro de colagem ou perfil demonstrado.

```
SCQ_COLD_ORIGINAL=DENY; WIN32=1816; NO_SCM_START; NO_RETRY
SCQ_COLD_FIRST_CAUSE stage=2 reference=1 mapped=1816 native_error=0 query_attempted=1 query_succeeded=1 count_known=1 own_count=125 query_reference=1 guard=124 cap=128; SCOPE=ORIGINAL_PRE_OPEN_QUERY_NOT_NEW_OBSERVATION; UNKNOWN_NOT_ZERO
SCQ_COLD_NATIVE_OPEN attempted=1 reference=0 ntstatus=0x00000000 iosb_status_snapshot=0x00000000 iosb_information_snapshot=1 handle_returned=1; CAPTURE_NOT_NEW_QUERY; IOSB_INITIALIZED_NOT_PROVEN_WRITTEN_ON_FAILURE; METADATA_NOT_AUTHORITY
MANUAL_P1_DIAGNOSTIC_NATIVE_EXIT=2; NO_RETRY; NO_STOP; NO_PRODUCTIVE_PROMOTION; OS_QUIESCENCE=UNKNOWN
```

## Primeira causa específica desta execução

scqc_plan abre request(index0), decisão(index1), imagem(index2), nessa ordem.
Stage2/reference1/count125/querysuccess1 é a guarda own>=124 imediatamente
ANTES da segunda NtCreateFile. Código1816 foi produzido pelo gate explícito,
não propagado de uma API NT, amostra falha ou inferência sobre falta de memória.
125 é inferior ao máximo128, mas viola o guard pré-aquisição124; ambos permanecem.

O snapshot NT pertence à referência0: primeira NtCreateFile do pedido retornou
status0/IOSB0/InformationFILE_OPENED1/handle válido. A decisão NÃO foi aberta;
não confundir os dois registros ou ler NTstatus0 como resultado da segunda.
Logo existe agora prova nativa positiva da primeira abertura no contexto desta
sessão. Não prova toda a admissão OHI, conteúdo/owner do pedido pelo cold owner,
segunda abertura, âncora/cadeia da imagem ou equivalência com a causa histórica.

Pelo fluxo qualificado, tipo/ID do primeiro arquivo, aquisição de SD, hash do
descritor e LocalFree passaram antes de alcançar a consulta da referência1.
A resolução candidata da decisão também precede essa consulta e não negou.
Isso delimita o trecho, não atribui cada handle a essas APIs. Não foram
alcançados planos/confirmações/consumos E/Q/S, endpoint/SCQ/Start ou provas
runtime/bilateral/receipt/close. Não há readback SCM atual inferido de NO_SCM_START.

## O que ainda não foi medido

A contagem pré-request desta MESMA execução foi sobrescrita pelo campo de
última query; sabe-se apenas que foi<124, pois a primeira abertura passou.
Não subtrair o count113 de outra sessão para afirmar crescimento12 nesta.
Há pelo menos dois handles líquidos a mais frente a um máximo inicial123,
mas esse limite algébrico não identifica ownership, pico ou distribuição.
Um handle original do pedido é controlado/retido legitimamente pelo owner.
Não foi demonstrado vazamento, handle dispensável ou fechamento seguro dos demais.

Entre as duas contagens há NtCreateFile/info/SD/hash/LocalFree do pedido e
GetDriveType/QueryDosDevice da decisão. papacc_store_hash fecha seu hash/provider
no sucesso; eventual retenção incerta nega. Não atribuir handles internos a CNG,
filesystem ou namespace sem evidência. Inspecionar offline os lifetimes desse
trecho é o próximo foco, sem nova campanha genérica de recursos ou coleta.

Close conhecido da coorte documental não prova fechamento do controlador.
Não há marcador positivo explícito desse último close na saída; retorno ao
prompt/exit2 não resolve retenção histórica ou quiescência OS. Preservar UNKNOWN.

## Próximo gate

Não repetir driver, recriar decisão/originais, mudar quotas/guards, liberar
reservas ou executar UPDATE. A sessão autorizada foi utilizada, embora E/Q/S
nativos não tenham sido alcançados. Registro pendente anterior é histórico
agora sucedido por esta reconciliação.
NEXT-P1-FIRST-REQUEST-TO-DECISION-OWNERSHIP-OFFLINE.md delimita a revisão de
ownership desse intervalo, com evidências existentes. Só defeito comprovado
justifica correção; nenhuma referência interna pode ser fechada por conveniência.
Sem defeito demonstrado, delimitar dado faltante, não preparar retry.

Sem mudança funcional nesta reconciliação: builds/campanhas não repetidos.
Preservar124/128/G3/DENY/UNKNOWN/históricos/pins/EXEs/originais/atos consumidos,
HEAD/WIP/reservas/fixture/PST0.6.4. Nenhuma coleta/Start/Stop/endpoint/SCQ/UPDATE/
SCM/ACL/CA/CURRENT/rede/TPM/NV/Q4/limpeza/refund/recuperação executada pelo agente.
