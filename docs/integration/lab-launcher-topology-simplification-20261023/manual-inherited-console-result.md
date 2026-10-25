# Resultado manual elevado — herança de console

O owner executou uma vez o preflight read-only autorizado. [Recibo final](../canonical-preflight-busy-diagnosis-20261023/manual-prepare-134372739910985976-final.json) conferido no disco; script SHA2563983d3fc6143a50bfebf7ecc56a28a1213a6275390a50bda4a38d46fc9a5df4a corresponde ao launcher atualizado. Nenhuma repetição pelo executor.

```text
PREFLIGHT_RESULT=STOP
FIRST_BUSY_STAGE=PREFLIGHT_CENSUS
FIRST_BUSY_API=process_census_predicate
WIN32_ERROR=170
OBSERVED_PROCESS=conhost.exe
OBSERVED_PID=19516
OBSERVED_PARENT_PID=13056
child=1
project_prefix=0
QUALIFIER_PID=22060
QUALIFIER_CREATION=31286091:2265030170
close=0
starts=0
stops=0
intent_consumed=0
effect=0
diagnostic_created=0
log_failed=false
```

A alteração de criação do qualificador passou em fixtures, mas NÃO eliminou a rejeição no ambiente elevado real. Não se promove o resultado isolado a PASS de topologia operacional. O censo continua falhando fechado.

O recibo não registra o mapeamento completo PID/creation dos dois atores PowerShell nem a relação nativa console/host do filho rejeitado. Portanto não é possível atribuir a origem desse conhost ao launcher original, à elevação UAC ou a outro passo usando somente parent PID. Não reutilizar esses PIDs como identidades atuais, não remover processo nem criar whitelist.

A primeira lacuna discriminante é registrar os dois atores já validados (papel/PID/creation) e a associação de console no mesmo instante do censo. Antes de nova operação, preparar diagnóstico focal e testes isolados; qualquer nova execução manual exige autorização específica. Não aumentar B4.B.4, não reinterpretar baseline sem evidência e não selecionar revisão normativa D apenas por este resultado.

close=0 é sucesso reportado pelo qualificador para seu fechamento; quiescent=0 não prova quiescência operacional nem término do console host. Nenhum efeito SCM/ACL/Start/Stop/CA/CURRENT; autorização positiva não concedida. ED11 PARTIAL; créditos NONE_GRANTED; autoridades DENY. A tentativa operacional continua não consumida.
