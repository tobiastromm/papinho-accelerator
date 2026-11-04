# Executor UPDATE — adaptador mínimo implementado e qualificado offline

2026-11-01. main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469.
Autorização desta tarefa: inventariar e, na ausência de executor adequado,
implementar o menor utilitário técnico no owner existente. Sem execução real.

## Inventário limitado e ausência demonstrada

Escopo: entrypoints de apps/tools e targets CMake do projeto canônico, artefatos
administrativos da configuração Selected/Release e evidência estática já obtida
do instalado. Não é busca irrestrita pelo equipamento nem inventário de todos
os executáveis do Windows. Dados anteriores à implementação em
EXISTING-INVENTORY.json e UPDATE-CALLERS.before.txt.

| Executável/classe existente | Capacidade e proteção | Disposição |
|---|---|---|
| Instalado papacc_server.exe |571392bytes/c0f27f…806ecf; histórico anterior ao UPDATE, leitura estática sem seus literais; proteção histórica do staging, não nova custódia original | É o próprio alvo; U2 proíbe self-update do executor vivo, independentemente de eventual capacidade |
| Workspace papacc_server.exe |740352bytes/021c7e5c…26dd; entrypoint real UPDATE/17 argumentos, mas também é o candidato; snapshot de ACL com owner estranho e AU Modify | Não é executor independente protegido; não executar para tentar admitir DATA |
| Snapshots preservados do server | Artefatos de build/revisão, sem prova de preparação protegida e sem autoridade por nome/hash | Nenhum executor operacional qualificado demonstrado |
| LAB hosts/registration/runtime/TPM helpers | Propósitos e exceções próprios, sem admissão produtiva U2; não presentes como executores protegidos qualificados no escopo | Não reutilizar privilégios LAB, SCM/Start ou TPM |
| scm-dependency-acl-readonly | Somente diagnóstico/consulta, não caller UPDATE | Não fornece capacidade de mutação |
| Reference clients/testes | Cliente ou fixture, não fonte administrativa | Não promovidos a executor |
| Ferramentas Windows genéricas | Não implementam este owner nem o ato UPDATE | Proteção do executável do OS não prova integridade/autoria de um script nem fornece o ato por si |

Antes desta alteração, o único entrypoint administrativo do produto chamando
`papacc_server_launcher_binary_update_cli` era main.c.api3.inc. Não havia target
papacc_update ou artefato correspondente. Ausência demonstrada somente no escopo
acima, não uma alegação de inexistência de qualquer programa fora dele.

Uma cópia do server completo para outro nome também exigiria preparação
protegida e teria entradas de serviço/configuração desnecessárias. O adaptador
restrito abaixo reutiliza exatamente a mesma implementação com uma entrada
menor; não exige copiar o server completo ou criar produto de distribuição.

## Menor implementação e compatibilidade

`apps/server/update_main.c` é apenas um adaptador: aceita explicitamente
--owner-manual-binary-update e encaminha argc/argv e streams originais, sem
reconstruir o plano. Rejeita falta de argumentos, streams inválidos e qualquer
outro propósito; normaliza retorno não bem-sucedido para erro. Não tem fallback
para serviço, instalação, configuração, Start/Stop ou endpoint. Não instala,
protege ou atualiza a si mesmo. Nome do arquivo e flag não concedem autoridade.

Target interno `papacc_update` liga `papacc_server_run_win32`, a mesma biblioteca
STATIC do owner/root e todas as primitivas U1–U7/U2 já qualificadas. Nenhum
estado/root/controller-factory adicional, grant, ledger, canal ou protocolo.
O objeto do adaptador referencia externamente a função original do owner;
dumpbin/symbols preservado em Selected-Release-entry-symbols.log. Não duplica
a implementação desse owner. Cada processo mantém a composição canônica já
existente; não cria outra root administrativa persistente.

Não se disponibiliza esse target no perfil SINGLE_LAB_ADMISSION. Os quatro
targets qualificados preservam MSVC/Ninja, opções e quotas Selected/Legacy;
sem troca de toolchain/backend ou herança de exceção LAB.

Isso é um adaptador técnico permitido pela decisão U2 e pela autorização
explícita atual, não nova decisão de confiança. A política permanece no owner:
executor original estrito, identidade distinta do DATA/EXE anterior, token/
PID/criação, processo/imagem/dependências, confirmação, consumo, criação
exclusiva/cópia/hash/flush, dois renames, readback e retenção. Não houve alteração
em personal_staged_update_win32.h ou nas duas implementações nativas UPDATE.
Hashes dessas fontes são comparados com a evidência U2 ao capturar esta entrega.
Nenhuma nova ADR é necessária ou foi registrada.

O caminho U2 existente já aceita imagem executora independente diretamente no
mesmo parent protegido. Logo um futuro papacc_update.exe original nesse parent,
com todas as provas e seleção legítima, pode executar o ato sobre
papacc_server.exe sem se substituir. Não ampliar paths, allowlists ou guards.
Dados/pins continuam DATA e a aprovação/confirmacão futuras continuam necessárias.

## Qualificação afetada, sem execução do utilitário

Selected/Legacy Debug/Release: quatro builds PASS, zero warnings.
Somente os dois testes afetados por configuração:8 pares PASS.

