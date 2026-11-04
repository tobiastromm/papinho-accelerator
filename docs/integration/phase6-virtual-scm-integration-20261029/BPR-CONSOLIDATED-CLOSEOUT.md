# SCQ — par original e readiness da primeira prova bilateral

TASK: PACC-PHASE6-NTA-ORIGINAL-PAIR-AND-ENDPOINT-READINESS
2026-10-30; main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preservado.

## Resultado factual novo

Duas consultas SCM locais somente leitura, `sc.exe queryex PapinhoAccelerator`
e `sc.exe qc PapinhoAccelerator`, reportaram erro1060: serviço não instalado no
contexto observado. Não foi erro5/acesso negado. Não há processo SCM original
adquirido. Isso é uma observação desta sessão, não afirma ausência futura nem
uma identidade autenticada de host/boot. Não foi solicitada elevação. Os exit
codes individuais dos processos sc.exe não foram capturados; registrar apenas
os erros SCM efetivamente impressos. Evidência: BPR-READONLY-SCM-OBSERVATION.json.

Known folders via Environment.GetFolderPath, sem criação: ProgramFiles C:\Program
Files; ProgramData C:\ProgramData. Resultam os candidatos binários C:\Program
Files\Papinho\Accelerator e dados C:\ProgramData\Papinho\Accelerator,
parent C:\ProgramData\Papinho, contêiner Accelerator. Não são alvo operacional
confirmado, parent adquirido, instalação ou imagem original. Perfil PERSONAL_LOCAL
lógico; nenhuma pasta Personal. BPR-KNOWN-FOLDER-METADATA.json.

## Correção do diagnóstico de readiness

O bloqueio não é conhecer antecipadamente um PID/token futuro. O código já aceita
runtime unbound na preparação/take e `papacc_scq_profile_bind_runtime_data` o
vincula depois do consumo, antes de offer. A qualificação SDR caso31, preservada
nos logs NTA, cobre essa ordenação em modelo. Não pedir nova coleta de PID para
corrigir um problema que não existe nessa fase.

Há três lacunas de composição nativa, além da disponibilidade do serviço:

1. `server_service_win32.c`: `ps_binding_ready()` retorna literalmente0. O
   ServiceMain não LAB encerra antes do runtime; instalar/iniciar o binário por
   fora não completa o owner/config/admission binding nem autoriza informar RUNNING.
2. `server_configuration_personal_native_win32.inc`: registro operacional NULL;
   fábricas `scqt_create/open_local` sem caller de ativação no owner. O constructor
   atual faz `v.controller=o; v.recipient=o`. Ele compõe a fonte local existente,
   não entrega sozinho um original opaco a outro principal. O runtime virtual não
   pode satisfazer a condição administrativa simplesmente reutilizando o packet
   do controlador. Precisa da entrada receptora e aquisição local próprias no mesmo
   owner, com política de papel já aceita em B/OHI, sem conceder acesso administrativo.
3. `scqt_close`: ack DATA não comprova fechamento remoto. Sem recibo de fechamento
   nativo autenticado, o adapter só libera referências remotas após término observado
   do processo original. Endpoint-only não autoriza Stop e não pode prometer liberação
   com runtime vivo. Não incluir Stop por conveniência nem interpretar echo como ack.

As linhas/hashes estão em BPR-SOURCE-TRACE.json. NTA demonstrou primitivas/fábricas
compiladas com efeitos modelados; não uma jornada bilateral nativa executável.
O relatório NTA permanece histórico. Este registro torna explícito que apenas
aprovar um endpoint não resolve essas lacunas técnicas. Não atribuir isso a uma
nova exigência de PID pré-sessão nem à necessidade de bootstrap Q4/C8/TPM.

## Bindings prévios e gates frescos

Matriz completa: BPR-BINDING-MATRIX.csv. Antes da sessão: escopo PERSONAL_LOCAL,
serviço/conta esperados, artefatos qualificados, finalidade, direitos mínimos,
alvo confirmado/congelado e pedido/decisão originais. Os dois últimos ainda não
estão ligados operacionalmente. Aprovações B/OHI não são autorização operacional.
Binário qualificado no workspace não é a imagem original instalada.

Durante a sessão, após autoridade específica e consumo governado:

- Controlador: seu próprio processo original e token primário; SID aprovado,
  elevação, grupos completos e atributos, restrições, sessão, PID/criação e
  TokenId/AuthenticationId/ModifiedId. Reter referências e revalidar mudanças.
- Se o serviço já existe e está elegível, capturar seu objeto SCM original e
  configuração/imagem; se não existe, parar antes de endpoint. Sua eventual
  disponibilização exige etapa de instalação/lifecycle explicitamente autorizada.
