# Coleta corretiva binária — autorização registrada e handoff manual

Task PACC-PHASE6-BINARY-ONLY-OBSERVATION-AUTHORIZATION. O proprietário aprovou
explicitamente uma única coleta manual conforme pedido integral SHA256
`3acf886784123d2f3523736077bcf03b728c56fc590c87edc0aea80d6ad158cf`.

Registro independente: OWNER-BINARY-ONLY-AUTHORIZATION.json, SHA256 literal
`e001fa0cf42d91b917c6b18f3168cff85ab4e077b0a4f822d64ce9032f0114cc`.
Pedido, script e três artefatos conferidos e preservados. Registro documental,
não grant de runtime ou prova de consumo entre invocações. Aprovações anteriores
consumidas não são reutilizadas; execução/consumo/evidência desta coleta ficam
pendentes da saída manual. Nenhuma invocação pelo agente.

O escopo aprovado inclui a exceção Process/BYPASS/restauração descrita no pedido,
sem mudar CurrentUser/LocalMachine/GPO ou elevar automaticamente. Nenhuma nova
aprovação normativa, assinatura ou autoridade produtiva resulta deste registro.
Gates frescos de contexto/identidade/objetos/recursos serão conferidos na execução;
não estão demonstrados por hashes nem preenchidos com identidades históricas.

## Comando manual definitivo

Em Windows PowerShell5.1 Desktop x64 já elevado, colar o bloco inteiro UMA vez.
Se já iniciou esta coleta corretiva binária, não executar novamente. Este é o
script novo; não usar os comandos antigos. Verificação de hash antecede chamada,
e o script reconfere seu hash e o pacote antes da única chamada nativa.

```powershell
& {
    $stagingCollector = 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-staging-layout-result-20261031\collect-staging-binary-layout.MANUAL.ps1'
    $stagingScriptSha256 = 'd1f72e217e90ee167267aada614c0bd1b04b12fce2ddc1de1c24cec12ce23c5f'
    if ((Get-FileHash -LiteralPath $stagingCollector -Algorithm SHA256 -ErrorAction Stop).Hash -ne $stagingScriptSha256) { throw 'SCRIPT_HASH_DENY' }
    $stagingPreviousPolicy = Get-ExecutionPolicy -Scope Process
    try {
        Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force -ErrorAction Stop
        if ((Get-ExecutionPolicy) -ne 'Bypass') { throw 'EXECUTION_POLICY_DENY' }
        & $stagingCollector -ExpectedScriptSha256 $stagingScriptSha256
    } finally {
        Set-ExecutionPolicy -Scope Process -ExecutionPolicy $stagingPreviousPolicy -Force -ErrorAction Stop
    }
}
```

Sem retries/loops, processos auxiliares ou ajuste permanente de política. Se a
política efetiva impedir, STOP e enviar o erro; não procurar outro meio de executar.
Os comentários PROPOSTO no pedido/script são histórico byte-exato preservado;
a aprovação posterior está neste registro separado, sem alterar seus hashes.

## Reconciliação esperada

Enviar a saída integral. Conferir wrapper/pins, operador/identidade quando emitidos,
post-entry, phase7 do layout BINARY_ONLY, demais frames alcançados, guard/query
por objeto, memória/commit/físico/quota de volume, primeira localização/erro e
close controlado/retidos. Contagem UNKNOWN não é zero; pico amostrado não é teto.
Exit0 é somente observação, não admissão. O lookup obrigatório de ProgramFiles
pode continuar insuficiente; não foi afirmada redução real de handles.

Falha, Stop, interrupção, pending, perda de continuidade ou close incerto não
permite repetição. Ausência de receipt não prova que uma tentativa não ocorreu.
Registrar o uso futuramente em evidência derivada separada, vinculada ao hash
aprovado, sem rearmar ou modificar o registro literal.

A coleta não demonstra custos ativos de criação/escrita/flush/metadados de FS,
reserva/margem futura ou quiescência interna do OS. ED11 incremental-envelope e
perfil Windows3MiB W4/W5 mantêm custos materiais UNKNOWN em DENY. Sem a saída
não se determina admissão ou emite pedido de staging como pronto. Se necessária
evidência adicional, sua obrigatoriedade deverá ser demonstrada, sem ampliar
infraestrutura, ADR ou efeitos por inferência.

Sem staging, instalação, diretórios/contas/CA operacionais, SCM/ACL, Start/Stop,
endpoint, CURRENT, rede, KSP/ownerAuth, TPM/NV/Q4, cleanup/refund ou recuperação.
E21 PENDING; ED11/Phase6 PARTIAL; staging DENY. Nenhum código alterado/rebuild
ou campanha inalterada reexecutada na preparação deste handoff.
