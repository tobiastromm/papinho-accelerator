# ED11 — composição e coexistência

Selected x64 medido pelo compilador: owner COMMIT242136, root319192; backing38872 por slot,116616 nos3 slots. A composição fixa conhecida cresce1827064→1943680. Legacy permanece1790872, arrays e quotas individuais intactos. O cenário conservador anterior2103847 cresce2220463; a diferença até3145728 é925265 ARITMÉTICOS, não crédito ou reserva operacional. Default2097152 não mudou.

packet30592 já inclui SNT/PDG/issuer/BODY/LST1/CP3; não somar esses componentes novamente. CA8192 é cópia física separada por slot e está cobrada. Metadata88/slot inclui dois hashes, dois tamanhos e o serial existente correlacionado. Os snapshots issuer históricos e buffers committed/previous/next permanecem cobrados no antes; não foram eliminados ou compartilhados. Todos os3 backings são simultâneos, inclusive pinned/retained e CA por geração distinta.

Caller mantém imagem/CA imutáveis e arena durante captura. O teste usa arena32768; capacidade real do caller deve ser cobrada. Decode CA e encode SPKI terminam antes do mesmo arena receber BODY decode; nenhum ponteiro decode escapa. Isso prova somente essa reutilização local síncrona, não exclusividade entre operações ou Sessions. Durante cópia coexistem inputs e novos arrays por valor. Hashes não apagam o predecessor/committed.

Cenário nomeado se inputs independentes e arena32768:2220463+30592+8192+32768+24+88=2292127, NÃO pico medido. O cenário anterior futuro v22142951+116616=2259567 continua hipotético. Crescer3 arrays worker para v3 exigiria ainda4896; não aplicado. O cenário conservador de pacote3626009+116616=3742625 ultrapassa3MiB em596897: ativação dependente DENY; não é medição de pico nem reserva aprovada.

Disassembly de capture/borrow/CA helper registra sub rsp principal Debug512/264/128 e Release400/136/72, ALÉM de pushes, temporários e chamadas aninhadas. Não representa stack transitiva completa. O projeto mantém reservas históricas de stack, mas esta entrega NÃO afirma suficiência delas para ativação. A implementação não cria heap explícito/worker/handle CA retido; CryptoAPI/BCrypt/CRT/provider/kernel e todo custo opaco permanecem UNKNOWN, não zero. Nada foi movido para stack/heap para escapar do censo.

O harness reutiliza globals dos testes v2/v3, owner de modelo, inputs e arena; esses custos de fixture não foram convertidos em economia/reserva do produto. Fonte/graph/tickets não são adquiridos por callbacks de trust. Sem máximo operacional completo e crédito factual, nenhum caller operacional é admitido. ED11 total PARTIAL, quotas e admissão preservadas.
