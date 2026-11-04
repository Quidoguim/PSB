# Estudo para a apresentação do Trabalho 2

## Visão geral

O `malloc` é uma função de alocação de memória da biblioteca padrão de C. Com ele fazemos alocação dinâmica: em vez de definir um tamanho fixo para variáveis ou vetores, o programa pede ao sistema uma quantidade específica de bytes enquanto roda.

Então a gente implementou um mini `malloc`. A ideia é pedir um bloco grande ao sistema (o pool) e ir entregando pedaços dele. Para saber o que está ocupado, usamos uma lista encadeada de `allocation_t`, sempre na ordem de endereço. O espaço livre é o que sobra entre um nó e o próximo. Para escolher onde alocar, usamos o First Fit, que é o primeiro buraco onde o pedido cabe.

## Escolhas do código

1. Um único `malloc` para o pool

Como o enunciado só permite `malloc` para o pool e para as estruturas de controle, toda a memória do usuário tem que sair de dentro do pool. O `mymemory_init` faz `malloc(sizeof(mymemory_t))` para o gerenciador e `malloc(size)` para o pool (mymemory.c:27 e :31). Se o segundo falhar, libera o primeiro antes de retornar `NULL`. O terceiro `malloc` fica no `mymemory_alloc`, um para cada nó novo da lista (:73). São 3 `malloc` de verdade: o gerenciador, o pool e cada nó. Nada mais.

2. Lista encadeada guardando só os blocos ocupados

A lista encadeada só guarda os blocos ocupados, como a estrutura do enunciado definiu (`allocation_t` com `next`). Não precisamos guardar o espaço livre, porque ele é deduzido. Cada nó tem `start` (onde o bloco começa no pool), `size` e `next`. O `head` fica dentro do `mymemory_t`. A lista é o mapa do que está ocupado, e o livre são os buracos desse mapa.

3. Lista ordenada por endereço

Com os nós em ordem, o buraco entre dois vizinhos sai de uma conta: início do próximo - fim do atual (fim = `start + size`). Além desses, existem mais dois buracos possíveis: o do começo do pool até o primeiro nó e o do fim do último nó até o fim do pool. Assim, `alloc`, `free` e `stats` viram um laço simples. O `alloc` insere o nó novo entre `anterior` e `atual`, que é justamente o buraco escolhido (mymemory.c:80). Se `anterior == NULL`, o nó novo vira o `head`. Ordem de endereço = os buracos ficam entre os nós.

4. Estratégia First Fit

Escolhemos o First Fit porque é o mais simples e o mais rápido: ele para no primeiro buraco que cabe. No Best Fit e no Worst Fit, teríamos que olhar todos os buracos antes de decidir. O laço do `alloc` mede cada buraco e dá `break` no primeiro que couber (mymemory.c:62). Se não achar, testa o buraco final, depois do último nó (:70). Se também não couber, retorna `NULL`.

5. Contas de endereço com `char *`

Para achar o fim de um bloco ou o tamanho de um buraco, a gente precisa somar e subtrair endereços em bytes. O problema é que o `start` é `void *`, e C padrão não deixa fazer conta com `void *`, porque não sabe o tamanho do que está sendo apontado. Por isso convertemos para `char *`: o `char` tem 1 byte, então somar `n` num `char *` anda exatamente `n` bytes. O fim de um bloco é `(char *)atual->start + atual->size`, e o tamanho de um buraco é início do próximo - fim do atual, também em `char *` (mymemory.c:61 e :64). É a aritmética de ponteiros da aula: `p + 1` anda o tamanho do tipo (`int` anda 4, `char` anda 1).

6. Arredondar o tamanho para múltiplo de 8

Se alguém pede 10 bytes e o próximo bloco começa logo no byte 10, um `int *` ou `double *` guardado ali fica desalinhado, ou seja, num endereço que não é múltiplo do tamanho do tipo. Para evitar isso, todo pedido é arredondado para cima até o próximo múltiplo de 8 (mymemory.c:47): `size = (size + 7) / 8 * 8`. Como é divisão inteira, o `+ 7` empurra para o próximo múltiplo: 10 vira 16, e 16 continua 16. O custo é desperdiçar até 7 bytes por bloco, que é a fragmentação interna. Isso aparece nos testes: pedimos 10 e o `display` mostra 16.

7. O `free` libera o nó, não o ponteiro do usuário

