# CA_INITIALIZE — integração nativa offline

Estado: **QUALIFIED_OFFLINE** para a entrada e composição em fixtures; **NORMAL operacional DENY**. Phase 6 e ED11 permanecem PARTIAL.

## Implementação

A entrada `papacc_server_launcher_configuration_ca_initialize` está no owner canônico. Encaminha somente o attempt e a reserva desse owner à entrada `papacc_configuration_personal_ca_initialize`; não permite trocar root, criar serviço ou escolher outro attempt. Não foi adicionada chamada automática de startup/CLI.

A entrada valida o plano exato de propósito conjunto NEW_INSTALL/CA_INITIALIZE já confirmado e consumido, operador/alvo/geração/serviço/configuração, writer, exclusividade, contexto original e quotas. Revalida as referências antes e depois de uma única chamada ao owner existente `papacc_ca_key_create`. Não assina, exporta chave privada, publica ou aciona RECOVERY.

O estado da tentativa e a referência candidata são registrados antes da chamada. Sucesso permite o borrow/handoff existente; falha registra a primeira causa, mantém a referência candidata, marca unresolved e impede segunda criação, retirement ou fechamento que descartaria o estado. Não há rollback/cleanup automático. A criação da primeira CA não exige uma CA anterior; a aquisição EXISTING_CURRENT permanece borrow-only e separada.

## Qualificação e limites

- Selected/Legacy Debug/Release: PASS, zero warnings.
- 13 CTests por configuração, 52 execuções: PASS.
- 2.718 verificações do modelo CNG por configuração: PASS, incluindo o corpo da nova entrada com **somente a API de criação substituída no teste**, criação única, rejeições antes do efeito, falha de criação, Stop após criação, primeira causa e retenção.
- O teste nativo de binding chama a nova entrada sem substituir o guard: DENY antes de qualquer criação. O wrapper canônico também é exercitado negativamente.
- Oito fixtures nativos v1/v2 do mesmo worker: PASS. CAS/PENDING/re-read/PUBLISHED já qualificados foram reutilizados; esses fixtures não contêm uma CA operacional.
- NEW_INSTALL → criação substituída → borrow → handoff → ativação NORMAL → emissão sintética: positivo no modelo. CA existente/restart: positivo no modelo sem nova criação. Concorrência, identidade divergente, custódia/falhas, Stop e retenção continuam cobertos pela campanha afetada.

CNG/assinaturas privadas e publicação operacional não foram executados. CryptoAPI pública e fixtures de metadados não demonstram criação/autoassinatura da CA real nem admissão produtiva. O guard `personal_native_ceremony_ready` continua retornando zero; os proofs NORMAL da factory real continuam incompletos. Não foram instalados callbacks positivos em produção.

O layout gerenciado aumentou 16 bytes pela referência candidata/estado: Selected 1.972.016/3.145.728, Legacy 1.803.448/2.097.152. Custos provider/kernel e picos não observados permanecem UNKNOWN; não há concessão de capacidade.

## Continuação funcional e primeiro gate

M7 mantém identidade exata e isolamento de credenciais; positivo nativo limitado ao cohort vazio e efeitos modelados. Dependentes reais, receipts/Stop e quiescência não foram inferidos. Enrollment L2 e E8/fonte positiva permanecem pendentes. D4/D5 e composição Browser/provider continuam independentes; TPM/Q4 não foram retomados.

A lacuna seguinte é a composição nativa de admissibilidade/certificado original e continuidade da primeira instância com a publicação, antes de solicitar qualquer operação real. A mera entrada de criação não fecha a jornada criptográfica nativa. É necessário qualificar a aquisição das provas originais, custódia e certificado/geração, reserva e identidade do alvo, continuidade A/P/re-read no mesmo owner. Não há comando operacional pronto nesta entrega.

Ver `NEXT-NATIVE-FIRST-INSTANCE-COMPOSITION.md` para continuação técnica sem efeitos. Somente depois dos gates completos cabe pedido específico para a primeira instalação real. Não é necessária nova aprovação de M0–M11 nem reabertura do CAS/PUBLISHED.

## Rastreabilidade

`TECHNICAL-AUTHORIZATION.md`, `QUALIFICATION.json`, `SOURCE-CHANGES.json`, `FUNCTIONAL-DELTA.patch`, logs e `ARTIFACT-HASHES.json` documentam escopo, preimages do WIP, binários e resultados. HEAD, registros históricos, ADRs accepted, pins operacionais e PST 0.6.4 foram preservados.
