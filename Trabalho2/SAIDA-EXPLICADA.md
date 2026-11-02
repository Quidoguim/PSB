# T2 — A saída do `make run`, linha por linha

Este guia mostra o que cada linha da saída significa, de onde vem cada número e o que ele prova. Cada conta pode ser refeita no papel.

**Duas regras explicam quase todos os números:**
1. **Arredondamento para 8:** todo pedido vira múltiplo de 8 (`(size + 7) / 8 * 8`).
   - 10 → 16;
   - 25 → 32;
   - 50 → 56;
   - 100 → 104;
   - 250 → 256;
   - 500 → 504;
   - 1000 → 1000;
   - 200 → 200.
2. **First Fit:** o bloco vai para o **primeiro** buraco, em ordem de endereço, onde ele cabe.

> Os endereços (`0x1009ca790` e outros) **mudam a cada execução**, porque o sistema entrega o pool em lugares diferentes. Os números "bytes X a Y do pool" **não mudam**, porque são posições relativas ao início do pool.

---

## Parte 1 — Exemplo da professora (pool de 1000 bytes)

```
Depois de alocar v (128 bytes) e v2 (64 bytes):
--- Alocacoes (pool de 1000 bytes em 0x1009ca790) ---
 1) inicio 0x1009ca790 | bytes 0 a 127 do pool | tamanho 128
 2) inicio 0x1009ca810 | bytes 128 a 191 do pool | tamanho 64
```

| Trecho | Significado |
|---|---|
| `pool de 1000 bytes em 0x...790` | `total_size` e o endereço que o `malloc` do `init` devolveu |
| `1)`, `2)` | ordem na lista encadeada, que é também a ordem de endereço |
| `inicio 0x...` | o ponteiro que o `alloc` devolveu ao usuário (`start` do nó) |
| `bytes 0 a 127` | posição dentro do pool: `start - pool` até `start - pool + size - 1` |
| `tamanho 128` | `v` = 32 `int` × 4 bytes = 128 (já é múltiplo de 8) |
| `v2` em `128 a 191` | 16 `int` × 4 = 64 bytes, colado logo depois de `v` |

**Para mostrar:** `0x...810 − 0x...790 = 0x80 = 128` em decimal. A distância entre os dois endereços é exatamente o tamanho de `v`. É a aritmética com `char *` aparecendo na saída.

```
Depois de free(v) e alocar str (16 bytes) -> "ola mundo":
 1) inicio 0x1009ca790 | bytes 0 a 15 do pool | tamanho 16
 2) inicio 0x1009ca810 | bytes 128 a 191 do pool | tamanho 64
```

- `free(v)` tirou o nó de `v` da lista, e os bytes 0 a 127 viraram buraco.
- `str` (16 bytes) foi para o **byte 0**, com o mesmo endereço que `v` tinha. É o First Fit: o primeiro buraco (0 a 127) já serve.
- `v2` não se mexeu: continua em 128 a 191, e o teste confere que os valores dele estão intactos.
- `"ola mundo"` prova que a memória entregue é usável: o `sprintf` escreveu nela.

```
--- Estatisticas ---
Numero de alocacoes:        2
Memoria alocada:            80 bytes
Memoria livre:              920 bytes
Maior bloco livre contiguo: 808 bytes
Fragmentos livres:          2
```

| Linha | Valor | De onde vem |
|---|---|---|
| Número de alocações | 2 | nós na lista: `str` e `v2` |
| Memória alocada | 80 | soma dos `size`: 16 + 64 |
| Memória livre | 920 | `total_size − alocado`: 1000 − 80 |
| Maior bloco livre contíguo | 808 | buraco do fim: do byte 192 ao 999 = 1000 − 192 |
| Fragmentos livres | 2 | buraco 16–127 (112 bytes) + buraco 192–999 (808 bytes) |

**Confere:** 112 + 808 = 920, a memória livre total. Os 920 livres **não** estão juntos: o maior pedido que cabe é 808. Essa diferença é a fragmentação.

---

