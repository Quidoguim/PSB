# Intensivão de C — do zero até a prova

Para quem **não sabe nada de C** e tem pouco tempo. A meta é não zerar: escrever um código simples que funcione, ler um código e dizer o que ele imprime, e acertar o V ou F.

Siga na ordem. Cada lição é curta e termina com um teste rápido. Os gabaritos estão no fim de cada arquivo. Todo o código foi compilado e testado.

## As lições

| # | Lição | Tempo |
|---|---|---|
| [1](1-o-minimo-de-c.md) | O mínimo de C: molde do programa, variáveis, `printf`, `scanf`, contas | 30 min |
| [2](2-if-e-lacos.md) | `if`, `for`, `while`, vetores e os 4 padrões de laço | 45 min |
| [3](3-funcoes.md) | Funções, passagem por valor e por ponteiro | 45 min |
| [4](4-strings-e-structs.md) | Strings e structs | 30 min |
| [5](5-ponteiros-e-memoria.md) | Ponteiros, `malloc`/`free` e lista encadeada | 45 min |
| [6](6-pegadinhas-vf.md) | **As pegadinhas do V ou F** (o mais importante) | 1 h |
| [7](7-teste-de-mesa.md) | Como descobrir "o que imprime" sem errar | 30 min |
| [8](8-simulado.md) | Simulado com gabarito (31 questões; a parte 4 tem 15 códigos no estilo das listas da professora) | 1h30 |

## Plano

**Hoje à noite (~2h)**
1. Lições 1 e 2. Escreva os exemplos no caderno à mão.
2. Lição 6, só as linhas marcadas com ★.
3. Dormir. Sono conta mais que a terceira hora de estudo.

**Amanhã de manhã**
4. Lições 3 e 7.
5. Lição 6 inteira: tape a coluna "É" e responda.

**Amanhã à tarde**
6. Lições 4 e 5.
7. Simulado (lição 8) sem consultar. Depois corrija e refaça o que errou.

**Antes da prova (última hora)**
8. Reler as ★ da lição 6 e a folha abaixo. Nada de assunto novo.

## Na hora da prova: como não zerar

- **Nunca deixe em branco.** Questão de código vale por partes.
- **Questão de escrever função:** comece pela primeira linha (`int soma(int v[], int n) {`), depois o laço, depois o `return`. Mesmo incompleto, isso já mostra que você sabe a estrutura.
- **Questão de "o que imprime":** faça o teste de mesa (lição 7). Uma tabela com uma coluna por variável. Não tente adivinhar de cabeça.
- **V ou F:** para justificar uma falsa, reescreva a frase do jeito certo. Ex.: "F — `strlen` **não** conta o `'\0'`; `strlen("oi")` é 2".
- Não sabe? Responda com o que a regra geral diz. Chutar com justificativa ganha mais que deixar vazio.

## Folha de última hora

```c
#include <stdio.h>

int main() {
    int n;
    scanf("%d", &n);            // & no scanf (menos em string)
    printf("n = %d\n", n);      // \n pula linha
    return 0;
}
```

- `int / int` corta a parte decimal: `7 / 2` é `3`. `%` é o resto: `7 % 2` é `1`.
- `=` guarda valor. `==` compara.
- Vetor `int v[5]` vai de `v[0]` a `v[4]`.
- Laço padrão: `for (i = 0; i < n; i++)`.
- Para a função mudar uma variável de fora: passe `&x` e use `*p` lá dentro.
- String termina em `'\0'`. Compare com `strcmp(a, b) == 0`, nunca com `==`.
- `p->campo` é o mesmo que `(*p).campo`.
- Todo `malloc` tem um `free`.
