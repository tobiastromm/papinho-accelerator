# Resolução focal de topologia — proposta, sem autorização

O recibo mostra exatamente dois clientes, self10252/pai17204, e HWND-process17204. O requisito de terceiro processo distinto está no pedido exato aprovado e no guard C/C# anteriores. Não é defeito introduzido na tradução C. Não remover `console==parent` para obter PASS.

GetConsoleWindow retorna HWND associado ao console; em pseudoconsole pode ser janela somente para fila de mensagens. GetWindowThreadProcessId retorna o criador da janela, não comprova a identidade do verdadeiro host/broker de console. Não se conclui pela igualdade dos PIDs que o auxiliar não existe, que seus custos são zero ou que somar o pai duas vezes o contabiliza. Também não se provou que esta execução utilizou pseudoconsole: essa é uma explicação compatível, não diagnóstico factual completo.

## Alternativas concretas

A — Preservar o contrato atual. Qualificar separadamente, sem TPM, um console suportado no qual o processo de janela seja distinto e os clientes/parent/helper reais sejam identificáveis. Não lançar outra prova para descobrir se a topologia funciona. Uma evidência desse ambiente, com imagens/ID/PID/criação e recursos, permitiria preparar novo pedido exato; não há autorização para nova execução ou alteração global de console no presente pacote. Custo: dependência explícita desse ambiente; não suporta automaticamente todos os hosts de terminal.

B — Binding explícito do auxiliar real por mecanismo canônico já existente. Reutilizar a infraestrutura única de PowerShell/identificação N7 do projeto para fornecer um auxiliar por PID/criação, proveniência e ownership verificáveis, sem usar HWND como autenticador. Preservar o pai por identidade própria, clientes exatos, censo de todos os participantes, contabilização por processo único, limite compartilhado1024 e próprio124/128, Stop/retenção. Se auxiliar e pai forem o MESMO objeto real, somente uma identidade original e cobrança; se houver outro broker necessário não identificado, DENY, nunca custo zero por alias. Isso exige revisão explícita da topologia do pedido e da aplicabilidade N7 ao novo contexto; autoridade LAB antiga não se transfere. Não introduzir IPC/serviço/owner novo. Custo: integração técnica e qualificação de binding/concorrência/fechamento antes de novo pedido, sem efeitos LAB.

Recomendação: para suportar o ambiente atual, desenvolver B sob decisão específica de contrato, com prova de cobertura administrativa, em vez de repetir A sem medição. Se o proprietário preferir manter o contrato de três processos, A é a alternativa mínima. Nenhuma delas concede prova TPM, NV, Q4 ou orçamento.

## Gate antes de outro pedido

Demonstrar uma associação e contabilização administrativa completas e compatíveis com o escopo escolhido, sem depender da suposição de que HWND identifica conhost. Reproduzir alias pai/self, PID reutilizado, auxiliar omitido, quarto/quinto participante, budget1025, query/close incerto, Stop e freeze. Reusar regressões atuais. Nova aprovação operacional somente após essa propriedade e os pins finais; a aprovação4a5f foi usada e não pode financiar a investigação.

Status=PROPOSED_TECHNICAL_CONTRACT_SELECTION; não ADR accepted, não decisão de autoridade, não comando executável. Guard funcional permanece íntegro.
