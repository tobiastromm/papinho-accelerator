# Proposta focal — duas representações exatas do readback GA da fixture

PROPOSED / NOT_ACCEPTED / NO_OPERATIONAL_AUTHORITY. 2026-11-03.

## Evidência e limite

Consulta manual70bc5bae6e9fc95dddfb9cebd2d0491910b5a0f557acb4dfa137738d685ccbf6 concluída/consumida/fechada, sem retry. OpenProvider/OpenKey/GetProperty status0, descriptor152bytes/flags5. Owner operador/protectedTrue/três ACEs sem flags ou callback: DENY ao runtime exato, ALLOW ao operador e SYSTEM, todas com máscara0xd01f01ff. A projeção local rejeitou EXACT_MASK_ORDER_OR_SID; como tipos/ordem/SIDs estão corretos, a divergência factual atual é a máscara literal frente a0x10000000 exigida por ca_acl. Não houve erro de acesso nativo nessa consulta atual.

Código/modeI reproduz rejeição local CA_D_ACE_MASK/NTE_PERM com leitura nativa bem-sucedida e esse template exato de máscaras. Não prova retrospectivamente que o getter da criação anterior teve sucesso: processo/token/handle novos e ausência de publicSHA/referência original preservam UNKNOWN histórico. Consulta atual não é controle positivo da MESMA chave originalmente retida nem qualificação/adoção da fixture parcial.

0xd01f01ff contém GENERIC_READ0x80000000, GENERIC_WRITE0x40000000, GENERIC_ALL0x10000000, bits padrão0x001f0000 e bits específicos0x1ff; não contém ACCESS_SYSTEM_SECURITY/MAXIMUM_ALLOWED/bits reservados. É decomposição numérica, NÃO prova de permissões operacionais da chave. A igualdade numérica dos bits baixos com FILE_ALL_ACCESS não autoriza usar mapeamento de arquivos em NCrypt. Nas fontes primárias consultadas não foi estabelecido um GENERIC_MAPPING específico do Software KSP suficiente para demonstrar equivalência universal dessas duas máscaras.

