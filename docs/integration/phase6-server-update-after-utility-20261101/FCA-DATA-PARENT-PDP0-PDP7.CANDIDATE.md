# PDP0–PDP7 — preparação do parent de dados FCA sob ancestral Windows compartilhado

CANDIDATE / PROPOSED / NOT_ACCEPTED / NO_DEPENDENT_IMPLEMENTATION / NO_OPERATIONAL_AUTHORITY.
2026-11-03. Proposta focal única, motivada pela leitura consumida de pedido SHA25639cf3699b037983e9bfd84959740b170eab502b2c6a1a319f27215f9490e7ae3.

## Evidência e fronteira

O owner recebeu na leitura independente: ProgramData KnownFolder local C:/ProgramData; original de volume417614100:327680:5; original ProgramData417614100:131072:227641; projeção da política existente REJECT na ACE4 BUILTIN Users mask0x116. A abertura relativa Papinho retornou NTSTATUS0xc0000034, sem handle. Nenhuma revalidação final foi alcançada; referências do observador fecharam, autorização consumida, nenhum objeto criado. Não é lease de existência nem prova do futuro owner. O digest anterior OWNER|GROUP|DACL e o atual OWNER|DACL não devem ser comparados para inferir mudança de ACL.

Há duas lacunas relacionadas, não um defeito de recurso: o parent requerido ainda não foi adquirido porque a abertura da folha falhou, e a política conservadora rejeita permissões compartilhadas no ancestral Windows. Mesmo criar Papinho por fora não fecharia o segundo gate. Não editar ProgramData, escolher outro local, alterar quotas, remover OBJ_DONT_REPARSE ou reexecutar a coleta para evitar a decisão.

F0/F1/F2 exigem direitos factuais/originais, F3 continuidade e identidade factual depois de criação exclusiva, F5 consumo monotônico/uma criação por ato e F6 retenção. FCA1 mantém essas garantias e FCA4 limita o ato posterior a um contêiner Accelerator e uma chave. A norma vigente não concede criar um diretório adicional ou abrir exceção genérica a ancestrais graváveis. O código atual nega corretamente sob sua política conservadora. Este texto propõe somente a composição faltante, sem modificar documentos accepted existentes.

## Alternativas

A — Manter todos os limites atuais: preparação FCA permanece DENY com esta evidência. Não criar/remediar/migrar automaticamente. Nenhuma execução nova é necessária para demonstrar esse bloqueio no estado observado.

B — Aprovar indivisivelmente PDP0–PDP7 para integrar e qualificar OFFLINE o parent preparatório exclusivo e a fronteira compartilhada estritamente delimitada abaixo. Recomendada como menor caminho que conserva o destino vigente, não altera a ACL Windows e não amplia o ato FCA de um contêiner/uma chave. Aprovação não autoriza criar parent, coletar, UPDATE ou FCA real.

## PDP0 — Escopo e composição

Somente PERSONAL_LOCAL Selected AMD64/3MiB, preparação inicial para FCA0–FCA7. Reutilizar o launcher/owner/pending/worker e mecanismos administrativos existentes. Sem novo owner, serviço, canal, protocolo, diretório de distribuição, store de autoridades ou entrada CLI universal. Não altera READ, TRANSPORT_ONLY, UPDATE, NEW_INSTALL histórico genérico, NORMAL, recuperação, CA, emissão ou produção. Não há novos caminhos de dados: parent = ProgramData KnownFolder acrescido da folha fixa Papinho; contêiner FCA posterior = Accelerator.

## PDP1 — Uma criação preparatória distinta

Integrar um ato administrativo explicitamente restrito à preparação do parent, separado do NEW_INSTALL+CA_INITIALIZE posterior. No máximo UMA submissão FILE_CREATE de diretório Papinho, relativa ao original retido de ProgramData, com proteção final na criação. Não criar Accelerator nem chave nesse ato. Operação real futura exige pedido próprio, apresentação exata, operador elegível não impersonado, confirmação atribuída da única criação e riscos, CSPRNG original, gates frescos e consumo irrevogável antes da submissão. Não promover argv/path/hash/arquivo de decisão a autoridade. Reutilizar o mecanismo de apresentação e associação existente; não congelar novo wire/formato persistido ou CLI genérica por esta proposta.

