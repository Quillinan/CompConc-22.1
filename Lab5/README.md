# Comp Conc - Lab 5

## Atividade 1

### Q2

Sim. o programa printou "bye" após 2 prints de "hello"s.

### Q3

O programa não encerrou. Devido a alteração feita, a variável global x jamais chega ao valor 2, de forma análoga o sinal de desbloqueio da thread B não ocorre, assim o programa nunca se encerra.

## Atividade 2

### Q2

Sim. As threads A só printam "bye" depois que a threads B printa "hello".

## Atividade 3

### Q2

Não. Em alguns dos testes foi printado um resultado diferente do esperado.

### Q3

Sim. Ambas realizam a operação de repetição necessária para tal etapa do programa, inclusive a linha 51 corrige um erro existente na linha 50.

### Q4

Sim. A execução ocorreu de forma correta. Da forma que estava antes, utilizando o _if_, a condição era verificada antes do bloqueio, podendo ocorrer uma falha na situação em que _x_ era alterado após o desbloqueio, já que não haveria uma nova checagem, printando assim um resultado divergente do esperado. Todavia com o loop do _while_ a checagem sempre ocorre, garantindo o resultado esperado.
