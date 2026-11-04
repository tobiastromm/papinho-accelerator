# Pedido diagnóstico focal — origem da negação de handles

PROPOSTO, NÃO APROVADO, NÃO EXECUTADO. Não reutilizar autorização9ffe2080...
ou exceção Process38fc35af... já usadas na coleta anterior.

## Propósito e bytes

Uma única coleta manual read-only pelo proprietário em Windows PowerShell5.1
Desktop x64 já elevado, sem instalação ou staging. Mesmo owner/processo/volume
e mecanismos existentes; não criar host, serviço ou IPC alternativo.

Script: `collect-staging-diagnostic.MANUAL.ps1` neste diretório.
SHA256: `7c1ca380bf8e01fdee7fa3f564131843de7381bc61c80ac3b9ce4d46b7dfe496`.
Somente parsing estático validado; não executado.

Fonte dos três arquivos:
`C:\Projetos\PapinhoAccelerator\build\phase6-functional-20261028\Selected\Release`.

| Arquivo | Bytes | SHA256 |
|---|---:|---|
| papacc_server.exe |553472|1d2b92c2c397e1415db83d57cb8b3ac22127107855123f0340c573e7940dbe85|
| libssl-3-x64.dll |1321472|3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed|
| libcrypto-3-x64.dll |7217664|09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044|

Total9.092.608 bytes. Conferir pins antes da chamada; sem mapped-image attestation
ou autoridade produtiva derivada dos hashes. Seleção somente para diagnóstico;
não selecionar/promover pacote de instalação. PST0.6.4 permanece produtivo.

## Delta mínimo da coleta

Mesma entrada `--qualification-staging-observe`, sem RNG/confirmação/take/efeito.
Além dos cinco frames anteriores, observar após layout, após contexto nativo e
após materialização dos descritores, total8 se saudável. Reportar o resultado
original do GetProcessHandleCount pré-abertura: sucesso, contagem ou erro e
negação de guard separadamente, sem segunda query para substituir esse resultado.
Uma query simples adicional imediatamente após a emissão do primeiro frame,
antes de qualquer layout, registra sucesso/contagem/erro nesse outro instante.
Ela isola o intervalo do próprio observador, cujas outras APIs/stdio ocorrem
depois do handles do frame1. São3 frames completos adicionais e1 query simples
adicional; não são uma amostra universal, teto ou custo nulo.

Preservar124/128/1024 e todos os predicados. Se a coleta parar antes, registrar
somente os fatos alcançados. Não relaxar guard ou ignorar referências para chegar
ao volume. Contagem inicial115 anterior não será aplicada como headroom futuro.

O perfil continua abrindo somente FILE_OPEN para volume/ancestrais existentes e
fontes públicas, incluindo direitos do preflight explicitados no pedido antigo;
ADD_SUBDIRECTORY pode ser solicitado para verificar elegibilidade, nunca usado
para criação. Consultar token/identidade/proteção, recursos próprios/sistema e
volume por handle; não abrir/criar Papinho/Accelerator ou root de dados.
Identidades são adquiridas frescas; não inventar SID/PID ou usar fixtures.

## Exceção de política proposta para esta invocação

Se necessária por scripts não assinados, esta aprovação deverá abranger também:
salvar ExecutionPolicy Process, usar Bypass APENAS no processo PowerShell atual,
verificar política efetiva e restaurar Process no finally. Nenhuma política
CurrentUser/LocalMachine/GPO, assinatura/certificado, UAC ou processo auxiliar
será criado/alterado. Essa exceção só poderá valer após aprovação deste pedido;
o `autorizo` anterior não é herdado. O próprio script não altera política.

## Efeitos, limites e STOP

Uma invocação do exe com os pins acima; consultas/leituras e saída no console.
Nenhum arquivo operacional, ACL, SCM, Start/Stop, endpoint, CA/CURRENT, KSP,
ownerAuth, TPM/Q4, rede, cleanup/refund ou recuperação. Caches/loader/metadados
de acesso e custos internos do OS não são declarados inexistentes ou limitados.

STOP em qualquer pin/contexto/proteção/query/aritmética divergente, guard,
insuficiência, Stop/perda de continuidade, pending ou close incerto. Sem retry,
troca de caminho, rearmar aprovação, alterar limites ou exceção de UNKNOWN.
Conservar backings/IO status enquanto o owner vive; não afirmar persistência de
handles após fim do processo. Interrupção/receipt incompleto é UNKNOWN.

Exit0 e snapshots saudáveis são somente observação. Esta prova pode falhar de
novo e não demonstra teto de FS/kernel, margem/reserva, disposição global ou
admissão de staging. Saída integral será reconciliada antes do próximo gate.
Após aprovação explícita, registrar novo ato e emitir comando manual preenchido;
não executar automaticamente nem entregar um comando de retry da coleta antiga.