Exclusividade é demonstrada pelo retorno FILE_CREATE, nunca por consulta anterior ou pela leitura consumida. Colisão, substituição ou objeto já existente => DENY, sem adoção, mudança de nome, overwrite, reparação ou retry. Não fabricar ID futuro: capturá-lo no retorno original e manter até readback/close. Falha ou criação parcial persiste para reconciliação, sem cleanup/refund. Resultado do parent não autoriza o ato FCA seguinte.

## PDP2 — Fronteira compartilhada estrita de ProgramData

Especialização de elegibilidade SOMENTE no objeto obtido da KnownFolder ProgramData e retido na cadeia nativa, no contexto PDP ou FCA explicitamente selecionado. A exceção não vale para string equivalente, namespace Device isolado, mapeamento histórico, ancestral arbitrário ou objetos abaixo dele. A resolução legítima permanece a existente; ausência de reparse e continuidade, identidade, owner confiável e DACL protegida permanecem obrigatórios.

Permitir como exceção conservadora apenas a ACE ACCESS_ALLOWED de BUILTIN Users S-1-5-32-545, flags exatamente CONTAINER_INHERIT (0x02, sem INHERIT_ONLY/INHERITED/callback), máscara exatamente0x00000116 observada. Nenhuma concessão de DELETE, DELETE_CHILD, WRITE_DAC, WRITE_OWNER ou GENERIC_ALL/GENERIC_WRITE por essa exceção. Demais ACEs continuam sob a política atual; ACEs não suportadas, outro trustee/flags/mask ou outro ancestral não recebem ampliação por fallback. A exceção AU self-only0x4 da âncora de volume permanece separada e não se amplia.

Isso é uma tolerância normativa focal à criação/escrita limitada no diretório compartilhado; não é prova de proteção efetiva completa desse ancestral. A segurança do Papinho exige também PDP3/PDP4 e os gates nativos abaixo. A implementação não pode concluir apenas pelo SD que handles preexistentes/compartilhamento/reparse/lifetime sejam seguros. Incompatibilidade desses gates mantém DENY e deve ser reportada; não enfraquecer o compartilhamento ou a política para forçar resultado.

## PDP3 — Proteção do novo parent e continuidade

Novo Papinho: owner original do operador elegível; DACL protegida final com allowlist exata operador/SYSTEM/Administrators, FILE_ALL_ACCESS e herança OI|CI, sem outros trustees e sem posterior SetSecurityInfo/ACL repair. Mesmo padrão protegido de diretório já validado na preparação FCA; não importar template TEST_ONLY de chave. Não alterar owner/DACL ou outros atributos de ProgramData/volume.

Antes e no worker: revalidar identidade, descritor/política, ausência de reparse, direitos efetivos, contexto, plano e recursos dos originais; abertura nativa direta do volume e componentes relativos, OBJ_DONT_REPARSE e criação relativa ao mesmo parent retido. Preservar a política de compartilhamento vigente e demonstrar compatibilidade das referências necessárias; sem caminho textual em substituição ao original. Se proteção/revalidação/compartilhamento não bastar para preservar a associação durante efeito, DENY. Nunca fechar referências por quota/conveniência ou assumir que uma ACL limita recursos internos/latência.

Readback do Papinho no próprio retorno: identidade, diretório/semreparse, owner/DACL final e continuidade. Fechar uma vez pela sequência existente somente quando quiescente; pending/close incerto conserva packet/backings/handles/créditos e UNKNOWN. A criação do parent não deve receber reset de owner/consumo ou tratamento de nova instalação por ausência de arquivo.

## PDP4 — Reuso pelo FCA posterior

