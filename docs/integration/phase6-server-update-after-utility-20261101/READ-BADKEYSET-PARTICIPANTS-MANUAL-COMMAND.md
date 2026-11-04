# Uma consulta manual read-only

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoLogo -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\observe-read-badkeyset-participants.MANUAL.CANDIDATE.ps1' -ExpectedScriptSha256 'b8da1cdc818813ef6e6246f1c58e17836eaee97b35efb9d171a301ae62d7843a' -ExpectedRequestSha256 '9ea0ce86bac95cca0d4085bf7f779765d8038f2f4186fbc24cb9a2d48c32538e'
```

Confirmação exata OBSERVE_READBADKEYSET_CURRENT_PARTICIPANTS_ONCE. Sem retry/kill.
