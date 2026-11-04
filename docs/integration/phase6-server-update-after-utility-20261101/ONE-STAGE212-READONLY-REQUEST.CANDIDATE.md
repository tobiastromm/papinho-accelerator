# Pedido: uma consulta manual de disposição pós-Start, etapa 212

Estado: CANDIDATE; nenhuma execução autorizada por este arquivo.

Escopo PERSONAL_LOCAL no host TCORP-TOBIAS, operador S-1-5-21-785699550-1440458426-1682014700-1108: uma invocação manual do script observe-stage212-disposition.MANUAL.CANDIDATE.ps1, SHA256 ec0505b97a4da4b9b5acbc3261f100e650f1846455cb9bdb8c0cc129d50b129c. Reutiliza o observador de disposição existente; nenhuma execução pelo agente.

Operações fixas: sc.exe queryex, qc e qsidtype de PapinhoAccelerator, sequenciais, interrompidas na primeira falha; depois Get-Process do controlador PID 7580 e comparação do FILETIME original (high=31288063, low=2826307890), HasExited e Dispose do objeto do próprio observador. PID isolado não atribui identidade. Não consulta token, memória, handles internos nem enumera processos. O PID publicado por SCM é somente candidato atual: não atesta identidade histórica do runtime. Ausência/mismatch permanece UNKNOWN, sem retry.

Gates: hashes do pedido e script, host/operador/arquitetura x64/Desktop/console original; confirmação manual OBSERVE_STAGE212_DISPOSITION_ONCE somente após aprovação específica. O wrapper verifica hashes antes das consultas SCM. Registrar saída integral e primeira falha. Nenhuma resposta fornecida automaticamente.

Custos: novo processo PowerShell, sc.exe sequenciais e referências temporárias do observador; máximos nativos opacos, sem admissão de capacidade. Read-only significa ausência de mutação administrativa de serviço/arquivos-alvo, não ausência de custos do observador.

Proibidos: Start/Stop, endpoint, SCQ operacional, kill/terminate, espera por término, UPDATE, rename, exclusão/limpeza, ACL, CA/CURRENT, rede operacional, TPM/NV/Q4/refund. Não concede autoridade para execução direta do host de serviço. Uma execução manual; sem retry.

Preservar os originais e autorização consumida, retenção histórica, fechamento da aplicação e quiescência OS UNKNOWN. Consulta atual não resolve a causa histórica do erro 5 nem prova autenticação bilateral, receipt/close ou prontidão produtiva. A autorização é exclusivamente para a consulta descrita; quaisquer medidas posteriores dependem da evidência e de seu próprio escopo.
