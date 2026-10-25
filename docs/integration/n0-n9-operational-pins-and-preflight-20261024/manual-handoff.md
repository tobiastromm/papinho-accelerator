# N0PF-20261024-01 — uma operação manual read-only, aprovação ainda necessária

Não executar nesta tarefa. Este pacote prepara a operação; aprovação de fonte/pins não equivale à autorização da execução.

## Identidade e efeitos exatos

Um único Windows PowerShell5.1 x64 já elevado, operadorSID S-1-5-21-785699550-1440458426-1682014700-1108. HostSHA2562b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff, Windows10.0.19045 AMD64, bootKernel-General12 record236441/StartTime2026-10-23T18:36:34.5000000Z, NTFS serial417614100/ACLs persistentes. Uma invocação --prepare-n0 da tentativa75330d11cc7e4123ae62eadc9a59c2f5, serviço existentePapinhoAcceleratorLab20261023. Políticas/inputs/config/imagens/ancestrais exatos em pin-review-matrix.json e qualified-pins.json.

Permite somente consultas e referências/locks temporários: verifica operador/hash/boot/binding protegido, readback SCM sem alteração, mutex canônico read-only, confirmação, censo N0–N9, métricas e limites ED11, primeira falha, fechamento/retenção. Não cria input, diretório, arquivo diagnóstico operacional ou intent; não consome/rearma claim anterior. Não é nova admissão operacional ou autorização de Start. Os dois inputs existentes foram qualificados; ainda serão revalidados nos originais, sem promover a consulta antiga a snapshot fresh.

Script SHA256 `e8eb796e24634a8e98319e025d5870597296d69bff4f7bff83f213c8d4209e55`.
Qualificador Selected Release SHA256 `f3c38ced18c6e25b85f1e129a6c1501daf752eef15748294c48eed7f8b479a08`.
O binário admite SOMENTE a forma --prepare-n0; modos positivos/observadores são rejeitados antes de aquisição. Não reutilizar o binário anterior.

## Comando candidato depois da aprovação específica

Abrir uma NOVA janela Windows PowerShell5.1 x64 como administrador. Não usar `powershell.exe -File` a partir de outro PowerShell: isso criaria um segundo participante. Não reutilizar janela que já carregou PaccFileAcl nos procedimentos anteriores. Se a política bloquear scripts e for permitido, apenas Scope Process Bypass; não alterar política permanente/GPO.

Conferência independente do hash do script, sem executar preflight:

```powershell
Get-FileHash -LiteralPath "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1" -Algorithm SHA256
```

Após aprovação específica e conferência com o hash acima, comando exato, uma vez:

```powershell
& "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1" -Mode Prepare -ExpectedScriptSha256 e8eb796e24634a8e98319e025d5870597296d69bff4f7bff83f213c8d4209e55
```

A nominação consulta o tuple contemporâneo do candidato auxiliar. Zero/múltiplos/erro para antes do qualificador; não escolher outro automaticamente ou substituir PID histórico. Nome/session/criação são somente seleção, nunca autorização. O owner valida toda a prova N2 e mantém N7 explícito.

## Duas confirmações obrigatórias

1. O PowerShell imprimirá a linha COMPLETA:

```text
PREPARE_N0_READ_ONLY 75330d11cc7e4123ae62eadc9a59c2f5 <PID auxiliar observado> <criação auxiliar observada>
```

Os campos entre <> são descritivos, não texto para digitar. Copiar exatamente a linha numérica exibida na execução, sem reutilizar valores desta conversa.

2. O owner nativo apresentará tuple, arquivo/proveniência, limites de classificação, plano/hash e um desafio aleatório existente. Confirmação literal será:

```text
BIND_BD_READ_ONLY 75330d11cc7e4123ae62eadc9a59c2f5 <PID real do qualificador> <criação real> <desafio real>
```

A linha completa só pode existir na execução autorizada. Não a inventar ou pré-gravar. Confirmar somente o plano correto. A janela60s inclui essa confirmação nativa; se expirar, DENY sem refresh/retry. A segunda apresentação usa diretamente o console herdado, mesmo com stdout capturado pelo launcher.

## Resultado e Stop

Aguardar IG_PREPARED_READ_ONLY em caso de PASS e sempre conferir IG_FINAL: primeiro stage/code, close0, starts0/stops0, intent_consumed0/effect0/diagnostic_created0. Falha fechada pode retornar exit2; não é PASS. Se o gate falhar, preservar primeira falha/predicado e não repetir. Console sem saída pode estar aguardando a confirmação nativa: ler a janela antes de concluir que terminou.

F10 SOMENTE após o aviso de retorno factual do qualificador. Ele volta ao mesmo prompt, não cria/fecha outro PowerShell. Console perdido não comprova cleanup. UNKNOWN/receipt faltante/close incerto exige preservar o contexto e referências; não kill, delete, timeout-cleanup, força de mutex ou rearm. Ctrl+C/perda da janela pode deixar resultado incerto; não se promete cleanup por notificação. Stop durante confirmação pode cancelar a entrada; Stop nativo e quiescência são fatos separados. Nenhum StopService é autorizado ou necessário nesta operação read-only.

Não iniciar WPR/ETW nem abrir receptor externo para esta operação. Enviar a saída completa, incluindo as linhas N0-N9, IG_DIAGNOSTIC/IG_FINAL e N0_READ_ONLY_EXIT. Texto fornecido pelo owner será identificado como tal; não é criado recibo persistente secreto/registro administrativo paralelo.

Scripts residem em desenvolvimento: autoverificação não autentica um script já substituído. Exige conferência independente e fonte revisada. Depois do início, streams e locks dos ancestrais protegem o caminho do executável durante a invocação. Compromisso do SO/admin equivalente continua fora da garantia; N7 permanece exatamente aceito, não ampliado.

## Autorização solicitada

Aprovar uma única execução MANUAL read-only N0PF-20261024-01 com estes hashes e duas confirmações, sem retry automático. Não autorizar StartService/StopService/rebind/ACL/colocação/CA/cert/bootstrap/CURRENT. ED11 PARTIAL; CURRENT, assinatura e recuperação autoritativa DENY. Mesmo um preflight PASS não autoriza a operação diagnóstica positiva posterior.
