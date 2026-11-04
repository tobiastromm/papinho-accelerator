# Etapa207 — análise e qualificação offline concluídas

main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469, WIP preservado.
Evidência: OWNER-RETAINED-CAUSE-SESSION-PARTIAL-OUTPUT.txt e resultado parcial.
Take retornou5, start_submission_phase0, prestart_close996/service_close0/retained1.
StartServiceW NÃO alcançado. Controlador15744/criação31288058:1378940322 retido;
nenhuma consulta nova, interrupção, endpoint, Start/Stop, UPDATE ou ACL executada.
A sessão8ee498d2... está utilizada sem retry; não reutilizar decisões anteriores.

## Predicados reais do caminho

1. launcher consulta carrier original; qualquer negação era convertida em5.
2. take_image valida view/handle/path/hash/thread e fonte administrativa atual.
3. Revalida recursos por chamada nativa JÁ existente, retornos8/13/1816/1460/50
   também eram convertidos em INVALID_STATE e depois5, sem projeção da amostra.
4. Decodifica plano, consulta identidade do handle, compara identidade observada,
   identidade capturada, SHA256 e caminho canônico derivado do alvo original.
5. take revalida novamente fonte original e executa scqa_start_take, exigindo
   confirmação S, fonte SCQ consumida/originais/continuidade, sem replay/Stop.

O outer207 não distingue esses predicados. A sessão não emitiu a amostra do
recurso interno ou motivo interno da fonte. NÃO excluir crescimento privado/WS,
physical/commit/handles/freshness/deadline nem afirmar que erro5 é ACL. Ramo201
anterior ter passado não garante que a chamada interna posterior passe.
A hipótese Shell também não foi provada nem corrigida por inferência.

## Reprodução e implementação

Extraído o MESMO corpo para personal_scq_start_take_win32.inc, incluído no owner
nativo existente; nenhuma API/wire/autoridade externa nova. Teste compila esse
corpo e a máquina real scqa_start_take/original com fronteiras de arquivo/fonte/
recursos modeladas. Caso positivo passa e consome S uma vez; negativos abrangem
view ausente, fonte/revalidação ausente, todos os5 erros de recursos preservados,
plano alterado, arquivo/identidades/hash/path divergentes, overflow de caminho,
Stop, confirmação ausente e replay. Não se reproduziu uma negação inesperada no
caso válido. Nenhum defeito funcional específico da207 foi demonstrado.
Causa histórica interna permanece UNKNOWN delimitado à lista acima.

Integrado diagnóstico SCQ_START_TAKE_FIRST_CAUSE somente após a primeira
negação: step/result/cause e campos já retidos input/chain/source/start/resource.
Etapa1 identifica o getter carrier original. Etapas2/3 view/fonte;4 recursos;
5 decode;6 consulta do arquivo;7 identidade observada;8 identidade capturada;
9 hash;10 construção do path;11 comparação do path;12 revalidação da fonte no
take;13 consumo S original. cause0 pode ser predicado sem retorno adicional,
não PASS. Campos não alcançados/zerados não são prova. Retornos públicos e
mapeamento externo5 preservados. Nenhum getter, query ou relógio adicional.

Em recurso negado, reutiliza SCQ_START_RESOURCE_FAILURE para projetar SOMENTE
observations/READY/limites já retidos, sem resampling. A função ganhou guard de
include para reutilização no mesmo TU dos testes; não muda sua semântica.
Não serializar segredos/referências ou alterar fonte/SCQ/SQB/receipt/close.
Não relaxa a retenção: prestart_close996 continua preservando referências.

Escopo de mudança não inclui reorganizar inicialização, aumentar quota,
normalizar descriptor, trocar resolvedor ou abrir exceção de identidade. Não
há nova ADR ou decisão normativa. P0–P7/OHI/SSO-B/G0–G9/QUERY/G3-P1 e todos os
gates/Stop/consumo/UNKNOWN preservados. Censo persistente do packet não mudou;
stack/CRT/provider/kernel máximos continuam opacos.

## Qualificação final

Quatro builds Selected/Legacy Debug/Release PASS sem warnings;84 pares
(21 por configuração), incluindo25 cenários focais do corpo real. Demais20
regressões de fonte/recursos/cold/SD/identidade/READ/TO/host/censo/fechamento
passaram. A expectativa inicial do teste de alteração do version foi corrigida:
decode_fields apenas projeta os campos, a revalidação da fonte original é que
rejeita version alterado. Não era falha do produto. Nenhuma API protegida real.
Receita MSVC14.51x64/Ninja /W4 /WX e configurações/backends/SDK existentes
preservados. Logs e hashes TAKE207-OFFLINE-QUALIFICATION.json.

Candidato final Selected/Release804352bytes,
SHA2564665f220a8b8faba25bcda2e9671eb063208dd9db4d9bfa219e86ab662ecdbdd,
congelado papacc_server.take207-804352.exe. Delta+16384bytes contra instalado
787968/ID417614100:1638400:326399/pina2626bf7... . Instalado não alterado;
DLLs/configuração SCM/updater/ProgramData não tocados. Não é fix causal provado,
é preservação dos retornos internos que foram mascarados, para futura prova.

## Primeiro gate concreto e caminho mínimo

A instância15744 permanece retida; código já instalado não tem saída normal
acessível de Sleep(INFINITE). Não aplicar autorização antiga do PID148.
Preparado UM pedido excepcional de disposição desta instância, mesmo mecanismo
transitório anterior com alvo/identidade/token/imagem atualizados. Gates
revalidam objeto de processo pelo mesmo handle e exigem serviço parado antes
e depois; caso identidade/token tenha mudado, DENY, sem fallback/retry.
23 cenários com15 APIs/arquivo/hash/console simulados PASS; fonte real compilada
sem RunFixed, wrapper AST, deltas de escopo exatos e core equivalente.
Nenhuma execução real, novo serviço/owner/root/canal ou infraestrutura.

Pedido ONE-TAKE207-CONTROLLER-DISPOSITION.REQUEST.CANDIDATE.md,
SHA25696a08f75fec8c1a9aacec9de48d1817b94cfb6e2546eeb9fc4c743bb8e44ebba.
G5 requer decisão específica, não há autoridade por "continue". Não executado.
Somente após aprovação/execução manual/saída observada reconciliar a disposição
localizada sem declarar close SCQ, refund ou quiescência global. Depois preparar
UPDATE específico do candidato acima, se admissível, e E/Q/S nova independente.
Não repetir coleta/sessão/disposição consumidas nem misturar os atos.
Sem esse gate, não emitir UPDATE prematuro ou encerrar processo por conveniência.
Causa5 histórica, outros retidos e OS UNKNOWN; produtivo DENY. Históricos,
originais, EXEs, pins, reservas, HEAD/WIP e PST0.6.4 preservados.
