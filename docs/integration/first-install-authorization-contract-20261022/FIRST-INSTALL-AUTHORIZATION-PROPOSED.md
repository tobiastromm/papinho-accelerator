---
status: proposed
revision: 1
date: 2026-10-22
task: PACC-PHASE6-FIRST-INSTALL-AUTHORIZATION-CONTRACT-1
---
# F0–F9 — Origem e consumo da autorização de primeira instalação

Este contrato focal é PROPOSED. Não modifica P1 accepted, não prova fonte disponível e não autoriza implementação ou execução. A aceitação futura deve delimitar expressamente sua relação com P1/P2/P3/P7/P8 da decisão local de bootstrap de 2026-10-19 e D2/D4. Nenhuma proposta/handoff anterior é incorporada por referência. Não cria formato persistido, wire, CA, autoridade de assinatura, registry ou store de aprovações.

## F0 — Escopo e separações

O objeto é a autorização de UMA tentativa NEW_INSTALL para criação exclusiva e preparação inicial da única root canônica, atribuída a operador específico. Não é autorização UPDATE, restore, reset, adoção, migração, geração de CA, assinatura, publicação CURRENT ou admissão. Bootstrap inicial não exige CURRENT anterior. A configuração candidata e a confirmação CLI não são a origem da autorização. Uma criação exclusiva bem-sucedida não substitui a aprovação, nem a aprovação substitui criação/custódia factuais.

## F1 — Origem legítima, anterior e independente

A autoridade originária é o responsável legítimo pela administração e implantação do equipamento, ou seu delegado de provisionamento, segundo política institucional ou de propriedade já vigente fora do Accelerator. Sua legitimidade não nasce da root que será criada. Para equipamento pessoal, o responsável pode ser seu proprietário/administrador legitimamente designado; para equipamento institucional, exige-se a designação da instituição. O autor da aprovação e o operador podem ser a mesma pessoa SOMENTE se uma política independente permitir e provar essa competência; a igualdade de identidades não prova competência.

A política da plataforma deve identificar uma fonte administrativa já existente capaz de comprovar essa designação e seu escopo. Pode ser um mecanismo governado de gestão/provisionamento já implantado ou política administrativa nativa protegida que expresse essa competência. Não basta inventário textual, nome informado, ticket copiado, senha de administrador, membership genérico, acesso ao filesystem ou alegação do operador. Não se cria serviço de aprovação, grupo privilegiado, arquivo de autorizações ou segunda autoridade para preencher sua ausência. Se nenhuma fonte verificável já existir para o deployment, SOURCE_ABSENT/DENY. Não há fonte concreta declarada disponível nesta proposta.

Custódia física isolada não prova legitimidade. Operador terceirizado precisa de delegação demonstrável. Uma instituição pode impor condições adicionais; elas não relaxam P1. Comprometimento do administrador/OS ou fraude na designação não são resolvidos por este contrato; não alegar fencing ou prova global de propriedade.

## F2 — Prova administrativa verificável

O adapter consulta a fonte de F1 e obtém referência protegida à decisão específica; o núcleo não aceita um booleano ou digest isolado. A prova reúne, com associação verificável:

- identidade e competência do responsável autorizador, origem e revisão da política/delegação independente;
- identidade nativa do operador e contexto de execução elegível;
- decisão explícita AUTORIZAR NEW_INSTALL, propósito BOOTSTRAP_INITIAL_CREATE_AND_PREPARE e ausência de autorização implícita de outros efeitos;
- contexto de alvo de F3, configuração inicial exata e suas referências protegidas/revisões;
- identidade da invocação canônica e nonce público de 32 bytes obtido pelo CSPRNG existente; não é chave nem identificador de root;
- origem/autenticidade, revisão/status da decisão, meios de revalidação, condições de validade e invalidação;
- owner da fonte, borrow/pin correspondente, tamanho e lifetime factual da evidência.

Esses campos são requisitos semânticos, não um novo arquivo/token serializado, ABI ou protocolo. A fonte deve autenticar sua decisão e sua associação ao pedido completo por mecanismos já aprovados para aquela plataforma. O adapter não pode preencher lacunas com strings, campos CLI ou uma assinatura cuja chave foi escolhida pelo pedido. Política/delegação deve ser resolvida fora do candidato por uma cadeia de autenticação administrativa já estabelecida. Sem prova completa, protegida e revalidável, DENY.

A revisão da decisão é da própria fonte; não é CURRENT/R/A/P/I. O nonce só correlaciona uma aprovação já legítima: sozinho nunca autoriza. Não há exigência de uma CA Accelerator anterior, PapinhoTrustStore, internet ou UTC prévio.

## F3 — Alvo antes e depois da criação

