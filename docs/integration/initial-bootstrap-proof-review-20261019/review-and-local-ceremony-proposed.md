---
status: proposed
scope: focal local initial-bootstrap proof review; no implementation
---
# Revisão factual e cerimônia local mínima

## Decisão recomendada ao owner

Não exigir do filesystem uma prova impossível de ausência global de história. O código atual NÃO satisfaz D2 quanto a nunca inicializado/nenhuma história anterior, nem possui um vínculo administrativo de bootstrap aprovado. INITIAL_BOOTSTRAP continua DENY.

A menor alternativa local é uma cerimônia explícita do administrador, combinada com criação positiva e custódia contínua de um NOVO objeto-contêiner pela mesma root. Ela comprovaria fatos desde a criação daquele objeto, no escopo local definido; a afirmação de não estar restaurando/substituindo uma instalação é uma declaração administrativa responsável, não um fato técnico global. O owner precisa decidir se aceita esse alcance como especialização/exceção delimitada de D2. A proposta NÃO considera D2 satisfeito por interpretação nem aceita a declaração isolada como prova.

Não é necessário novo registry, marcador, backup universal, serviço ou autoridade distribuída. O novo diretório proposto é o próprio contêiner do store canônico, não um arquivo-sinal de autorização. Esta revisão não o cria. Sua mera presença/ausência/emptiness não autoriza nada.

## Inventário de mecanismos e limites reais

| Fonte exata | Fato que fornece hoje | O que NÃO fornece |
|---|---|---|
| server_configuration_commit_win32.c / path_ok e commit_init | validação local de caminho sob fault model documentado, consulta ERROR_FILE_NOT_FOUND, geração do domain candidato e owner local | autorização administrativa, instalação sem história, fencing, proteção contra toda mutação privilegiada |
| mesmo / papacc_configuration_commit | CREATE_NEW inicial em transação existente, write/flush/close/CommitTransaction e retenção em erro | bootstrap D3 completo, prova histórica ou CURRENT; não deve ser chamado para persistir D3 por atalho |
| server_configuration_pending_win32.inc / pending_worker | update TxF OPEN_EXISTING, identidade e bytes-base completos, Stop/poll/retidos | criação inicial: exige predecessor e rejeita previous_bytes pequeno; precisa de modo inicial explícito no MESMO worker |
| mesmo / d3_begin08 e committed08 | guards de CURRENT/pin/CAS e recibo factual de commit sem epochs | constructor D3 inicial, resolvedor de bootstrap ou publicação da coleção |
| server_launcher_owner_win32.c / bootstrap_cli_preflight | validação textual/modelo seguida de NOT_SUPPORTED | autoridade de bootstrap ou config apply |
| server_ca_key_win32.c / ca_identity | padrão de consulta ao token efetivo thread/processo, SID e fechamento factual | autorização root: igualdade com SID fornecido não prova papel administrativo; não chamar CA/CNG para bootstrap |
| source/material/participants/Model E existentes | fronteiras de validação, referências e lifetime no escopo real | não se tornam RESOLVED ou autorizados por presença, hash ou callback de fixture |

Nenhuma dessas funções atesta que um path nunca foi usado, que não exista backup fora da máquina, que não houve instalação apagada ou que todas as instâncias remotas estejam fenced. Não usar hardware/file ID, timestamp ou root domain aleatório como substituto dessas provas.

## Cerimônia proposta, passo a passo

1. O administrador invoca explicitamente a operação inicial no frontend administrativo da mesma root, com source e alvo exatos, e declara que está criando UMA instalação nova, não restaurando/adotando/substituindo uma anterior. Sem comando implícito ao iniciar serviço ou ao encontrar arquivo ausente. A CLI ainda não existe; interface documental possível: `papacc-admin config bootstrap --source <papacc.conf> --new-container <path>`. Não é comando executável/autorização por flag.
2. O adaptador verifica a identidade efetiva e o direito administrativo de commissioning fora do candidato: proposta inicial Windows moderno é membro efetivamente habilitado do papel local Administrators, operação humana explícita, distinta da identidade operacional do serviço, com direitos nativos sobre o parent governado. A política precisa ser accepted antes do código. Não tomar SID digitado, membership só nominal/deny-only, ser dono de arquivo ou mero write como autorização suficiente. Não elevar automaticamente nem modificar ACLs existentes. Falha/impersonação/contexto incompleto nega. Reutilizar primitives de identidade/fechamento, não uma autoridade CA.
3. O owner ainda não inicializado valida source/participantes/refs/leases/policy e suporte do target por D2.1/D4. Nenhum CURRENT anterior é exigido. Fonte validada/model-bound não concede RESOLVED. Material real e autorização precisam de prova; fixture não substitui nenhuma delas.
4. Sob parent administrativamente controlado e não-reparse, o MESMO owner solicita criação exclusiva de um contêiner novo. Deve observar sucesso explícito de criação e adquirir custódia nativa contínua da identidade desse objeto antes de aceitá-lo. Contêiner já existente, ainda que vazio, não é adotado; sucesso de consulta de ausência não basta. O mecanismo de lease/path do adaptador deve ser qualificado contra renomeação/substituição e corridas do fault model; rechecagem antes/depois sozinha não prova custódia contínua. Se o target não consegue garantir esse vínculo, UNSUPPORTED/DENY, sem improvisar lock/marker.
5. Vincular em memória, na preparação existente, ator, declaração, invocação, identidade do parent/contêiner, refs e root domain candidato. É evidência da cerimônia em curso, sem novo ID operacional ou registry. Revalidar antes do efeito. Qualquer indício de store, instalação predecessora, restore, recurso retido ou tentativa anterior incerta nega bootstrap. Declaração falsa/conluio/host privilegiado comprometido não são detectados pelo filesystem; são limites explícitos, não segurança comprovada.
6. Preparar o candidato D3 integral: R candidato1/predecessor0, base e LST1 ausentes, carrier/I ausentes, checkpoint zero. R1 é revisão candidata, não CURRENT nem epoch. Gerar root domain pelo owner existente não comprova história. Manter coleção e target-capacity rejection; não truncar.
7. Congelar packet e encaminhar ao MESMO worker, com modo inicial explícito e expected predecessor ABSENT (não blob zero). CREATE_NEW do store no mesmo contêiner sob custódia, write integral, flush, closes e commit factual. Não reutilizar o leitor ROOT06 ou ramo OPEN_EXISTING para fingir bootstrap. Se o mecanismo de persistência não for suportado/durável no target, negar; não trocar backend silenciosamente.
8. Sucesso nativo completo fornece somente COMMITTED_PENDING e recibo. CURRENT depende dos participantes, seleção imutável, pins e autorização aplicável da mesma root; permanece DENY nesta revisão. A/P somente após commit e pela autoridade existente, nunca contadores de teste. CP8 não contém prova da cerimônia nem substitui autorização.
9. Auditoria usa o logger durável/recibos existentes, com ator/referências/identidades/resultados não secretos. Logs observam a cerimônia, não a autorizam. Não colocar novos campos em reserved D3 nem criar sidecar. Persistir evidência adicional ou retomá-la após crash exige contrato específico se o formato atual não a contém. A menor versão proposta não retoma autoridade de bootstrap após restart.

