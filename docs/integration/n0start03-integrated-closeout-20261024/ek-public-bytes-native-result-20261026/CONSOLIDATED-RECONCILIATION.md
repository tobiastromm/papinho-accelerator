# EK pública real — reconciliação e próximo gate

A coleta manual foi reconciliada como **PASS_AT_OBSERVATION**: first=NONE, exit=0; bytes públicos e certificado disponíveis e coerentes com os fingerprints anteriores. A chave EK corresponde exatamente à chave do certificado. As assinaturas da cadeia até a root pública Intel foram verificadas, sem instalar confiança ou conceder autoridade. Posse nativa, continuidade da geração NV e revogação completa continuam não demonstradas. E21 PENDING; ED11/Phase6 PARTIAL; autoridades DENY.

## Evidência recebida e preservada

Transcrição exata do anexo preservada em OWNER-TRANSCRIPT.AS-RECEIVED.txt, com SHA256 no manifesto; JSON derivado em OWNER-RESULT.PARSED.json. Os arquivos DER/PKCS1/SPKI são derivados por base64/normalização explícita e identificados individualmente, não evidência de identidade por handle protegido. Não houve nova consulta EK pelo agente.

Script executado pelo owner SHA256 e64cd0d6b4660fcb4594aceeb4cde12aa7808263d93e914ab40cc4d335394279 conferido contra o arquivo existente. Boot237900, operador esperado e hashes de módulo/contexto correspondem ao relato; não viraram admissão fresca ou geração física autenticada. Duração local reportada0.2282733s; não concede freshness alvo. Dispose controlado/processo retornou; fechamento interno permanece UNOBSERVED_NOT_ASSERTED. Retained não foi informado, não inferido False.

## Chave e certificado

- ASN.1 real: PKCS#1 RSAPublicKey DER, RSA2048, expoente65537. OID UNSPECIFIED do wrapper AsnEncodedData não significa algoritmo desconhecido após parse dos bytes.
- SHA256 bruto permanece406874753bd60db238226c888765b49940cc6f0229b8a0296f4ad1547e92aba2. Certificado DER permanece965f56aa0d4c5b5772b423d65d8a06a623109f5828c0eddcb8ec4ae8fa3de761.
- Bytes PKCS1 iguais aos EncodedKeyValue reportados. Normalização independente via OpenSSL da EK e extração do certificado produzem SPKI idêntico. O hash SPKI derivado é separado; não sobrescreve o hash do cmdlet.
- Certificado CA:FALSE, Key Encipherment, EKU2.23.133.8.1 (Endorsement Key Certificate), validade2020-09-04 a2049-12-31. Chave do sujeito RSA; assinatura do certificado ECDSA/SHA256: algoritmos distintos, sem contradição.
- SAN TPM declara fabricante INTC, modelo CML, versão01F4000E; origem AdditionalCertificates permanece histórica. Somente depois da verificação de assinaturas foi demonstrada a relação com a cadeia Intel baixada; origem AdditionalCertificates isoladamente não era prova.

## Cadeia e revogação

Obtidos somente objetos públicos dos endpoints HTTPS Intel indicados em AIA/CRL; não houve instalação no trust store, certificado emitido, acesso privado ou novo serviço. URL da autoridade é proveniência de distribuição, não uma designação normativa automática de trust anchor.

