# Phase 6 — disponibilidade do executor independente do UPDATE

Continuar CLOSEOUT.md desta pasta e a decisão accepted U2 de2026-11-01.
Não reimplementar DATA, criar distribuição, repetir staging/registro ou executar
UPDATE. A origem de bytes agora é distinta do executor e qualificada em modelo.

Bloqueio exato: não há executor protegido, independente e capaz demonstrado.
O instalado é c0f27f…806ecf,571392bytes, sem literais do caminho UPDATE conforme
leitura estática, coerente com seu histórico. Mais forte: usar o mesmo objeto
como executor do seu próprio UPDATE viola a proibição de self-update da
especialização accepted. Não repetir testes/coletas para tentar superar isso.

Identificar somente uma alternativa REAL já existente e legitimamente protegida,
se houver, com capacidade, proveniência e relação de papéis admissíveis. Não
inventar file ID, assinatura, autorização, processo futuro ou controller de fixture.
O código atual só admite executor diretamente no mesmo parent protegido, por
referências originais estritas; não expandir roots/paths silenciosamente.

Se não houver alternativa existente, apresentar concretamente a menor decisão
necessária para disponibilizar o executor OU rever a restrição que impede uma
alternativa mais simples. Não promover o candidato do workspace, alterar ACLs,
copiar controller ou criar helper/serviço/root/distribuição. Não criar ADR por
rotina ou pedir aprovação operacional de procedimento ainda impossível.

Concluir trabalho offline autorizado que realmente feche essa dependência.
Mudança normativa eventual exige aprovação explícita exata antes do código;
nenhuma inferência de que U2 autorizou provisionamento. Não solicitar novamente
a aceitação de DATA já registrada. Preservar as guardas estritas e o UPDATE
individual, consumo único, Stop, dois renames sem overwrite, UNKNOWN e retenção.

Entregar fonte concreta/qualificada ou a única dependência exclusiva restante,
delta se houver, testes afetados somente quando necessários, e pedido operacional
somente com todos os bindings. Sem comando com placeholders ou retry.
Não executar UPDATE, provisionamento/staging, ACL/SCM, Start/Stop, endpoint,
CA/CURRENT, rede, TPM/Q4, rollback/exclusão/limpeza/refund. Preservar HEAD/WIP,
pins/históricos, EXE anterior, autorizações utilizadas, UNKNOWN e PST0.6.4.
