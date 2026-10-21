# Validação independente

44 execuções diretas de binários existentes: Selected/Legacy, Debug/Release,11 testes por configuração, todas exit0. Incluem codecs v1/v2/v3, kind8 modelo, kind8 native gate, censo e perfis/créditos ED11. Comandos, SHA256 dos executáveis e logs constam de regressions.json/evidence.zip. Nenhum teste operacional de dispatch v3 foi criado ou declarado positivo.

Não houve novo build porque nenhum fonte foi alterado e a integração dependente está em STOP. Os quatro builds finais anteriores são evidência histórica, não compilação desta tarefa; warnings desta tarefa NOT_APPLICABLE. Não se declara correspondência binário/fonte por compilação nova: executáveis foram reexecutados com hash medido nesta campanha. O manifesto histórico fixa71 artefatos de fontes/logs, verificados integralmente sem divergência; ele não pinava os executáveis de teste. A verificação inicial do script recusou corretamente a ausência desse pin antes de executar testes; script inicial preservado. A revisão do script passou a registrar hashes atuais sem alegar pin histórico ou rebuild.

Kind8 modelo usa assinatura sintética; native gate chama verificador público real que deve rejeitar essa assinatura. Os fixtures são os dois DER-base64 existentes; nenhum arquivo de chave/certificado/pacote novo foi gerado. Positivo nativo de signer/CURRENT/pin v3 não foi fabricado. Os casos CAS/Stop/retenção nativos anteriores não foram reexecutados nesta campanha: evidência histórica apenas, não qualificação do dispatch não integrado. A matriz preserva essa distinção.

Nenhuma execução de rede, assinatura privada real, Browser ou quatro LAB. Nenhum log histórico LastTest.log foi sobrescrito; execuções diretas gravam apenas TEMP antes da entrega documental.
