# P1 — aquisição nativa do descritor na referência original

2026-11-02, America/Sao_Paulo. main/HEAD
f341985924d29870d8ff627ed37c8a0874ad6469; WIP preservado.
IMPLEMENTED_OFFLINE / QUALIFIED / UPDATE_NOT_AUTHORIZED / NOT_EXECUTED.

## Conclusão focal

Há uma alternativa técnica compatível para a aquisição do descritor no P1:
NtQuerySecurityObject sobre a MESMA referência original, sem lookup por nome,
reabertura, duplicação ou novos direitos. A documentação Microsoft descreve
explicitamente uso Nt em user mode, self-relative SD e READ_CONTROL para owner/
DACL. Não há garantia pública de redução de handles/latência/custos internos.
Foi integrada nas duas pontas e na revalidação da cadeia, preservando políticas.

Não foi provado que GetSecurityInfo, sozinho, criou os oito handles. A captura
continua delimitando GetSecurityInfo/IsValidSecurityDescriptor/query extra no
intervalo3/4. Não atribuir a COM/RPC/cache, fechar internos ou aumentar quota.
O console antigo teve fechamento apenas relatado e autorizado; nativeClose0,
retenção histórica e OS UNKNOWN permanecem. Nenhuma nova observação executada.

## Alternativas avaliadas

| Alternativa | Resultado |
|---|---|
| Manter GetSecurityInfo e fechar oito handles internos | Rejeitada: ownership individual não demonstrado; nenhum HANDLE adicional entregue ao caller |
| Warm-up antes da guarda ou descontar internos | Rejeitada: não elimina recursos; contornaria124/128/G3/P5 |
| GetFileSecurity por string | Rejeitada: muda aquisição para nome e rompe objetivo de mesma referência retida |
| GetKernelObjectSecurity do binder PSR existente | Não transportado: uso local LAB não é equivalência normativa; tabela low-level da Microsoft distingue file APIs. Não importar buffer1024/quota/máscaraGROUP do LAB |
| NtQuerySecurityObject no handle existente | Escolhida como adaptação técnica focal; API documentada, mesma máscara, direitos e custódia; redução nativa ainda UNKNOWN |

Sem driver, novo owner, utilitário, serviço, root, canal ou autoridade. Resolução
GetModuleHandleW/GetProcAddress segue o adaptador NtCreateFile existente: usa
módulo ntdll emprestado, sem LoadLibrary/FreeLibrary ou fechamento de handle interno.
Não realizar syscall por ordinal/número ou fallback permissivo. Export ausente nega.

## Compatibilidade normativa e representação

P2/P4 exigem objetos originais, metadados/owner/DACL/ancestrais/continuidade, não
congelam GetSecurityInfo como única API para esses objetos P1. P5 preserva ordem
e contabilização; P6 Stop/negação/fechamento factual/UNKNOWN. OHI continua
exigindo originais e proveniência legítimos; nenhum SD válido substitui essa
origem. G3/G4/QUERY/SSO-B/SCQ mantêm os gates e a autenticação originais.

Não há dispensa de uma obrigação accepted nem mudança de autoridade: adaptação
Win32 privada, sem nova ADR. A auditoria RAW1 de outro perfil não autoriza troca
genérica; esta implementação não altera aquele perfil/slot8, CA, READ ou NEW_INSTALL.
Não normaliza SDDL/ACE/owner nem substitui guardas por teste estrutural genérico.

A máscara continua OWNER_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION(5),
sem GROUP/SACL, novos privilégios, WRITE_DAC/WRITE_OWNER ou acesso adicional.
Os bytes self-relative retornados são usados literalmente nos hashes/compare
existentes. O helper não reconstrói nem reordena SD/ACEs, não altera preimages
P3, SCQ1/SQB1/SQB2 ou receipts/close. Layouts de saída de APIs distintas não
são presumidos byte-idênticos: todos os sites correspondentes P1 usam o mesmo
adapter nas duas pontas. Mudança/conflito de bytes ou descritor revalidado nega;
nunca comparação apenas semântica para obter PASS ou fallback ao snapshot antigo.

