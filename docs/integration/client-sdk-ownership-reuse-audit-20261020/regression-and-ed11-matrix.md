# Evidências históricas e ED11

Nenhum build/teste de produto foi executado nesta auditoria. Resultados abaixo não são novos PASS e não superam negativas oficiais.

| Componente | Evidência anterior consultada | Limite |
|---|---|---|
| PACC | Accelerator docs/integration/papinhopacc.md;BrowserADR0013 | VC6/consumer/C90/goldens históricos. Aqui só20 hashes comparados. Live doc histórica registra14 falhas secure em77 testes do repo; não afirmar suite total atual PASS |
| Browser prefixo público | papacc-simple-import-implementation-20261020/CLOSEOUT.md |1416 checks Debug/Release e GUI negativos históricos;sem conteúdo/trust positivo|
| Browser KDF/AEAD | papacc-crypto-adapter-vc6-20261020/CLOSEOUT.md;CALLERS-M1.md |940 checks/config, KAT/cross-provider/Stop;VC6 host moderno;NT4 NOT_RUN|
| Browser provenance | papacc-provenance-v2-implementation-20261020/CALLERS-AND-OWNERSHIP.md |Mesmo writer/selector/gate/leases;dados LAB sintéticos;replay não autentica|
| Browser novos D3 helpers | papacc-d3-independent-import-20261020/CLOSEOUT.md;REVIEW-OBJECTION.md |Compilados;novos testes NÃO executados após negativa. Objeção preservada, não reexecutada|
| Accelerator PDG1/M1 | pdg1-m1-trust-consumption-20261020/closeout.md |2 package+14root suítes históricos por4config;admin sintético/crypto TEST;TRUSTDENY|
| Enrollment | automatic-enrollment-listener-pairing-20261020/closeout.md |10suítes/4config históricos;DATA/PSTprojection/RNG;sem listener/TLS nativo|

Regressões futuras: mesmo corpus M2/ZIP/TLV para ambos; golden/limites/versão/trailing/corrupção/aliases/outputs em falha. KAT/cross-provider PBKDF2/RFC8439/RSA exatos, sem reduzir custo operacional. Zero KDF sem assinatura+trust; q privada através AEAD/CONTENT/recheck/RELEASE; ledger inativo não reativa. Revogação, CURRENT/pin inválido, confirmação divergente e Stop negam. Fixture antes rejeitada requer revisão oficial própria; não contornar por novo gerador.

ModelE: ownership_accepted separado do resultado; só DESTROYED permite finalizar; PRECONDITION_RETAINED/CLOSE_UNRESOLVED_RETAINED conservam dependências conforme contrato. Pin root não protege objeto externo. Timeout/process exit/FINAL não são quiescência ou cancelamento. Crédito volta por fechamento factual, não por retorno de helper.

## Custos e cenários

Delta runtime desta auditoria:0. SDK não implementado, portanto sem censo compilado SDK. M7 arrays149848 são subtotal ilustrativo, não reserva. Último censo PDG1/M1 conservador3625925 excede Windows3145728 por480197 antes de UNKNOWN; não é pico operacional medido, nem prova universal de que toda arquitetura SDK excederá. Ativação dependente continua negada.

Browser tem orçamento próprio. Windows3MiB accepted é do processo Accelerator, não por cliente nem aplicável automaticamente ao Browser. Captura pública pode alocar131072, q até65536; cópias, KDF/HMAC/RSA/DER, certificado/chave, ledger, stack/callbacks, fontes/gerações antigas/retidas precisam censo único por owner. Não somar spans e slabs duas vezes. ZIP STORE não precisa decompressor. Heap/stack/provider externo não são memória grátis.

Unificar fontes elimina divergência, não prova redução de buffers. Workspace do caller só com exclusividade demonstrada; não reutilizar RX emprestado/pinned/retido. Provider/kernel/transitive/coexistência UNKNOWN, ED11_TOTAL_STATUS=PARTIAL, ED11_ADMISSION=DENY. Nenhuma quota ou reserva foi alterada.
