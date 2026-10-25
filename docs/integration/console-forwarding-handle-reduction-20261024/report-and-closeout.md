# Console forwarding: qualificação focal

A substituição única de `Write-Host $line` por `[Console]::Out.WriteLine($line)` foi aplicada no loop de stdout nativo. A comparação binária inversa confirma que todos os demais bytes do launcher, incluindo prompts, Stop, retenção, guardas e fechamento, permanecem iguais à preimagem. Não houve alteração nativa ou de contratos accepted.

## Evidência e limites

Windows PowerShell 5.1.19041.6456, Windows 10: onze casos de I/O do loop extraído e cinco estados do predicado de retenção passaram. Incluem saída extensa, ordem, linhas vazias, Unicode em TextWriter, truncamento com drenagem, recibos finais, UNKNOWN, Stop anterior ao efeito e erros de escrita/disposição propagados. A retenção foi verificada como predicado e estrutura; o ciclo operacional não foi executado.

Os fixtures redirecionado, console nativo e host personalizado passaram dentro de suas asserções documentadas. Em console nativo CP850, ASCII e o protocolo UTF16 hexadecimal permanecem exatos; Unicode arbitrário fora da página de códigos perde caracteres na saída direta. Write-Host preservou esses caracteres. UTF8 configurado somente no fixture preservou Unicode; nenhuma alteração de encoding foi aplicada ao produto.

A saída direta não aparece em Start-Transcript nem no Information Stream e contorna a UI do host personalizado. A documentação vigente lida não exige esses canais como registro persistente do stdout encaminhado; o sink e os recibos existentes continuam obrigatórios. Portanto esta qualificação cobre o protocolo atual ASCII/hexadecimal em console nativo, sem afirmar equivalência geral entre os métodos. Se outro consumidor exigir Unicode arbitrário, transcript ou UI personalizada, esta troca isolada não atende esse requisito e deve permanecer bloqueada para esse uso.

FreeConsole provocou erro de escrita, preservado como falha. Esse teste não representa CTRL_CLOSE_EVENT nem prova cleanup após fechamento abrupto. Não houve confirmação operacional ou teste de StopService.

Review com pin atual passou; pins antigo e inválido foram negados. O primeiro harness confundiu exit de powershell -File com LASTEXITCODE do launcher; os logs já registravam DENY. O harness corrigido propaga explicitamente LASTEXITCODE. Duas falhas anteriores de sintaxe/tipo do fixture estão preservadas em semantic-initial e semantic-second; não foram tentativas LAB.

A redução de handles permanece FIXTURE_OBSERVED_PRODUCT_UNQUALIFIED. A evidência histórica +8 PowerShell/+2 auxiliar e 1022→1032 não permite prometer redução exata no LAB. Não foram necessárias ferramentas adicionais.

## Pins e próxima operação

Launcher SHA256: `deb0ab304110e5675057261df09e2057199db11884be5718c77a86ca1d382cec`.

Pins históricos e aprovação consumida permanecem preservados. O launcher ainda contém a identidade histórica N0START-20261024-02: não executar Prepare/Cycle com esse pacote. Uma próxima operação exige decisão própria sobre identidade, reconciliação de efeitos e aprovação exata; nenhuma tentativa nova foi criada. Nenhum comando operacional é autorizado por este relatório.

BUILDS=NOT_REQUIRED_NO_NATIVE_SOURCE_CHANGE; WARNINGS=NO_NATIVE_BUILD_RUN; OFFLINE_TESTS=PASS_WITH_DOCUMENTED_SEMANTIC_LIMITS; NATIVE_TESTS=ISOLATED_CONSOLE_FIXTURES_ONLY; NETWORK_TESTS=NOT_RUN.

LAB_ATTEMPT_REPLAY=NO; START_EXECUTED=NO; SCM_MUTATIONS=NONE; ACL_MUTATIONS=NONE; ED11_TOTAL_STATUS=PARTIAL; CURRENT_PUBLICATION=DENY; SIGNATURE_AUTHORITY=DENY; AUTHORITATIVE_RECOVERY=DENY.

Referência Microsoft: [Write-Host Windows PowerShell 5.1](https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.utility/write-host?view=powershell-5.1). Os resultados locais qualificam as diferenças efetivamente observadas neste host.
