# Sucessora LAB -03 — implementação e qualificação offline

## Resultado e autoridade

Implementado o binding exato aprovado: N0START-20261024-03/tentativa0b1de869e11493b7cb08fb329578f6ce. Autoridade funcional: resposta explícita “autorizo” do owner ao pedido de implementação/qualificação offline após aceitação do anexo L1 revisão2. A autorização não inclui preflight/Start, ativação, colocação protegida ou aprovação operacional. [L1 ACCEPTED](../../adr/DECISION-independent-lab-successor-exact-binding-annex-20261024.md), [L0–L9](../../adr/DECISION-first-independent-lab-successor-scope-20261024.md), [H0–H9](../../adr/DECISION-l5-p5-target-specific-historical-resource-coverage-20261024.md), P0–P9/P3, N0–N9, RAW1, B+D e ED11 permanecem íntegros.

Revisão focal de fontes pelo agente executante, não parecer externo independente. Contexto/ADRs aplicáveis de OrganizationEngineering/PapinhoEngineering, capability e Future Direction PACC relacionados continuam subordinando a implementação. PROJECT-CONTEXT.md ausente; não foi criado substituto. Mesma branch/main HEAD913c90cf2328f7939d8f5b927640601903ac6e91, MSVC19.51x64/Ninja/SDK isolado existente e produção0.6.4. Preimagens preservam o WIP existente; patch é contra essas preimagens, não contra uma árvore limpa inventada.

## Integração e revisão de segurança

[Patch focal](review.patch) e [matriz](binding-matrix.json) registram:

- Owner existente mantém mutex Global/PapinhoAccelerator.ED11.SingleLabAdmission, thread/ownership/close/retention e consumo CREATE_NEW. Guard aceita somente a nova tentativa fixa; antiga/arbitrária não passa. Intent/launch/registro consumido usam o novo ID; nenhum tombstone antigo foi alterado. Não há segundo owner/serviço/registry, reserva ou parâmetro genérico de novas tentativas.
- Descriptor B+D, IG_ATTEMPT e confirmações usam a nova identidade. Fonte/cópia de imagem têm SHA256 factual do novo Selected Release, leaf novo exclusivo e FILE_CREATE final. Probe pré-consumo também usa folhas novas, mantém pai original/ACL/IDs e só aceita ausência nativa inequívoca. Consulta não reserva nome; colisão posterior continua consumida/retida sem retry.
- Pré-estado SCM esperado agora é o ImagePath histórico papacc_server_lab_host_ntstatus.exe, sem adotar seus bytes como imagem nova. Após rebind, readback exige a folha nova. Readbacks de startup/conta/SID/recovery/triggers/estado e primeira falha preservados. Nenhuma consulta SCM foi executada nesta tarefa.
- Recorder original mudou somente attempt/leaf/texto de binding. PSR_IMAGE_LEAF exige a nova imagem. Arm/final/checksum e primeira API/NTSTATUS preservados; sete códigos NTSTATUS originais passaram no modelo, incluindo propagação e GetLastError. Sink/arm antigos e antiga imagem/hash são explicitamente rejeitados nos modelos. Não foi criado logger, thread, ServiceMain nonce ou protocolo wire.
- Adaptador protegido abre NOVOS nomes, e plano vincula a operação/identidade, políticas L1/H/P3 compiladas, approval futuro, tokens/processos/arquivo OS e IDs nativos efetivos dos inputs. As quatro referências originais de políticas-base permanecem; não foi acrescentado conjunto de handles de políticas.
- Registro privado V2 acrescenta attempt e operation ao mesmo dado limitado de arquivo OS. Parser exige valores exatos, representação canônica, sem campos extras/CRLF/truncamento. Isso não é wire nem nova autoridade: só dados dentro do mesmo contexto nativo protegido. V1 histórico continua íntegro e é negado como novo registro. Evidência estática exige aplicabilidade/revalidação próprias; não se copiaram claims antigos.
- Pins SHA do novo dossiê/evidência são VAZIOS, volume/file IDs/tamanhos ZERO NÃO QUALIFICADOS. Zero não significa recurso zero nem ID válido. Match exige pins nativos reais e falha fechado. ig_n0_inputs_ready nega a entrada antes de qualquer aquisição/preflight; não aceita booleano/ID/hash arbitrário de argv como prova. Dossiê protegido não foi criado. Falta factual permanece deliberadamente bloqueante.
- Launcher existente usa PowerShell único5.1x64, operação03 e qualifier novo pinado. Todas as linhas Write-Host administrativas e o loop inteiro de encaminhamento permaneceram iguais à preimagem; [Console]::Out.WriteLine($line) preservado. Confirmação/desafio, stdin/stdout, truncamento, supervisão, Stop/F10 após retorno e retenção UNKNOWN não mudaram. Qualificação de protocolo continua ASCII/UTF16hex, não Unicode arbitrário/transcript/customhost.

