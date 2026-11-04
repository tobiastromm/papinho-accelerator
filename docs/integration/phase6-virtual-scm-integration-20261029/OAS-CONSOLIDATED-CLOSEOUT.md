# SCQ — aquisição de conteúdo original e correlação da imagem

Task PACC-PHASE6-SCQ-ORIGINAL-IMAGE-AND-ADMIN-SOURCE, 2026-10-30.
Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preservado.
B/OHI accepted permanecem byte-exatos; nenhuma nova ADR ou aprovação foi criada.

## Implementação e callers

`papacc_configuration_personal_scq_acquire_contents` foi integrado ao mesmo
owner/packet. Exige perfil controlador original da projeção, NEW_INSTALL layout
congelado e revalidado, sem worker/CA/publicação/RECOVERY. O plano congelado deve
vincular request/decision aos digests dos dois primeiros objetos. Revalida o
administrador pelo token nativo existente e confronta SID, sessão, PID/criação,
TokenId/AuthenticationId/ModifiedId com o plano antes de adquirir conteúdo.

A custódia `scq_inputs` já existente agora adquire três readers por ReOpenFile
sobre os handles originais, sem abrir caminhos de caller. Exige tipo disk,
identidade original, tamanho, link único e descritor protegido. Readers síncronos
com cursor próprio e FILE_SHARE_READ rejeitam escritores/deleção concorrentes;
são mantidos no mesmo owner até fechamento terminal. Um erro de sharing nega,
sem adotar outro objeto ou retentar. Duplicates originais e SDs continuam retidos.

Conteúdos de pedido, decisão e imagem são processados em chunks de 4096 bytes,
com deadline do plano e Stop. CNG SHA256 compara conteúdo e descritor com os pins
congelados. O resultado só publica content_verified após revalidação dos três
originais; reader/hash/close incerto mantém UNKNOWN e bloqueia retry/retire.
Buffers controlados são zerados; nenhum conteúdo bruto é impresso/exportado.
Nada afirma limpeza interna do provider ou orçamento opaco completo.

`papacc_configuration_personal_scq_observe_image_data` liga a observação runtime
existente à mesma custódia. Revalida SCM/processo/token antes/depois; compara o
nome nativo do processo original com o nome final NT do reader original e
projeta a identidade nativa e digest daquele objeto retido. A consistência de
caminho é adicional: não substitui objeto, proteção, conteúdo ou continuidade.
Em divergência/Stop a saída inteira é apagada. O modelo passa de known15 para31
somente na conjunção; isso é DATA diagnóstica, não grant ou atestação de memória.
Geração/alvo/política/pedido/decisão permanecem no plano congelado; não são
inventados pelo adapter de imagem.

## Limite da conclusão das fontes

Foi implementada a aquisição/verificação de conteúdo dos objetos originais e a
correlação diagnóstica da imagem. Não se declara que a fonte administrativa
original de AUTORIDADE esteja concluída. Hash correspondente não prova decisão
do proprietário, classificação não secreta de um documento arbitrário, semântica
de aprovação, confirmação administrativa atribuída ou consumo durável.

O source OHI continua explicitamente ausente no caller
`personal_scq_receive_original(...,NULL,...)`. O teste do root agora injeta até
content_verified e comprova que isso não libera recepção/dispatch/KSP. Não houve
constructor público de grant, setter approved, callback permissivo ou reutilização
de confirmação NEW_INSTALL. Tampouco se serializam ponteiros para outro processo.

