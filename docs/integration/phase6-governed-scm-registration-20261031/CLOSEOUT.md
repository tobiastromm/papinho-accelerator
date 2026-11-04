# Registro SCM pessoal — seleção fechada e fonte original

Data: 2026-10-31. Continuação de NEXT-QOC-GOVERNED-INSTALL-BINDING.md.
Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469; WIP preservado.

## Resultado e limite

Integradas e qualificadas offline a seleção explícita de registro diagnóstico
fechado, sua vinculação ao comando da imagem retida, a rejeição de substituição
e a separação do diagnóstico de autoridade de registro versus startup.
**Não está concluída a aquisição de um ato original que autorize o registro.**
O código continua negando antes de adquirir SCM; não existe pedido operacional
completo nem comando autorizado. Não confundir esse avanço com uma instalação
executável ou com fonte administrativa positiva demonstrada.

M1 permanece posterior à disponibilização legítima do processo. Readiness,
TokenUser/PID de um runtime futuro não são requisitos da fonte de registro.
A ausência dessas observações não pode ser substituída por uma constante positiva.

## Implementação no owner existente

- `server_service_data.c/.h`: seleção privada explícita `--scq-closed` somente
  em `service install`. É seleção de comando, nunca aprovação. Forma anterior
  preservada; não há aceitação do seletor em Start/Stop ou combinação com LAB.
- `server_service_virtual_win32.inc`: comando fechado fixo da mesma imagem,
  sem interpolar argv. Caminho limitado e comando revalidados antes de Create e
  do readback. Rejeita perfil desconhecido, comando/imagem divergentes e fallback
  fechado→produtivo. LAB não aceita a seleção pessoal. Conta virtual, demanda
  manual, SID type/readback e referências originais existentes são preservados.
- `server_service_startup_win32.h/.inc`: relatório específico da fonte de registro
  exige ato original, alvo/imagem, escopo exato, lifetime/consumo e recursos;
  não exige readiness futura. Retenção, Stop, thread divergente ou tentativa já
  submetida negam. Relatório não é grant, setter, loader ou registro persistido.
- `server_service_win32.c`: CLI e dispatcher de mutações distinguem esse gate
  do startup. A primeira causa é REGISTER_SCM/source unavailable, com seleção e
  campos faltantes, em vez de atribuir a ausência ao processo ainda não criado.
- CMake: dependências explícitas dos includes de seleção e fonte, inclusive nos
  modelos. Nenhum formato BODY/ROOT/CURRENT, SCQ wire ou contrato accepted mudou.

A função de fonte retorna DENY em todos os caminhos atuais. Isso é uma lacuna
explícita de aquisição original; não se alega que acrescentar um relatório tenha
implementado o producer. Nenhum callback positivo de autorização foi instalado.

## Por que não reutilizar uma confirmação existente

| Fonte existente | O que comprova | Limite que impede promoção para registro |
|---|---|---|
| Presenter F0–F9 | Declaração/confirmacão atribuída do plano NEW_INSTALL e handoff DATA | O plano atual não apresenta comando SCM, tipo de início e efeitos de registro; não confirma esse efeito |
| SCM LAB prepare | Imagem/configuração/descritores por referências originais | Fecha as referências no término e declara LAB_ONLY/CREATE_SERVICE=DENY |
| OHI/SCQA | Pedido/decisão SCQ originais e confirmação `OWNER_SCQ`, quando legitimamente registrados | Não é autorização genérica de instalação; o getter operacional segue NULL |
| SSO-B/S0–S7 | Fonte controller-local pré-runtime e readiness fechada | S1 exige decisão específica; S4/S7 separam efeitos operacionais |
| TO0–TO9 | Transporte sem KSP no propósito2 | TO4 exige decisão própria; TO8 separa implantação/registro/lifecycle/parent |
| Hash/imagem/conta resolvida | Observações necessárias | Não são declaração ou decisão atribuída sobre registro |

Referências de código: `server_launcher_personal_win32.inc`,
`server_launcher_scm_prepare_win32.inc`, `personal_scq_admin_win32.inc` e
`server_configuration_personal_native_win32.inc` (seletores operacionais NULL).
OHI, alternativa B, SSO-B e TO0–TO9 foram conferidos com seus originais e pins;
F0–F9, M5/P3, conta virtual e destino mantidos. Nenhuma nova aprovação normativa
foi solicitada/registrada. Não foi demonstrada uma impossibilidade normativa de
implementar a cerimônia; foi demonstrada a ausência da fonte específica no código
e nos originais operacionais disponíveis. Inventar esse original seria incorreto.

## Qualificação

