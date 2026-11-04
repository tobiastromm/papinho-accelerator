# Revisão focal — layout pessoal e identidade SCM

Estado: RECOMMENDED, não selecionado operacionalmente. Branch `main`, HEAD `f341985924d29870d8ff627ed37c8a0874ad6469`; WIP anterior preservado. Nenhum código funcional ou ADR accepted foi alterado. Phase 6/ED11 PARTIAL; autoridade produtiva DENY.

## Recomendação concreta

| Campo | Proposta final para decisão | Condição |
|---|---|---|
| Binários | `%ProgramFiles%\Papinho\Accelerator` | Known folder do target x64; separada dos dados mutáveis e da root. Não é diretório da chave CNG. |
| Parent dos dados | `%ProgramData%\Papinho` | Deve existir e ser legitimamente qualificado por handle original antes do fluxo atual. Não há criação recursiva implícita. |
| Novo contêiner | `Accelerator` | Criação exclusiva sob o parent original; contêiner existente, mesmo vazio, não é NEW_INSTALL. |
| Dados/root pessoal | `%ProgramData%\Papinho\Accelerator` | Candidato à única root da instalação, não root publicada por existência de pasta. Configuração/estado/história usam formatos e owners existentes. |
| Perfil | `PERSONAL_LOCAL`, lógico | Não é necessária subpasta `Personal`; ela era apenas uma recomendação anterior, nunca decisão do proprietário. |
| Logon do serviço | `NT SERVICE\PapinhoAccelerator` | Recomendado, condicionado à especialização normativa focal e futura qualificação factual. |
| Chave privada CA | Microsoft Software KSP, MACHINE, não exportável | Administrador autorizado distinto do runtime; runtime explicitamente sem uso/open/export da CA. Não copiar chave para dados ou binários. |
| Logs | Logger/sink já governado | Nenhum novo diretório/sidecar/ledger ou formato de log é instituído. Se um destino em disco for configurado, deve ficar na área de dados sob política existente, sem conteúdo secreto. |

Neste equipamento as known folders resolvem para `C:\Program Files` e `C:\ProgramData`. Consulta Get-Item somente de metadados retornou ItemNotFoundException separadamente para `C:\Program Files\Papinho\Accelerator`, `C:\ProgramData\Papinho` e `C:\ProgramData\Papinho\Accelerator`. Isso é observação no contexto atual, não prova autoritativa de ausência/reserva, identidade nativa, direitos ou proteção. A primeira lacuna factual do layout é o parent proposto: não foi adquirido e qualificado; este trabalho não o cria. Sua eventual preparação precisa integrar o procedimento autorizado de instalação, antes da abertura do parent pelo owner atual, sem adotar dados existentes.

Resolver known folders e congelar o caminho absoluto antes da apresentação; não passar `%ProgramData%` não expandido à entrada nativa, nem trocar o alvo depois da confirmação. O adaptador atual exige path local absoluto ASCII, abaixo de MAX_PATH, sem reparse e com direitos e identidade revalidados; known folder fora desse domínio deve negar, sem fallback silencioso. Leaf `Accelerator` tem onze bytes ASCII e atende à gramática/limite de 64 bytes do plano. Não há alteração de formato persistido.

## Rastreabilidade normativa e factual

| Fonte | Resultado focal |
|---|---|
| M0–M11 accepted, especialmente M5 | SCM hospeda o mesmo runtime/owner; conta padrão dedicada selecionada e token observado; sem default privilegiado. Nenhuma cláusula exige pasta física Personal. |
| P0/P3/P4 da preinitialization accepted | MACHINE CA; a interpretação vigente qualifica conta SAM local para console. Hosting SCM é escolha separadamente revisada. Isolamento real da chave continua obrigatório. |
| F0–F9 e bootstrap P3/P5/P6/P7 | Parent original protegido, novo leaf exclusivo, plano imutável confirmado; sem CURRENT anterior exigido na primeira instalação, sem CURRENT simulado como autoridade; logger e owner existentes. |
| C8/CA_INITIALIZE existentes | Mesmo owner, identidade original e publicação governada. Nome de pasta não concede C8; presença da entrada não concede execução. |
| R0–R9 accepted | Exceção restrita ao LAB histórico; não qualifica `PapinhoAccelerator` pessoal por herança. |
| Código personal_native/effect_plan | Parent e leaf são parâmetros; não há necessidade de acrescentar `Personal`. Parent aberto com direitos/descritor/identidade e revalidação; criação de ancestrais não é implícita. |
| server_ca_key_win32.c | SID de runtime válido e distinto do administrador; identidade administrativa por TokenUser; criação MACHINE sem overwrite, export policy zero, signing-only; DACL protegida nega runtime e permite administrador/SYSTEM. Validação estrutural não prova acesso efetivo nem mínimo privilégio. |

