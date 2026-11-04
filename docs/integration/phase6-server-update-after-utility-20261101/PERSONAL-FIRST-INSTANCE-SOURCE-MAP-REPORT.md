# Primeira instância — bloqueio determinístico e mapa dos originais

2026-11-03. Auditoria de código offline. Não é mais uma coleta de recursos. Branchmain/HEADf341985924d29870d8ff627ed37c8a0874ad6469/WIPpreservado.

## Achado principal

O serviço consegue executar o diagnóstico fechado; essa etapa terminou comcloseknown. O fluxo operacional de instalação/CA/startup não está conectado. O bloqueio é explícito no código, não um erroWindows ainda semidentificação:

1. personal_present, emserver_launcher_personal_win32.inc, apresenta/confirmaplanoconfigurado mas chama papacc_configuration_personal_handoff_begin. Esse é oworkerpacket12 dehandoffDATA. Ele valida bytes/referências, não cria oalvo.
2. O caminho capaz decriaçãoexclusiva existe como papacc_configuration_personal_target_begin/packet13, mas depende de personal_native_ceremony_ready. Esse predicado, emserver_configuration_personal_data_win32.inc, retorna0 em todos os casos.
3. A entradaCA_INITIALIZE existe. Exige bootstrapchildoriginaljácriado/atoNEW_INSTALL_CAconsumido/handoffVERIFIED/objetoCAEMPTY/reservaeidentidades e personal_ca_native_current. Esse último também exige ceremony_ready. Não há callerpositivo naCLIatual que complete a cadeia.
4. StartupNORMAL usa papacc_server_service_startup_source_status, que reportaORIGINAL_UNAVAILABLE/ERROR_ACCESS_DENIED naausência deprodutororiginal. Executar o comandoSCM peloCLI não fornece essa fonte. O caminhoSCQdiagnóstico éespecializado, não licençaprodutiva.

Portanto trocarhash, aumentarG3, repetirStart ouaprovarnovamente o mesmopacote não supre as fontes faltantes. O nome first-install não significa que o presenter atual é instaladoroperacional. Seus receipts dizem expressamenteDATA_ONLY/BOOTSTRAPDENY/CURRENTDENY.

## Cadeia de dados e fontes exigidas

| Fato original | Código reaproveitável | Ownership/lifetime | Fronteira que falta |
|---|---|---|---|
| Operador/contexto técnico | personal_token/native_open/current | token/SD/parentno packet doowner até fechamento factual | Admissibilidade conjuntaF1/F2, não só elevação |
| Parent governado | native_open/NtCreateFileOBJ_DONT_REPARSE/FI/SDcurrent | handleoriginalFILE_ADD_SUBDIRECTORY, shareREAD; mesmoobjeto atéfilho/workerclose | Proteçãoefetiva/ancestrais e vinculação ao propósito deinstalação; DACLpresente sozinhoinsuficiente |
| Configuração exata | ReadFile/parse/validate/source_current | sourcehandle ebytesfrozen no packet; revalidaçãoantesdeefeitos | Fontes/autoridade/material/gatesoperacionais além deparserDATA |
| Legitimidade/plano/ato | presenter/console/nonceCSPRNG/effect_confirm | mesmoowner/invocação/plano; nenhuma autorização persistente | F5conjunçãoadmissível, não transformarCONFIRMEDemgrant |
| Criaçãoexclusiva | target_begin/worker/bootstrap_create_child | consumoirrevogável antesdesubmissão; filhooriginalretido; Stop/UNKNOWNnãoautorelease | Sourceoriginal de admissibilidade/recursos antesdechamartarget_begin |
| CA inicial | ca_initialize/keyownerexisting | candidatoantesdaAPI/uma criação/retentionparcial; mesmaborrow/handoff | Callerjointpurposeoriginal, P4/recursos e contextoqualificados; semCAprévia exigida |
| CA pública/estado | TBS/matching/backend/core/root existentes | backingcertificado/key/ref/buffers e leases atéconsumo/close | Origemcertificado/validadeUTC/reservadurável e mesma geração, nãoponteirooufixture |
| CURRENT/ativaçãoNORMAL | rootCAS/poll/PUBLISHEDv2/readback/activate | transação eissueroriginais, mesmosowners/serials/pins | Continuidadeoriginal e autorizaçãoantesdepublicação; nãoinventarproofbits |
| Startupoperacional | SSO-B/getters/SCMownerexistentes | originalindependente antes doefeito e todoslifetimes | Fontepositivainstalação/readiness/lifetime, nãoargv/RUNNINGposterior |

