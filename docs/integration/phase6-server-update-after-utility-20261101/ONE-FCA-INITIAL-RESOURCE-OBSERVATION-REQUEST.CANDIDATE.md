# Pedido operacional — uma observação prospectiva dos recursos iniciais FCA
CANDIDATE / READ_ONLY / NOT_EXECUTED. 2026-11-03.

## Limitação anterior à autorização
A amostra original da tentativae3c11dbcdc45ed4f4919946ae0cbe04306080f78acd48882b4550f6c991a8ca5 não foi emitida/persistida. A observação solicitada NÃO reproduz aquela tentativa: outro processo/loader/CRT/token/instante e custos próprios. Handles serão do observador, não do controladorFCA. Física/commit variam no tempo, não atestam passado ou futuro. Mesmo resultado favorável não concede admissão nem reautoriza FCA.
Não executar esta observação esperando recuperar causa histórica ou autorizar Start. A causa histórica exata ficará UNKNOWN.

## Escopo
UMA execução manual, UMA chamada do coletor personal_resource_collect existente, fase1/legacy_handle_guard0, após confirmação atribuída OBSERVE_FCA_INITIAL_RESOURCES_ONCE. Sem loops/retry/resample/espera/coleta de terceiros.
Queries originais: QPF/QPC, GetProcessHandleCount do próprio processo, GetProcessMemoryInfo próprio, GlobalMemoryStatusEx, GetPerformanceInfo, QPC. Nenhuma abertura de original FCA, SCM, processo de terceiros, token, chave ou diretório.
O observador recebe stdin/stdout por console original. GetStdHandle retorna referências emprestadas, não fechadas; queries usam pseudohandle do próprio processo. Não adquire handle persistente próprio por OpenProcess/CreateFile/CreateEvent/KSP. Loader/CRT/stdIO/consultas podem manter recursos internos opacos; não fechar handles Windows. Custo não subtraído da contagem, nem conhecido como teto.
Wrapper retém apenas referência read-only do próprio EXE com shareREAD durante a chamada, sem tocar instalação; pins de disco não são mapped-image attestation. Não altera ACL ou quotas.

## Artefatos selecionados
HostTCORP-TOBIAS, Windows PowerShell5.1 Desktop AMD64, console original. Observador de workspace não é owner administrativo nem autoridade, não usa conta de serviço.
fca-initial-resource-observer.exe SHA2565c4c293d8c25da16e8d19f0e296f6bfb6baa3c02c566fbf5385122cb2b6503db
fca-initial-resource-observer.c SHA25697058a574747e42a174168a713dc037ee0081ccfa546184ce328e590d5553862
Coletor de produto reutilizado sem alteração apps/server/server_personal_resource_sample_win32.inc SHA25604f2443675fb4c73c63bb3d8d15d9b6354683f8e02f9cb52b1ca185fe18b4ea3
Procedimento observe-fca-initial-resources.MANUAL.CANDIDATE.ps1 SHA2563db6a1993601158e7ce654f98fc2dc1062ef5d596b78d9e241ef3fd66ca4a5e2
Pins do script/pedido/EXE antes da chamada, nenhuma resposta automática. Após autorização específica registrar aprovação e entregar linha manual com hashes reais.
Compilado MSVC18/vcvars64 AMD64 /W4 /WX /MT /O2, zero warnings. Modelo executa coletor real com todas as APIs de coleta substituídas: seis cenários PASS (piso exato, pressão, query indisponível, não-retry após primeira falha, commit inválido, overflow). Observador real não executado. Não repetir campanhas do produto inalteradas.

## Saída e interpretação
Registrar query stage/retorno/attempts/samples. Se query falhar não imprimir números inicializados como prova.
Se completa: handles do próprio observador, física/commit disponíveis e QPC/frequência da amostra; below_mask bit1=handlesobservador>128, bit2=física<7239172096bytes, bit4=commit<3221225472bytes. Maskzero não é PASS FCA/capacidade futura. Não usar sample independente como original/readback/autoridade nem transformar UNKNOWN em PASS.
Sem log/disco ou captura de caminhos/SIDs/chaves/segredos no observador; somente stdout. Wrapper/lib loader possuem custos opacos externos ao sampler. Antes/depois da própria observação não são coletados: seria custo/amostra adicional desnecessária.

## Efeitos proibidos, consumo e reconciliação
Sem UPDATE/FCA/novo ensaio protegido/configuração/Papinho/Accelerator/chave/fixture/endpoint/SCQ/StartStop/SCM-ACL/CA-CURRENT/assinatura/certificado/CSR/exportação privada/rede/TPM-NV-Q4/limpeza/refund/kill/produção. Não alterar guard128/188/192, pisos ou demais contratos.
Uma confirmação, uma observação, consumo ao confirmar antes da única coleta. Cancelamento não autoriza retry automático. Falha/resultado parcial: STOP/UNKNOWN sem nova consulta. Nenhum arquivo do host operacional criado pelo observador. Wrapper fecha somente sua referência própria.
Após saída, registrar primeira falha/consumo/amostra, comparar floors apenas como estado prospectivo, manter causa histórica/handlesFCA/retencão/quiescência OS UNKNOWN, readinessDENY/PST0.6.4. Não executar outra FCA como continuação implícita; qualquer novo ato exige decisão específica independente.