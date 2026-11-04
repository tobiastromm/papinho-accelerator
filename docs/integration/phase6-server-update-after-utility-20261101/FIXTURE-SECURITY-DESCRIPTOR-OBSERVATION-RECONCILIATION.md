# Reconciliação — leitura atual fechada, rejeição literal de máscara demonstrada

2026-11-03. Fonte: saída enviada no chat pelo proprietário. Linhas relevantes literais preservadas em OWNER-FIXTURE-SECURITY-DESCRIPTOR-OBSERVATION-OUTPUT.EXCERPT.txt; não se alega exportação byte-exata do terminal. Resultado FIXTURE-SECURITY-DESCRIPTOR-OBSERVATION-RESULT.json/SHA256315f3016e265369463be70ef282d829c37456bd1258482feb9354b48053a1324. Nenhuma consulta repetida ou operação adicional pelo agente.

Consulta first0/consumedTrue/completeTrue/observer_close_knownTrue. OpenProvider/OpenKey/GetProperty todos0, descriptor152bytes/buffer1024/flags5. O nome MACHINE selecionado pôde ser aberto e lido ATUALMENTE pelo administrador. Isso não comprova identidade pública/continuidade original, finalização/proteção histórica ou controle administrativo completo K2 da chave criada; publicSHA permanece desconhecido. Fixture/READ/Start NÃO qualificados. NTE_PERM do administrador na criação não é prova de isolamento do runtime.

## Predicado atual demonstrado

Owner operador=True, DACL protegida=True, exatamente três ACEs sem flags/callback, ordem/tipos/SIDs previstos: runtimeDENY, operadorALLOW, SYSTEMALLOW. Todas possuem0xd01f01ff. A projeção não rejeitou uma máscara parcial, owner divergente ou SID inesperado: ca_acl exige `ace->Mask == GENERIC_ALL`, cujo valor é0x10000000, e0xd01f01ff é diferente. Leitura NCryptGetProperty ATUAL sucedeu; a rejeição da projeção atual é local e estritamente literal de máscara.

Modelo do MESMO owner, recebendo leitura bem-sucedida com as três máscaras observadas e mantendo owner/protected/flags/tipos/ordem/SIDs, retorna NTE_PERM com first_failure.origin=CA_D_LOCAL_GUARD/stage=CA_D_ACE_MASK e18samples. Quatro configurações Selected/Legacy Debug/Release de dois testes afetados=8pares PASS, zero warnings, fixture255checks/source37cenários/146checks. Seis contraexemplos mantêm a distinção de getter nativo falho versus rejeição local. Logs anteriores preservados; campanha produtiva52pares não repetida. Código produtivo/EXE/guards não alterados; manifesto FIXTURE-OBSERVED-MASK-OFFLINE-QUALIFICATION.json registra pins e resultados.

Essa reprodução e o estado atual são fortemente compatíveis com a fronteira histórica de18amostras, mas NÃO recuperam os campos first_failure não exportados da criação: outro processo/token/handle e falta de publicSHA/reforiginal preservam UNKNOWN quanto ao ramo nativo exato histórico. Não afirmar que o setter ou getter histórico sempre sucedeu, que a chave parcial já é qualificada, ou que o Windows mapeou os bits de uma forma específica.

## Bits não são mapeamento de objeto

Decomposição numérica:0xd0000000 de genéricos(GENERIC_READ|GENERIC_WRITE|GENERIC_ALL),0x001f0000 padrão e0x000001ff específico. GENERIC_ALL continua presente; a comparação de igualdade é que falha. Os bits SACL/MAXIMUM_ALLOWED/reservados estão ausentes. Não interpretar0x1ff como operações de arquivo ou NCrypt, não aplicar FILE_GENERIC_* ou MapGenericMask de filesystem só pela coincidência numérica com FILE_ALL_ACCESS.

[Generic Access Rights](https://learn.microsoft.com/en-us/windows/win32/secauthz/generic-access-rights) e [ACCESS_MASK](https://learn.microsoft.com/en-us/windows/win32/secauthz/access-mask) definem bits e que o mapeamento depende do tipo de objeto. [Key Storage Property Identifiers](https://learn.microsoft.com/en-us/windows/win32/seccng/key-storage-property-identifiers) define a propriedade de descritor de chave; as fontes primárias consultadas não estabelecem a equivalência universal exigida para normalizar essa máscara como objeto de arquivo. Resposta em fórum/Q&A não foi usada como autoridade normativa ou definição de mapping.

## Gate normativo focal, sem código permissivo antecipado

K1 accepted exige congelar o projeto exato do SD/direitos antes da operação; T3 congelou DENY GA ao runtime e ALLOW GA ao operador/SY conforme primitiva qualificada. Não há decisão accepted sobre essa segunda forma de readback. A leitura atual não autoriza aceitá-la, aceitar qualquer superset de GA ou editar ACL do objeto parcial.

MACHINE-FIXTURE-GA-READBACK.CANDIDATE.md/SHA2563b090974b4abbf5db2543e8ef223cebf78ec8a4b08581d9fa3d7e7beb9b53d47 propõe SOMENTE B/S0–S5: allowlist fechada dos templates uniformes GA/GA/GA OU d01f01ff/d01f01ff/d01f01ff, no mesmo owner/provedor/fixture e READ diagnóstica da MESMA fixture legitimamente admitida. Todos os requisitos de owner/protected/trêsACEs/tipos/ordem/SIDs/flags/continuidade/autoridade/consumo/controle positivo/probe permanecem. Mistura, bit extra/faltante, terceira máscara, outro provider/perfil/objeto continuam DENY. Setter continua enviando GA original, sem escrever a máscara observada ou mudar ACL existente.

Aceitação proposta é risco explícito da representação exata observada, não afirmação de mapping CNG universal ou de permissões específicas. Abrange já a futura validação READ da mesma fixture para não criar outra etapa de arquitetura; não amplia READordinary/CA/issuer/TO/NEW_INSTALL ou autoridade de startup/KSP. Nenhuma quota nova ou alteração de recursos.

Primeiro gate: aprovação normativa explícita B/S0–S5. Ainda NÃO aprovada/implementada, nenhum código produtivo foi relaxado. Só após aceitação registrar norma separada, integrar/qualificar os dois templates e negativos/isolamento, congelar candidato e preparar UPDATE único para autorização própria. Sem UPDATE intermediário de diagnóstico ou nova coleta. Depois, eventual nova preparação fixture exige decisão operacional NOVA e tentativa exclusiva, preservando a chave parcial atual sem adoção/recuperação/delete. READ E/Q/S/P/Start continuam atos independentes.

## Preservação e próximo prompt

Autorização read-only utilizada/consumida, sem retry. Snapshot inicial pendente preservado; uso registrado em FIXTURE-SECURITY-DESCRIPTOR-OBSERVATION-AUTHORIZATION-USED.json. Ownerclose do observador não resolve application-close/retenção/reservas/quiescênciaOS históricos UNKNOWN. Chave parcial, decisões/originais, todos os .previous/candidatos/EXEs/DLLs/SCM/HEAD/WIP/pins/consumos/PST0.6.4 preservados. Sem ACLedit, limpeza/refund/rollback/delete/kill, UPDATE, fixture, StartStop/endpoint/SCQ/P/CA-CURRENT/rede/TPM-NV-Q4 ou produção nesta continuação.

Próximo prompt NEXT-MACHINE-FIXTURE-GA-READBACK-DECISION.md.
