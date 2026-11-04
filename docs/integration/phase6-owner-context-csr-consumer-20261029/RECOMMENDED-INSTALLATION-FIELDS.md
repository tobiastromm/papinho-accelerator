# Campos recomendados — candidatos, não designados

Estado: RECOMMENDED_CANDIDATES. O proprietário informou que nenhum dos três campos foi formalmente escolhido. Nenhuma seleção, instalação, conta, CA ou autorização é registrada por esta recomendação.

| Campo | Recomendação concreta | Justificativa |
|---|---|---|
| Parent absoluto | `C:\ProgramData\PapinhoAccelerator` | Namespace do produto já existente no equipamento; usar como parent de um filho novo, não adotar o diretório existente como root publicada. |
| Contêiner novo | `Personal` | Resulta em `C:\ProgramData\PapinhoAccelerator\Personal`, separado de `Protected\N0`. Nome relativo ASCII compatível com os limites existentes. Não exige novo owner/root paralelo: é a localização candidata da única root pessoal. |
| Conta operacional | `.\PapaccRuntime`, usuário local padrão dedicado | Segue P3, distinta do administrador que inicializa a CA. Nome proposto não significa conta existente, SID resolvido, perfil qualificado ou permissão para criá-la. |

A recomendação de parent/leaf é uma escolha técnica de configuração sob F0–F9/P3, não um default normativo já aceito. A CLI já recebe `--parent` e `--container`; nenhuma nova arquitetura de diretórios é necessária. Neste equipamento a known folder CommonApplicationData resolve para `C:\ProgramData`; Windows distingue essa pasta comum de dados de Program Files ([Microsoft KNOWNFOLDERID](https://learn.microsoft.com/en-us/windows/win32/shell/knownfolderid)). O local recomendado é para estado/configuração/history/referências da root; a chave privada MACHINE permanece sob o KSP Windows, sem exportá-la para a pasta.

## Evidência disponível e o que falta

Consulta somente de metadados: `C:\ProgramData` e `C:\ProgramData\PapinhoAccelerator` existem como diretórios e não foram marcados reparse pelo Get-Item. O nome `...\Personal` retornou ItemNotFoundException no contexto observado. Isso não é reserva, prova de ausência durável, identidade protegida, DACL qualificada ou direito de criação. Volume/FileID/descritor por handle original e elegibilidade precisam ser observados sob o contexto administrativo legítimo depois da seleção. Não se alterou ACL nem se listaram conteúdos protegidos.

O histórico existente `Protected\N0` não é root operacional C8. Ele e seus objetos/efeitos não devem ser movidos, limpos, adotados ou usados para autorizar a nova instalação. A root pessoal continua sendo exclusivamente o novo contêiner escolhido e criado futuramente pelo procedimento governado, nunca o diretório preexistente.

## Alternativas realmente compatíveis

1. Outro **novo nome relativo** sob o mesmo parent qualificado: mesma arquitetura e gates; precisa constar no plano exato.
2. Outro parent local absoluto existente e qualificado, com criação exclusiva do filho. Exemplo candidato adicional: parent `C:\ProgramData`, leaf `PapinhoAccelerator-Personal`. Não há fallback automático se o parent preferido falhar.
3. Outra conta local padrão dedicada, nova ou existente legitimamente designável para esta instalação, após comprovar propósito/owner/SID/token e separação. Não adotar conta compartilhada ou material antigo como atalho.

P0/P3/P4 de [preinitialization accepted](../../adr/DECISION-personal-ca-preinitialization-20261022.md) exigem MACHINE KSP para a CA e identidade operacional distinta. Nome/SID informado não basta. [NCryptCreatePersistedKey](https://learn.microsoft.com/en-us/windows/win32/api/ncrypt/nf-ncrypt-ncryptcreatepersistedkey) distingue a flag MACHINE do escopo de usuário; o fixture CurrentUser não qualifica o perfil MACHINE.

Windows permite configurar diferentes contas de logon para serviços, mas isso não escolhe o perfil do produto ([Microsoft Service User Accounts](https://learn.microsoft.com/en-us/windows/win32/services/service-user-accounts)). Não recomendo SYSTEM, Administrators ou a conta interativa do proprietário como identidade implícita. LocalService, NetworkService e virtual accounts não são substitutos automaticamente aceitos por P3. A regra R0–R9 é explicitamente do LAB fixo e não cobre produção ou outra instância; `NT SERVICE\PapinhoAcceleratorLab20261023` não é uma alternativa aprovada para a primeira instalação pessoal. Um perfil SCM diferente exige análise da norma aplicável e qualificação específica, sem criar serviço para obter SID.

Para eventual hosting SCM, a conta escolhida precisará dos direitos de logon e acesso efetivos pertinentes, observados no processo real; nada disso será concedido nesta tarefa. Não é solicitado segredo ou senha. A CA continua administrativa; o runtime deverá ter negativa factual de uso privado da CA, sem perder seus direitos legítimos sobre sua própria credencial TLS.

## O que depende do proprietário

Escolher o par parent/leaf e a conta operacional proposta (ou alternativa permitida). Essa seleção é distinta da futura autorização para criar conta, configurar serviço/permissões, executar NEW_INSTALL/CA_INITIALIZE ou publicar CURRENT. Até seleção e qualificação: bindings UNKNOWN, execução DENY. Não é necessária nova família de ADRs apenas para informar valores previstos pelos contratos atuais.
