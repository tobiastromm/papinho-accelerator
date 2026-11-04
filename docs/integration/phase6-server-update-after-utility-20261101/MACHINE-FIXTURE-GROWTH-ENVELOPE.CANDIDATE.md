# Proposta focal — crescimento de memória da preparação fixture

PROPOSED / NOT_ACCEPTED / NO_OPERATIONAL_AUTHORITY. 2026-11-03.

## Fato novo e decisão necessária

Na sessão a772ab19934fcfc1b5829e04fe5fba1f36f5973aadbec50058238c8895f5594a, D1 concluiu uma nova decisão protegida, ID417614100:5177344:169433, hash e2e47dc1062f26b5952bddc68e720426210d66bb9148419b06562bb1f90118f3. F negou first1816/source_stage11, consumed0/created0, closeknown1, depois da confirmação e ANTES da criação. A sessão foi utilizada; não repetir D1/F, adotar nome gerado ou reutilizar decisão.

Seis consultas/seis amostras tiveram sucesso, query_error0/query_stage0. Entrada116<=128; última contagem143<188/192, físico8441376768>=7239172096 e commit10500542464>=3221225472. A conjunção com o código instalado e budget_first1816 delimita a causa desta tentativa: crescimento privado>1994944 OU working set>1994944, ou ambos. As magnitudes e qual dos dois falhou NÃO foram exportados e permanecem UNKNOWN. Não é falta de handles, nem evidência de insuficiência de físico/commit nessa última amostra, nem falha de consulta. A causa da tentativa1816 mais antiga permanece UNKNOWN, sem atribuição retroativa.

O produtor retém os originais e SDs exigidos pela norma; scratch de revalidação é liberado e os hashes BCrypt destroem hash/provider uma vez quando conhecidos. Não há vazamento demonstrado ou fechamento seguro adicional que esta evidência autorize. O custo gerenciado conhecido permanece K3071004/quota3145728; isso não mede crescimento real privado/working de CRT/loader/provider/heap/kernel. Não mudar baseline, fechar originais ou reordenar a guarda para obter PASS.

F1 accepted explicitamente preservou crescimento privado/working1994944 cada. Portanto uma tolerância maior exige aprovação focal própria. A aceitação anterior e o UPDATE não concedem essa alteração por analogia. A correção offline do texto antigo de limites é técnica, não altera esse requisito.

## Alternativas

A — manter1994944bytes para ambos: sem nova tolerância; operação continua sujeita a negar. Corrigir a apresentação, preservar UNKNOWN e não repetir sessões para coletar valores ou tentar sorte.

B — recomendação como ESCOLHA DE RISCO, não resultado de medição: selecionar R0–R4 abaixo. Aumentar ambos os tetos evita fingir que sabemos qual cresceu.16MiB é uma tolerância finita proposta para o ensaio limitado, não o pico observado, nem o menor valor suficiente provado, nem promessa de que a criação e o controle posteriores passarão. O proprietário pode manter A; nenhum aumento será implementado antes de aprovação explícita.

## R0 — Escopo exclusivo e números

Somente produtor existente Selected PERSONAL_LOCAL MACHINE_TEST_ONLY/K1, preparação exclusiva de uma fixture sob T0–T6/F0–F5. Substituir F1/T2 apenas nestes dois números:

| Predicado | Limite proposto |
| --- | ---: |
| Crescimento privado desde baseline original | 16777216bytes (16MiB) |
| Crescimento de working set desde baseline original | 16777216bytes (16MiB) |

Exatamente o limite é admissível quanto ao respectivo predicado; limite+1 nega. Crescimento é somente diferença positiva, com aritmética segura. Decréscimo não causa underflow. Baseline, ordem de aquisição e contagem completa permanecem; sem rebase/renovação/desconto/fallback. Falha de consulta/valor inválido/overflow/freshness/Stop/deadline mantém DENY.

Manter entrada128/guarda188/cap192, físico7239172096/commit3221225472, freshness100/global60000 sem renovação, todos os demais limites e quota gerenciada ED11.16MiB não é nova reserva gerenciada nem aumento do teto ED11: é tolerância ao crescimento medido do processo incluindo opacos. Uma métrica não substitui outra. Cada limite representa cerca de0,52% do piso de commit3GiB, cálculo aritmético que NÃO demonstra suficiência ou segurança universal.

