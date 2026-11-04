# Especialização prospectiva da entrega privada SCQ

Status: PROPOSED. Não é aprovação nem pedido operacional. Não altera T0–T14,
OHI, NEW_INSTALL, formatos ROOT/CURRENT ou PACC wire enquanto não aceita
explicitamente. Escopo exclusivo: PERSONAL_LOCAL, uma sessão factual SCQ,
administrador controlador e runtime `NT SERVICE\PapinhoAccelerator`.

## Necessidade demonstrada e alternativas

OHI exige origem administrativa separada do destinatário virtual. O receptor
atual empresta ponteiros/handles locais; isso não cruza processos. O único
transporte privado encontrado exige mesmo TokenUser/AuthenticationId/sessão,
origem CreateProcess e stdout de qualificador para supervisor. T0 exclui seu
uso por runtime/serviço; T11 proíbe transportar autoridade, credenciais ou
comandos. O ensaio `scq_transport_scope_test` exercita seu predicado real e
rejeita identidades, autenticações e sessões distintas, sem abrir pipes.

Alternativa A: manter somente aquisição/custódia de metadados e DENY. Não
completa a sessão SCM/CNG, mas preserva integralmente os contratos atuais.

Alternativa B recomendada: aceitar a especialização estrita abaixo, permitir
qualificação offline de um perfil privado SCQ no mecanismo nativo existente.
Não herdar o perfil N0 ou substituir `same_token` por true. O custo é manter
duas políticas explícitas de pares/payload, sem dois owners ou serviços.

Não recomendar executar o probe como administrador, impersonar o serviço,
copiar ponteiros entre processos ou exigir que conta virtual e administrador
tenham o mesmo token: isso não prova a negação efetiva pelo KSP.

## Regras propostas, indivisíveis

1. Manter T0–T14 e seus pares/stdout originais intactos. O perfil SCQ será
   prospectivo, separado, no mesmo owner/runtime/recursos governados. Sem
   serviço adicional, helper permanente, autoridade de aprovação, root,
   registry de confiança ou endpoint genérico/remoto. Nenhuma autorização ou
   exceção de LAB/TPM é reutilizada. Este texto autoriza apenas implementação
   e qualificação offline após aceitação, sem criação de endpoint real nesta
   decisão.

2. Para SCQ somente, permitir um vínculo efêmero local de entrega de
   referências diagnósticas somente leitura entre o controlador administrativo
   original e o processo SCM original. Reutilizar as primitivas nativas de
   transporte/custódia existentes, não o direito concedido ao LAB. Um par,
   uma tentativa, namespace local fechado, proteção desde criação exclusiva,
   sem adoção/reconexão/retry. Obter ambos os pares por referências originais,
   PID/criação/TokenUser/grupos/restrições/continuidade e identidade de imagem
   e serviço governadas; nome/PID/nonce/pipe/hash isolados não autenticarão.
   Não exigir igualdade entre as identidades administrativa e virtual.

3. Permitir exclusivamente um empréstimo vinculado ao pedido e decisão
   originais, política, propósito SCQ, tentativa, imagem/serviço/conta, chave
   MACHINE fixture e processo destinatário exatos. Transferir somente direitos
   mínimos de leitura/query necessários e referências controladas; nenhum
   direito administrativo de ação, token impersonável, chave privada,
   credencial/segredo, comando de emissão/instalação ou admissão produtiva.
   Não serializar ponteiros locais como referências válidas no destinatário.
   A representação privada exata, tamanho compilado e validação por ambas as
   pontas deverão ser demonstrados em código e negativos antes de produzir
   pedido operacional. Não é formato de interoperabilidade público nem
   alteração de ROOT/CURRENT/PACC wire.

4. A origem continuará sendo a decisão operacional byte-exata do proprietário
   e a confirmação administrativa atribuída do plano, sob OHI. Para o plano,
   usar uma projeção SCQ tipada, estritamente não secreta e com todos os campos
   ativos reconhecidos/validados. Não imprimir arquivos arbitrários, conteúdo
   bruto, extensões desconhecidas ou material privado. Campo desconhecido,
   ambíguo, secreto ou não representável implica DENY; não omiti-lo para obter
   confirmação. Hashes/identidades resumem a associação aos originais retidos,
   não substituem conteúdo/proveniência/SD ou aprovação. Aquisição de arquivo
   e token elegível isolados nunca aprovam SCQ.

5. O controlador consome a autorização de uso único antes do primeiro efeito
   protegido, inclusive eventual Start. Reutilizar seu mecanismo governado
   de exclusividade/consumo/receipt, sem novo ledger de autoridade. O runtime
   recebe empréstimo restrito da tentativa já governada, não outra aprovação
   nem um contador de memória apresentado como consumo durável. Rejeitar
   replay, troca de objeto/peer/propósito/geração e concorrência indevida.
   Restart não restaura uma aprovação por arquivo/CLI/novo token; perda do
   controlador ou continuidade não comprovada mantém DENY/UNKNOWN.

6. Referências/buffers/operações pendentes ficam no owner existente até
   fechamento factual, com Stop, primeira falha e receipt de cada lado
   separados. Close incerto/perda de peer/persistência parcial conserva consumo
   e retenção UNKNOWN; sem nova tentativa, overwrite, cleanup, refund ou
   recuperação implícita. Não prometer fencing ou resistência global a rollback.

7. Manter 124/128/1024 e todos os gates aplicáveis de recursos, freshness,
   headroom, identidade, proteção e lifetime. Custo do novo perfil e limites
   faltantes devem ser demonstrados, não copiados do LAB. UNKNOWN obrigatório
   continua DENY. A aceitação não afirma qualificação da topologia/SCM/token,
   provider/kernel, KSP MACHINE, consumo durável ou capacidade operacional.

8. Nenhuma sessão, instalação, diretório, conta, CA produtiva, CURRENT,
   Start/Stop, SCM/ACL, rede operacional, TPM/NV/Q4, limpeza/refund ou recuperação
   é autorizada. A futura execução manual exige pedido operacional próprio,
   byte-exato, gates frescos e aprovação independente. Criação de parent,
   se indispensável, continua exigindo autoridade própria. Não ampliar o
   escopo da primeira instalação nem mudar prioridade para outra frente.

Aceitar B permitirá integrar e qualificar a especialização limitada; não
declara a entrega autenticada já implementada. Sem aprovação explícita desta
especialização, prevalece A. Nenhum documento accepted anterior será editado.
