# F0–F9: revisão exata e gate de implementabilidade

Task PACC-PHASE6-FIRST-INSTALL-F0-F9-ACCEPTANCE-1; consulta documental 2026-10-22.
main / HEAD 913c90cf2328f7939d8f5b927640601903ac6e91.
Resultado: PROPOSED / STOP antes de registro accepted. Nenhum patch funcional ou ADR accepted alterado.

## Requisitos conformes no nível normativo

F0/F1 separam a origem legítima externa da instalação candidata, de CURRENT e de privilégios técnicos; não exigem predecessor para bootstrap inicial. F3 separa parent/nome relativo do objeto-filho ainda inexistente. F4 define validade por invocação/eventos, com negação se uma exigência temporal externa não puder ser satisfeita. F5 exige consumo indivisível antes da submissão, sem rearmar em falha. F6 preserva Stop, retenção, quiescência, UNKNOWN e ausência de autoridade restaurada no restart. F7 mantém CA inicial/assinatura/publicação separadas. F8 preserva adapters e quotas, sem Win32/SID/CNG no núcleo. Isso não é aceitação parcial nem prova de fonte factual.

## Lacuna material exata

F2 exige:

> A fonte deve autenticar sua decisão e sua associação ao pedido completo por mecanismos já aprovados para aquela plataforma.

O pedido completo inclui nonce32/invocação, parent nativo/nome relativo e configuração exata. F5 repete:

> A fonte deve vincular sua decisão ao contexto completo; um ID ou nonce esperado pelo caller não prova esse vínculo.

O contrato não distingue uma fonte que emite uma decisão já ligada a todos esses campos de uma fonte administrativa existente que apenas prova competência/escopo, para a qual o adapter teria de construir uma associação local protegida. Considerar a segunda forma automaticamente equivalente ampliaria o significado das cláusulas durante a implementação. A correção focal adjacente propõe essa distinção, sem permitir que o adapter fabrique competência ou amplie o escopo externo.

Não se demonstra impossibilidade universal de F2, nem se afirma que toda plataforma precise desse segundo perfil. O resultado é mais estreito: os mecanismos locais encontrados não fornecem essa decisão completa e a documentação oficial da classe Windows candidata não comprova os vínculos adicionais. Assim, a condição conjunta de aceitação e fonte implementável ainda não foi demonstrada; F0–F9 permanecem PROPOSED.

## Fontes Windows: candidato não é implementação nem qualificação

Classe candidata concreta: processo institucional preexistente de aprovação de implantação para usuário/dispositivo/aplicação. Configuration Manager é exemplo documental da classe, NÃO fornecedor selecionado, dependência aceita ou instalação observada. A Microsoft descreve integração com sistema de aprovação da organização e CreateApprovedRequest com ClientGUID, Username e ApplicationID; ApplicationID identifica aplicação sem versão. Isso sustenta a existência da classe, mas não prova autorização para configuração exata/parent/nonce NEW_INSTALL. Não executar esse método nem permitir sua instalação automática como parte desta revisão.

Referência oficial consultada: [Application approval process](https://learn.microsoft.com/en-us/intune/configmgr/develop/apps/application-approval-process), seções Scenario 1/Scenario 2 e parâmetros requeridos. Documenta aprovação por usuário/dispositivo e integração com processo externo; não prova legitimidade de uma instituição ou instalação específica.

Mecanismos já no Accelerator: bootstrap_admin e bootstrap_auth_native oferecem token/role/identidade e correlação com custódia; v3act_native observa elevação/contexto mas pertence ao UPDATE e não pode autorizar NEW_INSTALL; administrative->authorize no issuer consome autorização, não produz sua origem. PAPACC_CA_KEY protege chave/borrows/retidos e não emite aprovação de instalação. Nenhum desses mecanismos é convertido em fonte F2.

[Access tokens](https://learn.microsoft.com/en-us/windows/win32/secauthz/access-tokens) documenta contexto de identidade/privilégios. [Authorization Functions](https://learn.microsoft.com/en-us/windows/win32/secauthz/authorization-functions) descreve AccessCheck como avaliação de direitos por security descriptor/token. Essas capacidades não estabelecem, isoladamente, legitimidade institucional ou decisão NEW_INSTALL. Não propor criar grupo, ACL, serviço ou registry para suprir a origem ausente.

SOURCE_CANDIDATE=EXISTING_INSTITUTIONAL_WINDOWS_DEPLOYMENT_APPROVAL.
SOURCE_IMPLEMENTED=NO (nenhum adapter F2 desse tipo identificado).
SOURCE_QUALIFIED=NO; fonte real não fornecida/observada.
SOURCE_IMPLEMENTABILITY=CONDITIONAL_UNPROVEN_FOR_EXACT_F2_F5; há componentes concretos para um adapter, mas não prova completa de semântica, origem, revalidação, lifetime ou recursos. A classe não foi aceita automaticamente.

## ED11 e efeitos

A revisão não mede memória nem autoriza reserva. Um futuro adapter precisa limitar evidência/snapshots, nonce/ato, objetos WMI/COM ou outro acesso administrativo realmente escolhido, autenticação/CRT, buffers, native tokens/parent/worker, stack aninhada, retenção e coexistência. Não trazer um cliente de gerenciamento completo para dentro do processo por inferência. Custos UNKNOWN não são zero; 3MiB exige seleção explícita e nenhuma quota aumenta. ED11 PARTIAL.

Somente navegação de documentação oficial e inspeção local read-only foram realizadas. Nenhuma rede operacional, consulta a servidor administrativo, build, teste, bootstrap, chave, certificado ou CURRENT. As aprovações e testes anteriores permanecem no seu escopo histórico. Não há operação de registro accepted a submeter ao executor enquanto esta lacuna material estiver aberta.

## Próxima revisão mínima

Revisar a correção F2/F5 adjacente e demonstrar, para uma fonte real candidata, qual perfil pode ser realizado sem nova autoridade persistente. Manter F0–F9 originais intactos. Somente após coerência e implementabilidade comprovadas, submeter o texto exato de aceitação ao executor. Nenhuma consulta/publicação de autorização real está autorizada por esta continuação documental.
