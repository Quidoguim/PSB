# Trabalho 2 — Planejamento

Gerenciador de memória com *pool* (ver [enunciado](T2-PSB.pdf)). Este documento é o roteiro: o que vamos construir, em que ordem, e que parte da matéria cada módulo usa. O material de estudo de cada assunto está em [Estudo/](../Estudo/).

Cada módulo segue o mesmo ciclo:

1. Implementar só aquele pedaço.
2. Compilar com `make` (zero avisos) e rodar um teste pequeno.
3. Explicação do que foi feito, ligando com a aula.
4. Tu escreves tuas anotações em `anotacoes/` (um arquivo por módulo).
5. Commit.

Só passamos para o próximo módulo quando o anterior estiver entendido.

---

## A ideia em uma figura

```
mymemory_t
+-------------+
| pool  ------+------> [##########............######.............]  (1 malloc grande)
| total_size  |          ^          ^           ^
| head  --+   |          |          |           |
+---------|---+        start=0    (livre)     start=600
          v
   allocation_t            allocation_t
   +-----------+           +-----------+
   | start     |---------> | start     |
   | size =100 |           | size =60  |
   | next  ----+---------> | next=NULL |
   +-----------+           +-----------+
```

- O **pool** é um único bloco pedido com `malloc` no `init`. Toda memória entregue ao usuário sai de dentro dele.
- A **lista** guarda só o que está ocupado. Cada nó (`allocation_t`) diz onde a alocação começa (`start`) e quantos bytes tem (`size`).
- O **espaço livre** não fica guardado em lugar nenhum: ele é o que sobra entre um nó e o próximo.

## Decisões de projeto (propostas — confirmar antes do Módulo 1)

| # | Decisão | Proposta | Por quê |
|---|---|---|---|
| D1 | Estratégia de alocação | **First Fit** | A mais simples de explicar: percorre os buracos em ordem e usa o primeiro que couber. O enunciado deixa escolher uma. |
| D2 | Ordem da lista | **Ordenada por endereço** (`start` crescente) | Com a lista em ordem, o buraco livre entre dois nós vizinhos é só `próximo->start - (atual->start + atual->size)`. Alloc, free e stats viram laços simples. |
| D3 | Contas de endereço | Converter para `char *` antes de somar | Somar em `void *` é extensão do GNU, não é C padrão (o compilador avisa com `-pedantic`). Com `char *`, `+1` anda exatamente 1 byte. É o "ponteiro para byte" dos exercícios de aritmética de ponteiros. |
| D4 | Alinhamento | Arredondar `size` para múltiplo de 8 | Sem isso, um `int*` pode cair num endereço ímpar (ex.: depois de um bloco de 10 bytes). A conta é uma linha: `(size + 7) / 8 * 8`. Alternativa mais simples: não alinhar, e explicar isso na apresentação. |
| D5 | `mymemory_alloc` com `size == 0` | Retorna `NULL` | Não faz sentido criar um nó de 0 bytes. |
| D6 | Programa de teste | Nosso `main.c`, com o cenário da professora (nomes corrigidos) + bateria de ~50 blocos | O `main.c` dela não compila (chama `mymemory_malloc`/`mymemory_release`). Mantemos o original intocado em `codigo-base/` como referência. |

## Estrutura de arquivos (ao final)

```
Trabalho2/
├── mymemory.h        # interface (cópia do header da professora, sem mudar assinaturas)
├── mymemory.c        # implementação das 6 funções
├── main.c            # cenário da professora + bateria de testes
├── Makefile          # make / make run / make clean
├── anotacoes/        # tuas anotações, um arquivo por módulo
├── codigo-base/      # arquivos originais da professora (referência, intocados)
└── PLANEJAMENTO.md   # este arquivo
```

Na entrega vai um `.tar.gz` só com `mymemory.h`, `mymemory.c`, `main.c` e `Makefile`.

---

## Módulos

### Módulo 0 — Esqueleto do projeto e Makefile

- **O que fazer:** criar `mymemory.h` (cópia), `mymemory.c` com as 6 funções vazias, `main.c` mínimo e o `Makefile` com `-Wall -Wextra`. Compilar e rodar sem fazer nada.
- **Matéria:** Modularização (`.h` com protótipos e *include guard* `#ifndef`, `.c` que inclui o próprio `.h`, `gcc -c` gerando `.o`, ligação, `make`, `CFLAGS`, `clean`). → [Estudo/05-modularizacao-e-makefile.md](../Estudo/05-modularizacao-e-makefile.md)
- **Pronto quando:** `make` compila sem nenhum aviso e `make clean` apaga tudo.

### Módulo 1 — `mymemory_init` e `mymemory_cleanup`

