# Inventário factual — clientes/revogação/histórico

| Fonte | Fato | Falta |
| --- | --- | --- |
| src/issuer/issuer_core.h/.c | Request/record; PREPARED/RESERVED/SIGN_REQUESTED/ISSUED_UNPUBLISHED/PUBLISHED/REVOKED/ABANDONED/RECONCILIATION_REQUIRED; prepare/reserve/issue/query/transition/recover/Stop/finish | Fonte factual/UTC/provas por propósito, nenhuma listagem/CLI operacional só por esses exports |
| issuer_codec.c | Validação de record/transitions/serial/predecessor/revogação/renovação | Não há campo de ocultação/idade autorizando uso de padding |
| server_configuration_issuer06_win32.inc | Mesmo owner/root, projection/inspeção/adapters/recoverynegado; DATAzera epoch/proofs | Record íntegro não é CURRENT/autoridade; issuer hash histórico ainda tem BCrypt e custos opacos, não globalmente migrados |
| src/provisioning/enrollment*.c/.h | Framing/trace/pair-code/context/limiter/Stop DATA e adapterWindows/PST | Native approval/policy/CURRENT/CSRbinding ainda não fonte positiva; DATA_ONLY explícito |
| manual_package*.c/.h | Pacote/crypto/PDG/validação existentes | Crypto não substitui proveniência/autorização/liberação/import factual |
| src/provisioning/shared_structure/* | Core C89 estrutura/suite/M1 e extrações/equivalência | Manter shared-owner-first; não reimplementar no Browser; autoridade E0–E8/adapters ainda necessária |
| server_c2_configuration_win32.c e issuer/C2 | Admissão/policy/snapshots/epochs/referências | C2 pairing de role pósSession não é enrollment inicial |
| server_config_cli.c | Dispatcher de configuração | Sem views active/history, revoke/hide-history/retention operacional completo no dispatcher |

EnumREVOKED/papacc_issuer_transition não significam UI, status publicado ou teardownSession. R-ACTIVE é POLICY_PENDING vigente; M7 propõe fechar. Remover record para esconder quebraria predecessor/serial/status e poderia reabilitar credencial.

T0–T7, ALPN/framing de enrollment e listener separado já estão accepted nas decisões de20/10; P7 histórico não significa lacuna normativa atual. A lacuna é integração/representação/autoridade/capacidade: runtime singular não trunca coleção. Manual/automático não são operacionais por codec. E0–E8 é accepted em DECISION-factual-evidence-e0-e8-lifecycle-20261021; handoffs anteriores dizendo PROPOSED são históricos, fontes ausentes continuam DENY.

Exibir nome/tipo/fingerprint/cert/data só com fonte canônica; dataUNKNOWN não vira relógio inventado, CSR/CN não vira identidade aprovada. Sem vínculo factual registro-cliente-credencial, mostrar indisponibilidade. Views futuras usam snapshot/pin/autorização e limites bounded do cadastro existente, sem segundo banco. Browser primeiro consumidor viaSDK, não dependência de núcleo; nenhuma alteração Browser/PST aqui.