- update_entry:19 verificações; dispatch/streams/argumentos exatos, negação
  de service/install/host/start/stop/staging/SCQ/help/vazio e falhas do owner.
  Dispatcher é modelado nesse teste; não se declara prova do protocolo por ele.
- staged_binary_update:102 cenários/602 verificações por configuração. O corpo
  exato do novo adaptador agora encaminha ao owner real sobre APIs modeladas
  nos casos positivos e negativos de CLI: executor independente válido,
  confirmação ausente e executor no workspace. Os demais negativos U2 permanecem.

Não executar papacc_update.exe para testar --help ou experimentar uma tentativa.
Ele não foi invocado. O binário real só foi compilado/inspecionado estaticamente;
processo/SCM/RNG/IO mutante permanecem modelados nos testes. Não repetidas as
campanhas inalteradas de staging, SCM registration ou cadeia fria.

PE/imports demonstra que libssl-3-x64.dll e libcrypto-3-x64.dll são dependências
do artefato real, compatíveis com os módulos/arquivos que U2 exige no mesmo
diretório protegido. Nenhuma DLL foi copiada. Esse exame não prova loader futuro,
imagem mapeada ou continuidade de processo: só imports estáticos do artefato.

Não houve crescimento do packet, buffers, slots ou quotas; o adaptador usa
apenas argumentos/streams emprestados e um retorno int, dentro dos frames já
conservadoramente contabilizados do caller. O censo U2 continua aplicável à
mesma composição: K Selected2027282/Legacy1858714, não picos nativos/capacidade.
Loader/CRT/kernel/FS/RPC e quiescência continuam UNKNOWN sob U4, sem novo waiver.
A preparação do utilitário terá seus próprios custos/efeitos e não herda U4.

## Artefato, proteção e proveniência

Selected/Release workspace: papacc_update.exe,100352bytes,
SHA256f449ca4d96853897d009e5b317ec71bdd9fd70dde65240266504e6d07d7390a6.
Artefato técnico qualificado offline; NÃO selecionado operacionalmente, instalado,
protegido ou admitido para executar. Proveniência factual: fontes/build desta
branch/WIP, libs do owner e manifestos/hashes, sem alegação de assinatura.

WORKSPACE-PROTECTION.json captura owner/DACL dos dois EXEs do workspace no
contexto do agente: owner fora do operador esperado, AU Modify herdado, SD não
protegido. Não é o descritor de um handle operacional do proprietário. Confirma
que o build não entrega automaticamente a proteção U2; nenhum PASS por hash.

O papacc_server.exe candidato permanece740352bytes/021c7e5c…26dd, sem alteração
funcional ou de bytes nesta tarefa. DLLs mantêm3fb3cd78… e09eec573…. Instalado e
SCM não foram consultados novamente ou modificados; reutilizada a evidência
estática U2 do c0f27f…806ecf. Não publicar produção ou substituir artefatos.

## Primeiro gate: preparação protegida de UM arquivo

Disponibilidade técnica do executor está resolvida por um adaptador recém-
implementado no produto existente, não outro serviço/subsistema. Disponibilidade operacional ainda não:
é necessária preparação exclusiva de UM papacc_update.exe em
%ProgramFiles%\Papinho\Accelerator, parent existente resolvido por known folder,
retido por identidade/proteção atuais. Nenhuma pasta nova ou distribuição.

Escopo mínimo a qualificar/autorizar separadamente: fonte exata acima lida como
DATA, um arquivo novo exclusivo, proteção aplicada na criação BA/SY full e AU
read/execute, cópia/tamanho/hash/flush/readback e fechamento. Sem overwrite,
adoção, troca de leaf para superar colisão, edição de ACL existente, rename,
DLL copy, EXE canônico, ProgramData, SCM ou execução. Falha parcial retém o
arquivo e mantém UNKNOWN, sem exclusão/retry. File ID futuro não é inventado.

O ator dessa preparação deve ser o administrador legítimo por mecanismo
governado existente do host/owner; NÃO o utilitário do workspace tentando
proteger-se. A tarefa atual não autoriza essa preparação, seu risco ou novo
ato. Qualificar a atribuição/autoridade de preparação antes de pedido operacional,
sem exigir instalar outro utilitário para preparar este e sem improvisar um
script elevado que conceda confiança por hash. U2 permite o executor distinto,
mas explicitamente não provisiona controller nem autoriza cópia.

Pedido operacional atual: NOT_READY, pois faltam composição/ato específico de
preparação e originais frescos. Não há comando para executar agora. Depois
de preparação autorizada e reconciliada, selecionar seu ID/hash original no
pedido UPDATE corrigido U1–U7/U2; após UPDATE verificado, endpoint/Start
diagnóstico permanecem atos separados. Não reabrir novamente a implementação
do utilitário, DATA, staging canônico, registro SCM ou custos opacos do UPDATE.

[Próximo prompt](NEXT-PROTECTED-UTILITY-PREPARATION.md). Autorizações anteriores
continuam utilizadas; sem retry. HEAD/WIP, históricos/pins, EXE anterior,
DLLs/SCM/ProgramData, reservas, UNKNOWN e PST0.6.4 preservados.
E21 PENDING; ED11/Phase6 PARTIAL; OS_QUIESCENCE UNKNOWN; autoridade produtiva DENY.