O ponteiro que o usuário recebe aponta para dentro do pool. Ele não veio de um `malloc`, então dar `free` nele quebraria o programa. O que veio de `malloc` foi o nó. Por isso o `mymemory_free` percorre a lista procurando o nó com `start == ptr`, guardando sempre o `anterior`. Quando acha, religa a lista (o `anterior` passa a apontar para o `next` do nó, ou o `head` muda, se era o primeiro) e faz `free(atual)` (mymemory.c:110). Se não acha nenhum nó com esse endereço, não faz nada, como o enunciado pede. Isso já protege contra ponteiro inválido, `NULL` e free duplo. É o mesmo `removeByValue` do exercício da lista, comparando endereço em vez de número. Tirar o nó da lista faz aquele pedaço do pool voltar a ser buraco.

8. O `cleanup` libera na ordem certa

Se a gente liberasse o `mymemory_t` primeiro, perderia o `head` e não conseguiria mais chegar nos nós, o que gera vazamento. Então o `mymemory_cleanup` libera de dentro para fora (mymemory.c:183). Primeiro percorre a lista dando `free` em cada nó, mas guarda o `proximo` antes de cada `free`, porque depois de liberado o nó não pode mais ser lido. Depois dá `free(pool)` e, por último, `free(memory)`. C não tem garbage collector, então tudo que foi alocado tem que ser liberado por nós. A ordem é: nós, pool, gerenciador.

9. O `stats` calcula os buracos na hora

Como o espaço livre não fica guardado, as estatísticas saem de um passeio pela lista, usando a mesma conta de buraco do `alloc` (mymemory.c:137). Durante o passeio, o `stats` conta os nós (número de alocações) e soma os `size` (memória alocada). A memória livre é `total_size - alocado`. Cada buraco maior que zero conta como um fragmento, e o maior deles é o maior bloco livre contíguo. Depois do laço, ainda confere o buraco do fim do pool.

10. Três arquivos e um Makefile

O enunciado já deu o `mymemory.h`, então seguimos a modularização vista em aula. O `mymemory.h` é a interface: declara as structs e as funções. O `mymemory.c` é a implementação, e o `main.c` só usa a biblioteca e roda os testes. O `.h` tem include guard (`#ifndef MYMEMORY_H`), que evita definir as structs duas vezes se ele for incluído mais de uma vez. O Makefile compila cada `.c` com `gcc -c`, gerando os `.o`, e depois liga tudo no executável. As flags `-Wall -Wextra` ligam os avisos do compilador, porque o enunciado cobra compilar sem erros nem avisos. O `.h` diz o que existe, e o `.c` diz como funciona.

11. Testes automatizados no `main.c`

O enunciado pede uns 50 blocos de tamanhos variados, em ordens diferentes, e a avaliação cobra funcionar sem erro, sem vazamento e com pouca fragmentação. O `main.c` tem uma função `verifica(condição, descrição)`, que conta acertos e falhas e só imprime quando algo falha. A `verifica_lista` confere que cada bloco está dentro do pool, em ordem de endereço e sem sobrepor o anterior. Os blocos alocados também são escritos de verdade com `memset`, para provar que a memória entregue é usável. No fim, se houve alguma falha, o programa retorna 1, e o `make` acusa erro. O resultado foi 22 verificações OK, 0 vazamentos no `leaks --atExit` e compilação sem avisos.

12. Tipos e `printf`

Usamos `size_t` para todos os tamanhos, porque é o tipo sem sinal que o próprio `malloc` usa, e ele é impresso com `%zu`. Os endereços são impressos com `%p`. No `display`, além do endereço, mostramos a posição do bloco dentro do pool, que é `(char *)start - (char *)pool`, de novo fazendo a conta em bytes (mymemory.c:129).

## Saída do make run

A professora disse que não vai pedir para rodar, então aqui fica só o essencial, caso ela pergunte sobre os testes.

O `main.c` tem três partes. A Parte 1 repete o exemplo da professora num pool de 1000 bytes: aloca `v` (128 bytes) e `v2` (64 bytes), libera `v` e aloca `str` (16 bytes). O `str` cai no byte 0, no lugar de `v`, porque o First Fit pega o primeiro buraco onde cabe. No fim sobram 920 bytes livres, mas divididos em 2 buracos (112 e 808), então o maior pedido que cabe é 808. Essa diferença é a fragmentação. A Parte 2 testa os casos especiais: tamanho 0, pedido maior que o pool, pool cheio, `free` de ponteiro inválido, de `NULL` e free duplo. A Parte 3 aloca 50 blocos de 10 a 1000 bytes em três ordens (crescente, decrescente e misturada), libera os de índice par e tenta encaixar blocos de 200 nos buracos.

O resultado mais importante é o da ordem crescente: depois do `free`, sobram 1368 bytes livres e mesmo assim um bloco de 200 não cabe, porque o maior buraco tem só 104 bytes. Isso é fragmentação externa. No final aparecem 22 verificações OK e 0 falhas.

