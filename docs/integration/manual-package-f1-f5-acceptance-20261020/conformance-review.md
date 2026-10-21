# Revisão focal de F1–F5

Resultado: PROPOSED; aceitação dependente PENDENTE. Não há conflito identificado entre a separação semântica de F1–F4 e M0–M8/D3; há lacuna material de representação em F5, expressamente reconhecida pela própria proposta. A condição5 desta tarefa exige layout completo antes da aceitação dependente. Portanto não criar registro ACCEPTED nem inferir aprovação de uma nova representação.

Cláusula exata F5: “Aceitação desta proposta semântica futura NÃO aprovará offsets, tamanho, cardinalidade, magic, enumeração, campos ou APIs por inferência. Persistência da delegação exige contrato versionado byte-exato no participante policy da mesma root, com limites, presença, hash, histórico, predecessor/CAS e coexistência explícitos antes de qualquer escrita.”

A proposta G0–G7 e matriz de offsets completam documentalmente essa lacuna para revisão futura, não para aceitação automática nesta tarefa. Original F1–F5 foi preservado byte-exato em seu caminho e em F1-F5-original-preimage.md. A extensão proposta altera apenas uma futura interpretação PACROOT8/v3; v2 e históricos não foram tocados. Não existe código, parser novo, API, pacote, fonte de trust ou CURRENT criado.

| Condição | Evidência normativa | Resultado |
|---|---|---|
| Identidade, issuer e PACKAGE_SIGNING explícitos | F1/F2; perfil1 §1.3; D3.3 | Conforme semanticamente; fonte factual ausente |
| Autorização policy distinta de assinatura/certificado | F1 e D3.3/D3.4 | Conforme; RSA/PACKAGE/codeSigning insuficientes |
| Validade, revogação, status separados | F1/F2; D3.6 | Conforme; G3 propõe bytes de disposição, sem duplicar status issuer |
| Revisão CURRENT, pins e lifetime | F2/F4; root/C8; issuer/C2 P1/P3/P4 | Conforme como obrigação; nenhum pin/CURRENT qualificado é inferido |
| Associação ao pacote específico | F3 e D3.5 | Confirmação transitória por operação; não delegação global implícita |
| A: evidência Accelerator | F2/G6 | Observação sob fontes/pins; não confiança local |
| B: confirmação independente Browser | F3/D3.2–D3.6/G6 | Cerimônia explícita, separada e vinculada; sem TOFU |
| C: instalação local | F4/D3.8/G6 | BrowserProfile/owner existente, não concedida pela root |
| Layout/presença/versão/offsets/histórico | F5 exclui definição | PENDENTE; G0–G7 e CSV são nova proposta específica |
| Não criar autoridade paralela | F1/F5/G0 | Mesmo arquivo/root/policy/worker; PDG não é standalone store |
| Ordem M1 e quarentena | F4; M1; D3.7 | Preservada, sem KDF antes de trust |
| ED11 | F5/D3.10/G7 | Limites preservados; reserva/viabilidade não aprovadas |
| Aceitação oficial | tarefa §5/§8 | NÃO SOLICITADA como ACCEPTED: condição5 não satisfeita pelo original |

Não registrar aceitação semântica parcial para contornar o gate conjunto solicitado. Registro futuro deve incorporar intervalos/hash exatos de F1–F5 e da extensão efetivamente aprovada, delimitar precedência somente para v3 e preservar todos os históricos. Minuta alguma é ACCEPTED por esta revisão.
