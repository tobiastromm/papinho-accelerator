# ED11 — native proof candidate, L1 amendment and parallel closure

TASK=PACC-PHASE6-ED11-TPM-L1-NATIVE-PROOF-AND-PARALLEL-CLOSURE

**Pacote concreto preparado para decisão do proprietário:** emenda L1 de formato e uma única prova nativa transitória, independentes entre si. Foi implementado um host candidato, com modo nativo bloqueado por aprovação/hash/confirmacão; nesta tarefa só foram executados builds, selfchecks sem TPM e testes simulados. Nenhuma operação TPM, Q4 ou LAB foi executada. L1 persistente no produto continua PENDING_REVIEW; E21/ED11/Phase6 permanecem PENDING/PARTIAL/PARTIAL.

## 1. Integração L1: decisão exata, não outro marcador

Auditoria confirmou que config_root8_v4.c valida versão/offsets/bytes reservados exatos e que server_configuration_d3_worker_win32.inc aceita os discriminadores originais. Não existe extensão Q4 autorizada nesses bytes. A referência L1 accepted reserva revisão de schema/participante antes da implementação/publicação dependente. Não afrouxamos o parser ou criamos um arquivo como segunda autoridade.

Preparada [emenda normativa candidata](../../../adr/ADR-CANDIDATE-q4-l1-single-root-component-revision1-20261026.md): componente512 bytes PACQ4D01, campos/ordem/checksum/zeros exatos, root carrier privado versão5, mesmo owner e CAS, política de predecessor/retensão/Stop/UNKNOWN e backing de custódia exclusivamente DATA. Min/max propostos21216/32352 são tamanhos serializados, não quota ou orçamento. Só uma aprovação normativa exata permite implementar o novo carrier/worker; publicação real continuará separada. Código produto/ROOT8 aceito ficou inalterado.

O codec puro candidato foi implementado isoladamente: round-trip,512 adulterações,512 truncamentos e rejeição do formato anterior. Ele não escreve a root e não demonstra rollback físico. A proposta preserva todas as evidências nativas/RAW1/manifests, não troca identidade por digest.

O worker isolado anterior recebeu correções justificadas: só a thread do owner usa ou fecha suas referências; cada append compara a preimagem completa esperada antes de alterar o arquivo, atualizando-a apenas após persist/readback.17 testes PASS (14 regressões afetadas e3 focais), incluindo12 processos concorrentes,9 fronteiras de crash/torn write, perda/substituição/replay/rollback, escrita concorrente e fechamento UNKNOWN. Snapshot/root+testemunha ambos restaurados continuam um limite explicitamente demonstrado, não proteção. A testemunha é um arquivo SIMULADO; não virou ledger independente.

## 2. Prova de posse menor e sem segredo de hierarquia

A revisão de fontes primárias identificou uma alternativa adequada ao escopo matemático: StartAuthSession com salt protegido pela EK já acessível, bind NULL e sessão HMAC; depois GetRandom com resposta cifrada e autenticada. Isso evita criar AK/primary, carregar chaves ou usar ownerAuth/endorsementAuth. O perfil final NÃO usa sessão audit; assim não muda a exclusividade de auditoria por essa opção. Atividade interna preexistente de auditoria/contabilidade TBS/TPM continua UNKNOWN, não afirmada zero.

O piloto só prossegue com uma EK existente única, acessível, com chave pública igual à observada e Name recomputado. Se ausente, inacessível, perfil diferente ou inventário incompleto: DENY, sem criação de fallback. Uma nova coleta EK/certificado de metadados não é necessária.

Implementações novas: SaltedProofFixture, PublicAreaGateFixture, SaltedSessionPlanFixture e NativeProofHost.CANDIDATE. O host contém APIs TBS reais, mas seu ramo nativo NÃO FOI EXECUTADO. Exige exe/request/approval hashes literais, operador elevado observado, não impersonation, token LUIDs constantes, host/boot atuais fixados, PID/criação e contexto vivo, orçamento amostrado e confirmação interativa. Modo padrão nega. Nenhum arquivo OWNER-APPROVED existe; o template NOT-APPROVED não passa os guards.

