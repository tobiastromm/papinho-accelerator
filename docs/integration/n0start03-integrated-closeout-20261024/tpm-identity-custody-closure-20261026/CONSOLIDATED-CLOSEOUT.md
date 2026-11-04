# ED11 — identidade TPM, custódia e integração durável: relatório consolidado

TASK=PACC-PHASE6-ED11-TPM-IDENTITY-CUSTODY-CLOSURE

A qualificação isolada avançou; a identidade real, custódia operacional e persistência integrada ainda não foram demonstradas. Não há pedido de provisionamento executável nem aprovação criada pelo agente. CURRENT_TASK_COMPLETED=NO para o objetivo integral; o escopo de evidência novo está qualificado com os limites abaixo. E21=PENDING, ED11/Phase6=PARTIAL, autoridades produtivas=DENY.

## Estado normativo e evidência preservada

W0–W7 e L0–L7 já estão ACCEPTED, em registros separados; conferidos novamente os hashes literais das propostas e decisões. Não foram editados. Mantidos guard histórico, pins operacionais, recibo EK, fontes operacionais, configurações, inputs e reservas UNKNOWN. HEAD main f341985924d29870d8ff627ed37c8a0874ad6469 preservado. Nenhuma consulta EK repetida nem nova chamada TPM foi executada.

A consulta pública anterior permanece PASS_AT_OBSERVATION. A confiança do AdditionalCertificates, a posse da chave e a continuidade da geração do índice continuam separadamente não demonstradas. Não inferimos cadeia ou SPKI a partir de um hash, nem tratamos fabricante/firmware/Name/boot como autenticação de hardware. Não foi inventado Retained=False para a consulta do cmdlet; fechamento interno segue UNOBSERVED. [Revisão de identidade e fontes primárias](IDENTITY-AND-POSSESSION-REVIEW.md).

## Implementações e correção realizadas

- Fixture criptográfico de credencial RSA/SHA256: construção pública e abertura com chave de software efêmera, label OAEP IDENTITY com terminador, KDFa, CNG AES128-CFB128 e integridade ligada ao Name. Sem chave persistente, assinatura, CA, TBS ou acesso ao TPM. Não é um adapter nativo ActivateCredential nem prova de posse real.
- Parser bounded de parâmetros TPM2B de resposta e proteção de uso único/contexto no fixture; não substitui autenticação da resposta/sessão TPM.
- Codec candidato372 bytes e máquina de estados para PREPARED/CONSUMED/RESULT/UNKNOWN. Publicação RESULT é somente formato candidato; worker durável não implementado. Preserva primeira falha, lease PID/criação, freeze/Stop e nenhuma repetição após consumo ambíguo.
- Revisão encontrou a necessidade de revalidar Stop após callback de preparação e antes do incremento; corrigido o modelo. Stop durante commit também bloqueia leitura. Primeira falha agora é preservada no registro serializado do modelo.
- Helper de análise pública offline: comparação de chave/certificado/SPKI e cadeia com bundle explícito e stores padrão desabilitados. Resultado sempre DATA/DENY; âncoras não designadas e revogação offline UNKNOWN. Não foi processada cadeia EK real; testes positivos desse helper usam outputs simulados, com rejeição nativa de DER inválido.
- Novo coletor candidato de bytes públicos exatos, sem execução: obtém os bytes ausentes, não apenas os metadados anteriores. Restringe a chave e certificado aos fingerprints já observados; conteúdo público fica local. AST PowerShell5.1 válido, sem alegação de ensaio nativo do script.

## Custódia e integração

[Modelo concreto e schema proposto](SCHEMA-AND-CUSTODY.PROPOSED.md) separa definição pela cerimônia autorizada do proprietário, incremento pelo owner canônico com sessão/lease, leitura pública e leitura autenticada do contador. Propõe DPAPI CurrentUser no mesmo participante root/RAW1, sem cofre ou autoridade adicional. Explicita acesso de software no mesmo usuário, administrador, perda de perfil/TPM, lockout e retenção. O SID observado não foi convertido em designação de custódia. Nenhum segredo operacional foi criado, extraído, exportado ou registrado.

A API funcional DATA de88 bytes já existente permanece. A integração de persistência root/CAS/worker não está concluída. L1 exige revisão exata do componente; não foram reutilizados bytes ROOT8, eventos issuer, CURRENT ou epochs. O novo schema/protocolo é isolado e proposto, não uma extensão aceita/publicada. Callbacks simulados e o lock Python não provam armazenamento durável ou exclusividade entre processos.372 bytes não é custo total de memória e não concede crédito ED11.

O adapter anterior de serialização NV_Increment/DefineSpace e as provas DPAPI/CNG válidas foram reutilizados sem reconstrução inalterada. Continua sem transporte nativo de escrita habilitado, estabelecimento de sessão salgada real ou custódia real. Nenhum NV_DefineSpace/NV_Increment/NV_UndefineSpace/Clear foi enviado.

## Qualificação nova

