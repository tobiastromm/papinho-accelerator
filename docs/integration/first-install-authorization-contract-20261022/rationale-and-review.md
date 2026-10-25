# Fundamentação e revisão documental F0–F9

Status: PROPOSED; 2026-10-22. Somente elaboração normativa, sem implementação.

## Alternativas de origem avaliadas

| Possível origem | Uso na proposta | Insuficiente isoladamente |
|---|---|---|
| Administrador local | Executa a cerimônia se legitimamente designado e tecnicamente elegível | Membership, elevação ou ACL não provam competência para qualquer implantação |
| Operador de provisionamento | Pode ser delegado pelo responsável legítimo, com escopo verificável | Conta técnica ou posse de credencial não provam delegação |
| Custódia física | Evidência complementar sob política específica do equipamento | Presença local, posse ou jumper não autorizam por si |
| Autorização institucional | Fonte legítima quando baseada em processo/política já existente e autenticável | Ticket, nome ou declaração copiados para CLI não são prova |
| Política específica da plataforma | Mapeia competência e prova externa para verificações reais | Não pode ser derivada do candidato nem criada automaticamente para aprová-lo |

A recomendação é uma única origem conceitual: responsável legítimo pela implantação, comprovado pela fonte administrativa preexistente da plataforma/deployment. Não selecionar produto, fornecedor, servidor, formato de aprovação, instituição ou pessoa por inferência. O contrato é concreto sobre os fatos, vínculo e consumo exigidos; a disponibilidade e a qualificação de uma fonte concreta continuam pendentes. Não existe nesta tarefa prova positiva de legitimidade de qualquer operador.

## Cerimônia proposta e consumo

Identificar operador real -> resolver competência independente -> reter identidade nativa do parent governado e contexto de alvo -> registrar NEW_INSTALL/configuração -> fonte autentica a decisão associada à invocação/nonce/contexto -> revalidar -> transição única AUTHORIZED para CONSUMED -> submissão ao mesmo worker -> revalidar imediatamente antes do efeito -> UMA criação exclusiva -> observar identidade do filho e manter custódia -> preparar segundo os contratos existentes.

Parent/escopo podem ser identificados antes da criação; a identidade factual do filho só pode ser anexada depois. Isso evita pedir file ID de um alvo que ainda não existe e impede tratar path como identidade. Uma tentativa consumida não volta a autorizada nem por erro anterior à criação. UNKNOWN preserva retenção e impede retry. A disposição da operação permanece distinta da liberação de recursos e da publicação CURRENT.

A revisão exata deve avaliar expressamente a nova exigência de prova de competência independente, o vínculo invocação/nonce32, o limite de um ato vivo por owner, a validade por eventos e o ponto de consumo anterior à submissão. São escolhas PROPOSED, não interpretações silenciosamente inseridas em P1. Não alteram seus bytes ou sua vigência atual.

## Compatibilidade e fronteiras

- P1: identidade/contexto técnico continuam distintos de competência administrativa e ação atribuída.
- P2/P3: declaração não prova ausência global de história; criação exclusiva/custódia e mesma preparação permanecem obrigatórias.
- P5/P6 e D2/D4: sem CURRENT anterior no modo inicial; DATA/modelo não vira autoridade; commit/publicação preservam seus gates.
- P7/P8 e Model E/ED08: mesmo owner/worker, sem sidecar/pin paralelo, Stop não bloqueante e recursos incertos retidos até quiescência factual.
- Issuer cláusula6: CA inicial é ação distinta; não gerar, abrir ou emprestar chave por autorização NEW_INSTALL.
- ED11: não há novo censo medido, reserva implícita ou aumento de quota. Snapshot/pins/evidências/token/CA/stack/CRT/retidos e custos opacos exigem fechamento no perfil aplicável.

Não há justificativa para criar uma CA de aprovações ou autoridade da própria instalação: isso seria circular. A fonte preexistente é um requisito de prova externa, não um novo serviço implementado. Se a plataforma não a oferece, o caminho continua negado. No perfil sem validade temporal absoluta, não se exige UTC da própria instalação; se a fonte usar certificados/prazos que precisem de UTC ou revogação qualificada indisponíveis, o adapter deve negar, não ignorar a dependência.

## Evidência e limites

HEAD main 913c90cf2328f7939d8f5b927640601903ac6e91 confirmado. Fontes accepted de bootstrap D2/D4 e cerimônia P0–P9, issuer/custódia, ED11 e STOP anterior foram confrontadas com a proposta. Autoridades e preimagens estão pinadas em preimages.json; conferência final adjacente. Nenhum documento accepted foi alterado, nenhuma fonte funcional modificada. Fontes de lifetime/ED08 são preservadas por referência e pelas obrigações explícitas F5/F6/F8; não se cria lifecycle concorrente de recursos.

Builds, testes, bootstrap, rede, relógio, chaves, certificados e publicação não executados. Não se transforma revisão documental em teste de autorização ou capacidade. Nenhum target, backend ou origem factual é declarado qualificado. A próxima tarefa é revisão exata para eventual aceitação normativa, não execução.