## Parte 2 — Casos especiais (pool de 100 bytes)

```
===== Parte 2: casos especiais =====
Casos especiais executados.
```

Só aparece uma linha porque **verificação que passa não imprime nada**. Se alguma falhasse, apareceria `FALHOU: <descrição>`. Os 6 testes desta parte:

| Teste | Por que dá esse resultado |
|---|---|
| `alloc(0)` → `NULL` | decisão nossa: pedido de 0 bytes não faz sentido |
| `alloc(101)` → `NULL` | 101 arredonda para 104, maior que o pool de 100 |
| `alloc(96)` funciona | 96 já é múltiplo de 8 e cabe nos 100 |
| `alloc(1)` → `NULL` | 1 vira 8, mas só sobraram 4 bytes (100 − 96) |
| `free` de endereço local / `NULL` | o laço não acha nó com esse `start`, então não faz nada, e a lista continua com 1 nó |
| `free` duplo | o 1º `free` remove o nó; no 2º o nó não existe mais, então não faz nada |

---

## Parte 3 — 50 blocos, pool de 10000 bytes

Cada ordem faz 3 etapas e imprime estatísticas depois de cada uma:
1. **aloca** os 50 blocos;
2. **libera os de índice par** (0, 2, 4…), o que cria buracos intercalados;
3. **tenta 20 blocos novos de 200 bytes** nesses buracos.

Os tamanhos usados são 10, 25, 50, 100, 250, 500 e 1000, que viram 16, 32, 56, 104, 256, 504 e 1000.

### Ordem crescente: 8×10, 7×25, 7×50, 7×100, 7×250, 7×500, 7×1000

```
Alocados 46 de 50 blocos.
Memoria alocada: 9792 | livre: 208 | maior: 208 | fragmentos: 1
```

- Soma dos 43 primeiros: 8×16 + 7×32 + 7×56 + 7×104 + 7×256 + 7×504 = **6792**.
- Cabem mais 3 blocos de 1000, o que dá **9792**. O 4º precisaria chegar a 10792, maior que 10000. Por isso **46 de 50**: os 4 últimos blocos de 1000 recebem `NULL`.
- Sobram 208 bytes, **todos no fim** do pool, por isso 1 fragmento.

```
Depois de liberar os blocos de indice par:
Alocacoes: 23 | alocada: 5232 | livre: 4768 | maior: 1000 | fragmentos: 24
```

- 23 blocos liberados: os pares de 0 a 44. Os pares 46 e 48 nunca foram alocados, e `free(NULL)` não faz nada.
- **24 fragmentos** = 23 buracos novos + o buraco de 208 do fim. Os buracos não se juntam porque entre dois pares sempre há um ímpar ocupado.
- Maior buraco = **1000**, o lugar do bloco 44.

```
Novos blocos de 200 bytes que couberam nos buracos: 17 de 20
Alocacoes: 40 | alocada: 8632 | livre: 1368 | maior: 104 | fragmentos: 23
```

- **Buracos onde o 200 cabe:**
  - 3 de 256 → 1 bloco cada = 3;
  - 4 de 504 → 2 cada = 8;
  - 1 de 1000 → 5;
  - o do fim (208) → 1.
  - Total: **17**.
- **Buracos onde não cabe:** os de 16, 32, 56 e 104.
- **Ponto principal da apresentação:** sobram **1368 bytes livres** e mesmo assim um bloco de 200 **não cabe**, porque o maior buraco tem só 104 bytes. Isso é **fragmentação externa**.

### Ordem decrescente: 8×1000, 7×500, 7×250, 7×100, 7×50, 7×25, 7×10

```
Alocados 15 de 50 blocos.
Memoria alocada: 9992 | livre: 8 | maior: 8 | fragmentos: 1
```

- 8×1000 = 8000.
- **Blocos de 500 (504 cada):** cabem 3, total 9512, e o 4º não cabe.
- **Blocos menores:** um de 256 (9768), dois de 104 (9976) e um de 16 (**9992**).
- Os blocos grandes entram primeiro e ocupam quase tudo, por isso só **15** cabem.
- Sobram 8 bytes no fim.

