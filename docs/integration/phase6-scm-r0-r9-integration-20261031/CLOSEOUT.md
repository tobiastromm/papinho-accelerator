# R0–R9 — aceitação e integração offline do registro parado

Task PACC-PHASE6-SCM-R0-R9-ACCEPTANCE-AND-INTEGRATION, 2026-10-31. Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469, WIP preservado. R0–R9 ACCEPTED antes do código dependente. Nenhuma execução SCM ou aquisição protegida/coleta real nesta tarefa.

## Decisão e preservação

Proposta original conferida integralmente, SHA256 da633b69d2f1214bc7f0a35b2d327f0f45fa7a850d02e3a0551729a03c2c999a, mantida byte-exata. Registro normativo: DECISION-personal-stopped-scm-registration-r0-r9-20261031.md, SHA256 9ac53b7d8f2b23690e1408d1d53526bdf4df8d88585fb667e7f7348808dd19c6. Incorpora somente R0–R9, precedência exata sobre a suficiência opaca de incremental20261012/W4/F8; F1–F6, quotas, M5/P3/conta virtual, SSO-B/OHI e demais domínios permanecem. Não herda B0–B8/B3.

Relatórios PROPOSED/NOT_READY anteriores permanecem históricos; a capability recebeu atualização posterior. Future directions de backup/listeners não mudam de estado por esta especialização. Nenhum ADR accepted anterior foi editado. Autorizações de staging seguem consumidas; seus pins/resultado e PST0.6.4 preservados.

## Código e callers

`service install --scq-closed` permanece a entrada explícita no mesmo owner, presenter e packet SCM_REGISTER_CLOSED. O presenter atribui internamente LIMITED_R0_R9 somente após validar o escopo exato; não há setter/flag genérica de capacidade ou grant persistido. Originais do EXE staged e duas DLLs, token/parent/ancestrais, plano, identidade, descritores, hashes e lifetime continuam retidos/revalidados. A confirmação continua própria, interativa e atribuída; nenhum nonce ou aprovação normativa executa efeito.

`server_scm_registration_resources_win32.inc` aplica a coleta sem guard124 SOMENTE a esse ato limitado, verificando reserva controlada no orçamento Q e pisos físico/commit >= Q. A função compartilhada estrita e seus callers conservam o guard anterior; LAB/TPM/SCQ/SSO-B não recebem dispensa. Dados de handles/privado/working set continuam observados, sem teto opaco fictício ou desconto de referências herdadas.

O relatório mantém missing=63 em observação saudável (incluindo opacos), pre_missing31/effect_missing32. Elegibilidade limitada é um predicado calculado da fase original confirmada, referências capturadas, perfil, reserva e amostra pós-confirmação; não limpa UNKNOWN nem converte o relatório em autorização produtiva. A source admite somente esse perfil/conjunção; produto/startup e fontes incompletas continuam DENY.

`ps_install_virtual` consulta o nome local antes do consumo, com SERVICE_QUERY_STATUS. Serviço existente, acesso negado ou ausência inconclusiva nega sem adoção. A consulta não reserva nome; Create continua exclusivo contra corrida. Um manager e um serviço/contexto no máximo; não há duplicação ou loop.

`..._take` revalida e obtém amostra/QPC antes do consumo/Create. `..._effect` mantém sequência monotônica de dois efeitos, registra retorno/intervalo/frequência e exige nova revalidação/amostra antes de ChangeServiceConfig2. Stop ocorrido durante a coleta também impede a próxima submissão. Não usa H_READY+2 como teto de handles do processo.

Readback preserva conta/comando/tipo/início/SID type, acrescenta conferência do error-control apresentado e exige SERVICE_STOPPED com tamanho completo. Sucesso limitado só é emitido após disposição confirmada e fechamento controlado conhecido. Falha parcial/readback divergente/close incerto retém o cohort existente; não Delete/Start/Stop/retry. OS quiescence não é provada por retorno de API/exit.

As novas estruturas de observação de efeitos pertencem ao packet existente. Campos privados nativos e headers da aplicação, sem formato persistido, wire/API portátil, owner ou pool novo. Censo Selected KNOWN=1.989.272/Q3.145.728; Legacy KNOWN=1.820.704/Q2.097.152. Incremento estrutural conhecido +56 bytes sobre o censo anterior. A reserva do presenter inclui novos frames nomeados; isso não limita ABI/CRT/RPC/kernel e não é uma reserva física.

## Qualificação offline

Quatro configurações Selected/Legacy Debug/Release: builds PASS, zero warnings finais. Treze CTests principais por configuração + três regressões de domínio adicionais: 64 pares teste/configuração distintos PASS. Os logs principais/de domínio registram 76 execuções, incluindo 12 reexecuções por correções dos fixtures compartilhados. Mais oito execuções finais do presenter/recursos verificaram a correção da apresentação do hash, total84 nos logs finais; não são 84 requisitos independentes.

