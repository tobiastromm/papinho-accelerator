# Entrada manual curta

Solicitada pelo owner para evitar colagem multiline. Script: tools/qualification/n0-root-manual-entry.ps1. SHA256: 46f4a26d404857c5746f75e21d107ee06a957721e63800c4238fd324fbf63112.

Encapsula o launcher original mantido aberto e verificado por hash, sem alterar seus pins. Review PASS; Create não executado. Confirmação e autorização N0R originais preservadas. Não iniciar retry se houve efeitos anteriores. ED11 PARTIAL, autoridades DENY.

Comando em PowerShell 5.1 x64 já elevado:

```powershell
& "C:\Projetos\PapinhoAccelerator\tools\qualification\n0-root-manual-entry.ps1" -Mode Create
```

O script de entrada reside em desenvolvimento; conferir seu hash independentemente quando necessário. Não se afirma proteção contra substituição do próprio script de entrada. Verificações internas do launcher permanecem obrigatórias.
