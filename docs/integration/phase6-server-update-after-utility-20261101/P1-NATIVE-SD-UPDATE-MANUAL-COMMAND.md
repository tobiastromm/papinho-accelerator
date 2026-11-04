# Comando manual autorizado — UPDATE P1 SD

P1-NATIVE-SD-UPDATE-AUTHORIZATION.md registra a autorização específica.
Execução ainda pendente. Em PowerShell como administrador, executar UMA vez:

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\update-p1-native-sd.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 '6bdd439d307af22177e5a3cc35fef94d1c0831fc1f30ea590224e35553f524b7' -ExpectedRequestSha256 '599e1b4c4cb986b2eaf5dcfd08d7aab73abffd41c4f745bf4ad3074f756d3b6e'
```

O procedimento confere hashes antes da chamada ao executor protegido, contexto,
originais e continuidade. O owner exige seu plano/confirmacão nativa atribuída,
SCM parado/configuração original, proteção/identidade e K/R frescos.
Bypass vale só para o processo filho, não muda política persistente ou GPO.
Nenhum comando foi executado pelo agente. Não executar o EXE candidato.

Antes de confirmar, conferir anterior774656/hash981658...0220,
novo781312/hashdc4855...3076, updater104448/hash084aa...822a, parent/IDs,
K2027314 e R pela geometria real. Para cluster4096,R782336 é apenas exemplo.
Conferir os DOIS leaves exclusivos concretos da mesma tentativa no plano.
Não copiar resposta de outra tentativa ou aceitar qualquer divergência.

Se houver falha/DENY/UNKNOWN, não repetir nem limpar/restaurar. Enviar saída
integral do plano e resultado, incluindo STAGED_UPDATE_RESULT, primeira causa,
renames/readback/fechamento e MANUAL_SERVER_UPDATE_EXIT. Se não retornar por
retenção, não encerrar processo por esta autorização; informar última saída.
Não iniciar serviço após UPDATE: E/Q/S dependem de nova aprovação específica.
