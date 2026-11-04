# FCA stage4 — resultado, lacuna interna e diagnóstico qualificado
Tentativaae3b8ea2937a01bf2498b3c19c48b4058529c691c43d9f83b07e93df84073ec0 usada manualmente, sem replay autorizado.
FCA_PREFLIGHT_REJECT stage4/operation_result4=PAPACC_RESULT_INVALID_STATE conforme include/papacc/types.h. A chamada original papacc_configuration_personal_first_open falhou. Não é Win32=4, ACL=DENY ou prova de recursos insuficientes.
Sem apresentação/confirmacão ou consumo nativo de efeito observado. prepared0/flagszero/close1/wrapperCloseTrue/exit2; flagszero não são prova independente de ausência de objeto. Não executar novamente ou reaproveitar autorização. Estado/resultado registrados em FCA-STAGE-CAPTURE-PREPARATION-RESULT.json e AUTHORIZATION-STATE correspondente.

## Lacuna factual
A aquisição reúne retornos INVALID_STATE sem preencher first_error: estado/coorte, aquisição de parent, cadeia, continuidade, candidato SID do runtime, RNG/IDs e SD novo. LAST_CAPTURE só prova últimos snapshots; não é registro de aprovação de todos os passos.
Modelo existente usa LookupAccountName substituído retornando SidTypeUser; não demonstra classificação/retorno do host. Isso não autoriza aceitar outro tipo nem atribuir falha ao lookup. Nenhuma guarda de SID, autoridade ou DACL foi alterada.
Não foi demonstrado defeito funcional de identidade/proteção/recursos. Defeito demonstrado continua observabilidade incompleta do primeiro ramo rejeitado.

## Correção diagnóstica focal
server_configuration_first_prepare_win32.inc agora emite FCA_ORIGINAL_ACQUISITION_REJECT com número estável do ramo e o mesmo PAPACC_RESULT. Sem nova coleta, chamada de lookup ou amostra/handle. Sem campos/storage adicionais no owner ou quotas alteradas.
Prova de equivalência: remover o helper de impressão e substituir seus26 retornos pelos mesmos retornos originais restaura byte-exatamente SHA25610dd573a745f186ce816842a7ef42b5fba384fc3b82a3ee8a1d180c25002c287. Predicados/curto-circuito/ordem e guards mantidos.
Mapa de ramos:
1 estado do owner;2 perfil suportado;3 modo;4 sobreposição storage;5 coorte/budget original;
6 caminho/tamanho;7 caracteres;8 leaf/conversão source;9 conversão parent/layout;10 parent KnownFolder;11 associação parent;
12 recursos/native parent open;13 recursos/installer SID;14 nome nativo root;15 recursos/root open;
16 capacidade chain parent;17 parent component;18 capacidade chain source;19 source component;20 source file;
21 metadata source/revalidação chain;22 índices originais;23 recursos/lookup/validação candidatoSID;24 recursos/RNG;25 IDs gerados;26 formatação/recursos/SD final.
Ramos compostos ainda não provam subpredicado ou erro nativo. Não se promete causa exata de Windows nem resolução histórica. Nunca remover OBJ_DONT_REPARSE ou flexibilizar SID para produzir PASS.

## Qualificação e candidato
Quatro builds Selected/Legacy Debug/Release sem warnings,120 pares PASS, incluindo funções reais de aquisição e falhas sob mocks. Manifesto FCA-ACQUISITION-BRANCH-OFFLINE-QUALIFICATION.json. Nenhuma consulta operacional/coleta/UPDATE/Start/chave/diretório executada pelo agente.
Novo candidato912384bytes/SHA256f4d91013a34da09053dcb800dcd909f82ba544384f1430d9c22584d1d24ea82b, congelado papacc_server.fca-acquisition-branches-912384.exe.
Instalado permanece911360bytes/417614100:8716288:318363/SHA256747ba426d55af50947169a178c8366def46cb57ad4a8c29308ff44664aeaee96; nenhum UPDATE executado nesta tarefa.

## Primeiro gate restante
Pedido de UPDATE já preparado, separado da FCA:
ONE-FCA-ACQUISITION-BRANCH-IMAGE-UPDATE-REQUEST.CANDIDATE.md
SHA2565fb51f54b9832364c19cd79ea4f1189c24a54a43258aecb2720b9982bc575355.
Usa updater protegido/U1–U7/U2 sem recompilar, parse0/helpers idênticos/16 argumentos conferidos. Não repetir builds/testes inalterados. Pendência: autorização específica do pedido; depois registrar decisão real e comando manual. Sem executar automaticamente.
Após UPDATE reconciliado, só um novo ato independente autorizado poderá obter o ramo original, sem repetir FCA usada. Configuração/Papinho já concluídos não devem ser recriados.
Preservar UNKNOWN/retenção/quiescência histórica, DENY, P4/K5, HEAD/WIP/pins/EXEs/consumos/PST0.6.4.