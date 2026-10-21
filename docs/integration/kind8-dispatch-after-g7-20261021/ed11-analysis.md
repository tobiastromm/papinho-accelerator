# ED11 — kind8 sob backings existentes

Fixo medido permanece1943680 Selected e1790872 Legacy; delta fixo0. Os116616 bytes dos backings G7, snapshots issuer e buffers anteriores continuam cobrados, sem redução de reserva. Nenhum buffer de root/worker cresceu, quota individual ou cap mudou. Windows3145728 somente por opt-in; default2097152 inalterado.

Contexto privado Selected medido112 bytes (antes32, +80 por instância caller). O teste usa CA arena32768 e signer arena32768 SIMULTÂNEOS/disjuntos, certificate scratch8192,2 workspace descriptors48,2 borrows locais176 e signer descriptor56. DER da CA, packet, row e carrier são referências ao owner já contado; não copiá-los/somá-los novamente. Metadata/pointers do caller continuam custos reais. Não há pool ou objeto permanente de contexto introduzido.

Sobre cenário anterior2220463, o subtotal nomeado de dispatch é2294583, margem aritmética851145, NÃO crédito ou pico operacional. Ele inclui arenas/scratch/descritores nomeados, não todo frame/CRT/provider. Não foi presumida exclusividade com inputs/arena ainda existentes depois de capture: cenário anterior de capture2292127 acrescido desses74120 bytes de dispatch=2366247, também hipótese, não medição de pico/reserva. Não somar essas hipóteses como reservas aprovadas nem eliminar custos opacos. A alocação real e eventual retenção do caller devem ser demonstradas antes de ativação.

Disassembly direto: pinned_signer624Debug/344Release; pinned_ca128/72; dispatch848/736 de sub rsp principal. Não incluem todos os pushes/temporários/chamadas/inlining/transitive stack. Componente X509/hash/codec/CryptoAPI e provider/kernel continuam adicionais/UNKNOWN, não zero. Não foi declarado que reservas históricas de stack qualificam este caminho operacional.

As arenas pertencem ao caller e permanecem exclusivas até retorno síncrono; CA decode não pode ser sobrescrito pelo verificador X509. Nenhum deep CERT_INFO é usado depois do retorno; não há novo handle/heap explícito/thread/pin/lease. Primitivas nativas podem ter custos opacos. Stop/UNKNOWN não devolvem recursos/creditos do owner; referências antigas permanecem sob retenção e quiescência G7.

O cenário conservador anterior de pacote completo3742625 já excede3MiB, antes dos custos completos desta ativação. Não é medição operacional nem margem aproveitável. Ativação de pacote completo, fonte E0, CURRENT, assinatura e admissão continuam DENY; ED11 total PARTIAL. Nenhum campo reserved de contexto vira prova factual de reserva.