Selected/Legacy × Debug/Release: quatro builds finais PASS, zero warnings.
24 execuções de CTests PASS (seis por configuração): lifecycle DATA, registro
virtual modelado, regressão do registro LAB, host produto, host LAB e owner
canônico fechado sem originais. Não foram executados testes SCM nativos.

Cobertura nova: seletor explícito/fora de escopo; criação e readback fechados
modelados; comando produtivo substituído no readback; perfil alterado antes do
efeito; imagem/comando divergentes; perfil inválido; caminho ausente ou sem
terminador; fonte ausente apesar de imagem/metadata RUNNING; thread divergente;
Stop; consumo já realizado; retenção; CLI negando antes do manager. Regressões
mantêm existência/conflito, falha antes/depois de Create, Stop e close incerto.
Exclusividade da CLI e consumo pré-Create existentes permanecem; não se afirma
novo stress concorrente ou prova de consumo durável por esses modelos.

Três mutantes Selected/Debug compilaram e foram rejeitados por comportamento:
inversão do gate da fonte, fallback fechado→produtivo e aceitação de comando
substituído. Arquivos restaurados byte-exatos; rebuild e teste restaurado PASS.
Detalhes em MUTANTS.json e logs correspondentes. Foi corrigida uma asserção de
teste que usava PS_REQUEST depois de uma parse inválida que o zera; não foi
alterada a política de parsing para satisfazer a asserção.

Modelos substituem SCM/Create/Change/Start/Delete e APIs de imagem/console. Na
CLI testada, contador de aquisição SCM é zero; Start/Delete zero. Primitiva
positiva modelada NÃO é prova de autoridade administrativa positiva. Funções
Windows locais de identidade de thread/estrutura SID e hash de bytes modelados
não demonstram token original, imagem operacional ou serviço instalado.
Nenhum endpoint, RNG/KSP/CNG real, CA, CURRENT, rede ou serviço foi executado.

Censo puro (sem modo self/M1): managed Selected1979784/3145728 e
Legacy1811216/2097152 preservados. Modelo do owner SCM:12816 + host104 + CLI4 =
12924 bytes, sem aumento observado do total estático (campo privado ocupa
alinhamento existente). Request acrescenta um unsigned privado; relatório de
registro tem cinco DWORDs. Não há heap novo, processo ou quota/crédito adicional.
Custos transitivos/stack de plataforma e envelope operacional continuam UNKNOWN;
esses sizeof não concedem headroom. Limites124/128/1024 inalterados.

## Primeiro gate restante e sequência

1. A fonte específica de registro ainda precisa adquirir e manter, no owner
   canônico, o pedido/decisão atribuídos que nomeiem serviço, imagem original,
   comando fechado, conta, demanda manual, SID type, alvo e efeitos exatos.
   A decisão SCQ não pode ser reinterpretada como esse ato. Os originais não
   existem como aprovação operacional disponível nesta tarefa; não foram criados.
2. O producer deve ligar esse ato à revalidação nativa e ao consumo anterior ao
   registro, mantendo a fronteira fechada após falha. A função diagnóstica agora
   separada não faz essa aquisição. A autorização técnica offline não preenche
   o original nem a evidência de recursos que ainda faltam.
3. Somente com fonte, alvo/imagem protegidos, artefato final selecionado e
   quantitativos suficientes, preparar pedido byte-exato de registro parado.
   Implantação/staging e parent, se necessários, têm efeitos/autorização próprios.
4. Registrar não inicia o processo. Start/Stop diagnóstico e endpoint continuam
   escopos próprios; PID/token/continuidade vêm de gates frescos, sem IDs fictícios.
5. M1 pode observar o processo então, sob seu pedido próprio; não é admissão SCQ.
   NEW_INSTALL/CA_INITIALIZE/CURRENT e operação produtiva permanecem separados.

Pedido operacional: NOT_READY. Não há comando com placeholders ou seleção
antecipada de artefato. Os quatro binários derivados são de trabalho, não
congelados/selecionados/instalados; hashes em ARTIFACTS.json. Não se afirma que
falta somente aprovação operacional: falta o producer específico e a prova de
capacidade aplicável. E21 PENDING; ED11/Phase6 PARTIAL; autoridade DENY.

## Preservação

Nenhum SCM query/M1/Start/Stop, endpoint, instalação, diretório operacional,
conta, ACL, CA/CURRENT, rede, TPM/Q4 ou limpeza/refund executado. Nenhuma nova
autoridade/root/owner/ledger/formato persistido. Preimagens e postimagens locais
registradas; todos os ADRs locais preservados. WIP anterior não descartado;
chave fixture retida não acessada; produção PST0.6.4 preservada. Não atribuir
qualificação NT4 à configuração denominada Legacy deste build Windows moderno.