1. Emissor CML_EPID_PROD pid13: [certificado público Intel](https://trustedservices.intel.com/content/CRL/ekcert/CMLEPIDPROD_EK_Platform_Public_Key.cer).
2. Root: [EKRootPublicKey.cer](https://trustedservices.intel.com/content/CRL/ekcert/EKRootPublicKey.cer), SHA2562e1b3ba79af56d758be51697621bc4b9e8cee0983db3e749c55eb9b37c6d2ae0.
3. OpenSSL verify com bundle explícito, stores padrão desabilitados, intermediário separado e purpose any: exit0. Constraints/finalidade EK verificados separadamente. Autoassinatura da root com check_ss_sig: exit0. Verificação temporal usa instante de criação do processo reportado, não freshness nova ou tempo autenticado.
4. [CRL de plataforma](https://trustedservices.intel.com/content/CRL/ekcert/EK_Platform.crl) obtida, assinatura pela root verificada, número8, período2018-08-30 a2049-12-31, sem entradas revogadas nessa CRL. Isso não é conclusão universal de não revogação.
5. CRL do dispositivo CMLEPIDPROD_EK_Device.crl retornou HTTP403. Não foi contornada, substituída ou considerada sucesso. Estado de revogação da EK: UNKNOWN. O verify de cadeia não incluiu uma prova completa de revogação.

Logs/fontes/hashes em CHAIN-ANALYSIS.json, FETCH-intermediate.json e PUBLIC-CHAIN-FETCH.json. O primeiro acesso de rede foi negado pelo sandbox; consulta pública específica foi autorizada fora dele, sem alteração TPM/SCM/ACL. Não é rejeição de aprovação operacional.

## Correções técnicas e regressões

A evidência real revelou limitações antes não exercitadas pelo helper simulado:

- EK é PKCS1, não SPKI. Normalização explícita e hashes tipados separados, sem reinterpretar fingerprints históricos.
- O OpenSSL local rejeitou DER binário via stdin; falha e versão anterior preservadas. Corrigida a análise para PEM armor derivado dos mesmos bytes em memória, mantendo hashes do DER original.
- A finalidade EK é exibida pelo OpenSSL como Endorsement Key Certificate, não apenas OID numérico. Helper agora reconhece ambos os formatos conhecidos e exige Key Encipherment e CA:FALSE.
- Cadeia recebe intermediário em -untrusted separado; não transforma intermediários em âncoras silenciosamente.

Quatro testes com dados públicos REAIS PASS: chave/cadeia corretas, chave divergente, assinatura adulterada e intermediário ausente. Resultado DATA conserva authority=DENY, anchor_designation=NOT_GRANTED, possession=NOT_EXECUTED, revocation=UNKNOWN_OFFLINE. A classe faz análise de arquivos públicos por caminho, não implementa RAW1/original-handle identity ou um novo gate operacional. As fontes anteriores/históricos não foram sobrescritos. Não houve mudança em código produto, launcher ou pins; builds e campanhas inalterados não foram repetidos.

## Próximo gate real

Os bytes públicos não estão mais ausentes. Não é necessário repetir EK metadata/coleta. A identidade pública e correspondência estão resolvidas no escopo observado; prova de posse TPM é a primeira evidência nativa ausente, com confiança/revogação e custódia ainda limitadas.

POSSESSION-READINESS.AFTER-PUBLIC-BYTES.json vincula agora RSA2048/65537, PKCS1, SPKI, certificado e root Intel reais ao desenho anterior. Não inventa AK Name/template, autorização endorsement/AK, sessão ou novo identificador operacional. O comando nativo de posse permanece não executável até os bindings e aprovação exata para eventuais objetos/sessões transitórios. Nenhuma autorização NV ou Q4 é inferida.

O worker permanece QUALIFIED_ISOLATED conforme relatório anterior. L1 exige integração/revisão do participante persistente no owner/CAS real; arquivo sintético não vira autoridade. Custódia efetiva e persistência independente de hardware ainda faltam. Provisionamento NOT_READY; sem definição/incremento/undefine/Clear, sem Q4/Start/Stop/SCM/ACL, limpeza/refund/CURRENT/assinatura/recuperação autoritativa. UNKNOWN/reservas preservados. E17/E18 e E19/E20 são independentes; E22 não concluído.

## Hashes atuais

- offline_public_evidence.CORRECTED.py: `10dc3a327675837a5193a1b2b074506f954114634497b7997e6cf45eb72aae31`
- test_actual_public_chain.py: `ec2f395f329955196a7ac5984102f1d1d64c1e51dc4e90df5be1693903f8bb43`
- RECONCILIATION-CHECKS.json: `319d8eaec02f65bcc60401e2ce00d10a6f41a58e8585707f6f5c62f3d797b40a`
- POSSESSION-READINESS.AFTER-PUBLIC-BYTES.json: `8f6a65bc11a9ddd01f3c05a66f0d1854777b44c83ae007d47c468af316643f79`
