# Decisão de garantia — comparação e recomendação

Task PACC-PHASE6-STAGING-ASSURANCE-DECISION. Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469. Somente preparação documental; sem nova aceitação, código, quota, coleta ou efeito operacional. Este relatório encerra a investigação genérica de custos opacos como caminho crítico: a escolha pendente é explícita, não outra contagem.

## A/B concretas

| Aspecto | A — provisão prévia fundamentada | B — ensaio pessoal/local limitado proposto |
|---|---|---|
| Garantia | Fonte suficiente de provisão/margem para iniciar, preservando regra atual de UNKNOWN material | Aceitação explícita de não ter teto demonstrado para componentes opacos enumerados, com controles e risco delimitados |
| Viabilidade com fontes atuais | Não concluível: APIs/medições atuais não estabelecem provisão opaca/margem suficiente | Normativamente possível somente por especialização posterior expressa; não autorizada hoje |
| Pré-condições | Originais/proteção/autoridade/recursos e fonte suficiente | Mesmas garantias de segurança; plano de risco próprio completo com critérios quantitativos explícitos e fundamento |
| Recursos | Não preencher limites desconhecidos com observações | Não afirmar bound físico/kernel; tolerâncias de monitoração precisam ser justificadas/selecionadas, sem número novo nesta proposta |
| IO futuro | Retornos de create/write/flush/readback durante execução, close depois; não sucesso futuro provado antes | Idêntico protocolo de resultado/falha/retidos; risco de insuficiência durante a tentativa é explicitamente aceito apenas no escopo B |
| Efeitos | Se autorizado, pacote e destino exatos | Um ensaio; três arquivos; até dois diretórios/três arquivos criados; sem overwrite/adoção; falhas parciais podem persistir |
| Stop | Governado, sem garantia de cancelar IO síncrono | Mesma garantia; não impede necessariamente pico entre amostras ou pressão de host |
| Produção | Sem autorização automática | Proibição expressa de promoção automática; nenhuma instalação/SCM/CA/CURRENT |
| Situação após escolha normativa | A continua DENY até fonte suficiente; não iniciar coleta genérica | B continua DENY até política/artefatos/valores/gates/qualificação e autorização operacional próprios |

Recomendação: B como alternativa de QUALIFICAÇÃO para sair da exigência de teto opaco prévio não demonstrável pelas fontes atuais, desde que o proprietário aceite explicitamente a redução de garantia e depois exista plano verificável completo. Não recomendar B como capacidade produtiva comprovada. Se essa redução não for aceitável, escolher A e manter bloqueio por ausência de fonte; não fabricar evidência ou prosseguir investigando o mesmo132 indefinidamente.

## Cláusulas que realmente precisam de especialização

1. D3/D4 e limite D1 incorporados em DECISION-personal-staging-envelope-domain-d0-d7-20261031: UNKNOWN material e justificativa de margem/custos antes de admissão. B não pode ser implementada fingindo que essa regra já foi satisfeita.
2. DECISION-ed11-preserve-incremental-resource-envelopes-20261012: “Unknown required evidence fails closed” e capacidade factual além da quota. É regra de projeto independente de LAB. B propõe exceção expressa só para provisão opaca desse ensaio, preservando censo/fatos e demais gates.
3. W4 do perfil Windows3MiB e F8 de F0–F9 preservam pendências nativas/transitivas/retidas; B especializa somente a garantia de provisão opaca no ato independente. Nenhuma exceção para criar CA/NEW_INSTALL real, nem para quotas geridas.

F3/F5/F6, W5, D5 e demais regras de identidade, originais, consumo único, proteção/retenção/Stop ficam intactas. Guard124/128 de LAB/TPM/SCQ não é dispensado nesses domínios. Não se invoca M3/LAB como fonte de autoridade ou autorização residual. Não há conflito literal novo registrado; existe mudança proposta de nível de garantia que só seria eficaz após aceitação designada e conferência de compatibilidade.

## Proposta única para decisão

LIMITED-STAGING-ASSURANCE.CANDIDATE.md, B0–B8 integrais, PROPOSED.
SHA256 literal: b4b705cc20899a58295c67e95105a6ad3792df4505fe51c0cd68a10d9e8acfbb.

Aprovar B significa aceitar exatamente essa garantia reduzida, não executar staging. Nenhum registro ACCEPTED foi criado; candidato original e ADRs accepted preservados. Não se propõe aceitar UNKNOWN de identidade, proteção, fonte administrativa, volume ou fechamento como PASS.

## Riscos residuais que o proprietário precisa avaliar

- Recursos do OS/provider/cache/metadata podem ultrapassar uma estimativa ou falhar dentro de uma única chamada; monitoramento não é contenção física.
- Pressão de memória/commit/disco pode afetar o equipamento e outros processos; disponibilidade pode mudar após observação.
- Stop pode suprimir a próxima submissão sem cancelar a atual; não existe hard deadline demonstrado.
- Falha parcial pode deixar diretórios/arquivos persistentes e RETAINED_UNKNOWN que bloqueiem outra tentativa; não há autorização automática de limpeza/recuperação.
- Resultado positivo de um ensaio não vira teto de recursos, reboot-proof, ED11 PASS ou licença de publicação produtiva.

Esses riscos não são resolvidos por rebatizar observação como capacidade; B pede aceitação explícita e escopo restrito, mantendo fatos UNKNOWN como tais.

## Evidências e critérios ainda indispensáveis

Reutilizáveis: censos geridos, limites de buffers/multiplicidades, até16slots mais contexto, composição/cópia/hash/flush/Stop/retidos em modelos e última qualificação quatro builds/68CTests. A última imagemeee02c93... é candidata não selecionada; pins/bytes futuros precisam conferência no pedido. Não repetir campanhas inalteradas nesta decisão documental.

Antes da execução B: fixar manifesto/fontes/destino e decisão administrativa originais; justificar/selecionar critérios quantitativos de observação/interrupção, margem/volume, lifetime e duração do próprio ensaio; demonstrar custos do observador e caminho capaz de adquirir os fatos antes de efeitos. Não basta escrever “sem hard bound” ou aceitar risco em abstrato. Nenhum número foi inventado aqui; B3 impede admissão com esses campos ausentes.

Não é possível apresentar hoje pedido byte-exato de staging nem comando manual: falta escolha normativa e plano específico. Isso não recomenda outra coleta de contagem; somente depois da escolha se qualifica a política e o caminho. A definição de garantias/falhas por fase está concluída e não deve ser reaberta como nova investigação.

## Caminho mínimo até primeiro staging autorizável

Escolha A ou aceitação explícita B0–B8/hash → registro normativo correspondente, sem editar histórico → no caso B, composição e qualificação offline da política específica e seleção integral do plano/artefatos/valores → preflight fresco mínimo de originais/volume conforme política e autoridade próprias, se necessário → pedido operacional único de staging delimitado → aprovação/execução manual/reconciliação → aquisição/readback dos artefatos staged e pedido independente SCM_REGISTER_CLOSED parado. Start/Stop, SCQ/CNG, CA/CURRENT e NEW_INSTALL não são incluídos por consequência.

Sem código alterado, testes repetidos ou execução protegida. HEAD/WIP, UNKNOWN, reservas, chave fixture, pins e autorizações consumidas preservados; PST0.6.4 inalterado. Staging DENY/NOT_READY; E21 PENDING; ED11/Phase6 PARTIAL.
