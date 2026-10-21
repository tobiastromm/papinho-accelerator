# PROPOSED — revisão exata do fragmento privado de listeners

NÃO ACEITO, NÃO IMPLEMENTADO. A decisão textual F5 mantém codec/layout/integridade pendentes. Proposta de seção LST1 na MESMA transação/root, não arquivo/registry de autoridade novo. A versão da envelope root que a conteria ainda exige aceitação; não reinterpretar BODY0.5, ROOT06 ou trailer512. A seção abaixo não é por si uma configuração completa ou prova de CURRENT. Não persistir dumps de struct/size_t/ponteiros/padding.

## Encoding proposto

Inteiros unsigned little-endian explícitos. Header128 + count*record512 + trailer32 SHA256; count0..4; tamanho exato160+512*count (máximo2208). Digest cobre header/records completos; SHA256 usa adapter existente aprovado, não algoritmo novo. Integridade não é assinatura, freshness, fencing ou autorização. Counts, comprimentos, campos reservados e flags validados antes de acessar conteúdo.

| Offset header | Bytes | Campo |
|---|---:|---|
| 0 | 8 | magic ASCII PACLST01 |
| 8 | 2 | versão1 |
| 10 | 2 | header128 |
| 12 | 4 | total exato |
| 16 | 2 | count0..4 |
| 18 | 2 | recordbytes512 |
| 20 | 4 | presence: root/R/A/P bits0..3 obrigatórios, I bit4 opcional; demais rejeitados |
| 24 | 32 | domínio da root factual |
| 56 | 8 | R do snapshot de resolução |
| 64 | 8 | A do snapshot factual |
| 72 | 8 | P do snapshot factual |
| 80 | 8 | I se presente; zero reservado para ausência explicitamente marcada |
| 88 | 8 | predecessor factual correspondente, não escolhido pelo issuer |
| 96 | 2 | algoritmo integridade1=SHA256 |
| 98 | 30 | zero reservado |

Estes R/A/P/I identificam a proveniência da resolução, NÃO publicam nem ressuscitam contexto runtime. A envelope precisa vincular esses dados e o predecessor CAS à revisão committed real. Ausência de prova de qualquer vínculo exigido nega aplicação/recuperação; campos textuais não os escolhem.

| Offset record | Bytes | Campo |
|---|---:|---|
| 0 | 4 | presence: identity/binding/port/profile/authkind/policy obrigatórios; security condicional Secure; nenhum bit desconhecido |
| 4 | 1 | profile explícito Secure1/Legacy2; zero inválido |
| 5 | 1 | admission OPEN1/CLOSED2 |
| 6 | 1 | binding ALL1/SELECTED2 |
| 7 | 1 | authkind NONE0/CERTIFICATE_IDENTITY1, com bit de presença obrigatório; outros rejeitados |
| 8 | 2 | port1..65535 |
| 10 | 2 | id_length1..64 |
| 12 | 2 | security_ref_length:1..64 Secure,0 Legacy |
| 14 | 2 | auth_ref_length0..64, exigido quando kind requer provider aprovado |
| 16 | 2 | policy_ref_length1..64 |
| 18 | 2 | interface_count0..8; ALL exige0,SELECTED exige>0 |
| 20 | 4 | zero reservado |
| 24 | 64 | listener_id ASCII, tail zero |
| 88 | 64 | security_ref opaca, tail zero |
| 152 | 64 | auth_ref opaca, tail zero |
| 216 | 64 | policy_ref opaca, tail zero |
| 280 | 32 | identidade da policy na mesma root |
| 312 | 32 | digest do modelo BODY0.5 correspondente já canonicalizado pelo codec existente |
| 344 | 40 | zero reservado, sem autoridade ou extensões inferidas |
| 384 | 128 | oito entradas16: validU8 +7zeros +PersistentIDU64; somente entradas count presentes; demais zero |

Refs carregam intenção/contexto não secreto. Não armazenar private key/password/token/trustDER raw. Runtime interface_instance_id/index/scope/socket NÃO persistidos como PersistentID. Identidades locais não são globais.

## Regras e incompatibilidades preservadas

Rejeitar magic/version/length/count/enum/presence desconhecidos, zero profile, duplicatas de id/interface, tail/reserved nãozero, digest inválido/truncamento/trailing bytes, refs sem alvo autorizado e model digest divergente. Nenhuma aceitação parcial. Cardinalidade da seção não concede capacidade operacional: target atual1 nega count>1, não trunca. Perfil não muda no lifetime nem migra dados produtivos automaticamente.

O digest do BODY0.5 é ligação ao conteúdo EXISTENTE, não substitui armazenamento íntegro/versionado dos participantes da root. A envelope atual contém um único modelo; antes de persistir count>1, precisa haver uma composição governada da coleção e conteúdos/ref targets na mesma root, sem alias externo mutable. Mapeamento de auth_ref e interface_ref continua bloqueado até fonte normativa factual. Assim, esta proposta de offsets não é suficiente para aceitar/publicar o codec hoje: dependências semânticas/persistência/CAS/ownership precisam revisão junto do patch, não promoção automática por relatório.

Testes futuros após aceitação: roundtrip por presença/perfil, limites/count, digest e corrupção em cada região, overflow/truncamento/unknown/trailing/zeros; histórico BODY0.2/0.3/0.4/0.5/ROOT06 inalterado e rejeição explícita de reinterpretar como LST1; fonte/ref obsoleta; predecessor/epoch divergente; Stop/retidos/slots/CASconflict/UNKNOWN. Nenhum desses testes de codec novo foi executado nesta tarefa.

ED11 futuro:2208 encoded bytes máximo da seção, pelo menos storage+decode/staging/predecessor/coexistência separadamente, além dos participantes reais e hash workspace. Não somar ao orçamento como reserva aprovada; multiplicidades e retenção dependem do patch/root. Quotas não aumentam e não usar bytes livres históricos.
