# K5 — compatibilidade do retorno exato e fechamento da investigação

2026-11-03. OFFLINE_REVIEW_COMPLETE / NO_GOVERNANCE_CONFLICT_DEMONSTRATED / NO_CODE_CHANGE.

## Decisão técnica desta revisão

K5 é um critério suficiente de qualificação limitada, deliberadamente conservador. Não é uma afirmação de que o Windows sempre retornará NTE_PERM quando um principal não obtiver uma chave. Não foi demonstrada incompatibilidade normativa: K5 exige NTE_PERM para qualificar; classificaBADKEYSET como inconclusivo. I0–I6 mantém essa classificação e permite terminar de forma controlada semRUNNING. A implementação e o resultado recebido correspondem exatamente a essa combinação accepted.

A documentação NCryptOpenKey descreve BADKEYSET como chave não encontrada e apresenta retornos não exaustivos. Não garante retornoNTE_PERM para toda falha ligada a permissão, nem fornece um oráculo de motivo interno. Portanto, um teste que exigeNTE_PERM pode legitimamente não qualificar um ambiente mesmo sem sucesso de abertura. Isso é insuficiência de evidência para o critério escolhido, não bug automaticamente demonstrado no controlador ou no Windows.

Não aplicar a semântica documentada deCryptAcquireContext/CSP por analogia: é outra API, outro contrato/provider. Documentação que admiteBADKEYSET como acesso negado naquela API não torna a hipótese provada emNCrypt/SoftwareKSP. Tampouco extrapolar cachesLSA, efeitos de perfil de serviço ou ausencia5058 para uma causa interna exata.

## Três afirmações separadas

| Afirmação | Evidência atual | Resultado |
|---|---|---|
| Esta consulta restrita não entregou handle de chave ao runtime | caminho original tipadoBADKEYSET estrito/handleNULL, resultado transportado, auditoria5061sobSIDruntime0x80090016 | Observação limitada corroborada |
| K5 qualificou negação por permissão da mesma chave | K5 exigeNTE_PERM+NULL, controlepositivo e demais provas; retornouBADKEYSET | Não qualificado; DENY |
| Sessão encerrou controladamente | SESSION_CLOSED_INCONCLUSIVE, CONTROLLED_CLOSEKNOWN, runtimeexit/SCM/finish exigidos no código, CLI3 e retorno ao prompt | Marco de lifecycle diagnóstico alcançado |
| Prontidão produtiva/isolamento universal | não decorrem de um probe, nem de exit ou auditoria | DENY |

A prova de não obtenção de handle nesta tentativa não implica impossibilidade de outros acessos, operações privadas ou ambientes. O controlepositivo administrativo demonstra que a chave era abrível naquele contexto; não permite concluir por si só que o runtime resolveu a mesma identidade interna e foi barrado pela ACL. Essa última ponte factual ainda falta. Os eventosSecurity não carregam aqui tokenoriginal/clientPID/causa interna suficiente.

## Evidência concreta e escopo

Sessão2a2db773415e5b78cf4b165e5bbf91ae78f918ee8bb64acf030d0692aab5095b fechada inconclusiva. Auditoria já existente: operador5058/5061sucesso duas vezes; SIDruntime5061record1170245/UTC2026-11-03T21:13:47.3425783/0x80090016. Consulta consumida com5eventos/fim observado/observerclose conhecido. Ausência5058runtime nofiltro não demonstra ausência deIO/ACCESS_DENIED interno/NAME_NOT_FOUND/cache/ACL como causa.

Comparação de código já realizada: mesmo nome/provider/legacyKeySpec0/MACHINE|SILENT; contextos de segurança distintos intencionais, runtime próprio semimpersonação; nome176bytesWCHARcomterminador, controlepositivo mesma chave/adminretido; transporte e fechos originais. Nenhuma falha técnica demonstrada a corrigir. Não há motivo para atualizar EXE, mudarquota ou repetir a tentativa para provocar determinado erro.

## Necessidade normativa

Nenhuma revisão normativa é indispensável para reconhecer o marco já alcançado: I0–I6 cobre integralmente o encerramento inconclusivo. Por isso NÃO foi criada novaADR/proposta de substituiçãoK5 por conveniência. Também não há autorização para esse enfraquecimento.

Para declararREADqualificado, permanece obrigatório satisfazerK5 vigente. Se o proprietário quiser mudar o requisito de produto para aceitar somente não obtenção dehandle num ensaio limitado, isso será decisão durável diferente, com análise explícita das garantias perdidas e dos gatesP4/OHI/deinstalação; não pode ser efeito implícito deste relatório, de um statusWindows ou de uma aprovação deUPDATE. Não recomendar converterBADKEYSET emPASS para destravar progresso.

## Primeiro gate e caminho mínimo legítimo

O objetivo de primeiroStartdiagnósticofechado já foi cumprido, com outcomeinconclusivo e nãoREADPASS. Encerrar a cadeia de correções e observações destinada somente a esse objetivo. Não há novo comando operacional a executar agora.

O gate para uma afirmação mais forte de isolamento continua sendo prova de negação qualificada conformeK5/P4/OHI, ou mudança normativa explicitamente decidida após escopo de requisito e riscos. A causa internaKSP só justifica nova investigação se houver mecanismo específico de evidência com poder discriminatório demonstrado; o mesmo probe/status/auditoria não basta. Não inventar que uma próxima sessão necessariamente produziráNTE_PERM.

Próximo trabalho permitido sem operação: inventariar quais condiçõesPERSONAL_LOCAL permanecem abertas e quais evidências cada uma requer, tomando lifecyclediagnósticofechado como concluído, READisolamento comoDENY, e sem reconstruir infraestrutura ou repetir esta investigação. Nenhuma nova fase de implantação ou autoridade produtiva é concedida.

## Preservação e qualificação

Produto/binários/normas unchanged nesta revisão; sem builds/testes repetidos. Quatro builds/88pares já existentes são evidência de integração, não de isolamento nativo. Imagem882688/ID417614100:1966080:412413/SHA256e3a433c4a6edb565c783fee672bfc82a5794ba44988b123e7a38eb4ec5c9d6b7 preservada. Fixture/originais/autorizações consumidas/EXEs anteriores/HEADWIP/PST0.6.4 intactos. Histórico de retenção/applicationclose/reservas/quiescênciaOS continuaUNKNOWN; fechamento presente conhecido não resolve histórias.

## Autoridade e fontes

- READ-MACHINE-DIAGNOSTIC-SCOPE.CANDIDATE.md/K5 incorporado pelo accepted DECISION-personal-scq-read-machine-k0-k9-20261103.md: critério exato e tratamento deBADKEYSET.
- READ-INCONCLUSIVE-FINITE-CLOSURE.CANDIDATE.md/I1–I4 incorporado pelo accepted DECISION-personal-scq-read-inconclusive-i0-i6-20261103.md: terminal inconclusivo, semRUNNING/readiness e semretrofitting.
- [Microsoft NCryptOpenKey](https://learn.microsoft.com/en-us/windows/win32/api/ncrypt/nf-ncrypt-ncryptopenkey): API/flags/retornos; não promete critério de qualificação do produto.
- [Microsoft5058](https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-10/security/threat-protection/auditing/event-5058) e [5061](https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-10/security/threat-protection/auditing/event-5061): schemas de auditoria, não causa universal ou autoridade original.
- [CryptAcquireContext](https://learn.microsoft.com/en-us/windows/win32/api/wincrypt/nf-wincrypt-cryptacquirecontextw): consultado somente para explicitar por que não transferir semântica deCSP paraNCrypt neste ensaio.
