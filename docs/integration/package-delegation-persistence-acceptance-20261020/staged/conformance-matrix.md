# Revisão conjunta — F1–F5 e G0–G7

Resultado normativo: conforme no escopo delimitado; nenhum conflito material identificado. Aceitação somente após revisão oficial dos bytes exatos e incorporação verificada. Não é prova de implementação, operação, trust factual ou viabilidade de ativação.

| Condição da tarefa | Cláusulas/fontes | Constatação |
|---|---|---|
| Signer, issuer, purpose, delegação explícita | F1/F2; G3; perfil técnico1 §1.3; D3.3 | Policy identity e signer/certificado/SPKI/authority/generation/CA distintos; PACKAGE_SIGNING explícito; RSA/PACKAGE/codeSigning não autorizam |
| Validade, status e revogação | F1/F2; G3/G5 | Delegação e certificado separados; APPROVED fora de validade nega; REVOKED não apaga história nem revoga certificado implicitamente |
| CURRENT, revisão, pins/lifetime | F2/F4; G4/G7; issuer/C2 P1/P3/P4 | Leitura decodificada não é autoridade; pins protegem memória; revisão alterada exige nova validação; fontes externas requerem owner/lease |
| Pacote/operação | F3; G6; D3.5/D3.6 | Digest completo imutável + SPKI/contexto esperado + confirmação independente por operação; não reutilizável como trust global |
| Três autoridades Browser | F3/F4; G0/G6; D3.2/D3.4/D3.8 | Evidência Accelerator não é confirmação Browser ou direito local de instalar; confirmação transitória não persistida na root |
| Magic, versão, endian e presença | G1/G2/G3/G4 | PACROOT8/v3; PACPDG01/PACPDL01/v1; PACR8CP3/v3; externos LE; fragmentos históricos preservam seu endian; valores discriminados |
| Header e offsets | G2 e matriz frozen |50 campos cobrem[0,512), sem gap/overlap; PDG header9 campos[0,64); linha22 campos[0,384); CP15 campos[0,512) |
| Cardinalidade e tamanho | G1/G3 |N,B1..4; K1..N; D0..K, no máximo uma linha por listener Secure; D0 nega trust; imagem20608..30592, PDG96..1632 |
| Padding, ordem, corrupção | G1/G2/G3/G5 |Contiguidade, ZERO tails/reservados, ordinal estritamente crescente, unknown/truncamento/trailing/overlap/hash inválido rejeitam imagem inteira |
| Integridade | G4 |PDG e SNT trailers; C3 prefixo até Q; Hcp3 CP[0,480); W3 até fim CP; sem domínio circular; SHA não é aprovação/freshness |
| Predecessor e CAS | G0/G4/G5; root/C8 |Update-only v2/v3, imagem predecessora integral + CP + CURRENT/pin; conflict/UNKNOWN sem fallback ou retry destrutivo |
| R/A/P/I | F1/F2; G2/G4; issuer/C2 accepted |Distintos; publicação/epochs pós-commit; I não inventado para delegação; P só preservado com projeção operacional idêntica comprovada |
| Históricos | F5; G0/G2/G5 |ROOT06/BODY0.5/ROOT08v1/v2/CP1/CP2/SNT1/LST1/PACCMP02 intactos; reservado v2 não reaproveitado; sem downgrade/migração automática |
| M1 | F4; G4/G6; M1 e D3.7 |STRUCTURE→SIGNATURE→TRUST→KDF→AEAD→CONTENT→RELEASE; recheck final; conteúdo privado até validação completa |
| Stop/Model E/ED08 | F4; G4/G7 |Retenção/borrowers/créditos preservados até quiescência; sem overwrite pinned, timeout-as-destruction ou Stop-as-cancellation |
| ED11 | F5/G7 e W1–W6 |Subtotais serializados e arrays são aritmética documental; não reserva; runtime novo0, provider/kernel/ativação desconhecidos, total PARTIAL |
| Implementação/produção | G0/G7 e registro proposto |Nenhum hunk funcional aceito; sem código, testes, CURRENT, assinatura, CA/pacote, rede, admissão ou release |

## Encerramento da lacuna da revisão anterior

A revisão anterior recusou aceitação conjunta porque F5 expressamente não definia layout. O owner agora condicionou aceitação EXATAMENTE de F1–F5 e da proposta G0–G7 que completa o gate; não há aceitação por inferência. G1 escolhe explicitamente cardinalidade do primeiro subconjunto v3, enquanto F1 isolado não escolhia. F5 continua proibindo qualquer outro campo/API/layout inferido. O registro delimita essa composição e preserva as propostas como histórico.

## Conferência documental de limites

Para N=B=K=1,D0: O2048,S864,U2912,V96,L672,J16384,Q20064,T20608. Com D1: V480,T20992. Para N=B=K=D=4: O6656,S3168,U9824,V1632,L2208,J16384,Q30048,T30592. Header/presença/campos fixos têm limites e bitmasks inequívocos. Matriz de offsets não precisa conter linhas para cada digest variável: suas posições exatas constam nas fórmulas G1/G3/G4.

Conferência de CSV/aritmética é revisão documental, não teste de codec. Nenhum binário/CSR/certificado/pacote/chave foi produzido ou executado. Sem builds/testes/rede.

## Readiness permanece negada

As fontes factuais de delegação e confirmação ainda não existem no produto. Ambiente CURRENT/autoridade positiva e composição de ativação continuam gates separados. Nenhum exemplo/manifesto/história/hash/contador pode suprir isso. A aceitação normativa fecha apenas a escolha de contrato; o próximo trabalho precisa implementação exata, aquisição de fontes e prova de lifetime/ED11 antes de efeitos.
