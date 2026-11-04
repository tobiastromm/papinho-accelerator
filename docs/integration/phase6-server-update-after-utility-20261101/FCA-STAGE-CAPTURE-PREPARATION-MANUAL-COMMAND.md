# Comando manual — nova preparação FCA autorizada

Uma execução no PowerShell administrativo original. Conferir plano e digitar somente a confirmação atribuída exibida. Não executar wrapper antigo/UPDATE, não repetir em falha; enviar saída completa.

& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\run-fca-stage-capture-preparation.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'f664df8123bb7098d9df6c83829f7fa708f242ba27f1cad0673ee106576e91ec' -ExpectedOperationalRequestSha256 'ae3b8ea2937a01bf2498b3c19c48b4058529c691c43d9f83b07e93df84073ec0' -ExpectedDecisionSha256 '310d914faa601ee5bda6666a66f49023dd91983e79b6cc8bac89a1abefec76a5'
