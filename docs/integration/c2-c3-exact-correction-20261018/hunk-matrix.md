# Matriz de hunks exatos

As linhas abaixo são da preimagem. old/new completos estão nos manifests; não houve substituição global. O patch funcional foi aprovado e aplicado separadamente dos builds. As regressões receberam submissão oficial própria.

| Hunk | Arquivo | Função | Linha anterior | Contrato, efeito e regressão |
|---|---|---|---:|---|
| PST01 | apps/server/server_c2_dispatch_binding_win32.inc | static PAPACC_RESULT configuration_site_wire( | 95 | SDK API3 nested structs and exact provider/caps; replace absent API, no stub. Validate compile/link and fields. |
| GRACE01 | apps/server/server_c2_dispatch_binding_win32.inc | static PAPACC_RESULT configuration_site_wire( | 99 | PST feature enum2 and C3 exact guard/ED08; no release order change. Validate Stop/retention and source. |
| DEADLINE01 | apps/server/server_c2_dispatch_binding_win32.inc | PAPACC_RESULT papacc_server_launcher_c2_deadline( | 40 | Remove local used by invalid second concurrency count; zero-warning compile. |
| DEADLINE02 | apps/server/server_c2_dispatch_binding_win32.inc | PAPACC_RESULT papacc_server_launcher_c2_deadline( | 42 | C2 factual Session/grants/root revalidated, no second count of RESERVED slot. Preserve checked absolute deadline/live expiry. DENY negative and static expiry audit; native positive pending. |
| SITE01 | apps/server/server_c2_dispatch_binding_win32.inc | static PAPACC_RESULT configuration_site_prepare( | 70 | Model E factual trust acquisition; reject null success, preserve uncertain nonnull reference, no retry. Test local prepare/abort/retention. |
| GRACE02 | apps/server/server_launcher_owner_win32.c | PAPACC_RESULT papacc_server_launcher_c2_site_template( | 975 | PST/C3/ED08: reject OPTIONAL/disabled, preserve other authority checks. Source and Stop/retention regressions. |
| ALPN01 | apps/server/server_launcher_owner_win32.c | PAPACC_RESULT papacc_server_launcher_c2_site_template( | 985 | C2 bounded ALPN: guard before unsigned subtraction. Test limits/mismatch. Existing factual negotiated verifier unchanged. |
| SITE02 | apps/server/server_configuration_root_transaction_win32.inc | static int root_dormant_owned( | 9 | ADR0044/0048 and Site decision: actual trust/owned empty C3 pool before commit after apply; no synthetic ready. Test missing trust DENY and local commit/abort. |
| T01 | tests/canonical_root_transaction_test.c |  | 4 | Private consumer declaration, not PST API; allows no-authority negative. |
| T02 | tests/canonical_root_transaction_test.c | int main( | 39 | Verify missing Session/owner cannot mint a deadline or capacity authority; no synthetic authentication. |
| T03 | tests/canonical_root_transaction_test.c | int main( | 73 | Verify missing mandatory trust blocks commit preparation before file effects; actual reference restored, no resource release/fiction. |
| T04 | tests/site_tls_catalog_offline_test.c | int main( | 38 | Exact ALPN allow/mismatch/truncation/empty/oversize regressions of real pure validator; not negotiated TLS evidence. |