Orçamento independente REQUIRED_NOT_GRANTED e N7 operacional NOT_GRANTED. H0–H9 não é refund, reserva liberada, desaparecimento de UNKNOWN ou quiescência histórica. IDs/PIDs/tokens/métricas futuros não são inventados. O histórico -02 mantém intent consumido, efeitos/P3 e first1816/starts0/stops0/close0/quiescent0. Nenhum intent/imagem/sink/arm/dossiê operacional ou arquivo protegido criado.

## Qualificação, falhas intermediárias e correções

Quatro compilações do qualificador e quatro do runtime, Selected/Legacy Debug/Release, PASS /W4 /WX, warnings0. Serviço usa o perfil runtime original SINGLE_LAB_ADMISSION=OFF; qualificador mantém SINGLE_LAB_ADMISSION=ON. As árvores são separadas apenas para respeitar os perfis documentados do MESMO serviço/owner; nenhum serviço/owner adicional é criado.

O primeiro host intermediário herdou a opção pública do qualificador e acrescentou estado administrativo não usado ao serviço: total25208 em modelo. Foi descartado como pin operacional, preservado no manifesto intermediate, e o runtime recompilado no perfil original. Perfil final comprovou PS_DIAGNOSTIC4400, PS_RUNTIME12080, PS_NATIVE24904, total25120+CLI4=25124, sem aumentar orçamento. O Selected Release pinado vem SOMENTE da árvore -runtime. Nenhum binário foi colocado na instalação protegida.

Modelo de recorder inicialmente falhou porque fixture devolvia a folha histórica. Corrigido somente o fixture para a nova folha; adicionados casos explícitos de sink histórico e arm histórico válido sob identidade antiga. A validação do runtime não foi relaxada. Fixture de concorrência inicialmente negou o novo namespace/diretório antes de ENTER; ajustada somente sua allowlist exata isolada, sem alterar política de mutex/DACL canônica para obter PASS. Evidências intermediárias preservadas.

A auditoria de build encontrou ausência de dependências explícitas de contexto/pins, relevante porque /showIncludes está localizado. Foram adicionadas dependências diretas de includes do qualificador e fixtures/runtime; o header do pin da imagem não é dependência do serviço, evitando autorreferência de hash. Qualificador recompilado mudou seu hash após incorporar o contexto final. Não se usaram os resultados intermediários como qualificação final das fontes. Ferramentas/backends/toolchain não foram trocados.

[Resumo](build-summary.json), offline-tests.json e runtime-tests.json:36 suítes/configurações PASS do qualificador mais12 do perfil runtime. Cobrem identidade antiga, replay/consumo duplicado, arquivo/hash antigo/substituído, sink/arm histórico, dossiêV1/token/boot/policy divergentes, identidade/criação/lifetime, quinto processo,129nativo/1025admin, recovery/startup/readback, freshness60s/1s, Stop, first-failure, UNKNOWN/close incerto/retenção. Modelos positivos NÃO são admissão nativa.