## Perguntas do tipo "me mostra no código onde"

As linhas são do `mymemory.c`, a não ser quando diz `main.c`.

1. Onde vocês alocam o pool? No `mymemory_init`, linha 31: `memory->pool = malloc(size)`. Logo antes, na linha 27, fica o `malloc` do próprio `mymemory_t`.

2. Onde vocês tratam a falha do `malloc`? Linhas 28 e 32 do `init`. Se o pool falhar, a linha 33 libera o `mymemory_t` antes de retornar `NULL`, para não vazar. No `alloc`, a linha 74 testa o `malloc` do nó.

3. Onde vocês arredondam o tamanho? Linha 47 do `alloc`: `size = (size + ALINHAMENTO - 1) / ALINHAMENTO * ALINHAMENTO`. O `ALINHAMENTO` é o `#define` da linha 23, que vale 8.

4. Onde está o First Fit? No laço do `alloc`, linhas 57 a 67. A linha 61 mede o buraco entre `livre` e o início do nó atual, e a linha 62 dá `break` no primeiro que couber.

5. Onde vocês calculam o tamanho do buraco? Linha 61: `inicio_ocupado - livre`, com os dois em `char *`. O `livre` anda para o fim de cada bloco ocupado na linha 64: `inicio_ocupado + atual->size`.

6. Onde vocês testam o buraco do fim do pool? Linha 70: se o laço terminou sem achar buraco (`atual == NULL`), compara o pedido com `fim_pool - livre`. O `fim_pool` é calculado na linha 49.

7. Onde o `alloc` devolve `NULL` quando não cabe? Linhas 44 e 45 (tamanho 0 ou `memory` nulo), linha 71 (não coube em nenhum buraco) e linha 75 (falhou o `malloc` do nó).

8. Onde vocês inserem o nó novo na lista? Linhas 80 a 84. A linha 80 liga o novo ao `atual`. Se não tem `anterior`, a linha 82 faz o novo virar o `head`; se tem, a linha 84 faz o `anterior` apontar para ele.

9. Onde o `free` procura o bloco? Linhas 97 a 100: anda pela lista comparando `atual->start != ptr` e guardando o `anterior`.

10. Onde o `free` ignora ponteiro inválido? Linha 102: se o laço chegou ao fim sem achar (`atual == NULL`), retorna sem fazer nada. A linha 91 já cuida do `ptr == NULL`.

11. Onde o `free` religa a lista? Linhas 105 a 108. Se o nó removido era o primeiro, o `head` passa para o próximo; se não, o `anterior` pula o nó. A linha 110 libera o nó com `free(atual)`.

12. Onde o `display` calcula a posição no pool? Linha 129: `(char *)atual->start - (char *)memory->pool`.

13. Onde o `stats` conta os fragmentos e o maior bloco? Linhas 152 a 157, dentro do laço: cada buraco maior que zero soma um fragmento e é comparado com o maior. As linhas 167 a 172 fazem o mesmo para o buraco do fim.

14. Onde o `stats` calcula a memória livre? A linha 160 soma os `size` em `total_alocado`, e a linha 178 imprime `total_size - total_alocado`.

15. Onde o `cleanup` evita perder o próximo nó? Linha 190: guarda `atual->next` em `proximo` antes do `free(atual)` da linha 191. Depois, as linhas 195 e 196 liberam o pool e o `mymemory_t`.

16. Onde está a struct `allocation_t`? No `mymemory.h`, logo no começo: `start`, `size` e `next`. Logo abaixo vem o `mymemory_t`, com `pool`, `total_size` e `head`, igual ao enunciado.

17. Onde vocês testam com os 50 blocos? No `main.c`: `NUM_BLOCOS` e `TAM_POOL` estão nas linhas 16 e 17, a função `testa_ordem` começa na linha 117 e as três ordens são montadas em `parte3_cinquenta_blocos`, na linha 155.

18. Onde vocês conferem que a lista está certa? No `main.c`, a `verifica_lista` começa na linha 34: para cada nó, confere que ele está dentro do pool, depois do anterior e sem sobreposição.

19. Onde vocês provam que a memória é usável? No `main.c`, linha 128 (`memset` em cada bloco) e linha 77 (`sprintf(str, "ola mundo")`).

## Perguntas prováveis

1. Por que First Fit? É a mais simples e para no primeiro buraco que serve. Best Fit e Worst Fit teriam que percorrer a lista inteira para comparar os buracos.

