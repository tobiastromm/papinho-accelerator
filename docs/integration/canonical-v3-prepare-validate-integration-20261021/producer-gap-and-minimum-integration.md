# Produtor canônico: fronteira exata ainda não integrada

Esta auditoria não altera contrato accepted, API, layout ou fonte funcional. Não cria outro produtor e não registra nova decisão. Complementa a [prova do worker](../canonical-v3-transaction-completion-20261021/transaction-proof-and-remaining-gate.md), sem repetir seus testes como prova positiva de preparação.

## Chamadas existentes e incompatibilidade factual

| Ponto real | Precondição / ownership atual | Consequência para v3 update |
|---|---|---|
| server_launcher_owner_win32.c: preparation_owned | singleton root, thread owner, launcher ACTIVE | Só esse owner pode preparar; componente diagnóstico não pode substituir o produtor |
| configuration_prepare_source | t.state EMPTY; commit não inicializado, exceto root_recovery_candidate_matches; nenhum runtime/listener ativo. Copia o.config para t.model e inicia source | Não oferece entrada de atualização sobre CURRENT publicado; retorno NOT_SUPPORTED mesmo após source_prepare OK não significa VALIDATED v3 |
| root_policy_model_valid / policy_prepare_model | t.model.config é cercado por igualdade com o.config; commit inicializado só aceito como candidato de recuperação | t.model participa da geração atual; reescrevê-lo com candidato não constitui preparação isolada |
| participant_candidate / prepare_listener/capabilities/C2 | Mesmo t, source/runtime/provider e modelo; commit inicializado exige recovery matching | Falta geração candidata de update com fontes próprias protegidas; remover condição de recovery enfraqueceria o fence |
| transaction_prepare_store | Somente primeiro commit dormente: expected deve ser zero, commit não inicializado, dormant_applied | Não é dispatcher v3. expected-base/CURRENT update não é um modo atualmente implementado nessa entrada |
| configuration_current/current_pin | Commit e publicação normativos, modelo/C2/epochs/projeção e pin factuais | Geração publicada deve permanecer imutável enquanto prepara o sucessor; bitmap, digest ou imagem fixture não satisfazem o contrato |
| v3_capture_data / pinned_signer | Captura exige free prepared slot; kind8 de consumo exige backing já protegido por pin e ranges exatos | Não fornece autorização de preparação. Fabricar pin do candidato ainda não publicado não é solução de lifetime |
| packet11 | Guard/persistência no mesmo worker, entrada somente por macro de teste | Não existe chamada produtiva de prepare/validate/commit v3; sucesso TxF diagnóstico não cria esse caminho |

Não foi encontrada fonte CURRENT factual nova. A ausência já declarada pelo owner continua respeitada, sem repetir solicitação de store nem criar um para teste. O bloqueio do produtor é IMPLEMENTAÇÃO/LIFETIME, distinto do bloqueio AMBIENTAL de CURRENT e token administrativo.

## Plano mínimo de implementação no MESMO produtor

