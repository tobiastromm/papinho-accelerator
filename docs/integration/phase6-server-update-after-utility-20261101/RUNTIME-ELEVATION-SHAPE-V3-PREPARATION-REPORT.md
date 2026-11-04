# Preparação V3 — consulta limitada da forma do token

Estado: qualificada offline; consulta manual PENDENTE de autorização específica. Nenhuma coleta ou operação de serviço executada.

O retorno V2 foi sucesso com comprimento diferente de4 na classe21; o comprimento histórico exato permanece UNKNOWN. O esclarecimento do mantenedor Microsoft em https://github.com/microsoft/win32metadata/discussions/2308 distingue BOOLEAN de um byte e alinhamento DWORD. A referência pública ainda diz DWORD; não há matriz universal qualificada de versões/arquiteturas. Não atribuir retroativamente comprimento1 à V2.

V3 exige sucesso e comprimento1 exclusivamente em21, lê apenas byte0 com endereço alinhado a4 e mantém capacidade defensiva4. Demais escalares continuam exigindo4. Tamanho divergente mantém DENY. Captura o comprimento da chamada original sem consulta adicional; impressão não mascara a primeira falha. Valores provisórios não substituem revalidação final. Não altera o predicado do produto que rejeitou TokenIsElevated não zero.

Qualificação: compilação da fonte real sem RunFixed; 47 cenários modelados,20 verificações de capacidade,1 de alinhamento e3 de preservação de erro PASS;13 fronteiras nativas substituídas; AST do wrapper PASS. Sem repetição de builds do produto porque nenhum código do produto foi alterado. Evidências e hashes: RUNTIME-ELEVATION-SHAPE-V3-OFFLINE-QUALIFICATION.json.

O controlador6120 e o runtime localizado exclusivamente pelo SCM permanecem sujeitos à correlação e revalidação do pedido. Nenhum processo foi consultado ou encerrado nesta preparação. Close do observador não prova close das aplicações. Retenção histórica, quiescência OS e admissão bilateral permanecem UNKNOWN; readiness produtiva DENY.

Primeiro gate: aprovação específica do pedido ONE-RUNTIME-ELEVATION-SHAPE-V3-READONLY-REQUEST.CANDIDATE.md, SHA256 `8642212538880521ede3e4b7b8dadc2a2b46914aab24abf76cd1682dbe5044ee`. Autorizações V1/V2 não são reutilizadas. Não há necessidade de UPDATE para esta correção do observador. Após saída manual suficiente, confrontar a forma do token com os contratos accepted antes de qualquer alteração dependente.
