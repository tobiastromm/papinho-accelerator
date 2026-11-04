# C8 — descoberta administrativa por metadados

Estado: root operacional NOT_DEMONSTRATED; binding ownerAuth DENY; E21 PENDING; ED11/Phase 6 PARTIAL.

Ref: main f341985924d29870d8ff627ed37c8a0874ad6469. PROJECT-CONTEXT.md não existe. A investigação anterior permanece em OWNERAUTH-ROOT-DISCOVERY-20261028.md.

## Consulta preparada, não executada

tools/qualification/c8-root-metadata.MANUAL.ps1 destina-se a uma única invocação manual em Windows PowerShell 5.1 x64 já elevado. Não eleva, não altera ACL nem lê conteúdo de arquivos, chaves, credenciais, registry ou memória de processos. Lista somente nomes/tipos e owner ACL dos candidatos pelo nome, sob C:\ProgramData\PapinhoAccelerator, até três níveis de diretórios enumerados e 256 entradas. Não segue reparse points. Qualquer erro interrompe a consulta, preservando resultado parcial. Limite de profundidade é explicitado e não autoriza conclusão de ausência global. Não há escrita de recibo ou marcador operacional.

A checagem de hash do script precede a enumeração; não constitui pin de handle original ou defesa contra administrador concorrente. Metadados de caminho/ACL são sujeitos a mudança e não são identidade protegida RAW1. Não usar este coletor como admission, autoridade de publicação ou leitura Q4.

## Critérios de classificação

- Nome de root/manifest/CURRENT/epoch: candidato somente.
- Root em build/testes: qualificação ou fixture conforme sua proveniência, nunca autoridade implícita.
- Diretório protegido existente: existência sem publicação comprovada.
- Erro/permissão/profundidade/escopo incompleto: UNKNOWN.
- Root legítima: exige posteriormente manifesto autorizado, objeto original e proveniência verificáveis, estado CURRENT/epoch e correlação ao owner/root canônico. Esta consulta não lê nem comprova esses dados.

O código server_configuration_commit_win32.c, na recuperação, declara que seleção factual sem leases reais/current-policy/capability não publica autoridade e retorna NOT_SUPPORTED. Nenhum leitor externo de metadados pode produzir essas referências.

## Próxima sequência

Receber o JSON manual. Se houver candidato, preparar verificação específica do manifesto não secreto e da autoridade, sem abrir conteúdo protegido por inferência. Reconciliação RAW1/identidade original exige procedimento próprio, não igualdade de hash. Se não aparecer publicação comprovada, manter UNKNOWN no âmbito não observado e preparar proposta de primeira publicação pessoal/local.

Proposta de bootstrap condicional: designação humana do owner e destino novo; revisão dos contratos de primeira instalação, source/custódia e política; definição de manifesto de publicação e identidades originais; exclusividade e consumo governados; CURRENT/epoch emitidos somente após commit autorizado; falhas retêm UNKNOWN sem limpeza/retry. Não é ADR accepted, autorização, comando executável ou proposta de promover fixture. Não escolher destino fictício enquanto a descoberta não retornar.

## Frentes independentes

E17 exige prova operacional de identidade/acesso/readiness; E18 lifecycle/Stop/quiescência. E19 ainda depende de D4/D5 quantitativos justificados e custos ativos provider/kernel. E20 depende de bindings reais root/CURRENT/epoch/SiteTLS/backend/destino e composição correlacionada. Nenhuma dessas lacunas é satisfeita por mais uma consulta STORAGE_20 ou pelo locator. E22 permanece derivado pendente. Nenhum build repetido: não houve alteração funcional de produto.

Sem acesso ownerAuth, NV, Q4, Start/Stop, SCM/ACL, limpeza, refund, CURRENT, assinatura ou recuperação. Propostas e aprovações históricas preservadas.
