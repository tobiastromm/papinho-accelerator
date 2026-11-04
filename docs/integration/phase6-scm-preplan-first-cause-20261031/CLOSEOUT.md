# Primeira causa pré-plano — defeito de caller demonstrado e corrigido

Task PACC-PHASE6-SCM-PREPLAN-FIRST-CAUSE-OFFLINE, 2026-10-31. Branch main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469, WIP preservado. Nenhuma execução manual, consulta/registro SCM, RNG do produto, staging, ACL, Start/Stop, endpoint, CA/CURRENT, TPM/NV/Q4 ou outra operação protegida realizada nesta tarefa.

## Causa demonstrada e limite histórico

Foi encontrada uma incompatibilidade funcional determinística no caller, não uma ACL inadequada:

1. O presenter de SCM_REGISTER_CLOSED prepara o layout pelo caminho `papacc_configuration_personal_layout_binary` → `papacc_personal_paths_binary`: modo BINARY_ONLY=3, campos de dados vazios.
2. Depois de ORIGINALS e RNG, `scm_register_current` chamava `papacc_configuration_personal_layout_current` → `papacc_personal_paths_current`.
3. O validador genérico aceita EXISTING ou NEW; BINARY_ONLY retorna ERROR_INVALID_DATA antes de qualquer consulta de known folder. O wrapper traduz para PAPACC_RESULT_INVALID_STATE e o presenter/CLI termina no erro genérico5.
4. Assim, quando a execução alcança esse predicado, o layout preparado pelo próprio presenter é necessariamente rejeitado antes do plano. O fixture antigo escondia a incompatibilidade: modelava modo NEW e retorno positivo incondicional do validador.

A primeira condição EXATA da execução manual recebida não pode ser reconstruída retroativamente: o log não informa se RNG falhou/retornou zero antes desse bloqueio. Portanto: **defeito funcional obrigatório no caminho comprovado offline; primeira falha nativa histórica ainda UNKNOWN no intervalo já delimitado**. Não se afirma que Windows retornou AccessDenied de uma API/ACL, que RNG passou, nem que o registro existe/está ausente. Aprovação7eb9e691… UTILIZADA, sem retry.

## Correção mínima

Somente o caller de revalidação SCM passou a usar `papacc_configuration_personal_layout_binary_current`, já existente. Não se alterou o validador genérico para aceitar BINARY_ONLY. A função binária conserva ownership/thread/invocação/Stop e revalidação do known folder, modo e campos de dados não utilizados vazios; a revalidação nativa seguinte continua exigindo token/contexto, parent original e descritor/identidade. Sem fallback para NEW/EXISTING, novo parent, fixture ou root.

Exclusividade, conta/comando/SID type, consumo anterior a Create, amostras R3 por efeito, readback SERVICE_STOPPED, retenção e separação registro/Start permanecem. Nenhum contrato accepted modificado, nenhuma ADR nova necessária; é correção de composição sob R0–R9/F3/F4/F6, não redução de segurança adicional.

## Diagnóstico integrado

Metadados privados de primeira causa pertencem ao mesmo PAPACC_SCM_REGISTRATION_ACT. Estágio, tipo, valor, ordinal do objeto e fase da primeira falha são imutáveis depois da primeira atribuição. Quando disponíveis, os três campos do diagnóstico nativo de token são copiados antes do fechamento. Não são entradas de admissão, flags de sucesso, referências remotas ou grant persistido.

O frame SCM_REGISTER_FIRST_CAUSE é emitido no stderr existente antes do close; o erro CLI genérico permanece compatível. A ausência de entrega do frame por falha de output não é sucesso. Tipos distinguem predicado de política, GetLastError de API diretamente falha, NTSTATUS de BCryptGenRandom, resultado de método, subcausa de helper UNKNOWN e escrita curta. Não se lê LastError de callbacks booleanos para inventar API/ACL causal; zeros não significam PASS. Subdetalhes indisponíveis continuam UNKNOWN. Native kind1/2 segue o esquema original de API/predicado, distinto do enum do novo frame.

RNG falho/zero; captura e revalidação de token/contexto; references/queries/identidades/atributos; path/comando/digest; descritor versus alteração de descritor; hash/helper versus divergência; pacote e DLLs; primeira escrita API/curta versus output posterior e confirmação são distinguíveis. A primeira escrita conserva os bytes originais e captura o erro antes de outro helper/close. Nenhum nonce, SID/token bruto, handle, ponteiro, descritor, conteúdo de arquivo ou segredo entra no frame. O mapa numérico privado está em DIAGNOSTIC-MAP.json; não congela wire/API ou formato persistido.

## Provas e qualificação

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings finais. Dezoito CTests afetados por configuração: 72 pares teste/configuração distintos PASS; os ensaios do presenter/recursos/primeira causa também foram reexecutados após fechar a contabilização dos novos frames e o subdiagnóstico de captura de token. Não repetidas campanhas criptográficas históricas ou qualquer operação manual.

