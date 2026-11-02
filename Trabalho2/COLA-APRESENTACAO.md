# T2 — Cola para a apresentação

Leitura de ~10 minutos. Para cada escolha: **por que** usamos, **como** está no código e de **qual aula** vem.

---

## O trabalho em 30 segundos

> "Fizemos um mini `malloc`. Pedimos **um bloco grande** ao sistema (o *pool*) e, a partir daí, entregamos pedaços dele. Para saber o que está ocupado, usamos uma **lista encadeada** de `allocation_t`, sempre **em ordem de endereço**. O espaço livre é o que sobra **entre um nó e o próximo**. Para escolher onde alocar, usamos **First Fit**: o primeiro buraco onde o pedido cabe."

```
pool: [ v2 (64) | livre (112) | str (16) | livre (808) ... ]
head -> {start, 16} -> {start, 64} -> NULL      (lista em ordem de endereço)
```

---

## As escolhas, uma por uma

### 1. Um único `malloc` para o pool
- **Por quê:** o enunciado só permite `malloc` para o pool e para as estruturas de controle. Toda memória do usuário tem que sair de dentro do pool.
- **Como:** `mymemory_init` faz `malloc(sizeof(mymemory_t))` e `malloc(size)` para o pool (`mymemory.c:27` e `:31`). Se o segundo falhar, libera o primeiro antes de retornar `NULL`.
- **Aula:** alocação dinâmica: `malloc` devolve `NULL` quando falha, então sempre testamos.
- **Para fixar:** *3 tipos de `malloc` de verdade: o gerenciador, o pool e cada nó. Nada mais.*

### 2. Lista encadeada guardando só os blocos ocupados
- **Por quê:** é a estrutura que o enunciado definiu (`allocation_t` com `next`). Não precisamos guardar o espaço livre: ele é deduzido.
- **Como:** cada nó tem `start` (onde começa no pool), `size` e `next`. O `head` fica dentro do `mymemory_t`.
- **Aula:** lista simplesmente encadeada (o exercício da lista: `add`, `addStart`, `removeByValue`, `freeList`).
- **Para fixar:** *a lista é o "mapa" do que está ocupado; o livre são os buracos do mapa.*

### 3. Lista ordenada por endereço
- **Por quê:** com os nós em ordem, o buraco entre dois vizinhos sai de uma conta: `início do próximo − fim do atual`. Alloc, free e stats viram **um laço simples**.
- **Como:** o `alloc` insere o nó novo **entre** `anterior` e `atual`, que é justamente o buraco escolhido (`mymemory.c:80`). Se `anterior == NULL`, o nó novo vira o `head`.
- **Aula:** inserção em lista (início = `addStart`; meio = `novo->next = atual; anterior->next = novo`).
- **Para fixar:** *ordem de endereço = buracos ficam "entre" os nós.*

### 4. Estratégia First Fit
- **Por quê:** o enunciado deixa escolher. First Fit é a mais simples e a mais rápida: para no primeiro buraco que serve. Best Fit e Worst Fit precisariam olhar **todos** os buracos.
- **Como:** o laço do `alloc` mede cada buraco e dá `break` no primeiro que cabe (`mymemory.c:62`). Se não achar, testa o buraco final, depois do último nó (`:70`). Se também não couber, retorna `NULL`.
- **Para fixar:** *First Fit = "o primeiro que couber, eu pego".*

### 5. Contas de endereço com `char *`
- **Por quê:** `void *` não aceita aritmética em C padrão. `char` tem 1 byte, então somar `n` num `char *` anda **exatamente n bytes**.
- **Como:** `(char *)atual->start + atual->size` = fim do bloco; `(char *)prox - (char *)fim` = tamanho do buraco em bytes.
- **Aula:** aritmética de ponteiros: `p + 1` anda o tamanho do tipo (`char` = 1, `int` = 4); `char *` como "ponteiro de byte" (exercício de mostrar cada byte de um vetor).
- **Para fixar:** *`char *` = régua de bytes.*

