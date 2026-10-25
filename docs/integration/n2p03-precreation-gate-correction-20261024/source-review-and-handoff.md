# N2P03 — correção focal, qualificada offline; nova aprovação necessária

HEAD main 913c90cf2328f7939d8f5b927640601903ac6e91. Preimagens e hashes em baseline.json; proposta anterior e seus documentos/autorizações preservados. Somente os dois arquivos de preparação03 foram corrigidos. Nenhuma fonte de produto, pin operacional, ACL ou SCM mudou. A autorização de hash533c9b39dd1ca1c2c31119f22ce05ed61889c7cf8f894767830bfbe1117fa438 NÃO se transfere.

## Consulta e memória nativas

Removido GetFileAttributesW. Absence usa NtCreateFile com OBJECT_ATTRIBUTES.RootDirectory=dirs[4], mesmo handle original qualificado por CheckDirectories antes/depois. UNICODE_STRING contém somente um leaf fixo, sem slash/colon, <=128. DesiredAccess=0x100080 (READ_ATTRIBUTES|SYNCHRONIZE), ShareAccess7, FILE_OPEN1, options0x00200020 (OPEN_REPARSE_POINT|SYNCHRONOUS_IO_NONALERT), atributos0x1040 (CASE_INSENSITIVE|DONT_REPARSE). Nenhuma escrita/criação/adoção na consulta. Somente STATUS_OBJECT_NAME_NOT_FOUND c0000034 sem handle é ausência; path-not-found, access/sharing/reparse/informational/UNKNOWN negam. Sucesso com handle é colisão, inclusive reparse, sem seguir/adotar objeto. Handle transitório fechado e checado; erro de fechamento retém owner sem retry. Nome ausente NÃO é reserva: CREATE_NEW final preservado.

Reutiliza o padrão RootDirectory/FILE_OPEN já usado no gate canônico scm-instrumented-cycle-lab.inc; não altera esse owner. Frames de nome/OA/IOSB/output são unmanaged e permanecem alocados em pending, juntamente com handle original; Close retorna UNKNOWN sem soltar pendências. Resultado/status gravado no canal de evidência existente, não novo recorder.

Referência oficial: [NtCreateFile](https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntcreatefile). RootDirectory seleciona nome relativo; FILE_OPEN não cria; OPEN_REPARSE_POINT não segue reparse final. A classificação deste gate permanece fail-closed e não autentica o console.

## Ordem corrigida

Pins/hostboot/operador → Prepare (token elevado/estatísticas, ancestrais, fontes/config/runtime/decisões/RAW1, dois nomes) → plano/confirmacão literal → Check-OperatorBoot novamente → RevalidateBeforeEffects (token/objetos/nomes) → Run com CREATE_NEW/readback/verify/fechamento → evidência no console → recibo exclusivo somente se EffectsCreated.

Falha antes de qualquer input não cria recibo. Prepare e confirmação divergente passam pelo finally para fechar aquisições. Se o primeiro input foi criado e a segunda aquisição falhou, há efeito parcial e recibo factual é tentado após esse resultado. Colisão/falha de recibo posterior preserva inputs, primeira falha e evidência no console; não repete Place. Primeiro erro anterior não é substituído pelo do recibo. Close incerto conserva owner/writer/stream e não retorna sucesso. Não há exigência contratual de recibo anterior aos inputs neste procedimento.

## Qualificação

Compilação C# em Windows PowerShell5.1 x64, nível4/TreatWarningsAsErrors: PASS; modelo existente50 checks PASS. Fixture exercita o tail REAL do launcher, substituindo somente owner/boot/confirmação/canalconsole e destino de recibo por mocks/diretórios temporários. Dez cenários: Prepare negado, pai negado, confirmação divergente, revalidação negada, Stop, primeiro create negado (zero recibos/inputs); segundo create negado (partial+recibo), recibo existente (sem overwrite), close UNKNOWN, sucesso sintético. Dez combinações NTSTATUS/IOSB testam ausência, colisão, pending, acesso negado, sharing, reparse, path-not-found e inconclusivo. Modelo50 cobre ainda pai/ACL/objeto divergentes, replay e parcial. Ver isolated-workflow-tests.ps1 e resultados.

Nenhum NtCreateFile de admissão real foi executado: classificação de status é teste sintético, não prova positiva da raiz ou ausência atuais. A raiz tem PASS manual histórico fornecido pelo owner; revalidação fresca obrigatória. Fixture criou apenas diretórios/recibos isolados em TEMP, preservados; zero arquivos protegidos. Duas falhas de montagem inicial do harness (referência assembly/using) foram corrigidas no fixture; não representavam falha de produto. Não houve Place operacional. Fonte C/C++ inalterada: builds Selected/Legacy não repetidos; compilação C# requerida passou.

## Candidatos preservados

Evidência7573bytes SHA15b643829189855149826779e9b6a54c1650ed9df6360de63e90cd35bf4887d7; V2397bytes SHA5087db7a1e02e3334e827360a464c25b3edc1b0059d0a01d506cf17da6ec4050. Descritor152bytes original SHA0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814. Nomes/recibo e confirmação não alterados; IDs futuros não inventados.

## Handoff substitutivo — comando candidato, NÃO executar antes de reaprovação

```powershell
& "C:\Projetos\PapinhoAccelerator\tools\qualification\n0start03-raw1-input-placement.ps1" -Mode Place -ExpectedScriptSha256 0d44d3b5112dd37f16abcee1f367b1dca4eb6270eafd6b7fed89c985bb306b5e
```

Confirmação: `PLACE_N2_INPUTS N2P-20261024-03 0b1de869e11493b7cb08fb329578f6ce`.

Um único PowerShell5.1 x64 já elevado/interativo, sem classes do placer carregadas. Efeitos futuros limitados aos mesmos dois novos inputs ProgramData/N0 e recibo manual-N2P-20261024-03.txt no pacote de preparação anterior, agora posterior aos inputs. Sem criação de diretório, sem alteração de ACL existente, sem intent/imagem/sink/SCM/preflight/Start. Escape solicita Stop entre chamadas; perda de host não é cleanup. Não retry/overwrite/delete/rollback/kill. Saída e recibo devem ser enviados completos; exigir VERIFIED/hash/IDs/SD para ambos, NATIVE_OWNED_CLOSED=True, FIRST_ERROR=NONE, ADMIN_RECEIPT_CLOSE=CONFIRMED e EXIT0. Qualquer parcial/UNKNOWN permanece preservado e DENY. Novas consultas/pins só após revisão do resultado, sem reutilizar leitor antigo como prova de arquivos03.

CURRENT_TASK_COMPLETED=YES
PRECREATION_ABSENCE_GATE=RELATIVE_NT_FILE_OPEN_OFFLINE_QUALIFIED
PARENT_HANDLE_BINDING=ORIGINAL_QUALIFIED_PARENT_REVALIDATED
PREPARE_BEFORE_EFFECTS=YES
ADMIN_RECEIPT_ORDER=AFTER_INPUT_EFFECTS_ONLY
EXCLUSIVE_CREATION=PRESERVED
PLACEMENT_EXECUTED=NO
FILES_CREATED=NO_PROTECTED_FILES_TEMP_FIXTURES_ONLY
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
SUCCESSOR_ADMISSION=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_EXCEPT_AUTHORIZED_TWO_FILE_PATCH
FIRST_UNRESOLVED_DIVERGENCE=NEW_EXACT_MANUAL_REAPPROVAL_PENDING
NEXT=N2P03_EXACT_MANUAL_REAPPROVAL
