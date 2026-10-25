# Comando seguro disponível e próximo gate

Executável candidate absoluto: C:\Projetos\PapinhoAccelerator\build\windows-scm-lifecycle-20261022\Selected\Release\papacc_server.exe
SHA256: e0e5c3b617382b88e3ca2aa69737d31a8f3976768e8e4a74b9da11447cb54a39

Consulta somenteleitura (não exige nem concede autoridadeCA):

```powershell
& 'C:\Projetos\PapinhoAccelerator\build\windows-scm-lifecycle-20261022\Selected\Release\papacc_server.exe' service status
```

ServiceName=PapinhoAccelerator. ABSENT é observação pontualSCM, não declaraçãoNEW_INSTALL ou prova de ausência de CA. Zero significa consulta/fechamento concluídos, não instalação produtiva. Todos os comandos mutantes permanecemDENY antes de abrirSCM. Não fornecer comando positivoCreateService: produtor de conta/credencial/instalação nãoqualificado. Não utilizar conta/model do teste, SYSTEMdefault, novoUAC ou setter/envflag para contornar.

Próximo gate concreto: referenciar/qualificar conta local padrão realmente selecionada, vínculo ao executable/config/instalação e ACLs, logonSCM com credencial por mecanismoWindows protegido, sem criar conta/ACL implicitamente; ligar ServiceMain ao runtime existente com prontidão e quiescência factuais e orçamento. Só então preparar comandosLABexatos para revisão oficial, semexecutar automaticamente. M0–M11 jáaccepted, não é nova rodada de escopo/backup.
