# N0START-20261024-02 — comando manual após reaprovação

Revisão estática reconciliada em reapproval-final-review-20261024.json. Aprovação real do owner registrada exclusivamente em EXACT-OPERATION-APPROVED.json (9392 bytes). A aprovação condicional anterior não foi promovida nem reutilizada. Nenhum fonte/binário/pin foi alterado.

Script SHA256: 573bf1ca9a95dc052c992a0893b47f06f9dd96a92f53d3aa41cdf5bcc25951c2
Qualificador Selected Release SHA256: b482b959400b41e6e64ecb86858dfd00522a0aa988b36300dc7d70f4d3c33eae
Registro aprovado SHA256: 556167c630bf870797dbd2b970c6d5a5f2177cc06d731b1986c4aa30dbe18456
Request aprovado SHA256: 9f7443f233c4c08a052186a779e7e60f51b79d68594edab7f4d964ccd8a48f65

Em um único Windows PowerShell5.1 x64 NOVO e já elevado. Não iniciar outro PowerShell pelo comando. Se necessário, ajustar apenas a política deste processo:

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force
& "C:\Projetos\PapinhoAccelerator\tools\qualification\scm-n0-protected-single-powershell.ps1" -Mode Cycle -ExpectedScriptSha256 573bf1ca9a95dc052c992a0893b47f06f9dd96a92f53d3aa41cdf5bcc25951c2 -ExactApprovalSha256 556167c630bf870797dbd2b970c6d5a5f2177cc06d731b1986c4aa30dbe18456
```

Executar uma única vez. O script apresenta START_N0_SINGLE_LAB com identidade auxiliar contemporânea. Copiar literalmente a confirmação apresentada naquela execução. Depois, digitar os desafios BIND_BD_READ_ONLY e INSTRUMENT_SCM_LAB exibidos pelo qualificador; não usar PID/criação/challenge/plano antigos nem automatizar respostas.

Todos os gates factuais continuam obrigatórios imediatamente antes dos efeitos; esta conferência estática não substitui admissão fresca. Host/boot236441,operador1108,LAB/tentativa/config/candidato/políticas e RAW1 devem conferir. Colisão,claim anterior,recovery inesperado,divergência,recurso excedido,stale ou Stop: DENY/STOP sem repetir. A consulta de ausência não reserva nomes; FILE_CREATE/CREATE_NEW permanecem. Pode haver consumo antes de Start e efeitos parciais sem rollback.

Efeitos condicionados: intent único,imagem exclusiva,sink/arm exclusivo,um rebind ImagePath,uma chamada StartService,até uma solicitação segura de Stop,NTSTATUS/métricas/recibos e retenção. Sem rede/listener/CA/cert/bootstrap/CURRENT/produção. Limites128/1024,quatro papéis,60s/1s,NONE_GRANTED preservados. N7 e demais incertezas históricas aceitas permanecem delimitadas.

Stop solicitado/aceito/concluído,quiescência e referências fechadas são fatos distintos. Não fechar janela por silêncio/timeout; não matar/liberar à força. F10 somente após aviso factual de retorno/fechamento do qualificador. Em falha ou UNKNOWN,preservar saída/recibos e não repetir. Enviar saída completa,IG_DIAGNOSTIC/IG_FRESHNESS/IG_FINAL ou IG_RETAINED/IG_FIRST_RECORD/N0_CYCLE_EXIT.

START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=OWNER_MANUAL_N0START_20261024_02
