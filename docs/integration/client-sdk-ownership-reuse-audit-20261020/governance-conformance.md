# Matriz normativa

| Obrigação | Autoridade exata | Resultado |
|---|---|---|
| Taxonomia/aceitação | OEADR0001/0002/0003 |Relatório separado de ADR candidate;proposed não implementação|
| Shared owner first | PEADR0011rev2, Regra central/O que conta/Componentes existentes/Dependência correta |Gramática M3 comum comprovada;parar novas extensões locais e designar owner. Princípio geral já existe|
| Core/provider | PEADR0007/0010 |Sem Win32/store/root no header comum;UNSUPPORTED honesto|
| Identidade/Trust/AuthN/AuthZ | PEADR0009;AcceleratorADR0006/0028 |Assinatura/senha/hash/CSR não criam autoridade|
| Multi-consumer | BrowserADR0007 Contexto/Multi-consumer/Escopo |Já explícito;Browser não define wire/adminservidor|
| Uma implementação wire | AcceleratorADR0025 Decisão |PACC intacto20/20;sem provisão dentro do protocol-only|
| Distribuição | BrowserADR0013 Decision/Provenance convention |Archive/snapshot/upstream iguais;fixes upstream/pin atômico|
| Modos/custódia | provisioning18/10;issuer/manualcustody3-5/9 |Manual exceção limitada;automático chave cliente;simple/advanced convergem|
| Formato/M1 | M0-M8 frozen e metadata congelada |Sem reinterpretar bytes ou KDF antes trust|
| Trust/instalação | D3.1-D3.11/F1-F5 |Policy/issuer servidor;confirmação independente e instalação consumidor|
| Delegação persistente | G0-G7/matrizaccepted |Rootservidor;não obrigar Browser a conhecê-la|
| Provenance local | BrowserDECISIONprovenanceRevision2 |Formato/seleção/replay locais;0não reativa;sem confluir com PDG|
| Enrollment | P0-P9/T0-T7Rev2;L0-L5 |PAE1/ALPN próprios;mTLSoperacional intacto;helpers parciais|
| Root/C8/epochs | root/issuer-C2 e contratos existentes |Sem CURRENT fabricado ou CASfallback;Browser não possui autoridade da root|
| Lifecycle | PSTModelE/ADR0008/0009;ED08 |Retidos preservam dependências;close não é timeout/FINAL|
| Memória | accepted incremental envelopes;Windows3MiB;M7/D3.10/G7 |UNKNOWN não zero;sem quotas novas;ED11PARTIAL|

GOVERNANCE CONFLICT — alternativa A condicional: ampliar papacc_protocol para policy/provisionamento contrariaria ADR0025 Decisão vigente. Nenhuma alteração dependente executada. Alternativa recomendada preserva a boundary. A duplicação identificada exige disposição sob PEADR0011, não uma permissão para outras cópias permanentes.

Não usar PEADR0001 superseded como autoridade corrente; OEADR0002 rege governança de ADRs. authority-discovery.json é discovery/status/hash, não aceitação integral dos arquivos por referência nem declaração de aplicabilidade universal. Registros de pacote incorporam somente intervalos/anexos congelados. Propostas históricas mantêm precedência delimitada. Future network-egress/protocol governance do Accelerator são direção/contexto, não API SDK accepted. Browser sem docs/future no checkout examinado.
