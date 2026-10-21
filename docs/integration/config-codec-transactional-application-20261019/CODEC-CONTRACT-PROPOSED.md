---
status: proposed
revision: 1
scope: private offline listener fragment only
---
# LST1 — contrato exato proposto para decisão do owner

## C1 Escopo e precedência

Esta proposta especializa N6 e F5 somente após aceitação explícita e revisão oficial. Não é ACCEPTED por esta gravação. Não substitui ADR0007/0009, N1–N8, F1–F6, decisão PersistentID, root/C8, issuer/C2, Model E ou ED08/ED11. Preserva integralmente codec-layout-proposed.md histórico. Não incorpora a proposta antiga inteira por referência.

O primeiro slice é um codec portável de fragmento de intenção/refs/contexto, sem formato AST e sem autoridade, arquivo, journal, registry, parser, worker ou supervisor novos. Não integra bytes a BODY0.5/ROOT06/checkpoint, nem define uma envelope nova. A incorporação futura à MESMA root exige contrato versionado próprio e reconciliação dos slots, CAS, recovery e participantes. Um fragmento não é configuração completa. Configuração desejada não é CURRENT.

## C2 Layout byte-exato

Unsigned little-endian, sem padding, ponteiros, struct dumps ou dependência Win32/CNG. Tamanho exato 160+512*N; N=0..4; máximo2208. Header128, N registros512 em ordem administrativa original, trailer32 SHA256 sobre todos os bytes anteriores. SHA256 por adapter existente, sem implementação criptográfica nova. Falha/ausência de hash rejeita; nenhum fallback. Digest comprova somente integridade relativa, não autenticidade/freshness/fencing/autoridade. Não aceitar trailing bytes.

Header (offset:bytes):
- 0:8 ASCII PACLST01; 8:2 versão1; 10:2 header128; 12:4 total exato; 16:2 N; 18:2 record512.
- 20:4 presence, bits0=root,1=R,2=A,3=P obrigatórios; bit4=I opcional; demais zero.
- 24:32 identidade root; 56:8 R de proveniência; 64:8 A de proveniência; 72:8 P de proveniência; 80:8 I opcional; 88:8 predecessor histórico de R; 96:2 algoritmo1=SHA256; 98:30 zero.
- Root não toda zero; R/A/P presentes não zero; I ausente exige bytes zero, presente exige não zero. Predecessor < R; zero permitido como predecessor da primeira revisão. Nenhum valor é escolhido/derivado pelo codec: todos são dados de proveniência fornecidos pelo adapter factual. Encode/decode não validam atualidade administrativa. Sem contexto factual obrigatório, encode rejeita em vez de inventar zeros/epoch. O codec não é serializer universal de SOURCE ainda sem contexto.

Registro (offset:bytes):
- 0:4 presence; bits0=id,1=binding,2=port,3=profile,4=authkind,5=policy obrigatórios; bit6=security condicional; bit7=auth_ref condicional; demais zero.
- 4:1 profile Secure1/Legacy2; 5:1 admission OPEN1/CLOSED2; 6:1 binding ALL1/SELECTED2; 7:1 authkind NONE0/CERTIFICATE_IDENTITY1 (identidade do canal, não provider de usuário/senha); enums desconhecidos rejeitados.
- 8:2 port1..65535; 10:2 idlen1..64; 12:2 securitylen; 14:2 authreflen; 16:2 policyreflen1..64; 18:2 interfacecount; 20:4 zero.
- 24:64 listener_id; 88:64 security_ref; 152:64 auth_ref; 216:64 policy_ref; 280:32 policy_identity; 312:32 digest SHA256 dos bytes1024 BODY0.5 correspondentes; 344:40 zero.
- 384:128 oito entradas16: validU8=1, sete bytes zero, PersistentIDU64; somente interfacecount entradas usadas, demais bytes zero. ALL exige count0; SELECTED count1..8. Zero de PersistentID é válido quando valid=1. Duplicação por valor rejeitada, sem nome/índice/IP/alias persistido. Tokens são machine-local, não identidades globais.

IDs e policy_ref usam exatamente identificadores ASCII aceitos pelo parser existente; comprimento explícito, sem NUL interno, tail zero. Referências de segurança/auth são bytes opacos não secretos, sem NUL interno e tail zero. Secure exige bit6/securitylen1..64; Legacy exige bit6 ausente/securitylen0/bytes zero. NONE exige bit7 ausente/authreflen0/bytes zero. CERTIFICATE_IDENTITY não permite inferir provider/password: no primeiro slice é UNSUPPORTED para encode/decode sem contrato/mapeamento específico. CLOSED também UNSUPPORTED no primeiro slice: não substituir usuário/senha obrigatório por mTLS. Assim bytes reservados no domínio de enums não criam um provider fictício. Qualquer combinação ativa não suportada falha integralmente.

