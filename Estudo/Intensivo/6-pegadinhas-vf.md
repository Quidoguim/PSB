# Lição 6 — As pegadinhas do V ou F

Como usar: tape a coluna **É** e responda. ★ = cai muito, estude primeiro.

Para justificar uma **falsa**, escreva a frase corrigida (é a coluna "Por quê").

## Básico

| | Se a prova disser... | É | Por quê |
|---|---|---|---|
| ★ | `7 / 2` vale 3.5 | F | int com int corta a vírgula: dá 3. Para 3.5: `7 / 2.0`. |
| | `%` funciona com `float` | F | Só com inteiros. |
| | `sizeof` dá o tamanho em bits | F | Em **bytes**. |
| | C diferencia maiúsculas de minúsculas | V | `Nota` e `nota` são variáveis diferentes. |
| ★ | Variável local não inicializada vale 0 | F | Tem lixo. |
| ★ | `if (x = 5)` testa se x é 5 | F | `=` guarda 5 em x; é sempre verdadeiro. Comparar é `==`. |
| | Em C, 0 é falso e qualquer outro valor é verdadeiro | V | |
| ★ | `do...while` pode não executar nenhuma vez | F | Executa **pelo menos uma vez** (testa no fim). |
| | `while` pode não executar nenhuma vez | V | Testa antes de entrar. |
| ★ | No `switch`, sem `break`, só o `case` escolhido roda | F | Continua rodando os `case` de baixo. |
| | `for (i = 0; i < 5; i++)` roda 5 vezes | V | i = 0, 1, 2, 3, 4. |
| ★ | Em `int v[5]`, o último elemento é `v[5]` | F | É `v[4]`; índice vai de 0 a 4. |
| | C confere se o índice do vetor passou do limite | F | Não confere; o erro é do programador. |
| ★ | Em `b = a++;`, b recebe o valor já somado | F | Recebe o **antigo**; `++a` é que soma antes. |
| ★ | `scanf("%d", x);` está correto | F | Falta `&`: `scanf("%d", &x);` |
| | `printf` pula linha sozinho | F | Precisa de `\n`. |

## Funções

| | Se a prova disser... | É | Por quê |
|---|---|---|---|
| | Uma função pode ser definida dentro de outra | F | Em C, não. |
| | Função `void` devolve um valor | F | `void` = não devolve nada. |
| | Função definida depois do `main` precisa de protótipo antes | V | |
| ★ | Na passagem por valor, a função altera a variável original | F | Recebe uma **cópia**. |
| ★ | Para alterar a variável de quem chama, passa-se o endereço dela | V | `f(&x)` e `*p` na função. |
| ★ | Vetores são passados por cópia | F | Vai o **endereço**; a função mexe no original. |
| | `return` encerra a função | V | |

## Strings, structs, unions

| | Se a prova disser... | É | Por quê |
|---|---|---|---|
| ★ | `"Hello"` ocupa 5 bytes | F | 6: 5 letras + `'\0'`. |
| ★ | `strlen` conta o `'\0'` | F | Não conta: `strlen("Hello")` é 5. |
| ★ | Strings podem ser comparadas com `==` | F | `==` compara endereços. Use `strcmp(a, b) == 0`. |
| | `s = "oi";` copia o texto para o vetor s | F | Não compila. Use `strcpy(s, "oi");` |
| | `strcmp` devolve 1 quando as strings são iguais | F | Devolve **0**. |
| | `scanf("%s", nome)` precisa de `&` | F | Nome de vetor já é endereço. |
| | Struct agrupa campos de tipos diferentes | V | |
| ★ | `p->x` é o mesmo que `(*p).x` | V | |
| | Uma struct pode ser copiada inteira com `=` | V | |
| | O tamanho de uma union é a soma dos campos | F | É o do **maior** campo; os campos dividem a memória. |

## Ponteiros e memória

| | Se a prova disser... | É | Por quê |
|---|---|---|---|
| ★ | `&x` dá o endereço de x e `*p` o valor apontado | V | |
| ★ | Se `p` é `int *`, `p + 1` anda 1 byte | F | Anda 1 elemento: `sizeof(int)` bytes. |
| ★ | `v[i]` é o mesmo que `*(v + i)` | V | |
| ★ | `*p++` soma 1 no valor apontado | F | Anda o **ponteiro**. Para o valor: `(*p)++`. |
| | Ponteiro não inicializado aponta para NULL | F | Aponta para lixo. |
| | `malloc` devolve memória zerada | F | Vem com lixo; `calloc` zera. |
| | `malloc` devolve NULL se não conseguir alocar | V | |
| ★ | Memória do `malloc` é liberada sozinha no fim da função | F | Só com `free`; senão, vazamento. |
| | Depois de `free(p)`, ainda dá para usar `*p` | F | A memória não é mais sua. |
| ★ | Variáveis locais ficam na pilha; `malloc` usa o heap | V | |
| | Retornar o endereço de uma variável local é seguro | F | Ela some quando a função termina. |
| ★ | Na lista encadeada, o `next` do último nó é NULL | V | |

## Arquivos

| | Se a prova disser... | É | Por quê |
|---|---|---|---|
| ★ | `fopen` com `"w"` mantém o conteúdo antigo | F | `"w"` apaga. Para acrescentar no fim: `"a"`. |
| | `fopen` devolve NULL quando falha | V | |
| | `fread`/`fwrite` são para arquivo texto | F | São para **binário**. Texto: `fprintf`/`fscanf`. |
| | `fclose` não é necessário | F | Sem ele, dados podem não chegar ao disco. |

## Bits

| | Se a prova disser... | É | Por quê |
|---|---|---|---|
| | `x << 1` equivale a multiplicar x por 2 | V | |
| | `~` é o NÃO lógico | F | `~` inverte bit a bit. O lógico é `!`. |
| | `&` e `&&` fazem a mesma coisa | F | `&` é bit a bit; `&&` é o E lógico. |
| | Cada dígito hexadecimal vale 4 bits | V | |

## Modularização

| | Se a prova disser... | É | Por quê |
|---|---|---|---|
| ★ | O `.h` contém a implementação das funções | F | Só protótipos; o corpo fica no `.c`. |
| | `#ifndef`/`#define`/`#endif` evita incluir o `.h` duas vezes | V | |
| | `gcc -c arq.c` gera o executável | F | Gera o `.o` (objeto), sem ligar. |
| | O `make` sempre recompila tudo | F | Só o que tem dependência mais nova. |
| | Biblioteca estática é copiada para dentro do executável | V | |
| | Com biblioteca dinâmica, atualizar a biblioteca exige recompilar o programa | F | Ela é carregada na execução. |