Após resultado factual conhecido do ato PDP, uma nova preparação FCA independente pode readquirir Papinho como parent existente protegido, com identidade esperada apenas como DATA a confrontar, novas referências originais e continuidade até seus efeitos. Sem adoção de resultado incerto. Não reutilizar a confirmação ou a autorização PDP para FCA. A exceção PDP2 aplica-se somente ao ancestral ProgramData nessa cadeia; Papinho, source e todos os demais ancestrais continuam integralmente protegidos pela política anterior.

FCA4 permanece UMA criação exclusiva Accelerator + UMA chave, sem parent extra implícito. O source mínimo43bytes permanece original protegido distinto, com proveniência/readback, mesmo volume e slots existentes. Não alterar formatos de plano ou preencher identidade futura fictícia para evitar confirmação. Não criar uma autoridade transferível a partir do parent recém-criado.

## PDP5 — Recursos e assurance limitada

Selecionar expressamente para o ato preparatório PDP o mesmo envelope numérico limitado, sem herança implícita: entrada<=128, rejeição pré-aquisição>=188, cap192; privategrowth1994944/workinggrowth3145728; físico7239172096/commit3221225472; freshness100ms, confirmação240000ms, global60000ms sem renovar/rebase. Orçamento Selected3145728 e todos os demais limites existentes. Não modifica FCA, READ, TO ou outros perfis; Legacy não recebe fallback. Contar todos os recursos e qualificar censo/backings/worker, transitivos, overflow/consulta indisponível/Stop/expiry antes de pedido operacional.

Custos opacos CLR/CRT/Windows/kernel/armazenamento e latência continuam UNKNOWN. Esta escolha é tolerância explícita de risco limitada à criação de um parent, não suficiência ou teto nativo demonstrados. Nenhuma chave/KSP é acessada no ato PDP. Nenhum bônus de quota, renovação ou fechamento interno Windows para PASS. Excesso ou qualquer outro gate UNKNOWN => DENY/retenção.

## PDP6 — Integração e qualificação antes de efeitos

Após aprovação e registro normativo designado, integrar somente a extensão necessária no owner/mecanismo existente. Primeiro baseline/preimagens/censo e confronto de compatibilidade; então modelos das funções reais e quatro builds Selected/Legacy Debug/Release afetados, sem repetir campanhas inalteradas. Negativos: seleção ausente/cruzada; KnownFolder/namespace divergente; qualquer reparse; ausência de proteção/owner/ACL; ACEs fora de PDP2; ancestor arbitrário com mesmo SD; parent já existente/colisão; troca antes e durante criação; source/continuidade quebrados no FCA; recursos/overflow/freshness/deadline/Stop; falha antes/depois de consumo; pending/close UNKNOWN; replay; isolamento READ/TO/UPDATE/Legacy; zero criação de chave/certificado/CURRENT. Provar que nenhum branch de diagnóstico/flag cria autoridade.

Modelos e tipos de acesso não provam sozinhos segurança nativa universal ou quiescência do host. Se qualificação demonstrar incompatibilidade de contrato ou necessidade de ampliar a exceção/efeitos/quotas, STOP e reportar antes de implementar esse aumento. Não substituir teste negativo por expectativa positiva para fazer o modelo passar.

## PDP7 — Aprovação proposta e operações futuras

Aceitar B autoriza SOMENTE registro em documento arquitetural normativo designado novo, preservando todos os accepted anteriores e este candidato, e implementação/qualificação OFFLINE dependente. Não concede execução real de parent/source/FCA/UPDATE/coleta, assinatura/certificado/CSR/exportação privada, CA/CURRENT, NORMAL, endpoint/SCQ/StartStop/SCM-ACL/rede/TPM-NV-Q4/limpeza/refund/kill ou produção. Quando completo, preparar pedido operacional próprio com hashes/efeitos/riscos/gates para autorização específica. Eventual UPDATE só se houver delta técnico qualificado, pelo U1–U7/U2 existente; não reinstalar por observação apenas.

P4/K5/I0–I6, recibos/close, UNKNOWN e retenção histórica permanecem; esta proposta não resolve BADKEYSET nem autoriza dispensar autenticação/isolamento. Preservar HEAD/WIP/históricos/pins/EXEs/originais/autorizações consumidas e PST0.6.4. Readiness produtiva DENY.