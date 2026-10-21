# Revisão exata C1–C5 — PROPOSED

GOVERNANCE CONFLICT: C2 revision1 define CERTIFICATE_IDENTITY como identidade do canal. ADR-0028 separa Transport Security de Authentication e preserva CLOSED/certificate identity; a decisão accepted production-client-provisioning cláusula4 restringe senha no enrollment e explicitamente não elimina CLOSED por certificado nas demais operações. Não alterar nenhuma autoridade accepted para acomodar a proposta. Nenhuma implementação dependente é permitida nesta revisão.

## Matriz de conformidade

| Cláusula/condição | Revisão1 | Evidência/menor correção | Revisão2 proposta |
|---|---|---|---|
| C1 offline/versionado/sem autoridade | CONFORME | N6/F5; sem AST persistido, arquivo/root/registry novo | preservado |
| BODY0.5/ROOT06/CURRENT/epochs intactos | CONFORME NO ESCOPO OFFLINE | BODY05 encode1024 e ROOT06/linked16896 existentes; nenhuma seção é gravada neles | preservado; envelope coleção continua gate separado |
| C2 layout128+N*512+32, N0..4 | CONFORME | intervalos contíguos header0..127, registros0..511, SHA trailer; total160..2208 | sem offsets novos |
| C2 presença/admission | AMBÍGUO NA LEITURA | byte5 enum obrigatório sempre presente, sem zero/default; bits0..5 obrigatórios e condicionais6/7 | explicitar, sem ampliar |
| C2 autenticação | CONFLITO MATERIAL | não equiparar certificate identity de acesso e mTLS; manter modelo/gramática candidatos; subset codec CLOSED/CERTIFICATE_IDENTITY UNSUPPORTED | corrigido semanticamente; nenhum provider criado |
| C2 gramática/strings | INCOMPLETO | ident()=[A-Za-z][A-Za-z0-9_.-]*; UTF8 e strings decodificadas, byte bounds, controles, case, tail | explicitado; sem parser novo |
| C2 predecessor | PERMISSIVO RELATIVO AO BODY | body04_model exige revision=predecessor+1 sem wrap | igualdade exata R-1 proposta, não prova de atualidade |
| C2 BODY digest/R/A/P | INSUFICIENTEMENTE EXPLÍCITO | BODY0.4 exige policy_epoch0; A/P reais vêm da root, não do BODY. Hash1024 inclui trailer; domain/R/predecessor e policy vinculados externamente | corrigido sem alterar bytes existentes |
| C2/C4 não promover MODEL_BOUND/VALIDATED | CONFORME | bind_existing_models só model_bound; adapter CURRENT separado; topology ainda não proven | codec não contém flags de resolução/autoridade; decode não resolve |
| C3 saída inválida/overlap | CONTRADIÇÃO POTENCIAL | não pode garantir tamanho zero se pointer/overlap é inválido e escrita proibida | pré-condições inválidas não escrevem; falha com outputs seguros zera metadados; retorno sempre obrigatório |
| C3 lifetime | PARCIAL | input imutável/disjunto; faltava contrato de hash callback e resultado não emprestado | synchronous no-retain/no-reentry, output próprio, nada no Stop/event loop |
| C3 integridade e histórico | CONFORME COM LIMITES | SHA relativo não assinatura/freshness/fencing; corrupt/unknown/trailing rejeitados; leitores históricos não reinterpretados | preservado; testes ainda não executados |
| C4 aplicação/CLI | CONFORME COMO HANDOFF | SOURCE/PARSED/VALIDATED/MODEL_BOUND/RESOLVED/PREPARED/COMMITTED/CURRENT; CAS UNKNOWN nega; nenhum apply/publish implementado | preservado; listener coleção não vira singular |
| C5 ED11 | CONFORME CONDICIONAL, NÃO RESERVA | 2208 fragmento;4416 apenas dois buffers; não inclui normalizado/AST/stack/hash/retidos. Root182688 e managed1807176 anteriores não são orçamento livre | explicitar custo transitivo por patch, sem aumentar2097152 |

## Hunks normativos exatos

normative-hunks.patch e hunks.json identificam nove substituições únicas, sua preimagem completa SHA256 e texto anterior/proposto. A revisão2 integral em CONTRACT-REVISION2-PROPOSED.md é material de revisão, não substitui o arquivo original. H1 incrementa revision; H2 predecessor; H3 separação auth/admission; H4 strings; H5 representabilidade/auth suporte; H6 digest/proveniência; H7 saídas; H8 lifetime/hash; H9 ED11. Nenhum hunk funcional é preparado ou aceito.

Aceitação futura deve delimitar C1–C5 com estes hunks e a preimagem exata. Não aceitar por referência a envelope futura, comandos CLI ou patch funcional. Não alterar ADR-0028, I4, F5, N6, BODY/root ou gramática para acomodar limitações do primeiro codec. I4 mantém guards de provider/acesso aplicáveis; não ampliar seu escopo de modo a revogar as combinações representáveis do ADR-0028. A especificação de uma autenticação operacional concreta permanece decisão própria.

## Validação desta revisão

Inspeção estática de contrato, parser/bridge, codecs BODY04/05, root/issuer linked constants e decisões accepted. Nenhum build, teste, rede, assinatura, certificado ou publicação. Não alegar completude de runtime ou qualificação do codec não implementado. Testes necessários: presença/ausência; campos0/max; conjuntos/ordem/duplicatas; truncamento/corrupção em todas regiões; unknown magic/version/flags/enum; length overflow/tails/trailing; UTF8/escapes já decodificados; hash falho/não disponível; overlap/saída stale; CLOSED/CERTIFICATE_IDENTITY unsupported sem conversão; SELECTED serializável como dados mas apply DENY; nenhuma flag operacional em decode; comparações BODY/proveniência externas; formatos históricos inalterados. Fixture SHA sintético não prova backend nativo.

## Decisão e limites

CURRENT_TASK_COMPLETED=YES (revisão focal e proposta de correção somente)
CONTRACT_REVISION1_CONFORMANCE=CHANGES_REQUIRED
CONTRACT_REVISION2_STATUS=PROPOSED_NOT_ACCEPTED
NORMATIVE_HUNKS=9_PREPARED_NOT_APPLIED_TO_ORIGINAL
FUNCTIONAL_SOURCE_CHANGED=NO
ORIGINAL_CONTRACT_CHANGED=NO
ACCEPTED_CREATED=NO
BUILDS=NOT_RUN
OFFLINE_TESTS=NOT_RUN
NETWORK_TESTS=NOT_RUN
ED11_TOTAL_STATUS=PARTIAL
INTERFACE_TOPOLOGY_APPLICABILITY=DENY_UNTIL_QUALIFIED
AUTH_PROVIDER=PENDING
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
FIRST_UNRESOLVED_DIVERGENCE=OWNER_ACCEPTANCE_OF_CORRECTED_C2_AND_C3_PENDING
NEXT=OWNER_REVIEW_OF_EXACT_CODEC_CONTRACT_CONFORMANCE
