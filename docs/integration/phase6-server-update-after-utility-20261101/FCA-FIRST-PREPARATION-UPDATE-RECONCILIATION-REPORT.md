# Reconciliação do UPDATE FCA — 2026-11-03

Resultado: **LIMITED_UPDATE_VERIFIED_CLOSED** na saída original do executor. Primeira falha 0, autorização consumida, criação exclusiva confirmada, máscara de renames 3 (dois renames), readback canônico e fechamento conhecidos. O EXE anterior está preservado na folha `.previous` nomeada no resultado JSON. Nenhuma operação foi repetida.

Novo canônico: 905728 bytes; identidade `417614100:2555904:1047601`; SHA256 `1ee37a465e6820a5d5967e25630673aadb81d1972a734fb9bf4046ef3b19b2e6`. DLLs e configuração SCM constam do plano original e são gates do executor existente. Esta reconciliação não representa uma nova consulta ou lease atual do estado SCM/filesystem.

Saída preservada byte-exata em `OWNER-FCA-FIRST-PREPARATION-UPDATE-OUTPUT.txt`, SHA256 `32069e22171a07076b95727ee81ea05245e4e1d5e155b0f781c5b4adb6851746`. Pedido/decisão/procedimento conferidos contra seus pins antes do registro. As 228 amostras foram processadas integralmente: sequência contínua, known=1, QPC ordenado e frequência positiva; handles observados 169–171 no executor UPDATE, não baseline ou capacidade FCA. `missing=2` corresponde à preservação dos custos nativos opacos UNKNOWN. Exit 0 não foi usado isoladamente como prova.

A autorização está consumida em `FCA-FIRST-PREPARATION-UPDATE-AUTHORIZATION-STATE.json`. O manifesto offline anterior permanece histórico, inclusive sua referência à antiga imagem instalada. Nenhum código mudou e nenhuma campanha inalterada foi repetida.

## Primeiro gate restante

Preparar o ato FCA próprio: localizar evidência original do parent de dados esperado pelo caller NEW_INSTALL e da configuração mínima protegida. A identidade do parent ProgramData/Papinho, proteção dos ancestrais e existência/disposição do destino ainda não estão demonstradas neste pacote. Isso é falta de evidência, não afirmação de ausência ou ACL inadequada. O source deve ser regular/link único, no mesmo volume, protegido e retido junto à cadeia total de até oito diretórios.

O UPDATE não autorizou materialização do source, contêiner ou chave. Não há comando de criação pronto que possa ser emitido honestamente antes de fechar esses bindings e delimitar os efeitos. Próximo trabalho: `NEXT-FCA-DATA-PARENT-AND-CONFIG-ORIGINAL-PREPARATION.md`.

P4/K5, retenção histórica e quiescência OS continuam UNKNOWN/não qualificados. Readiness produtiva DENY. Não repetir READ, fixture, UPDATE ou sessão consumida. PST 0.6.4 preservado.
