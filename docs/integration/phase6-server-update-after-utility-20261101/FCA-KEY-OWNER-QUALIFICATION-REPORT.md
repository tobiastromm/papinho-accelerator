# FCA0–FCA7 — componente de chave qualificado; cadeia completa pendente

2026-11-03. OFFLINE_ONLY. Não é conclusão da integração integral FCA nem pedido de execução.

## Autoridade registrada

Aprovação original conferida: `365103d7f06cbe55dd0a1a260e4a274ad5db6dab40ae9dee6864edaf83803f5a`. Alternativa B incorporada antes do código em `docs/adr/DECISION-personal-first-instance-preparation-assurance-fca0-fca7-20261103.md`. A proposta e decisões anteriores permanecem intactas. HEAD main/f341985924d29870d8ff627ed37c8a0874ad6469 e WIP preservados.

## Integração factual

O key owner existente agora possui escopo exclusivo de preparação FCA, com o coletor e storage existentes. Begin captura baseline uma vez; arm só encurta a janela para o menor entre a expiração original e 60 segundos. Arm é gate de recursos, nunca confirmação administrativa ou autoridade F0. Legacy rejeita esse escopo. Não há fallback para a tolerância de crescimento da fixture.

Entrada até 128 handles, pré-aquisição abaixo de 188, pós-operação até 192. Commit 3221225472, memória física 7239172096, crescimento privado 1994944 e working set 3145728 bytes. Freshness 100 ms, janela inicial 240000 ms. Stop, deadline, overflow, consulta indisponível ou violação tornam a falha sticky. O orçamento não prova teto de custos nativos opacos.

Criação exige orçamento armado; abertura/adopção de chave existente é recusada no escopo FCA. O owner consulta owner/DACL e exige owner administrativo e DACL protegida, preservando o setter e máscaras da CA ordinária. Não importa a aceitação de readback S0–S5 da fixture. Borrow/transfer produtivos são recusados. Cleanup factual uma vez continua possível após Stop/falha; fechamento inconclusivo retém referências. Todos os efeitos CNG e consultas de recursos dos testes foram substituídos em modelo.

## Qualificação executada

Selected/Legacy, Debug/Release: quatro builds, zero warnings. Cinco testes afetados por configuração, 20 pares PASS: memory_census, personal_ca.lifetime, scq_machine_fixture, scq_machine_read_chain e scq_machine_read_chain_observed_sd. Logs e SHA256 dos arquivos estão em `FCA-KEY-OWNER-QUALIFICATION.json`. Falhas intermediárias de link e suas correções permanecem nos logs históricos; o resultado final usa build6 para Selected/Debug.

Casos FCA incluem entrada 127/128/129, pré-guarda 187/188/189, máximo 191/192/193, pisos e crescimentos imediatamente abaixo/igual/acima, baseline imutável, expiração sem renovação, rearm/replay, owner copiado/thread divergente, indisponibilidade/overflow/Stop, criação antes de arm, recusa de open, falhas e Stop após cada um de 12 pontos CNG, owner inadequado, DACL desprotegida e close UNKNOWN sem repetir FreeObject. Isso qualifica o componente; não demonstra a cadeia F0 completa.

Censo Selected/Release obtido do teste já executado: root 380408; conhecido gerenciado 2007456; limite 3145728; key owner 760 bytes. Não equivale ao pico final do caller FCA ainda ausente nem à suficiência host/provider/kernel.

## Imagens

Build Selected/Release intermediário: 883712 bytes, SHA256 `93e90733c8f834d7fde5d74fee4d1fc5980e56a8b0c9816a7e62454a4e309cb0`. Não congelado para UPDATE.

Última imagem instalada reconciliada: 882688 bytes, identidade `417614100:1966080:412413`, SHA256 `e3a433c4a6edb565c783fee672bfc82a5794ba44988b123e7a38eb4ec5c9d6b7`. Nenhum novo readback nativo ou UPDATE foi executado nesta qualificação. Delta de tamanho do build: +1024 bytes; não implica instalação.

## Primeira lacuna concreta

`personal_present` ainda submete handoff DATA packet12. `personal_target_begin`, worker e `personal_ca_native_current` continuam dependentes de `personal_native_ceremony_ready`, que permanece zero. A aprovação FCA não transforma esse guard genérico em verdadeiro.

A ligação específica de preparação ainda precisa integrar e testar: token original não impersonado; parent/source e ancestrais retidos sem reparse; proteção efetiva e continuidade; confirmação exata com configuração sem listeners/admissão; consumo antes do worker; proteção final do contêiner na criação; revalidação e orçamento do worker sem rebase; passagem ao mesmo key owner e fechamento/retenção sem assinatura ou publicação. O primitive `bootstrap_create_child` atual passa SecurityDescriptor NULL; isso não comprova por si só a proteção final FCA4. Ainda não foi ligado um ramo FCA a esse primitive.

Não foi demonstrada incompatibilidade normativa que exija outra aprovação. A próxima integração técnica offline permanece autorizada. Não se deve gerar pedido UPDATE a partir apenas deste componente.

UPDATE=NOT_READY; PREPARATION_CHAIN=INCOMPLETE; CURRENT/PRODUCTION=DENY; P4/K5=NOT_QUALIFIED. Histórico, retenção e quiescência OS UNKNOWN preservados. Nenhuma coleta, chave/contêiner real, endpoint, Start/Stop, ACL/SCM ou operação produtiva foi executada.

Próximo trabalho: `NEXT-FCA-ORIGINAL-CALLER-AND-PROTECTED-TARGET.md`. Caminho mínimo: concluir cadeia F0 FCA em modelo → qualificar regressões/censo final → pedido UPDATE byte-exato → autorização operacional própria → readback instalado → pedido próprio de preparação delimitada. Criação preparatória não autoriza assinatura/CURRENT/serviço; esses marcos continuam independentes.
