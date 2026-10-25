# P3.5 — contexto de acesso e preparação manual read-only

## Resultado factual e limites

O contexto externo atual do executor é inadequado para a consulta protegida: SID do operador esperado S-1-5-21-785699550-1440458426-1682014700-1108, mas TokenIsElevated=0, TokenElevationType=3(Limited), Administrators S-1-5-32-544 deny-only, integridade média S-1-16-8192 e IsInRole(Administrator)=false. Thread impersonation não foi observada. Grupo Users/AU/Everyone habilitados não concede o READ_CONTROL requerido pela ACL histórica restrita a SYSTEM/BA. O token sandbox atual é outra identidade, CodexSandboxOffline1003, integridade média, sem BA habilitado. Não é operador autorizado N0–N9.

A consulta histórica que falhou não registrou seu token, PID/criação, grupos, integridade ou NativeErrorCode numérico. **Não é possível recuperar retroativamente o token efetivo exato ou provar que ele era idêntico ao atual.** A explicação contexto insuficiente é comprovada para o executor atual e consistente com a falha passada; não é prova exclusiva da causa histórica nem de ausência de alteração de ACL. P35_ACCESS_DENIED_CAUSE=INSUFFICIENT_CURRENT_CONTEXT_PROVEN_HISTORICAL_EXACT_CAUSE_NOT_DEMONSTRATED.

O registro antigo conserva Win32Exception com texto Acesso negado originado em CreateFileW/GetLastError. ERROR_ACCESS_DENIED é o código Win32 correspondente5, mas esse número não foi preservado como campo original da execução antiga. Não rotular um número reconstruído como novo NTSTATUS capturado. O leitor existente, quando autorizado e se falhar na API, preserva NativeErrorCode em WIN32=...; não foi executado em Verify nesta tarefa.

## API e argumentos reais

No helper pinado scm-file-acl-native.ps1, `Open(path,false,true)` usa:

| Campo | Valor efetivo |
|---|---|
| API/path |CreateFileW; C:/ProgramData/PapinhoAccelerator|
| DesiredAccess |0x00020080 = READ_CONTROL(0x20000) + FILE_READ_ATTRIBUTES(0x80)|
| ShareMode |1 = FILE_SHARE_READ; não pede share-write/delete|
| SECURITY_ATTRIBUTES/template |NULL/NULL; sem herança de handle|
| CreateDisposition |3 = OPEN_EXISTING; não cria|
| Flags |0x02200000 = FILE_FLAG_BACKUP_SEMANTICS + FILE_FLAG_OPEN_REPARSE_POINT|
| Pós-abertura |Identidade/tipo/reparse, GetSecurityInfo(SE_FILE_OBJECT,mask7), RAW SHA256, conteúdo quando arquivo, revalidação e close conferido|

BACKUP_SEMANTICS permite abrir diretório; não ativa privilégio nem concede autoridade. Get-Item ter funcionado não comprova READ_CONTROL. Esses argumentos são os mesmos do leitor anteriormente qualificado; nenhum pedido de escrita, WRITE_DAC/WRITE_OWNER ou criação foi identificado. Não foi demonstrado erro de flags/DesiredAccess. Não houve tentativa de reduzir direitos ou share protections para fabricar PASS.

## Comparação A–D

| Hipótese | Evidência | Estado |
|---|---|---|
| A token insuficiente |TokenElevation0/Limited3, BA deny-only, integridade média no contexto externo observado|COMPROVADO para executor atual; forte compatibilidade com a falha histórica, sem atribuição retroativa|
| B elevado autorizado |Reader exige SID exato/admin habilitado/sem impersonation/hostboot correto/console interativo|NÃO OBSERVADO nesta tarefa; resultado manual futuro obrigatório|
| C flags/DesiredAccess inadequados |Directory BACKUP_SEMANTICS e reparse point, read-only/OPEN_EXISTING, iguais ao reader qualificado|NÃO DEMONSTRADO como defeito; sem alteração|
| D ACL divergente |Não houve reabertura da árvore; descritor104bytes/hash688654...579 e inputs152bytes/hash0fcfd0...814 são pins históricos|UNKNOWN_CURRENT; não declarar ACL intacta nem divergente sem leitura autorizada|

