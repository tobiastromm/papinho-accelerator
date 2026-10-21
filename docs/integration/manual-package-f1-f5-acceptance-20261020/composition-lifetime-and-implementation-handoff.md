# Handoff sem implementação

A representação mínima proposta acrescenta somente delegação pública de policy:96 bytes header/trailer e384 por listener delegado. Confirmação independente e autorização local ficam fora da root; captura/owner Browser dependem de integração própria. Observação Accelerator deriva da root/issuer/TLS reais e é vinculada à operação/pacote, sem novo token ou recibo persistente de trust.

A proposta usa PACROOT8/v3, PDG1 e CP3, update-only a partir de v2/v3. Magic/cardinalidade/códigos novos são PROPOSED, não escolhas aceitas por referência. Nenhum código pode inferi-los de F1–F5. Matriz enumera integralmente os offsets fixos; fórmulas do contrato enumeram regiões/digests variáveis. Certificado signer e histórico permanecem no issuer existente, sem duplicação de DER/status em catálogo.

Gate imediato: owner revisar conjuntamente F1–F5 e G0–G7, especialmente discriminator v3, uma linha por listener Secure (D<=K<=4), ausência D0, disposition APPROVED/REVOKED e persistência apenas da delegação. A ausência de coluna de confirmação/instalação é deliberada. Não é necessário reabrir M0–M8 ou D3.

Depois da aceitação exata e oficial, preparar patch delimitado do codec/participante policy na root existente. Adoção v2 para v3 exige CURRENT, predecessor integral, CP2 e pin factuais; decode/fixtures não os qualificam. Completar aquisição de certificado/issuer/policy/TLS, revisão da aprovação e origem independente, preservando API de produção sem injeção de boolean de trust. Não implementar neste trabalho.

## Compatibilidade proposta

| Entrada/base | Tratamento |
|---|---|
| ROOT06/BODY históricos | Bytes/leitores preservados; não são base v3 deste perfil |
| ROOT8/v1, CP1 | Preservados; sem migração direta implícita |
| ROOT8/v2, SNT1, CP2 | Preservados; base v3 só com adoção explícita autorizada |
| ROOT8/v3, PDG1, CP3 | Proposta sem leitor/worker; produção DENY |
| LST1/D3/SNT1/issuer carrier | Fragmentos históricos mantidos, sem substituição de autoridade |
| Desconhecido/corrompido | Rejeitar; sem fallback/truncamento/rehash reparador |
| .papacc PACCMP02 | M0–M8 intactos, sem novo membro ou signed-attestation |

## Critérios futuros, não executados

Round-trip, limites, offsets, hash, truncamento e trailing bytes; policy/listener/signer/issuer vinculados; revogação de delegação separada de certificado; D0 nega; retirada de linha não restaura ancestral; event-prefix/high-water preservados; status/CURRENT ausentes e pin inválido negam; confirmação de outro pacote/operação não serve; KDF0 antes de trust; revalidação antes de release; Stop/UNKNOWN retêm fontes/buffers/crédito; coexistência/snapshots pinned e v2/históricos intactos. Estes são critérios propostos, não resultados PASS. Não executar builds/testes nesta tarefa.

## ED11

Runtime novo0. V96..1632 serializados; imagem20608..30592;3/4/6 buffers máximos91776/122368/183552. Acréscimos hipotéticos4896/6528/9792 sobre arrays v2 não são composição ALL nem reserva. Objetos novos não foram compilados/medidos: source/attestation/leases/histórico/DER/stack/provider/copies/retidos ainda a censar. Não somar PDG de novo se já incluído na imagem. Cenário anterior3594733 é conservador não compartilhado incluindo provider de fixture, não pico operacional. Nenhuma reserva reduzida ou integração ativada. Provider/kernel UNKNOWN; ED11 total PARTIAL; limites2MiB/Windows3MiB explícito e quotas individuais intactos.
