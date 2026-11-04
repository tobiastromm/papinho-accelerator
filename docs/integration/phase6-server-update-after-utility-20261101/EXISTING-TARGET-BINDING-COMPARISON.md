# Alvo existente — comparação e recomendação focal

Estado: decisão preparada, implementação dependente NÃO autorizada.
Análise offline na branch main/HEAD f341985924d29870d8ff627ed37c8a0874ad6469,
com WIP preservado. Não repetir pesquisa de APIs, coletas ou campanhas.

| Alternativa | Contratos/autoridade | Lifetime e G3 | Conclusão |
| --- | --- | --- | --- |
| Descobrir ProgramFiles pelo mecanismo atual, adquirir e validar originais | BDI knownfolder; OHI/TO4–5/G2 e atos atribuídos | Handles completos,124 antes de novas referências/128máximo. Shell opaco já ocorreu antes do primeiro open | Fonte preservada, mas observações atuais negaram136; nenhuma economia demonstrada |
| Reusar referência protegida viva | BDI preserva instalação existente; origem/continuidade/Stop OHI/SSO-B | Referência conta no processo dono e no empréstimo; fechar só no fim factual. Revalidar IDs/SD/ancestrais | Compatível em princípio; NÃO disponível no controlador frio. Snapshots históricos não são handles vivos |
| Apenas reordenar consultas/gates | G4 exige contabilização completa por participante e gate fresco antes de cada efeito | Consultar antes de Shell não permite ignorar recursos adquiridos depois | Rejeitada como solução. Mover Shell depois do open pode apenas transferir a falha para outra guarda e continua sem provar origem/margem |
| Alvo existente selecionado e readquirido, sem discovery atual | Exige clarificação explícita da fonte BDI para esta instalação e vínculo original com G2/TO/OHI | Todas as referências, sobreposições, readers e retenções contadas; nenhuma promessa de passar124/128 | Menor alternativa recomendada; proposta focal única preparada, sem implementação positiva |

## Por que a ordem sozinha não basta

No código scqc_plan: token/caminho precedem os3 originais; depois profile,
session_acquire_originals, chain, close dos raw e confirmação. Abrir request/
decisão primeiro só é possível enquanto count<124, mas não elimina discovery
posterior ou sua verificação. A guarda da cadeia também consulta o total antes
de cada referência. Consultar count113 antes do custo que levou a136 e conservar
essa amostra para efeitos posteriores violaria freshness/G4. A confirmação não
transforma136 em admissível. Deslocar custo a outro processo não o elimina nem
estabelece a origem do empréstimo automaticamente.

Pode haver reorganização legítima de sobreposições quando ownership/lifetime
forem demonstrados, mas não se identificou fechamento novo autorizado nesta
análise. Readers são ReOpenFile com cursores próprios, não aliases descartáveis;
originais e duplicações têm papéis distintos. O custo exato do futuro desenho
deve ser censado por fase e pode continuar excedendo o envelope.

## Evidência de viabilidade e limite factual

Já existem no mesmo owner image_chain com8slots de diretórios, aquisição relativa
com OBJ_DONT_REPARSE, comparações volume/file ID, descritores originais,
proteção anchor/child, fechamento/retention e projeção target no plano SCQ.
O histórico qualifica ProgramFiles417614100:131072:226316; a consulta terminou
com fechamento conhecido, não deixou referência viva emprestável nesta entrada.
O root é zerado no owner_acquire, sem producer inicial de diretório protegido.

paths_existing somente copia strings e supõe referências validadas separadamente.
Não cumprir essa segunda metade e remover a rejeição de modo do SCQ seria bypass.
OpenFileById só ajuda a reaquisição depois de fonte legítima do alvo e volume;
GetModuleFileName/SCM só ajudam a localizar candidatos. Nenhum concede a origem.
Essa constatação é de código e contratos já examinados; não foi feita coleta nova.

A fonte BDI manda resolver pela knownfolder e preservar a instalação existente.
Não há conflito entre ADRs demonstrado; a questão é tornar explícito que nesta
sessão se preserva o OBJETO da instalação previamente qualificada, mesmo sem
provar novamente o mapeamento ATUAL do Windows. Isso é mudança de garantia de
descoberta/proveniência, não mero reordenamento de uma chamada. Recomendação
conservadora: obter essa decisão antes de código dependente.

## Única proposta e primeiro gate

EXISTING-PROGRAMFILES-ORIGINAL-BINDING.CANDIDATE.md: alternativa B/P0–P7,
restrita a TRANSPORT_ONLY desta instalação existente. Não nova infraestrutura
ou quota. Requer seleção administrativa NOVA, reaquisição/validação de objetos,
binding imutável pelo target existente e validação independente nas duas pontas.
Sem novo canal, fields/wire/persistência ou autoridade por relatório/hash.

Garantia reduzida exposta: não se certifica o mapping atual de ProgramFiles no
OS; preserva-se o objeto existente legitimamente selecionado e validado agora.
Não se promete continuidade histórica. Se o proprietário exigir discovery atual,
alternativa A mantém DENY. Nenhuma aprovação/ACCEPTED inferidos.

Primeira lacuna concreta: aceitação ou rejeição dessa regra de fonte e, caso
aceita, integração do producer do alvo original no packet das duas pontas.
Não falta outro valor de quota, nova contagem ou PID/token futuro.

Qualificação nesta tarefa: revisão de código/contratos, sem alteração funcional,
sem builds/testes repetidos. O plano P7 define regressões de origem, volume/ID/
arquitetura, relação parent/filho, reparse/SD/direitos, quota/pico, Stop/replay,
autenticação/receipt/close e UNKNOWN para eventual implementação autorizada.
Não declarar viabilidade operacional só pela existência das primitivas.

Caminho mínimo: decisão focal → implementação/censo/qualificação offline no owner
existente → artefato estável → eventual UPDATE próprio → novo pedido E/Q/S →
gates frescos/prova nativa. Sem novo Start enquanto DENY. Não executar coleta,
UPDATE, endpoint/SCQ/Start/Stop/SCM/ACL/CA/CURRENT/rede/TPM/NV/Q4/limpeza/refund.
Preservar retidos/atos consumidos/HEAD/WIP/pins/reservas/PST0.6.4 e UNKNOWN histórico/OS.
