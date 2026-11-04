# Producer original do ato SCM_REGISTER_CLOSED

Task: PACC-PHASE6-GOVERNED-SCM-REGISTRATION-ORIGINAL-PRODUCER.
Data: 2026-10-31. Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469.
WIP anterior preservado; nenhuma decisão normativa/operacional registrada.

## Resultado

**Producer, presenter e consumo do ato específico de registro integrados no
owner existente e qualificados em modelo.** A fonte deixou de ser uma função
que apenas descreve sua ausência: adquire referências nativas pelo backend
existente, apresenta o plano e exige confirmação própria. A aquisição nativa
real não foi executada nesta tarefa; não há ato operacional real emitido.

Quando o ato válido existe no modelo, o gate reconhece a origem, mas ainda
nega por RESOURCES_UNQUALIFIED. Sem ato, continua ORIGINAL_UNAVAILABLE/DENY.
Não se exige readiness/PID/token de um serviço que ainda não foi criado.
Pedido de registro parado: NOT_READY por alvo/imagem operacionais e capacidade
aplicável ainda não demonstrados; não falta outra implementação da confirmação.

## Integração e callers

`main.c.api3.inc` mantém a CLI canônica. A rota existente
`papacc_server_service_cli` → seleção fechada →
`papacc_server_launcher_scm_registration_present` usa o mesmo owner global.
Não há novo serviço, root operacional, ledger, loader de grants ou IPC.

O producer está em `server_launcher_scm_registration_win32.inc`, incluído no
launcher pessoal existente, após os helpers de proteção SCM. O escaping do
presenter foi fatorado sem mudança semântica para
`personal_console_token_win32.inc`. A entrada exige console original, não stdin
redirecionado; sua existência não confirma nada automaticamente.

O subestado privado `PAPACC_SCM_REGISTRATION_ACT` pertence ao mesmo packet
`PAPACC_PERSONAL_NATIVE_CONTEXT`. Não é formato persistido ou autoridade separada.
Estados: captura, apresentação, confirmação, consumo, término, retenção UNKNOWN
e invalidação. Consumo permanece marcado depois de falha; não há reset produtivo.

### Fontes originais

- Operador: mesmo adapter F1–F6 de token próprio/elegibilidade, sessão, elevação,
  identidade e revalidação; operador administrativo não é o runtime virtual.
- Alvo: known folders novos congelados, diretório original dos binários adquirido
  pelo adapter nativo existente, identidade/descritor retidos e revalidados.
- Imagem: módulo retido pelo owner SCM e uma segunda referência adquirida
  relativamente ao diretório original pelo owner canônico. Comparar identidade,
  atributos, tamanho/data, SHA256 de conteúdo e proteção/descritor. Digest não
  substitui os objetos vivos. Nenhuma abertura de chave ou certificado.
- Destinatário local: `papacc_server_service_registration_view_current` verifica
  a própria view do owner SCM, endereço original, thread/invocação CLI ativa,
  console instalado e os backings/handle originais. Cópia equivalente é rejeitada.
  Não se serializam esses ponteiros nem se transferem privilégios.

A aquisição relativa reutiliza `scm_open_relative` através de
`papacc_configuration_scm_registration_image_open`; não herda autorização LAB.
O caminho exigido é o executável canônico sob a pasta de binários resolvida.
Executar um candidato da árvore de build não o transforma em imagem instalada.
Não há fallback para outro diretório, criação/adopção ou root de fixture.

### Plano apresentado e confirmação

Propósito exclusivo SCM_REGISTER_CLOSED, operador observado, serviço
PapinhoAccelerator, imagem/identidade/hash originais, comando fechado exato,
conta NT SERVICE\PapinhoAccelerator, DEMAND_START (3), SID type UNRESTRICTED (1),
alvo original e efeitos: UMA criação exclusiva do serviço, configuração do SID
type e readback. Start, endpoint, diretório, CA, CURRENT e rede ficam excluídos.

Tentativa de 32 bytes usa o CSPRNG existente; falha/indisponibilidade nega. A
declaração `OWNER_SCM_REGISTER` da tentativa é confirmada no console e atribuída
ao operador observado, não ao OS como aprovador do plano. Token elegível,
digest, seleção CLI ou nonce isolados não autorizam. Não se cria aprovação em
nome do proprietário por essa implementação; o ato só existe após a ação local.

Reutilizam-se as mecânicas F1–F6 sob M5/P3 e as especializações accepted de conta
e destino. NEW_INSTALL/CA_INITIALIZE e suas confirmações não são consumidos ou
reclassificados. OHI/SCQ/SSO-B/TO0–TO9 permanecem separados. Nenhuma alteração
normativa ou de BODY/ROOT/CURRENT/SCQ wire é necessária para esse subestado local.

### Efeito e retenção

`papacc_server_launcher_scm_registration_current` revalida o ato original;
`..._take` consome irreversivelmente antes de CreateService no caminho fechado.
Revalidar novamente antes da configuração e após readback. O mesmo ato não
autoriza Start ou outro registro. Ausência/substituição, outra thread, Stop,
outro destinatário ou perda de continuidade negam novos efeitos.

Create/configuração/readback parcial ou incerto preservam ambos os cohorts,
consumo e créditos; `ps_close` não os descarta por retorno síncrono da API.
Close incerto também retém. O fechamento factual conhecido libera primeiro as
referências SCM emprestadas e depois imagem/token/parent canônicos; término não
rearma o ato. Getters/blocks existentes impedem reutilizar packet retido.

## Qualificação e fronteiras da prova

