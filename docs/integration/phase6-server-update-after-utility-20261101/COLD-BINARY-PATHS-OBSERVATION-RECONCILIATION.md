# ProgramFiles-only — sem redução de handles observada

Estado BINARY_PATHS_OBSERVATION_VERIFIED_CLOSED_NO_HANDLE_REDUCTION.
Pedido3a8611...74beb/autorizaçãof20229...f717 UTILIZADOS, sem retry. Pendências
anteriores dos registros são históricas; não modificar seus bytes pinados.

Fonte: saída emitida colada pelo proprietário, preservada como excerto em
OWNER-COLD-BINARY-PATHS-EMITTED-OUTPUT.txt, SHA256
2ec444701b09f974dccebc0ff3217e47f739e4d4eaa75ef23c75aa0a54e5be59.
Dados COLD-BINARY-PATHS-OBSERVATION-RESULT.json.

| Corte | Anterior full-layout | ProgramFiles-only | Delta novo por fase |
| --- | ---: | ---: | ---: |
| Entrada pós-CRT |112|112|não aplicável|
| Após owner |112|112|0|
| Após console |112|112|0|
| Após token |113|113|+1|
| Após resolver/drive |136|136|+23|

Todos known1/error0, first0/close_known1/sample_bytes60; wrapper exit0,
INVOKEDTrue/WRAPPER_CLOSE_KNOWNTrue. Coleta concluída segundo o caminho pinado,
não provisão de capacidade, autenticação SCQ ou quiescência global OS.

O artefato566ee...8e4f selecionado e procedimento pinado exercitam o resolver
binary-only; código retorna após ProgramFiles antes de consultar ProgramData.
Redução de chamadas2→1 permanece comprovada offline. Redução de contagem final
observada entre as duas execuções: ZERO. Logo excluir ProgramData isoladamente
não resolveu a fronteira136 nesta prova e não justifica UPDATE ou novo Start
como solução para esse bloqueio. A correção de escopo continua válida, mas seu
benefício de handles não deve ser anunciado.

Não concluir que ProgramData nunca utilize handles ou que23 pertençam todos
a ProgramFiles: são duas execuções distintas. O último grupo ainda contém
ProgramFiles/SHGetKnownFolderPath, operações de memória/strings/allocator,
GetDriveTypeW e query final. Variações assíncronas, estado do ambiente, loader/
CRT e custo do observador não foram eliminados por ensaio causal controlado.
Identidade dos handles/lifetime interno e separação dessas APIs continuam UNKNOWN.
Caches não demonstrados não são vazamento provado ou objetos fecháveis pelo app.

Guard124/cap128 inalterados.136 nega novas referências SCQ; token original
obrigatório não pode ser fechado para alterar contagem. Nenhum fechamento seguro
novo foi demonstrado. Não fechar internos do shell/allocator/kernel, remover
Stop, descontar herdados ou adiar custos reais além de uma guarda para passar.

Primeira lacuna restante: existe fonte de resolução do alvo igualmente legítima,
congelável e revalidável que evite essa inicialização opaca, ou o percurso exige
de fato a API atual? Examinar offline os contratos de knownfolder/imagem/SCM e
primitivas existentes antes de propor outro instrumento. Caminho fixo, getenv,
imagem/SCM autodeclarados ou cache por conveniência não substituem prova original.
Se alternativa exigir especialização, identificar a cláusula e apresentar somente
a decisão focal necessária. Não alterar norma ou quota por esse resultado.

Não executar outra coleta/tracing como reação automática, não repetir a coleta
consumida nem instalar o candidato supondo PASS. Se uma separação factual entre
ProgramFiles e drive continuar indispensável, justificar sua suficiência e custo
antes de pedido específico, sem campanha genérica ou infraestrutura adicional.
Não há novo pedido operacional de Start pronto; admissão permanece DENY.

Nenhuma operação adicional foi executada pelo agente nesta reconciliação.
Canônico instalado/EXEs retidos/documentos/HEAD/WIP/pins/reservas preservados;
retenção histórica e OS_QUIESCENCE UNKNOWN. História1816 anterior permanece
delimitada. E21 PENDING/ED11 Phase6 PARTIAL/readiness produtiva DENY/PST0.6.4 intacto.