- Se houver Start separadamente autorizado: consumir antes de Start sem exigir
  PID futuro; depois capturar o processo a partir do serviço original, verificar
  criação, TokenUser virtual exato, grupos/restrições e re-read SCM. Não reportar
  RUNNING só para satisfazer `scqp_peer`; readiness continua factual e governada.
- Vincular o runtime observado ao plano unbound; depois obter offer e verificar
  os dois peers nas pontas antes de entregar referências. Falha/incompletude/
  substituição/perda mantém DENY/UNKNOWN. Sem fallback de token/conta/LAB.
- Originais locais de pedido, decisão e imagem: verificar objeto, conteúdo,
  descritor e proveniência independentemente. Query-only de processo/token não
  concede impersonation; nunca transferir PROCESS_DUP_HANDLE administrativo.
- Endpoint aleatório nasce somente no efeito autorizado; nome não é credencial.
  Validar peer IDs kernel com referências originais e identidade completa, não
  somente PID/HWND/nome/hash. Sem reabertura/reconexão após perda.

Não congelar valores de processo futuros em aprovação. A decisão aprova o alvo,
origem e predicados exatos; as instâncias de processo são obtidas e verificadas
no momento governado. Ausência atual de serviço não autoriza criar outro helper.

## Separação dos efeitos e pedido

Pedido operacional de endpoint: NOT_READY / NOT_EMITTED. Nenhum comando ou
placeholder. Não seria correto pedir aprovação de um executável sem entrada
bilateral nativa e sem originais de operação vinculados. Também faltam valores
justificados da janela/duração/headroom específica; caps124/128/1024 e intervalos
modelados não fornecem esses valores. Não copiar limites do LAB ou converter
custos UNKNOWN em capacidade.

Escopos independentes para o futuro pacote:

| Escopo | O que pode autorizar | O que não decorre dele |
|---|---|---|
| Endpoint bilateral | Uma criação/conexão SCQ e referências estritas; sem KSP | Instalação, Start/Stop, sessão CNG ou criação de parent |
| Sessão SCM/CNG | Probe exato MACHINE sob autoridade própria, depois do transporte qualificado | CA produtiva, CURRENT ou novas permissões |
| Disponibilização SCM/lifecycle | Somente instalação/Start/Stop expressamente enumerados, se indispensáveis | Conta local, helper, execução de rede ou bypass de readiness |
| Parent | Aquisição se já existe; criação somente por autorização específica | Adoção silenciosa, ACL ampla, bootstrap histórico |

Para a prova de transporte em si não executar KSP, criar chave, CA, root ou
CURRENT. Não criar parent só para um endpoint; se a cerimônia do serviço o exigir,
explicitar a dependência e o efeito separado. Não publicar um pedido com quatro
permissões agregadas. A ausência de serviço não transforma uma aprovação de
endpoint em autorização de implantação.

## Qualificação e primeira ação

Manifesto NTA conferido integralmente na entrada; fontes NTA sem alteração.
Reutilizados quatro builds zero warnings, 52 CTests, 53 cenários/1131 checks por
configuração, inclusive rejeições de par/token/service SID, replay, forwarding,
Stop e UNKNOWN. Não repetida campanha: nenhuma alteração funcional ocorreu.
Novas evidências são a observação SCM e a rastreabilidade do código, não outra
contagem de testes. BPR-QUALIFICATION.json e BPR-ARTIFACT-HASHES.json.

Primeira ação técnica: integrar offline, no host/owner existentes, os papéis
controlador/receptor SCQ e a prova de fechamento original, mantendo registro
operacional vazio. Não substituir `ps_binding_ready` por true. Isso precede
qualquer pedido de execução e não requer nova aprovação normativa inferida.
Se o contrato vigente não cobrir uma responsabilidade concreta, demonstrar a
cláusula exata antes de implementar; nenhuma família de ADR é proposta aqui.

Depois: concluir o pacote operacional com artefatos e política justificados;
autorizar separadamente a disponibilização legítima do runtime, se necessária;
obter suas identidades por gates frescos; executar a única prova bilateral somente
sob aprovação própria. Seguem sessão SCM/CNG/custódia MACHINE, cerimônia NEW_INSTALL
própria, publicação e continuidade. Não inserir TPM/Q4/C8 histórico nesse caminho.

E21 PENDING; ED11/Phase6 PARTIAL; autoridade DENY. Nenhum endpoint, Start/Stop,
instalação, conta, parent, CA, chave, CURRENT, ACL, TPM/Q4 ou cleanup executado.
Chave fixture retida não aberta. HEAD/WIP/PST0.6.4 preservados.

[Próximo prompt técnico offline](NEXT-BPR-NATIVE-ROLE-ENTRY-AND-CLOSE-PROOF.md).
