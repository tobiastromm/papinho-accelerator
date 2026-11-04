# Imagem staged TRANSPORT_ONLY — correção qualificada offline

2026-10-31. Branch main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469, WIP preservado. Continuação da autorização técnica explícita do proprietário. Candidato original conferido integralmente: SHA256 39becca194a2971acb98398eb2e8213e8322bbb195e6846773b26afece447af6. Ele permanece imutável e histórico. Alcance da autorização registrado em ../phase6-original-selection-start-admission-20261031/OWNER-GUARD-TECHNICAL-AUTHORIZATION.md. Nenhuma nova ADR ou decisão ACCEPTED foi criada; a tentativa de registrar uma aceitação normativa foi rejeitada automaticamente e substituída por registro factual da autorização técnica, sem modificar contratos accepted.

## Predicado corrigido

personal_scq_inputs_protected, a captura ordinária e os perfis READ continuam exigindo owner igual ao operador. Request e decisão passam pela mesma guarda estrita, inclusive quando a imagem usa a captura TO.

A nova entrada privada personal_scq_inputs_capture_transport aceita exclusivamente o plano tipado de propósito2, validado e ligado ao owner e aos caminhos conhecidos congelados. A exceção de owner BA só é avaliada no índice interno2 (papel3/imagem), depois de uma aquisição original e revalidação da cadeia. Não é selecionada por um bool de caller ou por key vazio. Nenhum outro owner da imagem foi acrescentado: SYSTEM, TrustedInstaller ou owner estrangeiro continuam rejeitados nesse predicado, salvo o próprio operador original já aceito antes.

personal_scq_image_chain_win32.inc retém, no mesmo input packet, os diretórios originais e uma abertura relativa da imagem. Usa FILE_OPEN, SHARE_READ, direitos de leitura/query/travessia, OBJ_DONT_REPARSE e FILE_OPEN_REPARSE_POINT. Não cria objetos nem altera ACL. O FileID/volume/tamanho/atributos/links da imagem relativa deve coincidir com o original adquirido. Nomes fixos e paths conhecidos são consistência adicional, não autoridade.

Todos os ancestrais passam por verificação conservadora de owner e DACL efetiva: descarta ACEs não suportadas, interpreta somente ACEs aplicáveis ao objeto e rejeita qualquer ALLOW de escrita/modificação por principal estrangeiro. Todos devem ter DACL protegida. A exceção exata AU/0x4 só existe na âncora de volume e só resulta em prova válida depois de o primeiro filho e os demais diretórios passarem na proteção estrita. DELETE/WRITE_DAC/WRITE_OWNER ou direitos adicionais não são dispensados. Os principals Windows usados na proteção ancestral são os já usados no stager/anchor-child; isso não estende os owners permitidos da imagem.

Cada revalidação compara identidades e os bytes dos descritores originais, paths congelados, escopo, janela e Stop. Mudança de objeto/SD/path ou falta de prova nega. A checagem é uma prova corrente conservadora, não uma promessa de proteção futura contra administrador hostil ou de quiescência global. O owner BA, o sucesso da cadeia e o estado QUERY nunca concedem Start, endpoint, CA ou acesso a chave.

A cadeia, seu plano copiado, IOSB, OBJECT_ATTRIBUTES, UNICODE_STRING e nomes ficam no packet existente. Pending inesperado conserva esses backings e referências como UNKNOWN; não há resubmissão ou close por timeout. Fechamento conhecido ocorre depois dos leitores/originais, na ordem imagem → diretório filho → ancestrais. Falha de ownership/close retém a coorte e não rearma a tentativa.

## Callers efetivamente alterados

1. papacc_configuration_personal_scq_capture_inputs: no controlador, plano TO do mesmo owner seleciona a captura tipada; READ e ausência desse contexto usam a captura ordinária estrita.
2. papacc_configuration_personal_scq_recipient_prepare: no preparo receptor com origem legítima, captura TO exige a mesma prova; resolve/congela os known folders como DATA quando ainda não presentes, sem criar parent ou assumir autorização.
3. papacc_configuration_personal_scq_ingress_continue: depois da captura/validação de candidatos, adquire os originais pela entrada TO e só então prossegue em conteúdo/SD/imagem, scqr_first_prepare e promoção. O carrier não preenche a fonte administrativa independente ausente.
4. personal_scq_inputs_check/contents/close: mantêm a prova viva através de confirmação, hashing, consumo e fechamento. Os expected refs usados no hashing também precisam coincidir com o escopo congelado da cadeia TO; não pode verificar outro conjunto e reutilizar a prova.

O transporte/SCM/register/staging não foram reimplementados. N0, T0–T14, ROOT/CURRENT, SQR1/SQC1 e o comando SCM persistido permanecem intocados.

