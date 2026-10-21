# Revisão exata anterior à submissão

Operações em operations.json, preimagens e diffs0–6. Escopo: storage de valor, não autoridade.

0/1: interface_snapshot_copy.h/.c — cópia bounded32interfaces/128endereços, sem presentation pointer, validação anterior à escrita e present por último; arrays embutidos, invocation/revisão de preparação reais fornecidas pelo owner. Não é novo catálogo/registry, não há heap/pins/threads, CURRENT ou epoch. View síncrona não autentica origem; é contrato de empréstimo do mesmo owner.

2: CMake adiciona unidade portável ao papacc_network existente. Nenhum target/backend novo.

3: owner existente embute copy no participante. sizeof(root) cobra storage automaticamente; não duplicar censo de arrays embutidos.

4: prepare_listener copia input exclusivo imutável durante preparação, resolve somente da cópia, guarda invocation e revisão do modelo existente (zero permanece pending); revalida candidato/Stop antes de publicar apenas o participante. Proíbe substituição existente e exige reserva para next+4096 de scratch, sem consulta nativa nova. View local da cópia next não escapa, destino não contém self pointers.

5: preparação retida marca snapshot retained antes de reter source, inclusive se retenção source falhar. Rollback existente só zera participante após provas zero-child; falhas anteriores preservam storage. Não introduzir cleanup nem liberação nativa. Não modificar CURRENT/epoch/pins/grants/admissão.

6: teste existente de discovery adiciona fixtures de lifetime e copy de snapshot Windows real. Invocation/revision de teste são explícitas fixtures; não prova de CURRENT. Snapshot de SO, IDs e endereços nativos não são fabricados nem alterados. Não executar transação ou rede.

Contrato de origem e freshness não é inventado: receber snapshot no prepare_listener não comprova que ele veio de enumeração nativa; a cópia estabelece ownership mas não autentica o produtor nem topologia atual. A entrada SELECTED nativa continua DENY enquanto faltarem essas provas e binding de revisão pós-commit. Não substituir pin CURRENT por flags deste storage. Nenhuma publicação de configuração é executada na tarefa.
