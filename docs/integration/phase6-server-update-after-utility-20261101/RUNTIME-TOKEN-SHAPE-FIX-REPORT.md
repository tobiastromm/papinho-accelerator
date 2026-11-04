# Correção funcional — token virtual no diagnóstico TRANSPORT_ONLY

## Resultado e escopo

FUNCTIONAL_FIX_QUALIFIED_OFFLINE_NOT_INSTALLED. Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469; WIP/históricos preservados. A correção remove a equivalência indevida entre TokenIsElevated não zero e participação administrativa, exclusivamente na aquisição virtual fechada TRANSPORT_ONLY. Não muda contratos accepted, política G3, CA/KSP, readiness produtiva ou permissões do Windows.

A sessão original demonstrou rejeição no predicado TokenIsElevated do adapter (site30 na preimagem qualificada); V3 observou posteriormente o runtime10080/criação134381246145672895 com elevation1/typeDefault, Administrators ausente, integridadeHigh e SeImpersonatePrivilege habilitado. A observação atual não é atestação retrospectiva completa do token original. Esses campos não são normalizados para zero, nem demonstram privilégio mínimo ou isolamento CA.

## Confronto normativo

- DECISION-windows-virtual-service-identity-20261022 V0/V2 distingue logon, TokenUser, grupos e configuração; V4 preserva separação administrativa e negação de CA privada. V5 exige prontidão e close factuais. Nenhuma dessas cláusulas define TokenIsElevated0 como identidade da conta virtual.
- DECISION-personal-scm-virtual-account-20261029 incorpora ADR-CANDIDATE-personal-scm-virtual-account-20261029: TokenUser virtual selecionado, grupos/restrições integrais, continuidade e provas independentes de acesso. Preserva P4 e exclui herança R0–R9/LAB. Ausência de grupo Administrators não substitui provas de acesso efetivo.
- OHI incorporado em DECISION-personal-scq-original-handoff-20261030, Handoff/destinatário: separar administrador que autoriza e runtime; processo não assume cerimônia administrativa nem recebe chave privada.
- Alternativa B incorporada em DECISION-personal-scq-cross-principal-offline-20261030, regras2/4/5/6/7: pares originais, finalidade, autoridade independente, consumo, recursos e retenção.
- TO0/TO1/TO3/TO4/TO5/TO6 do texto incorporado em DECISION-personal-scq-transport-only-to0-to9-20261030: diagnóstico sem KSP, discriminação explícita, todos os demais bindings, grupos/contexto, bilateral e close. A dispensa de key não dispensa identidade. Aqui não se amplia essa dispensa: corrige-se o predicado técnico usado como proxy de identidade administrativa, mantendo consultas e os fatos brutos. READ e outros perfis conservam a regra estrita anterior.
- SSO-B/S0–S7 e G0–G9 não dispensam fonte, autenticação, recursos ou close. Os limites P1 128/188/192, commit3GiB e demais G3 permanecem intactos.

Não foi encontrada obrigação accepted de igualdade do bit a zero para este diagnóstico. Não se edita ADR nem se cria exceção operacional ou norma por teste. As obrigações de isolamento efetivo P4/V4 continuam pendentes e impedem inferir produção. Não há nova decisão arquitetural dependente a aceitar para corrigir esse proxy técnico dentro da autorização funcional offline vigente.

Fontes primárias: https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-token_elevation define bit não zero como privilégios elevados; não o define como membership Administrators. https://learn.microsoft.com/en-us/windows/win32/api/winnt/ne-winnt-token_elevation_type define Default como ausência de linked token. Default não significa ausência de elevação. A correção conserva ambos os fatos, sem reclassificar o token como não elevado.

## Código e callers efetivos

personal_runtime_identity_win32.h/.inc: aquisição específica capture_transport_closed; capturas antigas SAM/READ permanecem estritas. A nova aquisição exige virtual/closed, TokenPrimary, SID selecionado/resolvido, tipoDefault, todas as consultas/grupos/restrições e revalidação SCM/processo. Permite somente os valores brutos0/1 neste ramo, rejeitando outros; rejeita qualquer entrada Administrators, inclusive desabilitada/deny-only. Guarda o valor bruto e nega sua mudança durante current. observe_data projeta o valor real, nunca zero presumido. A aquisição sozinha não fornece autorização operacional.

personal_scq_profile_win32.inc: valida o mesmo fato nos consumidores e no binding tardio, discriminando propósitoTRANSPORT_ONLY do próprio plano. READ conserva elevated0; Administrators, tipo incorreto, restrições/contexto e todas as demais negativas permanecem. Campos, tamanhos220 do peer, SCQ1/SQB2 e formatos públicos/persistidos inalterados; valor raw1 circula no campo existente.

