# G3-P1 — diagnóstico final pós-cadeia qualificado offline

2026-11-02. main/HEAD f341985924d29870d8ff627ed37c8a0874ad6469.
IMPLEMENTED_OFFLINE / QUALIFIED / HISTORICAL_CAUSE_UNKNOWN.

## Resultado e lacuna factual

A sessão consumida alcançou o marcador EXISTING_TARGET_CANDIDATE após
scqi_chain_capture. Esse método já exige scqp_static e scqp_window na entrada,
e scqi_chain_current revalida ambos, inclusive prazo no fim. Só depois ocorre
o stdout e o retorno final static/unbound/window. A associação estática já
passou na cadeia; a primeira verificação explícita de runtime não vinculado é
no retorno final. A janela também pode vencer entre verificações, inclusive
por bloqueio do stdout. NÃO há evidência para atribuir a falha a qualquer uma
dessas hipóteses, a um atributo/ACL/token específico ou a excesso de handles.

Nenhum defeito funcional comprovado. O erro histórico13 permanece UNKNOWN
quanto ao subpredicado/campo exato. Não relaxados SID, atributos, owner/DACL,
identidade/proveniência, namespace, continuidade, G3, consumo, Stop ou retenção.
As duas novas cópias da sessão continuam preservadas; autorização usada sem retry.

Uma consulta posterior ao filesystem/token de outro processo NÃO reconstruiria
o plano efêmero original nem o instante da janela após stdout. A decisão e os
arquivos não preservam esses campos da memória da invocação encerrada.
Não se recomenda outra observação independente como prova retroativa.
O menor passo técnico é capturar a primeira rejeição nesse mesmo retorno do
owner numa eventual sessão NOVA, sujeita a autorização e identidade novas.

## Integração focal

personal_scq_profile_win32.inc permaneceu inalterado: scqp_static e
scqp_unbound continuam sendo os predicados originais. scqc_validate_final
preserva ordem/curto-circuito static -> unbound -> window. A janela usa a MESMA
leitura GetTickCount64 na mesma posição após stdout e após os outros testes;
expressão until>from && now>=from && now<until idêntica, sem renovação.

Em negação o erro continua13. Diagnóstico privado de primeira causa stage9,
reference3 (não arquivo), mais SCQ_COLD_FINAL_PLAN:
- attempted0: retorno ainda não alcançado; nunca PASS presumido.
- passed_mask:1 static,2 unbound,4 janela; bit ausente não significa falha se
  o predicado não foi alcançado.
- rejected1 STATIC;2 UNBOUND;3 WINDOW;0 sem rejeição capturada.
- unbound_reason1: escalar de runtime futuro indevidamente preenchido;
 2:image_digest presente;3:geometria/tamanho SID;4:SID inválido;
 5:padding SID não zero;6:autoridade/count/prefixo de SID de serviço;
 7:diagnóstico divergente/UNKNOWN, jamais fallback permissivo.
- unbound_field é índice, NÃO valor:1pid;2/3creation;4/5token_id;
 6/7authentication;8/9modified;10groups;11enumerated;12restricted;
 13elevated;14elevation_type;15matches;16attributes;17token_type;
 18query_error;19wait_status;20exit_code;21session;22logon;
 23administrator_attributes;24service_type;25service_state;26service_sid_type;
 27/28/29image. Em reason5, indica posição do padding, sem emitir byte/SID.
- window_known e from/until/now são snapshot da leitura original de relógio,
  não consulta adicional posterior ou prazo renovado.

A classificação detalhada de unbound ocorre SOMENTE depois da rejeição pelo
predicado original e não participa da admissão. Não emite valores de campos,
SID, tokens, referências, conteúdo ou segredos. Sem nova abertura/handle/query
de processo, sem owner/canal/serviço/protocolo paralelo. Campos privados +48
bytes; nenhum formato SCQ1/SQB1/SQB2/persistido foi alterado. Censo44 de handles
inalterado. Plan/clock metadata não constitui autoridade/readiness.

## Qualificação

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings nos logs finais,
mesma receita MSVC x64/W4/WX/PST0.6.4. A primeira tentativa encontrou somente
redefinição da macro de relógio do fixture; corrigida com undef no teste, sem
alterar flags/toolchain. Logs attempt1 preservados, finais separados.
56 pares teste/configuração PASS,14 CTests por configuração, logs verbose
 g3-p1-final-plan-*-tests.log. Testes reais de componente/modelo com APIs
protegidas simuladas, sem endpoint/SCM/token/RNG operacional.

41 novos cenários: plano válido; rejeição estrutural; digest/SID/forma/padding/
autoridade de runtime não vinculado; janela futura/expirada/malformada;
29 escalares futuros não zero, com índices distintos. Conferidos erro13,
stage9/short-circuit, bits de passagem, consulta de relógio zero ou UMA conforme
o caminho e ausência de novas queries de recursos/abertura. Modelos existentes
continuam cobrindo replay/Stop/UNKNOWN, G3-P1/READ/NEW_INSTALL, identidade,
query/bilateral/receipt/close. Nenhuma campanha operacional foi repetida.

Censo conhecido por sizeof: Selected2002152<3145728; Legacy1833584<2097152.
Não é máximo stack/heap/kernel/CRT ou capacidade nativa comprovada. Opacos e
quiescência OS UNKNOWN. Valores G3-P1 e demais quotas permanecem inalterados.

## Artefato e primeiro gate operacional

Selected/Release784896 bytes, SHA256
0b00a21a148b4b015d2fc40d8f412441a8af78a77fcfa7bed93a67a60048fde3.
Delta+1536 do canônico reconciliado783360/ID417614100:1245184:106492/
2d056518dd4da617e747e0e79b8e78acf5aab54b8b694118bfec95b75deb4d13.
Anterior/final congelados em workspace; DLLs byte-idênticas. Imagem instalada
NÃO atualizada; estado SCM/proteção atual não consultado.

Pedido [ONE-G3-P1-FINAL-PLAN-IMAGE-UPDATE-REQUEST.CANDIDATE.md](ONE-G3-P1-FINAL-PLAN-IMAGE-UPDATE-REQUEST.CANDIDATE.md),
SHA256 **43a5bad791d20ee5d48c7bd0efe90e25ca956a139c1f491bc8f7bc986720b4c7**, pronto para decisão específica do proprietário.
Reutiliza o updater protegido U1–U7/U2; seis helpers do procedimento idênticos
por AST, nenhum código mutante modificado/executado. K2027314 pertence ao
MESMO updater protegido, não ao servidor recém-compilado.

Primeiro gate: selecionar/aprovar byte-exato esse UPDATE diagnóstico. Após
execução manual única/reconciliação: nova sessão E/Q/S independente e novos
originais, sem reutilizar a sessão anterior. Não emitir comando de Start agora.
O diagnóstico novo não comprova causa histórica e pode não ser atingido se
outro gate negar antes; não é garantia de completar Start. Nenhuma operação
real, coleta, UPDATE, E/Q/S/Start/Stop, SCM/ACL/CA/CURRENT/rede/TPM/NV/Q4/
limpeza/refund/recuperação executada. Preservar todos os objetos e EXEs,
autorizações consumidas, HEAD/WIP, pins, PST0.6.4, UNKNOWN e DENY produtivo.
