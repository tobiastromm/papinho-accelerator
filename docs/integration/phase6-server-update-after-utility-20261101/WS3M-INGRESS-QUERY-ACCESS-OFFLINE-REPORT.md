# WS3M — fronteira de acesso do ingresso antes do primeiro pacote

## Estado reconciliado

Consulta manual autorizada aba0cba004b31a231c2b3ac68c4ca1431300d95b84be86f13e609203141e0525 utilizada: first0, queryex/qc/qsidtype exit0. SCM START_PENDING/OWN_PROCESS/PID19836/win320/specific0/checkpoint0/hint0; config DEMAND_START/ERROR_NORMAL/virtual/UNRESTRICTED e comando esperados. Controlador19860/criação134381311017119087 atribuído por criação e vivo. Isso não é lease, token original do runtime ou prova de retenção interna. Não repetir consulta ou sessão. Não houve Stop/kill/UPDATE.

Timeout1460 em wait step5/stage212/start_submission_phase1 foi demonstrado pela continuação da sessão. O wait espera evento de transporte ou término do runtime com o prazo original restante; o progress tem vários retornos ERROR_IO_PENDING. Ainda não há evidência de qual operação pendente ou qual primeiro erro do receptor. START_PENDING não implica RUNNING/prontidão nem que o processo esteja progredindo.

## Auditoria focal do código instalado

server_service_win32.c ps_main mantém START_PENDING até prontidão/close próprios; o ramo retido pode permanecer vivo deliberadamente. server_launcher_owner_win32.c executa aquisição da root/carrier e loop de ingress, e conserva retenção quando falha/fechamento incerto. Não é prova de vazamento ou motivo para aumentar deadline.

server_configuration_personal_native_win32.inc papacc_configuration_personal_scq_ingress_continue: runtime capture -> policy G3 local -> recursos QUERY -> scqb_open_query -> primeira leitura -> candidatos -> validação dos originais -> bilateral/receipt-close. Sem sucesso dessas aquisições, não se promove readiness.

personal_scq_bootstrap_win32.inc scqb_open_mode exige, em sequência: CreateEvent, CreateFile no endpoint privado, OpenProcess do controlador com QUERY_LIMITED|SYNCHRONIZE, OpenProcessToken do controlador com TOKEN_QUERY, captura de estatísticas e scqb_current, antes de scqb_read. Os modelos substituem essas funções; não provam direitos nativos entre os principals reais. Uma falha após abrir o pipe e antes de ler o pacote pode explicar silêncio/timeout, mas não foi observada diretamente nesta sessão. Não declarar essa hipótese causa histórica.

Referências primárias: https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openprocesstoken explicita DACL/direitos na abertura do token; https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-getkernelobjectsecurity explica consulta owner/DACL com READ_CONTROL. O vínculo do locator/endpoint não substitui essas permissões, originalidade ou OHI.

## Menor evidência preparada

ONE-WS3M-CONTROLLER-QUERY-ACCESS-READONLY-REQUEST.CANDIDATE.md, SHA256c51072d516532ac5b452ec4e4fa8e898697f275bae29010d12bdb069ad4ce036. Mesmo observador V3 corrigido, alvos atuais fixos19860/19836, mais GetKernelObjectSecurity OWNER|DACL do processo/token do controlador. Só ao observador administrativo é solicitado READ_CONTROL nesses dois objetos. Runtime não recebe direito/token/impersonação. Captura atributos/grupos/privs do token virtual atual para confronto com os dois descritores; sem AccessCheck sob token duplicado, nenhuma leitura de memória e nenhum endpoint. Não abrir objetos como o runtime ou modificar DACL para testar hipótese.

Fonte real somente compilada, RunFixed não chamado.50cenários,20capacidades escalares,1alinhamento,3preservações de erro/14APIs modeladas PASS; novos negativos SD queryfailure, lengthoverflow e formato inválido. AST wrapper PASS. Sem build/testes do produto repetidos, sem alteração da imagem instalada ou guards. Ver WS3M-CONTROLLER-QUERY-ACCESS-OFFLINE-QUALIFICATION.json.

Primeiro gate: autorização específica dessa nova observação, seguida de execução manual única. Não usar autorização de consulta já consumida. A observação será prospectiva e pode demonstrar incompatibilidade atual, não reproduzir o retorno original interno. Se houver allowACE, isso também não comprova acesso completo, MIC/privs/restrições/continuidade ou a etapa histórica. UNKNOWN preservado onde faltar evidência.

Sem correção funcional ou normativa aplicada nesta entrega: falta evidência para escolher mudança de aquisição/protocolo. Não relaxar autenticação, remover consultas, habilitar Debug/impersonação, alterar ACL ou introduzir autoridade por DATA. G0–G9/OHI/SSO-B/TO0–TO9/P0–P7/G3/PST0.6.4, reservas e retidos preservados. Bilateral/receipt-close/OS/histórico UNKNOWN; produção/KSP/CA/CURRENT/rede/TPM/NV/Q4 DENY.
