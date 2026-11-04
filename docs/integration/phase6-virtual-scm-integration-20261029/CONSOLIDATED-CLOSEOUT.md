# Conta virtual SCM pessoal — integração offline concluída

TASK: PACC-PHASE6-VIRTUAL-SCM-ACCEPTANCE-AND-INTEGRATION. Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469. Histórico/WIP preservados, sem commit/checkout, instalação ou efeitos produtivos. ED11/Phase 6 PARTIAL; produção PST 0.6.4 e autoridade DENY.

## Decisão registrada

Conferência integral confirmou a proposta aprovada SHA256 `09c5ee8ccf4bfe1d5e3025e5eb0f7ce79318735494154ee8a2e4330f641bdd93`. [Registro ACCEPTED](../../adr/DECISION-personal-scm-virtual-account-20261029.md), escrito antes da implementação dependente. Proposta original preservada byte-exata. Especialização somente PERSONAL_LOCAL SCM, mesma root/runtime, preservando M5/P3/P4/F0–F9 e a alternativa SAM console. Nenhuma exceção R0–R9 LAB é herdada. Aprovação normativa e autorização técnica não concedem instalação ou uso produtivo.

## Código e callers integrados

`personal_runtime_identity_win32.h/.inc` define subestado embutido no `PAPACC_PERSONAL_NATIVE_CONTEXT` do packet canônico: não é owner/root/serviço novo. As entradas privadas existentes do configuration owner agora incluem `papacc_configuration_personal_runtime_bind/current/close`. Não há criação ou registro automático de serviço, setter de grants ou callback permissivo.

O perfil virtual resolve exatamente `NT SERVICE\PapinhoAccelerator` por LookupAccountNameW, verifica o SID NT de família service SID e exige TokenUser primário exatamente igual. Resolução não é prova de logon. A referência original ao processo é duplicada com direitos de consulta; PID, criação e estado vivo são observados por esse handle. Token original e token atual independente são comparados por TokenId/AuthenticationId/ModifiedId, sem reabrir por PID. Troca, término, thread errada e consulta incompleta negam.

O adapter abre somente referências SCM de consulta e conserva-as: serviço exato `PapinhoAccelerator`, account StartName exato, OWN_PROCESS, ServiceSidType esperado (NONE ou UNRESTRICTED) e PID RUNNING devem corresponder. Readbacks são repetidos antes/depois da consulta do token. O estado guarda tipo esperado separadamente; o valor esperado só se torna correspondência quando o readback confere. RESTRICTED não é reinterpretado como UNRESTRICTED: permanece não suportado/negado neste adapter até prova adicional. Não há Install/Start/Stop/ChangeServiceConfig.

TokenGroups e TokenRestrictedSids são retornos completos bounded, com limites, tamanho, ponteiros/SIDs e censo TokenStatistics conferidos. Ausência de duplicação do SID primário é permitida no ramo aprovado, nunca usada como substituto da consulta. SID presente desabilitado, DENY_ONLY ou ENABLED_BY_DEFAULT sem ENABLED nega; ENABLED e ENABLED_BY_DEFAULT não são confundidos. Administrators habilitado, token elevado/filtrado ou restrito nega conservadoramente; não se fabrica acesso efetivo para variante restrita. O perfil SAM dedicado mantém seleção separada, SID SAM primário e os mesmos controles de identidade/lifetime, sem SCM virtual implícito. O guard administrativo interativo anterior permanece inalterado.

O SID associado ao plano de efeitos deve coincidir com o binding vivo quando presente; associação CA/plano rejeita runtime substituído. O caminho CA revalida a continuidade do binding antes de uso. O guard CNG original continua rejeitando o TokenUser runtime como operador da chave; nenhum direito/ACL foi ampliado. Não há exportação nem uso de chave CA por runtime.

Pending/busy/retire e fechamento do owner incluem o subestado novo. Fechamento do binding nega enquanto houver CA emprestada/candidata ou worker ativo, preservando a ordem de lifetime. Há até cinco referências controladas (processo, token original, token de revalidação transitório, manager e service). Handles SCM usam CloseServiceHandle; tokens/processo usam CloseHandle. Falha de close retém as referências restantes, marca UNKNOWN e proíbe retry de fechamento pelo adapter. Stop do owner/packet impede novas consultas/bindings; fechamento factual continua separado. Identidade aceita por esta API é evidência somente: `personal_native_ceremony_ready` continua negado, sem autorização CA/CURRENT/admissão.

## Builds, testes e recursos

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings nos logs finais. 21 CTests afetados/configuração, 84 execuções PASS. Novo teste focal: 91 verificações/configuração. Compila o MESMO código do adapter com APIs de processo/token/SCM modeladas, helpers Windows reais de SID e uma thread Windows real concorrente rejeitada antes de I/O. Não cria processo artificial para satisfazer topologia.

