# Resolução candidata v3 - gate factual e handoff delimitado

Revisão de implementação, sem decisão normativa nova e sem patch funcional. D2.2/D4 e F1/F2/G3/G4/G7 continuam obrigatórios. Não há conflito entre autoridades: a lacuna é entre a exigência accepted e o produtor funcional disponível.

| Referência | Fonte e proteção existentes | Limite factual / disposição |
|---|---|---|
| CURRENT/predecessor | current_pin/current da root; ticket movido para o cohort; bytes completos em committed/G7/previous | Instalação factual indisponível conforme owner; fixtures não suprem esse requisito. Não fabricar pin, revisão ou epoch. |
| Configuração/policy/security_ref | config_source_resolve + pc_bind_existing_models, leitura síncrona de CURRENT; source publicada com borrow existente | Resolve somente configuração modelada correspondente ao CURRENT e referências Secure existentes. Não produz aprovação da tupla candidata nem delegação PACKAGE_SIGNING. |
| SNT/ExpectedServerName | Codec e componente público kind7; bytes G7 imutáveis | Correlação de identidade/issuer não prova ato administrativo aprovando o nome/perfil candidato. kind7 estágio3 retorna INCOMPLETE pelo gate DER/perfil/SAN. |
| PDG | Codec, kind8 e linha de disposição G3 | APPROVED e disposition_approval_root_R são dados; faltam produtor/consulta da autorização aplicável à tupla candidata e sua revalidação factual. Não inferir aprovação de hash ou revisão<=R. |
| CA/issuer | Carrier íntegro, histórico/high-water/revogação preservados; CA DER retida no mesmo G7 | Continuidade/certificado público não substituem autoridade, UTC confiável, status vigente e aplicabilidade da delegação. |
| Lifetime candidato | Slot livre do owner, serial0, sem pin exportável; pin e source anteriores retidos | Existência de backing imutável prova memória, não autoridade administrativa. kind8 exige endereço exato no backing protegido pelo pin; não se pode passar pin predecessor para bytes candidatos. |

## Por que não foi ligado o resolver existente por adaptação permissiva

O cohort recebe uma imagem binária, não retém um PC_DOCUMENT validado nem fontes de aprovação de SNT/PDG. source_resolve recebe documento e pin CURRENT; resolve contra pin.view.config e a source publicada. Seu out.resolved pertence a esse escopo. Chamar esse leitor e copiar o flag para a candidata mudaria sua semântica e violaria D2.2/D4. Converter a candidata em snapshot pinado para satisfazer kind8 fabricaria proteção/publicação que o cohort expressamente não concede. Nenhuma dessas adaptações foi aplicada.

A implementação atual de update_validate reconstitui uma view do mesmo ticket real, revalida source/CURRENT e termina NOT_SUPPORTED; update_commit chama esse gate e não inicia packet11. O STOP da seção9 desta tarefa impede a integração dependente enquanto as fontes não forem comprovadas. A ausência já declarada de CURRENT não explica sozinha a lacuna funcional do produtor de aprovações.

## Menor sequência de continuação no mesmo owner

1. Conectar o ato administrativo autorizado já exigido por F1/G3 à preparação candidata no produtor existente, com consulta factual da tupla completa, revisão, validade/status/revogação e origem. Reutilizar a policy/issuer/root; nenhum boolean de caller ou PDG decodificado pode substituir o ato. Se a representação/API necessária ultrapassar os contratos accepted, submeter somente a extensão focal antes de implementar.
2. Proteger a observação e todas as fontes pelo lifetime da operação no mesmo cohort. Para validação pública pré-publicação, demonstrar acesso ao slot candidato por seu ownership existente, sem pin fictício e sem enfraquecer o contrato de borrow publicado do kind8. Reconciliar buffers/arenas e stack antes de hunks.
3. Reutilizar os participantes/resolver canônicos para as referências cobertas, correlacionando explicitamente o resultado ao documento/imagem candidata imutável e à mesma base. Somente a prova completa de todas as fontes pode produzir RESOLVED; resultados parciais continuam negados.
4. Em ambiente autorizado com CURRENT factual, revalidar predecessor completo, ticket, revisão, identidade, fontes e Stop antes do efeito; usar o worker/CAS existente. Não executar commit positivo nesta campanha nem publicar CURRENT por este handoff. UNKNOWN mantém ambas gerações, source, buffers e créditos até disposição/quiescência factuais.

Abort continua próprio do cohort após quiescência; não usar transaction_abort histórico. Stop não libera nem presume cancelamento. Não há alteração de E0/M1, confirmação Browser, formatos históricos ou wire. Handoff não aprova APIs/hunks funcionais por referência.

## ED11

Sem mudanças funcionais: delta fixo/transitório novo=0. Censo anterior permanece referência, não medição de pico nova: Selected fixo1948656; Legacy1790872; três G7=116616 e três imagens=91776 já incluídos. Input caller até30592 e frames transitivos não são gratuitos. Cenários anteriores2225439/2299559/2371223 não são reservas; composição completa conservadora3747601 excede Windows3MiB em601873. Não se iniciou essa composição nem se usou a margem fixa para autorizar resolução. Novos custos de produtor/fontes/arenas/lifetime continuam não determinados; provider/kernel/stack transitivo UNKNOWN, ED11 totalPARTIAL. Windows3145728 apenas explícito, default2097152 e quotas individuais intactos.

## Cobertura desta retomada

Reexecutados quatro builds incrementais e74 suítes existentes (21+21 Selected,16+16 Legacy), zero warnings nos logs. Incluem negativos CURRENT, kind8, backings, cohort/abort/Stop/retenção, corrupção/revisões/compatibilidade e ED11. Não foram criados testes funcionais novos, prova positiva de resolução nem prova nativa de CURRENT. Casos positivos DATA/lifetime sintéticos continuam identificados como tais. Regressões específicas de fonte factual candidata não podem ser apresentadas como executadas: o produtor ainda não existe.
