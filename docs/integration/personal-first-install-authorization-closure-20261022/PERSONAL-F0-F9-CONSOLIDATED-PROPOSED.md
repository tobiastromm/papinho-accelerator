---
status: proposed
revision: 2
date: 2026-10-22
task: PACC-PHASE6-PERSONAL-FIRST-INSTALL-AUTHORIZATION-CLOSURE-1
---
# F0–F9 consolidado — primeira instalação pessoal/local

Texto consolidado mínimo preparado sob a decisão explícita do owner. Este arquivo permanece PROPOSED como proveniência; eventual aceitação será registrada separadamente após revisão oficial. Substitui, SOMENTE como texto candidato à aceitação, as três propostas anteriores de F0–F9/correções; preserva seus arquivos. Institucional é direção futura fora deste escopo, sem infraestrutura corporativa obrigatória.

## F0 — Escopo

O perfil atual é PERSONAL_LOCAL: proprietário ou administrador legítimo inicia voluntariamente uma primeira instalação no equipamento que administra. NEW_INSTALL é operação explícita da única root, distinta de UPDATE, restore, reset, adoção e recuperação. Não exige CURRENT/predecessor anterior nem a CA que ainda será criada. Ausência de arquivo/CURRENT não autoriza bootstrap. Este contrato define a autorização/associação local e não modifica formatos persistidos, wire, algoritmos, quotas, worker, root ou pins.

## F1 — Origem local e compatibilidade com P1

A origem da autorização é o ato explícito do proprietário ou administrador legítimo sob esta política de primeira instalação, previamente fixada e aceita fora da configuração candidata. A política não pode ser criada ou alterada pelo pedido que pretende aprovar. Não se exige aprovação institucional, serviço externo, Active Directory, Configuration Manager, nuvem, CA de aprovações ou store de autorizações. Institucional permanece DEFERRED, sem aceitação de fonte ou protocolo institucional.

A admissibilidade local exige conjuntamente: (a) identidade/contexto nativos elegíveis e direitos factuais sobre o parent governado; (b) declaração atribuída de que o operador é proprietário/administrador legítimo e pretende uma instalação nova, não restore/substituição; (c) apresentação e confirmação deliberada do plano exato em F3; (d) associação protegida, revalidação, exclusividade, custódia e gates restantes. Membership, elevação, posse física, write no filesystem ou flag CLI isolados não autorizam. O perfil confia no controle administrativo legítimo do ambiente local e na declaração atribuída dentro desse limite; não afirma que o OS comprova propriedade jurídica, presença humana ou ausência global de história. Comprometimento do OS/administrador, declaração fraudulenta, clones e fencing estão fora dessa garantia. Contradição/indício de instalação anterior ou resultado incerto implica DENY.

Esta é a especialização explícita do mecanismo de autorização local de P1, não alteração de seu texto: ator fora do candidato, administração da mesma root, direitos qualificados e ação atribuída continuam obrigatórios. P2–P9 e D2/D4 permanecem. A declaração de legitimidade é registrada como declaração, não convertida em observação nativa. A política local aceita e os fatos/ação acima substituem, neste perfil SOMENTE, a exigência proposta de uma fonte externa emitir uma aprovação individual. Não há nova autoridade persistente.

## F2 — O que é provado e quem decide

O backend nativo comprova identidade/contexto técnico, elegibilidade efetiva e direitos sobre o parent retido, conforme garantias e erros de sua plataforma. Windows candidato usa token efetivo próprio, rejeita impersonação segundo o adapter de bootstrap, verifica papel habilitado, contexto de elevação aplicável e direitos sobre o objeto governado. Não aceitar SID/texto do caller como identidade. Outros backends demonstram propriedades equivalentes sem SID/Win32/CNG no núcleo. Falta/UNKNOWN de garantia nega; Windows/box/ISA não são qualificados por este texto.