SID BA deny-only não satisfaz ACE allow BA; ele só participa em ACEs deny. [Microsoft: atributos SID](https://learn.microsoft.com/en-us/windows/win32/secauthz/sid-attributes-in-an-access-token). Limited3 é a enumeração pública do token limitado, não full/elevated. [TOKEN_ELEVATION_TYPE](https://learn.microsoft.com/en-us/windows/win32/api/winnt/ne-winnt-token_elevation_type). A abertura de diretório e os flags/compartilhamento seguem [CreateFileW](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew). Essas especificações explicam propriedades; os fatos locais constam nos JSON/outputs.

## Procedimento reutilizado e pins

Nenhuma mudança funcional/pin foi aplicada. Usar diretamente o reader existente em console NOVO Windows PowerShell5.1 x64 já elevado pelo owner (sem childPowerShell, runas automático, senha, bypass de UAC/GPO ou alteração de ACL). O reader Verify mantém seus próprios checks, não chamar Run diretamente nem contornar Check-OperatorBoot.

- n0-protected-input-readonly.ps1 SHA2566732b4166a46dc93dbfade6040b084f148e5460b441cd17d5132068ee4bfc0e0.
- wrapper histórico n0-protected-input-readonly-manual.ps1 SHA256d0a6dee80ea9a6d2ac15989bba36f2cf5af61c288811b84a8d37679c98e88810; não modificado, sem necessidade de criar outro.
- n0-protected-input-readonly.cs SHA256cd3678f0a593c1c9e5de2c81172d9d6534c9655379c3070bc81fac181e07aec3.
- scm-file-acl-native.ps1 SHA256f52242fe74932d1539e83a742430ea523eda746ce39d8815b1b977cf6bb6ac75.

O reader valida self hash/runtime/helper/CS/recibo N2P, operador1108 administrador, sem impersonation, console, hostHash2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff e boot236441/2026-10-23T18:36:34.5000000Z. Mudança de hostboot não é repin automático.

Confere C:/ ID417614100:327680:5 e ProgramData417614100:131072:227641, depois raiz417614100:786432:1087874, Protected417614100:851968:2672648 e N0417614100:720896:2672656. Raiz/Protected/N0 exigem os104bytes originais,controle0x9004/hash688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579. Evidence5358bytes ID417614100:196608:2841429/hashbabab3ea393b605802933054ab011e6fcb66b21bffcc80923448715e911f27e0 e approved326bytes ID417614100:131072:2841430/hash5a2b7203afdece0adca77be278d6b86c18cc2cf457cab87474ee7dce0311c294 exigem descriptorRAW1 de152bytes/hash0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814. Identidade original,finalpath,tipo/reparse/links/noninheritance e segunda leitura são verificados. SACL permanece UNKNOWN.

## Testes isolados executados

Reutilizados binários SelectedRelease existentes, fontes auditadas; nenhuma build ou mudança de toolchain. BD model83casos PASS; busy/context111casos PASS; SDmodel13casos PASS. CompileOnly do reader original PASS, sem abrir alvo. Outputs e hashes em isolated-tests.json/logs. Mensagens PREFLIGHT ou valores de handles emitidos por fixture NÃO são preflight operacional nem nova medição qualificadora do LAB.

| Caso solicitado | Prova desta tarefa |
|---|---|
| Token autorizado |Positivo SINTÉTICO no BD model; token elevado real pendente|
| Não elevado |TokenIsElevated0 atual e modelo TokenElevation falso negado|
| BA desabilitado/deny-only |whoami atual e IsInRole falso; nenhum token foi alterado|
| Acesso negado |Resultado histórico preservado e faultinjection do contexto; alvo protegido não reaberto|
| Objeto divergente |Modelos identidade/fileID divergente negados|
| Reparse |Faultinjection busy/context ig_n0_input_identity nega FILE_ATTRIBUTE_REPARSE_POINT|
| Descriptor divergente |SDmodel13casos: diferenças owner/group/DACL/ordem/layout negadas; não aplicadas a arquivo|
| Fechamento incerto/UNKNOWN |BD/busy mocks de close failure e retenção sem segunda tentativa; não close de objeto protegido|

Primeira tentativa de capturar campos extras de token chamou CompileOnly no host de execução PowerShell7 e foi corretamente negada por WINDOWS_POWERSHELL_5_X64_REQUIRED antes de qualquer target. Corrigido apenas o host dessa consulta de token para PS5.1, sem repetir abertura protegida. whoami /all reportou erro opcional ao obter claims; user/grupos/integridade saíram, e elevação/impersonation foram consultadas separadamente com sucesso. Nenhum erro foi apagado ou tratado como prova positiva.

## Handoff e saída

Consulta manual preparada em manual-readonly-request.md: UMA execução Verify, sem salvar recibo automaticamente ou tocar alvo por escrita. Executar somente após aprovação específica. Resultado esperado N0_INPUT_READ_FIRST_ERROR=NONE/RETAINED=False/EXIT0/OWN_REFERENCES_CLOSED. Qualquer divergência de descriptor/ID/hash/boot/token, acesso negado ou close incerto mantém STOP, sem replay nem correção. Se houver retenção, preservar a janela/processo como o reader determina; não fechar/kill para fabricar cleanup.

CURRENT_TASK_COMPLETED=YES (diagnóstico do contexto atual e preparação concluídos; prova histórica exata limitada, P3.5 ainda pendente).
P35_ACCESS_DENIED_CAUSE=INSUFFICIENT_CURRENT_CONTEXT_PROVEN_HISTORICAL_EXACT_CAUSE_NOT_DEMONSTRATED
TOKEN_CONTEXT=EXPECTED_SID_LIMITED_TOKEN_ELEVATION0_ADMIN_DENY_ONLY_MEDIUM_NO_IMPERSONATION_OBSERVED
PROTECTED_ACL=HISTORICAL_PINS_PRESERVED_CURRENT_STATE_NOT_REVALIDATED
READONLY_PROCEDURE=EXISTING_READER_PINNED_MANUAL_APPROVAL_REQUIRED
P35_QUALIFICATION=PENDING_AUTHORIZED_MANUAL_RESULT
ADMINISTRATIVE_TRANSITION_APPLIED=NO
REFUND_AUTHORIZED=NO
SUCCESSOR_AUTHORITY=DENY
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
FIRST_UNRESOLVED_DIVERGENCE=AUTHORIZED_NATIVE_PROTECTED_DOSSIER_READ_NOT_YET_OBSERVED
NEXT=P35_AUTHORIZED_MANUAL_READONLY_RESULT