1. Modelar a atualização como fase da preparação existente, separada de bootstrap/restore. Fixar antes de qualquer mutação a identidade/revisão do CURRENT v2/v3, seu pin, imagem completa e checkpoint/história de fontes. Não reusar t.model/source da geração vigente como scratch candidato. O estado/pin predecessor deve permanecer alcançável pelo mesmo owner durante PREPARE→RESULT e resultados retidos.
2. Usar slots/backings G7 já existentes para candidato e manter o predecessor no slot pinado. Escolher slot livre por critérios canônicos, sem substituir slot com readers/retained e sem segunda tabela de pins. A coleção completa é preservada: target singular pode recusar N>1, jamais truncá-lo.
3. Resolver referências do candidato por fontes administrativas reais, mantendo borrows/CA/issuer/policy e sua revisão. Definir claramente validação síncrona PREPUBLICAÇÃO: ela pode consumir referências já protegidas do owner, sem fingir CURRENT/pin publicado para o candidato. O dispatch atual de consumo permanece protegido; um adapter candidato só pode ser implementado após comprovar proveniência/lifetime dessas entradas. Sem fonte, resultado DENY, não callback MODEL convertido em RESOLVED.
4. Comparar predecessor inteiro (imagem/checkpoint/R/A/P/I/C/W/native identity/histórico), não apenas LST1/digest. Validar SNT/PDG, purpose/status/revogação e issuer sem inferir aprovação de bytes. Atualizações de aprovação não podem ser obtidas pela remoção dos memcmp de preservação do packet11.
5. Somente após sucesso de todas as validações e reserva factual congelar previous/next no packet existente. O prepare/validate não inicia thread nem publica; commit separado revalida CURRENT/pin/Stop/native identity imediatamente antes do efeito. Reutilizar pending_worker/TxF e suas dispositions; NÃO mover source/policy/pins ao worker nem criar outro produtor.
6. Definir término do mesmo grafo: abort antes de efeito libera apenas candidato após quiescência; commit/UNKNOWN/Stop pendente mantêm ambas as gerações e créditos; liberação de predecessor depende de fechamento factual e todos os borrows/Sessions. As rotinas históricas transaction_abort/close atualmente operam sobre t inteiro; não chamá-las como se já fossem abort de candidato de update.
7. Reconciliar layout compilado e pico transitivo dessa extensão ANTES de aplicar campos/hunks. As autoridades accepted limitam o resultado; se os fatos demandarem representação/autoridade adicional não coberta, propor somente essa extensão, nunca preencher campos por inferência.

Esses itens são trabalho funcional pendente, não claims de APIs implementadas nem decisão normativa nova. Nenhum hunk que apenas removesse guards ou encaminhasse buffers foi submetido como integração segura. A ligação dependente permanece interrompida por falta de predecessor factual e prova do lifetime desse grafo de update. O plano não presume que falta de ambiente dispense os itens1–7.

## Provas necessárias e resultado desta execução

Prepare v3 positivo e validate v3 positivo: NÃO IMPLEMENTADOS/QUALIFICADOS. Para uma implementação posterior, exigir testes de geração atual intacta antes/durante preparo, candidato sem pin fictício, referências divergentes/ausentes, predecessor divergente, issuer/PDG/CP3 corrompidos, status/revogação, abort antes de efeito, CAS conflict/UNKNOWN, Stop com worker e borrows, quiescência e slots retidos. Positivos de fixture ficam identificados como MODEL, sem autoridade operacional.

Nesta tarefa, quatro targets/configurações de build existentes foram verificados atualizados sem warnings e72 suítes existentes passaram.24 casos nativos locais foram reexecutados sobre stores fixture exclusivos:8 v3 e16 históricos. Eles preservam regressões de persistência, negativos CURRENT/pins, formatos e G7; não comprovam um produtor ainda ausente. O manifesto anterior de15 entradas e todo baseline dos dois projetos foram verificados.

## ED11

Sem alteração funcional ou armazenamento adicional nesta auditoria: Selected fixo1.948.576 /3.145.728 explícito; Legacy1.790.872 /2.097.152. As três imagens30592, backings116616 e reservas de stack existentes continuam contabilizadas no [censo anterior](../canonical-v3-transaction-completion-20261021/ed11-census.json).

O novo grafo de preparação NÃO recebe reserva por este plano. Seu custo deverá incluir candidate model/source/material, predecessor/pin, CA/issuer/status, crypto/hash/encode/decode, caller arenas, stack, handles, instrumentação, concorrência e resultados retidos. Reuso requer lifetime demonstrado; não contar twice imagens já no owner nem omitir coexistência possível. O cenário conservador completo3.747.521 supera3MiB em601.793; não é pico operacional medido. Provider/kernel/stack transitivo UNKNOWN; ativação dependente e ED11 total permanecem DENY/PARTIAL. Nenhuma quota ou limite foi modificado.
