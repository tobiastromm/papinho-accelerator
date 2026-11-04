# Referências originais de consulta do controlador — especialização focal

Status: CANDIDATE. Não accepted; sem implementação dependente ou autorização operacional.
Data: 2026-11-02. Escopo exclusivo P1 PERSONAL_LOCAL TRANSPORT_ONLY, sessão diagnóstica fechada.

## Necessidade factual

A observação aprovada c51072d516532ac5b452ec4e4fa8e898697f275bae29010d12bdb069ad4ce036
terminou first=0, consumed=True, complete=True, observer_close_known=True.
Controller19860/criação134381311017119087 e runtime19836/criação134381311235391902
foram correlacionados por referências retidas durante a consulta. As DACLs atuais
do processo e token administrativos permitem BA, SYSTEM e o logon interativo
S-1-5-5-0-566083. Nenhum desses SIDs está no TokenUser/grupos do runtime; seu
logon é S-1-5-5-0-165007955 e SeDebugPrivilege está ausente. Comparação offline
confirmou interseção vazia. Não houve AccessCheck/impersonação/tentativa de abertura
sob o token do serviço; não se afirma NTSTATUS histórico nem estabilidade histórica
das DACLs. A barreira demonstrada é a falta atual de concessão DACL para reabertura.

O código scqb_open_mode exige OpenProcess(0x00101000) e OpenProcessToken(TOKEN_QUERY)
antes de ler o bootstrap. Há outra reabertura em scqr_acquire, depois da leitura.
Corrigir somente o primeiro ponto não fecha a cadeia. A espera original também
exige capture_controller antes de recebê-lo. Os originais administrativos já estão
retidos no controlador; o transporte já duplica três arquivos para o runtime real.

## Alternativas e compatibilidade

A: manter DENY e não executar novas sessões. B recomendada: entregar duas referências
originais somente consulta pelo transporte existente, sem alterar ACLs/privilégios.
Não recomendar Administrators/SeDebug no serviço, impersonação do administrador,
relaxamento de autenticação, ou promoção de PID/carrier/DATA a autoridade.

A decisão accepted de entrega cross-principal, regras2/3/5/6/7, já permite direitos
mínimos read/query e referências controladas; proíbe token impersonável e direitos
administrativos de ação. OHI, SSO-B e QUERY/G3 exigem fonte independente, originais,
autenticação bilateral, consumo e fechamento. São reutilizados integralmente.

A decisão SQB2 de20261101 incorporou exclusivamente 8bytes de deadline e buffer:
PRIVATE-DEADLINE-DELTA.REQUEST.md SHA256
62548c758282dd6b828b451a01a92bc4a369843ac55f7cc9c52f8c84a25ae11c.
Ela preservou SQB1/SQR1/SQC1 e não autoriza interpretar16bytes extras como SQB2.
A especialização indispensável é representação privada versionada, espera QUERY
limitada antes do peer original e fechamento atribuível dessas referências.
Nenhum contrato accepted anterior será reescrito.

## Regras indivisíveis propostas B

Q0. Mesmo owner, ServiceMain, pipe privado, fonte administrativa, E/Q/S e tentativa.
Nenhum serviço, root, canal, conta, concessão ACL ou mecanismo administrativo novo.
Somente implementação/qualificação offline após aprovação desta proposta.
READ/NEW_INSTALL, N0 e todos os demais perfis permanecem inalterados.

Q1. Depois das confirmações e consumo originais e da autenticação nativa do runtime
pelo controlador, duplicar para esse runtime exatamente os dois objetos já retidos:
processo administrativo com PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE=0x00101000;
token primário original com TOKEN_QUERY=0x00000008. Não herdar handles. Sem
DUPLICATE_SAME_ACCESS, PROCESS_DUP_HANDLE no destinatário, TOKEN_DUPLICATE,
TOKEN_IMPERSONATE, TOKEN_ADJUST_*, VM, escrita, término ou direitos administrativos.
O PROCESS_DUP_HANDLE para entrega continua exclusivamente no controlador. Não
reabrir os objetos administrativos por PID para obter uma nova origem.

Q2. Criar somente uma versão privada SQB3: layout de SQB2 preservado, magic SQB3,
seguido de dois identificadores u64 little-endian de handles na tabela destinatária,
na ordem processo/token. Total1100bytes, contra1084deSQB2. Buffer existente cresce
16bytes, sem alocação/canal paralelo. SCQ1, SQB1, SQB2, SQR1 e SQC1 continuam
byte-exatos e rejeitados quando não correspondem à versão/etapa selecionada;
sem downgrade/fallback. SQB3 exige P1 explícito legítimo nas duas pontas. IDs são
localizadores candidatos, nunca ponteiros originais ou aprovação.

Q3. Antes de receber essas referências, a QUERY pode apenas abrir uma vez o pipe
previsto, verificar sua natureza/identidade kernel de servidor, ler um frame
estritamente limitado e aguardar seu evento original, Stop e menor deadline.
Ainda não existe referência original do peer para esperar sua saída: esta
limitação deve ser expressa, sem substituí-la por PID ou declarar peer vivo.
Quebra do pipe, Stop, erro, expiração ou ausência de referências: DENY/UNKNOWN.
Não renovar janela240000ms, duração global60000ms ou freshness100ms. Nenhum
empréstimo admitido, confirmação, receipt ou readiness ocorre nessa fase.

