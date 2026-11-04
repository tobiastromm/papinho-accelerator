# Produtor SID runtime — correção focal qualificada offline

2026-11-02. main/HEAD f341985924d29870d8ff627ed37c8a0874ad6469.
IMPLEMENTED_OFFLINE / QUALIFIED / NO_REAL_OPERATION.

## Defeito demonstrado e limites da evidência

O produtor atribuía cbSid diretamente a runtime.sid_bytes. A documentação de
[LookupAccountNameW](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lookupaccountnamew)
define esse parâmetro como capacidade de entrada e informa o tamanho necessário
quando o buffer é insuficiente; não garante comprimento semântico atualizado
no sucesso. O código dependia de uma atualização não garantida pelo contrato.

Modelo com SID de serviço válido32 bytes/buffer68 e sucesso preservando cbSid68
reproduz reason3 no produtor antigo. O produtor corrigido registra32, satisfazendo
a MESMA guarda original. Isso prova o defeito de contrato; a sessão real revelou
geometria inválida, mas não os valores. Não afirmar que eram68/32 nem comprovar
retroativamente a causa de todas as tentativas anteriores. Causa histórica
específica permanece não demonstrada além do grupo reason3 registrado.

## Correção mínima

Novo helper privado scqc_runtime_sid no owner existente, mesmo caller e mesma
lookup original NT SERVICE\PapinhoAccelerator, sem retry/alocação/novos handles.
Exige runtime vazio na origem, tipo SID aceito e capacidades coerentes, count
limitado/geometria dentro do buffer e da capacidade retornada; valida o SID
antes de chamar GetLengthSid. Registra o comprimento semântico validado e
submete à scqp_unbound ORIGINAL e intacta, incluindo autoridadeNT/count6/prefix80,
campos futuros zerados, digest ausente e padding zero. Não modifica bytes do SID,
não limpa padding, não normaliza objeto inválido ou fabrica destinatário.

[GetLengthSid](https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-getlengthsid)
exige SID válido; por isso [IsValidSid](https://learn.microsoft.com/en-us/windows/win32/api/securitybaseapi/nf-securitybaseapi-isvalidsid)
é chamado após os limites e antes do comprimento. Não se aceita truncamento ou
capacidade incoerente como prova. Fonte original e todas as validações posteriores
serviço/imagem/processo/token/bilateral permanecem obrigatórias; SID não é grant.

Nenhuma nova decisão normativa: é correção de construção coberta pelos contratos
accepted, sem alterar policy, formatos SCQ1/SQB1/SQB2, quotas ou autoridade.
P0–P7/OHI/SSO-B, READ/NEW_INSTALL/demais perfis, G3-P1/Stop/deadlines/consumo/
receipt/close/retenção intactos. Nenhum PID/token futuro exigido antecipadamente.
Diagnóstico final existente preservado. Novo sizeof do packet não aumenta.

## Qualificação

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings finais,
mesma toolchain/flags/PST0.6.4. Falhas iniciais somente no mock (macro redefinida
já existente e nome local sombreando global) foram corrigidas sem dispensar
W4/WX; logs attempt1 preservados. 56 pares teste/configuração PASS, incluindo
17 novos casos SID: capacidade68 preservada, retorno32, capacidade40 compatível,
falha/tipo inadequado, capacidades0/69/16, count16, revisão inválida, padding,
autoridade/prefixo/count divergentes, runtime previamente preenchido/reentrada.
Casos de falha não são limpos para passar. Confere zero/uma lookup, sem segunda
consulta ou retry. Os41 cenários do diagnóstico final e10 do envelope continuam.
APIs protegidas simuladas; nenhum LookupAccountName real/SCM/endpoint/Start,
coleta ou UPDATE foi executado. Censo/managed limit PASS; sem quota ampliada.
Receita build-g3-p1-runtime-sid-offline.cmd; logs g3-p1-runtime-sid-*-build/tests.log,
hashes e fontes em G3-P1-RUNTIME-SID-PRODUCER-EVIDENCE.json.

## Artefato e continuidade operacional

Novo Selected/Release784896 bytes, SHA256
201b0f01d0d6b33f73d653bd492567ff663f14e183170ced4d18a6a6f0ddb752.
Mesmo tamanho do instalado, bytes/hash diferentes. Congelado em workspace como
papacc_server.g3-p1-runtime-sid-784896.exe; anterior congelado preservado.
Instalado histórico784896/ID417614100:4325376:164699/SHA256
0b00a21a148b4b015d2fc40d8f412441a8af78a77fcfa7bed93a67a60048fde3;
nenhuma consulta/alteração instalada nesta task. DLLs byte-idênticas.

Pedido UPDATE 2c25b581fd2a506d369bb536d58d3da76028e2dd8489891e299823ab2a576836, procedimento adb80e3f24fa5ebf4b55914c43fc505c46ce223e4d07f5f0e44a8e1a51a773ff;
U1–U7/U2 e updater protegido existente, seis helpers idênticos por AST,
16 argumentos e uma chamada, K2027314 do executor protegido inalterado.
Não usar censo do novo servidor como K desse executor. Originais/retidos da
sessão consumida preservados, sem limpeza/refund. Close nativo histórico/OS
UNKNOWN não resolvidos por correção de código.

O proprietário autorizou contextualmente este novo UPDATE durante a preparação,
solicitando o comando imediato. Registrar separadamente sua fala REAL e a
vinculação final do pacote, sem atribuir-lhe uma resposta contendo hash que
não escreveu. Preparar comando manual após conferência byte-exata; NÃO executar.
Gates nativos frescos/confirmador/consumo/SCM STOPPED permanecem obrigatórios.
Após saída manual/reconciliação, outra sessão E/Q/S precisa de novos originais
legítimos e autorização independente. Nenhum Start autorizado por este UPDATE.
