# Material para a prova — terça, 10/11

Feito a partir dos slides da Prof. Eduarda (aulas 01 a 10), das apresentações do Prof. Cohen e dos exercícios de fixação do Moodle. Cada arquivo é um assunto, curto o bastante para passar para o caderno.

## Resumos (ler e copiar)

| # | Assunto | Aula no Moodle |
|---|---|---|
| [01](01-basico-de-c.md) | Básico de C: tipos, operadores, printf/scanf, laços | Introdução à Linguagem C |
| [02](02-funcoes-e-parametros.md) | Funções, passagem por valor e por referência | Passagem de parâmetros |
| [03](03-strings.md) | Strings e `string.h` | Strings, structs e unions |
| [04](04-structs-unions.md) | Structs, typedef, unions | Strings, structs e unions |
| [05](05-arquivos.md) | Arquivos texto e binário | Arquivos |
| [06](06-manipulacao-de-bits.md) | Operadores de bits, máscaras, campos de bits | Manipulação de bits |
| [07](07-modularizacao-e-bibliotecas.md) | `.h`/`.c`, Makefile, bibliotecas estática e dinâmica | Modularização |
| [08](08-ponteiros.md) | Ponteiros e aritmética de ponteiros | Ponteiros |
| [09](09-alocacao-dinamica-e-lista.md) | malloc/free, pilha x heap, lista encadeada | Alocação dinâmica |

## Treino (fazer sem olhar o gabarito)

| # | Tipo | Quantidade |
|---|---|---|
| [10](10-verdadeiro-ou-falso.md) | Verdadeiro ou falso, com justificativa das falsas | 48 |
| [11](11-o-que-imprime.md) | "O que imprime?" e "ache o erro" | 35 |
| [12](12-exercicios-de-codificacao.md) | Escrever funções (das listas do Moodle) | 15 |

## Roteiro sugerido

**Domingo (08/11)**
1. Resumos 01 e 02 → V/F 1 a 12 → treino A, B e C.
2. Resumos 03 e 04 → V/F 13 a 21 → treino E e F.
3. Resumo 08 (ponteiros, o mais cobrado) → V/F 39 a 42 → treino D.

**Segunda (09/11)**
4. Resumos 05, 06 e 07 → V/F 22 a 38 → treino G.
5. Resumo 09 → V/F 43 a 48 → treino H.
6. Exercícios de codificação: escrever 5, 7, 9, 12 e 15 à mão, sem consulta.
7. Refazer o que errou nos treinos.

## As 10 pegadinhas que mais caem

1. `int / int` descarta a parte decimal (`5 / 9` é 0).
2. `scanf` precisa de `&`, menos para string.
3. `=` atribui, `==` compara.
4. `switch` sem `break` continua nos casos de baixo.
5. String tem um byte a mais: o `'\0'`. `strlen` não conta, `sizeof` conta.
6. String não se compara com `==` nem se atribui com `=`.
7. Vetor vai para função por referência; variável comum, por valor.
8. `*p++` anda o ponteiro; `(*p)++` muda o valor.
9. `p + 1` anda um **elemento** (`sizeof` do tipo), não um byte.
10. Todo `malloc` precisa de um `free`; nada de usar depois do `free`.