## Callers integrados efetivamente

- personal_scq_cold_acquisition_win32.inc/scqc_descriptor: modo selected-object
  READY/role1/thread original/AMD64, somente entrada P1 TRANSPORT_ONLY existente.
  Stage8 preserva NTSTATUS nativo de SD separadamente do erro Win32.
- personal_scq_image_chain_win32.inc/scqi_chain_open e scqi_chain_current:
  aquisição e revalidação dos mesmos diretórios/imagem, somente cadeia selected
  construída pelo capture TRANSPORT_ONLY existente. Roles1/2 preservados.
- personal_scq_inputs_win32.inc/personal_scq_inputs_check: captura/revalidação
  request/decisão/imagem quando a cadeia selected TRANSPORT_ONLY está admitida.
  Chamado pelo controlador e receptor original do ServiceMain existentes.
- server_launcher_scq_cold_start_win32.inc: imprime SCQ_COLD_NATIVE_SD com
  calls/status/needed/capacity, snapshots sem consulta adicional/segredos.

READ, NEW_INSTALL, BINARY_ONLY e demais caminhos continuam GetSecurityInfo e
validadores anteriores. A entrada diagnóstica antiga não instala estado selected
e continua seu percurso antigo: NÃO reutilizar seu script consumido para testar
este novo adapter. Não há observador novo ou autorização de coleta preparada.

## Aquisição, recursos, Stop e retenção

personal_scq_native_sd_win32.inc: no máximo UMA consulta de tamanho e UMA
consulta de preenchimento, ambas na MESMA referência; sem retry se tamanho
mudar. Sizing só aceita STATUS_BUFFER_TOO_SMALL/length válido. LocalAlloc
zero-initialized do tamanho exato, ownership transferido imediatamente ao slot
checking/scratch/sd existente. Retorno/falha/Stop após alocação mantém o slot
rastreável; nunca perde ponteiro, libera internals ou rearma tentativa.

Tamanho20..65536bytes, limite65536 já presente na cadeia SCQ e documentado para
SD em NTFS, não número importado do LAB/TPM/Browser. Outros tamanhos/filesystems
sem suporte demonstrado negam, não ampliam buffer arbitrariamente. Não é teto
dos recursos nativos. Rejeita comprimento retornado incoerente, offsets/SID/ACL
fora do buffer, forma absoluta/SD inválido; depois executa políticas estritas
existentes de owner/DACL/ancestrais e comparação literal. SD desconhecido não PASS.

Privados de diagnóstico acrescentam3*16=48bytes aos tipos do owner; sem wire ou
formato persistido. Censo atual KNOWN2002080bytes/limite3145728 (ambos Selected
3MiB; não mudar quota). Buffer request alocado<=65536bytes, sem novo HANDLE de
arquivo próprio. Até9SDs da cadeia+3originais+1temporário=13buffers simultâneos
por packet SCQ ativo, limite de requests851968bytes. scratch e checking são
mutuamente exclusivos na sequência normal (chain_current precede o loop de
checking); falha/retained bloqueia novas aquisições. Cold pre-plan usa um SD
temporário por vez, liberado antes do próximo original.

Soma conservadora do root medido+requests máximos desse packet=2854048bytes;
não inclui overhead de alocador/CRT/kernel/provider e outros opacos, não é prova
de capacidade física/commit nem reserva universal. Não subtrair esse orçamento
de counts ou transformar o censo em readiness. G3 growth1994944 privado/WS,
pisos7239172096/9823326208,freshness100ms,janela240000/global60000 sem renovação,
guard124/cap128 continuam iguais. Amostras reais e margem podem negar.

O fluxo continua retendo originais até disposição factual. LocalFree/CloseHandle
apenas dos slots próprios pelos helpers existentes. Status pendente/erro/cleanup
incerto preserva UNKNOWN; não há recuperação, delete, refund, rollback ou retry.
A correção separada do aborto do observador não é licença de restaurar coortes.