Protocolo: salt e nonces frescos via RNG; RSA OAEP SECRET, KDFa ATH, HMAC e cifragem CFB da resposta. ReadPublic verifica formato, RSA2048/65537, perfil estrito, modulus e Name. Máximo1 contexto TBS e1 sessão, máximo12 caller Submit:1GetCapability, até8ReadPublic iniciais,1StartAuthSession,1GetRandom e1ReadPublic final. Não há NV/AK/CreatePrimary/hierarchy/signing/remote-CA/FlushContext arbitrário. Close com ambiguidade retém contexto/plano/referências originais, sem retry; Escape atua entre chamadas síncronas, host loss fica UNKNOWN.

Resultado válido demonstraria conhecimento do material privado de decifragem da EK no endpoint TBS acessado; NÃO autentica localização física, plataforma íntegra, revogação, geração NV ou continuidade antirollback. Sessão terminada por continue-clear autenticado e fechamento TBS são estados separados; não significam quiescência global. [Revisão de fontes/limites](SOURCE-REVIEW-NATIVE-PLAN.md).

## 3. Qualificação nova e evidência reutilizada

| Artefato/escopo | Resultado |
|---|---|
| Packet/HMAC/cifragem de sessão |162 verificações por Debug/Release |
| Public area/Name/matcher/inventário |369 por Debug/Release |
| Salt/nonces frescos/plano uma sessão |15 por Debug/Release |
| Total novo C# simulado |1092 verificações PASS |
| Builds finais isolados |8 builds, Debug/Release de4 targets, PASS zero warnings |
| Host nativo |2 selfchecks e2 negações sem modo, todos SEM TPM |
| Worker corrigido |17 testes PASS, persistência em arquivos sintéticos/processos reais |
| Componente512 candidato |4 métodos PASS, adulteração/truncamento em todas posições |
| Owner528 e desafio46 anteriores |hashes de fontes/binários conferidos, válidos em seu escopo; não repetidos |
| Núcleo criptográfico728 anterior |reutilizado, não recontado como prova nova |

Os testes com endpoint mock não provam interoperabilidade nativa, disponibilidade da EK existente, métricas reais ou fechamento TPM físico. O ramo nativo, inventário e topologia interativa são precisamente o experimento proposto. Guard128/1024 é amostrado, não pico universal; reserva controlada mais estrita124 antes de observers não aumenta128. Não foram concedidos créditos ou orçamento independente.

## 4. Custódia e revogação

A prova proposta não precisa de segredo de hierarquia; portanto custódia NV NÃO precisa bloquear esse experimento matemático. A futura definição de índice continua exigindo autorização OWNER real e custódia aprovada; incremento/leitura AUTHWRITE/AUTHREAD continuam sob o owner canônico, sessão autenticada e credencial própria. Root/RAW1/DPAPI CurrentUser/purpose/device/index/custodian devem ser vinculados; DPAPI não isola de código no mesmo usuário/admin nem é testemunha antirollback. Não criamos, extraímos ou instalamos segredo operacional. Perda/redefinição/Clear/TPM substituído/lockout/close incerto continuam DENY/UNKNOWN, sem rebaseline/refund.

CRL=HTTP403 e REVOCATION=UNKNOWN são preservados, sem nova tentativa equivalente de rede. CPS oficial anterior não fornece OCSP para esta CA. É possível propor a prova matemática sem declarar o certificado não revogado: o pedido exige reconhecimento explícito da incerteza e proíbe usá-la como admissão. Isso NÃO é uma exceção W2/W4 para provisionamento ou Q4. Se o uso futuro exigir tratar essa incerteza como confiança suficiente, será necessária decisão normativa própria ou evidência assinada aplicável; nenhuma foi inferida.

## 5. Trabalho paralelo E17–E20

