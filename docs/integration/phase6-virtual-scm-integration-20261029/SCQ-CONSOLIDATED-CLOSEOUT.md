# Destino pessoal e qualificação offline SCM/CNG

TASK: PACC-PHASE6-PERSONAL-BINARY-DESTINATION-AND-SCM-CNG-QUALIFICATION.

Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469. Implementação offline, sem efeitos protegidos. HEAD/WIP e evidências anteriores preservados. Não se criou diretório de instalação, conta, serviço, chave persistente ou CA produtiva. Não houve execução nativa do novo probe, consulta SCM real ou uso de chave MACHINE; a regressão criptográfica efêmera existente é explicitada abaixo. PST 0.6.4 permanece vigente.

## Proposta normativa exata

[BINARY-DESTINATION-NEW-INSTALL.CANDIDATE.md](BINARY-DESTINATION-NEW-INSTALL.CANDIDATE.md), SHA256 `e6d5ab6d57486dbe15d548e68e35dea4fd9abb124852ae36f9143378e9335d4d`.

Estado PROPOSED, não ACCEPTED. A proposta antiga e a decisão accepted de SCM opcional foram integralmente confrontadas e preservadas. A única especialização é `%ProgramFiles%\Papinho\Accelerator` para **novas** instalações PERSONAL_LOCAL. Instalações existentes mantêm caminho/configuração/identidade; nenhuma migração ou adoção é inferida. Não se alteraram default/código de destino ou documento accepted. Manter o destino anterior continua alternativa sem nova decisão. A conta virtual já accepted não exige reaprovação.

## Código técnico integrado

Novo `apps/server/personal_cng_probe_win32.h/.inc`, subestado `runtime_cng_probe` no packet canônico existente. Sem root, serviço, manager de chaves, allocator ou IPC adicional. A entrada `papacc_configuration_personal_runtime_cng_probe` usa o mesmo owner/writer, Stop, pending/borrows e o guard nativo existente. Ela continua NOT_SUPPORTED/DENY sem autoridade nativa; nenhum callback positivo ou argv concede autorização. Mesmo uma observação diagnóstica não retorna admissão produtiva.

O coletor abaixo dessa fronteira implementa a sequência concreta: congelar/copy bounded do nome de fixture no formato original do owner CA; verificar thread/Stop; rejeitar impersonação; verificar processo próprio, perfil virtual e binding original; abrir Microsoft Software KSP fresco; tentar NCryptOpenKey MACHINE|SILENT uma única vez; revalidar processo/token/SCM e contexto efetivo; fechar referências governadamente. Ele usa o binding SCM/token já integrado, sem herdar R0–R9 do LAB. O nome sozinho não prova que uma chave é fixture: a futura operação precisa ligá-lo ao original protegido e ao propósito autorizado.

Não confundir TokenUser de um processo observado pelo administrador com o token efetivo da thread que fará CNG. O coletor exige PID próprio e ausência de thread token antes/depois do KSP; falha de query ou impersonação não é negação KSP. Não duplica token para impersonar outra conta nem usa privilégios administrativos como substituto da identidade virtual.

Classificações distintas, com origem da primeira ocorrência (local, Win32, NCrypt ou binding), estágio, raw status e contadores:

| Evento | Classificação diagnóstica | Alcance |
|---|---|---|
| Perfil/processo/thread efetivo errado | LOCAL_DENY | KSP não consultado; não prova isolamento nativo |
| Token/SCM/continuidade incompletos | QUERY_FAILURE | Nenhuma concessão por resultado parcial |
| Provider falhou ou resultado inconsistente | PROVIDER_FAILURE | Não é permissão negada |
| Open retornou NTE_BAD_KEYSET | NOT_FOUND | Não é PASS de isolamento |
| Open retornou NTE_PERM, sem handle, contexto contínuo | PERMISSION_OBSERVED | Fato diagnóstico da API, não autoridade ou prova global |
| Open teve sucesso ou devolveu handle mesmo com erro | UNEXPECTED_OPEN | Não assinar/exportar; fechar/registrar e negar |
| Stop observado | STOPPED | Não iniciar novas consultas, somente closes governados |
| Close de KSP/thread token falhou | CLOSE_UNKNOWN | Reter referências; sem segunda tentativa |

O coletor não contém Create/Finalize/SignHash/ExportKey/DeleteKey. Portanto não cria, assina ou exporta material privado, nem transforma uma abertura inesperada em uso da chave. Assinatura/exportação runtime não executadas permanecem NOT_REACHED, não retornos nativos inventados. Propriedade export0 e negação do caminho fresco continuam evidências distintas.

O guard CA administrativo original foi preservado: papacc_ca_key_open/validate continuam negando runtime como operador. O teste do coletor é uma qualificação explícita da camada de observação, não uma forma de forçar PASS na aplicação. O teste root fornece nome bem formado e contexto local suficiente para detectar uma entrada acidental: confirma NOT_SUPPORTED, phase zero e zero chamadas de provider pela aplicação.

Pending/retire/fechamento passam a contabilizar o novo subestado. Referências NCrypt e thread usam closes próprios. Falha conserva primeira ocorrência e close_error separado, retém handles e bloqueia reuse, reinitialization e fechamento que omitiria recursos. Binding runtime não pode fechar enquanto probe estiver ativo/retido ou CA estiver emprestada. Resultados terminais guardam contadores: raw zero com zero chamadas não é confundido com API bem-sucedida.

## Builds, testes e censo

Selected/Legacy Debug/Release: quatro builds PASS, zero warnings nos logs finais. 22 CTests/configuração, **88 execuções PASS**. Novo teste source-linked: **95 verificações/configuração**, compondo o binding real de código e o coletor com APIs de processo/token/SCM/NCrypt modeladas. Helpers Windows de SID são reais; nenhuma chave real é aberta/criada por esse teste. Continuidade de identidade mantém suas 91 verificações/configuração e thread concorrente real.

