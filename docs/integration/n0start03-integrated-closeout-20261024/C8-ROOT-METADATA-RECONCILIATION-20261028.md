# C8 locator — reconciliação da saída manual

Resultado: PASS da consulta delimitada de metadados, não da publicação C8.
Recebido do proprietário; observação declarada 2026-10-29T01:48:21.4786085Z
(2026-10-28 22:48:21 no fuso UTC−03:00). Sem repetição da consulta.

## Fatos e limites

- first=NONE, complete=true; scope=C8_LOCATOR_METADATA_ONLY.
- Oito entradas: dois diretórios Protected e Protected/N0; seis arquivos N0, pares approved/evidence das tentativas 0b1de869e11493b7cb08fb329578f6ce, 10584f02e11f92c92b43cddf0d9d316f e 75330d11cc7e4123ae62eadc9a59c2f5.
- Nenhum candidato por nome foi marcado; nenhum manifesto C8/CURRENT/epoch foi identificado pela coleta. Não houve leitura de conteúdo para confirmar a finalidade dos seis arquivos.
- Não houve reparse point informado nem depth_limited no resultado. complete=true significa conclusão do escopo do coletor, não inventário de todo o equipamento ou ausência universal de root.
- ACL owner permaneceu null: o coletor só consulta owner de nomes candidatos, não houve candidato. Null não significa objeto sem proprietário.
- SID reportado S-1-5-21-785699550-1440458426-1682014700-1108 coincide textualmente com a designação; PID reportado 10108. Sem criação de processo, token original mantido ou atestação independente, isso não estabelece binding de custódia.
- original_object_identity=NOT_ACQUIRED; RAW1=NOT_VERIFIED; publication=UNKNOWN. Metadados de caminho não substituem identidade original por handles.
- ownerAuth=NOT_ACCESSED; authority=DENY; no_retry=true.

O comando fornecido inclui pin do script 9f0f88d38e2dac693a7bd8fa62f3a7ed21f113b6c220b429a7c8abfc01aa2cec; os bytes locais foram reconferidos com esse mesmo SHA256 na reconciliação. A checagem precede enumeração no código, mas não representa atestação de imagem mapeada nem elimina TOCTOU contra administrador. Saída é evidência fornecida pelo proprietário, não execução independente pelo agente.

## Classificação

Namespace N0 observado: diretórios/arquivos existentes sem autoridade C8 demonstrada. Publicações de teste anteriores: qualificação/fixtures. Root administrativa operacional: NOT_DEMONSTRATED, não comprovadamente inexistente. Continuidade após reboot, manifesto de publicação, domínio, leases/current-policy/capability e owner vivo: UNKNOWN.

Nenhum binding ownerAuth pode ser finalizado com esses dados. Não repetir STORAGE_20, TPM ou Q4 para preencher a lacuna de publicação.

## Proposta condicional de primeira root pessoal/local

Preparação apenas; sem aceitar nova norma, criar objetos, escolher caminho fictício ou emitir comando executável.

1. Estabelecer se a intenção é uma primeira instalação pessoal/local, distinta de adoção de N0, restore ou promoção de teste. Enquanto não houver evidência de publicação anterior, não afirmar autoridade existente.
2. Reconciliar o contrato vigente da cerimônia administrativa com um ato específico NEW_INSTALL: fonte legítima fora do candidato, vínculo operador/parent/configuração/invocação/destino e consumo único na criação exclusiva. A lacuna histórica está documentada em initial-bootstrap-custody-authorization-20261022/authorization-consumption-stop.md; ela precisa ser conferida contra eventual evolução antes de implementação dependente, não presumida automaticamente atual ou resolvida por uma flag.
3. Delimitar destino novo, configuração e perfis reais; autoridade, reservas, freshness, lifetime e Stop; criação exclusiva e identidade nativa original; tratamento de falhas parciais/UNKNOWN sem limpeza ou retry automático. Não herdar CA/issuer, confiança TPM ou créditos de LAB.
4. Preparar manifesto de publicação associado ao commit real, domínio/revisão, CURRENT/epoch emitidos pelo mecanismo existente e evidências de custódia/continuidade. Nenhum desses valores é inventado antes da operação.
5. Submeter a decisão/autoridade de bootstrap específica e os efeitos exatos ao proprietário. Publicação CURRENT permanece proibida até autorização própria; esta proposta não a concede.
6. Somente após publicação factual e owner/root autenticados completar o pedido separado de acesso em memória STORAGE_20. Designações de custodiante/fonte já accepted não precisam ser reaprovadas e não autorizam o bootstrap.

Primeira decisão real: confirmar a direção de primeira instalação governada pessoal/local, em vez de continuar supondo uma root operacional existente. Não se solicita aprovação operacional genérica ou publicação sem pacote exato e pré-condições demonstradas.

## Continuidade ED11

E21 permanece PENDING, ownerAuth DENY. E17 identidade/acesso/readiness e E18 lifecycle/Stop continuam independentes; E19/D4/D5 e E20 composição real continuam sem justificativas e bindings completos. Este resultado remove a dúvida de acesso negado na listagem delimitada, não fecha qualquer desses gates. E22 derivado pendente; ED11/Phase 6 PARTIAL.

Sem alteração de código produto, build, pin operacional ou ADR accepted; nenhuma campanha repetida. Sem leitura protegida, TPM/NV, segredo, Start/Stop, SCM/ACL, limpeza, refund, CURRENT, assinatura ou recuperação.
