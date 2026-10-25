# U6 corrigido / U0-U7 — conformidade para aceitação delimitada

R1 da revisão anterior está resolvido pelo único hunk autorizado; não foi identificado conflito material remanescente. Conformidade é normativa, sem qualificação funcional/ambiental. Proposta original e revisão anterior não são reescritas.

| Condição da tarefa | Evidência / cláusula | Resultado |
|---|---|---|
| Persistência histórica | U6 mantém persistir disposições apenas em PACROOT8/v3/PDG1/CP3 accepted | Conforme; não serializa prova transitória nem cria journal/recibo novo |
| Restart sem autoridade automática | U6 corrigido nega validade operacional e restauração do ato; D3.7/G5/P6 exigem provas próprias | Conforme; R1 encerrado, arquivo/checkpoint íntegro não autoriza |
| CURRENT/publicação/reuso | U2/U3/U6 e D2.2/G4: ato por imagem/operação/tupla, base factual, revalidação e commit antes de CURRENT | Conforme; restart não publica, não emite epochs nem reusa prova para outra candidata |
| Recuperação e incerteza | U6/D3.7/G5/P6: DENY e retenção, sem fallback/rollback presumido | Conforme; AUTHORITATIVE_RECOVERY continua DENY |
| Stop/ownership/pins/borrows | U3/U5/U6/G7/E0 e Model E | Conforme; quiescência/fechamento factuais, sem liberação por timeout; RELEASE_INVALID preservado |
| Bootstrap v1 e base v2/v3 | U0/U6 sob D2/D3/G0/G5 | Conforme; modos separados; base v2 usa CP2 e base v3 CP3, não CP3 fictício em v2 |
| F/G, SNT/PDG/issuer/policy/root | U2/U4/U6 e F1-F5/G0-G7 | Conforme; tuplas/propósito/status/revogação separados; uma root, nenhum provider/registry paralelo |
| Identidade versus aprovação | U1 permite ato explícito U2 pelo administrador local; token isolado não é aprovação | Conforme; não reutiliza bootstrap/C8 como fonte UPDATE |
| UTC qualificada | U4 mantém gate factual de backend aprovado/qualificado | Conforme como requisito; não qualifica clock/backend por texto |
| E0 e confiança | U5/U6; E0-E8/D3/F3/F4 | Conforme; acquire síncrono protegido, nenhum lease/trust positivo; Browser independente |
| ED08/ED11 | U5/U7 e Model E; perfil Windows explícito | Conforme; nenhum custo runtime novo, nenhuma quota/reserva ou ativação automática, totalPARTIAL |
| Incorporação exata | Patch um hunk reconstruído em memória, anexo congelado/hash/intervalos; U0-U5/U7 byte-exatos | Conforme; fontes e metadata PROPOSED preservadas |

## Precedência e implementação posterior

Registro aceita somente as cláusulas corrigidas e sua especialização do ato UPDATE; não supersede decisões superiores nem aceita artefatos funcionais por referência. Status histórico PROPOSED dentro das cláusulas não cria ambiguidade de autoridade: o registro designado supera somente esse status dos bytes incorporados. Todas as restrições e fontes factuais exigidas permanecem.

CURRENT factual, autorização nativa positiva e UTC/status qualificados continuam gates independentes da aceitação. O produtor não está implementado por este registro. Antes de código: dimensionar estruturas/prova/handles, fontes e grafo de retenção no mesmo owner; revisar hunks exatos; não usar committed/previous/source publicados como scratch. Sem capacidade comprovada ou fonte factual, DENY. Nenhum build/teste/rede executado nesta tarefa.