## Qualificação

| Configuração | Build server/targets afetados | CTest afetados |
|---|---|---|
| Selected Debug | PASS, zero warnings | 9/9 PASS |
| Selected Release | PASS, zero warnings | 9/9 PASS |
| Legacy Debug | PASS, zero warnings | 9/9 PASS |
| Legacy Release | PASS, zero warnings | 9/9 PASS |

36pares. Novo helper31casos/94checks, todas as consultas nativas modeladas:
mesmo handle/máscara, export ausente, sizing errado, acesso negado, pending,
tamanho0/pequeno/65537, falha de alocação, Stop por fase, tamanho alterado entre
queries, SD truncado/offset/ACL inválido e slot preexistente. Sem fallback/retry.
Cold-open104casos/838checks inclui fonte selected inválida e export ausente
sem GetSecurityInfofallback. Inputs/cadeiaP1/controller/entry/censo e negativos
READ/NEW_INSTALL/modelo de owner da imagem requalificados; fixtures simulam
NtQuerySecurityObject, sem consultas a arquivos reais/tokens/SCM/endpoint/Start.

Logs p1-native-sd-*-build*.log/full-*-build.log e *-tests.log. Warnings de teste
por shadowing foram corrigidos, não dispensados. PDBs com C1033 preservados
sob before-native-sd e regenerados somente em build; nenhuma troca de flags/
toolchain, limpeza operacional ou causa do erro de PDB afirmada sem prova.

## Artefato final e caminho direto

Selected/Release781312bytes, SHA256
dc4855c4644cc0255e86aeeea3b5799ae15c4741cf8468cd00a489bb63b43076.
DLLs idênticas aos pins históricos; não recompiladas/substituídas no instalado.
Observador antigo779776/7b6d...e726 preservado como arquivo histórico workspace
papacc_server.interval-observer-779776.exe. Não executar/reutilizar seu ato.
Instalado continua EXPECTATIVA histórica774656/ID417614100:1638400:813992/hash981658...0220;
nenhuma consulta fresca feita ou continuidade de coorte presumida.

ONE-P1-NATIVE-SD-UPDATE-REQUEST.CANDIDATE.md/hash
599e1b4c4cb986b2eaf5dcfd08d7aab73abffd41c4f745bf4ad3074f756d3b6e
prepara uma única atualização de arquivo pelo updater protegido104448/084aa...822a
existente, sem novo utilitário/infraestrutura. Script6bdd439d...24b7 reutiliza
seis helpers byte-exatos, mesmos16argumentos; apenas pins/IDs/bytes/banner/pedido
atualizados; AST sem erros, não executado. U4/K2027314 do executor antigo não
é o K do servidor novo ou de updater recompilado não selecionado.

Primeiro gate: seleção/autorização específica do UPDATE candidato. Nenhum comando
operacional emitido ou executado aqui. Depois de UPDATE reconciliado, preparar
nova sessão E/Q/S independente, com novos originais legítimos e guards frescos.
Essa é a prova futura de admissão/identidade/bilateral/receipt/close; não exigir
PID/token futuro agora. Economia real de handles, prontidão diagnóstica e
produtiva permanecem não demonstradas; não marcar o Start como pronto/PASS.

## Fontes primárias

- https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-ntquerysecurityobject
- https://learn.microsoft.com/en-us/windows/win32/api/aclapi/nf-aclapi-getsecurityinfo
- https://learn.microsoft.com/en-us/windows/win32/secauthz/low-level-security-descriptor-functions

Sem coleta/UPDATE/endpoint/SCQ/Start/Stop/SCM/ACL/CA/CURRENT/rede/TPM/NV/Q4/
limpeza/refund/recuperação. HEAD/WIP/históricos/pins/EXEs/atos consumidos,
PST0.6.4,DENY/UNKNOWN/retidos e quiescência OS desconhecida preservados.
