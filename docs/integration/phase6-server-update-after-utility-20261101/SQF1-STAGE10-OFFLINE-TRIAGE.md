# SQF1 recebido — etapa10 / Win325

Evidência exata OWNER-SQF1-SESSION-OUTPUT.txt e SQF1-SESSION-RESULT.json.
Duas cópias/close conhecidos, três confirmações e Start submetido; SQF1 retornou
stage10/win325/kind0/raw0. Controlador rejeitou com5/212/retained1, pending0.
Não foi novo timeout1460 neste registro: houve retorno negativo do peer.
Não comprova ACL inadequada, qual arquivo falhou, etapa interna ou NTSTATUS.

## Retornos do código que permanecem possíveis
personal_scq_inputs_contents começa validando estado/plan/check; falha desse check
inicial é convertida para5023, portanto não basta citá-lo para explicar5.
Antes do hashing de conteúdo, abre os três readers por ReOpenFile. Há:
- subetapa31: GetProcessHandleCount falha e propaga GetLastError original;
- subetapa15: ReOpenFile retorna INVALID_HANDLE_VALUE e propaga GetLastError original;
- subetapa24: revalidação FINAL dos inputs ou deadline falha e sintetiza5.
São três caminhos que podem produzir o5 observado. Falhas de CNG/leitura/hash
nessa função são mapeadas a13/6/1223 etc.; não atribuir5 ao BCrypt sem prova.
O código externo10 não contém c->first, índice do arquivo, status da cadeia/SD ou
qual predicado final falhou. Não inferir que hashing começou: readers antecedem CNG.

Documentação Microsoft ReOpenFile descreve nova abertura do objeto com access/
share/flags e restrições de compartilhamento:
https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-reopenfile
Isso NÃO demonstra que a chamada falhou nesta sessão, nem prova DACL/causa histórica.
Não trocar ReOpenFile por DuplicateHandle sem confrontar cursor/lifetime: o código
exige readers de conteúdo independentes e referências originais retidas.

Primeira lacuna exata: subetapa original c->first e, se15, índice original do reader;
ou, se24, qual revalidação/deadline. Nenhuma coleta nativa adicional executada.
Sem defeito funcional novo demonstrado, nenhum código de produto alterado e nenhum
build/teste inalterado repetido nesta reconciliação. Não relaxar SD/rights/deadline.

Controlador20104/criação134381442604810139 identificado no plano.
Runtime PID não impresso e SCM atual não consultado. Não presumir término;
autorização da sessão consumida. Disposição antiga não atinge estes participantes.
Manter aplicação/retidos/reservas/OS UNKNOWN, objetos/pins/PST0.6.4/DENY.