## Estados e evidências

| Estado encontrado | Classificação/evidência | Disposição |
|---|---|---|
| Novo objeto-contêiner criado nesta cerimônia, custódia contínua, admin e refs válidos, sem indicativo predecessor | NEW_LOCAL_CEREMONY; positivo local condicionado ao novo contrato | elegível apenas após aceitação/qualificação; hoje DENY |
| Contêiner/store existente com root válida | EXISTING | update exige CURRENT/pin/CAS ou recuperação própria; não bootstrap |
| Store existente vazio/truncado/corrompido | CORRUPT_OR_PARTIAL | DENY e conservar; não apagar/recriar |
| Store ausente em diretório antigo ou de origem incerta | UNKNOWN_HISTORY | DENY, mesmo com declaração ou CREATE_NEW bem-sucedido |
| Backup, cópia, migração ou restauração conhecida | RESTORE | não emitir nova identidade para contornar; checkpoint/fencing/história pelo gate próprio |
| Processo de inicialização anterior interrompido, contêiner criado e store talvez ausente | INTERRUPTED_OR_UNKNOWN | DENY; consulta/reconciliação de resultado, sem repetir cerimônia automaticamente |
| Commit confirmado nesta invocação, CURRENT ainda ausente | COMMITTED_PENDING | preservar conteúdo/packet/leases; não iniciar novamente |
| Commit/rollback/close incerto | RETAINED_UNKNOWN | Stop sem espera ilimitada; memória/handles/créditos retidos; nenhuma autoridade nova |

## Concorrência e falhas

- Duas tentativas no MESMO alvo: criação exclusiva do contêiner e depois do store dão um vencedor; perdedor não adota objetos do vencedor. CREATE_NEW não prova exclusividade global entre instalações diferentes.
- Parent/path trocado, reparse, lease inválida ou direitos insuficientes: rejeitar antes do efeito; incerteza após efeito conserva recursos. A forma nativa exata deve ser provada, não presumida por possuir um handle.
- O worker usa packet próprio; token e handles da cerimônia têm ownership explícito e nenhuma dependência do owner é fechada enquanto houver borrower. A montagem/validação administrativa não vai para o hot path/Stop; nenhuma consulta nativa é alegada cancelável/limitada sem evidência.
- Stop antes do commit impede efeitos novos; Stop durante persistência só sinaliza e observa, não espera consulta nativa, não mata thread e não usa timeout como prova de ausência de efeito.
- Write/flush parcial, commit incerto ou close falho: manter RETAINED/UNKNOWN e charge. Rollback factual só libera o comprovadamente não consumido. Não deletar store/contêiner como cleanup de conveniência; não criar nova CA/root/serial para compensar.
- Crash elimina a cadeia de custódia em memória. Contêiner/store remanescente não é novo; diretório/arquivo ausente tampouco prova que não houve efeito. Primeira versão não possui retomada autoritativa; recuperação permanece DENY enquanto seus gates não estiverem comprovados.

## Obrigação normativa exata em aberto

D2 exige “instalação/store alvo factualmente nunca inicializado, nenhuma root/CURRENT/história anterior”. D3.6 exige “exclusive virgin creation with external absence/history proof”. Os mecanismos atuais só atestam observações locais, não a negativa histórica completa. Há duas escolhas reais: manter a leitura estrita e continuar DENY até fonte positiva de história aprovada, OU aceitar explicitamente a especialização local proposta — criação observada+cústodia contínua de NOVO objeto e declaração administrativa com responsabilidade e limites. Bater um novo hash ou chamar CREATE_NEW não resolve essa decisão.

Recomendação: segunda alternativa apenas para nova instalação local sem issuer, sem restore/adoption, com autorização OS administrativa qualificada e nenhuma alegação de ausência global. Requer aceitação específica de CONTRACT-LOCAL-BOOTSTRAP-PROPOSED.md e do adaptador/ator/escopo; nenhuma norma vigente é alterada nesta revisão. Se o owner exigir ausência histórica/global mais forte, a alternativa local é insuficiente e INITIAL_BOOTSTRAP continua DENY.
