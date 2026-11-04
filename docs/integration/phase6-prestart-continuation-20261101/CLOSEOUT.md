# Continuação pré-Start QUERY/G3/SQB2 — fechamento offline

Data: 2026-11-01. Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469; WIP preservado.

## Decisão e implementação

O proprietário confirmou explicitamente o hash real terminado em f68:
9637246ad9ad6de34ff9eb5cb9440e628a14909ea7f95cb6a2ba2b7265287f68.
O pedido original foi conferido e preservado. A decisão foi registrada antes
do código em [accepted](../../adr/DECISION-personal-scq-prestart-continuation-binding-20261101.md).
A divergência intermediária de transcrição está resolvida, sem nova autoridade operacional.

Em `server_service_startup_win32.inc`, a inicialização permanece
`initial_receiver_source_unavailable=1`. Somente os getters canônicos de source
original e recursos G3 revalidados reconhecem a continuação compilada. O bit
DELIVERY_CONTINUATION só é removido com namespace original revalidado. Os
callers reutilizam o owner/controlador, três atos independentes e o receptor
QUERY/SQB2 já existentes. Nenhuma flag de teste foi instalada no produto.

Sem source, atos, identidade/proteção, G3 ou namespace, permanece DENY. Não se
exige PID/token futuro antes do Start; autenticação do runtime, receipt/close e
readiness continuam obrigações posteriores. `resource_pre_missing=63` e
`later_obligations=15` permanecem visíveis: elegibilidade limitada não é prova
de provisão integral nem readiness produtiva. Prazos não são renovados.

## Qualificação e fronteira da evidência

Selected/Legacy Debug/Release: quatro builds PASS, zero warnings; 28 CTests
por configuração, 112 pares PASS. Logs finais e detalhes estão nesta pasta.
O teste QUERY usa o relatório real com bridges exclusivamente de teste que
invocam os atores reais de source/G3/namespace sobre fronteiras nativas modeladas.
São 34 cenários e 1.755 verificações por configuração. Incluem source/ato
ausente, política divergente, namespace ausente, prazo vencido, replay,
substituição, Stop, perda do peer, receipt e close incertos.

Seis caminhos alcançam o predicado de readiness antes das falhas injetadas;
cinco terminam com fechamento conhecido e um preserva fechamento incerto.
Isso não demonstra autenticação bilateral Windows nem RUNNING nativo. RNG,
KSP, SCM, endpoint e operações protegidas não foram executados. Nenhuma
campanha operacional consumida foi repetida. OS_QUIESCENCE permanece UNKNOWN.

## Imagem e primeiro bloqueio concreto

Selected/Release no workspace: 710.656 bytes, SHA256
08ac546a7e9653effe9d438706b3cb82aed39e9e8bbe7b6151c2f6e840bfc043.
EXE staged histórico: 571.392 bytes, SHA256
c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf.
Delta: +139.264 bytes; não mede custo nativo. DLLs de workspace mantêm os pins
históricos: ssl 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed;
crypto 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.
Não houve leitura operacional nova do serviço nem atualização staged.

A cadeia fria e sua ligação pré-Start estão qualificadas em modelo. O primeiro
bloqueio passa a ser a primitiva/admissão de UPDATE da imagem protegida existente.
O stager usa FILE_CREATE exclusivo ou FILE_OPEN de leitura: não atualiza o EXE
existente. O UPDATE de configuração v3 opera packets PC_ROOT8, não substituição
de binários. Uma primitiva binária aplicável não está demonstrada no caminho
canônico inspecionado. Não se presume sua inexistência global nem se autoriza
Copy-Item/rename/delete como substituto.

O [procedimento anterior](../original-first-delivery-20261031/STAGED-IMAGE-UPDATE-PROCEDURE.CANDIDATE.md)
continua histórico, NÃO EXECUTÁVEL e com pin anterior. O artefato acima é candidato
técnico, não seleção operacional. É necessário qualificar UPDATE no mesmo owner,
originals, proteção/ancestrais, exclusividade, recursos, consumo, readback e
retenção parcial antes de apresentar pedido byte-exato de atualização.

Depois: autorização própria de update, execução manual e reconciliação; somente
então pedido separado de endpoint/Start diagnóstico com gates frescos de serviço,
imagem, controlador e G3. Nenhum desses efeitos é autorizado aqui. CA/CURRENT e
readiness produtiva continuam separados. Pedido operacional NOT_READY; autoridade
DENY; E21 PENDING; ED11/Phase 6 PARTIAL. PST 0.6.4, pins, reservas, chave fixture,
históricos e UNKNOWN preservados.

[Próximo prompt](NEXT-GOVERNED-STAGED-BINARY-UPDATE.md).
