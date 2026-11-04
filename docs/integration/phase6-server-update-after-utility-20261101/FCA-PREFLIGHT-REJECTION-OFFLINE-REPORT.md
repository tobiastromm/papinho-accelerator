# FCA independente rejeitada antes da apresentação — reconciliação e fechamento offline

Pedido2d7d03574ffcacce1bffd64e3510e7082d4e7c5841c1d74cdf157334957ec8a3 usado uma vez manualmente, sem retry. Não houve plano/confirmacão FCA exibido. Consumo nativo de efeito não observado; não confundir com disponibilidade para repetir a tentativa manual. Estado atualizado sem inventar consumed=True.

Saída: prepared0, flags de criação0, ID0:0:0, chave sem nome, first0, close_known1, wrapperCloseTrue, exit2. Flags zero não provam ausência independente de objeto. Pela ordem do código sem confirmação não se alcança consumo/submissão de worker/CA; não houve consulta adicional para verificar existência. CUSTODY_METADATA_ONLY é diagnóstico de autoridade DENY, não a primeira causa.

## Causa demonstrada e lacuna
Defeito demonstrado de diagnóstico: prefixo de personal_present tem goto done anteriores à apresentação sem registrar first_error. first0 não é sucesso. LAST_CAPTURE contém somente últimos snapshots, sem provar qual predicado rejeitou. A captura da source e SD é compatível com progresso na aquisição; não demonstra sucesso de LookupAccountName, validação do SID, CSPRNG, SD final, releitura/parse ou construção do plano. Não atribuir causa ao commit, ACL, key policy ou recursos sem evidência.

## Correção técnica mínima
Acrescentado somente registro local de etapa1–12 no launcher existente e captura do retorno PAPACC_RESULT original das chamadas de aquisição4 e plano11. Linha FCA_PREFLIGHT_REJECT emitida antes de qualquer close quando ainda não foi alcançada a apresentação. Sem nova consulta nativa ou detalhes sensíveis.
operation_result representa o último retorno capturado somente das etapas4/11; em outras etapas não identifica errno/NTSTATUS nem prova sucesso do predicado corrente. Essa limitação permanece explícita. O instrumento localiza a primeira região de rejeição, não revela por si só um erro interno de uma chamada.
Nenhum gate, quota, ordem, owner, perfil, autoridade ou contrato alterado. Não preenche first_error com erro inventado. Nenhum handle fechado por conveniência. D1/PDP não recriados.

## Qualificação
Quatro builds Selected/Legacy Debug/Release PASS sem warnings MSVC;120 pares de regressões afetadas PASS, toolchain e diretórios originais. Logs fca-preflight-stage-*. Manifesto FCA-PREFLIGHT-STAGE-OFFLINE-QUALIFICATION.json.
Candidato Selected/Release 911360 bytes/SHA256 747ba426d55af50947169a178c8366def46cb57ad4a8c29308ff44664aeaee96.
Instalado permanece911360bytes/SHA256c14dc2a0e2f3e51d19588c9930e354d512c9f3d3c41f0eaff7957dae0734f474; não atualizado ou consultado operacionalmente.
A instrumentação nova não demonstra retroativamente a causa histórica.

## Primeiro gate restante
Preparar UPDATE específico U1–U7/U2 do candidato diagnóstico apenas se necessário para obter a etapa original; não repetir a FCA consumida. Antes de novo ato, verificar compatibilidade dos objetos históricos retidos e utilizar confirmação/decisão/tentativa independentes.
Não há pedido operacional de UPDATE entregue por este relatório. Não executar o candidato workspace como instalação ou owner administrativo. A ausência de captura no EXE instalado é a lacuna factual.
Próximo prompt NEXT-FCA-PREFLIGHT-STAGE-UPDATE-PREPARATION.md.
Mantidos UNKNOWN/retenção/quiescência OS, DENY produtivo, PST0.6.4 e WIP/HEAD/históricos.