### 6. Arredondar o tamanho para múltiplo de 8
- **Por quê:** se alguém pede 10 bytes, o próximo bloco começaria no byte 10, e um `int *` ou `double *` ali ficaria **desalinhado**. Com múltiplos de 8, todo bloco começa num endereço "redondo".
- **Como:** `size = (size + 7) / 8 * 8;` (`mymemory.c:47`). É divisão inteira: 10 vira 16, 16 continua 16.
- **Custo:** desperdiça até 7 bytes por bloco (fragmentação interna). Aparece nos testes: pedimos 10 e o `display` mostra 16.
- **Para fixar:** *+7 e divide = arredonda para cima.*

### 7. `free` libera o **nó**, não o ponteiro do usuário
- **Por quê:** o ponteiro do usuário aponta para **dentro do pool**, não veio de `malloc`. Dar `free` nele quebraria o programa. O que veio de `malloc` foi o nó.
- **Como:** procura o nó com `start == ptr` guardando o `anterior`, religa a lista e faz `free(atual)` (`mymemory.c:110`). Não achou: **não faz nada** (regra do enunciado), e isso protege contra ponteiro inválido e free duplo.
- **Aula:** é o `removeByValue` do exercício, comparando endereço em vez de número.
- **Para fixar:** *tirar o nó da lista = o pedaço do pool volta a ser buraco.*

### 8. `cleanup` libera na ordem certa
- **Por quê:** se liberar o `mymemory_t` primeiro, perdemos o `head` e não conseguimos mais chegar nos nós → vazamento.
- **Como:** percorre a lista guardando o `proximo` **antes** de cada `free`; depois `free(pool)`; por último `free(memory)` (`mymemory.c:183`).
- **Aula:** "não há garbage collector em C"; liberar matriz = primeiro as linhas, depois o vetor de linhas.
- **Para fixar:** *libera de dentro para fora: nós → pool → gerenciador.*

### 9. `stats` calcula os buracos na hora
- **Por quê:** como o livre não é guardado, as estatísticas saem de **um passeio pela lista**, com a mesma conta de buraco do `alloc`.
- **Como:** soma os `size` (memória alocada); livre = `total_size − alocado`; cada buraco > 0 conta como fragmento e é comparado com o maior. Inclui o buraco antes do primeiro nó e o do fim do pool.
- **Para fixar:** *fragmentos = quantos buracos separados; maior bloco = o maior deles.*

### 10. Três arquivos + Makefile
- **Por quê:** o enunciado já deu o `.h`. Separar interface (`mymemory.h`), implementação (`mymemory.c`) e uso (`main.c`) é a modularização vista em aula.
- **Como:** o `.h` tem *include guard* (`#ifndef MYMEMORY_H`). O Makefile compila cada `.c` com `-c` e liga os `.o`. `CFLAGS = -Wall -Wextra -g` liga os avisos.
- **Aula:** Modularização e Makefiles (`gcc -c`, código-objeto, ligação, `make`, `clean`).
- **Para fixar:** *`.h` diz O QUE existe, `.c` diz COMO funciona.*

### 11. Testes automatizados no `main.c`
- **Por quê:** o enunciado pede uns 50 blocos de tamanhos variados, em ordens diferentes, e o critério cobra funcionar sem erro, sem vazamento e com pouca fragmentação.
- **Como:**
  - uma função `verifica(condição, descrição)` conta acertos e falhas, e o programa retorna 1 se algo falhou;
  - `verifica_lista` confere que cada bloco está dentro do pool, em ordem e sem sobrepor o anterior;
  - os blocos são **escritos** de verdade (`memset`) para provar que a memória é utilizável.
- **Resultado:** 22 verificações OK; `leaks --atExit` mostra **0 leaks**; compila sem avisos até com `-pedantic -std=c99`.

### 12. Tipos e `printf`
- `size_t` para tamanhos: é o tipo sem sinal que o `malloc` usa; imprime com `%zu`.
- `%p` imprime endereço.
- No `display`, a posição no pool é `(char *)start - (char *)pool`.

---

## Resultados para mostrar (`make run`)

> Explicação de cada linha da saída, com as contas: [SAIDA-EXPLICADA.md](SAIDA-EXPLICADA.md).

**Exemplo da professora** (pool de 1000): `v` (128) no byte 0, `v2` (64) no byte 128. Depois de `free(v)`, `str` (16) vai para o **byte 0**, porque o First Fit reaproveita o primeiro buraco. Ficam 2 fragmentos livres: 112 bytes (entre `str` e `v2`) e 808 bytes (no fim).

