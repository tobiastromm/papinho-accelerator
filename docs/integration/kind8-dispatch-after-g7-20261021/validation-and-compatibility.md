# Validação e compatibilidade

4 builds MSVC x64 /W4 /WX passaram na primeira rodada, Selected e Legacy Debug/Release. Selected19+19, Legacy16+16=70 suítes; zero warnings. Mesmo toolchain/SDK local/candidato, sem rede. Incluem codecs v1/v2/v3/LST1, issuer/checkpoint, root layout, G7 backing, kind8 público anterior e perfis/créditos ED11.

Novo modelo do dispatch:78 asserções/config. Native negative:77/config. Ambos usam o dispatch e G7/pin/hash/DER reais. Um único executable de teste define papacc_root8_signer_data como stub explícito de encaminhamento MODEL (model_calls3); o outro usa biblioteca pública nativa real (model_calls0) e deve rejeitar o certificado sintético de3 bytes já existente. Nenhum macro/stub MODEL compila na biblioteca de produto/canônica. O componente público existente mantém seus próprios testes de perfil/assinatura modelo e rejeição nativa real de assinatura sintética.

Portanto, sucesso positivo aqui prova encaminhamento e guards DATA em MODELO, não aceitação nativa de certificado/perfil ou autoridade positiva. Nenhum certificado/signer nativo válido foi fabricado para fechar essa lacuna. A origem administrativa e a qualificação positiva nativa permanecem gates independentes. A CA usada é somente ca.der.b64 existente; nenhum novo arquivo de fixture/CA/chave/certificado/pacote criado, nenhuma assinatura privada.

Cobertura nova: backing íntegro, CA ausente/corrompida, packet corrompido, row externo/fora do alinhamento, clone externo, kind desconhecido, contexto/versão ausentes, pin alterado/obsoleto, backing ausente, R/I divergentes, arenas alias, scratch sobre owner, reserva inferior insuficiente, erro do componente sem fallback, Stop dos workspaces/owner, authority_lost e unresolved. Quatro linhas/listeners e status REVOKED como DATA são preservados. Stop/retained nega novas chamadas, conserva memória já emprestada e finish permanece negado; o teste não libera retenção por timeout/exit.

16 casos nativos locais Selected Debug/Release: issuer history/snapshots/pendingUNKNOWN/pendingStop e D3 reject/conflict/UNKNOWN/Stop. Usam worker/file/CAS históricos, arquivos de teste novos em build/kind8-after-g7-20261021/native. Contexto administrativo é sintético explícito; não prova CURRENT/commit v3 operacional ou bootstrap produtivo. Native hash/DER/canonical pins também são executados nos novos testes, com publicação de slot MODELO, não root CURRENT factual.

| Compatibilidade | Evidência/disposição |
|---|---|
| ROOT06/BODY0.5 e issuer/high-water/history/revocation | fontes byte-exatas preservadas; regressões históricas passam |
| ROOT08/v2 e CP2 | codec/layout inalterado; testes v2 e native history passam |
| PACROOT8/v3/PDG1/CP3 | codec/layout inalterado; somente nova rota DATA consumindo G7 |
| Legacy/default2MiB | não compila rota kind8 nova ou campos privados;16 suítes/config, fixo inalterado |
| Windows3MiB explícito | rota e contexto privado v1, sem seleção implícita ou novas quotas |
| E0/M1/Browser/PST/wire | fontes e snapshots distribuídos intocados; todos gates positivos fechados |
| box/ISA/NT4 | não qualificados por estes builds MSVC x64 |

G7/CAS/Stop/Model E não são alterados pelo resultado DATA. Nenhum novo build ou fallback foi usado para contornar falha; não houve falha ou negativa oficial nesta campanha. Artefatos históricos e logs de campanhas anteriores preservados, sem sobrescrever LastTest.log antigo.
