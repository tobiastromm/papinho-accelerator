# Continuação focal — atribuição de ownership no intervalo109→132

Continuar PACC-PHASE6-STAGING-HANDLE-OWNERSHIP-AND-REDUCTION.
Ler CLOSEOUT.md e o resultado BINARY_ONLY anterior. Não repetir contador agregado.

Pesquisar somente evidências já existentes de criação/fechamento de handles no processo observado, entre query pós-frame e sample7; conferir processo/criação, objetos, origem de aquisição, direitos, herança e lifetime. Não inferir ownership de nome/tipo de objeto ou import de DLL. Não executar produto/coleta, tracing ou operação protegida.

Se existir evidência suficiente, implementar apenas a redução cuja propriedade e liberação segura estejam demonstradas; qualificar offline builds Selected/Legacy Debug/Release e regressões afetadas, mantendo124/128/1024 e originais. Se não existir, preparar especificação focal de uma prova de atribuição, não outra contagem: explicar exatamente como observar criação/fechamento e origem nesse intervalo, overhead do observador, separação de participantes e respeito ao STOP. Não alegar instrumento nativo qualificado sem implementação/evidência. Qualquer execução exige autorização separada; não emitir comando prematuro.

Sem nova arquitetura, serviço, helper para deslocar recursos, desconto de baseline ou herança, fechamento arbitrário, mudança de API/quotas por conveniência ou nova autoridade. Preservar DENY, UNKNOWN, WIP, pins e autorizações consumidas. Entregar resultado de ownership, correção se demonstrável e a única evidência ainda necessária.