Q4. Validar localmente os objetos recebidos, tipo kernel, GrantedAccess exato,
não herança, largura/valores válidos e ausência de aliases com pipe/evento/arquivos.
Validar processo por PID kernel do pipe, PID/criação originais, imagem, lifetime e
liveness; token por tipo primário, User, Statistics/TokenId/AuthenticationId/ModifiedId,
grups/restrições/elevação/sessão/role administrativa originais. Origem do token:
duplicação do token original pelo controlador legítimo, seguida da correspondência
integral ao plano e à decisão original protegida; igualdade de SID isolada não basta.
Troca de token/processo ou continuidade não demonstrada conserva DENY. Nem o token
QUERY-only nem o frame concedem autoridade. A admissão só ocorre após verificação
independente dos três arquivos, conteúdo/SD/proveniência, seleção P1 e autenticação
bilateral completa. Depois do binding, voltar à espera com referência original do
peer e monitoramento de saída. Não executar OpenProcessToken novamente sob o serviço:
scqr_acquire deve duplicar localmente a referência TOKEN_QUERY já validada, preservando
as mesmas verificações. Revalidação usa referências retidas, nunca novas permissões.

Q5. Reutilizar capture_controller/capture_token e os dois slots de originais do
recipient; não acrescentar outra cópia local persistente. Os dois handles recebidos
substituem duas aberturas locais existentes; a cópia de custódia posterior substitui
a reabertura de token já existente. IDs remotos são bookkeeping no remetente, não
handles locais adicionais. Censo44 inclui esses4slots. Qualificação deve demonstrar
pico completo por participante e falhas parciais, sem pressupor que o censo prova
capacidade nativa. Manter entrada128/guard188/cap192, parcelas44/16/4, memória física,
commit3GiB, privado1994944, working3145728 e demais parâmetros/política vigentes.
Amostragem fresca de origem/destino antes de cada duplicação. Nenhuma expansão de
quota ou fallback; divergência do censo implica STOP e relatório antes de ampliar.
Estado de bookkeeping e buffer+16 devem entrar no censo de memória antes dos builds.

Q6. Custódia única desde cada duplicação, inclusive antes de entrega do frame.
Registrar cada efeito parcial; sem repetição de duplicação nem segundo bootstrap.
Fechar as cópias locais somente após sua última validação necessária, preservando
continuidade até a transição final. Acrescentar versão privada SQC3 no MESMO pipe:
layout SQC1 com magic SQC3 seguido dos mesmos2IDs u64, total1092bytes. Ela comprova
fechamento nativo dos três arquivos e das duas referências de consulta, incluindo
suas cópias locais conhecidas, antes de RUNNING diagnóstico. Nenhuma declaração de
close antecipado para liberar recursos. SQC1 continua exclusivamente seus três
arquivos; SQR1 continua receipt, não close. Rejeitar SQC1 como fechamento completo
de sessão SQB3. Remetente compara os5IDs e a tentativa original; não opera handles
remotos por valores depois de fechados. Provas do sender e receiver e fechamento
do próprio pipe/evento continuam distintos. A ordem exata deve ser qualificada
para não exigir uso de uma referência já fechada nem dispensar validações finais.

Q7. Stop, peer perdido, falha de duplicação/envio/readback/validação/close, namespace
ou tentativa divergente, replay e expiração conservam consumo e retenção UNKNOWN.
Antes de posse local validada, não fechar valores arbitrários recebidos como se
fossem recursos próprios. Duplicação remota parcial sem recepção completa implica
retenção UNKNOWN; sem DUPLICATE_CLOSE_SOURCE por valor remoto, rollback, cleanup,
refund ou inferência de fechamento pela saída do wrapper. Deadlines limitam espera;
não demonstram cancelamento/fechamento nativo. Readiness produtiva sempre DENY.

## Qualificação e gates seguintes

Testar offline ambos os lados em conjunto: direitos exatos e excedentes, tipo errado,
alias/substituição, token divergente, replay, destinatário divergente, consumo duplo,
parcial em cada uma das5duplicações, frame truncado/versão divergente, espera inicial
sem peer, Stop/deadline sem renovação, perda do peer após binding, segunda reabertura
proibida, fechamento das cópias, SQC1 insuficiente, SQC3 divergente/UNKNOWN e readiness
prematura. Quatro builds e regressões afetadas após implementação, sem campanhas
inalteradas. Demonstrar redução de direitos e ausência de impersonação/ajuste ACL.

Riscos: recursos internos opacos, exposição de metadados administrativos via
TOKEN_QUERY restrito, entrega parcial retida, lacuna de detecção de saída do peer
antes da aquisição original (limitada por pipe/Stop/deadline), ausência de recuperação
automática e close nativo inconclusivo. Não são tratados como zero ou suficiência.

Aprovar B autoriza registrar esta especialização e implementar/qualificar offline.
Não autoriza UPDATE, nova sessão, coleta, disposição dos processos19860/19836,
Start/Stop, SCM/ACL, CA/CURRENT, rede, TPM/NV/Q4 ou limpeza/refund. Processos atuais
continuam retidos; sua disposição exige decisão específica G5. Histórico de timeout,
bilateral, application close, retenção histórica e quiescência OS seguem UNKNOWN.
Depois: código/testes completos, uma imagem congelada, UPDATE governado U1–U7/U2
sob autoridade operacional própria e nova sessão E/Q/S independente. Preservar PST0.6.4.