## R1 — Isolamento e autoridade

Não ampliar READ/K7, TRANSPORT_ONLY/P1, NEW_INSTALL, UPDATE, issuer/CA ou outros perfis. Legacy real permanece DENY por K2902436>2097152. Seleção fixture legítima e explícita, censo SelectedK3071004/quota3145728/margem74724, T1/opacos e gates de originais/token/imagem/SD/ancestrais/semreparse/continuidade/deadline/Stop continuam obrigatórios.

Não promover DATA, pedido histórico, arquivo de decisão, hash, keyname ou imagem a autoridade. Manter declaração administrativa atribuída e consumo único antes da criação, exclusividade/nooverwrite, owner/DACL antes da finalização, controle positivo público da mesma chave retida, nenhum certificado/assinatura privada/exportação privada e nenhum ato READ/E/Q/S/P/Start concedido pela preparação.

## R2 — Risco residual e correção técnica conjunta

Aceitar maior crescimento privado/working no ensaio limitado: exaustão possível apesar dos pisos, custos e latência opacos não limitados universalmente, efeito parcial persistente e fechamento UNKNOWN. Nenhuma amostra prova teto nativo. Não apagar riscos/retentionUNKNOWN, dispensar guard ou conceder readiness produtiva.

A apresentação nativa deve refletir os números efetivamente compilados e distinguir pedido histórico DATA de decisão nova, com testes sobre a projeção antes da confirmação. A correção técnica já qualificada dos textos não autoriza mudar os números sozinha. Pode-se incluir a captura de private/working já presente no owner, sem consultas ou estruturas novas; isso não revela as magnitudes históricas perdidas. Não instalar intermediário só para instrumentação. Se B aprovada, consolidar números, apresentação e delta quantitativo em um único candidato final antes de pedir UPDATE.

## R3 — Integração e testes exclusivamente offline

Aceitação autoriza registro normativo separado, preservando todos os accepted/propostas/históricos, seguido de integração no mesmo owner/budget específico. Nenhuma outra arquitetura, serviço, root, canal, updater ou mecanismo de autoridade.

Qualificar com APIs mutantes limites privado/working16MiB-1/igual/+1 separadamente e juntos, baseline/decréscimo/overflow/consulta indisponível/Stop/deadline/freshness/UNKNOWN/retention, apresentação concordante com guard, rejeição de seleção divergente/fallback e preservação dos demais perfis. Executar somente builds/regressões afetados, sem repetir campanhas inalteradas. Congelar tamanho/hash/delta final e preparar um único pedido UPDATE U1–U7/U2 se necessário; aguardar aprovação operacional própria. Não executar UPDATE automaticamente.

## R4 — Nova operação somente com nova decisão

As duas decisões fixture anteriores, o UPDATE e as materializações D/D1 permanecem utilizadas, não reutilizáveis. Preservar pedido histórico DATA e todas as decisões/cópias existentes, incluindo ID417614100:5177344:169433. Nome de chave mostrado foi apenas candidato gerado; não há chave criada ou publicSHA qualificado a adotar.

Após eventual UPDATE qualificado/reconciliado, preparar UMA operação fixture independente com NOVA decisão operacional efetivamente emitida e, se necessária, uma única cópia auxiliar exclusiva em leaf novo explicitamente delimitado no pedido, pelo mesmo mecanismo F3. Nunca sobrescrever/recriar/excluir o leaf existente ou promover a decisão usada. Nenhuma aprovação, hash/ID futuro/keyname/publicSHA é fabricado agora. Cópia e fixture mantêm confirmações/consumos/Stop/close independentes e nenhum retry/rollback/cleanup/refund/kill.

Sem coleta, fixture/KSP real, materialização real, UPDATE, StartStop/endpoint/SCQ/P, SCM/ACL existentes, CA/CURRENT/rootstore, rede, TPM/NV/Q4 ou produção. Preservar HEAD/WIP/EXEs/DLLs/SCM/pins/históricos/consumos/PST0.6.4; application-close/retenção/reservas/quiescênciaOS UNKNOWN. Aprovação normativa NÃO é autorização operacional.

## Decisão solicitada

Aprovar exclusivamente B/R0–R4 para registro e integração OFFLINE, ou conservar A. Não se afirma que16MiB seja suficiente; é uma escolha explícita de risco que só o proprietário pode aprovar.