2. Onde vocês usam `malloc`? Em três lugares: o `mymemory_t`, o pool e cada nó `allocation_t`. A memória entregue ao usuário nunca vem de `malloc`: é um pedaço do pool.

3. Como vocês sabem onde tem espaço livre, se a lista só guarda o ocupado? A lista está em ordem de endereço. O buraco é a distância entre o fim de um nó (`start + size`) e o início do próximo, mais o espaço antes do primeiro e depois do último.

4. O que acontece se o ponteiro passado ao `free` for inválido? O laço não acha nenhum nó com aquele `start`, e a função retorna sem fazer nada. Testamos isso com um endereço de variável local, com `NULL` e com free duplo.

5. Por que `free(atual)` e não `free(ptr)`? O `ptr` aponta para dentro do pool, não veio de `malloc`. Quem foi alocado com `malloc` é o nó.

6. Por que converter para `char *`? Para fazer conta em bytes. Aritmética em `void *` não é C padrão, e `char` tem 1 byte.

7. O que é fragmentação? O trabalho de vocês tem? A externa é memória livre espalhada em buracos pequenos, como no teste crescente, em que blocos de 200 não couberam com 1368 bytes livres. A interna é o arredondamento para 8, que desperdiça até 7 bytes por bloco. O First Fit não junta buracos sozinho, mas buracos vizinhos já viram um só automaticamente, porque o livre é calculado entre os nós ocupados.

8. Como garantem que não tem vazamento? O `cleanup` libera nós, pool e gerenciador, nessa ordem. Rodamos `leaks --atExit -- ./t2` e deu 0 leaks.

9. Por que a lista ordenada? Sem ordem, para achar os buracos teríamos que comparar todos os nós entre si. Com ordem, basta olhar os vizinhos.

10. O que o `stats` mostra? Número de alocações, bytes alocados, bytes livres, maior bloco livre contíguo e quantos fragmentos livres existem. Tudo calculado num passeio pela lista.

11. Por que arredondar para 8? Alinhamento: `int` e `double` devem começar em endereços múltiplos do seu tamanho. Sem isso, depois de um bloco de 10 bytes, o próximo começaria num endereço desalinhado.

12. O que muda se usassem Best Fit? Só o laço do `alloc`: em vez de `break` no primeiro buraco, percorreria todos guardando o menor que cabe, e inseriria ali. O resto (`free`, `stats`, `cleanup`) não muda.

13. O `main.c` da professora compilava? Não: ele chamava `mymemory_malloc` e `mymemory_release`, mas o `.h` declara `mymemory_alloc` e `mymemory_cleanup`. Reproduzimos o mesmo cenário com os nomes do `.h` (Parte 1 do nosso `main.c`).

14. Para que serve o `#ifndef MYMEMORY_H`? É o include guard: evita definir as structs duas vezes se o `.h` for incluído mais de uma vez.

15. Por que guardar o `anterior` no laço? A lista é simplesmente encadeada: um nó só conhece o próximo. Para inserir ou remover no meio, precisamos mudar o `next` de quem vem antes, e não dá para voltar na lista.

16. Por que o `alloc` devolve `void *`? Porque o alocador não sabe o que o usuário vai guardar ali. O `void *` é um endereço sem tipo, e quem chama converte para `int *`, `char *` etc., igual ao `malloc`.

17. Por que o `head` começa como `NULL`? Porque no começo nada está alocado. Lista vazia = pool inteiro livre, um único buraco do byte 0 até o fim.

18. Se liberarem dois blocos vizinhos, o buraco fica dividido? Não. Os dois nós saem da lista, e a conta início do próximo - fim do anterior já enxerga um buraco só, do tamanho dos dois somados.

19. O que acontece se o usuário escrever além do tamanho que pediu? Invade o bloco seguinte, igual acontece com o `malloc` de verdade. O alocador não protege contra isso; é responsabilidade de quem usa.

20. Quanto custa um `alloc` e um `free`? Os dois percorrem a lista no pior caso, então o tempo cresce com o número de alocações. O First Fit costuma parar antes do fim, o que ajuda.

21. Por que a memória do usuário não é zerada? O enunciado não pede, e o `malloc` de verdade também não zera. Quem precisa de zeros usa `memset` depois, como fazemos nos testes.

## Comandos

No Mac:

```
make          # compila
make run      # roda os testes
make leaks    # checa vazamento
make entrega  # gera T2-mymemory.zip
make clean    # apaga .o e executável
```

No Windows, para gerar o pacote de entrega (PowerShell, dentro da pasta Trabalho2):

```
Compress-Archive -Path mymemory.h,mymemory.c,main.c,Makefile -DestinationPath T2-mymemory.zip
```
