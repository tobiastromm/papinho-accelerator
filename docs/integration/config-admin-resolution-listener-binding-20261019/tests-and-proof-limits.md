# Evidência de testes e limites

4 invocações CTest por configuração, todas PASS;8 no total. Parser121 verificações, modelo20 verificações e native guard11 por configuração. Contagens incluem asserts auxiliares; não152cenários independentes. Layout da root é quarta invocação, mede sizes e nega admissão, sem medir recursos físicos.

MODEL positivo: PAPACC_SERVER_CONFIG/policy/security refs são fixtures sintéticas com identidades declaradas apenas no teste; código real de validação/bind-targets executado. Resultado MODEL_BOUND, resolved=0. Não é prova de root normativa, credenciais ou autorização.

Native guard: entrypoint real ligado ao owner canônico rejeita NULL e owner/pin não pertencentes à root; saída resolvida ausente. Fixtures stopped/unresolved/revision divergente continuam rejeitadas na primeira fronteira de root: NÃO provam execução dos ramos posteriores de Stop/stale/source sob um pin realmente CURRENT. Stop interlocked foi efetivamente solicitado/observado no processo diagnóstico próprio; não sinalizou produtos externos. Nenhuma transação/publicação CURRENT foi executada. Positive RESOLVED com root factual PUBLISHED e material real NÃO executado.

| Requisito | Cobertura factual |
|---|---|
| Referência válida | fixture content-match MODEL_BOUND; native RESOLVED positivo não executado |
| Ausente/ambígua | testes do bridge, saída atômica inválida |
| Interface inexistente | texto SELECTED sintaticamente válido rejeitado UNSUPPORTED sem lookup inventado; não enumeração nativa de interface |
| Perfil ausente/Legacy | parser121 inclui ausência/unknown/Legacytarget; bridge recusa perfil Legacy sem fallback |
| Listener excedente/conflito | parser negativo cap1/cap0/same-port e collectioncap2 offline; bridge mantém coleção e wrappercap1 |
| Policy inexistente/divergente | fixtures no bridge e validação canônica efetiva |
| Stale revision/Stop/retenção | comparação e guards em fonte; native teste sem root CURRENT cobre negação inicial, fases posteriores parciais |
| Candidato sem CURRENT | native root identity guard efetivamente rejeita |
| Quiescência/credits/leases | código não adquire/libera recursos; não qualifica teardown de produtos |

Sem teste de rede, issuer, certificado ou codec novo. Builds1 falharam C4005PC_NONE com SDK; correção aprovada de namespace. Build2 falhou C4335 por CR duplicado na preparação da correção; nova operação byte-exata aprovada preservou line endings originais. Builds3 passaram; suplemento final exigiu ALPN configurado papacc/1 e presença explícita de I, sem aumentar layout. Builds4 finais Debug/Release zero warnings/linkage completo, logs anteriores intactos. Não suprimiu warning nem alterou toolchain.

Logs finais: Debug/Release-build4.log e Debug/Release-tests2.log. Suplemento ALPN/presença também foi oficialmente aprovado; comparação da configuração TLS é estática do material já preparado, não negociação TLS executada.
