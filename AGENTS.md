# Instruções para agentes

Este arquivo operacionaliza `PapinhoEngineering/ADR-0006` sem substituir ou
duplicar integralmente os ADRs normativos.

## Fontes de contexto

Repositório transversal:

```text
C:\Projetos\PapinhoEngineering
```

ADRs transversais:

```text
C:\Projetos\PapinhoEngineering\docs\adr\
```

Fontes locais do PapinhoAccelerator:

```text
docs/adr/           ADRs locais
docs/capabilities/  documentação viva de capabilities
docs/               arquitetura, phases, planos, roadmaps e documentação técnica
```

## Processo obrigatório

Antes de planejar ou modificar código:

1. ler integralmente os ADRs transversais `accepted` aplicáveis;
2. ler integralmente todos os ADRs locais `accepted`;
3. ler os Capability Documents relevantes;
4. ler os documentos relacionados de phase, plano, roadmap, arquitetura e técnica;
5. auditar Code, Tests, Build e scripts antes de assumir estado factual;
6. comparar as fontes e reportar divergências ou não conformidades;
7. apresentar o estado factual, as restrições arquiteturais e o plano antes de implementar.

Na primeira retomada de um projeto pausado, ler todos os ADRs `accepted`
transversais e locais antes de restringir o foco.

## Governança

- ADR `accepted` não pode ser modificado autonomamente por um agente.
- Revisão documental de ADR `accepted` exige autorização explícita.
- Nova decisão apenas inferida ou proposta pelo agente deve gerar ADR
  `proposed`.
- Decisão explicitamente aprovada pode gerar ADR `accepted`.
- Capability Documents podem e devem ser atualizados quando estado,
  limitações, pendências, evidências ou decisões relacionadas mudarem.
- Code e Tests são a evidência factual do que está implementado.
- Código ou teste que contrarie ADR `accepted` constitui não conformidade; não
  transforma a implementação existente em norma.
- Divergências devem ser investigadas e reportadas antes de alteração
  arquitetural.
- Phases e planos organizam o trabalho; não constituem identidade de ADR.