- **O que fazer:** `init` aloca o `mymemory_t` e o pool, guarda o tamanho, começa com `head = NULL`; testa se o `malloc` deu `NULL`. `cleanup` percorre a lista dando `free` em cada nó, depois libera o pool e por último o `mymemory_t`.
- **Matéria:** alocação dinâmica (`malloc` retorna `NULL` se falhar; `free`; "não há garbage collector"), structs com `typedef` e acesso por ponteiro com `->`, ordem de liberação (igual à matriz: primeiro as linhas, depois o vetor de linhas), percorrer lista (igual ao `freeList` do exercício). → [04](../Estudo/04-alocacao-dinamica.md), [02](../Estudo/02-structs.md), [06](../Estudo/06-lista-encadeada.md)
- **Pronto quando:** `init` + `cleanup` rodam e `leaks --atExit` mostra 0 vazamentos.

### Módulo 2 — `mymemory_display`

- **O que fazer:** percorrer a lista e imprimir cada alocação: endereço (`%p`), posição dentro do pool (em bytes) e tamanho.
- **Por que agora:** ver a lista na tela ajuda a testar os módulos seguintes.
- **Matéria:** `%p`, subtração de ponteiros (`(char *)start - (char *)pool` = distância em bytes), laço `while (atual != NULL)` do `show`. → [03](../Estudo/03-ponteiros.md), [06](../Estudo/06-lista-encadeada.md)
- **Pronto quando:** imprime "nenhuma alocação" com a lista vazia (os outros casos testamos no Módulo 3).

### Módulo 3 — `mymemory_alloc` (First Fit)

É o módulo central. Vamos quebrar em passos:

1. Buraco **antes do primeiro nó** (do início do pool até `head->start`).
2. Buracos **entre nós vizinhos**.
3. Buraco **depois do último nó** (até o fim do pool).
4. Achou um buraco que cabe: cria o nó com `malloc`, preenche `start`/`size` e **insere na posição certa** (início, meio ou fim da lista) para manter a ordem.
5. Não achou: retorna `NULL`.

- **Matéria:** aritmética de ponteiros com `char *`, `void *` como "ponteiro sem tipo", inserção em lista encadeada (o `addStart` e o `add` do exercício, mais o caso novo de inserir no meio). → [03](../Estudo/03-ponteiros.md), [06](../Estudo/06-lista-encadeada.md)
- **Pronto quando:** três alocações seguidas aparecem em ordem no `display`; uma alocação maior que o pool retorna `NULL`; dá para escrever nos blocos (`v[i] = i + 1`) sem quebrar nada.

### Módulo 4 — `mymemory_free`

- **O que fazer:** procurar o nó com `start == ptr`, guardando o anterior. Achou: tira da lista e dá `free` no nó. Não achou: não faz nada.
- **Detalhe importante:** o `free` é do **nó** (que veio de `malloc`), nunca do `ptr` (que é um pedaço do pool).
- **Matéria:** é o `removeByValue` do exercício, comparando endereços em vez de inteiros. → [06](../Estudo/06-lista-encadeada.md)
- **Pronto quando:** liberar o bloco do meio abre um buraco que uma nova alocação reaproveita; liberar um ponteiro inválido (ou o mesmo duas vezes) não quebra nada.

### Módulo 5 — `mymemory_stats`

- **O que fazer:** um laço pela lista calculando:
  - número de alocações e total alocado (somar os `size`);
  - total livre (`total_size - alocado`);
  - maior bloco livre e número de fragmentos livres (mesma conta de buracos do Módulo 3, contando só buracos com tamanho > 0).
- **Matéria:** laços e acumuladores, a mesma aritmética de ponteiros do Módulo 3. → [03](../Estudo/03-ponteiros.md)
- **Pronto quando:** os números batem com uma conta feita à mão num cenário pequeno.

### Módulo 6 — Bateria de testes (~50 blocos)

- **O que fazer:** no `main.c`, alocar ~50 blocos de tamanhos variados (10, 25, 50, 100, 250, 500, 1000...) em **ordens diferentes** (crescente, decrescente, misturada); liberar alguns intercalados para criar buracos; alocar de novo; mostrar `display`/`stats` em pontos-chave; terminar com `cleanup`.
- **Checagens:** `make` sem avisos; `leaks --atExit -- ./t2` com 0 vazamentos; nenhum retorno fora do pool.
- **Matéria:** Makefile (`make run`), e a ideia de teste automatizado que usamos no T1.
- **Pronto quando:** a saída mostra a fragmentação mudando com a ordem das alocações, e isso dá para explicar na apresentação.

### Módulo 7 — Aplicação rodando e apresentação

- Rodar a demo completa do zero (`make clean && make run`).
- Registrar **como a IA foi usada** (o enunciado exige) — texto curto para a entrega.
- Roteiro da apresentação (~10 min): a figura do pool, a decisão First Fit + lista ordenada, `alloc` e `free` linha a linha, a saída dos testes.
- Gerar o `.tar.gz` da entrega.

---

## Pendências fora do código

- Data da apresentação/entrega: ainda não está no Moodle (não há "sala de entrega" do T2 na seção TDE).
- Grupo: escolher dupla/trio na atividade "Escolha de Duplas ou Trios" do Moodle.
