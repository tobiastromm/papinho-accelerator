# Resultado focal — corte4 e retenção excessiva do observador

2026-11-02, America/Sao_Paulo; main/HEAD
f341985924d29870d8ff627ed37c8a0874ad6469. Fonte: captura de console enviada pelo
proprietário, não nova coleta. Observação AUTORIZADA/INVOCADA/CONSUMIDA;
QUOTA_DENIED_RETAINED, sem retry. Execução de wrapper ainda sem resultado final.
Preservados pedido/procedimento/imagem congelados, atos antigos e todos os objetos.

## Evidência reconciliada da mesma execução

| Corte | known | handles | slots |
|---|---:|---:|---|
| 0, antes do lookup/open | 1 | 119 | 0x00 |
| 1, após lookup | 1 | 119 | 0x00 |
| 2, após open síncrono | 1 | 120 | 0x01 |
| 3, após tipo/identidade | 1 | 120 | 0x01 |
| 4, após SD válido | 1 | 128 | 0x03 |
| 5–8 | 0 | UNKNOWN | não alcançados |

NtCreateFile attempted1/NTSTATUS0/IOSBstatus0/info1/handle1; identity_checked1.
hash_called0; snapshots hash0 não significam que BCrypt foi executado.
first1816/first_cut4/final_guard0/close_known0. A guarda extra >=124 antes do
hash interrompeu;128 não é ultrapassagem de cap128, mas já não satisfaz guard124.
final_guard0 significa que a query terminal8 não foi alcançada, não PASS.

Saldo novo próprio raw0:+1,119->120. Crescimento+8 localizado entre3/4, abrangendo
GetSecurityInfo, IsValidSecurityDescriptor e a própria consulta adicional.
Não há aquisição explícita de oito HANDLEs pela aplicação nessa região.
GetSecurityInfo devolve memória de SD (checking), não oito handles do caller;
LocalFree é a API documentada para o buffer. Não atribuir os oito a COM/Shell,
RPC/cache/token/vazamento ou internals específicos sem prova individual.
Não comparar119 ou128 com113/125 de outra execução como se fossem o mesmo baseline.
Fonte histórica125 e ownership dos oito continuam UNKNOWN; nenhuma redução
segura de handles internos ou correção da admissão operacional foi demonstrada.

## Causa exata de a janela ficar parada

scqc_descriptor_digest obteve/validou SD, chamou sample4 e retornou1816 antes
do hash e antes do LocalFree normal. O done antigo era:

```c
if(n->scq_inputs.checking||n->scq_inputs.content_hash.retained||n->scq_inputs.content_hash.active)o->unresolved=1;
if(!o->unresolved){z=scqc_close_raw(o);if(z&&!v->first_error)scqc_interval_error(v,9U,z);}
```

Logo: checking presente -> unresolved -> rawclose não tentado -> launcher pula
native_close/finish -> imprime close_known0 -> RETAINED/Sleep(INFINITE).
Nesse caminho específico não há evidência de LocalFree ou CloseHandle terem
falhado: NÃO foram tentados. Retenção e ausência de close são reais, mas a
classificação conservadora confundia um temporário de ownership conhecido com
incerteza de aquisição/fechamento. É defeito focal de aborto do observador que
foi preparado, não prova de leak nem de falha nativa do Windows.

## Correção técnica offline, sem mudar a guarda

personal_scq_interval_observe_win32.inc: exclusivamente no aborto cut4/1816,
sample conhecida124..128,hash ainda não chamado e sem slots CNG/retained/active
ou unresolved anterior, tentar LocalFree do SD síncrono próprio UMA vez.
Se sucesso, zerar checking e executar scqc_close_raw existente UMA vez. Se free
ou close falhar, preservar unresolved/retidos; não limpar incerteza anterior.
Primeira falha1816, first_cut4 e amostras mantidos. Não continuar hash/queries,
abrir decisão, deslocar quota ou obter PASS por cleanup. Sem getters/grants/API
de recuperação, alterações de outros perfis ou fechar handles internos.

