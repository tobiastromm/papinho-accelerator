---
status: proposed
scope: PERSONAL_LOCAL independent qualification binary staging resource-envelope applicability only
supersedes: []
---
# Proposta focal — domínio do envelope e admissão própria do staging

NÃO ACEITA. Exige aprovação explícita do proprietário dos bytes integrais. Não é autorização operacional, dispensa de capacidade, política numérica aprovada ou autorização para remover guards. Documento normativo candidato designado desta frente, sem nova família de ADRs.

## D0 — Efeito delimitado

Aplica-se exclusivamente ao ato independente de staging do pacote público de qualificação de três arquivos para novas instalações PERSONAL_LOCAL, no owner existente. Não se aplica ao registro/start de serviço, sessão SCQ, TPM, LAB, CA, CURRENT, emissão, recuperação ou instalação completa. Uma composição que realmente execute esses outros domínios continua sujeita às respectivas regras; não pode ser rotulada staging para evitá-las.

## D1 — Domínio dos números existentes

Os limites de handles124/128 e administrativo1024 mantêm integralmente seus domínios accepted de LAB, prova TPM e especializações SCQ. Não se tornam limites universais de qualquer efeito do owner por compartilhamento de processo, sampler, biblioteca ou caminho de código. O ato D0 requer política própria de admissão de recursos, com rastreabilidade explícita de cada limite e participante.

A presente clarificação não aumenta quota alguma nem escolhe valor substituto. Não autoriza remover guard vigente do código ou executar um caminho antes bloqueado. Qualquer integração posterior deve apresentar política fundamentada, demonstrar quais quotas accepted são realmente aplicáveis e qualificar as alterações sob autorização técnica específica. Ausência dessa política mantém DENY. Não usar132 observado como nova quota.

## D2 — História e decisões preservadas

Condições dos pedidos de coleta aprovados continuam obrigatórias para suas invocações; falhas1816/132 permanecem falhas, autorizações consumidas e pins históricos imutáveis. Aprovação de uma coleta não se transforma em norma universal durável, nem esta clarificação rearma ou amplia aquela aprovação. Modelos, snapshots e flags não concedem autoridade operacional.

## D3 — Regra de admissão própria

A política específica deve identificar versão/artefato, propósito, fases, multiplicidade, participantes originais e volume/alvo; distinguir custos estáticos, incrementais e totais, margens, recursos compartilhados e retidos. Cada requisito precisa de fonte normativa e evidência suficiente no target. UNKNOWN material nega. Capacidade positiva não nasce da ausência de um número universal ou de erro de consulta. Preservar quotas geridas, individuais e de domínio que forem aplicáveis.

## D4 — Critérios verificáveis mínimos

| Recurso | Evidência e predicado exigidos antes de admissão |
|---|---|
| Handles | Contagem total do processo original e por fase, erros e identidade/criação; próprios, herdados e transitórios não são descontados. Referências controladas planejadas e multiplicidades somadas sem duplicar charge compartilhada; aliases de papéis só unificam o MESMO objeto original. Limite/margem de processo e de aquisições devem ter justificativa aplicável antes de serem selecionados. Query válida não prova pico interno nem disponibilidade futura. |
| Memória física/commit | Medidas correntes e domínio temporal conhecidos; separar memória gerida, PrivateUsage, working set e commit, sem somar métricas sobrepostas. Considerar custo ainda a adquirir, concorrência, margem do host e retenção; política deve fundamentar suficiência, não aceitar apenas valor maior que zero ou quota gerida que cabe. |
| Volume/disco | Volume original retido e identidade/revalidação; unidade de alocação e caller quota versus espaço global; tamanhos e hashes exatos do pacote; arredondamento e somas com overflow checado. Tratar custo de diretórios/metadados, cópia parcial e consumo concorrente; query de espaço não é reserva nem payload arredondado é teto de filesystem. |
| Transitórios | Multiplicidades vivas de buffers, hash, descritores, heap/stack, CRT/loader/OS/provedores e filesystem, incluindo alocação anterior à validação de comprimento. Distinguir bounded por implementação, observado e opaco; não promover observação a ceiling. |
| Duração/freshness | Mesma tentativa/invocação/identidades/plano, relógio monotônico e ordem verificáveis. Definir pontos de revalidação antes do consumo e de cada efeito dependente; fundamentar qualquer janela/duração numérica no próprio efeito, não copiar LAB/SCQ/Browser. Mudança/perda de continuidade/Stop invalida; nova amostra não rearma ato consumido. |
| Retenção | Preservar packet, buffers, referências originais e charges enquanto houver IO/borrow/close incertos. Política deve incluir custo da falha parcial e impedir nova tentativa conflitante; não devolver capacidade por timeout ou fim presumido. |
| Stop/fechamento | Cancelamento antes de efeitos, supressão de novos efeitos após Stop, resultado/primeira falha preservados. Fechamento único por referência adquirida, proveniência/lifetime originais, tratamento de falhas e UNKNOWN; Stop não prova cancelamento nativo ou quiescência global. |

