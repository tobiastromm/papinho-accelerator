# Custos específicos e fronteira de admissão

## Demonstrado por código/layout e testes offline

| Item | Evidência | Alcance |
|---|---|---|
| Packet de staging | 7.320 bytes; antes7.000 | +320, já incluso no root; não cobrar buffer duas vezes |
| Buffer de cópia | 4.096 bytes no packet | Um arquivo por vez, writes parciais, sem carregar pacote inteiro |
| Buffer de hash | 1.024 bytes + contexto SHA256 automático compartilhado | Reutilizado serialmente; não limita stack transitiva |
| Selected gerido | 1.988.664 /3.145.728 | Layout, não PrivateUsage nem capacidade física |
| Legacy gerido | 1.820.096 /2.097.152 | Mesma distinção; quota não aumentada |
| SCM separado | 13.060 bytes | Somado, não omitido:2.001.724/1.833.156 antes de automáticos/transitivos |
| Referências de staging | Até16 slots fixos |8 ancestrais,3 fontes,2 diretórios novos,3 cópias |
| Contexto externo | Token/volume/temporários existentes | Além dos16 slots; guard próprio continua124, cap128 |
| Descritores controlados | Parent + texto SID + SID TI + proteção nova + descritor temporário | Quantidade de referências é conhecida; custos de alocador/API não são |
| Payload atual |3 arquivos/9.092.096 bytes | Novo exe552.960 bytes; bytes em disco não são espaço reservado |

Não há processo SCM futuro ou root de dados no censo do staging. Wrappers,
shell/console e recursos compartilhados não viram créditos por não serem medidos.
O cohort1024 conserva sua aplicabilidade: não é franquia adicional concedida
ao stager nem licença para copiar a topologia do ensaio TPM.

## Predicados negativos integrados

O sampler de registro foi extraído/reutilizado, mantendo suas checagens de
identidade no wrapper do registro; staging verifica seu próprio owner/thread.
Handles>124, query falha, zero físico/commit disponível, domínio QPC inconsistente,
PageSize inválido e overflow negam. Os snapshots não são fontes de autoridade.
Antes de cada FILE_OPEN/FILE_CREATE há também a verificação de espaço para a
única referência controlada adicional: count124 não recebe outra abertura.
Isso impede acúmulo previsível entre frames, sem afirmar pico de handles internos.

O volume é consultado pelo handle retido de ProgramFiles, com caller quota e
espaço global distintos. Produto sectors*bytes, units*unit e soma/rounding são
checados contra overflow. Planejamento convencional de payload:
`sum(ceil(file_bytes/unit_bytes)*unit_bytes)`. Valores abaixo dessa projeção
negam o plano; valores acima NÃO admitem. Essa projeção não é mínimo universal
ou ceiling de FS: compressão, residência, sparse, metadados, diretórios e reserva
do filesystem não são qualificados por arredondamento dos bytes de conteúdo.

Normal/observação usam o mesmo censo; somente normal pode apresentar/confirmar
seu ato, e a entrada pública ainda nega efeitos. O modo observação não solicita
RNG, confirmação, take ou efeitos. Antes do take privado do teste há nova
amostra necessária de memória/handles, incapaz de conceder admissão operacional.

Máscara inicial127. Observação saudável e volume conhecido removem somente
32 e64. Permanecem31: envelope nativo/transitivo, margem/política do host,
retenção, atribuição administrativa e metadados/efeitos do filesystem.
Não existe setter/callback positivo, restore de grant ou flags do caller que
transformem esses dados em aprovação de staging.

## UNKNOWN material preservado

- Picos nativos de stack/heap/CRT, loader e custos internos do OS/provedor/FS.
- Metadados, diretórios, espaço reservado, concorrência de outros writers e
  garantia de espaço após o snapshot. Não prometer reserva por free-space query.
- GetSecurityInfo aloca ANTES de GetSecurityDescriptorLength. O validador aceitar
  até65.536 bytes não prova limite da alocação nativa/transitória. Comprimentos
  observados são registrados; não viram teto do allocator.
- Headroom/margem de segurança justificados; resíduos conservam quota até close
  factual. Não liberar por timeout, Stop ou contador aparentemente baixo.
- Efeitos parciais e close/NT query incertos: retenção do cohort/buffers, sem
  retry. IO_STATUS_BLOCK e buffer da query de volume pertencem ao packet.

A menor evidência seguinte é a coleta manual read-only de preparação/close no
mesmo processo e volume existentes, sem staging prévio. Ela não mede writes reais
e não promete concluir o envelope ativo do FS. Evidência ativa adicional, caso
material, precisará escopo e autoridade próprios; não é implicitamente aprovada.
Não foi criada exceção normativa para UNKNOWN nem novo ceiling numérico.