Quatro builds Selected/Legacy Debug/Release: PASS, zero warnings finais.
40 execuções de CTests afetados: PASS (10 por configuração). O teste novo
executa o presenter, current/take, primitiva e close reais, com fatos/APIs
nativos modelados: 36 cenários e 163 verificações por configuração.

Casos: confirmação exata, ausente/cancelada/divergente, contexto indisponível,
token/target/ACL mudados, imagem/comando/conta/start/SID divergentes, imagem
indisponível, falha RNG, view copiada/destinatário trocado, replay, concorrência
da CLI, consumo por outra thread, Stop, falha antes/depois de Create, conflito
com serviço existente, readback substituído, close incerto e retenção parcial.

Uma thread Windows real tentou consumir o ato do owner e foi rejeitada; foi
aguardada/fechada. Isso não autentica um serviço ou token SCM real. Identidade
administrativa, diretório/arquivo/descritores, RNG, console e SCM são modelados
no teste novo. Os mocks fornecem fatos de aquisição, não callbacks positivos de
autoridade: a confirmação e todas as transições são da implementação original.

As regressões incluem lifecycle DATA, registro/consulta SCM modelados, LAB
negado/preservado, host produto/LAB, owner fechado vazio, plano pessoal e
predicados de token. O presenter anterior foi recompilado; seu ensaio que usa
RNG real não foi executado nesta campanha. Não repetidas campanhas criptográficas.

Quatro mutantes compilaram e foram rejeitados por comportamento: confirmação
ignorada, escopo não revalidado, consumo omitido e liberação após registro
parcial. Restauração byte-exata e rebuild/teste restaurado PASS. Ver MUTANTS.json.
Foram corrigidos durante o desenvolvimento includes Windows do teste e stubs
negativos do host para evitar extração/definição duplicada do launcher; logs
iniciais preservados. Nenhum guard foi relaxado para resolver essas falhas.

Na CLI completa modelada, um ato confirmado ainda termina antes de SCM pelo
gate de recursos. Contadores de aquisição SCM/Start/Delete são zero nessa rota;
as chamadas Create/Change/Query positivas pertencem somente à primitiva modelada.
Nenhuma API SCM, endpoint, RNG/KSP/CNG real, CA ou operação protegida foi executada.

## Custos e admissão

Censo puro, sem self/M1: Selected1981152/3145728; Legacy1812584/2097152 bytes.
O aumento conhecido de1368 bytes está no subestado do packet existente. O owner
SCM modelado passou de12924 para13060 bytes (view original e estado de borrow).
Arrays locais/hash são contados como lower bounds, sem um ceiling fictício de
stack/CRT/provider/kernel. Não se aumenta quota nem se libera crédito compartilhado.

Mantidos124/128/1024. Há referências reais que só uma execução autorizada pode
qualificar: alvo instalado, token/parent/imagem originais, alocações dos providers,
headroom, duração e fechamento. Os limites opacos não viram zero por se excluir
KSP. O campo NEEDS_RESOURCES continua explicitamente obrigatório: confirmação
não o preenche; nenhum flag/setter permite marcar capacidade como provada.

## Primeira fronteira operacional restante

1. O artefato derivado existe na árvore de build, com hashes em ARTIFACTS.json;
   não foi congelado, selecionado, instalado ou autenticado como imagem mapeada.
   Selected/Release de trabalho: SHA256
   `4f8582b3f95a582e6016c137de646b9ab20f05effdbdba28b3693ad94a51058a`.
2. A imagem original/protegida no destino de binários ainda não foi adquirida.
   A leitura não secreta de known folder DoNotVerify/Get-Item reportou
   `C:\Program Files\Papinho\Accelerator` e seu `papacc_server.exe` como
   NOT_FOUND_REPORTED_BY_PROVIDER (TARGET-METADATA.json). Não prova ausência de
   história nem qualifica identidade/proteção. Não foi feita consulta SCM/ACL
   ou criada pasta. Disponibilizar esse candidato no destino exige staging e
   proteção sob operação própria, com revalidação do alvo e seus ancestrais.
   Não confundir esse diretório com o parent dos dados/C8/N0.
3. Fundamentar a admissão quantitativa do ato de registro, incluindo aquisição
   dos originais, console/RNG/hash/SCM, retenção e close. Nenhuma medição do serviço
   inexistente é pré-condição desse trabalho. Não habilitar o gate com um bool.
4. Só então congelar/selecionar o artefato e preparar pedido byte-exato de registro
   parado, com gates frescos e aprovação específica. Não existe comando operacional
   pronto ou candidato executável com placeholders neste pacote.
5. Start/Stop diagnóstico, endpoint, prova de token/SCQ e CNG permanecem atos
   separados. Depois vêm NEW_INSTALL/CA_INITIALIZE/publicação sob suas autoridades;
   registrar serviço não cria CA nem exige CA anterior.

Aquisição original: IMPLEMENTED_QUALIFIED_IN_MODEL / NATIVE_NOT_EXECUTED.
Pedido: NOT_READY. E21 PENDING; ED11/Phase6 PARTIAL; autoridade produtiva DENY.

## Preservação

Nenhum serviço/conta/diretório operacional/CA criado; nenhum SCM/ACL, Start/Stop,
endpoint, rede, CURRENT, TPM/Q4, limpeza/refund ou recuperação executado. Sem nova
aprovação/ledger/root/owner ou migração. HEAD/WIP, todos os ADRs locais, baseline
selecionado histórico, receipt e chave fixture retida preservados. A chave não
foi acessada. PST0.6.4 preservada. Legacy não equivale a qualificação NT4.

Próxima continuação: NEXT-REGISTRATION-TARGET-AND-RESOURCE-ADMISSION.md.
