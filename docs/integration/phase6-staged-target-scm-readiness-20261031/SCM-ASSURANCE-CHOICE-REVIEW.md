# Resultado — garantia específica do registro SCM parado

Task PACC-PHASE6-SCM-REGISTRATION-ASSURANCE-CHOICE. Data 2026-10-31. Branch main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469, WIP preservado. Revisão documental e leitura de código/evidências; nenhuma operação protegida, consulta SCM ou nova coleta realizada. PROJECT-CONTEXT.md ausente. Nenhum ADR accepted alterado.

## Decisão recomendada

Recomenda-se **B, sob aceitação explícita da proposta R0–R9**, para uma única tentativa de qualificação de registro parado. B NÃO é admissível sob os contratos atuais sem essa especialização: ED11 incremental20261012, W4 e F8 ainda conservam DENY para a provisão opaca material. Preparou-se uma proposta posterior de precedência restrita, sem modificar normas anteriores ou presumir autoridade operacional. Não foi identificada, no confronto aplicável, proibição superior que impeça apresentar essa decisão ao proprietário; não é alegação de que B já está permitida.

| Questão | A — integral vigente | B — proposta limitada |
|---|---|---|
| Custos controlados | Censo/reserva/quotas originais | Iguais; nenhuma dispensa |
| Nativo/CRT/RPC/SCM/kernel e retenção opaca | Exigir suficiência prévia fundamentada; hoje UNKNOWN/DENY | Aceitar risco de máximo/atribuição UNKNOWN só neste ato |
| Margem host | Fonte suficiente para todos os custos relevantes | Observação fresca com piso nominal Q; sem garantia/reserva para opacos |
| Identidade/proteção/ato/consumo | Obrigatórios | Integralmente preservados |
| Efeito | Só depois de todos os gates/autoridade | Uma CreateService + uma configuração SID, após gates/autoridade próprios |
| Registro parcial | Retenção/UNKNOWN, sem retry/remoção implícita | Mesmo tratamento |
| Startup/readiness/CA | Autoridades e evidências próprias | Continuam fora do ato, sem dispensa |
| Situação atual | DENY | PROPOSED, sem caminho positivo |

A não foi demonstrada impossível em todas as plataformas; simplesmente não existe aqui fonte suficiente que limite/provisione os custos opacos. Não é solucionada por repetir uma amostra de handles ou exigir processo futuro. B reduz uma garantia de suficiência prévia, em vez de declarar que ela foi satisfeita por monitoramento.

## Matriz normativa e momento da prova

| Requisito / origem | Antes do efeito | Durante/depois | Disposição |
|---|---|---|---|
| F2/F3, F4 e F5 | Operador/token elegíveis; plano e originais protegidos; contexto vivo; confirmação específica; consumo antes de Create | Vincular retorno/novo objeto ao mesmo ato | Sem especialização de segurança |
| F6 e W5 | Stop e exclusividade; packet/backings/charges preserváveis | Fechamento factual ou retenção UNKNOWN | Sem redução |
| F8 + W2/W4 + incremental20261012 | Custos geridos/individuais; host e nativos relevantes | Transientes, picos, retenção e efeitos reais | Somente opacos e margem integral correspondente especializados em R1–R3 |
| M5 / optional-SCM | Mesmo runtime/owner, CLI canônica, sem privilégios default | Configuração e lifecycle independentes | Registrar não é iniciar |
| P3/P4 + virtual-SCM accepted | Nome/conta selecionados e alvo; administrador distinto de runtime | TokenUser/grupos/PID/runtime e isolamento CA exigem processo/autoridade próprios | Não exigir token futuro antes de registrar, não declará-lo provado |
| SSO-B S1–S7 | Decisão SCQ e fonte original distintas da confirmação de registro | START_PENDING, entrega bilateral e readiness diagnóstica em sessão separada | Não autoriza Start/endpoint; limites SCQ preservados |
| B0/B2/B6 do staging | Três arquivos já copiados verificados no alcance histórico | Aquisição original fresca para ato SCM posterior | Não herdar risco, valores ou autoridade de staging |
| D0–D7 | Domínios específicos dos envelopes | Nenhum 124/128 universal por sampler compartilhado | Não aumenta/remove quota vigente de outro domínio |

M5 é a cláusula de DECISION-phase6-personal-mvp-20261022, P3/P4 de DECISION-personal-ca-preinitialization-20261022; a especialização virtual accepted incorpora o hash09c5ee8c… e preserva ambos. SSO-B accepted incorpora alternativa B/S0–S7 hash60cdd50b…. F4 admite validade por evento/contexto; não obriga inventar TTL para uma chamada local. SSO-B S6 preserva limites do seu domínio, não concede admissão ao registro pelo compartilhamento de funções.

As fontes de governança OrganizationEngineering/ADR-0003 e PapinhoEngineering/ADR-0005/0006/0007 distinguem proposta, estado factual e código, e preservam especializações locais/owners/backends. Future directions de backup e listeners foram confrontadas: não introduzem provisão ou autorização de registro. Não se altera sua direção nesta tarefa. Os closeouts de registro governado/original e o closeout staged são evidências históricas, não uma nova fonte normativa.

