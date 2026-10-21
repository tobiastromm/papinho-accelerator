# Enrollment listener ADR-0009 specialization — closeout

Decisão arquitetural explícita do owner registrada em ADR complementar L0–L5 após aprovação oficial: endpoint ENROLLMENT separado, perfil imutável anterior à publicação, mesma coleção/root; operacional mantém mTLS/TLS1.3/papacc/1. ADR-0009 e demais accepted anteriores preservados.

Concluídos T2/T6 Revision2 como PROPOSED: ALPN papacc-enroll/1, framing PAE1/1.0 byte-exato,8mensagens/direções/estados/tamanhos, erros/fechamento e continuidade, limites pré-código/handshake/origem/retidos/Stop/ED11. Nova proposta não altera implementaçãoPACCwire/PST. UmaCONFIRM é restrição explícita nova do perfil candidato, sujeita à aceitação, não alteração silenciosa deP4. Originais P0–P9/T0–T7 intactos. Cópia composta e hunks exatos incluem sóT2/T6 e coerência necessáriaT0/T3/T7.

ED11: subteto candidato262144, custo hipotético selected2365991 dentro de3145728 antes de custos não qualificados; Legacy2329799 excede2097152, portanto enrollment negado nesse perfil sob a composição atual. Nenhuma quota/reserva foi aumentada, nenhum buffer adicionado. Opaque/provider/kernel UNKNOWN; totalPARTIAL. Persistência/modelo/coleção runtime precisam extensão/capacidade qualificadas antes de ativarENROLLMENT; não reutilizar enums/bytesreservados. Sem fonte factual/admin/CURRENT/issuer, operações continuamDENY.

Minuta de aceitação delimitada preparada; ALPN/mensagens/limites NÃO ACCEPTED por inferência. Não executar implementação antes da aceitação específica e revisão oficial de patch. Nenhum build/teste/rede/chave/certificado/signature/CURRENT foi executado; Browser/PST/produção0.6.4 e wire operacional inalterados. WIP/preimagens/hash preservados.

```ini
CURRENT_TASK_COMPLETED=YES
ENROLLMENT_ENDPOINT=SEPARATE_LISTENER
ADR0009_SPECIALIZATION=ACCEPTED_ARCHITECTURE_ONLY
T2_STATUS=COMPLETE_BYTE_EXACT_PROPOSED_REVISION2
T6_STATUS=COMPLETE_EXPLICIT_LIMITS_PROPOSED_REVISION2
P0_P9_STATUS=PROPOSED
T0_T7_STATUS=PROPOSED
IMPLEMENTATION_AUTHORIZED=NO
AUTOMATIC_ENROLLMENT=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
ED11_TOTAL_STATUS=PARTIAL
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=EXACT_T2_T6_APPLICATION_BYTES_LIMITS_AND_FULL_PAIRING_CONTRACT_NOT_YET_ACCEPTED
NEXT=PAIRING_AND_INITIAL_TRANSPORT_NORMATIVE_ACCEPTANCE
```