Fontes: [Generic Access Rights](https://learn.microsoft.com/en-us/windows/win32/secauthz/generic-access-rights), [ACCESS_MASK](https://learn.microsoft.com/en-us/windows/win32/secauthz/access-mask), [Key Storage Property Identifiers](https://learn.microsoft.com/en-us/windows/win32/seccng/key-storage-property-identifiers). Elas definem bits/propriedade e dependência do tipo de objeto, não afirmam que o provider sempre retorna a forma observada ou que os bits específicos tenham semântica de arquivo.

## Por que decisão focal

K1 accepted exige congelar e qualificar o projeto exato do SD/owner/direitos antes de operação, sem escolhê-lo em execução para obter PASS. T3 accepted congelou DENY GA ao SID virtual e ALLOW GA ao operador/SY conforme a primitiva qualificada; a implementação estrita aceita só a máscara literal GA. Não há especialização accepted sobre essa segunda forma de readback. Aceitá-la por inferência de mapeamento de arquivos ou aceitar qualquer superset de GA seria alteração não autorizada. Esta proposta explicita somente a segunda forma exata, sem mudar trustee/owner, DACL efetiva existente ou autoridades.

Alternativa A — conservar igualdade literal0x10000000 e DENY na forma observada. Não repetir consulta/criação para tentar sorte, não mudar ACL da chave parcial.

Alternativa B — recomendação limitada: aprovar S0–S5 abaixo como política explícita de representação com risco residual, sem alegar equivalência operacional universal. Não é aceitação de máscara arbitrária ou de política mais permissiva por fallback.

## S0 — Escopo exclusivo

Somente validação de proteção da fixture MACHINE_TEST_ONLY RSA2048/export0/signing-only no Microsoft Software KSP, no owner existente sob K1/T0–T6/F0–F5/R0–R4, e a validação da MESMA fixture quando usada pela variante READ diagnóstica K0–K9 legitimamente admitida. Não ampliar CA/issuer real, READordinary, TRANSPORT_ONLY, NEW_INSTALL, outros providers/algoritmos/perfis ou objetos de filesystem/SCM/pipe.

Seleção continua pelos construtores fixture/READ diagnóstica já existentes, nunca por máscara, string, arquivo, argumento ou presença de GENERIC_ALL. READ mantém todos os gates de originais/source/token/imagem/seleção/consumo/atoP/autenticação bilateral, controle administrativo positivo da mesma identidade e recursos próprios. Esta política não abre KSP em startup genérico nem transforma runtime NTE_PERM em PASS sem as provas K2/K5.

## S1 — Template estrito e allowlist fechada

Manter owner igual ao operador original, DACL presente/protegida/válida, exatamente três ACEs explícitas sem flags/herança/callback, sem trustee adicional. Ordem/tipos/SIDs exatos: índice0 DENY ao runtime original; índice1 ALLOW ao operador; índice2 ALLOW a LocalSystem. Não usar grupo/BA como substituto de owner/SID. Ausência/divergência de qualquer requisito continua DENY.

Aceitar SOMENTE estes dois templates de máscaras, uniformes nas três ACEs:

| Forma | ACE0 runtime DENY | ACE1 operador ALLOW | ACE2 SYSTEM ALLOW |
| --- | --- | --- | --- |
| GA original | 0x10000000 | 0x10000000 | 0x10000000 |
| Readback observado | 0xd01f01ff | 0xd01f01ff | 0xd01f01ff |

Rejeitar mistura de formas na mesma DACL, máscara parcial ou sem GA, bit extra/faltante, GENERIC_EXECUTE separado, SACL/MAXIMUM_ALLOWED/reservado, outro tipo/ordem/SID/flags/count/proteção. Não basta `(mask & GENERIC_ALL)!=0`. Não usar MapGenericMask com FILE/registry mapping, ignorar/normalizar bits desconhecidos ou aceitar terceira forma automaticamente.

Validar os bytes retornados sem modificá-los, preservando identidade/proveniência/retention e controles de continuidade existentes. Digest ou diagnóstico de SD deve representar os bytes efetivamente lidos, não uma DACL reescrita para parecer GA. A segunda forma não concede autoridade por leitura nem prova semântica de operação privada.

## S2 — Escrita, efeitos e provas inalterados

O setter continua emitindo EXATAMENTE o template GA original, owner operador e DENY/ALLOW descritos antes da finalização, pelos mesmos flags/primitivas. Não passar a escrever0xd01f01ff, não editar ACL de chave já existente, não mudar flags/finalização/owner/trustees ou remover DENY. Proposta muda somente o predicado de readback dos objetos no escopo, não a proteção enviada ou os efeitos persistentes permitidos.

Criação exclusiva/CSPRNG/ausência de overwrite/adopção, confirmação atribuída/consumo único, referências originais retidas, provider/name/algorithm/length/export/usage, controle positivo público da MESMA chave, PublicSHA/continuidade/close, Stop/replay/deadline e UNKNOWN permanecem obrigatórios. MATCH de SD não substitui nenhuma dessas provas. Runtime ainda deve negar efetivamente a consulta privada no probe K5; sucesso inesperado é FAIL, não readiness.

## S3 — Integração offline e regressões

Aceitação autoriza registro normativo separado e integração/qualificação OFFLINE no mesmo ca_acl e seleção diagnóstica existente. Não modificar accepted/propostas anteriores ou generalizar helper a outros objetos. Vincular a especialização à política/plano original existente e à apresentação diagnóstica; não criar wire público, protocolo, serviço, owner, root, canal ou biblioteca distribuída nova.

Testar ambos os templates positivos com o owner real em modelo, misturas3×2/bit extra/faltante/GA ausente/máscara arbitrária e reserved/SACL/MAXIMUM_ALLOWED, owner/protected/count/flags/callback/tipo/ordem/SID divergentes, getter nativo falho, substituição/replay/Stop/closeUNKNOWN/retention. Testar criação e controle positivo da mesma chave e READ admitida com ambas as formas, sem relaxar autoridades/probe/receipt/close. Demonstrar que CA comum e outros perfis não recebem a nova aceitação ou permissão por máscara. Qualificar quatro configurações e somente regressões afetadas, preservando logs/censo/pins.

Não mudar qualquer quota ou parâmetro de recursos. Manter fixture entrada128/guarda188/cap192/físico7239172096/commit3GiB/privado-working16MiB/fresh100/global60000; READ/K7 e demais perfis com limites anteriores; ED11/censo/LegacyDENY inalterados. Nenhuma coleta real para implementar/qualificar.

## S4 — Risco residual explicitamente escolhido

O proprietário aceita, para este ensaio fechado, validar a segunda forma EXATA observada como template autorizado de readback, apesar de não haver demonstração pública de mapeamento operacional universal do Software KSP. Não é afirmação de que todos os providers/versões normalizam GA assim, de que0x1ff concede operações específicas, ou de que essa máscara garante o isolamento privado. Forma diferente continua DENY e exige evidência/decisão próprias.

Não conceder autoridade além dos mesmos três trustees/intenção de proteção; não ampliar o uso da chave, exportação, assinatura ou produção. A prova operacional continua nos gates/controle positivo/mesma identidade e probe restrito originais. Recursos/custos/latência/retention opacos e histórico/application-close/quiescênciaOS UNKNOWN preservados. Sem suficiência/prontidão universal.

## S5 — Gates posteriores sem adoção da chave parcial

Após integração qualificada, congelar candidato final e preparar UPDATE governado U1–U7/U2 se necessário, com hashes/efeitos/gates frescos para autorização específica. Não executar automaticamente nem instalar intermediário só para instrumentação.

Preservar a chave parcial nomeada dea8648758bebcbbcabde016bf911e77.b4f8ff6a41ba3b4d3e5ff403d0d017a7 e todas as decisões/cópias/EXEs anteriores; esta aceitação não autoriza recuperá-la/adotá-la, mudar ACL, apagar, repetir criação ou reutilizar decisões consumidas. Eventual nova preparação exige NOVA decisão operacional real, tentativa exclusiva e eventual cópia única em leaf novo pelo mecanismo F3/R4, explicitamente delimitadas em pedido próprio. Fixture/READ/E/Q/S/P/Start são operações independentes e não autorizadas por esta proposta.

Sem consulta/coleta, UPDATE, criação KSP/cópia real, assinatura/exportação privada, endpoint/SCQ/READ/StartStop, SCM/ACL existentes, CA/CURRENT/rootstore, rede, TPM/NV/Q4, cleanup/refund/rollback/delete/kill ou produção. Preservar HEAD/WIP/históricos/pins/originais/consumos/UNKNOWN/PST0.6.4. Aprovação solicitada é SOMENTE B/S0–S5 normativa/offline, nunca operacional.