| Evidência | Resultado e alcance |
|---|---|
| C# x64 .NET Framework, Debug/Release /warn:4 /warnaserror+ | 2 builds PASS; zero warnings |
| Credencial software/CNG | 364 verificações por build,728 total; mutações em seed cifrado/cipher/MAC/Name, chave errada, label errado, vetores KDF independentes |
| Python | 38 métodos PASS; 160 combinações de falhas prepare/increment/commit/contexto |
| Codec | todas372 posições adulteradas e372 truncamentos rejeitados |
| Concorrência | 32 threads na mesma invocação, uso único; reentrância BUSY; não prova exclusão nativa entre processos |
| Stop/falhas | antes/durante prepare/commit, resposta ambígua, rollback local, identidade divergente, primeira falha serializada, close UNKNOWN sem retry/double-close |
| Restauração conjunta | teste demonstra o limite: restaurar também a testemunha simulada remove a distinção; não é prova de rollback físico |
| Coletor | AST_PARSE=PASS; EXECUTED=NO |
| Produto | não modificado nesta tarefa; builds/regressões previamente válidos reutilizados, sem campanha redundante |

Fechamento de objetos CNG AES controlados é verificado pelo fixture reutilizado. RSA/handles gerenciados: Dispose retornou; não afirmamos observabilidade de todos os recursos internos. Vetores são sintéticos; não há medição de memória/handles do TPM real, persistência após reboot ou tolerância factual a queda de energia. Nenhum pico futuro foi inventado.

## Hashes finais

- CredentialActivationFixture.cs: `4cfedb27ab96e204b3800ef0d20114cbb19123528ccae0318442497d04983ada`
- CredentialDebug.exe: `1eacdc2687fd33ad4d90b29c209ba9ee4430fb040689dd27d0ab7905b9b56675`
- CredentialRelease.exe: `3ab8df21364536750bc95b6422b2b547cd037081582f43af40a155cf5f20218b`
- isolated_protocol.py: `9faeeaa93fa9eaf87185f41d7aa57259c581c10b3cef348a659d828a6ed74205`
- offline_public_evidence.py: `3ce4f8041623633171953a1741ddd48ec374ab485c8cd42ec7160359041003da`
- collect-ek-public-bytes.NOT-EXECUTED.ps1: `f11bc94bc903623a865e7731b48b62c1a682e8a5e8d8f9d62de318edad54aacd`
- SCHEMA-AND-CUSTODY.PROPOSED.md: `270d31d12e8342e09847196fb41d3e7e23d4cb5d9c2ac17956fdded44e79e9b9`
- PUBLIC-BYTES-OBSERVATION.CANDIDATE.json: `13615d3fa8f7b41abdbc79699d5efabb8288656d72a99cb60e92d5ac3b26cbcb`

Demais hashes e logs: [FINAL-ARTIFACT-HASHES.json](FINAL-ARTIFACT-HASHES.json), [CREDENTIAL-BUILD-RESULTS.json](CREDENTIAL-BUILD-RESULTS.json), [PROTOCOL-TESTS.log](PROTOCOL-TESTS.log), [AUTHORITY-VERIFICATION.json](AUTHORITY-VERIFICATION.json). Hashes são identificação de bytes, não identidade protegida ou autorização.

## Primeira evidência ausente e caminho mínimo

1. Obter os bytes públicos exatos EK/certificado e seu perfil para determinar issuer, correspondência e âncoras aplicáveis. O recibo anterior só contém fingerprints. Preparado [pedido público candidato](PUBLIC-BYTES-OBSERVATION.CANDIDATE.json) e coletor; não executados, sem autorização de provisionamento. Uma nova coleta só se justifica por esses dados discriminantes, não para reconfirmar os mesmos metadados. Sem DER não há como determinar legitimamente a cadeia Intel deste equipamento.
2. Designar/verificar a base de confiança, custódia e geração reais; qualificar uma prova de posse com desafio imprevisível e EK/AK efetivos. Se exigir objetos/sessões transitórios no TPM, solicitar aprovação exata separada. [Desenho mínimo](NATIVE-POSSESSION-OBSERVATION.DESIGN.json) lista os bindings faltantes; não inventa perfil, AK Name ou autorização nem contém comando executável. Não instalar âncoras ou ativar uma nova confiança por inferência.
3. Revisar o componente durável exato sob L1 e ligar ao mesmo worker/CAS com identidade nativa original e sessão autenticada. O codec/modelo fornecido deixa essa revisão concreta, mas não concede autoridade para editar o formato aceito.
4. Somente após essas condições, finalizar pedido de provisionamento: candidato0x01804143, atributos0x00040014,8 bytes, uma definição/até dois incrementos; ausência histórica não é reserva. [Estado candidato](PROVISIONING-READINESS.NOT-EXECUTABLE.json) mantém todos os bloqueios, nenhum approval ou comando. Prova nativa de posse não equivale à persistência do contador; ambas precisam evidência própria.
5. Após experimento nativo válido e persistência demonstrada, preparar nova leitura histórica Q4 e disposição documental com autorizações independentes. Nenhuma aprovação consumida será reutilizada.

Não existe, hoje, apenas o gate de escrever no TPM: faltam dados públicos e designações/provas de custódia, identidade e componente persistente. Apresentamos essas lacunas concretas, em vez de emitir pedido incompleto como executável. E17/E18 e E19/E20 continuam frentes independentes; E22 derivado permanece pendente. Nenhuma leitura Q4, Start/Stop, mutação SCM/ACL, quota, limpeza, refund, CURRENT, assinatura produtiva ou recuperação autoritativa ocorreu.