Antes do consumo, o alvo é o parent governado retido por identidade nativa, a plataforma/deployment legitimamente abrangida pela política e o nome relativo exato do novo contêiner. Direitos do operador sobre esse parent devem ser comprovados, sem elevar privilégios ou alterar ACL para obter aprovação. Nome de host/path/digest sozinho não basta. O nome ainda não designa um objeto-filho existente e não deve receber file ID fictício.

A mesma decisão vincula a configuração validada e suas referências protegidas. Alteração do parent, nome relativo, configuração, identidade do operador, política, revisão ou invocação invalida o ato antes do efeito. Não reabrir pelo spelling para substituir o parent retido.

Depois de UMA criação exclusiva, a identidade nativa observada do novo contêiner e seu resultado são anexados ao mesmo contexto existente, preservando o vínculo parent/nome/operador/ato/configuração. Essa observação não amplia a autorização. Contêiner preexistente, troca de namespace ou identidade incoerente nega; não apagar, renomear ou trocar de alvo para forçar sucesso.

## F4 — Captura e validade sem circularidade temporal

A CLI existente apresenta a intenção explícita NEW_INSTALL. O adapter identifica o operador real, verifica elegibilidade e obtém a prova F2 da fonte independente para o pedido F3. Não aceita SID/identidade textual como prova, não promove configuração ou flag CLI a aprovação e não reutiliza autorização UPDATE.

A validade intrínseca do ato é por evento e contexto: somente durante a invocação viva, com fonte/política/decisão elegíveis, referências protegidas, configuração imutável e nenhuma invalidação. A captura deve permanecer vinculada até a revalidação imediatamente anterior ao efeito; não existe validade indefinida de um recibo devolvido à CLI. O adapter não pode congelar status mutável sem garantia explícita de snapshot/freshness fornecida pela fonte.

Se a política externa exigir expiração UTC, prazo máximo ou outro relógio, a plataforma deve provar esse requisito por sua fonte temporal já qualificada; ausência/UNKNOWN nega. Não substituir por relógio local presumido, nem exigir UTC para o perfil puramente vinculado à invocação. Suspensão, perda de continuidade da fonte ou contexto, restart, troca de operador/política/alvo/configuração, revogação, expiração aplicável e Stop invalidam novos efeitos. Revalidar fonte, identidade, direitos e Stop imediatamente antes de despachar e novamente no worker imediatamente antes de criar; sem capacidade de revalidação no mesmo contexto, DENY.

## F5 — Consumo único

Há no máximo UM ato de first-install vivo no owner canônico por vez; não há nova tabela/pool de atos. Estados lógicos: CAPTURED (entrada sem autoridade), AUTHORIZED (prova F2 completa), CONSUMED (tentativa irrevogavelmente tomada), INVALIDATED (não autoriza efeitos), FINISHED (disposição factual) e RETAINED_UNKNOWN (efeito/cleanup incerto). Não são novos estados wire ou CURRENT.

O owner realiza uma transição indivisível AUTHORIZED -> CONSUMED antes da primeira submissão ao worker que possa criar o contêiner. Ela exige todas as revalidações, reservas e ausência de Stop. Só a submissão correlacionada pode usar o ato consumido para UMA chamada de criação exclusiva. Revalidação no worker não emite outro ato nem repete o consumo. Chamadas duplicadas, concorrentes, outro owner/invocação/nonce/alvo e retransmissões não recebem permissão. A fonte deve vincular sua decisão ao contexto completo; um ID ou nonce esperado pelo caller não prova esse vínculo.

Falha de criação do thread ou falha nativa definitiva não devolve o ato a AUTHORIZED. Nova tentativa exige nova decisão explícita da fonte, novo nonce e prova de quiescência/disposição da tentativa anterior. Não há retry automático, mesmo para o mesmo alvo. Se a tentativa criou um contêiner, nova aprovação não o torna novo outra vez. Não aumentar concorrência ou descartar retenção para obter outro ato.

Este consumo cobre a tentativa local na mesma invocação. Não prova unicidade global entre hosts/clones, nem fornece fencing. A política independente continua responsável por limitar sua competência de implantação; não inferir autorização de instalação duplicada. Nenhum registro persistente paralelo é introduzido para fingir essa garantia.

## F6 — Stop, falhas, retenção e restart

Antes do consumo, falha ou Stop invalida o ato e nega submissão. Após consumo, Stop nega novos efeitos onde ainda possível e usa os mecanismos existentes, sem garantir cancelamento nativo. Se criação pode ter ocorrido ou cleanup falhou, RETAINED_UNKNOWN conserva o mesmo packet, referências, handles e créditos; não se declara rollback nem devolução da autorização.