Não há umhandoff legítimo do resultado READTEST_ONLY para a autoridadeCAprodutiva. A continuidade deobjetos é exigida; osdocumentosconsumidos não podem ser restaurados numa cerimônia distinta.

## Correção legítima versus bypass

Correção necessária é desenvolvimento da composição nativa da primeira instância sobF0–F9/M0–M11, usando os owners existentes, não um conserto deparâmetrodeStart. O plano deve ligar os fatos originais deator/parent/plano/autoridade/proteção/recursos ao predicado deadmissibilidade e ao callerjointpurpose; depois criar uma vez, construir/custodiar a identidadeCA e continuar o mesmo root. Fonteausente/P4nãoqualificado/recursosUNKNOWNcontinuamDENY.

Trocarreturn0portrue, usarflags defixture, selecionar checkpoint93, aceitarqualquerconfirmaçãoCLI, importarcontextoLAB ouchamarca_createantesdasfontes é incompatível comF1/F2/F5/F7/F8. Não foiaplicadobypass oucallerdeefeitos semessasprovas. Nãohá defeitofuncionalprovadoqueautorizeapenas remover oguard.

O bloqueioP4/K5permanece separado: oresultadoBADKEYSETnãoqualifica a negaçãodepermissão exigida. Nãohá evidênciadeque implementar ocallerdeinstalação fariaK5passar. Assim, também nãofoifabricadoum pedidooperacional deCA/NEW_INSTALL ou um comandoquegarantissefuncionamento.

## Escopo de continuação técnica

A solicitação atual do proprietário autoriza continuar trabalhooffline e depuração sem reaprová-los emcadaetapa. Isso não alteraaccepted nem fornece osfatosoriginais que faltam. A aceitação deF0define a autoridadelocal, sem exigir órgãoexterno, mas condiciona oefeito àconjunção factual completa. Nãoénecessária novaADRpara umcallerjácompatível; também nãosepodedefinir outra política desegurança porconveniência.

A continuidade deimplementação exige resolver concretamente a aquisiçãooriginal, proteção efetiva eassurance da primeira cerimônia; osenvelopesUPDATE/READ/fixtureaprovados não seaplicam automaticamenteaCAprodutiva. Semeles nãoexiste umpatchpositivojustificado quepossa completar essaadmissibilidade. Oresultado desta auditoria não é afirmarimpossibilidadeuniversal; é delimitar precisamenteafronteira do códigoatual e dasprovasdisponíveis.

## Estado final e preservação

Concluído nesteescopo: rastreamentocaller→handoffDATA→guardnativofechado→target/CA→startupsource, comanchors/hashes em PERSONAL-FIRST-INSTANCE-SOURCE-MAP-EVIDENCE.json. Sem coletaouoperaçãonativa, sem alteração deproduto/accepted/binários, semtestesinalteradosrepetidos. Imageminstalada882688/ID417614100:1966080:412413 ePST0.6.4 preservados.

Não se declara oserviçooperacional concluído. Diagnósticofechadopassou como lifecycle, isolamentoREADnãofoiqualificado e fluxo deprodução permanece incompleto. HistóricoUNKNOWN/retenção/quiescênciaOS/autorizaçõesconsumidas continuaminalterados. Nenhum novo comando deStart/UPDATE é justificadopor repetir ainvestigaçãojáencerrada.
