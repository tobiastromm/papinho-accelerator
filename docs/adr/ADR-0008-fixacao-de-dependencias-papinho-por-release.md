---
adr: ADR-0008
title: Fixação de dependências Papinho independentes por release
status: accepted
decision-date: 2026-09-06
last-revised: 2026-09-06
revision: 1
scope: project
decision-makers:
  - Tobias Tromm
supersedes: []
superseded-by: []
---

# ADR-0008 — Fixação de dependências Papinho independentes por release

## Contexto

O PapinhoAccelerator precisa consumir bibliotecas Papinho desenvolvidas e
publicadas independentemente, começando pelo PapinhoSecureTransport (PST), sem
depender de checkouts locais, caminhos de workspace ou descoberta variável da
release mais recente.

Uma integração reproduzível precisa identificar tanto a versão lógica quanto o
artefato factual compatível com o target do consumer. Confiar apenas em um nome
de projeto, numa tag móvel, num diretório local ou numa URL sem verificação não
preserva essa propriedade.

## Alternativas consideradas

### Checkout local ou submodule

Acopla o build à organização do workspace ou ao source da dependência e permite
que estado local não publicado altere o resultado.

### Descoberta automática da release mais recente

Evita atualização manual, mas torna o mesmo commit do consumer variável ao
longo do tempo.

### Cópia ou vendor de source

Elimina o download durante o build, mas duplica o source e desfaz a fronteira
de release entre projetos independentes.

### Manifesto versionado de release

Registra no consumer repositório, tag, target, asset e hash exatos. A aquisição
usa exclusivamente esses fatos e falha quando não consegue prová-los.

## Decisão

Dependências Papinho independentes podem ser fixadas no PapinhoAccelerator por
um manifesto de release versionado no próprio consumer.

Cada pin deve declarar explicitamente:

```text
repository
release_tag
target
asset
sha256
```

O target é um identificador factual conforme PapinhoEngineering ADR-0002. O
artefato adquirido deve corresponder exatamente ao repositório, tag, target e
nome declarados. SHA-256 externo é obrigatório antes de confiar ou extrair o
pacote. Metadados, contrato de link e hashes internos fornecidos pelo pacote
também devem ser validados quando existirem.

A aquisição normal não pode:

- consultar `latest`;
- substituir release, target ou asset;
- aceitar hash ausente ou divergente;
- procurar checkout local como fallback;
- copiar source da dependência para o Accelerator;
- espalhar caminhos ou versões hardcoded por múltiplos pontos do build.

Ausência, incompatibilidade ou falha de integridade termina explicitamente a
aquisição/configuração aplicável. Não há fallback silencioso.

O primeiro pin regido por esta decisão é
`dependencies/papinho-secure-transport.txt`.

## Consequências

### Positivas

- builds podem reproduzir a mesma dependência publicada;
- target e contrato de link permanecem auditáveis;
- o Accelerator não depende do checkout local do PST;
- atualização de dependência torna-se mudança versionada e revisável;
- mismatch falha fechado.

### Trade-offs

- atualização de release exige atualizar e validar o pin;
- a aquisição precisa de comando explícito e acesso à origem publicada;
- caches/staging precisam permanecer separados por target;
- SHA-256 comprova integridade contra o pin, não autenticidade do publisher.

## Impacto

O mecanismo inicial é local e estreito para PST. Este ADR não cria um package
manager genérico nem uma regra transversal do PapinhoEngineering.

Artefatos adquiridos ficam fora da árvore versionada de source, sob a árvore de
build correspondente. O build e o runtime do consumer usam apenas o SDK
publicado e os arquivos declarados por ele.

## Relações

- PapinhoEngineering ADR-0002 — identificação factual e durável de targets.
- PapinhoEngineering ADR-0007 — fronteiras portáveis entre core, plataforma e
  backends substituíveis.
- PapinhoAccelerator ADR-0002 — Core portável entre deployments e backends.

## Verificação de conformidade

- o pin contém todas as cinco chaves obrigatórias, sem duplicatas;
- download deriva somente do pin;
- o hash externo é comparado antes da extração;
- manifests e hashes internos são verificados;
- target/linkage/API divergentes são rejeitados;
- CMake/cache/binários não referenciam um checkout PST local;
- não existe fallback para outro release, target ou caminho.

## Histórico de revisões

| Revisão | Data | Descrição |
|---:|---|---|
| 1 | 2026-09-06 | Decisão aprovada para fixar dependências Papinho independentes por release manifest versionado. |
