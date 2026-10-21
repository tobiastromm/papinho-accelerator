# Revisão focal e handoff executável

Estado: PROPOSED, nenhum código aplicado. HEAD/branch/baseline/preimagens constam do manifesto. PROJECT-CONTEXT.md ausente; AGENTS.md local vigente lido. Esta é continuação focal, não nova auditoria geral.

## Divergências da proposta histórica

1. N6/F5/I4 não aceitam codec: nenhuma autorização genérica pode promovê-lo implicitamente.
2. LST1 histórico não atribui números aos bits presence do registro: C2 os fixa na proposta nova, sem editar original.
3. Header R/A/P representa proveniência já observada, não prospective CURRENT. A nova publicação não pode obter epoch por encode. Predecessor histórico não substitui expected CAS da próxima transação.
4. authkind CERTIFICATE_IDENTITY não representa autenticação CLOSED usuário/senha. Sem provider governado, primeiro subset nega CLOSED e esse authkind; NONE é ausência explícita de provider de acesso, não ausência de mTLS do canal.
5. Modelo BODY0.5 guarda um único listener. Digest de modelo não persiste o conteúdo da coleção nem identifica fonte autorizadora suficiente. Root CURRENT/pins/CAS são existentes, mas não há envelope de coleção accepted. Não modificar BODY0.5/ROOT06/trailer de checkpoint para inseri-la.
6. Bridge config_source_resolve é read-only contra CURRENT preexistente, não constructor de nova configuração. Seu resultado não é consumido pelo bind. A raiz da transação atual recebe PAPACC_SERVER_CONFIG singular; transformar collection em singular é proibido.
7. SELECTED permanece negado no consumidor real, relatório topology_proven/resolved falso; comparação isolada não fornece aplicabilidade. Não bloquear o codec de dados por essa lacuna, mas negar apply dependente.

## Próximo patch após aceitação C1–C5

Somente biblioteca codec offline portável no módulo config existente, representação privada bounded, adapter SHA256 existente e testes. Não adicionar persistência/worker/registry. Submeter hunks e preimagens antes da gravação funcional. Reconciliar C90/build opt-in e ED11 antes de compilar. Não acoplar AST ao disco ou importar Windows/CNG no núcleo.

Testes Debug/Release zero warnings: roundtrip N0/1/4, ALL/SELECTED com IDs0/max, presença explícita, ordem, perfil ausente/desconhecido, CLOSED/CERTIFICATE_IDENTITY unsupported, duplicatas, corrupção em header/registro/trailer, truncamento em fronteiras, overflow/length/count, tails/reservados/trailing, versões/magic desconhecidos, null/overlap, hash indisponível, refs estruturalmente válidas porém unresolved sem autoridade, histórico não reinterpretado. Test hash sintético não prova SHA256 nativo nem autoridade.

## Integração transacional delimitada

Depois do codec offline, outra revisão exata deve definir composição versionada dos modelos reais na MESMA envelope/root e seus três slots/pins/journal/worker/CAS. BODY0.5 e ROOT06 atuais não têm extensão accepted. Nenhum formato de envelope é aceito por este handoff. Reutilizar prepare/validate/commit/publish/pending do owner existente sem chamada que publique CURRENT enquanto faltarem refs/capacidade/reserva. Tests CAS conflict/UNKNOWN/Stop/rollback/epoch/retention precisam executar a composição efetiva futura; PASS histórico não a qualifica.

Não implementar config apply nem publicar CURRENT nesta etapa. Validate/diff/show-effective também devem distinguir SOURCE da seleção CURRENT por pin real e não dar impressão de aplicação parcial. Named provider e topologia continuam gates próprios, não inventar implementação para concluir o codec.

## Aprovação requerida

Owner: aceitar ou corrigir exatamente C1–C5 de CODEC-CONTRACT-PROPOSED.md para o codec offline apenas. Isso não aceita envelope futura, publicação/CLI produtiva nem layout histórico alterado. Depois, registro normativo accepted e revisão oficial dos hunks funcionais, sem promover por referência nem contornar negativa. Revisão oficial desta entrega documental não substitui aceitação humana do novo layout.
