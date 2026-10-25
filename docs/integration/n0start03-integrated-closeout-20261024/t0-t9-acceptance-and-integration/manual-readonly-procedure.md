# Procedimento candidato â€” nÃ£o executar antes da aprovaÃ§Ã£o especÃ­fica

Pedido exato: `67c312608207eaf4e7dcff863a8c1253d4558b9c8e4d2e7d54dc5a416e90db16`.

A aceitaÃ§Ã£o T0â€“T9 autoriza a implementaÃ§Ã£o, nÃ£o esta observaÃ§Ã£o. EXACT-TRANSPORT-READONLY-APPROVED.json **nÃ£o existe**. Nenhuma aprovaÃ§Ã£o histÃ³rica pode ocupar esse endereÃ§o.

1. Owner revisa CLOSEOUT.md, os quatro hashes e o pedido DRAFT e aprova explicitamente seu SHA256. O texto em OWNER-APPROVAL-CANDIDATE.txt Ã© apenas candidato.
2. Somente depois: registrar a declaraÃ§Ã£o real em arquivo novo no endereÃ§o designado, referenciando este pedido e seus artefatos; conferir bytes/hash/tamanho (cap nativo16384). Calcular o hash literal completo desse registro e fornecer o comando final. NÃ£o editar o pedido, o launcher ou o qualificador apÃ³s aprovaÃ§Ã£o sem nova revisÃ£o.
3. A execuÃ§Ã£o serÃ¡ manual, uma vez, em um Windows PowerShell5.1 x64 jÃ¡ elevado. NÃ£o abrir outro PowerShell operacional. O launcher opt-in N0START03ReadOnly continua default EMPTY_DENY e Cycle DENY. NÃ£o hÃ¡ comando executÃ¡vel com hash de aprovaÃ§Ã£o fictÃ­cio neste pacote.
4. Conferir operador/token/host/boot, pins e console; ler o plano exibido. Primeira confirmaÃ§Ã£o: PREPARE_N0_READ_ONLY + tentativa fixa + PID/criaÃ§Ã£o do auxiliar factualmente escolhido. Segunda: BIND_BD_READ_ONLY + tentativa + identidade original/desafio nativo exibidos. NÃ£o copiar valores histÃ³ricos nem automatizar teclado.
5. Observar IG_HT checkpoints0/1/2, IG_PROCESS, IG_OBSERVER_WINDOW, IG_HOST, IG_FINAL e retorno; Escape solicita Stop governado, F10 somente apÃ³s recibo final e tÃ©rmino factual. Distinguir final close de quiescÃªncia global. Guardar o resultado completo; nÃ£o repetir em falha/timeout/UNKNOWN.

Efeitos preparatÃ³rios futuros autorizÃ¡veis: endpoint local com DACL explÃ­cita desde a criaÃ§Ã£o, evento de I/O e filho original sem heranÃ§a; DLL e binding no mesmo supervisor. Nenhum novo serviÃ§o/owner/autoridade; nÃ£o criar intent/arquivos protegidos/imagem/sink. Os custos dos mÃ³dulos, buffers e endpoint serÃ£o medidos e cobrados nos processos prÃ³prios; nenhuma economia de fixture concede orÃ§amento.

O prÃ³ximo resultado poderÃ¡ demonstrar apenas a composiÃ§Ã£o read-only observada. Capacidade e efeitos positivos exigem gates e autorizaÃ§Ã£o separados.

Preparação do operador: os guards existentes exigem ausência dos tipos/helpers de uma execução anterior e ausência da nova DLL previamente carregada. Não contornar esses guards nem encerrar processos históricos para fabricar headroom; reconciliar lifetimes, quatro papéis e orçamento no alcance exigido. Não iniciar um segundo PowerShell pelo launcher.
