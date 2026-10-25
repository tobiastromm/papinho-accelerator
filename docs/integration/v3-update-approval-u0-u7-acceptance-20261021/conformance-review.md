# U0-U7 — revisão exata e STOP de aceitação

TASK=PACC-PHASE6-V3-UPDATE-APPROVAL-U0-U7-ACCEPTANCE-1. Decisão condicional exige conformidade integral antes de ACCEPTED. GOVERNANCE CONFLICT na proposta U6: ambiguidade material de validade pós-restart frente a D3.7/G5/issuer-C2 P6. Não há conflito identificado entre as autoridades accepted. A proposta original e todos os ADRs accepted ficam byte-exatos.

## R1 — cláusula exata

U6 diz:

> A prova de origem não precisa ser serializada para conceder validade operacional após restart; recuperação autoritativa permanece DENY.

A primeira oração admite leitura de validade operacional pós-restart sem prova de origem, enquanto a segunda mantém a recuperação negada. D3.7 exige DENY após restart sem CURRENT/checkpoint/freshness/fencing factuais; G5 diz que CP3 íntegro é apenas história coerente e mantém AUTHORITATIVE_RECOVERY=DENY. O caráter transitório da prova não pode virar exceção de autoridade após restart. Não é seguro eliminar essa ambiguidade apenas no texto de aceitação de bytes que a preservam.

Correção mínima: substituir esse único trecho por:

> A prova de origem é transitória e não é serializada; sua ausência após restart não concede validade operacional, não restaura o ato UPDATE nem dispensa as provas factuais de recuperação e CURRENT. Recuperação autoritativa permanece DENY.

[Hunk PROPOSED](exact-u6-correction-PROPOSED.patch), [preimagem/intervalos/hashes](exact-correction-and-preimages.json) e [Revision2 candidata](U0-U7-REVISION2-PROPOSED.md). Patch NÃO aplicado à proposta original. U0-U5/U7 permanecem exatos; não há alteração funcional, novo formato ou exigência de persistir SID/token/prova transitória. A mudança da metadata revision no arquivo separado não altera as demais cláusulas.

## Matriz integral

| Condição | Cláusulas / autoridade | Resultado |
|---|---|---|
| Origem e identidade do ator | U0/U1/U3; F1, D2.2 e root/C8 | Conforme no escopo PROPOSED: solicitante local por token primário, sem impersonation/delegação por serviço, SID e contexto nativo verificáveis; outros targets não herdam Windows |
| Autenticação versus autorização | U1 identifica o papel autorizado para executar U2; U2 exige ato explícito da operação e tupla | Conforme: token isolado não é aprovação da candidata; bootstrap/C8 não são usados como equivalência |
| Root/operação/base/candidata/proveniência | U2/U3; D2.2/D4, G3/G4 | Conforme: invocação, pin/ticket/serial, revisão, imagem completa e tuplas exatas; digest apenas correlação; mudança invalida e impede reuso |
| CP3/SNT/PDG/issuer/policy | U2/U6 mais G0/G2/G3/G4/G5 herdados | Conforme: base v2 com CP2 ou v3 com CP3 completo e CURRENT factual; candidata CP ZERO até publicação factual; não exige CP3 fabricado em base v2; formatos/pins não alterados |
| Propósito/aprovação/delegação/revogação | U2/U4; F1/F2/G3 | Conforme: PACKAGE_SIGNING e tupla/intervalo/disposição explícitos; PUBLISHED não é delegação; revogação preparada não gera evidência positiva |
| Status e validade | U4; issuer/profile/F2/G3 | Conforme: fonte issuer/histórico protegidos, certificado e delegação separados; desconhecido/ausente/erro/expiração negam |
| UTC | U4; F2/G3 | Conforme como obrigação, NÃO qualificação: backend.utc existente não basta; sem fonte aprovada e qualificada DENY; nenhum clock/TTL novo é escolhido |
| CURRENT/Stop/invalidação | U3/U5/U6; D2.2/G4/E0 | Conforme no lifetime atual: revalidação antes do efeito; mudança de CURRENT/base/imagem/identidade nega; nenhuma promoção antecipada |
| Ownership/borrows/pins/retidos/CAS | U3/U5/U6; G7/E0/Model E/ED08 | Conforme: owner único, sem pin candidato fictício, fontes externas protegidas, quiescência e fechamento factuais; UNKNOWN não é rollback e timeout não libera |
| Bootstrap v1/update v2-v3/história | U0/U6 com G0/G5/D3 | Conforme quanto à separação de modos e bytes; U0 não reduz as bases accepted v2/v3 nem autoriza bootstrap v3 |
| Recuperação e validade após restart | U6 versus D3.7/G5/P6 | STOP R1: remover ambiguidade pela correção exata antes de aceitação |
| ED11 e capacidade | U7; G7, ED11 W1-W6 | Conforme como gate: custo0 nesta documentação; sem reserva/quotas novas; prova compilada/transitiva futura obrigatória; totalPARTIAL |
| Não autorização funcional/operacional | U6/U7 e gates finais | Conforme: não aceita APIs/hunks por referência, não concede CURRENT/commit/assinatura/E0/TRUST/admissão; Browser independente |

## Limites de avaliação

Conformidade semântica de uma obrigação não prova implementação: CURRENT factual permanece PENDING_EXTERNAL_ENVIRONMENT, token administrativo positivo bloqueado pelo ambiente e UTC/status operacionais ainda exigem qualificação. Censo conhecido fixo Windows1948656/Legacy1790872 e cenário completo3747601 são evidências anteriores, não pico medido nesta tarefa nem crédito disponível; excedente601873 sobre Windows3MiB não é removido pela proposta. Provider/kernel/stack transitivo UNKNOWN. Nenhum build/teste/rede foi executado, conforme seção10.

## Prontidão para registro posterior

Aceitação integral da Revision1 NÃO recomendada. Após revisão e autorização humana da correção U6, o futuro registro pode incorporar exclusivamente U0-U7 corrigido em anexo congelado, fixando fonte/intervalo/hashes e precedência limitada ao ato UPDATE. Deve preservar D2/D3/D4, F/G, E0, modos/formatos/CPs, Model E, ED08/ED11 e todos os gates; vocabulário histórico PROPOSED pode ser superado somente quanto ao status normativo dos bytes incorporados. Esse roteiro não é draft ACCEPTED, não aceita automaticamente Revision2 nem patches funcionais.
