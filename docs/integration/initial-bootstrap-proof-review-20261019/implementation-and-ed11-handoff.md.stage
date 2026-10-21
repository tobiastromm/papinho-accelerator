# Handoff mínimo de implementação e ED11 — PROPOSED

Não aplicar antes de decisão normativa específica. Arquitetura continua uma root, um store, um worker e três slots/pins existentes. Esta tarefa não implementa o procedimento, não executa testes e não aceita qualquer pacote funcional.

## Menor conjunto de alterações dependentes

1. Frontend administrativo da root existente: ação inicial explícita (CLI ainda proposta), confirmação de finalidade/declaração não secreta e alvo/source. Validar token efetivo/role OS pelo adaptador administrativo, separado de autenticação de cliente. Não aceitar parâmetros SID/proof=true como autorização. Reutilizar o padrão de identidade/CloseHandle da plataforma sem chamar ou depender da CA/CNG; extração compartilhada de código só após revisão própria.
2. Preparação existente: valor bounded da cerimônia (identidades nativas, estado, ator/correlação já existentes) com leases factuais e Stop/retenção. Não criar segundo catálogo/sistema de pins. Source/refs/material têm de ser realmente resolvidos neste modo. Se faltam credenciais/perfil/autoridade, negar mesmo quando o contêiner for novo.
3. Adaptador de caminho/armazenamento existente: criação exclusiva de novo contêiner e prova de custódia contra namespace substitution conforme o contrato aprovado. A implementação atual path_ok não é essa prova. Qualificar primitivas concretas e fault model; não presumir que handle share/exclusive ou comparação antes/depois bloqueia todas as corridas. Sem capacidade: UNSUPPORTED.
4. Mesmo PAPACC_CONFIG_PENDING/pending_worker: modo INITIAL_BOOTSTRAP distinto do update, previous ABSENT sem imagem zero, CREATE_NEW somente com prova viva da preparação. Ramo ROOT06 e ramo D3 update OPEN_EXISTING intocados semanticamente. Copies frozen, sem ponteiro root na thread. Nenhum worker novo. O packet já dispõe de espaço maior que9408 para no-carrier; não ampliar arrays por conveniência.
5. Resultado/poll e seleção da root: commit confirmado permanece COMMITTED_PENDING, exportável pelo recibo existente; CURRENT/epochs só pela conclusão normativa autorizada e snapshots completos/pins. Não tratar o campo published ou o retorno do decoder como prova. Publicação da coleção segue revisão/censo próprio.
6. Auditoria com logger/recibos existentes; sem schema paralelo, segredo ou marker. Toda falha de fechamento deve continuar representável/retida. Restart/restore não é retomar cerimônia nova; manter DENY sem os gates de recuperação.

## Fases e propriedade mínimas

| Fase | Owner/lifetime | Liberação/negação |
|---|---|---|
| Antes de criar contêiner | mesma preparação, ator/refs/autoridade validados | falha sem efeito não cria direito de bootstrap |
| Contêiner criado | custódia native lease da preparação; identidade/correlação bounded | não liberar enquanto worker/ref depender; perda/incerteza RETAINED |
| Packet congelado/worker ativo | arrays embedded e worker existentes | Stop sinaliza; owner não espera chamada nativa; resultado UNKNOWN não libera |
| Commit confirmado | root mantém packet/refs e recibo de commit | não CURRENT; fechar dependências só quando factual e quiescente |
| Stop/crash/close incerto | retenção e evidência existente | nenhum reset/marker/cleanup alternativo; bootstrap continua DENY |

## Custos mínimos e o que ainda não é mensurável

Nenhum custo de implementação foi acrescentado nesta revisão. Base factual anterior ALL2089223/limite2097152/margem7929 é apenas cenário, não autorização. Não reduzir suas reservas ou custos retidos.

- Necessários: estado de cerimônia, identidade efetiva/referência de ator, identidade nativa do parent e do novo contêiner, referências de source/leases, e no mínimo as dependências nativas de custódia e consulta de token. Root domain/invocation e packet existentes devem ser referenciados com lifetime, sem duplicação automática.
- Exemplo de payload lógico Windows: duas tuplas volumeU32+file-indexU64 =24 bytes; SID até68 bytes como limite da estrutura consultada; dois flags/estadosU32 =8. Isso dá até100 bytes de campos declarados para ESSE subconjunto, NÃO sizeof do estado, não serialização proposta, nem limite total ou garantia de compatibilidade. Escolher formato do valor privado só no patch; considerar padding, lengths, resultados/ownership/erros e correlação reais.
- Handle values, token/group-query scratch, buffers de security descriptor/path, logs/receipts, coexistência e retenção têm custo adicional. Listar handles/objeto de kernel separadamente; bytes de ponteiro não representam custo kernel. Provider/kernel seguem UNKNOWN. Não abrir handles de auditoria/observação extras sem necessidade.
- Não criar novas cópias D3 por fase: usar o packet/buffers já preços somente quando lifetime provar. Custos da futura coleção imutável por geração não desaparecem por esta cerimônia. Três novas cópias9408 adicionariam28224 e ultrapassariam o cenário atual; são um cenário a rejeitar, não uma implementação necessária.
- Antes do patch: census compilado Debug/Release, sizeof/multiplicidade e pico por fase; checked-add à reserva factual antes de aquisição/efeito. Se exceder2097152, interromper. Não oferecer threshold/headroom novo.

## Regressões necessárias depois de aceitação/patch oficial

| Caso | Resultado exigido |
|---|---|
| Admin autorizado + novo objeto criado e custódia validada + refs reais | elegibilidade no perfil local aceito; commit de teste, sem admissão |
| Token operacional, papel desabilitado/consulta falha, SID digitado | DENY antes de efeito |
| Arquivo/CURRENT ausente em contêiner antigo/vazio | DENY, nunca bootstrap por ausência |
| Store corrompido/truncado/backup/restore/histórico incerto | DENY e preservar |
| Duas invocações mesmo alvo | um vencedor factual, perdedor não adota |
| Parent trocado/reparse/lease perdida | DENY/retido; nenhum write em alvo divergente |
| Crash antes/depois da criação, write, commit ou CURRENT | resultado conservador, sem recomeçar cerimônia automaticamente |
| CAS conflict/UNKNOWN/close failure/Stop com worker pausado | sem fallback/retry destrutivo, crédito/ownership retidos |
| ROOT06/BODY0.5, issuer presente e D3 update | históricos intactos; issuer/restore negados; update conserva CURRENT/pin/CAS |
| Root collection/pins antigos | nenhum overwrite/truncamento; falta de slots/reserva nega |
| Token/dir/file/tx handles e Stop | closes factuais/exactly-once; nenhum DESTROYED por processo sair |

Distinguir fixtures sintéticas de cerimônia administrativa/refs reais e evidência nativa de persistência. Qualificação Windows moderno não prova NT4, box ou ISA; contrato comum continua sem Win32/CNG, adaptadores não suportados negam. Nenhum teste foi executado agora.

## Decisão mínima solicitada ao owner

Aceitar ou rejeitar P0–P9 em CONTRACT-LOCAL-BOOTSTRAP-PROPOSED.md, particularmente o alcance LOCAL de P3/P4 e a origem de autorização administrativa P1. Se aceitar, ainda exige registro normativo oficial e patch exato com primitiva nativa de custódia demonstrada, censo e testes. Se não aceitar a limitação local, manter D2 estrito e requerer uma fonte positiva de proveniência histórica aprovada; não inventar essa fonte. Não são necessárias novas arquiteturas diagnósticas ou um backup universal.