O OS NÃO é declarado autor de aprovação criptográfica do plano, nonce, legitimidade, finalidade NEW_INSTALL ou configuração. O operador fornece a declaração atribuída e a confirmação do plano. O adapter comprova correspondência e integridade da associação entre esses elementos e os fatos nativos protegidos. A decisão final de admissibilidade pertence ao Accelerator, sob esta política aceita e TODOS os contratos/gates aplicáveis; identidade técnica, confirmação ou associação correta isoladas nunca dispensam os demais requisitos.

A evidência da tentativa distingue: revisão identificável desta política local aceita; identidade/contexto observados; declaração atribuída; plano/configuração exatos; confirmação; identificador local F4; referências/owners e seus lifetimes; estado de validade/consumo/invalidação; resultados nativos e erros. Não fabricar campos de aprovação/revogação externos. No perfil pessoal, APROVAÇÃO_EXTERNA=NÃO_APLICÁVEL e identificador de aprovação significa o identificador local do ato F4, sem alegar origem OS/institucional. Não aceitar booleano/digest isolado como evidência de toda a cerimônia.

## F3 — Plano exato, alvo e proteção contra substituição

Antes de confirmar, a interface apresenta o propósito NEW_INSTALL, identidade observada do operador, declaração de legitimidade/nova instalação, alvo governado (parent por identidade nativa retida e nome relativo exato do contêiner), configuração inicial exata e suas referências/revisões, política aplicável, identificador F4 e consequências: identidade administrativa exclusiva; custódia privada local da CA; restrições de exportação; separação da credencial TLS; efeitos persistentes/UNKNOWN; publicação somente após todos os gates. Não apresentar chave privada, senha ou outro segredo; não omitir campos semanticamente ativos. Valores sensíveis necessários permanecem em referências protegidas sem impressão ou logging de seus conteúdos. Se a interface não conseguir apresentar/identificar inequivocamente o plano e seus efeitos, DENY.

A confirmação é ação distinta da mera passagem da flag NEW_INSTALL ou execução elevada; refere-se ao plano apresentado e à tentativa corrente. Não conta como aprovação uma resposta reaproveitada de outro plano/invocação. Não se congela nesta norma sintaxe CLI/wire, API ou novo arquivo de aprovação. A implementação deve demonstrar caminho de apresentação/entrada atribuível no contexto elegível; não alegar presença humana provada pelo token ou por bytes em stdin.

O mesmo owner canônico mantém plano/configuração/backings imutáveis e referências protegidas da apresentação até o fim de cada consumo legítimo. Hash pode detectar alteração mas não substitui bytes/referências, identidade e proveniência. Qualquer mudança de operador/contexto relevante, parent, nome, configuração, referências, revisão de política ou tentativa invalida a confirmação; reabrir path ou copiar strings não preserva identidade. Revalidar identidade/direitos/plano/Stop antes da submissão e no worker imediatamente antes do efeito; sem mecanismo capaz de fazê-lo com lifetime factual, DENY.

Antes da criação não existe identidade factual do filho. Depois de UMA criação exclusiva sob o parent retido, anexar a identidade nativa observada do novo objeto ao MESMO ato e manter custódia contínua. Contêiner existente, substituição, divergência ou perda de custódia nega. Não criar file ID/pin fictício, apagar/renomear alvo ou usar outra instalação para completar o plano.

## F4 — Identificador, validade e proveniência

Gerar para cada tentativa um identificador opaco de 32 bytes pelo CSPRNG existente qualificado, antes da apresentação. Falha/indisponibilidade implica DENY, sem fallback de relógio/contador. Distinguir tentativas vivas e rejeitar colisão observada. O domínio é o ato local e a invocação canônica: não é licença comercial, serial da CA, identidade da root, segredo, assinatura, grant ou prova de unicidade global. O adapter liga esse identificador à mesma identidade/contexto/plano/owner; conhecê-lo ou repeti-lo não autoriza.