```
Depois de liberar os blocos de indice par:
Alocacoes: 8 | alocada: 4880 | livre: 5120 | maior: 1000 | fragmentos: 8
```

- Ficam 8 blocos: 4 de 1000, 1 de 504, 1 de 256, 1 de 104 e 1 de 16. A soma dá 4880.
- **Buracos:** 4 de 1000, 2 de 504, 1 de 104 e o de 8 do fim, ou seja, **8** buracos.

```
Novos blocos de 200 bytes que couberam nos buracos: 20 de 20
Alocacoes: 28 | alocada: 8880 | livre: 1120 | maior: 504 | fragmentos: 4
```

- Cada buraco de 1000 recebe 5 blocos de 200, sem sobra. Os 4 primeiros buracos já bastam para os **20**.
- Ficam 4 buracos: 504, 504, 104 e 8. Somam 1120.

### Ordem misturada: 10, 25, 50, 100, 250, 500, 1000, repetindo

```
Alocados 40 de 50 blocos.
Memoria alocada: 9992 | livre: 8 | maior: 8 | fragmentos: 1
```

- Uma volta completa (16+32+56+104+256+504+1000) ocupa 1968 bytes.
- **5 voltas:** 9840 bytes, 35 blocos.
- **6ª volta:** cabem 16, 32 e 56 (9944). O de 104 já não cabe.
- **7ª volta:** cabem 16 e 32 (**9992**), total de **40** blocos.

```
Depois de liberar os blocos de indice par:
Alocacoes: 20 | alocada: 4680 | livre: 5320 | maior: 1000 | fragmentos: 21
```

- 20 blocos liberados, que viram 20 buracos. Somando o buraco de 8 do fim, são **21** fragmentos.

```
Novos blocos de 200 bytes que couberam nos buracos: 20 de 20
Alocacoes: 40 | alocada: 8680 | livre: 1320 | maior: 400 | fragmentos: 19
```

- O First Fit enche os buracos **em ordem de endereço**: 256 → 1, 1000 → 5, 504 → 2, 256 → 1, 1000 → 5, 504 → 2, 256 → 1, e o último buraco de 1000 recebe só 3.
- Os **20** blocos cabem.
- Os 400 que sobram nesse último buraco de 1000 (1000 − 3×200) formam o maior bloco livre.

### Resumo para comparar as três ordens

| Ordem | Couberam | Fragmentos depois do free | Blocos de 200 que couberam |
|---|---|---|---|
| crescente | 46 de 50 | 24 | 17 de 20 |
| decrescente | 15 de 50 | 8 | 20 de 20 |
| misturada | 40 de 50 | 21 | 20 de 20 |

**Leitura:**
- **A ordem dos pedidos muda tudo.** Blocos grandes primeiro (decrescente) enchem o pool com poucos blocos.
- **Blocos pequenos primeiro (crescente)** deixam, depois do `free`, muitos buracos pequenos demais para 200 bytes.
- **O First Fit é simples e funciona bem quando os buracos são grandes.** Quando os buracos são pequenos, aparece a fragmentação externa.

---

## Última linha

```
===== Resultado: 22 verificacoes OK, 0 falhas =====
```

**Contagem:**
- Parte 1: 7 verificações: `init`, os 3 `alloc`, First Fit reaproveitou o buraco, `v2` intacto e `verifica_lista`.
- Parte 2: 6 verificações.
- Parte 3: 3 ordens × 3 chamadas de `verifica_lista` = 9 verificações.
- A `verifica_lista` confere, para cada bloco: está dentro do pool, está em ordem de endereço e não sobrepõe o anterior.
- Total: 7 + 6 + 9 = **22**.

**Retorno do programa:** com 0 falhas, o `main` retorna 0 e o `make` termina sem erro. Com alguma falha, retorna 1 e o `make` acusa erro. É isso que torna o teste **automatizado**: não é preciso ler a saída para saber se passou.