Cobertura: virtual correto sem SID duplicado; grupo ENABLED; desabilitado/DEFAULT-only/DENY_ONLY; restrições/UAC; TokenUser divergente; processo encerrado, PID/criação/token trocados; query incompleta/censo inconsistente; falha de lookup/token/SCM; StartName/tipo SID/PID SCM divergentes; retorno SCM malformado; service SID NONE e UNRESTRICTED; replay de binding; thread concorrente; close de token e SCM incertos sem retry. As referências de falhas são modeladas, não handles OS esquecidos. A única thread real criada no fixture foi aguardada e fechada.

O teste da negação CA chama o guard original de `server_ca_key_win32.c` com TokenUser runtime modelado e confirma rejeição em IDENTITY_EQUAL, antes de qualquer NCrypt. O teste root/CA inclui troca de SID do binding e gates Stop. Não é prova de AccessCheck nem de negação CNG sob serviço real.

Regressões de enrollment, issuer NORMAL, PUBLISHED v2, presenter, plano, token administrativo, lifetime CA e M7 passam. Reutiliza-se o fixture efêmero CNG (89 verificações, três assinaturas/sete verificações) dentro da campanha afetada: criptografia real, publicação e autoridade modeladas, nenhuma chave persistente. A chave CurrentUser retida não foi aberta, alterada ou removida. Não houve nova campanha persistente MACHINE.

Novo subestado: 216 bytes fixos. Census Selected conhecido: 1.972.280/3.145.728; Legacy: 1.803.712/2.097.152, +216 em cada perfil. Os buffers bounded de consulta são transitórios na stack; esses números de layout não são pico total. Stack/CRT/provider/kernel e headroom operacional completo continuam não qualificados; nenhuma quota ou crédito aumentou. Logs layout mantêm PROVIDER_KERNEL=UNKNOWN e NATIVE_ADMISSION=DENY.

## Identidade e parent — estado factual

| Item | Estado |
|---|---|
| Norma virtual SCM | ACCEPTED neste perfil somente |
| Binding nativo | Implementado; qualificado offline |
| Token/SCM pessoal real | Não observado; nenhum serviço instalado/iniciado |
| Acesso CNG MACHINE real do runtime | Não demonstrado |
| Layout | Candidato: ProgramFiles/Papinho/Accelerator e ProgramData/Papinho/Accelerator; PERSONAL_LOCAL lógico |
| Parent | Observação anterior ItemNotFoundException; não adquirido, reservado ou qualificado por handle |
| Diretórios | Não criados; sem fallback para histórico N0 |
| Root/CURRENT pessoal | Não publicados; nenhum bootstrap autorizado |
| Histórico N0, Q4/TPM | Preservados, não executados e não herdados |

## Continuação funcional e caminho crítico

Enrollment CSR → issuer NORMAL → PUBLISHED → entrega permanece qualificado offline (77 verificações de composição), sem transformar CSR em autorização. M7 conserva identidade exata e Sessions não relacionadas (regressão de cohort PASS), sem alegar quiescência real. MANUAL/E8 ainda retorna SOURCE_ABSENT quando faltam o pacote original, observação administrativa e confirmação independente; conta virtual não fornece esses três produtores. Nenhum novo producer positivo foi fabricado. As origens factuais e o consumidor de confiança continuam a frente técnica independente; nenhuma cláusula nova é necessária para reabrir TPM/Q4.

Menor sequência: (1) concluir produtores/consumidores de confiança MANUAL/E8 e enrollment nas fronteiras já aceitas, com fixtures explicitamente isolados; (2) congelar a seleção operacional do target/layout e preparar procedimento exato, incluindo aquisição/proteção do parent ausente, sem criação implícita; (3) sob autorização própria, qualificar a instalação pessoal, SCM/token e negação/uso administrativo CNG MACHINE, publicação e reabertura; (4) observar lifecycle/Stop, composição Browser/C2/C3/provider e recursos restantes. Não transformar essas etapas em uma única autorização genérica ou executar agora. Nenhum novo SID/intent/root foi gerado para prepará-las.

E17: identidade pessoal implementada offline, prova operacional restante. E18: lifecycle/Stop real restante. E19/E20: D4/D5 e capacidade/composição quantitativa real independentes; UNKNOWN permanece. E21: obrigações históricas Q4 separadas, não satisfeitas por binding SCM. E22 depende dos gates aplicáveis e não é promovido. Demais requisitos de Phase 6, Browser, Legacy/NT4 e MANUAL/AUTOMATIC mantêm sua aplicabilidade na matriz recuperada.

Próximo pacote: [NEXT-PERSONAL-TARGET-AND-E8.md](NEXT-PERSONAL-TARGET-AND-E8.md). Hashes de fontes, executáveis e evidências em SOURCE-HASHES.json, QUALIFICATION.json e ARTIFACT-HASHES.json. Nenhum comando operacional é fornecido com placeholders.