Não liberar evidência ou backings enquanto worker/callback/borrow legítimo puder usá-los. Invalidação de autoridade é distinta de liberação de memória. Liberação só após quiescência e fechamento factuais, com exatamente uma disposição dos recursos. Timeout não prova liberação, ausência de efeito ou direito de tentar novamente. Resultados e erros permanecem consultáveis pelo owner existente.

O ato não é exportável nem restaurável. Após restart nenhuma prova em arquivo/log, declaração repetida ou novo token restaura AUTHORIZED. Aprovação antiga vinculada à invocação/nonce encerrados é rejeitada. Histórico criado/interrompido/incerto deve ser reconciliado pelos contratos existentes; não gerar outra identidade/CA nem repetir bootstrap em outro path para contornar UNKNOWN. P7 não é alterado: sem sidecar/marker novo ou prova persistida em reserved bytes.

## F7 — Custódia CA e limites de publicação

O ato de first-install não autoriza criar, abrir ou usar chave CA por si só. Uma referência à custódia já existente só pode vir do owner de chave e da autorização administrativa próprios do issuer, com identidade/geração/correspondência/status/lifetime factuais, borrow explícito e Stop/retenção preservados. Ausência de referência é registrada como ausente; não criar chave/certificado para completar o ato.

Inicialização CA é ação distinta sob cláusula6 da decisão issuer. A futura composição das duas cerimônias não pode tornar cada uma dependente de autoridade que só a outra criaria. Bootstrap inicial v1 continua sem carrier issuer; first-install não concede assinatura, CURRENT, A/P/I, grants ou listeners. Commit e publicação conservam seus gates próprios, inclusive validação imediatamente anterior ao efeito.

## F8 — Adapters e orçamento

Windows: reutilizar a identificação nativa do token e o owner/worker de custódia existentes. Exigir contexto efetivo, ausência de impersonação conforme o adapter de bootstrap, identidade estável, membership habilitado, contexto de elevação aplicável e direitos sobre parent retido. Esses fatos comprovam elegibilidade técnica; a decisão F2 proveniente da política independente continua obrigatória. O perfil deve demonstrar quais consultas/checagens satisfazem cada fato; não inferir elevação ou legitimidade de uma consulta TokenUser. Não instalar novo grupo/ACL/política automaticamente. Fontes administrativas e adapters concretos permanecem não selecionados/não qualificados nesta proposta.

Box/ISA: administrador ou provisionador legitimamente designado, autenticador e política do equipamento, identidade nativa equivalente do alvo e exclusividade/custódia devem ser comprovados por seus próprios adapters. Presença física, jumper ou acesso ao barramento isolados não equivalem a autorização. Outros targets seguem as mesmas obrigações. SID, CNG, Win32, NCrypt e formato de token não pertencem ao núcleo portável.

Contabilizar ato/nonce, evidência/borrows da fonte, parent/contêiner/token, configuração/CA, worker, stack/CRT, falhas retidas e coexistência no mesmo censo ED11 antes de patch/efeito. Não usar nonce32 como custo total nem duplicar cobrança de packet já contado. Tamanhos e custos transitivos do adapter devem ser limitados por perfil antes da implementação; UNKNOWN material mantém gate negado. Sem novo pool, journal ou quota. Windows3145728 somente com seleção explícita; demais limites inalterados. ED11 total PARTIAL.

## F9 — Qualificação exigida e aceitação delimitada

Revisão exata deve demonstrar origem externa legítima verificável, associação F2/F3, transições F5, retenção F6 e ausência de mudança em P1 e issuer. Antes de implementação, identificar fonte real e sua cadeia de autenticação/política, capacidades de revalidação, dimensões/lifetimes e símbolos concretos do mesmo owner. Fonte apenas abstrata não qualifica caminho positivo.

Testes futuros devem distinguir modelos de autoridade de fontes factuais. Cobrir operador elegível sem competência; competência independente válida/ausente/revogada; política/decisão divergente; token/SID/elevação; configuração/parent/nome trocados; decisão de outra invocação; nonce/replay; corrida de consumo; falha antes/depois de consumo; alvo existente; criação exclusiva; falha após criação; Stop em cada fase; UNKNOWN; borrows/cleanup/retidos; suspensão/restart; CA ausente; ED11. Uma aprovação de laboratório não autoriza instalação produtiva. Nenhum desses testes foi executado nesta tarefa documental.

Somente revisão e decisão explícitas futuras podem aceitar F0–F9. A aceitação não seleciona fonte/backend, não aplica patches, não qualifica targets e não autoriza cerimônia. Até lá: IMPLEMENTATION_AUTHORIZED=NO, BOOTSTRAP_EXECUTION=NOT_RUN, CURRENT_PUBLICATION=DENY, SIGNATURE_AUTHORITY=DENY, AUTHORITATIVE_RECOVERY=DENY.
