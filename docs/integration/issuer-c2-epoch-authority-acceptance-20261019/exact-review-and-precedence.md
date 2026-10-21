# Revisão exata das12 condições — aceitação documental

Proposta revista integralmente: CONTRACT-PROPOSED.md revision1. Handoff/impacto dos18 hunks são fontes de orientação PROPOSED, não conteúdo automaticamente aceito. A revisão não executa código ou prova implementação. A autoridade para registro accepted é a autorização condicional específica PACC-PHASE6-ISSUER-C2-EPOCH-AUTHORITY-ACCEPTANCE-1, condicionada a esta revisão e aprovação oficial da escrita.

| Condição do owner | Cláusula exata da proposta | Conclusão delimitada |
|---|---|---|
| 1: identidades distintas | tabela §1, P1.1–P1.3 | I/R/A/P/S/Principal/grants distintos; igualdade numérica não é relação factual |
| 2: preservar P só com projeção idêntica | P2.1–P2.3, P3.3 | ISSUER_ONLY exige prova de conteúdo/vínculos operacionais inalterados; UNKNOWN nega |
| 3: transição para mudanças operacionais | P2.1–P2.2, P3.4, P4.6, matriz §5 | policy, semântica/mapping autorizador de Principal, grants e demais inputs operacionais, inclusive admissão, não viram simples cadastro; exigem transição correspondente |
| 4: snapshots/pins até finalização | P4.1–P4.2, P4.7, P7.2 | aquisição antes de exposição, imutabilidade, quiescência, retenção/crédito e slot protegido |
| 5: pin não concede CURRENT | P4.2–P4.3 | pin histórico prova memória/proveniência apenas |
| 6: OPEN/BIND sem autoridade obsoleta | P4.3–P4.5, P5.1 | CURRENT, P vigente e gates normais; nenhuma exceção de epoch ou renegociação implícita |
| 7: revogação ativa pendente | P5.2–P5.3 | R-ACTIVE específico para credencial/Session já viva; não suspende enforcement de revogação operacional already accepted |
| 8: commit antes CURRENT/epoch | P3.1–P3.5 | participantes e commit normativo confirmado, sem ack/epoch fictício nem CURRENT positivo parcial |
| 9: recuperação comprovada | P6.1–P6.4, matriz §6 | checkpoint, high-water e fencing de instâncias antigas; integridade/timeout não bastam |
| 10: uma autoridade/sem paralelo | P0.2, P1.1–P1.3, P3.1–P3.2, P6.1 | A é contexto da mesma root; sem contador emissor autônomo, registry/journal/root/wire paralelo |
| 11: Model E, ED08/ED11, admissão | P0.2, P4.5–P4.7, P7.2–P7.4 | nenhuma dispensa de destruição factual, closure, quota, retenção, recurso UNKNOWN ou admissão |
| 12: contrato portável | P7.1 | sem CNG/Win32 no contrato comum; Windows/box/ISA representáveis, sem qualificação implícita |

Nenhum conflito material com as autoridades accepted foi identificado na relação proposta. P0.1 descreve somente o estado histórico de proposta e não será promovido como declaração de que o novo registro continua proposed. P0.2 e seções1–7 serão incorporados integralmente sem reescrita, com seu significado normativo esclarecido no cabeçalho de aceitação. Expressões preservadas como “esta proposta” não aceitam handoffs/hunks, não concedem implementação e não alteram a ausência de qualificação.

## Confronto com autoridades accepted

- C2 revision1 §4: campo wire P continua não zero, server-issued, com identidade de operação imutável e policy atual; BIND mantém Session/Principal, token/epoch/expiry e current policy. §6 mantém revogação local, dados já aceitos, narrow CANCEL/FINAL e ausência de fabricação de destruição.
- ADR0012: upper bound imutável mais enforcement contextual atual; sem silent add. P4/P5 não permitem que uma referência antiga substitua esses gates.
- ADR0048: mandatory participants, commit normativo, CURRENT, epoch pela autoridade existente; componente/file hash não bastam. P3 não cria segundo authority provider.
- Issuer accepted cláusulas2/7/8/11: emissão não concede acesso; mesma root live futura; revogação nova admissão e active-policy pendente; ED11 preservado.
- Perfil técnico accepted §2.2–2.5/§3.5–3.6: reserva antes da assinatura, CAS e high-water, nenhuma reutilização após incerteza, fencing/checkpoint/backup. Nenhuma chave/certificado é criado pelo registro.
- Model E ADR0033 e C2 §11.2/§11.3: attempted release não é DESTROYED; terminal-retained conserva dependências e crédito; encerramento ED08 ordenado, quota não ampliada, provider/kernel UNKNOWN continua DENY para admissão aplicável.

## Precedência

O novo registro somente esclarece que avanço de revisão administrativa não implica automaticamente transição P se a projeção operacional permanecer provadamente idêntica, e que A nunca substitui P. Ele complementa o registro de coexistência e a sequência root do issuer. Não revoga ADRs anteriores nem sua condição operacional CURRENT. Prevalece sobre inferências de relatórios/handoffs de que todo epoch/campo privado já significa simultaneamente A e P. O WIP e os18 hunks continuam evidência/propostas não qualificadas, e nenhuma negativa do executor é revogada.