## Qualificação

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings finais. 29 CTests afetados por configuração: 116 pares PASS. Logs por configuração e EVIDENCE.json nesta pasta. Execuções focais de diagnóstico não são somadas novamente ao total. As rodadas finais corrigiram a guarda de quota ENTRE as três duplicações e o tratamento de erro com buffer de SD não nulo. Esses casos foram acrescentados, não repetidos como campanha histórica inalterada. O isolamento READ foi reforçado com plano e key válidos; só o teste específico foi reexecutado nas quatro configurações e não foi somado novamente aos116 pares.

Novo ensaio SCQ_TO_IMAGE_OWNER:36 cenários,204 verificações. Inclui TO positivo com prova completa em modelo; negação ordinária/READ; request/decisão BA; owner estrangeiro e SYSTEM na imagem; herança/flags/DACL inadequados; root com DELETE/WRITE_DAC; reparse/substituição; divergência do objeto relativo; hash/refs e SD divergentes; paths/escopo alterados; Stop; expiração; provedor de open ausente; pending; fechamento incerto, erro de captura/revalidação com buffer SD devolvido e ownership perdido. O caso de contagem123→124 prova a checagem entre duplicações; falha da consulta nega antes de duplicar. Fixtures retidos são distintos e duráveis; não são zerados/rearmados como novas tentativas.

Os opens, arquivos, quotas, paths, processo/SCM/token e IO desses cenários são simulados. Helpers SID/SD operam em memória. O ensaio novo não chama CNG/RNG/KSP nem endpoint. A regressão scq_inputs_metadata já existente usa hashing público BCrypt sobre bytes de fixture, com IO modelado; isso não é KSP, CA ou assinatura. Não houve coleta de recursos, acesso ao alvo protegido, endpoint real, Start/Stop ou efeito produtivo. Não se declarou autenticação bilateral nativa.

## Recursos e admissão

O censo controlado passou a KNOWN1993744/limite3145728 em Selected e KNOWN1825176/limite2097152 em Legacy. A diferença2904 bytes é o novo subestado embutido no packet. Não é provisão física/commit, stack nativa ou teto de provider/kernel. A cadeia tem oito slots de diretório mais um de imagem, como limite da representação de custódia, não como quota de processo ou headroom demonstrado. No destino histórico usual, a aquisição usa quatro diretórios e uma imagem; o pico nativo real não foi medido.

As cópias controladas e cada open TO consultam a contagem corrente e negam novas referências a partir do guard124; cap128 e demais limites SCQ continuam vigentes. Custos internos de API não viram zero. Descritores têm validação de comprimento após GetSecurityInfo, que não prova ceiling do alocador; retenção, processo SCM, RPC, margens e relógios permanecem na política anterior.

O relatório nativo agora expõe sizeof dos inputs/chain e nove slots controlados, sem duplicar a reserva de campos já embutidos. resource_pre_missing continua63; resource_effect_obligations continua7; missing_pre_start conserva o bit de recursos. Estado da admissão de Start:UNKNOWN/DENY. Não foram importados B3, B0–B8 ou R0–R9 nem escolhidos freshness, duração, pisos ou risco residual do Start.

## Corte remanescente e caminho mínimo

A incompatibilidade owner-imagem está corrigida no caminho tipado com prova. Não se afirma que a CLI nativa inteira esteja pronta: ainda falta o caller administrativo que fornece os originais e o plano operacional completo antes de service_cli/launcher_scq_start_carrier, com confirmações SCQ/Start no mesmo controlador. No runtime vazio, a seleção original independente ainda é requisito anterior a scqb_open; o getter só empresta uma origem já adquirida. A nova prova de filesystem não fabrica essa origem. São cortes de composição específicos, não corrigidos por aprovação normativa, carrier, owner BA ou arquivo de grant.

Recursos materiais prévios e política de garantia também precisam de fonte aplicável. TO7 não aceita automaticamente risco opaco por ser diagnóstico. Identidade/PID/receipt e término futuros são obrigações posteriores, não provas impossíveis exigidas antes de Start. Uma coleta de pico isolada não preencheria a lacuna de provisão/política; nenhuma nova coleta foi proposta ou executada.

Ordem mínima: integrar esses dois callers de origem sem fabricar autoridade → fechar a suficiência nativa aplicável → qualificar a cadeia completa → estabilizar/selecionar uma única imagem → pedido separado de atualização governada/reconciliação → pedido específico de endpoint/Start diagnóstico. Nenhum pedido operacional executável está pronto agora. Ver NEXT-ORIGINAL-INGRESS-AND-NATIVE-START-ADMISSION.md.

EXE staged histórico não atualizado. O delta atual está em STAGED-IMAGE-DELTA.md. E21 PENDING; ED11/Phase6 PARTIAL; autoridade produtiva DENY; OS_QUIESCENCE UNKNOWN. HEAD/WIP, históricos, pins, reservas, chave fixture e PST0.6.4 preservados.
