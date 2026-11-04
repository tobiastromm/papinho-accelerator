# UPDATE manual — falha no primeiro rename, retenção UNKNOWN

Fonte: transcrição completa fornecida pelo proprietário, preservada em
OWNER-MANUAL-UPDATE-OUTPUT.txt, SHA256
4a7e4dbc5e6022a1c3978c89f6aaa14c667258b1c2ec395085107928c2f9dfd4.
Transcrição não é atestação independente/assinada do host nem nova consulta
do filesystem. Pedido557ad499…e744e e script60834ab0…4118c aparecem conferidos
no comando, sem erro de pin reportado. Confirmação nativa atribuída mostrada e
respondida pelo proprietário. Autorização UTILIZADA/CONSUMIDA, sem retry.

## Primeira causa demonstrada

first=3221225539 = NTSTATUS0xC0000043, STATUS_SHARING_VIOLATION.
Definição conferida no SDK local Windows Kits10/Include/10.0.19041.0/shared/
ntstatus.h, linha2619. Não é erro ACL5 nem insuficiência de handles demonstrada.
location701 é definida por staged_update_rename como700+bit1: primeira
NtSetInformationFile FileRenameInformation, antigo→leaf de retenção, sem overwrite.
Não é falha do segundo rename ou da cópia inicial.

A chamada encontrou conflito de compartilhamento. A saída não identifica o
handle/processo conflitante, o objeto específico envolvido na resolução do
rename ou se a referência é própria/emprestada/externa. Não atribuir a antivírus,
Explorer, serviço ou ACL sem evidência. native_predicate/kind/error0 não anula
o status NTSTATUS: são outros campos de diagnóstico, não causa alternativa.

## Disposição factual

prepared1, consumed1, submitted189, created1, renames0, close_known0;
canonical_verified_closed0; AUTHORITY DENY; exit2.
Wrapper INVOKED=True, WRAPPER_CLOSE_UNCERTAIN=False. Isso confirma apenas o
fechamento das referências do wrapper, não o cohort nativo ou quiescência OS.

Candidato criado: volume417614100/high1769472/low537456,758272bytes.
Leaf do plano:
papacc_server.742d832576fb64525de6d3db57eedc5eb457c8fc31990f7f745dd19243d0f10b.candidate.
Leaf de retenção planejado, NÃO confirmado como existente:
papacc_server.742d832576fb64525de6d3db57eedc5eb457c8fc31990f7f745dd19243d0f10b.previous.

Nenhum rename confirmado. Pelo caminho qualificado, cópia/hash/flush e
revalidações do candidato precedem a primeira chamada de rename; a execução
atingiu essa chamada. Isso não demonstra readback de novo canônico. O EXE
antigo tinha ID417614100:6225920:71237/hashc0f27f94…806ecf no plano; não declarar
nova identidade/presença atual do canônico ou do .previous sem readback posterior.
Não foi emitido LIMITED_UPDATE_VERIFIED_CLOSED.

Retenção é deliberada no owner: staged_update_close, ao detectar effects&&first,
vai para retained sem tentar fechar o cohort como saudável, mantendo fases8/
unresolved. Portanto close_known0 não é prova de que CloseHandle falhou nem
licença para liberar/repetir. Processo encerrado não comprova quiescência OS
ou crédito reutilizável. Sem rollback, exclusão, completar renames ou refund.

## Recursos e preservação

191 amostras: pico observado176 handles, privado4145152bytes, working set10776576;
mínimos físico6466834432/commit8902426624bytes. K2027314, R761856.
Nenhuma amostra insuficiente nesses pisos reportada. Observações não são
ceilings/reservas, não importam limites de outro domínio e não explicam o
sharing violation. Custos opacos permanecem UNKNOWN.

Executor protegido original417614100:1507328:540527/PID12060/criação reportados,
hash6c0e7717…0bb87; DATA7612d111…1cf7f, DLLs idênticas e configuração fechada
mostrados no plano. Não houve comando autorizado de Start/SCM mutante.
Estado SCM posterior não foi reconsultado nesta reconciliação; último gate
nativo alcançado verificou STOPPED conforme caminho, não prova universal atual.

## Próximo trabalho focal

Investigar OFFLINE a semântica da primeira operação de rename e compartilhamento:
handle do antigo com DELETE, parent/root de rename, opens/readbacks próprios,
locks do wrapper, lifetimes e semântica Win32/NT das referências retidas.
Distinguir erro funcional demonstrável de possível referência externa. Não
relaxar FILE_SHARE_DELETE/guards indiscriminadamente, nem encerrar processo/
fechar referência desconhecida para obter PASS. Reusar contratos U1–U7/U2.

Se faltar fato, identificar consulta read-only mínima específica e sua
autorização, sem tracing/coleta automática ou retry. Se necessário qualificar
correção offline, usar APIs mutantes simuladas e manter cohort UNKNOWN. Nenhuma
nova autorização de UPDATE está pronta nesta reconciliação.

E21 PENDING; ED11/Phase6 PARTIAL; autoridade produtiva DENY; OS_QUIESCENCE UNKNOWN.
Preservar candidato, EXE anterior, DLLs, configuração, dados, reservas, pins,
histórico e PST0.6.4. Nenhuma operação protegida executada pelo agente.