A validade é por evento/contexto: mesma invocação viva, mesma política/plano/referências protegidos, elegibilidade nativa corrente, confirmação válida e nenhuma invalidação. Não exige UTC que só a instalação futura poderia fornecer. Se outro contrato aplicável exigir UTC/prazo/freshness qualificados, a falta dessa prova continua DENY; não ignorar condição de validade de issuer/material. Suspensão ou perda de continuidade, alteração relevante, Stop, cancelamento e restart invalidam novos efeitos. Não há recibo transferível/restaurável nem autorização atemporal.

Proveniência e auditoria usam logger/recibos existentes, distinguindo política, observação nativa, declaração e confirmação, sem segredos. Não persistir prova operacional em reserved bytes, marker, sidecar ou registry paralelo; histórico não restaura autoridade.

## F5 — Consumo único e admissibilidade

No máximo uma tentativa local viva por owner; sem novo pool/tabela de autorizações. Estados lógicos: CAPTURED (entrada sem autorização), PRESENTED (plano protegido apresentado), CONFIRMED (ação atribuída ao plano), CONSUMED (tentativa tomada irrevogavelmente), INVALIDATED, FINISHED e RETAINED_UNKNOWN. São metadata privada, sem wire/CURRENT novo. O estado de consumo é monotônico: nenhum estado posterior retorna a CONFIRMED nem autoriza outro efeito pelo mesmo ato.

O Accelerator só considera admissível uma tentativa confirmada se toda a conjunção F1/F2/F3, reservas e contratos vigentes for satisfeita. Transição indivisível CONFIRMED -> CONSUMED imediatamente antes da primeira submissão capaz de criar o alvo. Só a submissão correlacionada pode executar UMA criação exclusiva; worker revalida e não gera outro ato. Repetição, concorrência, outro owner/invocação/plano/identificador ou reapresentação de confirmação negam. Não emitir autoridade positiva porque o consumo ou nonce é válido.

Falha de submissão/thread ou falha nativa definitiva não rearma o ato. Nova tentativa exige nova apresentação, confirmação e identificador e prova de disposição/quiescência da anterior. Se o objeto foi criado, não se torna novo novamente por nova aprovação. Resultado incerto/retido impede reutilização ou nova tentativa conflitante; nunca retry automático, troca de nome ou reset destrutivo para contornar.

## F6 — Cancelamento, revogação local, Stop e restart

Não há autoridade externa de revogação obrigatória neste perfil. Antes do efeito, cancelamento explícito da tentativa, Stop, perda de elegibilidade/contexto, alteração do plano/política/referências ou perda de custódia revogam a admissibilidade local. Distinguir VALID (todas as condições locais efetivamente observadas), INVALIDATED e UNKNOWN; UNKNOWN nega novos efeitos, não equivale a NÃO_REVOGADO presumido. A revogação aqui é da tentativa, não status de certificado/issuer ou revogação operacional de Sessions.

Antes do consumo, invalidação impede submissão. Após consumo, pedir Stop/cancelamento usa os mecanismos existentes e nega novos efeitos onde ainda possível; não garante que IO nativo pendente foi cancelado. Efeito ou cleanup incerto conserva packet, backings, handles e créditos no mesmo owner como RETAINED_UNKNOWN. Não declarar rollback/devolução do ato, liberar por timeout, dar double-free ou descartar evidência enquanto houver worker/borrow legítimo. Liberação somente após quiescência e fechamento factuais; invalidação de autorização não destrói memória.

Restart não restaura confirmação/consumo/autoridade por arquivo íntegro, log, novo token ou repetição CLI. Histórico interrompido/existente/incerto segue reconciliação/recuperação governada e permanece DENY; não recriar root/CA nem tentar outro path. Não prometer impedir clonagem global ou provar ausência de backups.

## F7 — CA, transação e publicação

