# Complemento read-only no console existente

O TASK atual autoriza investigaÃ§Ã£o nativa nÃ£o TPM. Este comando NÃƒO usa a aprovaÃ§Ã£o TPM consumida e nÃ£o Ã© nova prova/intent/admissÃ£o. Necessita do Windows PowerShell jÃ¡ elevado do operador; o agente nÃ£o contorna UAC. NÃ£o iniciar novo console/auxiliar artificial para procurar PASS.

ExecutÃ¡vel SHA256 `438735bb3ea0286fbc8f6b67ca04663f7e990d93cc6db159cb744efc70c20dc8`. Ã‰ o observador nativo de metadados, nÃ£o NativeTpmProof.exe. Espera o processo17204 com criaÃ§Ã£o134376272948693901, verificada por handle original. Se saiu ou mudou, NEGAR, nÃ£o trocar o PID silenciosamente. A consulta fixa esse seed mas nÃ£o confere autoridade por PID. Os outros processos serÃ£o descobertos e identificados por handles, token/sessÃ£o/criaÃ§Ã£o/arquivo e consultas repetidas; association/mapped image continuam qualificadas no alcance declarado.

Executar uma coleta no prÃ³prio PowerShell elevado jÃ¡ existente, sem pipeline ou wrapper:

```powershell
& "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\real-console-role-binding-20261027\Release\ConsoleRoles.exe" --observe-inherited 17204 134376272948693901
```

O modo inherited NÃƒO chama FreeConsole/AttachConsole; lÃª os clientes do console que o observador herdou. NÃ£o usa TPM/TBS/NV, rede, serviÃ§o, SCM/ACL, root/CURRENT ou aprovaÃ§Ã£o anterior. NÃ£o altera console/polÃ­tica de execuÃ§Ã£o nem escreve arquivos protegidos. Enviar o JSON integral. Contagens e lista de clientes sÃ£o novas observaÃ§Ãµes, nÃ£o orÃ§amento concedido. Nada autoriza repetir uma prova TPM.

O diagnÃ³stico anterior registrou before_clients2, FreeConsole TRUE/erro0, depois0, AttachConsole FALSE/erro5. A documentaÃ§Ã£o nÃ£o torna esse cÃ³digo prova universal de UAC; motivo exato do5 permanece indeterminado. O contexto manual herdado elimina a dependÃªncia dessa associaÃ§Ã£o externa e permite comparar operador/pai/cliente/auxiliar reais. Se houver retenÃ§Ã£o, preservar processo e saÃ­da UNKNOWN, sem fechar Ã  forÃ§a.

ADR C0â€“C9 Ã© PROPOSED, SHA256 `7dee709121cc9b56633efb6d968bd0935b81bc6c18b000226ed35cf0a179f985`. Leitura diagnÃ³stica nÃ£o a aceita. ApÃ³s a evidÃªncia, ainda serÃ¡ necessÃ¡ria decisÃ£o explÃ­cita de binding/risco, integraÃ§Ã£o tÃ©cnica autorizada e pedido TPM novo com pins finais, se os gates estiverem demonstrados.
