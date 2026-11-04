# READ/P — estado corrente da integração offline

2026-11-03. K0–K9 ACCEPTED, proposta SHA256 8489c009dfa62df0835415d4544c0632abd850924a35217b2ec451718147ad38 reconferida. Registro normativo anterior preservado. Este relatório vivo atualiza o snapshot READ-P-RUNTIME-OFFLINE-QUALIFICATION.json; o snapshot não foi alterado.

## Integração concluída neste recorte

P original consumido → projeção SKP1 no pipe original → receptor READ local admitido → probe único → SKR1 correlacionado → receipt original → close SQC3 → retorno/ confirmação de close. As funções reais foram exercitadas com APIs protegidas simuladas. A projeção e o resultado são DATA; não são receipt, fechamento, controle administrativo positivo ou autoridade.

A sessão retém ponteiros locais para o ato P e o receptor produzidos no próprio owner. O retorno READ exige o binding original, consumo, propósito, hashes, chave, resultado concluído e deadline global idêntico ao escopo de recursos. Preparação duplicada do receptor é rejeitada. Nenhum ponteiro é recebido do outro processo. Não há novos handles, pipe, evento, OVERLAPPED ou canal.

TRANSPORT_ONLY mantém a guarda anterior. READ anterior, sem a nova variante QUERY/P, mantém seu comportamento; NEW_INSTALL não recebe ampliação. A primeira campanha encontrou uma guarda ampla demais sobre READ anterior; o escopo foi corrigido no código, sem alterar o teste para aceitar a regressão. A campanha final passou.

## Qualificação final

Quatro builds Selected/Legacy Debug/Release: zero warnings. 29 testes/configuração, 116 pares PASS. Grupo READ: 21 cenários, 875 verificações. Regressão de preparação/readiness anterior: 57 cenários PASS. Ausência de binding, P não consumido, replay, substituição, Stop, recursos insuficientes, deadline zero/divergente, erro nativo, resultado inconclusivo e fechamento UNKNOWN permanecem negativos.

Mais 16 bytes por sessão para referências locais, zero handles novos. Selected ROOT=375800/KNOWN=2002848 (<3145728); Legacy ROOT=207232/KNOWN=1834280 (<2097152). Custos opacos/stacks/provider não foram convertidos em máximos nativos provados.

Derivado Selected/Release: 804864 bytes, SHA256 a0ecb94f28aa02aacf890844f37a4423007eb6a3abf2cd25f6db63954ffe6e2c. Não instalado, não selecionado para operação e não qualificado end-to-end. Compilação/otimização não demonstram capacidade operacional. Pins de fontes/logs e resultados: READ-P-RECEIPT-CLOSE-OFFLINE-QUALIFICATION.json.

## Limite exato da evidência

O teste ainda modela a fronteira de entrega QUERY de cinco referências e os dois identificadores de entrega QUERY já fechados. O close dos arquivos, a validação e o retorno SQC3 usam os corpos reais. Isso não demonstra um producer frio READ nativo, nem autenticação bilateral factual, nem execução SCM/RUNNING/exit reais. O getter operacional permanece NULL.

Primeira lacuna: producer administrativo original da preparação fixture TEST_ONLY, com confirmação/consumo, recursos/assurance próprios e SHA256 público ligado ao controle positivo da mesma chave. K7 READ não pode ser importado como envelope da criação. Depois: caller READ frio/entrega QUERY e ingresso/lifecycle SCM finito. São trabalhos técnicos já cobertos por K0–K9, sem necessidade de nova aprovação normativa para o mesmo escopo.

Não existe pedido operacional completo ou comando para executar. Nenhum KSP real, fixture, coleta, UPDATE, endpoint, Start/Stop, SCM/ACL, limpeza ou refund. Instalação não consultada nem modificada; histórico/autorizações consumidas/pins/EXEs/PST0.6.4 preservados. Retenção histórica e quiescência OS UNKNOWN; readiness produtiva DENY.

Continuação técnica: NEXT-READ-P-RUNTIME-NATIVE-PRODUCERS.md.
