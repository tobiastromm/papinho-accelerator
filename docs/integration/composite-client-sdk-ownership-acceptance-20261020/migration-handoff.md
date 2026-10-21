# Handoff — API compartilhada e migração

Arquitetura aceita não significa migração autorizada. PapinhoPACC continua protocol-only; módulo complementar é owner do domínio de provisão comum, com fonte canônica inicialmente no Accelerator. Browser é consumidor e mantém UI, confirmação independente, autorização de instalação e owners concretos locais. Root/issuer/policy permanecem no Accelerator.

Próxima tarefa deve definir a API bounded C89 e os contratos de dados/evidência/ownership antes da extração. Não publicar os headers atuais que incluem credential_store.h/Win32 ou config_root8_v3.h privado. Delimitar core de M2/M3/TLV/normalização/política de suite e adapters vetted; não converter callbacks sintéticos em trust. As APIs exatas não são aprovadas por esta entrega.

Preservar código, preimagens, testes, vetores, contratos, licenças e notices. Extrair sem reescrita desnecessária, provar equivalência, integrar Browser no mesmo pipeline simple/advanced e só então remover cópias aprovadas. Wrappers temporários precisam gate de equivalência/remoção antes da distribuição conjunta. Não prolongar consumer-first com novas extensões comuns.

Testes futuros distinguem contrato/API, crypto nativo, fixtures sintéticas, autoridade factual e qualificação operacional. Nenhum teste é executado aqui. A objeção oficial histórica Browser continua impedindo aquela geração de fixtures até revisão própria. Não contornar via outro gerador.

Direção aceita: API/ownership → extração comum → equivalência → Browser consumidor → remoção após regressões → distribuição versionada. Cada patch funcional exige autorização e revisão oficial próprias. Não antecipar SDK Phase12, rede, nova CA/trust store/protocolo, licenças presumidas, quotas ou release. Quatro LAB permanecem sem acesso/cleanup.

M1, segredo em quarentena, recheck final, CURRENT/pins/fontes factuais, Stop/retidos/ModelE e ED11 continuam gates. Sem censo completo, ativação nega; provider/kernel UNKNOWN. NEXT=SHARED_PROVISIONING_API_AND_MIGRATION_IMPLEMENTATION, com implementação ainda não autorizada por este registro.
