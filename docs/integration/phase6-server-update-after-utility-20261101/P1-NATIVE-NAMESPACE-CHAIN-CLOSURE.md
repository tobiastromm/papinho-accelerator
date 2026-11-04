# P1 — cadeia nativa fechada offline

Estado: integração/qualificação offline concluídas; operação DENY até atos
específicos futuros. main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469.
Sem nova coleta, UPDATE, endpoint/SCQ/Start/Stop ou consulta SCM/ACL.

## Integração funcional

O resolver antes integrado em scqc_selected_name foi extraído, sem duplicação
de mecanismo, para personal_scq_native_name_win32.inc (scq_p1_native_name).
É privado ao mesmo owner e compartilhado por:

- scqc_open_raw, via scqc_selected_name: pedido, decisão e imagem originais do
  controlador P1 SELECTED_OBJECT/role1/AMD64; demais modos permanecem anteriores.
- scqi_chain_capture: somente SELECTED_OBJECT/TRANSPORT_ONLY/AMD64, roles1 e2
  já previstos em P0–P7 para controlador e receptor. Resolve `C:\` para nome
  nativo direto da âncora; NEW/BINARY_ONLY preservam representação anterior.

Não há novo owner/serviço/root/protocolo/canal/grant/CLI. Não há novo contrato
ou necessidade de especialização normativa: P0–P7 e OHI mantêm seleção legítima,
originais, identidade/proteção/continuidade; não prescrevem manter a indireção
DOS dentro do nome passado à NtCreateFile. Accepted não foram alterados.

QueryDosDeviceW obtém a primeira string (mapping atual) como LOCALIZAÇÃO DATA;
valida limites/NUL, prefixo e componente nativo direto; nega SUBST/alias recursivo,
target com subcaminho, mapping vazio/malformado/overflow e componentes dot/dotdot.
`\Device\` por si só NÃO admite objeto. NtCreateFile, tipo/ID/SD, seleção e
binding do plano constituem as provas independentes posteriores.

Para a âncora: RootDirectoryNULL, nome `\Device\<mapping-atual>\`, direitos
FILE_READ_ATTRIBUTES|FILE_TRAVERSE|READ_CONTROL|SYNCHRONIZE, shareREAD, FILE_OPEN,
options00200021 e OBJ_CASE_INSENSITIVE|OBJ_DONT_REPARSE preservados.
Para cada filho: parent original retido e nome unitário relativo. A imagem é
adquirida pelo último parent, com direitos/opções de arquivo já existentes.
FILE_OPEN_REPARSE_POINT e DONT_REPARSE preservados; reparse não é seguido.
Não há fallback em falha de mapping, aquisição, identidade ou proteção.

Continuam obrigatórios: validação independente do volume/ProgramFiles/parent/
EXE e relação entre eles; ausência de reparse/tipo correto; owner/DACL efetivos;
IDs selecionados/proveniência; target digest imutável com request/decisão/
operador/tentativa/window; conteúdo/pins e correspondência processo/imagem.
As guardas estritas de request/decisão/READ permanecem; owner Administrators
continua restrito ao domínio de imagem TRANSPORT_ONLY já accepted.

## Continuidade, namespace divergente e lifetime

O mapping não é referência original. Resolver só localiza candidatos; a cadeia
adquire/retém objetos e valida SD/IDs antes de qualquer admissão operacional.
Novo mapping para outro volume/objeto não passa apenas por possuir prefixo
`\Device\`: comparação de identidades/seleção/imagem/binding nega.
Reparse filesystem, SD inadequado ou substituição de objeto também negam.

Após captura válida, scqi_chain_current revalida as MESMAS referências retidas,
não redescobre o caminho. Alterar uma string de mapping posteriormente não
transforma o objeto original em outro e não obriga abandonar continuidade.
Isso preserva P0–P7: objeto instalado selecionado é distinto do mapping atual
ProgramFiles. Não prometer mapping imutável, não substituir parent por string,
e não confundir namespace divergente na aquisição com perda fictícia de uma
referência original que permanece válida. Replay/recapture de cadeia não vazia
continua INVALID_STATE; mudança no plano/proveniência/IDs invalida seu binding.

Guard124 ocorre em cada scqi_chain_open DEPOIS da resolução; máximo128/G3 não
mudam. API/caches/internals não são excluídos da contagem. Não fechar handles
internos ou originais para obter PASS. Buffer temporário de nome da âncora
cresce de8 para264 WCHAR (+512bytes); drive_root4WCHAR e resolver com buffer
264WCHAR são automáticos, não tetos físicos/commit demonstrados. Struct/packet
persistente não cresceu. Margens/picos/latência nativos continuam UNKNOWN.

Stop/window original é verificado em cada aquisição e revalidação, sem renovar
deadlines. Falha/pending/handle inesperado/fechamento incerto preservam phase4,
retenção/UNKNOWN e impedem reutilização. Nenhum efeito ou consumo é fabricado.
Sem fonte/atos/gates válidos continua DENY; mapping/carrier/ack não dão autoridade.

## Auditoria restrita das aberturas necessárias P1

| Abertura | Caminho final e proteção |
|---|---|
| Pedido, decisão, EXE frio | scqc_open_raw: resolver compartilhado no P1, DONT_REPARSE, rights/share originais; posterior OHI |
| Âncora cadeia da imagem, duas pontas | scqi_chain_capture -> resolver compartilhado -> scqi_chain_open; objeto independente validado |
| ProgramFiles/Papinho/Accelerator | nomes unitários relativos aos parents originais retidos, scqi_chain_open/DONT_REPARSE |
| Imagem da cadeia | leaf papacc_server.exe relativo ao parent instalado retido, ID comparado com original/plano |
| Readers de conteúdo | ReOpenFile dos originais, identidade/SD/pins revalidados; não nova resolução por nome DOS |
| Bootstrap e transporte privados | CreateFileW do pipe privado com prefixo/formato fechados, SQB2/SSO-B/autenticação originais; não são caminhos de arquivo C: |

Não encontrada outra abertura de ARQUIVO no percurso P1 que permaneça
dependente de `\??\C:`. A representação antiga permanece deliberadamente nos
outros modos, fora do escopo. Não foi feita auditoria genérica do produto,
updater/staging/SCM ou alteração dos mecanismos de transporte.

## Qualificação afetada

MSVC18/toolchain existente; Selected/Legacy Debug/Release:4builds PASS, /W4 /WX,
zero warnings finais. 15CTests/configuração:60pares PASS. Logs
p1-chain-namespace-<perfil>-<config>-build.log e -tests.log.

P0_P7_EXISTING_TARGET ampliado17->29cenários,164checks: root nativo exato,
controle/receptor, mapping ausente/alias/subpath/overflow/não terminado/vazio/
dotdot, mapping para outro volume, reparse C000050B ainda negado, Stop durante
resolução antes do open, primeira string atual versus mappings anteriores,
replay, substituição/continuidade/proveniência/owner/DACL, guard/falha de query,
retenção e closeUNKNOWN. QueryDosDevice/NtCreateFile modelados.
Raw-open64cenários/240checks permanecem PASS com o MESMO resolver compartilhado.
Regressões READ/NEW_INSTALL/guards comuns/presenter/SCQ/receipt/close preservadas.
Nenhuma consulta nativa de mapping/arquivo/SCM/endpoint foi executada pelos novos
testes. Modelos não provam estado atual do host ou autenticação bilateral real.

## Delta final qualificado e primeiro gate

Workspace Selected/Release papacc_server.exe: **774656bytes**,
SHA256 **9816585b3d240ce59da70769245204378f8e08e67c67f5d5a0b16b729b8d0220**.
Comparado ao artefato offline anterior774144: +512bytes. Comparado à última
imagem instalada reconciliada772608: +2048bytes. NÃO foi instalado/selecionado
operacionalmente por este relatório. Não reconstruir silenciosamente após pins.

Última imagem instalada reconciliada, sem consulta fresca nesta tarefa:
772608bytes,ID417614100:1769472:132345,
SHA25654d374b2fddac81e30b4f8bb574fbaf3446b5a52cbdb002769fe5cdcfe1b4a50.
DLLs workspace verificadas idênticas aos pins originais:
- libssl-3-x64.dll1321472bytes:3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed
- libcrypto-3-x64.dll7217664bytes:09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044
Somente EXE precisa de UPDATE funcional governado U1–U7/U2 existente; não DLLs.
Configuração SCM/EXEs anteriores/objetos retidos preservados.

Primeira lacuna FACTUAL: sucesso da aquisição e da cadeia corrigida no contexto
nativo atual, recursos G3 e SCMSTOPPED fresco ainda não demonstrados. Não impor
nova coleta preliminar redundante: preparar pedido UPDATE existente com checks
originais/gates frescos e autorização específica, depois nova sessão independente
E/Q/S. A correção é funcional, não exige instalar imagem apenas para instrumentar.
Não garantir aprovação124/128 ou sucesso de Start; causas históricas/retidos/OS
continuam UNKNOWN. O próximo prompt é NEXT-P1-CLOSED-CHAIN-IMAGE-UPDATE.md.

Não executar UPDATE/observação/endpoint/SCQ/Start/Stop/SCM/ACL/CA/CURRENT/rede/
TPM/NV/Q4/cleanup/refund/recuperação. HEAD/WIP/históricos/pins/EXEs/originais,
atos consumidos/reservas/PST0.6.4 preservados. Nenhum retry autorizado.
