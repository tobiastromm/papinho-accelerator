# Pedido focal — uma coleta manual do layout binário

PROPOSTO, NÃO APROVADO, NÃO EXECUTADO. Autorizações das coletas9ffe2080... e
a4cbd102... foram usadas e não são reutilizadas por este pedido.

## Propósito e escopo

Uma coleta manual read-only no mesmo owner/processo em Windows PowerShell5.1
Desktop x64 já elevado. Avaliar preparação do staging de binários usando apenas
o known folder ProgramFiles necessário a esse propósito, sem consulta ProgramData.
Não há novo backend/host/serviço/root, token privilegiado substituto ou atalho de
autoridade. Não afirmar antecipadamente redução de handles/capacidade.

O layout completo NEW_INSTALL continua existindo e é explicitamente distinto
do privado BINARY_ONLY. O perfil binário não valida instalação completa/CA/SCQ.
Guard124, cap128, cohort1024, Stop, proteção/revalidação e UNKNOWN preservados.

## Artefatos exatos

Script: collect-staging-binary-layout.MANUAL.ps1 neste diretório.
SHA256: `d1f72e217e90ee167267aada614c0bd1b04b12fce2ddc1de1c24cec12ce23c5f`.
Somente parsing estático, não executado.

Fonte dos três arquivos públicos:
`C:\Projetos\PapinhoAccelerator\build\phase6-functional-20261028\Selected\Release`.

| Arquivo | Bytes | SHA256 |
|---|---:|---|
| papacc_server.exe |555008|4a68ce6bdb2737221824ba2f70930fbb8534ea5dfda3d9cfa460eaef22c546f9|
| libssl-3-x64.dll |1321472|3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed|
| libcrypto-3-x64.dll |7217664|09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044|

Total9.094.144 bytes, SDK normal/API3 qualificador0.6.5, não release ou staging.
Script/pacote serão conferidos antes da chamada, por hashes de disco, sem
alegação de mapped-image attestation. O script novo não altera política.

## Sequência e evidências

Uma chamada fixa a --qualification-staging-observe, sem RNG/confirmar/take ou
mutação de instalação. Mesmos frames e query pós-entrada do diagnóstico anterior:
entrada(1), layout(7), contexto(8), descritores(9), ancestrais(2), fontes/volume(3),
antes/depois do close(4/5). A query simples após o primeiro frame isola o intervalo
do observador antes do layout; a query pré-abertura mantém seu resultado original.
Coletar só os pontos alcançados; sem loops adaptativos ou tentativa de alcançar
fases posteriores quando um guard nega.

ProgramFiles continua vindo de SHGetKnownFolderPath com DONT_VERIFY; não de
strings de ambiente. Apenas FILE_OPEN para originais/volume/fontes; não criar,
abrir ou adotar Papinho/Accelerator. Os direitos do preflight, inclusive pedido
de ADD_SUBDIRECTORY no parent aplicável, são verificados mas não usados para criar.
SID/token/objetos frescos são adquiridos nessa invocação; não preencher com dados
históricos, fixtures ou conta SCM futura. Saída não secreta no console.

## Proposta de exceção de política para esta invocação

Se necessária para script não assinado, esta aprovação incluirá salvar o valor
ExecutionPolicy Process, aplicar Bypass somente ao processo PowerShell atual,
conferir política efetiva e restaurar Process no finally. Sem CurrentUser,
LocalMachine/GPO, assinatura, conta, UAC automático ou processo auxiliar. A
exceção anterior não é herdada; nenhum comando é emitido antes de aprovação.

## Efeitos e STOP

Uma invocação do exe com consultas/leituras; não há escrita solicitada, arquivo
de evidência pelo script, staging, diretório operacional, SCM/ACL, Start/Stop,
endpoint, CA/CURRENT, KSP/ownerAuth, TPM/Q4, rede, cleanup/refund ou recuperação.
Loader/cache/metadados de acesso e outros efeitos internos do OS continuam
opacos, não declarados zero. Consultas por si não reservam recursos.

STOP em divergência de pin/contexto/proteção, query falha, insuficiência/guard,
aritmética inconsistente, Stop, pending ou close incerto. Não aumentar limite,
omitir recurso obrigatório, mudar path, rearmar autorização ou tentar de novo.
Retenção/backings permanecem no owner enquanto vive, sem promessa após término
do processo. Interrupção ou receipt incompleto é UNKNOWN, não close provado.

Exit0 é observação, nunca admissão. Se o cold path de ProgramFiles obrigatório
continuar excedendo o limite, registrar esse fato: esta coleta não permite
dispensar a API ou elevar quota. Recursos ativos de FS, margem e retenção
continuam sujeitos aos contratos accepted; não são provados por snapshots.
Após aprovação explícita do pedido integral, registrar ato novo e preparar
comando manual preenchido, sem execução automática ou retry das coletas antigas.