cli-negative-results.json:24 invocações negativas dos binários finais, incluindo identidade antiga e novas entradas Prepare/Cycle bloqueadas por inputs ausentes ANTES de aquisição. Não executaram preflight nem SCM. native-results.json: concorrência nativa nas quatro configurações, mutex Local do fixture; primeira aquisição, peerBUSY170, liberação factual/reacquire, Stop, owner inválido1307, token esperado divergente5 e UNKNOWN retém mutex/nega finish/bloqueia peer. Token divergente é expectativa injetada, não logon de outro usuário. Fim natural do fixture UNKNOWN não é cleanup semântico e namespace não é reutilizado.

PowerShell5.1.19041.6456: AST e quatro casos usando SOMENTE as funções originais Hash-Stream/Pin, não a entrada do launcher: pin correto, hash errado, pin vazio antes de open e caminho não canônico. Referências read-only próprias fechadas. Launcher operacional não foi executado em Review/Prepare/Cycle. Sem ETW/novo recorder/ferramenta instalada/rede.

## Recursos e limitações

Estado permanente/roles/owned refs da admissão não foi expandido; PS_BD_DESCRIPTOR976, inventory208, state3128, admission3216, contexto1576 e sample1416 conforme modelos. O parser V2 acrescenta65 bytes de arrays locais nomeados (attempt33+operation32), sem alocação dinâmica ou thread; frame real otimizado/transitive stack e custos OS permanecem UNKNOWN. Plano reutiliza buffer2304 existente, sem novo logger ou buffer permanente. Tamanhos de binário não são Private Bytes/commit/peak.

128handles por nativo,1024admin compartilhados/quatro papéis, demais caps e60s/1s/headroom/Stop/NONE_GRANTED não mudaram. Histórico1022→1032, PowerShell+8/aux+2/qualifier124 permanece histórico, não medição desta tarefa. Nenhuma nova amostra de handles, garantia de redução ou headroom foi produzida. Serviço real/NTSTATUS de psr_prepare/Stop/quiescência continuam NÃO medidos neste slice.

## Saída

CURRENT_TASK_COMPLETED=YES
SUCCESSOR_BINDING=IMPLEMENTED_OFFLINE_QUALIFIED_OPERATIONALLY_DISABLED
CANONICAL_OWNER=EXISTING_SAME_MUTEX_EXCLUSIVITY_PRESERVED
RECORDER_BINDING=NEW_EXACT_IDENTITY_SINK_ARM_AND_IMAGE_OFFLINE_QUALIFIED
PROTECTED_DOSSIER=V2_ADAPTER_PREPARED_HASH_AND_NATIVE_ID_PINS_EMPTY_DENY_NOT_PLACED
N7_OPERATIONAL_ACCEPTANCE=NOT_GRANTED
INDEPENDENT_BUDGET=REQUIRED_NOT_GRANTED
BUILDS=SELECTED_LEGACY_DEBUG_RELEASE_QUALIFIER_AND_RUNTIME_PASS
WARNINGS=0
OFFLINE_TESTS=36_QUALIFIER_PLUS12_RUNTIME_SUITE_RUNS_AND4_PS_PIN_CASES_PASS
NATIVE_TESTS=24_PREACQUISITION_CLI_NEGATIVES_AND4_ISOLATED_CONCURRENCY_CONFIGURATIONS_PASS
NETWORK_TESTS=NOT_RUN
PREFLIGHT_EXECUTED=NO
START_EXECUTED=NO
STOP_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE_EXISTING_OBJECTS
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
NEW_ATTEMPT=NOT_ACTIVATED
SUCCESSOR_ADMISSION=DENY
FIRST_UNRESOLVED_DIVERGENCE=NEW_PROTECTED_INPUT_CONTENT_HASHES_NATIVE_IDENTITIES_AND_PLACEMENT_NOT_QUALIFIED
NEXT=SUCCESSOR_PROTECTED_DOSSIER_AND_NATIVE_ADMISSION_QUALIFICATION