Compatível com ownership/cleanup de P5/P6 e o helper existente: só recurso cujo
ownership e término de uso estão provados. A documentação Microsoft exige
LocalFree do buffer GetSecurityInfo. UNKNOWN de pending, hashretained, Stop e
outras falhas mantém o tratamento anterior. Não reescrever contratos accepted
nem ampliar autoridade operacional. Corrigir tecnicamente o aborto offline
não concede autoridade para repetir a observação nem para tocar na instância
antiga, que permanece parada com sua coorte intacta.

## Qualificação afetada, sem executar o observador

Quatro configurações Selected/Legacy Debug/Release: compilação da biblioteca
do owner e testes afetados PASS/zero warnings;12pares CTest (raw-open,
cold-controller,P1-entry) PASS. raw-open102casos/836checks;38casos modelados de
intervalo. Novo124/128 libera só SD próprio; LocalFreefalha/rawclosefalha e
unresolved anterior continuam retidos; no retry/decisão/hash e replay negado.
Nenhuma campanha inalterada repetida além das regressões afetadas.

Não relinkado o server operacional/congelado; EXE selecionado pelo wrapper
permanece779776/hash7b6d5e500723ba5db2f1d337cb1ca55e9fa7b3c5588ca6b31d4c7d81e693e726.
Não atualizado EXE instalado. Não selecionar esse EXE antigo como se tivesse
cleanup novo. Correção é fonte/biblioteca/modelo; nenhum novo artefato operacional
ou pedido de coleta está selecionado. PDBs de build Debug com C1033 foram
preservados sob nomes before-owned-abort e regenerados; sem trocar flags/toolchain
ou alegar causa da falha de build sem demonstração. Logs owned-abort preservados.

## Caminho imediato e primeiro gate

Processo já aberto não recebe correção por alteração de fonte. Não executar
de novo, não injetar cleanup e não transformar wrapperClose em nativeClose.
O handler próprio só trata CTRL_C/CTRL_BREAK marcando Stop; isso não acorda o
Sleep(INFINITE) terminal. Nenhum comando de retomada está implementado.

Preparado ONE-P1-RETAINED-CONSOLE-CLOSE.REQUEST.md para UMA eventual decisão
operacional específica do proprietário: fechar manualmente a janela original,
aceitando escopo de console e interrupção sem prova de cleanup. Não executado,
não autorizado pela coleta consumida nem pelo simples pedido de investigação.
G5 exige decisão própria para terminação. Ações sobre filesystem/SCM/serviço,
cleanup/refund/recuperação continuam proibidas. Se não aprovado, não intervir.

Depois disso, o bloqueio técnico do Start continua sendo margem real de handles
após SD, não o hang do observador. Não prometer que LocalFree elimina os oito
internos. Próxima análise técnica deve avaliar aquisição de SD equivalente sobre
mesma referência retida sem novo owner/canal, apenas se os contratos/provas
permitirem; não trocar automaticamente GetSecurityInfo por API que promete menos
handles sem medição/garantia. GetKernelObjectSecurity não foi escolhido como
substituto: documentação low-level distingue files dos outros kernel objects.
Não preparar UPDATE/Start ou nova coleta para essa hipótese neste fechamento.

## Fontes primárias

- https://learn.microsoft.com/en-us/windows/win32/api/aclapi/nf-aclapi-getsecurityinfo
- https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-localfree
- https://learn.microsoft.com/en-us/windows/console/handlerroutine
- https://learn.microsoft.com/en-us/windows/win32/secauthz/low-level-security-descriptor-functions

PST0.6.4,guard124,cap128,G3,DENY,históricos/pins/EXEs/originais/atos consumidos e
retenção/quiescência OS UNKNOWN preservados. Sem nova coleta, UPDATE,
endpoint/SCQ/Start/Stop/SCM/ACL/CA/CURRENT/rede/TPM/NV/Q4/limpeza/refund.
