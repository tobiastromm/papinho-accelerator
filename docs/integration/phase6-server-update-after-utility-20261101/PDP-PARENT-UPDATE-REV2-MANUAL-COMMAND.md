# Único comando ativo — UPDATE PDP revisão2

Autorização contextual específica registrada; nenhuma execução automática. Revisão1 substituída antes de execução/consumo. Não usar comando antigo.

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\update-pdp-parent-image.REV2.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'd4ceddf634a5752d653a3ad1bf1fa53fd9de9bb2e7681b79d39b8bb8066137c3' -ExpectedRequestSha256 '35ccc24ff164336a432a54fdaa4106b52cf3511240b8857a0d5a925e5f14e431' -ExpectedDecisionSha256 '4b381bfb94ff3f4e0cc7c866eddfe3ad914f91ab8e9c35141691e0a6db0e3399'
```

Conferir plano e digitar somente OWNER_BINARY_UPDATE com a tentativa nativa exibida. Sem retry, rollback ou criação de parent/source/FCA. Enviar saída integral.

SUPERSEDED_BEFORE_EXECUTION: NÃO EXECUTAR ESTE COMANDO. Único ativo: PDP-PARENT-UPDATE-REV3-MANUAL-COMMAND.md.