Policy_identity e digest BODY0.5 não podem ser todo zero. Comparação do digest/modelo e resolução de refs exigem consumidores factuais externos ao codec. O codec verifica forma e integridade, não declara refs RESOLVED: uma referência bem formada desconhecida é dado não resolvido; aplicação rejeita-a obrigatoriamente. Não procurar refs em stores paralelos. Nenhum certificado, chave privada, senha, token, payload ou trustDER raw é armazenado aqui.

N=0 é coleção vazia estruturalmente representável; não cria listener default nem config aplicável. Aplicação/runtime atual exige sua cardinalidade suportada; N>1 rejeita no target singular, nunca trunca. IDs de listener duplicados e duplicação de PersistentID no mesmo registro rejeitados. Endpoints e referências semânticas conflitantes são rejeitados pela validação/resolução, não inferidos de bytes iguais de refs. Ordem não é ordenada pelo encoder.

## C3 Contrato de API e recuperação

Entradas imutáveis/caller-owned durante chamada síncrona, sem concorrência de mutação/Stop-driven destruction. Saídas disjuntas das entradas; overlap rejeitado antes de escrita. Bounds/aritmética validados antes de offsets/cópias. Saída inválida não pode reutilizar bytes/flags de sucesso anterior: falha deixa resultado indisponível e tamanho usado zero; precondições inválidas de ponteiro/overlap não escrevem. Nenhum prefixo parcial aceito. Encoder valida o modelo inteiro antes de gerar resultado disponível; decoder valida tamanho/digest/campos/enums/presença/tails/duplicatas antes de publicar view concluída. Limites excedidos rejeitados, sem alocação oculta.

BODY0.2/0.3/0.4/0.5 e ROOT06/checkpoint permanecem byte-exatos; seus readers não interpretam LST1 e LST1 não reinterpreta formatos antigos. Nenhuma migração automática, uso de bytes livres ou mudança de discriminação histórica. Decode não reconstrói CURRENT/A/P, pins, grants ou capacidade de admissão. Sem checkpoint recente aprovado e fencing, recuperação autoritativa continua DENY, mesmo com digest válido.

## C4 Aplicação administrativa futura

SOURCE -> PARSED -> VALIDATED -> MODEL_BOUND -> RESOLVED -> PREPARED -> COMMITTED -> CURRENT utiliza a única transação existente. Codec é representação, não estágio de autoridade. Seleção SELECTED permanece DENY sem aplicabilidade topológica no consumo; named provider PENDING. Perfil e listeners[] explícitos preservados. CAS conflict/UNKNOWN impedem fallback a autoridade antiga e conservam ownership/créditos/buffers/serial. CURRENT/EPOCH somente pela root após commit e confirmação normativa, preservando tipos I/R/A/P, pins/Sessions/Stop/Model E. Alteração de policy operacional exige transição P aplicável; não escolher P em texto/codec.

CLI futura reutiliza dispatcher existente: config validate reporta estágio/limites; diff distingue desejado de CURRENT factual; show-effective exige pin CURRENT e presença explícita; apply fica indisponível até envelope/resolução/transação/gates completos. Nenhum desses comandos é implementado ou autorizado operacionalmente por esta proposta.

## C5 ED11 e aprovação separada

2208 bytes são limite do fragmento, não reserva aprovada. Pelo menos encoded+decode/reencode staging podem coexistir: até4416 bytes, mais modelo normalizado, SHA workspace, AST/input, refs/pins e retenção reais. Censo exato de sizeof/multiplicidade e reserva deve preceder patch; não somar duas vezes root/AST existentes. Não aumentar2097152 nem assumir espaço disponível pelo limite isolado. Target sem reserva suficiente nega. Provider/kernel UNKNOWN; headroom/freshness não inventados. Patch funcional exato e testes requerem revisão oficial posterior; aceitação normativa não aprova hunks por referência.

CLIENT_ADMISSION=DISABLED; SITE_TLS_ADMISSION=DENY; ED11_ADMISSION=DENY. Sem rede, CA/certificado/credencial real, Browser/PST/wire/produção0.6.4 alterados. PST0.6.5 candidato até encerramento Phase6.
