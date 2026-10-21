# Revisão E0–E8 — STOP de aceitação, sem alteração funcional

TASK=PACC-PHASE6-FACTUAL-EVIDENCE-E0-E8-ACCEPTANCE-1. Resultado: E0–E8 permanece PROPOSED. Condição de conformidade integral não comprovada por duas lacunas materiais de lifecycle. Nenhum registro ACCEPTED foi preparado/gravado ou submetido como conforme. Não é negativa do executor nem conflito entre duas autoridades accepted; é incompletude da proposta frente às obrigações accepted F4/Model E.

## R1 — aquisição retida sem caminho contratual definido

Fonte exata SHA256742088da6c361c277d423dbcb5cc3bc67b8bc638760d0f420a2242ccc416d937. E1 linha23 inclui SP_TRUST_RETAINED na enumeração retornada por acquire. E6 linha63: “Output de acquire permanece intacto em erro; não publicar empréstimo parcial.” E1 linha34 limita a consulta de liberação a um SP_TRUST_LEASE já conhecido.

A proposta não determina se acquire pode retornar RETAINED. Se puder após aquisição parcial/incerta, o output pode não fornecer lease consultável e não há outra operação de consulta da aquisição especificada. Tampouco identifica contratualmente um estado preexistente do owner que torne esse resultado alcançável sem o lease. Se RETAINED for proibido nessa função, isso precisa ser explícito. Não presumir que a implementação futura resolve a ambiguidade por conta própria.

F4 exige manter owner/dependências/créditos alcançáveis até quiescência e tratar falha de aquisição sem evidência parcial. Model E distingue aceitação de ownership do resultado da operação. Não afirmamos vazamento factual no produto: a API não foi implementada. A ausência da disposição normativa é suficiente para STOP de aceitação integral.

## R2 — resultado INVALID de liberação sem disposição

E1 linhas29–34 declara SP_TRUST_RELEASE_INVALID para release/release_query. E7 linha71 define somente RELEASED e RELEASE_RETAINED e proíbe repetir release, sem definir se INVALID ocorreu antes de tentativa, preserva handle ou consome a única tentativa.

Sem essa distinção o consumidor não pode decidir, apenas com o contrato, entre preservar referências, corrigir precondição antes da primeira tentativa, consultar resultado ou invalidar alias. Inferir INVALID=liberado ou repetir close seria incompatível com F4/D3.9/Model E. Também aqui é lacuna contratual, não alegação de falha na máquina M1 já implementada.

## Menor correção candidata

Patch PROPOSED anexo muda SOMENTE E1/E6/E7 da proposta: acquire limitado a ligação síncrona all-or-nothing sobre referências previamente adquiridas/protegidas pelo owner, nunca RETAINED; revalidate retém apenas lease já conhecido; INVALID de release/query é precondição sem tentativa, conserva estado/ownership, sem refund/limpeza/retry destrutivo. Não exige novo objeto de aquisição, registry, worker ou sistema de pins.

Essa restrição adicional NÃO foi escolhida pelo owner nesta tarefa e NÃO é accepted. É alternativa mínima para revisão explícita. Se for indispensável que acquire inicie aquisições incertas, esta alternativa não serve: deve-se propor um contexto de aquisição sob o owner existente com consulta/ownership exatos e censo próprio antes de aceitar. Não implementar nenhuma das alternativas aqui.

Não aplicar o patch à proposta original. Não alterar S0–S8, M0–M8, D3, F1–F5/G0–G7 ou provenancev2. Não criar ADR ACCEPTED, não aceitar parcialmente E0–E8 por inferência. STOP atinge a aceitação, preservando a máquina M1 offline e todos os componentes independentes.

## Compatibilidade e recursos

Tipos declarados usam C89, enum, ponteiros para incompletos e const; compatibilidade de declarações por inspeção não é build/teste desta API ou qualificação VC6/NT4. Sem ABI pública/layout persistente definidos por E1; ReviewRevision1 não versiona wire. Tamanhos concretos dos opacos, bounds do material referenciado, alinhamento/frames e reserva de evidências continuam gates da implementação; não inferir bytes reservados nem exigir Browser entender codecs da root.

Censo anterior: controle48x64/36x86, operação792x64, input128 e PDG72; controles já incluídos na operação. Cenário conservador nomeado3626009 excede3MiB por480281, não é pico operacional medido. Novos opacos/captura/source leases/stack/retidos ainda UNKNOWN; nenhum censo novo, quota ou reserva foi aprovado. ED11PARTIAL/ativaçãoDENY. A alternativa borrow-only não comprova economia nem custo zero: metadados/backing, pins, snapshots e retenção do owner continuam contabilizados.

Browser provenancev2 preserva duas observações/contexto da operação, confirmação local separada, ledger histórico não autorizador e ausência de fallback. E0/E2–E5/E8 respeitam esses limites semanticamente; não houve migração/instalação ou codec alterado. A afirmação de mesmo lifetime/owner em E6 também recebe esclarecimento na candidata: mesma operação com lifetimes protegidos pelos owners reais, sem fundir root e destinatário.