Cobertura nova: provider ausente, sucesso sem handle, erro com handle parcial, nome/alvo inválidos, observador versus self, query incompleta, impersonação/falha de consulta, Stop antes/depois da API, NTE_BAD_KEYSET, NTE_PERM, abertura inesperada com/sem erro, identidade pós-query divergente, falha de close KSP e de thread token, primeira ocorrência preservada e replay sem nova chamada. Aplicação continua negada; o positivo do componente significa somente classificação correta da negação modelada.

Regressões de presenter, CA/borrows, issuer NORMAL/PUBLISHED, enrollment e M7 foram executadas por mudança funcional do packet/headers. O fixture CNG efêmero existente mantém 89 verificações, três assinaturas e sete verificações reais: não é MACHINE persistente nem prova do token de serviço. Nenhum fixture persistente foi repetido. O receipt da chave retida é conferido sem abrir a chave.

Subestado novo: 304 bytes fixos. Censo gerenciado Selected: 1.972.584/3.145.728; Legacy: 1.804.016/2.097.152. São +304 sobre a integração virtual anterior; nenhuma quota aumentou. Há até duas referências NCrypt e uma referência de thread inesperada a reter no subestado; considerar também as do binding e seus overlaps. Stack, CRT, KSP/LSA, kernel e picos transitórios não viram conhecidos pelo tamanho do struct. NATIVE_ADMISSION=DENY e PROVIDER_KERNEL=UNKNOWN permanecem nos logs.

## Parent e operação factual

O parent continua não adquirido. O código existente abre/revalida parent e cria filho exclusivamente; esta tarefa não cria ancestrais nem relaxa proteção. O [plano mínimo](TARGET-READINESS-20261029.md) permanece válido quanto a: ancorar ProgramData por known folder original; preparação exclusiva/explicitamente autorizada de Papinho; revalidar owner/descritor/direitos e não somente DACL não nula; manter Accelerator ausente até NEW_INSTALL; não adotar colisões nem copiar proteção N0. Sem diretórios de instalação criados nesta tarefa.

A lacuna do probe KSP anterior ao guard local foi resolvida **em código offline**. Não foi habilitado ServiceMain diagnóstico, ps_binding_ready ou personal_native_ceremony_ready, nem criada fonte de autoridade para execução. Logo, esta entrega não afirma que já existe comando manual pronto ou instalação elegível. Despacho e autorização original da sessão nativa ainda devem ser vinculados antes de executar: manter um guard fechado não é uma prova de SCM/CNG.

Primeira prova factual restante: sob uma autorização específica, obter o processo real de `PapinhoAccelerator` com configuração/token originais e observar acesso administrativo e negação KSP à mesma chave MACHINE isolada, com parent/proteção, imagem, consumo, recursos, Stop e closes demonstrados. O admin owner já oferece criação sem overwrite, export0, signing-only, validação e reabertura; esse mecanismo não foi executado contra MACHINE real aqui. Nome de chave, hashes de fixture, matemática efêmera e classificações modeladas não substituem essa prova.

O pedido operacional não é criado com bindings ou hashes futuros. Antes dele faltam aprovação focal se o novo destino for escolhido, o handoff de autoridade/dispatch nativo e os bindings/descritor/censo da sessão. Sem criar nova arquitetura ou retomar TPM/Q4.

## MANUAL/E8 e enrollment

MANUAL/E8 permanece SOURCE_ABSENT antes do KDF: trust_unavailable.c não possui produtor dos objetos pacote/observação administrativa/confirmação independente. O novo coletor CNG não cria esses objetos nem uma trust lease. Não se instalou factory ou callback permissivo. Enrollment CSR → NORMAL → PUBLISHED → entrega permanece qualificado offline (77 verificações); M7 conserva identidade exata e Sessions não relacionadas. Não houve novo defeito factual que justificasse modificar esses caminhos; suas regressões afetadas passam.

O trabalho independente é a composição dos três produtores originais E8 e seu consumo Browser/GUI, conforme D3 vigente, sem depender de instalar SCM. GUI/checkbox/CSR/assinatura matemática não geram confiança. Não se alteraram Browser, wire ou formatos persistidos; nenhuma capacidade Legacy/NT4 foi promovida.

## Caminho crítico atualizado

1. Decidir apenas a substituição de destino para NEW_INSTALL, ou conservar o accepted anterior; não reaprovar conta virtual.
2. Vincular a sessão diagnóstica ao ato/autoridade original no owner existente e ao despacho nativo, sem ativar produção por flag; congelar imagem, objetos e limites. O coletor e sua classificação já estão implementados, não devem ser reinventados.
3. Emitir UMA solicitação exata de qualificação e aguardar aprovação operacional; executar somente manualmente, sem retry.
4. Reconciliar parent/SCM/token/KSP/Stop/UNKNOWN no alcance observado; publicação, CA produtiva e demais gates não decorrem da sessão.

Em paralelo: produtores E8, enrollment/M7 e política/composição real Browser/C2/C3/provider. E17/E18 continuam pendentes de operação; E19/E20 de capacidade/composição; E21 histórico é separado; E22 permanece derivado. Phase 6/ED11 PARTIAL, autoridade DENY.

Hashes e resultados: SCQ-QUALIFICATION.json, SCQ-SOURCE-HASHES.json e SCQ-ARTIFACT-HASHES.json. [Próximo prompt](NEXT-SCQ-DECISION-AND-NATIVE-HANDOFF.md). Nenhum ADR accepted foi alterado.