## Política quantitativa concreta proposta

R3 oferece uma escolha completa, explicitamente sujeita à decisão, evitando outra rodada de solicitação de números em branco:

- Preservar orçamento gerido Q do perfil: Selected 3.145.728 ou Legacy 2.097.152, sem incremento. Qualificar/reservar custos controlados reais e retenção, sem confundir o censo histórico KNOWN=1.989.216 com pico/reserva física.
- Propor físico e commit disponíveis >= Q imediatamente antes de cada submissão mutante, em amostra íntegra. O piso deriva de um orçamento controlado inteiro existente, mas **sua escolha como margem é nova tolerância de risco**, não teorema de suficiência e não recomendação de capacidade produtiva. Pode ser muito menor que os custos opacos e que uma margem segura do host. Aprovar B implica aceitar expressamente essa limitação; não foi aprovada neste turno.
- Limitar dois contextos SCM próprios simultâneos (manager e serviço), com inventário de todas as demais referências originais do owner. Contar SC_HANDLE não demonstra incremento igual em GetProcessHandleCount. Não inventar H_READY+2, ceiling de privado/working set ou novo 124/128 para o ato. Totais continuam observados e costs opacos identificados.
- Uma CreateService e uma ChangeServiceConfig2; queries finitas e fechamento, sem loops/retry. Zero efeito adicional de payload/disco pelo controlador; persistência interna Windows não vira zero e pertence ao risco R1. Não importar 2R/R do staging.
- Validade por invocação/contexto, revalidação e amostra depois da confirmação e antes de cada efeito; registrar QPC/continuidade. Sem prazo numérico universal ou promessa de cancelamento: o risco residual inclui chamada bloqueante sem bound e variação entre amostra e efeito. Não importar 500 ms/120 s.

Essas condições NÃO são guardas já implementadas nem valores já selecionados. A proposta inclui explicitamente ausência de teto nativo/latência e piso Q como parte indivisível da decisão. Não substitui quotas geridas por monitoramento. Se o proprietário exigir uma margem maior ou teto temporal/privado/handles total, é outra tolerância de risco a fundamentar/selecionar; não existe valor técnico inequívoco escondido nas observações atuais. A alternativa A segue disponível sem reduzir garantia.

## STOP e falha parcial

Stop, perda de contexto/originais, mudança de hash/descritor/plano/conta, concorrência/replay, consulta incompleta, overflow ou insuficiência segundo a política impedem novas submissões. Stop observado entre Create e Change não autoriza completar a configuração por conveniência; deve permanecer parcial, com readback/close governados quando seguros. Retorno de Create não é sucesso do conjunto. Erro na configuração, readback divergente/não STOPPED, resultado nativo incerto ou close incerto preservam primeira causa/consumo e retenção necessária. Não deletar, iniciar, parar, resetar ou tentar novamente. Mesmo close controlado conhecido não resolve quiescência OS universal.

## Código e evidências

Leitura focal de server_scm_registration_resources_win32.inc, server_personal_resource_sample_win32.inc, personal_scm_registration_win32.h, server_service_virtual_win32.inc e closeouts: gate permanece RESOURCES_UNQUALIFIED/ERROR_ACCESS_DENIED. Relatório saudável ainda separa pre_missing31 e effect_missing32, sem conceder suficiência. O sampler atual conserva guard124; alterar seu caller depende de aceitação R3 e testes de domínio, não deste relatório.

Nenhum código/build/teste alterado ou executado nesta tarefa documental. Reutilizadas quatro builds PASS/zero warnings e 40 CTests PASS da integração anterior, sem apresentá-los como nova qualificação de R0–R9. As três identidades/pins staged, resultado consumido e quiescência UNKNOWN permanecem os mesmos. Nenhum artefato foi novamente selecionado ou staged.

## Caminho mínimo e decisão exata pendente

1. Proprietário decide A ou aceita explicitamente os bytes de R0–R9 em SCM-STOPPED-REGISTRATION-ASSURANCE.CANDIDATE.md. Não solicitar autorização operacional nesta fase.
2. Se B aceita: registrar normativamente primeiro; implementar/qualificar offline política R3, gates por efeito, retenção/partial e separação de domínios no mesmo owner. Sem decisão B, manter DENY e não implementar redução.
3. Congelar/controlar controlador separado do pacote staged existente, conferir pins/planos e preparar pedido byte-exato de uma execução manual de registro parado. Originais atuais, serviço ausente, recursos e continuidade são gates frescos dessa tentativa autorizada; não exigem coleta separada genérica ou re-staging.
4. Obter autorização operacional específica. Somente ela poderá permitir Create/configuração/readback; Start/Stop, SCQ/endpoint, CNG, CA/CURRENT e instalação produtiva seguem atos posteriores próprios.

Pedido de registro atual NOT_READY. A primeira lacuna é a escolha normativa desta garantia, não M1, CA MACHINE, root C8, runtime iniciado ou uma nova contagem operacional. E21 PENDING; ED11/Phase6 PARTIAL; autoridade DENY; quiescência OS UNKNOWN; PST0.6.4 preservada.
