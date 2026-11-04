# Preparação da disposição WS3M — gate anterior ao UPDATE SQB3

Estado: READY_FOR_EXACT_OWNER_DECISION; não autorizado/não executado.
Pedido ONE-WS3M-PARTICIPANTS-DISPOSITION.REQUEST.CANDIDATE.md,
SHA256 ffd24d8f4bdcf272b819b61c43c352bff467d1a7caeaf07f8e2e528873357bfe.

Reutilizado o adaptador transitório de dois participantes já qualificado, apenas
remapeando os nomes e as constantes desta sessão. Não há novo owner, serviço,
canal, root, updater ou mecanismo permanente. U1–U7/U2 e SQB3 não foram alterados.

Alvos exclusivos: runtime19836/criação134381311235391902; controlador19860/
criação134381311017119087. SID, estatísticas de token e imagem antiga798720bytes/
ID417614100:4128768:327703/pin6ad6e378… estão fixados. Não usar o candidato804352
como imagem desses participantes. São os últimos fatos observados, não liveness
atual: o procedimento exige revalidação dos mesmos objetos antes de efeitos.

Sequência solicitada: validar tudo/configuração/zero ações de recuperação/SCM
START_PENDING do runtime fixo; confirmação manual; consumo; até1TerminateProcess
no runtime; observar saída exata e SCM STOPPED/PID0; revalidar controlador e imagem;
até1TerminateProcess no controlador; saída/SCM/close do adaptador. Não procurar
substitutos, não enviar Stop nem perseguir outro PID. Qualquer falha impede os
próximos efeitos; sem retry, rollback ou limpeza.

G5 exige decisão posterior específica e declara que não existe autoridade de
terminar processo implícita. “Siga” nesta preparação não foi registrado como tal
aprovação. Arquivo OWNER-WS3M-PARTICIPANTS-DISPOSITION-APPROVAL.txt ainda ausente.
O comando com o hash real dessa decisão será preparado somente depois de recebida.

Qualificação offline: fonte real compilada no PowerShell5.1 sem invocar RunFixed;
modelo sem DllImport/extern,52cenários PASS/17fronteiras nativas simuladas;
wrapper AST PASS/uma invocação futura/sem resposta automática. Core real e modelo
comparados integralmente, diferença apenas da confirmação simulada. Pins e logs
em WS3M-DISPOSITION-OFFLINE-QUALIFICATION.json e ws3m-disposition-offline.log.
Nenhum build/teste de produto repetido; nenhuma consulta nativa ou mutação real.

Término é abrupto, não SCQ close. Pode haver parcialidade, timeout, atualização
SCM assíncrona ou fechamento inconclusivo. Reserva, application close, bilateral,
retenção histórica e quiescência OS permanecem UNKNOWN, mesmo após eventual saída.

Após autorização e execução MANUAL, reconciliar a saída integral, sem repetir.
Somente então avaliar o pedido UPDATE SQB3 já preparado, sob autorização separada,
e depois nova sessão E/Q/S. Não interpretar aprovação da disposição como UPDATE.
Preservar HEAD/WIP/pins/EXEs/autorizações consumidas/PST0.6.4/DENY/UNKNOWN.