Jornada principal final: 305 verificações/66 cenários; recursos128 verificações/21 cenários, opener29. A prova relevante é a composição real presenter/source/take/primitiva/close com fatos e SCM/RPC/RNG simulados, inclusive uma CLI positiva completa; não autenticação nativa atual do host/serviço. Na revisão final corrigiu-se a posição do aviso de risco, que interrompia o rótulo do hash EXE, e acrescentou-se uma asserção de correspondência rótulo/hash. O controlador foi congelado somente depois dessa correção e seus pins no pedido/script foram recalculados antes de qualquer aprovação operacional.

Cobertura: originais ausentes/trocados, divergência de serviço/conta/comando, proteção/volume/reparse/alias/hash/ACL, confirmação/consumo duplicados, outra thread, Stop, serviço existente/ausência indisponível, falha antes/depois de Create/configuração, piso físico/commit insuficiente e na fronteira exata Q, orçamento excedido, perfil estrito não herdando a exceção, 129 handles modelados sem ceiling transplantado, nova observação antes do segundo efeito, readback não STOPPED/truncado/error-control divergente, fechamento incerto e retenção. Startup produtivo continua negado mesmo após o registro modelado positivo.

Regressões adicionais confirmam SCQ pré-readiness, registro LAB e runtime modelado sem nova autoridade. Foram corrigidos stubs negativos dos testes host/SCQ para não extrair um segundo launcher/configuração do linker e o fixture de controlador separado, que só pertence ao perfil não-LAB. Nenhum callback permissivo foi instalado. Um warning de signedness do teste e um target de teste incorreto foram corrigidos sem mudança de toolchain/limites. Modelos, handles simbólicos e APIs mutantes não chegaram ao SCM real. O script manual foi somente parseado; não foi invocado.

## Pedido operacional pronto para decisão, não autorizado

Pedido byte-exato: ONE-STOPPED-SCM-REQUEST.CANDIDATE.md, SHA256 7eb9e6912a3d2c4b3a4408d176bfcd356e511da3a8bfc240942ce82a7ad03a79.
Script: register-stopped.MANUAL.ps1, SHA256 55102d036d8add96c334df7d2fee18443aeae1f65aa58916b443280e56889571. Procedimento/hash-checks anteriores à chamada em MANUAL-PROCEDURE.CANDIDATE.md. NÃO EXECUTADOS.

Controlador Selected/Release: 579072 bytes, SHA256 ca4e1fa707230510da19eb7aebd4ab0e39504661504c27ebecfc494c95ccec55. É candidato à seleção pela aprovação futura do pedido, não uma nova imagem staged. O alvo permanece o EXE c0f27f94… e DLLs originais; referências preservadas no workspace foram reconferidas. O alvo protegido atual não foi adquirido agora: identidade/proteção/hash/ausência SCM/contexto e pisos são gates frescos obrigatórios da execução futura, sem campanha separada ou PID/token futuro antecipado.

O pedido fixa operador/host, seis pins (controlador+alvo e DLLs), paths e comando, Q Selected3MiB, uma tentativa/duas mutações máximas, direitos mínimos, consentimento original, Stop, readback/close e riscos explícitos. Sua aprovação operacional específica é a primeira pendência; se algum gate fresco falhar, termina sem retry. Nenhuma tolerância numérica adicional foi inventada ou devolvida em branco. Custo máximo nativo, margem integral, variação entre amostras e latência continuam UNKNOWN sob a especialização aceita.

## Caminho mínimo

1. Aprovação específica do pedido por SHA256 e seleção do controlador nele contido; somente então preparar/usar uma execução manual, sem automação/retry.
2. Reconciliar a saída original: consumo, primeira causa, efeito persistente, readback STOPPED, fechamento/retention. Em parcial/UNKNOWN não remover nem iniciar; autoridade própria para reconciliação adicional.
3. Com registro parado demonstrado, preparar a menor sessão de Start diagnóstico e fonte original SSO-B sob suas autoridades e gates próprios. Registro não prova TokenUser/grupos/runtime, autenticação bilateral, KSP ou readiness; não os declarar PASS nem executar Start/endpoint. Não exigir nova CA, staging ou processo futuro para qualificar este registro.

R0–R9 ACCEPTED / IMPLEMENTED_OFFLINE_QUALIFIED. Pedido READY_FOR_OWNER_DECISION / NOT_AUTHORIZED / NOT_EXECUTED. E21 PENDING; ED11/Phase6 PARTIAL; produção DENY; OS_QUIESCENCE UNKNOWN; PST0.6.4 preservada.
