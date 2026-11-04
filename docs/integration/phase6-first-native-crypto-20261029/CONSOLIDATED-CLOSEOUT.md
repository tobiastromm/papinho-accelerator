# Phase 6 — primeira instância criptográfica nativa isolada

**QUALIFIED_ISOLATED** para a composição criptográfica. NORMAL operacional **DENY**; ED11/Phase 6 **PARTIAL**. HEAD e WIP anterior preservados; PST 0.6.4 inalterado.

## Código integrado

A nova jornada `papacc.issuer.native_ephemeral_composition` atravessa o corpo da entrada CA_INITIALIZE, o TBS de CA já existente, borrow/handoff ao backend CNG NORMAL, emissão, histórico PUBLISHED v2 e re-read verificável. As substituições da cerimônia/owner existem exclusivamente na tradução de teste. O produto continua usando o guard nativo fechado; nenhuma chamada automática de startup/CLI foi adicionada.

A prova real revelou que `CryptExportPublicKeyInfoEx` devolve rsaEncryption sem parâmetros no equipamento observado. O perfil aceito requer NULL DER explícito. O novo helper de exportação **pública** `papacc_issuer_x509_key_public` usa `CERT_NCRYPT_KEY_SPEC`, preserva módulo/expoente e normaliza somente parâmetros ausentes para NULL antes de validar RSA2048/65537. Parâmetros presentes incorretos continuam rejeitados. O backend NORMAL reutiliza esse helper no public_match, mantendo o backing público no workspace antes do DER/hash. Não houve novo formato, algoritmo, relaxamento de perfil ou autoridade.

O mock antigo devolvia um CERT_PUBLIC_KEY_INFO com pointers externos. Foi corrigido para copiar o backing público, reproduzindo o contrato real e exercitando o mesmo helper. Isso não constitui fonte operacional.

## O que é real e o que é simulado

| Etapa | Evidência |
|---|---|
| Plano NEW_INSTALL e confirmação/identidade/alvo | Fixture governado em memória; não autorização real |
| CA_INITIALIZE | Corpo real incluído na tradução de teste; guard/contexto substituídos somente ali |
| Criação de chave | CNG Microsoft Software KSP real; RSA2048; nome NULL, chave efêmera; sem MACHINE_KEY ou persistência |
| Chave do cliente/CSR | Segunda chave efêmera distinta; CSR PKCS1/SHA256 assinado e verificado realmente |
| CA | TBS/profile do produto, autoassinatura CNG e encoding CryptoAPI na fixture; assinatura pública verificada |
| Custódia/lifetime | Borrow/transfer/return reais sobre chaves efêmeras; tamanho/export policy consultados no CNG; ACL e custódia operacional persistente não demonstradas |
| Issuer NORMAL | Código real de core/backend, assinatura CNG real e validação pública real; fontes/autoridade administrativa modeladas |
| CAS e PUBLISHED v2 | Callback de armazenamento de teste, codec v2 real com SHA256 real, DER efetivo, intenção/ack e re-read em memória; nenhuma publicação de arquivo/CURRENT |
| Revogação/M7 | Core/codec reais e API effect-only C2; cohort de Sessions modelado, selecionado pelo digest do DER realmente emitido |

Cada jornada efetua três assinaturas reais: CSR do cliente, autoassinatura da CA e certificado emitido. A publicação e a revogação não repetem a assinatura. A CA não assina a CSR do cliente. Não se exporta material privado nem se instala certificado em trust store. As chaves e provider efêmeros são liberados explicitamente no caminho positivo; isso não é cleanup de recursos operacionais históricos.

## Qualificação

- Selected/Legacy Debug/Release: PASS, zero warnings.
- 14 CTests/configuração: 56 execuções PASS, incluindo quatro jornadas criptográficas novas.
- Modelo CNG anterior: 2.718 verificações/configuração preservadas. CAS/worker/PUBLISHED previamente qualificados não foram reimplementados nem suas campanhas inalteradas repetidas.
- Jornada nova: 67 verificações/configuração; três assinaturas CNG reais, CSR/leaf verificados pelo backend e CA verificada explicitamente.
- Negativos novos: CA/chave incompatível, assinatura CA adulterada, exportação pública sem espaço/alias, chamada em outro thread, epoch divergente, histórico alterado, replay de ack e emissão após Stop.
- Falhas parciais, PENDING, concorrência, retenção/UNKNOWN e gate produtivo permanecem cobertos pelas regressões afetadas. Não se converte falha nativa real não exercitada em PASS.

M7 agora compõe a credencial real emitida com revogação em histórico simulado: invalida uma Session, solicita Stop da operação dependente, mantém borrower retido e preserva outra credencial do mesmo Principal. O receipt mantém close_proven=0/quiescence_proven=0. Isso não demonstra autenticação real, admissão, rede, fechamento ou quiescência produtiva.

## Limites e caminho restante

A chave efêmera não qualifica a chave persistente MACHINE KSP, ACL, operação sob token distinto, custódia durável, restart ou capacidade provider/kernel. Esses custos permanecem UNKNOWN; quotas e headroom não foram alterados. A proteção da chave persistente segue os contratos accepted; não se infere a partir deste teste.

O guard `personal_native_ceremony_ready` permanece falso no produto. A factory NORMAL não passa a emitir provas originais por esta jornada. Não houve CA/root operacional, assinatura produtiva, CURRENT, serviço/rede, SCM/ACL, TPM/Q4, refund ou recuperação.

A primeira operação real futura é NEW_INSTALL/CA_INITIALIZE pessoal/local especificamente autorizada, **somente depois** de qualificar a admissibilidade nativa, alvo/operador/identidade operacional/custódia, reserva e continuidade originais. Não está pronta para execução nesta entrega. A sequência deve distinguir criar chave, obter certificado da mesma geração, publicar/confirmar e adquirir NORMAL; nenhum fixture pode suprir essa autoridade.

O próximo trabalho técnico é ligar essas fontes nativas ao mesmo owner sem callbacks permissivos, mantendo os gates fechados enquanto faltarem fatos. E8/enrollment e D4/D5/composição Browser permanecem frentes independentes; não exigem retomar TPM por padrão. Ver `NEXT-NATIVE-CEREMONY-AND-ORIGINAL-INSTANCE.md`.

## Evidências

`QUALIFICATION.json`, logs de build/CTest/LastTest, `SOURCE-CHANGES.json`, `FUNCTIONAL-DELTA.patch` e `ARTIFACT-HASHES.json` registram resultados e hashes literais. Nenhum ADR accepted foi modificado.