**50 blocos, pool de 10000 bytes:**

| Ordem | Blocos que couberam | Depois de liberar os pares: fragmentos | Blocos de 200 que couberam nos buracos |
|---|---|---|---|
| crescente | 46 de 50 | 24 | 17 de 20 |
| decrescente | 15 de 50 | 8 | 20 de 20 |
| misturada | 40 de 50 | 21 | 20 de 20 |

**Como ler a tabela:**
- Na **decrescente**, os blocos de 1000 entram primeiro e ocupam quase tudo, por isso só 15 cabem.
- Na **crescente**, liberar os pares deixa vários buracos **pequenos** (16, 32, 56 bytes), onde um bloco de 200 não cabe. Por isso só 17 de 20 entram, mesmo havendo memória livre de sobra. Isso é **fragmentação externa**.

---

## Perguntas prováveis (resposta curta)

1. **Por que First Fit?** É a mais simples e para no primeiro buraco que serve. Best/Worst Fit teriam que percorrer a lista inteira para comparar os buracos.

2. **Onde vocês usam `malloc`?** Em três lugares: o `mymemory_t`, o pool e cada nó `allocation_t`. A memória entregue ao usuário nunca vem de `malloc`: é um pedaço do pool.

3. **Como vocês sabem onde tem espaço livre, se a lista só guarda o ocupado?** A lista está em ordem de endereço. O buraco é a distância entre o fim de um nó (`start + size`) e o início do próximo, mais o espaço antes do primeiro e depois do último.

4. **O que acontece se o ponteiro passado ao `free` for inválido?** O laço não acha nenhum nó com aquele `start`, e a função retorna sem fazer nada. Testamos isso com um endereço de variável local, com `NULL` e com free duplo.

5. **Por que `free(atual)` e não `free(ptr)`?** `ptr` aponta para dentro do pool, não foi `malloc`. Quem foi alocado com `malloc` é o nó.

6. **Por que converter para `char *`?** Para fazer conta em bytes. Aritmética em `void *` não é C padrão, e `char` tem 1 byte.

7. **O que é fragmentação? O trabalho de vocês tem?**
   - Externa: memória livre espalhada em buracos pequenos, como no teste crescente, em que blocos de 200 não couberam com 1368 bytes livres.
   - Interna: o arredondamento para 8, que desperdiça até 7 bytes por bloco.
   - O First Fit não junta buracos sozinho, mas buracos vizinhos já viram um só automaticamente, porque o livre é calculado entre os nós ocupados.

8. **Como garantem que não tem vazamento?** O `cleanup` libera nós, pool e gerenciador, nessa ordem. Rodamos `leaks --atExit -- ./t2` e deu 0 leaks.

9. **Por que a lista ordenada?** Sem ordem, para achar os buracos teríamos que comparar todos os nós entre si. Com ordem, basta olhar vizinhos.

10. **O que o `stats` mostra?** Número de alocações, bytes alocados, bytes livres, maior bloco livre contíguo e quantos fragmentos livres existem. Tudo calculado num passeio pela lista.

11. **Por que arredondar para 8?** Alinhamento: `int`/`double` devem começar em endereços múltiplos do seu tamanho. Sem isso, depois de um bloco de 10 bytes, o próximo começaria num endereço "torto".

12. **O que muda se usassem Best Fit?** Só o laço do `alloc`: em vez de `break` no primeiro buraco, percorreria todos guardando o menor que cabe, e inseriria ali. O resto (free, stats, cleanup) não muda.

13. **O `main.c` da professora compilava?** Não: ele chamava `mymemory_malloc` e `mymemory_release`, mas o `.h` declara `mymemory_alloc` e `mymemory_cleanup`. Reproduzimos o mesmo cenário com os nomes do `.h` (Parte 1 do nosso `main.c`).

14. **Para que serve o `#ifndef MYMEMORY_H`?** É o *include guard*: evita definir as structs duas vezes se o `.h` for incluído mais de uma vez.

---

## Comandos

```bash
make          # compila (sem avisos)
make run      # roda os testes
make leaks    # checa vazamento (macOS)
make entrega  # gera T2-mymemory.tar.gz
make clean    # apaga .o e executável
```
