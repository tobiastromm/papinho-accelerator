# Phase 6 — consumidor A/P e confirmação PUBLISHED (2026-10-29)

## Resultado

O consumidor de confirmação foi corrigido em código, sem alterar bytes persistidos: **A e P são campos distintos na view privada do issuer**. O caminho NORMAL completo continua **NOT_DEMONSTRATED / DENY**: falta a fonte original CA/key/custódia/history/writer e a representação persistida específica do ack PUBLISHED.

Não se reinterpretou o pedido como nova norma: conforme issuer/C2 accepted, A é confirmação de publicação administrativa após commit; P é contexto operacional de policy. “Commit administrativo A” e “publicação P” não são os significados normativos desses identificadores.

## Fontes integradas e fonte faltante

O root nativo agora fornece `administrative_epoch` a partir do contexto do slot **efetivamente selecionado**, enquanto `epoch` continua sendo o P do mesmo owner. CAS e poll comparam os dois vínculos originais separadamente. Nenhum deles vem do caller, de um hash ou de um valor futuro.

A CA disponível em código continua limitada à borrow da cerimônia. `personal_ca_bind` recusa published; `personal_ca_current` exige o pacote original. O backend CNG consome uma key já adquirida e verifica SPKI/geração, mas não é uma fonte de custódia original/exclusividade por si. Não existe uma instância original root/CA demonstrada para completar este binding, e esta tarefa não executou sua materialização. Nenhuma das flags NORMAL foi preenchida com fatos de fixture. Ver a [fronteira de fontes anterior](../phase6-normal-status-fanout-20261029/SOURCE-BINDING-FRONTIER.md), que permanece aplicável.

Primeira aquisição necessária: CA/key/cert/generation legitimamente detidos e vinculados ao owner live, com transferência de lifetime/custódia que não apague o pacote da cerimônia. Deve ser uma fonte original verificável ou uma cerimônia isolada especificamente autorizada para os objetos protegidos faltantes. Os registros N0 não foram adotados como C8. Não se solicitou segredo nem se reabriu M3.

## Correções funcionais

- `PAPACC_ISSUER_VIEW` ganhou um campo **não serializado** `administrative_epoch` (A). `epoch` é P. O codec continua deixando ambos ausentes na leitura de DATA de disco; bytes PACIS601/ROOT06/ROOT07 não mudaram.
- `view_ok` exige A e P não zero, além das provas e identidades existentes. Não é uma nova fonte de autoridade.
- Depois de CAS, `verify_store` exige P preservado para a transação issuer-only e A novo/confirmado, além de revisão, high-water, request, serial e digest exatos. A confirmação PUBLISHED é comparada com **A**, não com P. Atraso permanece PENDING; resposta inconsistente permanece UNCERTAIN, sem repetir CAS/assinatura.
- O consumidor CNG exige A disponível e inclui A na igualdade exata de view antes do efeito. Nenhum callback passa a gerar key, assinar ou conceder autorização.
- O port nativo inclui A na correlação completa de CAS/poll. A divergência é rejeitada sem iniciar outra transação. O re-read de metadados expõe o A realmente selecionado e mantém P.
- O re-read após fanout M7 exige o mesmo A/R/P observado antes da solicitação; uma troca de A não passa apenas porque R/P continuam numericamente iguais. Os receipts/invalidation/Stop do pacote anterior são preservados; quiescência e close não foram presumidos.

Igualdade numérica ocasional de A/P não estabelece identidade: o vínculo é o campo e sua proveniência na root. Os modelos usam valores separados para que a comparação incorreta não possa passar despercebida. Esses números de teste não foram utilizados como identidades nativas.

## PUBLISHED persistido — decisão focal preparada

O formato v1 exige published_epoch antes do append, mas a root só emite A após commit. O guard publish=1 permanece UNSUPPORTED no adapter nativo; não foi removido para satisfazer um teste.

