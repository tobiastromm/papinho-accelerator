---
status: accepted
date: 2026-10-19
task: PACC-PHASE6-LOCAL-BOOTSTRAP-CEREMONY-ACCEPTANCE-1
---
# Decisão — cerimônia local de primeira inicialização

## Aceitação e precedência delimitadas

A decisão condicional explícita do owner aceita exclusivamente P0–P9 abaixo, mediante aprovação oficial desta escrita. A proposta histórica permanece integralmente preservada. As expressões de status futuro/proposed em P0 e P1 são históricas: este registro estabelece a aceitação normativa, sem autorizar implementação ou execução.

A precedência especializa somente o alcance da prova de primeira inicialização de D2 e da referência a external absence/history proof em D3.6: fatos locais observados de criação exclusiva e custódia contínua, acompanhados de declaração administrativa atribuída. Não afirma ausência global de histórico, não oferece fencing e não autoriza recuperação ou recriação de instalação perdida. Todas as demais condições D2/D4 e D3 Revision 3 permanecem vigentes; seus documentos e formatos não são alterados.

O contrato comum exige ator autorizado, ação explícita, identidade do contêiner, exclusividade, custódia e commit factual. Token, Administrators, SID e parent não-reparse em P1/P3 delimitam o perfil do adaptador Windows moderno; não impõem Win32/CNG ao núcleo comum. Outros adaptadores precisam comprovar propriedades equivalentes e permanecem negados até qualificação. Box/ISA não estão qualificadas.

Não são aceitos por referência patches, primitivas nativas, layouts adicionais, CLI implementada ou aprovação ED11. A conformidade normativa não comprova que o código atual realize a cerimônia. Stop nega novos efeitos; não apaga um commit já factual. Resultados incertos conservam ownership e retenção.

IMPLEMENTATION_AUTHORIZED=NO; INITIAL_BOOTSTRAP=DENY; CONFIG_CURRENT_PUBLICATION=DENY; AUTHORITATIVE_RECOVERY=DENY. CLIENT_ADMISSION=DISABLED; SITE_TLS_ADMISSION=DENY; ED11_ADMISSION=DENY.

## Texto incorporado P0–P9

P0. Este texto não está accepted e não autoriza código/execução. Se aceito futuramente pelo owner e executor, sua precedência será SOMENTE a especialização do alcance de “nunca inicializado/nenhuma história anterior” de D2 e “external absence/history proof” de D3.6 para a cerimônia local abaixo. Não modificar bytes/offsets/formatos D3/LST1/BODY0.5/ROOT06 nem revogar outras cláusulas. Como a aceitação atual não prova esse alcance, não implementar a especialização por inferência.

P1. INITIAL_BOOTSTRAP é operação administrativa explícita da única root, separada de update/restore, com ator efetivamente autorizado fora do candidato. Perfil inicial proposto: Windows moderno, administrador humano identificado por token efetivo com papel local Administrators habilitado e direitos qualificados sobre parent governado, distinto da execução operacional. SID informado pelo usuário, token operacional, autenticação de cliente, write no filesystem ou flag CLI não concedem essa autoridade. Não elevar automaticamente nem alterar ACL/política existente para obter aprovação. O token comprova identidade/contexto OS, não presença humana: a ação explícita é declaração atribuída ao ator e auditada como tal. O direito inicial é da administração local da MESMA root; não cria Principal/grant/provider/autoridade operacional paralela. Outros targets precisam de adaptador de autorização/custódia qualificado e negam até lá.

P2. O administrador declara explicitamente que pretende uma instalação nova sem predecessor ou restore. O software registra a declaração como declaração, distinta de observações nativas. Ela não prova ausência global de história; sozinha não satisfaz o procedimento. Qualquer indicação de identidade/história/credenciais/CA associadas a uma instalação predecessora, restore, instalação existente ou tentativa incerta nega bootstrap. Proibido reset/adoption/migração por este caminho.

P3. A prova positiva local exige que a MESMA preparação observe sucesso de criação exclusiva de NOVO objeto-contêiner para seu store, estabeleça sua identidade e custódia nativa contínua sob parent autorizado e não-reparse, e vincule a operação ao ator/invocação e ao candidato. Um contêiner existente não é elegível mesmo vazio. Ausência de arquivo/CURRENT, CREATE_NEW do arquivo, digest e file ID isolados são insuficientes. O adaptador deve comprovar sua proteção contra namespace substitution e concorrência no fault model declarado. Falha/ausência de capacidade, revalidação incoerente ou perda de custódia nega; não introduzir marcador/registro/serviço para compensar.

P4. “Novo” nesta especialização significa novo objeto de armazenamento cuja criação e custódia foram observadas nesta invocação, para uma nova identidade administrativa declarada pelo administrador. Não significa que o path/host/mídia jamais continha outra instalação, nem que backups/clones não existam. Tais afirmações permanecem NÃO COMPROVADAS. Não interpretar esta regra como fencing, recuperação autoritativa ou permissão de recriar instalação perdida. Se a implantação exigir garantia mais forte, ela não é suportada por esta cerimônia.

P5. Participantes, source/material/leases, refs/policy/capabilities e limites do target são validados pela mesma root antes de efeitos. Nenhum CURRENT anterior é exigido para este modo, e nenhum CURRENT é simulado. MODEL_BOUND não vira RESOLVED por flag. O candidato usa o D3 bootstrap aceito, sem base/LST1/I/carrier/CP de publicação; R1/predecessor0 são dados candidatos, não epochs/autoridade. SELECTED, named provider, issuer e demais gates permanecem nas limitações accepted aplicáveis.

P6. Persistência usa o mesmo store e worker transacional, com modo inicial explícito e predecessor ABSENT. Criação exclusiva e commit durável são obrigatórios e têm resultados separados. FULL COMMIT confirmado e closes factuais precedem qualquer publicação; CURRENT e A/P dependem da autoridade/ack existentes e não podem ser fabricados. Esta proposta não autoriza sua publicação ou listener/admissão. Atualização continua exigindo CURRENT factual, revisão-base, pin, refs e CAS; conflito/UNKNOWN nunca vira bootstrap ou fallback.

P7. Custódia e evidência da cerimônia pertencem à preparação existente, sem novo owner/registry/journal/pin system. Reutilizar logger e recibos existentes para auditoria não secreta; observação não concede autoridade. Sem nova persistência da prova em reserved bytes/sidecars. A primeira implementação não reconstrói prova de bootstrap após restart: história interrompida/ausente/incerta permanece DENY e vai à reconciliação/recuperação adequada, nunca a nova criação automática.

P8. Stop, quiescência, snapshots/pins/Sessions, Model E e ED08 permanecem integrais. Nenhuma consulta nativa pendente bloqueia Stop do owner indefinidamente. UNKNOWN/close failure/rollback não comprovado retêm handles/packet/memória/crédito. Não usar exclusão, retry destrutivo, timeout, FINAL remoto ou saída de processo como destruição/ausência de efeito. Não criar nova CA/generation/identidade para contornar resultado incerto.

P9. Bootstrap não concede admissão, grants, Network Egress, TLS, emissão, certificados ou listeners. Quota2097152 inalterada; censo compilado/lifetime e recursos transitivos são gates de implementação, não aprovação pelos7929 bytes disponíveis no cenário anterior. Provider/kernel UNKNOWN. Sem backend suportado, prova/refs/autoridade ou orçamento suficiente: DENY. Código, builds/testes futuros exigem patches exatos e revisão oficial. Esta revisão mantém IMPLEMENTATION_AUTHORIZED=NO e toda publicação/admissão negada.
