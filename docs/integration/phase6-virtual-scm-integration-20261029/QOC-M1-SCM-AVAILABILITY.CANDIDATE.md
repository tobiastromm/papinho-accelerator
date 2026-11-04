# M1 — disponibilidade SCM: consulta única, somente metadados

Status: CANDIDATE_NOT_APPROVED_NOT_EXECUTED. Não é pedido SCQ nem autorização
operacional. O proprietário precisa aprovar explicitamente este texto byte-exato
antes de qualquer execução; a task atual proíbe SCM. Sem aceitação normativa nova.

## Finalidade e necessidade

Determinar se o serviço original PapinhoAccelerator está disponível atualmente
para uma futura observação de recursos/token por referências originais, SEM
instalar/iniciar processo para satisfazer o ensaio. Histórico de erro1060 não
prova ausência atual e diretórios/nomes não provam publicação. Esta é a menor
primeira observação: uma consulta de disponibilidade, status e PID. Não mede o
envelope SCQ, autentica runtime ou transfere autoridade. Se o serviço não existir,
STOP: não instalar, criar serviço, executar binário ou substituir por fixture.

## Ferramenta e escopo exatos

Resolver System32 pelo GetSystemDirectoryW foi permitido apenas para preparar
este vínculo. Ferramenta existente: `C:\WINDOWS\system32\sc.exe`.
SHA256 dos bytes em disco: `41f067c3a11b02fe39947f9eba68ae5c7cb5bd1872a6009a4cd1506554a9aba9`; 72192 bytes.
Não é atestação de imagem mapeada, assinatura/cadeia de confiança verificada,
identidade SCM adquirida ou permissão. Nenhuma consulta SCM foi executada.

Uma invocação manual de `sc.exe queryex PapinhoAccelerator`, em Windows
PowerShell5.1 x64 já elevado, depois de aprovação específica. Validar o hash
ANTES da chamada. Sem bypass de UAC/política, retry, script remoto, timeout
inventado ou comandos de controle. Consulta somente o serviço exato, sem `qc`,
registro, command line de serviço ou conteúdo de arquivos que possa conter
segredos. Nenhum endpoint privado SCQ é criado/conectado; IPC interno existente
do SCM pela ferramenta do Windows não é um novo canal administrativo do produto.

A aprovação futura é só desta consulta pública: não reutiliza autorização SCQ,
NEW_INSTALL, CNG, TPM ou LAB. Não registra grants nem preenche os três seletores
operacionais NULL. Não concede quota, capacidade ou dispensa quantitativa.
Validade por evento: mesma invocação manual/contexto/ferramenta/propósito e
aprovação específica; divergência implica STOP. Não afirma limite global de
latência para a chamada Windows nem importa duração das medições do coletor.

## Procedimento candidato — NÃO EXECUTAR SEM APROVAÇÃO

```powershell
if ($PSVersionTable.PSVersion.Major -ne 5 -or -not [Environment]::Is64BitProcess) { throw 'M1_CONTEXT_DENY' }
$scqM1Identity = [Security.Principal.WindowsIdentity]::GetCurrent()
try {
    $scqM1Principal = New-Object Security.Principal.WindowsPrincipal($scqM1Identity)
    if (-not $scqM1Principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) { throw 'M1_ADMIN_CONTEXT_DENY' }
} finally { $scqM1Identity.Dispose() }
$scqM1Tool = 'C:\WINDOWS\system32\sc.exe'
if ((Get-FileHash -LiteralPath $scqM1Tool -Algorithm SHA256).Hash -ne '41f067c3a11b02fe39947f9eba68ae5c7cb5bd1872a6009a4cd1506554a9aba9') { throw 'M1_TOOL_HASH_DENY' }
& $scqM1Tool queryex PapinhoAccelerator
$scqM1Exit = $LASTEXITCODE
'SCM_QUERY_EXIT={0}; AUTHORITY=DENY; NO_RETRY' -f $scqM1Exit
```

Contexto administrativo observado não é custódia nem aprovação SCQ. Não solicitar
credenciais. Evidência permitida: stdout padrão de queryex e exit code, somente
metadados de serviço/status/PID. PID é candidato, nunca identidade autenticada;
nenhum PID futuro é fixado. Após a saída, distinguir erro1060, acesso negado,
falha de consulta e status efetivamente observado. Ausência/negação não é zero
recurso nem serviço elegível. Resultado incerto continua UNKNOWN/DENY.

## STOP, fechamento e efeitos excluídos

STOP por hash/contexto divergente, qualquer erro, serviço ausente, PID0/processo
não observável ou saída incompleta. Não repetir, aguardar RUNNING, chamar Start/
Stop, create/config/control/delete, alterar SCM/ACL, criar parent/conta/CA, CURRENT,
endpoint SCQ, rede produtiva, KSP/CNG, TPM/Q4, limpeza/refund ou recuperação.
Se a chamada não retornar, registrar a incerteza; não interpretar timeout como
fechamento, não reinvocar ou executar controle do serviço por consequência.
A ferramenta controla seus handles internos; fechamento interno UNOBSERVED,
sem promessa de quiescência de serviço ou liberação de reservas de SCQ.

Se houver processo disponível, o passo factual posterior ainda precisa adquirir
referências originais, validar TokenUser/grupos/restrições/PID-criação/SCM/imagem
e medir preparação/recursos no contexto certo. Esta saída não preenche essa
prova nem determina limites de sessão, endpoint, Stop ou retenção. Qualquer
novo acesso protegido continua com escopo e autoridade próprios. Sem artefato
final/plano quantitativo/originais: pedido SCQ NOT_READY, E21 PENDING,
ED11/Phase6 PARTIAL, autoridade DENY.