- Prova do defeito: fixture agora prepara BINARY_ONLY, mantém dados vazios e distingue chamadas aos dois validadores. O caminho positivo exige o caller binário. Uma asserção chama o **validador genérico real** nesse modo e comprova a rejeição antes de consultas Windows. O teste install_paths exercita os validadores reais com known-folder/free simulados, preservando a rejeição de promoção a NEW_INSTALL.
- Novo preplan_first_cause: 36 cenários / 317 verificações. Fault injection de RNG/zero, aquisição/revalidação nativa e subpredicado de token, queries de cada referência, identidade/reparse, descritor/helper/hash/divergência, substituição de bindings, pacote, Stop, output API/curto/posterior e falha de close. Todos os caminhos negativos pré-plano impedem consumo/Create/configuração/Start/Delete/manager; NTSTATUS/API/UNKNOWN não se confundem e primeira causa sobrevive a erro posterior de fechamento.
- original_registration: 307 verificações / 66 cenários; resource_observations128/21; opener e regressões de layout/proteção/staging/close/SCQ pré-readiness/LAB/runtime e censo passaram. A jornada positiva presenter/source/take/primitiva/readback/close é a implementação real com fatos e APIs operacionais modelados, não prova nativa do alvo ou serviço atual.
- RNG, token/arquivos/console, SCM e demais fronteiras protegidas desses ensaios são simuladas. O validador genérico real usado na reprodução rejeita antes de resolver pastas. Thread/aritmética/censo e helpers de dados não constituem autoridade operacional.

Censo estrutural Selected KNOWN=1.989.312/Q3.145.728; Legacy KNOWN=1.820.744/Q2.097.152. +40 bytes de metadados no packet existente sobre a qualificação anterior. A reserva do presenter também inclui variáveis/argumentos novos nomeados, somados conservadoramente; sem alegar bound ABI/stdio/CRT/provider/kernel. Nenhuma quota foi aumentada nem pico opaco virou zero. Custos nativos e quiescência OS permanecem UNKNOWN sob R0–R9.

## Preservação e novo pedido

Controlador aprovado anterior ca4e1fa7… preservado em build/scm-preplan-preserved-20261031/approved-controller-ca4e1fa7.exe; scripts, pedido7eb9e691…, aprovação2062b2dd…, resultado9face677… e consumo anteriores permanecem imutáveis. O alvo staged c0f27f94… e DLLs NÃO foram substituídos ou lidos no destino protegido nesta tarefa; referências de bytes preservadas no workspace foram reconferidas. O novo binário do workspace é candidato distinto, não selecionado/instalado por inferência.

Novo Selected/Release: 582144 bytes, SHA256 `63291745746f9f7441bec0894ef2bf1fc2b57485cca38ece4e70a3536d511c83`. Pedido próprio ONE-STOPPED-SCM-REQUEST.CANDIDATE.md: SHA256 `fd08f3ae7278ba50fe712769ff28134d3efbfdf7d100db181cb92bd97ce73d9d`. Script candidato2c43a4f6… e procedimento preparados, parseados, NÃO EXECUTADOS.

Há fundamento técnico para submeter esse pedido separado: bloqueio determinístico corrigido e reproduzido, caminho positivo e negativos qualificados, disposição anterior reportada CLOSE0/RETAINED0 e nenhum alcance SCM por inferência do caminho qualificado. Isso NÃO concede retry, restaura confirmação ou prova ausência do serviço. Pedido exige nova aprovação operacional explícita por bytes/hash, novos originais e confirmação. Identidade/proteção/host/token, seis pins, ausência do serviço e capacidade corrente são gates frescos daquela tentativa, sem exigir PID/token futuro, M1 ou nova CA. Se algum falhar, STOP sem ampliação de permissões.

## Primeiro gate e caminho mínimo

1. Proprietário decide sobre o **novo** pedido específico; não executar ou reaproveitar o antigo. Não reabrir R0–R9 nem inventar novos números de risco.
2. Somente após aprovação específica, uma execução manual e reconciliação da saída; qualquer nova falha tem estágio/tipo/valor preservados, sem contorno de guard.
3. Só com readback parado e fechamento suficiente preparar separadamente Start diagnóstico. Nenhum Start/endpoint ou CA/CURRENT é autorizado por esta correção/pedido.

Presenter IMPLEMENTED_OFFLINE_QUALIFIED / defeito de caller corrigido. Registro SCM NOT_DEMONSTRATED; estado atual UNKNOWN. Pedido novo READY_FOR_OWNER_DECISION / NOT_AUTHORIZED / NOT_EXECUTED. E21 PENDING; ED11/Phase6 PARTIAL; quiescência OS UNKNOWN; PST0.6.4 preservada.
