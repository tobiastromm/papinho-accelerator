# Validação e limites da prova

Quatro builds finais MSVC x64, /W4 /WX, Selected3MiB e Legacy2MiB Debug/Release: zero warnings;17+17+16+16=66 suítes finais passam. Flags, SDKs locais e targets preservados, sem rede. Os codecs v1/v2/v3/LST1, issuer/checkpoint, root layout e créditos/perfis ED11 continuam nas regressões.

Novo teste Selected:134 asserções por configuração, incluindo cópia íntegra, CA ausente/SPKI divergente, imagem corrompida, revisão divergente, input/arena reutilizados sem afetar backing, ticket alterado, corrupção posterior da imagem/CA, I/snapshot issuer divergentes, tentativa de substituição, serial obsoleto, dois pins ativos, Stop, quiescência e retenção. Imagem máxima30592 contém4 listeners/4PDG, preservando uma disposição REVOKED como DATA. Não é delegação positiva.

A prova usa código REAL de tickets/pins/unpin/finish/hash/DER no Windows, mas prepared/published/slots da fixture são estados de modelo explícitos, sem arquivo ou root CURRENT factual. A CA é somente o DER público já existente ca.der.b64; seu perfil/autoridade não são qualificados. Não se gera CA/chave/certificado/pacote. O callback component3 já existente é MODEL de componentes públicos, não verificação positiva de signer nem autorização. Os testes kind8 separam modelo e rejeição nativa real de assinatura sintética. Nenhum dado sintético vira fonte E0.

16 execuções nativas locais em Selected Debug/Release: issuer history/snapshots/pendingUNKNOWN/pendingStop e D3 reject/conflict/UNKNOWN/Stop. Arquivos de teste novos sob build/v3-backings-20261021/native; não são instalação produtiva ou CURRENT v3. O worker é o existente; nenhuma rede/assinatura privada. Logs e resultados preservados.

Ciclo quiescente do novo teste faz unpin após terminar usos e confirma finish; o ciclo retained confirma finish DENY e deixa intencionalmente memória do owner retida até o término do teste. Não é fechamento qualificado por exit nem liberação de retenção por timeout. A coexistência de dois pins protege o mesmo backing, não novo pool ou tabela.

Falha inicial Selected Debug preservada: harness com macro main colidindo na inclusão v3, falta da definição offline e aviso fopen. Correção oficial limitada a harness/CMake: entry de fixture separado, definição offline explícita e fopen_s. Sem alteração de predicados funcionais, flags ou backend. Suplemento de casos máximos/pins também aprovado antes de aplicar. Legacy inicial passou. Rounds finais completos repetidos após alterações, zero warnings. Não houve negativa oficial ou fallback de toolchain.

Não qualifica CURRENT positivo, CA/perfil factual, native signer positivo, fonte E0, commit v3 operacional, NT4, box/ISA, pacote/instalação, assinatura, admissão ou ED11 total. Logs antigos não foram sobrescritos; novas árvores build v3-backings somente, sem clones/worktrees.
