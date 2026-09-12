# Instruções para agentes

Este arquivo operacionaliza a governança aplicável sem substituir ou
duplicar integralmente os ADRs normativos.

## Fontes de contexto

### OrganizationEngineering

Repositório canônico:

``` text
tobiastromm/organization-engineering
```

Ler os ADRs `accepted` aplicáveis, incluindo a taxonomia documental
definida por `OrganizationEngineering/ADR-0003`.

### PapinhoEngineering

Repositório canônico da Product Family `papinho-ecosystem`:

``` text
tobiastromm/papinho-engineering
```

Aplicar apenas ADRs vigentes e aplicáveis ao PapinhoAccelerator. Não
tratar ADR superseded como autoridade corrente.

### Fontes locais do PapinhoAccelerator

``` text
PROJECT-CONTEXT.md   contexto/composição de governança do projeto, se presente

docs/adr/            ADRs locais
docs/capabilities/   documentação viva de capabilities
docs/future/         Future Architecture Directions
docs/                arquitetura, phases, planos, roadmaps e documentação técnica

Code / Tests / Build / scripts
                     evidência factual do que está implementado
```

## Papel de cada categoria

``` text
ADR
→ decisão durável + rationale/histórico

Project Context / Profiles
→ aplicabilidade e composição de governança

Capability / Live Documentation
→ estado corrente, configuração, limitações, evidências e pendências

Future Architecture Direction
→ direção aprovada/deferida ainda não madura para ADR accepted

Workflow
→ procedimento repetível governado

Template
→ estrutura reutilizável de artefato

Code / Tests / Build
→ evidência factual da implementação
```

Future Architecture Direction nunca substitui ADR `accepted` e nunca
prova que algo está implementado.

## Processo obrigatório

Antes de planejar ou modificar código de forma substancial:

1.  determinar a branch/ref/working tree ativa;
2.  ler integralmente os ADRs `accepted` aplicáveis do
    OrganizationEngineering;
3.  ler integralmente os ADRs `accepted` aplicáveis do
    PapinhoEngineering;
4.  ler `PROJECT-CONTEXT.md`, quando presente;
5.  ler integralmente todos os ADRs locais `accepted`;
6.  ler os Capability Documents / live docs relevantes;
7.  procurar e ler os arquivos relevantes de `docs/future/`, quando
    existirem;
8.  ler documentos relacionados de phase, plano, roadmap, arquitetura e
    técnica;
9.  auditar Code, Tests, Build e scripts antes de assumir estado
    factual;
10. comparar as fontes e reportar divergências ou não conformidades;
11. apresentar estado factual, restrições arquiteturais e plano antes de
    implementar.

Na primeira retomada de um projeto pausado, ampliar a leitura para todos
os ADRs `accepted` aplicáveis e revisar também Future Architecture
Directions relevantes antes de restringir o foco.

## Regra especial para `docs/future/`

``` text
docs/future/ existe
+
documento é relevante ao trabalho atual
        ↓
MUST READ
```

Os arquivos de `docs/future/`:

-   preservam direção arquitetural aprovada/deferida;
-   podem identificar futuros ADR candidates;
-   podem listar decisões deliberadamente ainda não congeladas;
-   não autorizam afirmar implementação;
-   não congelam wire/API/configuração apenas por existirem;
-   não prevalecem sobre ADR `accepted`.

Capability/live docs correspondentes devem, quando aplicável, possuir
cross-reference para o Future Architecture Direction, e o future doc
deve referenciar os ADRs que já limitam sua direção.

## Governança

-   ADR `accepted` não pode ser modificado autonomamente por um agente.
-   Revisão documental de ADR `accepted` exige autorização explícita.
-   Nova decisão apenas inferida ou proposta pelo agente deve gerar ADR
    `proposed`/ADR candidate conforme a governança vigente.
-   Decisão explicitamente aprovada pelo owner pode ser registrada como
    `accepted` pelo processo autorizado.
-   Capability Documents podem e devem ser atualizados quando estado,
    limitações, pendências, evidências ou decisões relacionadas mudarem.
-   Future Architecture Directions devem ser atualizadas quando parte de
    sua direção virar ADR ou for implementada, para que não contradigam
    a autoridade vigente.
-   Code e Tests são a evidência factual do que está implementado.
-   Código ou teste que contrarie ADR `accepted` constitui não
    conformidade; não transforma a implementação existente em norma.
-   Divergências devem ser investigadas e reportadas antes de alteração
    arquitetural.
-   Phases e planos organizam o trabalho; não constituem identidade de
    ADR.
-   Não substituir a branch/ref ativa pela default branch ao auditar
    estado local.

## Conflitos

Quando fontes aplicáveis entrarem em conflito, reportar:

``` text
GOVERNANCE CONFLICT
```

antes de implementar uma mudança dependente do conflito.

## Precedência conceitual

``` text
accepted ADRs aplicáveis
        ↓
restringem
        ↓
Future Directions + Live Documentation
        ↓
são confrontadas com
        ↓
Code / Tests / Build factual state
```

## Descoberta de documentos relacionados

Ao trabalhar numa capability/subsistema, procurar explicitamente por
relações em:

``` text
docs/adr/
docs/capabilities/
docs/future/
docs/
```

Um arquivo relevante não deve ficar invisível apenas porque não foi
lembrado manualmente na conversa atual.

## Atualização documental após implementação

Quando uma implementação muda o estado factual:

``` text
Code / Tests PASS
        ↓
atualizar Live Documentation correspondente
        ↓
revisar Future Direction relacionada
        ↓
se decisão durável foi congelada:
    registrar/atualizar cross-reference para ADR apropriada
```

Não apagar automaticamente documentos históricos ou futuros sem aplicar
o lifecycle documental definido pela governança.

## Build / toolchain / projeto

Preservar as instruções específicas já documentadas pelo projeto para
build, testes, toolchains, segurança e deployment. Não trocar toolchain,
target ou backend silenciosamente para contornar uma falha.

Antes de executar build/testes, ler a documentação de build
correspondente ao target realmente trabalhado.
