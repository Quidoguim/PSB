# 10 — Treino de Verdadeiro ou Falso

Tape a coluna da direita, responda, e só então confira. Nas falsas, escreva a justificativa no caderno com as suas palavras.

## Básico e funções

1. `7 / 2` em C vale 3.5.
2. `sizeof` devolve o tamanho em bits.
3. Em C, todo programa precisa de uma função `main`.
4. Uma função pode ser definida dentro de outra função.
5. Se a função é definida depois do `main`, é preciso um protótipo antes.
6. C só tem passagem por valor; a por referência é simulada passando o endereço.
7. Vetores são passados para funções por valor (é feita uma cópia).
8. `scanf("%d", x);` lê corretamente um inteiro em `x`.
9. Variável local não inicializada vale zero.
10. Um `switch` sem `break` executa só o `case` escolhido.
11. `do...while` pode não executar nenhuma vez.
12. `if (x = 0)` testa se x é zero.

## Strings, structs, unions

13. A string `"Hello"` ocupa 5 bytes.
14. `strlen` conta o `'\0'`.
15. Duas strings podem ser comparadas com `==`.
16. `strcmp` devolve 0 quando as strings são iguais.
17. Uma struct pode ser copiada inteira com `=`.
18. `p->x` é o mesmo que `(*p).x`.
19. Passar struct por referência é mais barato que por valor.
20. Todos os campos de uma union podem guardar valores ao mesmo tempo.
21. O tamanho de uma union é a soma dos tamanhos dos campos.

## Arquivos

22. `fopen` com modo `"w"` mantém o conteúdo antigo do arquivo.
23. `fopen` devolve `NULL` quando não consegue abrir o arquivo.
24. `fread` e `fwrite` são usados com arquivos texto.
25. `fseek(fp, 0, SEEK_END)` posiciona no início do arquivo.
26. Arquivos binários ocupam mais espaço que os de texto para os mesmos dados.

## Bits

27. `x << 2` altera o valor guardado em x.
28. `x << 1` equivale a multiplicar x por 2.
29. `~` é o NOT lógico e devolve 0 ou 1.
30. Para desligar o bit b: `x & ~(1 << b)`.
31. Existe `%b` no `printf` padrão para mostrar binário.
32. Cada dígito hexadecimal corresponde a 4 bits.

## Modularização

33. O arquivo `.h` contém a implementação das funções.
34. `#ifndef/#define/#endif` evita que um `.h` seja incluído duas vezes.
35. `gcc -c arq.c` gera o executável.
36. O `make` recompila só o que tem dependência mais nova que o alvo.
37. Biblioteca estática é copiada para dentro do executável.
38. Com biblioteca dinâmica, atualizar a biblioteca exige recompilar o programa.

## Ponteiros e memória

39. Se `int *p` aponta para `v[0]`, `p + 1` aponta 1 byte adiante.
40. `v[i]` é o mesmo que `*(v + i)`.
41. `*p++` incrementa o valor apontado por p.
42. Usar um ponteiro não inicializado pode causar falha de segmentação.
43. `malloc` devolve a memória já zerada.
44. Memória do `malloc` é liberada sozinha quando a função termina.
45. Depois de `free(p)`, ainda é seguro ler `*p`.
46. Variáveis locais ficam na pilha; memória de `malloc` fica no heap.
47. Retornar o endereço de uma variável local é seguro.
48. No último nó de uma lista encadeada, `next` vale `NULL`.

---

## Gabarito

| # | | Justificativa (das falsas) |
|---|---|---|
| 1 | F | int / int descarta a parte decimal: dá 3. |
| 2 | F | Devolve em **bytes**. |
| 3 | V | |
| 4 | F | Funções são independentes; não se aninham em C. |
| 5 | V | |
| 6 | V | |
| 7 | F | Vetor é sempre por referência: vai o endereço do 1º elemento, sem cópia. |
| 8 | F | Falta o `&`: `scanf("%d", &x);` |
| 9 | F | Tem lixo; usar é comportamento indefinido. (Globais é que começam em 0.) |
| 10 | F | Sem `break` ele continua executando os `case` de baixo. |
| 11 | F | `do...while` executa **pelo menos uma vez**; o teste é no fim. |
| 12 | F | `=` atribui. Fica x = 0 e a condição é sempre falsa. Comparar é `==`. |
| 13 | F | 6 bytes: 5 letras + `'\0'`. |
| 14 | F | Não conta: `strlen("Hello")` é 5. |
| 15 | F | `==` compara endereços. Usar `strcmp(a, b) == 0`. |
| 16 | V | |
| 17 | V | |
| 18 | V | |
| 19 | V | |
| 20 | F | Os campos dividem a mesma memória; só um vale por vez. |
| 21 | F | É o tamanho do **maior** campo. |
| 22 | F | `"w"` apaga o conteúdo (recria). Para manter e acrescentar, `"a"`. |
| 23 | V | |
| 24 | F | São para binário. Texto usa `fprintf`/`fscanf`/`fgets`. |
| 25 | F | Posiciona no **fim**. Início é `SEEK_SET`. |
| 26 | F | Binários são mais compactos; texto ocupa mais. |
| 27 | F | Só calcula; para mudar x é `x = x << 2;`. |
| 28 | V | (para unsigned, sem estourar) |
| 29 | F | `~` é NOT **bit a bit** (inverte todos os bits). O lógico é `!`. |
| 30 | V | |
| 31 | F | Não existe formato padrão para binário. |
| 32 | V | |
| 33 | F | O `.h` tem só os protótipos; a implementação fica no `.c`. |
| 34 | V | |
| 35 | F | `-c` só compila e gera o `.o` (objeto), sem ligar. |
| 36 | V | |
| 37 | V | |
| 38 | F | Não precisa: o programa só tem uma referência e carrega a biblioteca ao executar. Isso vale para a **estática**. |
| 39 | F | Anda um elemento: `sizeof(int)` bytes (4). |
| 40 | V | |
| 41 | F | Avança o **ponteiro** (lê o valor antes). Para o valor: `(*p)++`. |
| 42 | V | |
| 43 | F | Vem com lixo. Zerada é com `calloc`. |
| 44 | F | Fica até um `free`; se ninguém liberar, é vazamento. |
| 45 | F | A memória não é mais nossa; ler é comportamento indefinido. |
| 46 | V | |
| 47 | F | A variável some quando a função retorna; o endereço fica inválido. |
| 48 | V | |
