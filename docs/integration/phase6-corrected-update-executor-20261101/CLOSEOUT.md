# UPDATE corrigido — executor protegido e DATA separados

2026-11-01. Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469,
WIP preservado. Nenhum novo owner, serviço, root, canal ou distribuição.

## Estado factual

Preparação do utilitário reconciliada e autorização consumida, sem retry.
Leitura atual autorizada nesta tarefa adquiriu referências originais read-only,
verificou file IDs, atributos/reparse, conteúdo SHA256 e SD observado, retendo
cadeias durante a captura e fechando-as uma vez com resultado conhecido.
CURRENT-READONLY.json preserva resultados, UTC e consultas SCM somente leitura.
Nenhum produto/controller/utilitário foi executado para obter esse snapshot.

| Papel | Objeto/pin conferido |
|---|---|
| Parent protegido |417614100:8257536:71209|
| Executor papacc_update.exe |417614100:1507328:540527;104448 bytes;6c0e77173f772b7748d08d9ef2ca29f844d629cc0090b3a73a3cafa87ea0bb87|
| Canônico anterior |417614100:6225920:71237;571392 bytes;c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf|
| Novo server DATA |758272 bytes;7612d1114b641875ad4536a6e9a2a4899a9115674b8a098fe7cc8b7767b1cf7f|

Parent, utilitário, canônico e DLLs: owner BA, DACL protegida
O:BAG:BAD:P(A;;0x1200a9;;;AU)(A;;FA;;;SY)(A;;FA;;;BA). AU RX, BA/SY full.
Ancestrais Program Files/TrustedInstaller e Papinho protegidos; raiz é anchor,
não dispensada universalmente. DLLs instaladas têm hashes idênticos selecionados.
Sem reparse observado ou quebra de continuidade dentro da captura retida.
Não é atestação de intervalos anteriores ou páginas executáveis mapeadas.

SCM qc/queryex/qsidtype: PapinhoAccelerator, WIN32_OWN_PROCESS, DEMAND_START3,
ERROR_NORMAL1, NT SERVICE\PapinhoAccelerator, SID UNRESTRICTED, comando
"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed,
STOPPED/PID0. Exit Win32 histórico1077 não é falha desta consulta ou prova de
readiness; nenhum Start foi tentado. Snapshot não impede ação de outro admin.
Saída localizada sc.exe pode conter substituições de acentuação no JSON;
valores ASCII de serviço/comando/tipos/conta/status estão preservados. Gates
operacionais usam structs/APIs nativas, não parsing dessa saída localizada.

## Binding efetivo e capacidade

update_main.c aceita somente --owner-manual-binary-update, encaminhando streams
e argv ao MESMO owner; não tem dispatcher de serviço ou autopreparação.
Target papacc_update liga a biblioteca canônica. Artefato preparado é o mesmo
qualificado nas quatro configurações, não cópia do server nem executor DATA.
U2 mantém slot7/executor estrito: path sob parent original, ID/hash esperado,
PID/criação/contexto, imagem nativa versus handle original, módulos DLLs versus
arquivos protegidos, SD e aliases; ausência nega. Slot8 é DATA somente após
executor legítimo, com SD interpretável, original/hash e continuidade retidos.
Entrypoint recebe argc17: caller agora fornece também ID3/hash do executor,
faltantes no comando anterior consumido. Nenhum positivo de fixture vira grant.

Capacidade estática/qualificação offline + preparação factual + snapshot de
proteção estão demonstrados. Processo futuro do utilitário, token original,
identidade de runtime, confirmação administrativa e capacidade real de completar
UPDATE NÃO foram demonstrados por execução nesta tarefa. São gates legítimos
tardios na própria cerimônia; não inventar PID/token nem executar para testá-los.

U1–U7 e especialização U2 originais conferidos integralmente e byte-exatos;
decisões accepted preservadas. Nenhuma especialização nova necessária: a
autorização de preparação não é importada para UPDATE. Executor estrito e
payload DATA já implementados/qualificados; não foi preciso alterar owner.

## Procedimento corrigido e qualificação

Novo wrapper candidato usa as mesmas definições read-only qualificadas e uma
chamada futura ao executor PROTEGIDO, jamais server/DATA do workspace. Cadeias,
executor/DATA/DLLs retidos sem share WRITE/DELETE; parent de criação share WRITE
somente para a criação relativa governada. Original antigo é conferido read-only
e fechado ANTES da invocação: manter lock do wrapper impediria DELETE necessário
ao rename. Native owner readquire antigo com ID/hash/policy e originalidade,
com direitos mínimos, antes do ato e dos efeitos. Não permitir share DELETE
amplo para contornar conflito nem afirmar continuidade através do intervalo.

Parsing completo e helpers read-only Windows PowerShell5.1x64:17 verificações
PASS; loader exercitado SOMENTE com executável de teste owner-stub/20 checks.
Nenhum produto executado. Testes estáticos de binding e hashes registrados em
WRAPPER-STATIC-CHECKS.json. Nenhuma mudança de C/target/censo/binário; campanhas
inalteradas não repetidas. Reutilizadas evidências anteriores dos quatro builds
zero warnings/64 pares afetados, incluindo UPDATE102 cenários/602 verificações
e entry20 por configuração. Não apresentar isso como nova campanha nativa.

K_UPDATE final Selected=2027314 bytes, quota3145728; Legacy1858746/quota2097152
não selecionado. São composição/censo, não teto de processo/host. R=novo EXE
arredondado pela unidade real; exemplo4096 dá761856 bytes, nunca substituir
geometria fresca por exemplo. Física/commit>=K, espaço e referências/gates antes
de cada efeito conforme U4. Custos opacos/latência/OS_QUIESCENCE UNKNOWN, sem
inventar limites124/128/1024 ou transportar B3/R3/G3/bootstrap de preparação.

## Pedido e primeiro gate

[Pedido byte-exato corrigido](ONE-CORRECTED-UPDATE-REQUEST.CANDIDATE.md), SHA256
557ad499e1435b2bbcde68663462aa3d9581f56b46cefef40f64da8dc5fe744e.
Procedimento candidato SHA25660834ab0542b72bec33caafae5b5a9bfdc7dc1fad21f05d0d2e06ebdea04118c.
READY_FOR_OWNER_SELECTION_AND_OPERATIONAL_AUTHORIZATION, NÃO AUTORIZADO.
Nenhum comando de UPDATE emitido/executado. Autorizações anteriores utilizadas.

Primeiro gate restante: seleção conjunta dos artefatos e autorização específica
de UMA execução manual desse pedido, com riscos U4 e intervalo entre renames
expressos. Depois registrar autorização/preparar comando, jamais autoexecutar.
Novo ID/candidate/previous nonce são adquiridos na cerimônia, não preditos.

Efeitos futuros delimitados: uma criação exclusiva protegida, cópia/hash/flush,
rename sem overwrite do antigo para retenção, rename do candidato ao canônico,
readback e close. Sem rollback, exclusão, limpeza, Start/Stop, endpoint ou SCM
mutante. Preservar utilitário, DLLs, configuração SCM e ProgramData.
Caminho: autorização UPDATE→execução manual/reconciliação→pedido independente
endpoint/primeiro Start diagnóstico. E21 PENDING; ED11/Phase6 PARTIAL; produção
DENY. Quiescência OS UNKNOWN, reservas/pins/históricos/EXE antigo/PST0.6.4 mantidos.
