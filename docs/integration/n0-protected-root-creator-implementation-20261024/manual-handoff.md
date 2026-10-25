# N0R-20261024-01 — operação futura, não executar sem aprovação específica

Efeitos candidatos: criar exclusivamente os três diretórios C:\ProgramData\PapinhoAccelerator, Protected e Protected\N0, com proteção explícita desde CreateDirectoryW; criar somente o recibo administrativo manual-N0R-20261024-01.txt neste pacote. Não coloca inputs, não altera ACLs existentes, não executa SCM, preflight ou Start. Qualquer nome existente interrompe; não adotar, corrigir, apagar ou repetir.

Operador SID S-1-5-21-785699550-1440458426-1682014700-1108, PowerShell Windows 5.1 x64 já elevado e console interativo. Host SHA256 2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff; boot Kernel-General 12 record 236441, StartTime 2026-10-23T18:36:34.5000000Z. Divergência exige STOP, sem repin automático. Os pais C:\ e C:\ProgramData possuem identidades e descriptors originais pinados no executável; alteração interrompe.

Executável Selected Release SHA256: `29226a2ff79f6bcc3f11374aba1383e3fd0e9b9850e4ae648e0d8f54fb233324`.
Script SHA256: `77c0ec6ce65febd2eeba58172ea89ce349ac2d19bb0c00847f088fa5ff25c9d9`.
Demais pins: qualified-pins.json. A fonte está em árvore de desenvolvimento; o bootstrap abaixo verifica e executa os mesmos bytes originais mantidos abertos, sem confiar apenas na autoverificação do script.

Após aprovação específica desta operação, colar TODO o bloco abaixo em um único PowerShell já elevado. Não executar agora:

```powershell
$source = 'C:\Projetos\PapinhoAccelerator\tools\qualification\n0-protected-root-create-manual.ps1'
$expected = '77c0ec6ce65febd2eeba58172ea89ce349ac2d19bb0c00847f088fa5ff25c9d9'
$held = [IO.File]::Open($source, [IO.FileMode]::Open, [IO.FileAccess]::Read, [IO.FileShare]::Read)
try {
    $buffer = [IO.MemoryStream]::new()
    try { $held.CopyTo($buffer); $bytes = $buffer.ToArray() } finally { $buffer.Dispose() }
    $hash = [Security.Cryptography.SHA256]::Create()
    try { $actual = ([BitConverter]::ToString($hash.ComputeHash($bytes))).Replace('-', '').ToLowerInvariant() } finally { $hash.Dispose() }
    if ($actual -cne $expected) { throw 'SCRIPT_PIN_DENY' }
    & ([scriptblock]::Create([Text.UTF8Encoding]::new($false, $true).GetString($bytes))) -Mode Create -ExpectedScriptSha256 $expected
} finally {
    $held.Dispose()
}
```

Confirmar SOMENTE quando o executável apresentar o plano exato e a operação estiver aprovada:

```text
CREATE_N0_ROOT N0R-20261024-01 75330d11cc7e4123ae62eadc9a59c2f5
```

Escape solicita Stop entre etapas; Ctrl+C sinaliza Stop cooperativo. Stop solicitado não significa concluído. Não fechar console/processo durante UNKNOWN. Perda do console não comprova quiescência. O criador não impede término forçado pelo SO e não garante rollback. Aguardar retorno factual, preservar recibo e qualquer árvore parcial. Não repetir em caso de falha. Não há comando de cleanup automático nem autorização para apagar efeitos.

O gate estrito do controle observado 0x9004 pode interromper depois de criação parcial se o Windows devolver outra representação; esse risco deve constar da aprovação exata. SACL e imagem mapeada não são provadas por esta operação. Recibo em docs é evidência administrativa, não input protegido ou autoridade operacional. O caminho positivo não foi executado nos testes offline.
