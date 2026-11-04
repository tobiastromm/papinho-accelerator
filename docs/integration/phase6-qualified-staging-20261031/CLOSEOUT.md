# Composição nativa do staging governado — qualificação offline

Task PACC-PHASE6-QUALIFIED-STAGING-COMPOSITION, 2026-10-31.
Branch main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469, WIP preservado.

## Resultado e alcance

Composição implementada no owner/controlador existente e qualificada offline.
Não foi executado staging. O caminho interno de efeitos é privado, sem comando
operacional e sem produtor positivo de admissão. A entrada de preparação sempre
retorna DENY; uma confirmação válida não a converte em autorização de cópia.
Pedido de execução continua **NOT_READY**. Primeira lacuna operacional:
**capacidade/admissão nativa do próprio staging ainda UNKNOWN**, incluindo custos
transitórios/opacos, margem factual e retenção. Builds e modelos não a suprem.

Não é mais necessário implementar outra vez a sequência de criação/cópia/close.
Não se exige serviço/CA/C8/ProgramData anterior para qualificar esse caminho.
Não há nova decisão normativa, root, ledger, serviço ou framework de instalação.

## Código e ligações reais

- `personal_qualification_staging_win32.h`: um cohort de 7.000 bytes no
  `PAPACC_PERSONAL_NATIVE_CONTEXT`, dentro do packet do owner canônico; não é
  owner independente ou formato persistido.
- `server_launcher_owner_win32.c` inclui o adapter
  `server_launcher_qualification_staging_win32.inc` junto do presenter existente.
  A entrada privada `papacc_server_launcher_qualification_staging` prepara e
  apresenta, mas nega aplicação. Não foi acrescentado despacho CLI de execução.
- `staging_prepare -> staging_current -> staging_apply -> staging_copy ->
  staging_close`: composição compilada; o caminho positivo de efeitos foi
  chamado somente pelo teste de componente, com APIs mutantes modeladas.
- `server_original_file_win32.inc`: extração sem alteração semântica do hash e
  comparação de identidade antes existentes no registro SCM. Ambos os caminhos
  e testes agora usam a mesma implementação; hash matemático não concede autoridade.
- `papacc_configuration_personal_native_close` rejeita fechamento externo
  enquanto staging está vivo/retido. Só o fechamento governado do cohort chega
  ao fechamento do contexto externo; falha conserva o owner RETAINED.

## Garantias exercitadas

Known folders são resolvidos/congelados pelo mecanismo existente. Volume/parent
originais e token elegível vêm do native context, não de paths/nomes/SIDs dados
pelo caller. Ancestrais de ProgramFiles são adquiridos relativamente ao original,
sem reparse, e conservam identidade e digest do descritor. A política de proteção
dos ancestrais OS distingue TrustedInstaller da política mais estrita dos novos
objetos da aplicação; não altera descritor existente. A aceitação real dessas
ACLs e do token do equipamento não foi demonstrada por estes modelos.

As três fontes são abertas nativamente sem reparse no caminho, compartilhando
somente leitura. O owner conserva handles, identidades, tamanhos, descritores e
SHA256, revalidando antes e durante os efeitos. O plano apresentado inclui fonte,
destino, operador, cada nome/tamanho/hash e consequências. O ato
OWNER_QUALIFICATION_STAGING é distinto de NEW_INSTALL, CA_INITIALIZE, SCQ e
SCM_REGISTER. Nonce32 do RNG existente, confirmação específica, mesma invocação,
thread e token; consumo único precede a primeira submissão criadora.

FILE_CREATE relativo exclusivo cria somente Papinho/Accelerator e três arquivos;
nenhum OPEN_IF, overwrite, adoção, rename ou edição de ACL antiga. Descritor
explícito na criação: `O:BAG:BAD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;GRGX;;;AU)`.
Novos objetos devem passar pelo predicado de aplicação e por DACL protegida.
O root/ancestors Windows não recebem esse descritor por inferência.

Cópia trata leitura/escrita parcial, tamanho exato, flush, SHA256 por releitura
do mesmo handle e identidade/proteção. Stop impede efeitos seguintes; não afirma
cancelamento retroativo de I/O. Não há atomicidade física entre os cinco objetos.
Colisão, substituição, falha após submissão ou close incerto não rearma o ato.
NTSTATUS inesperado/pending ou disposition não comprovada retém, sem declarar
conclusão ou iniciar outra operação. Falha de CloseHandle/LocalFree não recebe
retry; pais/backings e estado não resolvido permanecem no mesmo owner.