A [proposta focal](PUBLISHED-INTENT-AND-ACK.CANDIDATE.md), **PROPOSED**, recomenda intenção de publicação + ack original pós-commit, na mesma root/worker e sob revisão versionada explícita. Define autoridade, correlações, estados negativos, preservação de históricos e o requisito de não criar um ciclo de “ack do ack”. Não aceita bytes futuros, não reinterpreta ROOT07 e não cria nova família de ADRs. A decisão de representação é necessária precisamente por issuer/C2 P1.2; aceitar M3/M7 de novo não a resolve. SHA256 literal consta de QUALIFICATION.json/ARTIFACT-HASHES.json.

## Builds e testes

- Selected/Legacy Debug/Release: **quatro builds PASS, zero warnings finais**.
- **40 execuções CTests PASS**: dez suítes afetadas por configuração. Incluem issuer/core/CNG, enrollment e cohort M7/autorização/secure-owner isolados. Não se executou mTLS/rede real.
- **279 verificações do core por configuração**, explicitamente OFFLINE_ONLY. Testes novos: A/P distintos, P alterado, A ausente/antigo, ack zero, ack trocado por P ou por A anterior, ack atrasado, poll sem duplicação e A substituído após fanout. Falha mantém blocked/UNCERTAIN e não repete assinatura/commit.
- **32 execuções focadas PASS**: sete casos do port nativo e codec-only por configuração. Os casos novos confirmam A original no read, rejeitam A substituído antes de begin e durante poll, preservam a transação original e verificam A novo/P preservado após publicação do fixture. Codec confirma que o campo privado novo não é reconstruído dos bytes.
- As regressões de CA/chave divergente e falha de assinatura continuam nos modelos/consumidores já qualificados; não foram apresentadas como posse de uma CA real. Concorrência BUSY, Stop cedo/tardio, CAS conflitante, publicação não confirmada, replay, falha de re-read e retenção continuam no alcance do fixture de metadados.
- O cohort M7 foi reutilizado, sem repetir implementação ou inventar REVOKED nativo. Sua revalidação A/P ganhou teste de substituição. A execução positiva do fanout nativo continua dependente de uma credencial legitimamente publicada/revogada.

Os 28 arquivos nativos são novos/exclusivos sob `build/phase6-normal-ap-confirmation-20261029/final`. Os quatro codec-only não criam arquivo. Runner e manifesto registram paths, logs e resultados, rejeitando sobrescrita. Não se repetiu a campanha histórica inteira de 68 casos.

## Recursos, histórico e efeitos

A view x64 passou de 176 para 184 bytes; o core de 1056 para 1064 (a view esperada é embedded no pending). O root port continua 56 e não é somado novamente ao core. Os censos atuais por configuração estão nos logs: Selected known managed 1971896 / limite 3145728; Legacy 1803328 / limite 2097152. Limites não foram aumentados. Provider/kernel e pico não amostrado continuam UNKNOWN; isso não concede capacidade/headroom operacional.

HEAD permanece f341985924d29870d8ff627ed37c8a0874ad6469. Preimagens do WIP de entrada, deltas, hashes e logs preliminares/finais preservados. Nenhuma ADR accepted ou pacote PST foi alterado. Produção 0.6.4 preservada. Não houve root/CURRENT operacional, CA/key persistente, assinatura produtiva, bootstrap, serviço/Start/Stop real, rede operacional, SCM/ACL, TPM/NV/Q4/ownerAuth, cleanup/refund ou recuperação autoritativa.

## Impacto na aceitação e próximo gate

A comparação errada de A/P no consumidor foi removida. Isso não equivale a um ack persistido existente, nem permite preencher as provas NORMAL. Sem fonte original e representação válida, instalação pessoal/provisão/emissão/revogação positivas permanecem não demonstradas. ED11/Phase 6 continuam PARTIAL; E8/L2/D4/D5 e E17–E22 não são promovidos por estes testes.

Próximos gates reais, independentes: decisão focal sobre a representação versionada proposta e aquisição legítima da fonte original CA/custódia. A decisão normativa não autoriza uma operação de criação de objetos; a autorização técnica não equivale a posse de uma CA. Não é útil voltar ao TPM, refazer CAS ou repetir testes inalterados. [Próximo prompt](NEXT-FOCAL-ACK-AND-ORIGINAL-SOURCE.md).
