# Submissão exata — associação CURRENT e comparação de observações

Oito operações em operations.json/diffs0–7, com preimagens SHA256. Nenhum ADR alterado. Estado da captura imutável preservado; metadados são correlação, nunca autoridade de topologia.

0/1 interface_snapshot_copy.h/.c: comparação portátil por PersistentID selecionado e endpoint sets IP/scope/estado, sem usar nome/índice/instanceID como identidade. Resolvers existentes, scratch bounded8targets por interface/8IDs; excesso nega. Mesmo resultado MATCH não prova recency/origem nativa/continuidade ou CURRENT. Sem alocação, consultas SO, callback ou novo catálogo.

2 owner.h: metadados embutidos no mesmo participante para domínio/invocation/revisão preparada/R/A/P; report somente leitura. Não cria ID/epoch, pin, registry ou snapshot. A cópia continua5928bytes, metadados cobrados separadamente por sizeof(root).

3 root transaction include: hook executa SOMENTE após commit e publish existentes retornarem sucesso; exige revisão predecessora igual à captura, modelo idêntico e estados normativos válidos; copia R/A/P reais, não os emite. Qualquer falta deixa bound falso. Query exige owner/thread/pin CURRENT factuais, igualdade exata e copy não retida; report.topology_proven e resolved permanecem sempre falsos. Não altera resultado/ordem/CAS da publicação existente nem novos grants. Nenhuma publicação será executada nesta tarefa, respeitando decisão accepted I4 e CONFIG_PUBLICATION=DENY.

4 owner retain: invalida apenas a correlação CURRENT antes de reter source, preservando bytes e créditos. Rollback/reaper/limpeza não alterados.

5 native discovery test: fixtures iguais/diferentes/removidas/IDobsoleto/renumeração; duas enumerações reais comparadas como observações, sem prova de intervalo/freshness. Sem mudança de interface SO.

6/7 tests: caller com flag bound falsa autoridade continua rejeitado pelo owner real; query sobre root real ainda sem commit retorna negação. Modos snapshot continuam retornando antes de commit/publish. Resultado positivo CURRENT sob nova fonte não executado/qualificado nesta tarefa.

STOP da integração RESOLVED: backend possui GetAdaptersAddresses, mas não revision/lease/notificação de invalidação de topologia. Não transformar capturas iguais em prova contínua nem executar query potencialmente bloqueante no owner. Não criar mecanismo alternativo para burlar essa lacuna. Provider nomeado permanece PENDING; admissão e rede negadas.