Os critérios exigem fundamentação verificável; não exigem uma ferramenta específica, tracing global ou atribuição de cada handle do OS como pré-condição universal. Se uma redução de recursos depender de atribuição interna, essa evidência passa a ser necessária àquela correção. Não se fecham handles de terceiros/caches ou emprestados para obter resultado positivo.

## D5 — Identidade, proteção e autoridade independentes

Permanecem obrigatórios: known folders resolvidos/congelados; identidade original e proteção de ancestrais; ausência de reparse/substituição; aquisição e revalidação governadas; criação exclusiva sem adoção; conteúdo/hash/descritores exatos; ato administrativo atribuído com consumo anterior aos efeitos; exclusividade, replay/concorrência, Stop e retenção UNKNOWN. Path, hash, elevação ou capacidade isolados não são autoridade. Não criar owner, root, serviço, ledger, grant, helper de deslocamento de custo ou IPC paralelo.

## D6 — Compatibilidade normativa

Base de delimitação: DECISION-ed11-single-lab-diagnostic-native-boundary-20261023 B4.B.4 e scope-target da tentativa LAB; DECISION-canonical-lab-console-topology-bd-20261023 restringe composição ao named LAB; DECISION-tpm-proof-explicit-console-host-binding-20261028 incorpora C5/C8 da proposta C0–C9 com128/preobserver124 e domínio da prova C; DECISION-personal-scq-cross-principal-offline-20261030, DECISION-personal-scq-closed-startup-s0-s7-20261030/S6 e DECISION-personal-scq-transport-only-to0-to9-20261030/TO7 mantêm os números no SCQ.

Base da admissão preservada: DECISION-ed11-preserve-incremental-resource-envelopes-20261012 exige incremento e capacidade factual; DECISION-ed11-windows-3mib-process-profile-20261020 W1/W2/W4 delimita memória gerida e custos transitivos; DECISION-personal-first-install-authorization-f0-f9-20261022 F3/F5/F6/F8 exige originais, consumo/Stop/retenção e censo; DECISION-personal-binary-destination-new-install-20261029 mantém known folders e identidade/proteção. Nenhum desses textos accepted é editado ou superseded integralmente. Esta decisão, se aceita, clarifica apenas a não generalização por reutilização de implementação.

A lacuna é de rastreabilidade: documentos derivados do staging passaram de preservar aplicabilidades para exigir124/128 como guard próprio sem ponte accepted específica localizada. Não se afirma contradição literal entre duas normas accepted nem dispensa implícita. Se nova fonte accepted aplicável demonstrar obrigação específica diferente, interromper integração dependente e reportar GOVERNANCE CONFLICT antes de prosseguir.

## D7 — Limites da aceitação futura

Aceitar este documento não qualifica política numérica, implementação, capacidade, target, instrumento ou operação. Não autoriza staging/coleta/tracing, diretórios, SCM/ACL, Start/Stop, endpoint, CA/CURRENT, KSP, TPM/NV/Q4, limpeza/refund ou recuperação. Implementação posterior permanece separada; pedido operacional só quando política/bindings/evidências e gates frescos estiverem completos e houver autorização própria. DENY, UNKNOWN e PST0.6.4 preservados; nenhuma conclusão E21/ED11/Phase6 decorre desta clarificação.
