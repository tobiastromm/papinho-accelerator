# Auditoria de ownership e reúso

TASK=PACC-PHASE6-CLIENT-SDK-OWNERSHIP-AND-REUSE-AUDIT-1. Auditoria estática em2026-10-20, sem refatoração ou qualificação operacional.

PapinhoPACC já é compartilhado e protocol-only. A fonte autoritativa é Accelerator/include/papacc/pacc e src/protocol; tools/package-papinhopacc.cmake gera o pacote usando templates em tools/papinhopacc-package. Browser/third_party/papinhopacc é consumo imutável. As20 entradas do ZIP1.1.0, snapshot e respectivas fontes canônicas coincidem byte-exact; nenhuma divergência wire encontrada nesse conjunto.

Provisionamento é diferente: a gramática M2/M3 tem implementações nos dois projetos. Browser papinho_papacc_content.c e CONTENT-ORIGIN.json declaram port do manual_package.c Accelerator. Há repetição de normalização de senha, parâmetros M1 e domínio de assinatura. Primitivas BearSSL e OpenSSL são backends diferentes legítimos, não foi comprovada reimplementação de primitivas próprias. A gramática/política comuns precisam convergir antes de novas extensões locais.

Recomendação PROPOSED: SDK composto, preservando PapinhoPACC protocol-only, com componente complementar de provisionamento e adapters opcionais. Nome final, API/ABI, repositório e release continuam decisões futuras. Não criar trust store, CA ou SDK high-level Phase12.

| ID | Severidade/impacto | Evidência/autoridade | Disposição |
|---|---|---|---|
| O1 | Alta, manutenção/ownership; não vulnerabilidade demonstrada | ZIP/TLV/DNS/UTF8/CRC repetidos em manual_package.c e papinho_papacc_content.c; port explícito. PEADR0011rev2, Regra central/O que conta/Componentes existentes | Responsabilidade compartilhada agora comprovada. Parar novas extensões locais comuns; designar owner antes de continuar. Não inferir intenção/conhecimento histórico ou exceção normativa a partir da aprovação funcional anterior |
| O2 | Alta, escopo arquitetural | ADR0025 Decisão limita papacc_protocol a protocolo, sem policy/autorização/runtime | Alternativa de colocar provisão no core existente exige alteração normativa; não está autorizada |
| O3 | Alta, qualificação incompleta | Browser papacc-d3-independent-import CLOSEOUT registra compilação dos novos helpers, mas novos testes não executados após negativa oficial | Preservar REVIEW-OBJECTION.md e STOP; não repetir geração de fixtures por outro mecanismo. Aprovação desta auditoria não resolve a objeção |
| O4 | Alta, segurança se integrado incorretamente | UI simples retorna SIGNATURE_ONLY e nega antes senha; pipeline Accelerator nega fonte ausente em TRUST | M1/D3/F1-F4: assinatura, confiança e autorização local distintas; manter gates |
| O5 | Média, extração/portabilidade | Browser material.h depende credential_store.h Win32/PST; Accelerator validation.h inclui config_root8_v3.h privado | Não publicar esses headers como core comum. Separar slices/resultados de adapters root/store |
| O6 | Média, divergência de APIs locais | AEAD/normalização/outputs em falha/Stop diferem. Browser usa q privada; Accelerator copia após tag para staging do pipeline | Não comprova divergência de formato; M1 admite quarentena interna. Contrato unificado de outputs/aliases/lifetime e testes diferenciais antes da extração |
| O7 | Alta, ativação/ED11 | PDG1/M1 registra cenário conservador3625925, excedente480197 sobre3145728; Browser censo parcial | Não é pico medido nem prova universal de inviabilidade. Nenhuma reserva/ativação aprovada; Windows3MiB Accelerator não aumenta orçamento Browser |
| O8 | Média, distribuição | PACC ZIP sem LICENSE/NOTICE; PBKDF2 Browser derivado NSS com aviso MPL2; BearSSL tem LICENSE.txt | Fechar termos próprios/proveniência/obrigações dos artefatos antes de release, sem inferir licença concedida |
| O9 | Média, risco futuro | PAE1/pareamento helpers só no Accelerator; duplicá-los no Browser seria nova cópia | Extrair parte comum client-relevant sob P0-P9/T0-T7; RNG/claim/admin/issuer continuam servidor |
| O10 | Baixa, geração reprodutível | Recipe copia fontes/cria ZIP/hash, sem garantia declarada de timestamps determinísticos ou manifest interno de entradas | Snapshot atual íntegro não prova regeneração bit-reproducible. Propor melhoria futura sem regenerar pacote agora |

Não foi demonstrada divergência de PACC wire, relaxamento mTLS ou ciclo efetivo no pacote atual; não é prova exaustiva de segurança. O runtime Browser mistura HTTP, Win32 e lifecycle, portanto não deve ser transferido inteiro ao SDK. Duplicação de parser é factual; adapters de providers distintos não devem ser eliminados como se fossem a mesma implementação.

Já existe obrigação normativa multi-consumer em BrowserADR0007, compartilhamento wire em ADR0025 e shared-owner-first em PEADR0011. A lacuna é concreta: owner/módulo/API/distribuição da provisão compartilhada. ADR candidate preenche somente essa lacuna; não modifica M0-M8/D3/F/G.

Builds/testes/rede NÃO executados nesta auditoria. Resultados citados são históricos e têm limites explícitos. Hash não é CURRENT. Nenhuma fonte funcional escrita, nenhuma chave/certificado/pacote ou ACL criado/alterado. Quatro LAB sem acesso/cleanup. Gates operacionais permanecem negados.