## Builds, testes e distinção de evidências

Selected/Legacy Debug/Release: quatro builds PASS com /W4 /WX, zero warnings.
15 CTests afetados por configuração: **60 execuções PASS**. Incluem staging,
guard de close, registro original e recursos, lifecycle, SCM virtual/query,
separação LAB/host fechado, layouts, censo e predicados de token/plano.

Teste focal: **48 cenários / 141 verificações por configuração**, incluindo
criação válida, colisão em cada um dos cinco creates, reparse, identidade/ACL
substituída, ACL inadequada, I/O parcial/zero/falha, conteúdo/hash divergente,
flush, Stop, replay, exclusividade/conflito de entrada, thread/self divergentes,
guard124, query falha, close incerto, LocalFree incerto, NT pending/disposition
incerta e retenção. Guard nativo adicional exercita seis estados vivos/retidos
contra a implementação real de fechamento, sem adquirir objetos.

Três mutantes comportamentais compilados e rejeitados: omitir hash da cópia,
omitir consumo e ignorar falha de close. Restaurado o fonte; depois foram
acrescentados os negativos de NTSTATUS e repetidas as regressões afetadas finais.
Logs dos mutantes são evidência das falhas específicas, não prova nativa de I/O.

SHA256 BearSSL e parsing/SID/descritor Windows em memória são reais. Token,
filesystem, NtCreateFile, handles, counters, console/confirmação e RNG do adapter
são modelados. Streams CRT temporários de diagnóstico dos testes são reais;
não são payloads ou diretórios operacionais. Não houve chamadas mutantes reais
do adapter, consulta SCM, endpoint, KSP, CA, CURRENT ou acesso à chave fixture.
O modelo não prova proteção Windows efetiva, durabilidade/crash física,
capacidade nativa ou execução administrativa autorizada.

## Recursos e pacote final

Packet +7.000 bytes (buffer de cópia4.096 incluso, sem double charge).
Censo gerido: Selected1.988.344/3.145.728 e Legacy1.819.776/2.097.152.
Somando13.060 separados SCM: Selected2.001.404, Legacy1.832.836, antes dos
automáticos/transitivos. Arrays automáticos conhecidos são cobrados na preparação;
isso não é limite total de stack/CRT/provider/kernel. Slots privados: até16
referências de staging, além das referências existentes do contexto e alocações
de descritor. Não são um pico observado. Guard próprio124 permanece; cap128 e
cohort1024 não são elevados, dispensados ou concedidos por esse número de slots.

Custos de filesystem/kernel, descritores dinâmicos, loader/CRT, margem do host,
admissão e retenção factual continuam UNKNOWN. Nenhuma observação nativa de
capacidade foi executada. Nenhum crédito, reserva fictícia ou TTL importado.

O rebuild mudou o SHA256 do servidor para
`2978958c3384ae36f41281d865b67b4a85b10dca8c6881ebffb6f879630c58f4`.
As DLLs e os tamanhos são preservados: três arquivos/9.073.152 bytes, identificados
em QUALIFICATION-BUNDLE.json. Não é seleção operacional, staging, attestation de
imagem carregada ou release. Inventários antigos continuam históricos; não usar
seus pins como aprovação deste rebuild. Produção PST0.6.4 inalterada.

## Próximo gate e caminho até registro

STAGING-SCOPE.CANDIDATE.md fixa o escopo, sem pedido submetível ou comando.
NEXT-STAGING-NATIVE-CAPACITY.md delimita a menor continuação de observação e
admissão, sem refazer o adapter nem criar infraestrutura. Identidades novas são
gates frescos da futura operação; não exigir file IDs/PID futuros antecipadamente.
Nenhuma aprovação é criada pela implementação ou pela confirmação modelada.

Após capacidade e bindings de admissão comprovados: pedido byte-exato próprio,
aprovação específica, staging governado; coleta do controlador de registro sob
autoridade própria; capacidade e registro parado sob pedido independente.
Start/SCQ/parent de dados/NEW_INSTALL/CA_INITIALIZE seguem separados.
E21 PENDING, ED11/Phase6 PARTIAL, autoridade produtiva DENY.
