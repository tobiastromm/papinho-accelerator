# STO — proposta focal de transporte sem KSP

## Resultado

SCQ-TRANSPORT-ONLY-NO-KSP.CANDIDATE.md: PROPOSED, não aceita. SHA256 `0702a2e733024118bdad7c0993b4993e20cf44e825a872ad50245e8fd2b45d30`. TO0–TO9 limitam a dispensa de key ao novo propósito privado de transporte, exigindo ausência canônica e proibindo KSP. Demais perfis preservam o seletor obrigatório.

OHI vincula o destinatário à chave MACHINE fixture; B regra3 preserva essa exigência. SSO-B permite readiness fechada, mas não dispensou key. Por isso é necessária a exceção expressa. A proposta preserva autenticação bilateral, originais, consumo, receipt/close, quotas e ausência de autoridade produtiva; não edita decisões accepted, N0 ou formatos ROOT/CURRENT.

## Artefato e qualificação

Selected/Release revalidado: `90123fbadc3d7e4db1e5f48ea2b7fe85f6e78f61f6adee083b00d1288b951874`, 520192 bytes, somente candidato selecionado para planejamento. Não instalado/atestado e ainda incompatível com o novo propósito. Não foi substituído. Código fonte conferido contra ONB: 56 arquivos sem divergência. Nenhum build/teste novo ou efeito nativo foi necessário para documentação; evidências ONB históricas são reutilizadas sem reclassificá-las como prova bilateral real.

## Quantitativos e lacunas

Ver STO-QUANTITATIVE-ANNEX.json: guard124, limite128 e administração1024 preservados. Payload distinto3176 bytes não é envelope total. Censo estático Selected1992700 e Legacy1824132 bytes não demonstra headroom físico/commit. Sem KSP ainda há custos públicos RNG/hash, SCM, pipe e kernel. Validade, duração global, freshness, custos ativos e fechamento continuam sem valores justificados SCQ; nenhum valor Browser/LAB foi copiado. Não foram realizadas medições nativas nesta tarefa.

## Gates concretos

1. Aprovação explícita do SHA desta proposta; até lá, key obrigatório no código.
2. Registro normativo e integração offline integral, quatro configurações/regressões, censo e hashes derivados.
3. Nova seleção explícita do artefato derivado; completar política quantitativa e continuação nativa/fontes originais.
4. Pedido operacional byte-exato com endpoint, lifecycle SCM/Start/Stop e parent separados, gates frescos e autorização própria; executar somente depois dessa autorização.
5. Prova bilateral real não demonstra custódia/isolamento CNG. Estes e a cerimônia NEW_INSTALL permanecem frentes operacionais separadas com seus próprios gates.

Pedido operacional NOT_READY; nenhum comando executável emitido. Próxima continuação: NEXT-STO-TRANSPORT-ONLY-OFFLINE.md. E21 PENDING; ED11/Phase6 PARTIAL; DENY. Preservados históricos, reservas, chave fixture retida e PST0.6.4.