O plano pessoal pode solicitar explicitamente estabelecer a custódia da CA local, mas isso deve ser apresentado/confirmado como ato administrativo CA_INITIALIZE distinto, no mesmo contexto quando suportado, conforme cláusula6 do issuer. Autorizar NEW_INSTALL não autoriza CA por efeito implícito. A mesma interação pode confirmar ambos os atos apenas se nomear inequivocamente os dois propósitos, seus escopos e consequências; cada efeito exige sua autorização e revalidação próprias. Não exigir uma CA anterior para autorizar a criação inicial; tampouco substituir autorização CA por UPDATE.

Reutilizar o owner de chave e proteções existentes, sem material privado na CLI, exportação implícita, CA como credencial TLS ou startup regeneration. Identidade/geração e key/cert correspondence precisam ser factuais, conforme os contratos de issuer. Falta de backend/custódia/persistência/recursos ou disposição incerta implica DENY/retenção. Não alegar atomicidade física entre criação de contêiner, chave e store se o backend não a comprovar: observar efeitos separados, impedir publicação parcial e reconciliar sem overwrite/recriação.

Bootstrap v1 sem carrier e transições posteriores continuam em seus contratos; esta norma não inventa um bootstrap v4 nem remove histórico/issuer para acomodar formatos. Prepare/validate/commit durável, CAS de identidade e predecessor quando aplicável, CURRENT e A/P/I preservam suas autoridades e ordem. Bootstrap inicial não recebe predecessor fictício. Publicação só após todas as validações, persistência/custódia/ack e gates próprios; confirmação pessoal ou commit isolado não publica CURRENT, concede grants, admissão ou assinatura.

## F8 — Portabilidade e ED11

O núcleo expressa política local/ato, identidades abstratas, plano/referências, estados, ownership, consumo e resultados. Windows é primeiro backend candidato, com fatos nativos e proteção de objetos/contas do OS; não introduzir SID/Win32/CNG no núcleo. Box/ISA e outros targets precisam adapters equivalentes e permanecem não qualificados. Não selecionar fornecedor/serviço de aprovação.

Censo pré-patch deve incluir todos os custos: ato/identificador, plano/backings e cópias realmente vivas, buffers de apresentação, referências/material/CA, token/parent/contêiner/worker, heap/stack/CRT/provider/kernel, concorrência, retenção e host admission. Não usar o tamanho de um nonce como pico, omitir transitivos, duplicar charges compartilhados ou presumir reutilização. Nenhuma quota aumenta; Windows3MiB só por seleção explícita, Legacy/box/ISA inalterados. UNKNOWN material conserva gate operacional DENY, ED11 PARTIAL.

## F9 — Qualificação e limites da aceitação

Implementação posterior exige patches exatos revisados, preimagens, storage/lifetime e orçamento demonstrados. Testar separadamente fato nativo, declaração e autorização da cerimônia: token/elevação/direitos insuficientes; legitimidade declarada versus observada; plano/configuração/alvo trocados; apresentação incompleta; confirmação ausente/replay; consumo concorrente; criação exclusiva/existente; falhas antes/depois de criação; Stop/cancelamento/invalidação; UNKNOWN/cleanup/borrow/retidos; suspensão/restart; custódia CA e ausência de autorização CA; CAS; ED11 e regressões históricas. Modelos/LAB não qualificam autoridade produtiva.

Aceitação normativa pessoal não qualifica implementação, plataforma, fonte factual, custódia, emissão, capacidade operacional ou recuperação. Não autoriza código, bootstrap, gerar chave/certificado, CURRENT, assinatura, rede, release ou mudança de produção0.6.4 nesta tarefa. IMPLEMENTATION_AUTHORIZED=NO; BOOTSTRAP_EXECUTION=NOT_RUN; CURRENT_PUBLICATION=DENY; SIGNATURE_AUTHORITY=DENY; AUTHORITATIVE_RECOVERY=DENY; ED11_TOTAL_STATUS=PARTIAL. Institucional permanece DEFERRED.
