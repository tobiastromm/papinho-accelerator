# Descritor atual e proteção do destino

## Conclusão técnica

FILE_ADD_SUBDIRECTORY (`0x4`) permite criar filho, não concede isoladamente escrita no filho existente ou sua substituição. Um componente ausente sob parent que admite criação por terceiros pode ser ocupado antes do instalador: criação exclusiva deve então negar, sem adoção. A análise correta é por relação parent/filho, não remoção global do bit 4 da whitelist.

| Propriedade | Direitos/fatos que precisam ser distinguidos |
|---|---|
| Criar filho | ADD_SUBDIRECTORY no parent; não prova controle do filho existente |
| Alterar raiz/parent | WRITE_DAC/WRITE_OWNER, owner/privilégios; WRITE_ATTRIBUTES/EA são alterações distintas |
| Apagar/substituir filho | DELETE no filho ou DELETE_CHILD no parent, condições de rename/criação, handles/compartilhamento e continuidade |
| Modificar filho existente | DACL do próprio filho, token/grupos/restrições/privilégios e herança efetivamente aplicada |
| Travessia | FILE_TRAVERSE/bypass não concedem escrita; não ajustar privilégios na consulta |
| Filho futuro | identidade factual somente após criação exclusiva; proteção na criação e revalidação, sem adoção |

Fundamento: [segurança de arquivos Microsoft](https://learn.microsoft.com/en-us/windows/win32/fileio/file-security-and-access-rights) e [direitos específicos](https://learn.microsoft.com/en-us/windows/win32/fileio/file-access-rights-constants). A DACL do filho controla acesso ao filho; herança transmite ACEs conforme flags. DELETE_CHILD no parent exige análise específica mesmo com filho aparentemente restritivo. Isso não simula AccessCheck para todos os usuários.

Snapshot de ACL não demonstra ausência de handles previamente concedidos, mudanças entre amostras ou resistência a OS/administrador comprometido. F1 delimita confiança administrativa. UNKNOWN material sobre substituição mantém DENY; token administrativo não prova segurança universal.

## Cláusulas accepted

F3 exige parent por identidade nativa retida, nome exato, referências protegidas contínuas, revalidação antes do efeito e criação exclusiva. F2 distingue direitos/contexto de aprovação; F5/F6 preservam consumo/invalidação/retenção. D5 exige proteção/identidade dos ancestrais e ausência de reparse/substituição. B3.1/B4 mantêm proteção dos ancestrais, sem adoção, e descritor aplicado na criação. B1/B2 especializam recursos opacos, não proteção. A decisão de destino permite o novo caminho PERSONAL_LOCAL, não alteração de ACL ou migração.

Nenhuma dessas cláusulas localizadas equipara literalmente todo ADD_SUBDIRECTORY em ancestral à substituição do filho. Todas preservam proteção factual. FOREIGN_WRITE implementa aproximação estrutural mais abrangente; essa diferença não autoriza dispensá-la sem demonstrar preservação de F3/D5/B4. Não foi demonstrado conflito entre duas normas accepted. Nenhuma norma foi alterada.

## Evidências e lacunas

O pacote anterior preserva descritor histórico com AU ALLOW 0x4 aplicado à raiz, owner TrustedInstaller e DACL protegida; a regra atual o rejeita. Projeção OWNER+DACL de 168 bytes não prova igualdade com o descritor da tentativa. Erro 5/location 5 localiza a guarda; o submotivo daquela sessão não foi salvo.

Faltam OWNER/DACL/identidades atuais da cadeia existente, direitos por aresta, estado ausente/existente de Papinho/Accelerator e revalidação/close. Não preencher com história. O [pedido delimitado](READONLY-CHAIN-REQUEST.CANDIDATE.md) amplia explicitamente a especificação anterior root-only aos componentes existentes conforme esta task, sem ampliar autorização alguma. Está completo quanto ao escopo, mas faltam procedimento mínimo/pins/qualificação para prontidão executável. Não há comando prematuro.

## Menor correção candidata — não aplicada

Se evidência demonstrar que somente ADD_SUBDIRECTORY na âncora é o excesso conservador, separar o papel de âncora de parent criador e objetos novos, vinculando a decisão ao próximo filho original protegido. Preservar análise de DELETE_CHILD/DELETE/WRITE_DAC/WRITE_OWNER, direitos genéricos, owner, reparse, compartilhamento/handles e continuidade. Não excluir 0x4 globalmente ou permitir trustees/tipos desconhecidos.

Antes de aplicar, demonstrar proteção por aresta com lifetimes reais. Testar filho ausente/existente, criação concorrente, DELETE_CHILD, DELETE no filho, owner/ACL mutáveis, herança/INHERIT_ONLY/DENY, reparse, troca de identidade, Stop e UNKNOWN. Risco factual ou interpretação incompleta mantém DENY. Se exigir garantia normativa nova, apresentar somente a divergência exata, sem nova ADR automática.

## Trabalho realizado e próximo gate

Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469. Conferidos especificação anterior, código da guarda/aquisição e contratos F/D/B/destino aplicáveis. Somente documentação derivada mudou; nenhum build/teste inalterado foi repetido, nenhum código/guard/ACL alterado e nenhuma consulta executada. WIP/pins/históricos preservados.

Próximo passo: vincular e qualificar offline a aquisição mínima ao pedido, congelar pins e apresentar para autorização específica. Depois reconciliar cadeia atual e corrigir somente o que for demonstradamente compatível, ou identificar a aresta insegura mantendo DENY. Novo staging exige novo pedido; autorização anterior continua utilizada. Registro SCM parado é posterior/separado. E21 PENDING; ED11/Phase 6 PARTIAL; PST0.6.4 preservado.
