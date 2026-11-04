# Primeiro Start diagnóstico — resultado técnico

O registro parado permanece STOPPED_REGISTRATION_QUALIFIED no alcance original R0–R9. Autorizações de registro/staging permanecem utilizadas. Nenhuma operação protegida ou consulta SCM foi executada nesta tarefa.

## Mudança efetiva e alcance

Implementado `papacc_server_service_diagnostic_start_status` no mesmo módulo SCM. O relatório separa quatro requisitos prévios (ato original lifecycle, original do controlador, continuação de entrega, plano de recursos) das quatro obrigações posteriores (identidade fresca runtime, bilateral, readiness fechada, fechamento original). Não exige PID/token de processo futuro antes de Start. Não concede autoridade, altera o guard produtivo ou libera StartService. O relatório não constitui um producer positivo. Testes exercitam ausência de originais e substituição por metadados de imagem/readback sem qualquer dispatch/registro/status/RUNNING.

## Cadeia factual e primeira lacuna

1. `server_service_win32.c`: CLI usa ps_binding_ready antes de abrir SCM; ps_mutate revalida antes de StartServiceW. O gate continua DENY.
2. `server_service_startup_win32.inc`: falta producer original de admissão. Registro/readback não restauram F0.
3. `personal_scq_admin_win32.inc`: scqa_capture/present/confirm/take e scqa_startup_original existem e são qualificados em modelo. O empréstimo controller-local não é permissão lifecycle nem referência remota.
4. `server_configuration_personal_native_win32.inc`: personal_scq_registered_owner_decision retorna NULL; não existe seleção operacional original. A confirmação SCQ não autoriza Start. Não substituído por setter, flags ou loader.
5. `server_launcher_owner_win32.c`: scq_closed_prepare cria packet vazio quando não há owner; captured_empty impede transport_continue e encerra sem readiness. O progresso existente exige originais/endpoint já adquiridos no mesmo packet. Não há nesta ramificação a primeira aquisição autenticada que o povoaria.
6. Runtime e controlador são processos distintos. Não é válido entregar ponteiro de scqa_startup_original pelo argv, restaurar decisão por arquivo ou inferir aprovação de SID/hash/SCM. Isso violaria S1/S2/S5/S6 e OHI.

Primeira lacuna técnica: compor no owner existente a recepção inicial autenticada e o lifetime da fonte original no packet de preparação do runtime, incluindo o ato lifecycle separado no controlador. Uma liberação isolada de StartService não resolve essa lacuna. Não foi demonstrada impossibilidade normativa: SSO-B autoriza implementar essa composição offline. Ela permanece não implementada nesta entrega; o relatório diagnóstico não a substitui.

## Dois artefatos

Runtime registrado: c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf, 571392 bytes. Controlador do último registro: 63291745746f9f7441bec0894ef2bf1fc2b57485cca38ece4e70a3536d511c83. Rebuild do workspace não atualiza o staged. Nenhum arquivo protegido foi modificado. O binário workspace anterior foi preservado pelo próprio hash em build/first-diagnostic-start-preserved-20261031. Evidências modeladas da árvore atual não atestam a imagem mapeada de um serviço futuro. Se a composição exigir modificar runtime, selecionar e autorizar separadamente sua substituição protegida; não repetir staging antigo.

## Normas e recursos

SSO-B S1 rege origem/consumo; S2 determina identidade fresca; S3 permite preparo sem readiness; S4 separa lifecycle, endpoint, CNG e parent; S5 rege retenção; S6 mantém limites próprios; S7 exige producer/bindings antes de operação. OHI exige decisão específica e confirmação atribuída. R0–R9 qualifica somente registro parado e não provê piso Q ou risco RPC para Start. Nenhum número novo foi escolhido. M1 não é pré-condição circular para criar o processo; evidências reais posteriores precisam de gate fresco autorizado.

## Pedido e caminho mínimo

Pedido de Start: NOT_READY. Não emitido comando que apenas encontre DENY. Não falta reaprovação de SSO-B. Falta composição técnica positiva original, seu ensaio offline de ponta a ponta e plano próprio aplicável de recursos; depois verificar qual artefato contém a implementação. Somente então preparar seleção/autorizações separadas de imagem quando necessária, Start/Stop e endpoint. Start diagnóstico não conclui NEW_INSTALL: bilateral factual, fechamento, confiança/provisionamento e cerimônia CA mantêm requisitos próprios.

E21 PENDING; ED11/Phase 6 PARTIAL; OS_QUIESCENCE UNKNOWN. Não afirmada quiescência por exit/status/modelo. PST 0.6.4, HEAD/WIP, pins, históricos, reservas e chave fixture preservados.

## Verificação executada

Selected/Legacy Debug/Release: seis targets por configuração (server, três modelos host, fonte SCQA e cadeia pré-readiness) compilados serialmente; quatro builds PASS, sem warnings/errors de compilação. Cinco CTests por configuração, vinte pares teste/configuração finais PASS. São modelos de SCM/endpoint e fonte operacional, não execução Start. SID/thread nativos utilizados por testes existentes não comprovam token de serviço.

A primeira invocação do wrapper de build teve erro de caminho antes do CMake; corrigida para caminho absoluto. Uma rodada Legacy foi iniciada antes do término do build e encontrou executável de teste ainda ausente; não foi falha do código. As duas configurações Legacy foram repetidas após conclusão do build e passaram; os logs finais registram essas execuções. Nenhuma coleta operacional foi repetida.