server_configuration_personal_native_win32.inc: controlador escolhe o capturador a partir da registration original já revalidada pelo startup source, sem nova cópia de plano na stack; receptor com plano escolhe pelo purpose validado. O ingresso inicial QUERY usa o capturador diagnóstico no percurso exclusivamente fechado, mas continua sem autoridade até a aquisição dos originais e os gates posteriores. Carrier não constitui aprovação e não recebe campo permissivo novo.

Sem novo owner, serviço, canal, lookup de identidade alternativo, impersonação ou privilégio concedido. Métodos antigos e caminhos não TRANSPORT_ONLY mantêm seus predicados. Rejeição produtiva e proibição de KSP permanecem.

## Qualificação e recursos

Selected/Legacy Debug/Release: quatro builds PASS com /W4 /WX, zero warnings;21 regressões afetadas por configuração,84 pares PASS. Novo teste do corpo real:50 assertivas por configuração, incluindo raw0/1, valor inesperado2, alteração posterior, replay, grupo Administrators, TokenUser divergente, restrições/tipo/SCM divergentes, READ e SAM, rejeição no consumidor e projeção real. Demais regressões cobrem Stop/close UNKNOWN, origem, transmissão modelada, receipt e cadeia fria. APIs processo/token/SCM/endpoint são modelos; nenhuma sessão real foi executada. Não se repetiu a campanha completa do produto.

Estado persistente: +8bytes por root/censo, sem formato persistido. Selected ROOT375112/FIXED_GRAPH1576720; Legacy ROOT206544/FIXED_GRAPH1408152. Sem handle nem consulta nativa novos; o TokenElevation já era consultado. Limites gerenciados e G3 intactos. Custos opacos e capacidade nativa não viram PASS por esses números. Censo e logs: RUNTIME-TOKEN-SHAPE-FIX-OFFLINE-QUALIFICATION.json. Diff focal contra preimagens: RUNTIME-TOKEN-SHAPE-FOCAL-DELTA.diff.

## Artefato e gate operacional

Candidato Selected/Release congelado: papacc_server.runtime-token-798208.exe, 798208 bytes, SHA256 39a9da2f53ad6208ed875a916debd9c2fec760826f7a17d702d65afa28095b15. Delta+512bytes contra o último instalado documentado797696/ID417614100:3014656:330305/pinbf7d0793428f5f751e87b007b28d676aa10235e6decc36d33bdd1de434508d60. Não foi feita consulta nova do disco instalado; identidade atual exige gate fresco na operação futura. Updater, DLLs, SCM, ProgramData e EXE instalado não foram modificados.

Primeiro impedimento operacional: controlador6120/criação134381245905057064 e runtime10080/criação134381246145672895 estavam retidos na última observação. Não presumir encerramento nem SCM STOPPED. A autorização V3 está consumida. Não há autorização nova para terminar esses participantes, Stop, UPDATE ou repetir E/Q/S. Nenhum comando prematuro é emitido.

Caminho mínimo: preparar disposição excepcional estritamente desses dois participantes reutilizando o procedimento já qualificado, com identidade/token/imagem/SCM revalidados antes do efeito e aprovação específica; depois readback STOPPED/gates do UPDATE U1–U7/U2 e nova autorização byte-exata para este candidato; após instalação reconciliada, nova sessão E/Q/S independente. Disposição não será declarada close SCQ nem apagará UNKNOWN histórico. Não limpar arquivos, desfazer consumo ou reutilizar decisões anteriores. Prova nativa da correção permanece pendente, sem promessa de que todos os demais gates passarão.

## Preparação operacional concluída nesta entrega

O pedido ONE-RUNTIME-TOKEN-PARTICIPANTS-DISPOSITION.REQUEST.CANDIDATE.md está pronto para decisão específica, SHA256 `7e562fdc1c56c824e2fc6e38e517e17e8cf52ffbc9f801054825f59e31a62ae9`. Source/model/wrapper adaptados somente nos alvos/pins/nomes do mecanismo existente;52 cenários modelados/17edges e AST PASS. Fonte real somente compilada. Ver RUNTIME-TOKEN-DISPOSITION-OFFLINE-QUALIFICATION.json. Nenhum arquivo de decisão antecipado, execução ou autorização de UPDATE/Start. O primeiro gate é agora a aprovação específica deste pedido de disposição, seguida de execução manual com revalidação, não outra investigação de token.