Há uma fronteira normativa concreta, não uma incompatibilidade técnica demonstrada do Windows: a seleção virtual pessoal SCM ainda não é qualificada pelo P3 console nem pela exceção LAB. [Candidato focal](../../adr/ADR-CANDIDATE-personal-scm-virtual-account-20261029.md) especializa apenas essa seleção e o caso de duplicação ausente do SID primário, sob condições integrais. Não modificar o predicado accepted por esta revisão. A alternativa `.\PapaccRuntime` continua candidata compatível com o caminho SAM vigente; não foi criada ou selecionada.

## Conta virtual, TokenUser e custódia

O Windows oferece virtual accounts gerenciadas no formato `NT SERVICE\<nome-do-serviço>`, sem usuário local com senha administrada pelo produto ([Microsoft](https://learn.microsoft.com/windows/security/identity-protection/access-control/service-accounts)). Configurar esse nome como logon é diferente de executar outra conta e acrescentar service SID a TokenGroups. Exigir TokenUser factual e consulta completa dos grupos, atributos ENABLED/ENABLED_BY_DEFAULT/DENY_ONLY, restrições e PID/criação; não aceitar apenas LookupAccountName, SID derivado ou nome SCM. Não há serviço pessoal/token virtual criado ou observado nesta revisão.

`NCRYPT_MACHINE_KEY_FLAG` escolhe o escopo do computador, não a autorização de todos os processos; sem overwrite uma chave existente não deve ser substituída ([Microsoft NCryptCreatePersistedKey](https://learn.microsoft.com/en-us/windows/win32/api/ncrypt/nf-ncrypt-ncryptcreatepersistedkey)). A cerimônia administrativa cria/protege CA; o serviço não cria CA durante Start. A política existente deve negar o SID primário do runtime mesmo que não haja duplicação em grupos. Continuam pendentes prova real de acesso, revisão de mínimo privilégio do administrador e continuidade/reabertura no contexto elegível. CurrentUser fixture não prova MACHINE.

## Qualificação e limites

Reutilizados quatro builds Selected/Legacy Debug/Release e 80 CTests do [pacote anterior](../phase6-owner-context-csr-consumer-20261029/QUALIFICATION.json), com hashes preservados no manifesto desta revisão. Nenhuma alteração funcional justifica repetir essa campanha. Revisão estática do caminho MACHINE confirma flags/export policy e isolamento estrutural existentes; não houve nova criação ou abertura de chave. A qualificação offline não demonstra acesso efetivo sob token virtual.

Testes requeridos após aceitação focal: identidade primária correta com grupos completos, SID ausente nos grupos, grupo desabilitado/deny-only, token restrito, SID/PID/criação divergentes, falha de consulta, alias de papéis, Stop/concorrência/fechamento UNKNOWN; preservação do caminho SAM e negação sem autoridade. Posteriormente, em operação especificamente autorizada, comprovar SCM/logon/service SID type originais, acesso CNG administrativo e negação ao runtime, persistência/reabertura, recursos e lifecycle. Nenhuma dessas provas futuras é substituída por contagem de testes antigos.

## Compatibilidade e próximo passo

Não mover `C:\ProgramData\PapinhoAccelerator\Protected\N0`, adotar históricos, migrar instalação existente ou abrir a chave fixture retida. Aquisição de instalação existente mantém identidade/path originais; novo layout é proposta para primeira instalação apenas. Não modificar CURRENT/epoch, serial/high-water, formatos ou políticas para acomodar o caminho.

A decisão necessária agora é aceitar ou não a especialização focal da conta virtual. Os caminhos são parâmetros propostos, não mudança de formato nem seleção operacional implícita. Depois, integrar/qualificar apenas offline o perfil autorizado; preparar separadamente a aquisição do parent e a instalação real. E17/E18, E19/E20, E21 e E22 não são promovidos por esta revisão. Enrollment/MANUAL/E8 e composição quantitativa continuam independentes; TPM/Q4 histórico não vira requisito da primeira instalação.

[Próximo prompt](NEXT-VIRTUAL-ACCOUNT-OFFLINE-INTEGRATION.md). Hashes literais: ARTIFACT-HASHES.json no mesmo pacote.

## Resultado estrutural novo

check-in-memory-sid-acl.ps1 passou cinco verificações com o parser Windows real de SID/descritor em memória: principais sintéticos distintos, DACL protegida, deny runtime antes dos allows, allow administrativo e três ACEs. Saída: IN-MEMORY-QUALIFICATION.json. SIDs sintéticos não resolvidos, distintos do LAB; zero chamadas CNG/SCM e zero mutações ACL. Não é AccessCheck, token virtual real, persistência MACHINE ou prova de privilégio mínimo. Qualifica somente a estrutura offline do modelo.
