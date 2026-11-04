# Sessão G3-P1 3GiB — falha e retenção, reconciliação parcial

Fonte preservada: OWNER-G3-P1-3GIB-SESSION-PARTIAL-OUTPUT.txt. Não é log terminal.
O proprietário informou que o processo continua parado. Nenhuma coleta, ação
SCM, interrupção, fechamento de console, retry ou operação protegida pelo agente.
Branch main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469; WIP preservado.

Dois originais criados/verificados, first NONE e close_known True. Identidades
417614100:1179648:127916 e 417614100:983040:127922. Três confirmações atribuídas
visíveis para tentativa 241a0c96391265c83a5482c3f178c4f0367e68908b4d80c10eea6b994caf8082.
Não confundir texto confirmado com consumo efetivo de cada ato.
LIMITED_RISK_ELIGIBLE foi alcançado; não é autenticação bilateral/readiness.
Resultado SERVICE ERROR5 / CLOSE0 / RETAINED1, sem retorno do wrapper.

## Disposição e semântica demonstradas pelo código

server_service_win32.c:330 mantém Sleep(INFINITE) quando retained e Start SCQ.
Não é espera com timeout ou autorização implícita para matar o processo.
CLOSE0 é retorno de ps_close, não prova de fechamento do owner original.
prestart_close pode marcar retenção e retornar erro enquanto ps_close fecha
sua coorte SCM/imagem e retorna zero. O erro original5 é preservado.

Na implementação qualificada examinada, start_effect é fixado em1 ANTES de
StartServiceW. ps_close com start_effect1 retorna ERROR_INVALID_STATE e marca
retenção; somente o terminal diagnóstico completo muda para2 e retorna sucesso.
Assim ERROR5/CLOSE0 exclui submissão StartServiceW por esse caminho, sob o binding
da imagem qualificada. Não equivale a consulta fresca do estado global SCM.
Não afirma ausência de endpoint: namespace_prepare pode ter consumido Q/E e
aberto namespace ANTES de LIMITED_RISK_ELIGIBLE. Preservar objetos e referências.

## Primeira lacuna factual

O erro5 após LIMITED_RISK_ELIGIBLE não tem marcador de subetapa. Candidatos:
ps_image_open (resolver/abertura/atributos); binding; OpenSCManager/OpenService;
QueryServiceStatusEx; predicados de ps_mutate antes de start_effect1, incluindo
readback/imagem/take/freshness. Sem nova evidência não escolher uma dessas causas.
O log não demonstra violação de memória/handles nem ACL inadequada.

Desvio focal encontrado: ps_image_open usa scm_registration_target quando
start_selected; helper chama paths_binary/SHGetKnownFolderPath, em vez de usar
o objeto selecionado P1 já retido. É chamada real no código, não causa histórica
comprovada. Não remover guardas, mudar quotas ou contornar a proveniência.
Uma eventual correção deve reutilizar seleção e referências originais P1,
revalidar identidade/SD/continuidade e manter demais perfis estritos.

## Próximo gate

Preservar processo retido e console. Sem fechar/kill/Ctrl+C/Stop/limpeza/retry.
Estado SCM atual, consumo por ato, endpoint, retenção histórica e quiescência
OS continuam não demonstrados. Preparar somente a menor consulta read-only de
estado/configuração SCM e identidade do controlador, com autorização operacional
específica antes de execução; essa consulta não revelará retroativamente o
predicado interno5. Não usar PID148 isolado como identidade ou abrir handles
para modificar o processo. Para causa interna, qualificar offline diagnóstico
por subetapa no mesmo owner, sem resampling, nova autoridade ou descarte.
Nenhum defeito funcional causador do erro5 está provado; não alterar produto
com base em hipótese. Nenhuma campanha de build/testes inalterada repetida.