| Fonte/garantia | Estado desta entrega |
|---|---|
| Objetos de pedido/decisão/imagem | Readers originais, conteúdo e SD ligados ao plano; qualificação com APIs de arquivo modeladas. |
| Correspondência imagem/SCM/processo | Adapter integrado, testes de correlação/negativos modelados; processo SCM original não observado. |
| Pedido/decisão SCQ operacionais originais | Não existentes/adquiridos para nova sessão real nesta tarefa. |
| Aprovação do proprietário e confirmação atribuída SCQ | Não criada nem demonstrada; source original continua NULL. |
| Validação semântica/classificação do pedido e decisão reais | Não demonstrada pela comparação de hash; não promover conteúdo arbitrário a projeção. |
| Consumo antes do primeiro efeito | Modelos anteriores preservados; persistência/continuidade do controlador não demonstradas nativamente. |
| Bilateral/endpoint/KSP real | Não executados; endpoint não criado/conectado/testado. |

A primeira lacuna continua sendo o producer de autoridade administrativa:
originais com decisão operacional exata e confirmação SCQ atribuída, preservados
pelo controlador e ligados ao consumo governado. Esta implementação remove a
lacuna de aquisição/verificação dos bytes, mas não a substitui por autoridade.
Não há pedido operacional completo nem comando manual nesta entrega. Não se
alega que basta mais uma aprovação para executar o código atual.

## Qualificação

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings. Nove CTests
focais/configuração, 36 execuções PASS. Suite de inputs: 41 cenários e 248
verificações/configuração. Identidade anterior: 163; perfil privado: 3338.
Campanhas restantes inalteradas não foram repetidas.

Testes novos: aquisição por handle; sharing failure; identidade substituída;
conteúdo/SD divergentes; short reads válidos e zero read inválido; provider
indisponível; Stop/deadline; mutação tardia; perda de peer; imagem divergente;
resposta de tamanho inválido; acesso por outra thread; repetição; close incerto.
CNG de hashing, SID/SD e thread são reais. Arquivos, ReadFile/ReOpenFile,
processo, SCM, tokens e nomes de imagem são modelados. Nenhum arquivo protegido
real, serviço, endpoint ou CA foi acessado por esses ensaios.

O último cenário injeta falha de BCryptDestroyHash e mantém explicitamente as
referências CNG no owner estático do fixture. Não há segunda tentativa de close;
saída do processo não é apresentada como prova de cleanup. Não é a chave fixture
persistente: ela não foi aberta/usada/excluída, e seu receipt permanece íntegro.

Custódia compilada: 416 bytes, antes248; crescimento fixo168 bytes. Known managed
Selected1.976.248/3.145.728, Legacy1.807.680/2.097.152. Readers adicionais: até3;
CNG, SDs e buffers/stack têm custos separados ainda não qualificados no ciclo real.
Chunk4096 e buffers de nomes2x1024WCHAR não são ceilings totais/headroom. Sem
créditos novos; provider/kernel UNKNOWN e limites124/128/1024 preservados.

[Qualificação e hashes](OAS-QUALIFICATION.json), [fontes](OAS-SOURCE-HASHES.json),
logs OAS-*-build/ctest/LastTest/layout. Nenhuma operação protegida foi executada.

## Caminho até NEW_INSTALL

1. Concluir producer controlador de decisão/confirmacão SCQ original: validação
   semântica da projeção, atribuição e consumo/continuidade; não criar autoridade
   positiva a partir da aquisição agora qualificada.
2. Somente com bindings completos preparar pedido byte-exato. Endpoint, eventual
   implantação SCM, Start/Stop, chave MACHINE e parent devem ter efeitos/gates
   separados e aprovação operacional própria. Processo futuro é observado depois
   do efeito autorizado, nunca inventado antes.
3. Executar manualmente apenas após essa aprovação; provar os dois principals
   originais, negação KSP real, fechamento e retenção. Fixture same-user não basta.
4. NEW_INSTALL/CA_INITIALIZE continuam cerimônias próprias. Sem TPM/Q4/root C8
   como nova dependência inferida.

[Próximo prompt técnico](NEXT-OAS-ATTRIBUTED-ADMIN-SOURCE.md).
E17/E21 não promovidos; E18/E19/E20 independentes; E22 pendente.
ED11/Phase6 PARTIAL, autoridades DENY, PST0.6.4 preservada.
