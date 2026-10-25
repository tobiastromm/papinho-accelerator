# Pedido de aprovação — consulta P3.5 manual read-only

STATUS=PREPARED_NOT_AUTHORIZED_TO_RUN_AUTOMATICALLY. Este pedido não aplica P3 nem concede sucessora. A autorização pedida é só UMA execução manual do leitor já existente, em Windows PowerShell5.1 x64 já elevado pelo owner, com host/boot e hashes originais. Sem senha fornecida ao Codex, UAC/GPO bypass, ajuste de token/ACL, Start/Stop/SCM/preflight ou nova captura. Não iniciar outro PowerShell pelo script.

Antes de autorizar, conferir readerSHA2566732b4166a46dc93dbfade6040b084f148e5460b441cd17d5132068ee4bfc0e0 e pins do diagnóstico. Abrir uma janela apropriada manualmente; não usar uma sessão já contendo os tipos PaccFileAcl/PaccN0InputRead. O leitor nega tipo previamente carregado. Se execução estiver bloqueada pela política de scripts, STOP e informar a política; nenhum comando Set-ExecutionPolicy/Bypass é parte deste handoff.

Comandos candidatos, **somente após aprovação específica**:

```powershell
whoami /user
whoami /groups
& "C:\Projetos\PapinhoAccelerator\tools\qualification\n0-protected-input-readonly.ps1" -Mode Verify -ExpectedScriptSha256 6732b4166a46dc93dbfade6040b084f148e5460b441cd17d5132068ee4bfc0e0
```

A execução Verify exige SID1108, BA habilitado, sem impersonation e hostboot236441. Admin deny-only/integridade média não serve. whoami é evidência auxiliar, não substitui gates do leitor. Nenhuma frase de confirmação deve ser colada no terminal: esse leitor não possui prompt de autorização; a aprovação específica é dada pelo owner antes de invocá-lo.

Efeitos esperados: carregar/compilar em memória os helpers existentes (CodeDom pode usar temporários normais do compilador), abrir somente por leitura os sete objetos da cadeia/inputs originais, comparar IDs/path/type/reparse/RAW1/conteúdo antes/depois, fechar referências próprias e emitir resultado no console. Nenhum arquivo protegido/documental de recibo será criado pelo leitor; o owner pode fornecer o texto do console como evidência posteriormente. Console visível não é recibo persistente por si.

Qualquer divergência/falha cancela a qualificação, sem nova execução automática. Não remediar ACL. Com RETAINED=True/CLOSE_UNKNOWN, manter a janela/processo conforme retenção do leitor, sem kill/forçar close. EXIT0/NONE não aplica P3; apenas permite retomar a revisão de P3.5/P3.6 com os demais efeitos revalidados quando necessário.

Aprovação específica necessária: autorizar UMA consulta manual read-only deste leitor/hash, sem aplicar P3, sem sucessora e sem retry. Nenhum comando positivo LAB é incluído.
