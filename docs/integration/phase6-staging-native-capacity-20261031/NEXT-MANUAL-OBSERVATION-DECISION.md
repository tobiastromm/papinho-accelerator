# Phase 6 — decisão e handoff da coleta read-only de staging

Continuar CLOSEOUT.md e OBSERVATION-REQUEST.CANDIDATE.md deste diretório.
Não refazer sampler/cópia/close ou executar automaticamente a observação.

1. Conferir os bytes/SHA256 do pedido, script e QUALIFICATION-BUNDLE.json.
   O script tem pins finais; nenhuma identidade futura precisa ser inventada.
2. Aguardar decisão explícita sobre o pedido exato de uma única coleta manual.
   Este prompt de continuação não é aprovação. Não converter aprovação antiga
   de staging/SCQ/normativa em autorização da coleta.
3. Após aprovação válida, registrar a decisão pelo mecanismo governado existente,
   calcular seu hash e entregar o comando manual completo com o SHA256 conhecido
   do script. Não executar, elevar, alterar ExecutionPolicy, criar outro owner
   ou adicionar um ledger. Não usar placeholders ou gerar nova autoridade.
4. Após a saída manual, conferir pins/primeira localização e predicado nativo,
   operador, frames alcançados, quota/espaço do volume, físicos/commit/handles,
   consistência temporal e fechamento/retidos. Exit0 é coleta, não admissão.
5. Preservar máscaras UNKNOWN e qualificar apenas o alcance observado. Não
   promover picos em ceilings ou inventar margem. Se custos ativos de FS ainda
   forem materiais, delimitar a menor evidência adicional, sem autorizar writes
   por inferência nem exigir staging/serviço existente para observar o controlador.

Não repetir coleta após falha/UNKNOWN sem decisão própria. Corrigir defeitos
técnicos demonstrados offline e testar só mudanças afetadas. Sem staging,
SCM/ACL, instalação, Start/Stop, endpoint, CA/CURRENT, KSP/credenciais, rede,
TPM/Q4, cleanup/refund ou recuperação. Preservar HEAD/WIP/históricos, fixture
retida, UNKNOWN/reservas e PST0.6.4. E21/ED11/Phase6 não recebem PASS por coleta.
