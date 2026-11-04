# Reconciliação nativa TPM C0–C9 — 2026-10-28

## Resultado factual

PASS_AT_OBSERVATION para a prova matemática transitória de posse do material privado correspondente à EK pública qualificada. Saída manual original preservada integralmente em OWNER-MANUAL-TRANSCRIPT.txt e normalizada em NATIVE-RESULT.json; hashes e resumo em RECONCILIATION.json. Não é atestação independente do hardware, cadeia/revogação, custódia ou continuidade durável.

Pedido b575659fe446eb4b2ed3d53881dfbef77855a02a78d87fb76cccc96f3faf0c8b e aprovação94f9ae80a9d8c5bc6ef179a97e43244e8237cbfd235131828d94b2af82bd8afc conferem byte-exatamente. Fontes/dependências e manifestos qualificados conferem; originais observados dos quatro artefatos possuem volume/FileID/tamanho/hash registrados no JSON, sem substituir identidade por igualdade de hash. Guard de arquivos de papel retornou máscara7. Receipt original presente com174bytes e REQUEST/APPROVAL exatos; hash035749eb1b20ffacd1fb19c4ad24b2aa6a366572a35138bc52dd4711f799750d. Aprovação USADA, sem retry. O receipt continua suplemento administrativo, não ledger Q4 resistente a rollback.

## Participantes e recursos

Boot237900, nativo4268/criação134376716093842972; pai10108/criação134376716036161332; auxiliar20804/criação134376716036542731. Censo contém exatamente nativo e pai; HWND10108 é metadado. Consultas repetidas de pai/nativo raw20805 convergem para auxiliar20804; associação criptográfica completa e imagem mapeada permanecem UNPROVEN conforme C4. Identidades históricas não foram usadas como pins.

178 amostras próprias conhecidas/erro0: entrada80, pico102, última após PLATFORM_CLOSED93. Consulta guard99/124. Nenhuma amostra ultrapassa128. Quatorze observações administrativas: primeira716+270=986, demais691+271=962, todas dentro1024. Distância amostrada aos limites:26 próprios e38 administrativos; não é margem futura concedida, pico universal ou orçamento transferível. Processos distintos não foram somados ao orçamento próprio. Internos Windows/TBS/kernel/transitivos e intervalos não amostrados UNKNOWN.

Janela entre primeira/última amostra36047ms inclui confirmação humana; não é tempo exclusivo TPM. Máximos amostrados: private1400832bytes, working set6701056bytes. Não representam custo total kernel/transitivo ou teto de memória.

## Protocolo e fechamento

NATIVE_API_COUNTS open1/submit10/close1, firstNONE/stageNONE/native0. Contexto abriu e fechou CLOSED_CONFIRMED. Fluxo qualificado e seis fases PUBLIC_CANDIDATE implicam1GetCapability,6ReadPublic iniciais,1StartAuthSession,1GetRandom,1ReadPublic final:10submits diretos, dentro12. Distribuição inferida do código core byte-exato e fases, não de captura independente dos pacotes. Comandos internos TBS não observados.

proof=true só é emitido pelo core após nonce/salt frescos, validação HMAC da resposta e public area/Name final idênticos à EK selecionada. Name000b46b6236cd5e80474e064d24deea3db9c71d4cdb0d3952e419789389f5f89aa80. session_end=AUTHENTICATED_CONTINUE_CLEAR_SEMANTIC. Challenge/segredos não foram exportados para evidência; portanto não existe replay criptográfico independente dos bytes privados da sessão. Fechamento controlado retornou refs0, observer_close_error0, observer_refs_retainedfalse, retainedfalse; receipt_written174/expected174. Não se afirma fechamento de todos os recursos internos, nem quiescência global.

## Correções e qualificação

Nenhum defeito técnico demonstrado nesta execução; nenhuma alteração funcional, build ou campanha repetida. Reconciliadas contagens, sequência, identidades estáveis, artefatos e limites por assertions offline sobre a saída completa; os18CTests/Debug/Release anteriores permanecem evidência offline. HEAD f341985924d29870d8ff627ed37c8a0874ad6469 e históricos/pins preservados. Agente não executou TPM, Q4 ou operação protegida; leitura local do receipt não equivale a readback Q4.

## Primeiro gate e caminho mínimo restante

Subgate de posse matemática nativa agora demonstrado neste escopo. E21 NÃO encerrado. Primeira propriedade ainda não estabelecida para a testemunha: binding autenticado à geração/dispositivo e admissibilidade de confiança com revogação UNKNOWN. Custódia efetiva, autorização NV e integração persistente root/contador continuam não demonstradas. Próximo gate é resolver esse binding/confiança e a custódia segundo W/L antes de concluir um pedido exato de provisionamento; não repetir esta prova bem-sucedida nem presumir que ela autoriza NV.

Somente após requisitos técnicos/normativos completos: pedido separado de provisionamento/ensaio NV, decisão específica do proprietário e execução manual delimitada; depois continuidade/rollback factual e pedido próprio de readback cross-boot Q4. Disposição documental Q4 exige sua própria autoridade e evidência; sem limpeza/refund/recuperação inferidos.

E17 identidade/acesso/readiness e E18 lifecycle/Stop exigem prova de serviço independente elegível. E19 depende D4/D5 quantitativos justificados; E20 composição real Browser/C2/C3/provider e bindings ainda ausentes. Não dependem desta prova TPM nem são fechados por ela. E22 derivado pendente; classificação4demonstrados/12limitados/6pendentes preservada.

E21=PENDING; ED11=PARTIAL; Phase6=PARTIAL. NV/Q4/Start/Stop/SCM/ACL/CURRENT/assinatura/recuperação não executados ou autorizados por esta reconciliação. UNKNOWN e reservas históricas preservados.
