# Contexto de serviço e provisão — fronteiras existentes

1. Identificar a conta administrativa e a conta operacional legitimamente selecionada sob P3, sem criar conta ou resolver nome como prova. Qualificar TokenUser/contexto efetivo, ausência de impersonação e separação de papéis; registrar SID desconhecido como UNKNOWN. Seleção SCM não herda o resultado do console nem escolhe identidade implicitamente.
2. Reutilizar `personal_native_open/current`, `personal_source_current`, presenter conjunto e worker read-only para identidade de parent/source e revalidações. O resultado atual de preflight é DENY; não emitir permit por não haver erro Win32. Não solicitar nem ler segredos.
3. Preparar prova MACHINE KSP/ACL sob dois contextos reais, incluindo negativa de uso privado pelo runtime, somente após autorização própria para material persistente/identidade. Não usar a chave CurrentUser retida nem criar outra chave em testes automáticos.
4. MANUAL: ligar a fonte original de delegação/package signing e a aquisição borrow-only E8 antes de KDF. `mp_auth_pdg_candidate`, `sp_trust_acquire` e Browser M1 TRUST permanecem negativos enquanto faltar essa fonte. Receipt do presenter e certificado de fixture não são a fonte.
5. AUTOMATIC: preservar o claim/trace no owner e a distinção de persistência L2. Não registrar listener, rede ou formato novo por inferência. Expor a primeira dependência concreta, preservando DENY.

Não executar instalação/CA/CURRENT, serviço/SCM/ACL, rede, TPM/Q4 ou cleanup/refund. Não confundir preparação técnica com autorização operacional. Nenhuma nova decisão normativa é necessária para reafirmar P0/P3/P4 ou M0–M11; eventual diferença real deve citar a cláusula exata.
