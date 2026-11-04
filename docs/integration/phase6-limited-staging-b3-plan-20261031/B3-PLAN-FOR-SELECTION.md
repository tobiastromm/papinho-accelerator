# Plano B3 — pacote e critérios propostos para seleção operacional

Estado: PROPOSTA DE SELEÇÃO, não autorização operacional nem política instalada. B0–B8 continua ACCEPTED. Esta folha reúne as escolhas concretas restantes; não pede nova decisão normativa. Nenhum valor abaixo é registrado como escolhido pelo proprietário.

## Pacote selecionável

Manifesto PACKAGE-SELECTABLE.json, fonte C:\Projetos\PapinhoAccelerator\build\phase6-functional-20261028\Selected\Release. Três arquivos,9.094.144bytes (P):

| Arquivo | Bytes | SHA256 |
|---|---:|---|
| papacc_server.exe |555008|01db19a5009e227d2910d1f7266cfd43e79e36ed3dfadd3e3100bda4b94ffe90|
| libssl-3-x64.dll |1321472|3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed|
| libcrypto-3-x64.dll |7217664|09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044|

Destino recomendado já accepted: known folder ProgramFiles + Papinho\Accelerator. C:\Program Files\Papinho\Accelerator é caminho planejado, não identidade nativa observada. Confirmar known folder/resolução congelada e ancestrais originais no futuro gate. Se houver colisão/reparse/proteção inadequada, STOP; sem migração/adoção/ACL em objeto existente.

Uma tentativa/invocação, até dois diretórios e três arquivos novos; máximo cinco criações. Sem retry, overwrite, cleanup ou promoção produtiva. Cópia/hash/readback e receipt não secreto; nenhuma instalação/SCM/Start/CA/CURRENT incluída.

## Critérios derivados e recomendações concretas

R é sum(ceil(bytes_i/unidade_volume)*unidade_volume), com aritmética checada. READY é a única amostra de preparação completa antes de confirmar/consumir. Não se usa109/132 para preencher READY nem se renovam thresholds durante a operação.

| Critério proposto | Fundamento e consequência |
|---|---|
| Handles máximos = H_READY +5, soma checada | Cinco é o máximo de referências diretamente adicionadas pelas cinco criações, sem fechar originais. Permite apenas essa folga de contagem observada, não um novo teto132. Crescimento interno adicional pode disparar STOP. Não é limite físico de recursos opacos nem atribuição de cada handle; drops/peaks entre samples não são invisibilidade resolvida. |
| Private Bytes máximos = Private_READY +P; working set máximo = WS_READY +P, separadamente | Recomendação de tolerância de risco: limitar crescimento observado a uma cópia adicional do payload. O desenho não solicita essa cópia, usa4096/1024 serializados; o allowance não é buffer, quota gerida ou bound de CRT/cache. Pode provocar falha de ensaio se custos legítimos excederem P; nunca somar Private+WS como charges disjuntos. |
| Piso físico disponível6.296.461.312bytes; piso commit disponível8.479.985.664bytes | Recomendação de referência conservada: não iniciar/continuar em disponibilidade inferior à amostra phase7 do próprio staging fornecida pelo proprietário na segunda coleta. Valores não vêm de LAB/TPM/SCQ/Browser. Essa coleta falhou por handles e NÃO demonstrou staging seguro; usa-se seu contexto apenas como tolerância proposta para não aceitar host em condição pior, jamais como reserva/capacidade comprovada. Ainda pode existir pressão/pico entre amostras. Se host estiver abaixo, STOP em vez de baixar pisos automaticamente. |
| Antes da primeira criação, espaço caller E global >=2R; piso de interrupção durante operação >=R | Recomendação de slack explícito: um payload para conteúdo planejado e outro payload como tolerância observacional a metadata/concorrência. Fator2 é escolha de risco proposta, NÃO limite demonstrado de FS; insuficiência real ainda pode ocorrer dentro da chamada. Identificar como tal ao aprovar. Arredondamento/unidade/quota são fatos frescos, não assumir cluster4096. |
| Freshness máxima proposta500ms entre início da última amostra obrigatória e gate de efeito | Escolha de risco operacional proposta, não valor medido/accepted ou importado de outro perfil. Janela curta impede usar amostra de antes da interação humana. Não garante estabilidade500ms. Pode ser curta demais na máquina real: nesse caso STOP, sem retry automático/renovação de ato. Precisa implementação e qualificação do sampler/gate correspondente antes de execução. |
| Duração global proposta120.000ms desde primeira amostra nativa até fechamento observado | Escolha de tolerância humana proposta: ensaio curto de9MB/5criações; não estimativa de throughput ou duração máxima de kernel. Inclui tempo no presenter; se expirar antes do consumo, nada criar. Após efeitos, suprimir novas submissões e reter se incerto; não interrompe/kill uma chamada síncrona bloqueada nem faz cleanup por timeout. |

As quatro últimas escolhas de risco NÃO são deduções de capacidade e NÃO estão implicitamente aprovadas. Estão apresentadas para decisão explícita com fundamento/efeito, em resposta à solicitação do proprietário. Não se disfarça uma recomendação como medição. B3 exige seleção explícita no plano; sem ela DENY. Se não forem aceitáveis, o proprietário pode rejeitar o conjunto sem execução. Nenhuma observação extra é proposta para “provar” essas preferências.

## Separação das garantias

Antes: quotas controladas e custo de critérios72bytes no packet, plano/pins/ato originais, proteção/identidade/volume, queries válidas, READY e thresholds congelados, confirmação atribuída e consumo único. Custos opacos do OS/CRT/descritores/kernel/FS continuam risco residual admitido B1, não prova integral exigida novamente.

Gates frescos: identidade/Stop/recursos dentro dos critérios selecionados, relógios monotônicos, espaço/unidade/quota; nenhuma amostra saudável por si autentica plano. Limites precisam ser avaliados antes das submissões dependentes, sem thread/helper/tracing novo. Critério de duração suprime novas chamadas, não cancela chamada em andamento.

Durante: status/disposition da criação exclusiva, progresso/bytes WriteFile, flush, hash/tamanho/SD/readback. Depois: close factual, primeira falha e retenção UNKNOWN. Não exigir sucesso futuro como evidência anterior; não promover sucesso de cópia a publicação/SCM/NEW_INSTALL.

## Bindings originais e implementação restante

Operador/host/boot/tokens/PID/criação/handles/SD são bindings tardios, adquiridos no contexto autorizado; não preencher comSID1108 ou boot histórico por conveniência. O manifesto e critérios selecionados devem integrar o mesmo ato/presenter/packet protegido, com originalidade/lifetime/replay, sem setter positivo ou arquivo de grant. A aceitação normativa B não é esse ato.

Nesta task foi integrado armazenamento privado DATA e avaliação de critérios, sem concessão de autoridade. Ainda falta o producer original do plano selecionado, cálculo/congelamento READY e ligação completa dos gates de observação/volume/tempo antes dos efeitos segundo a seleção legítima. Em particular o entry limitado não executa efeitos; guard conservador124 ainda não foi substituído. Não emitir comando para o antigo coletor.

Portanto este documento é uma recomendação completa de seleção B3, não pedido operacional executável. Não omite esses limites técnicos para afirmar pronto. Depois da seleção explícita, concluir e qualificar essa ligação técnica no mesmo owner, conferir hashes finais se houver rebuild e só então entregar pedido byte-exato/comando manual para autorização operacional própria. Nenhuma nova aceitação de B0–B8 é necessária para correções ordinárias cobertas.