E17/E18: sete fontes/testes relevantes do serviço ainda correspondem aos hashes R0–R9 qualificados. Sem defeito técnico novo, repetir builds/campanhas não produz readiness real. A menor operação futura de serviço independente pode observar identidade/acesso/readiness e Stop/lifecycle no mesmo ciclo elegível, com nova aprovação e orçamento fresco; nenhuma identidade/intent foi criada aqui. Não repetir Q4/Q7 nem inferir Stop por término do processo.

E19/E20: auditados os getters C3/Browser e os retornos RESOURCE_UNKNOWN da produção passiva. A implementação técnica já existe; D2 já está ACCEPTED_PREPARATION_ONLY. Permanecem cinco decisões quantitativas sem valores justificados: idade/validade/duração/headroom físico/commit, além de bindings reais root/CURRENT/epoch/SiteTLS/destino/módulos/host. A diferença239132 bytes é apenas a reserva gerenciada Browser, NÃO sobra de TLS/kernel. Sem uma nova hipótese não fizemos outro harness ou setters falsos. NSS/x86 e OpenSSL/x64 continuam distintos. Essas frentes não dependem do TPM para preparação; admissão futura ainda contabiliza reservas reais e UNKNOWN, sem créditos.

[Verificação E17/E18](E17-E18-REUSE-VERIFICATION.json), [entradas E19/E20](E19-E20-DECISION-INPUTS.json). O caminho crítico foi separado para não fazer E17–E20 aguardarem E21; os bloqueios restantes são observação real ou decisão quantitativa, não falta de outro teste equivalente.

## 6. Duas decisões prontas e independentes

[Handoff único de decisões](OWNER-DECISION-HANDOFF.md).

- **L1 normativa:** SHA256 `335dd1f829d959220ab1e9e0c2038cae9209ec5b9c205e52a15952d55686d5e3` — proposta, não aplicada.
- **Uma prova nativa manual:** pedido SHA256 `357c33ceafb7e5a33c69c87f7a70e0db685a16547e0e7fe8741b33e0d07d77a1` — somente diagnóstico transitório; não NV, não Q4, não aceitação L1.
- Executável selecionado Release: `ffd02de63a9b8e95741707532b6e81b33307f7745741232ae91f4c35d4148334`.

Após uma aprovação operacional específica, registrar bytes exatos de aprovação e então entregar o comando manual com seu hash. Não ativamos comando/approval agora. A aprovação L1 pode ocorrer separadamente, antes/depois da prova; não autoriza executar o host ou provisionar NV. Os hashes completos das fontes/builds estão em FINAL-HASHES.json e MANIFEST.json.

## 7. Menor sequência até ED11 PASS

Em paralelo: (A) decidir L1 e implementar/qualificar o carrier/worker aprovado, sem publicação implícita; (B) aprovar/executar manualmente o único piloto nativo delimitado e reconciliar; (C) resolver D4/D5 e bindings do alvo, sem esperar B; (D) preparar o futuro ciclo de serviço E17/E18 sob escopo próprio. Depois, somente com identidade/custódia/adapter/recursos suficientes, pedido NV específico e evidência de persistência/rollback no hardware; leitura Q4 e disposição autorizadas separadamente. E19/E20 precisam a composição real autorizada; E22 depende dos gates aplicáveis. A prova EK isolada nunca fecha ED11 sozinha.

HEAD/WIP/históricos/pins/UNKNOWN/reservas preservados. Nenhuma operação TPM automática, NV, Q4, Start/Stop, SCM/ACL, limpeza/refund/CURRENT/assinatura/recuperação autoritativa. Nenhuma proposta accepted foi editada. Preparação técnica nova=QUALIFIED_WITH_SCOPED_LIMITS; L1=PROPOSED_PENDING_OWNER; native proof=NOT_EXECUTED; custody NV=PENDING; E21=PENDING; ED11/Phase6=PARTIAL; autoridades DENY.
