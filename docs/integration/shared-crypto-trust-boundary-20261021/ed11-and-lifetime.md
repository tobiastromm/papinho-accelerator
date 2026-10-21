# ED11 e lifetime reconciliados

Sem aumento de quotas, seleção de perfil ou reserva. Limites Accelerator:3145728 somente explicitamente Windows; default/Legacy2097152; Browser inalterado. Provider/kernel e ativação permanecem UNKNOWN; totalPARTIAL. Nenhuma economia operacional é presumida por deduplicar fonte.

| Objeto | Owner/lifetime | Contabilidade |
|---|---|---|
| SP_SIGNED_PARTS | caller síncrono; views emprestadas no mesmo pacote imutável | sizeof64 x64 /32 VC6x86 confirmado nos testes |
| domain | const estático no módulo comum |26bytes incluindo NUL; substitui literal26 anterior no adapter; não contar duas vezes no mesmo caminho |
| plan local no signature_exact | stack do adapter até retorno | substitui MP_ENVELOPE de quatro slices, mesma dimensão64 x64; não prova delta zero do frame compilado |
| e/v em signed_parts | stack durante inspeção e retorno | payloads C nomeados128 x64 /64 x86, coexistem com caller; custo de frame/ABI/stack máximo não medido |
| normalização | temporário32 + output caller32 | rotina comum existente apaga cópia local e output inválido; wrapper conserva preflight de aliases/null/oversize |
| salt/nonce/AAD/cipher/signature | views no backing caller | sem cópia ou buffer permanente novo; pacote continua retido por owner |
| M1 op/input/PDG | owner administrativo existente |776/128/72 x64; inalterados; seleção488 e digest32 já inclusos na op |
| q/staging/output | caller; retidos em incerteza |49654 cada no cenário anterior; não compartilhados nem removidos |
| provider/CRT | adapter específico | pico de teste1290354 novamente observado, inclui contexto global TEST ONLY; não é pico operacional |
| Browser KDF workspace | caller síncrono do adapter |480bytes relatados na suite VC6; sem alteração/migração nesta tarefa |

A soma conservadora anterior3625925 continua DENY frente3MiB por480197, antes de acréscimos transientes/stack e custos desconhecidos. Não é uma afirmação de pico necessário/real nem reserva aprovada. A nova chamada ao planner adiciona lifetime de metadata C durante preflight; não excluir sua coexistência com contexto provider já aberto. O caminho assinatura ainda não abriu EVP_MD_CTX naquele ponto, mas isso não autoriza compartilhar storage ou reduzir reserva opaca. Não inferir pico operacional pelo número de checks.

O produto/runtime não liga esta biblioteca offline; delta gerenciado fixo no servidor0. O CMake do componente offline agora liga suite/structure; páginas de código/RO, frames, CRT/alinhamento e backends precisam censo de ativação próprio. Os limites numéricos de entrada não são créditos. Memória de teste/oracle fica em processos isolados, fora de qualquer alegação de orçamento runtime; não é memória do produto movida para escapar da conta.

signed_parts não executa I/O, callbacks crypto, KDF/AEAD ou aquisição de evidência; Stop callback é somente observação e não pode reentrar/liberar backing. Sem retenção interna após retorno, mas views continuam dependentes do caller. M1 existente mantém input/segredos/referências/contexto/crédito em RETAINED; query-only finish apaga apenas após quiescência e nunca publica plaintext tardio. Suites146 de controle e1189 crypto cobrem regressões existentes; não provam pins/CURRENT factuais nem destruição operacional. NT4/box/ISA não qualificados.
