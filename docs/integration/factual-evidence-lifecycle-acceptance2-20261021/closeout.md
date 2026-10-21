# Closeout — E0–E8 corrigido, aceitação normativa delimitada

R1/R2 reconciliados pela correção exata E1/E6/E7, sem alterar proposta/história. O registro accepted incorpora apenas E0–E8 corrigidos por anexo byte-exato após aprovação oficial. acquire é ligação síncrona borrow-only: não inicia aquisição nativa e nunca retorna RETAINED; qualquer preparação anterior retida continua alcançável/consultável no owner original e mantém binding DENY até prova factual. Release INVALID é precondição sem tentativa/efeito parcial, conserva owner/borrow previamente conhecido e disposição anterior. Após tentativa incerta: somente query, sem retry destrutivo.

[Decisão](../../adr/DECISION-factual-evidence-e0-e8-lifecycle-20261021.md) · [conformidade](conformance-matrix.md) · [estados](state-and-disposition-matrix.csv) · [incorporação](incorporation-and-preimages.json) · [handoff ED11](ed11-and-implementation-handoff.md) · [hashes](final-manifest.json).

As exigências de tentativa/consulta externa são precondições do binding futuro, não qualificação de API produtora atualmente disponível. Este perfil não suporta acquire com trabalho assíncrono/pendente; essa necessidade deve negar em vez de inventar handle. A aceitação é normativa, não implementação/compatibilidade nativa/reserva/fonte factual/CURRENT/trust positivo. Declarações C89 revisadas por inspeção; nenhum build/teste/rede executado.

Propostas originais e relatório STOP preservados; apenas novo registro/anexos e append factual de capability aprovados. HEADs/main e WIP dos dois projetos verificados, quatro LAB não acessados. Sem fontes funcionais/Browser/PST/wire/prod0.6.4, clones/worktrees, chaves/certificados/pacotes, commit/push/tag/release. Provider/kernel UNKNOWN/ED11PARTIAL e gates fechados.

```ini
CURRENT_TASK_COMPLETED=YES
FACTUAL_EVIDENCE_CONTRACT=ACCEPTED
ACQUIRE_RETAINED_LIFECYCLE=BORROW_ONLY_NO_PENDING_ACQUIRE_PRIOR_OWNER_RETAINS
RELEASE_INVALID_SEMANTICS=NO_ATTEMPT_NO_PARTIAL_EFFECT_OWNER_BORROW_PRESERVED
SHARED_M1_STATE_MACHINE=IMPLEMENTED_OFFLINE
IMPLEMENTATION_AUTHORIZED=NO
FACTUAL_SOURCE_BINDING=DENY
BROWSER_MIGRATION=NOT_AUTHORIZED
PAPACC_INSTALL=DENY
AUTOMATIC_ENROLLMENT=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PST_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
BUILDS=NOT_RUN
TESTS=NOT_RUN
NETWORK_TESTS=NOT_RUN
WIP_ACCELERATOR_PRESERVED=YES_EXACT_NORMATIVE_NEW_FILES_AND_CAPABILITY_APPEND_ONLY
WIP_BROWSER_PRESERVED=YES_UNCHANGED
FIRST_UNRESOLVED_DIVERGENCE=NONE_IN_NORMATIVE_RECONCILIATION_FACTUAL_BINDING_UNQUALIFIED
NEXT=SHARED_FACTUAL_EVIDENCE_API_IMPLEMENTATION
```

YES depende da aprovação oficial da escrita e conferência pós-gravação, não de aprovação automática de qualquer fonte funcional. Implementação posterior exige autorização/revisão própria e todas as fontes/lifetimes/recursos factuais; este registro não substitui esses gates.
