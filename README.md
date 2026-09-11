# Trabalho 1 – Programação de Software Básico

Repositório dedicado ao Trabalho 1 da disciplina de Programação de Software Básico (PUCRS/Escola Politécnica). O trabalho consiste no estudo, compreensão e apresentação de um utilitário do GNU e/ou de funções da biblioteca padrão da linguagem C (conforme implementada no GNU C), com foco nos idiomas e práticas típicas da programação profissional em C no contexto de software básico.

## Objetivo

Conhecer e apresentar idiomas e práticas da programação em **linguagem C** no contexto de **software básico** (sistemas operacionais, compiladores, utilitários e bibliotecas).

> Atenção: não são aceitos trabalhos sobre programas escritos em C++, C#, Objective-C ou similares.

## Enunciado (resumo)

- A dupla deve escolher um utilitário do GNU (ex.: [GNU Coreutils](https://github.com/coreutils/coreutils/tree/master/src), [GNU Savannah](https://git.savannah.gnu.org/cgit/)) e/ou funções da biblioteca padrão de C ([GLIBC](https://github.com/lattera/glibc)).
- O código-fonte estudado deve cobrir **no mínimo 600 linhas** (incluindo comentários e linhas em branco), no texto original do repositório escolhido. Se o programa escolhido tiver menos linhas, devem ser incluídos programas/funções adicionais até atingir o mínimo.
- O utilitário `echo` **não pode ser reservado** (é usado como exemplo no relatório disponibilizado pela disciplina).

## Modalidade do Trabalho

- Trabalho em **duplas** (máximo dois integrantes).
- Reserva prévia do programa/função de biblioteca escolhido, feita na área indicada no Moodle.
- Apenas **uma reserva por programa/função principal** — em caso de duplicidade, vale a primeira reserva registrada.
- Após a data de reserva, não é mais possível reservar novos programas, apenas ajustar os já alocados.

## Cronograma

| Etapa | Data |
|---|---|
| Escolha de grupos e programas | 08/09 |
| Entrega | 22/09 |

**Programa/utilitário escolhido:** [`sort.c`](https://github.com/coreutils/coreutils/blob/master/src/sort.c) (GNU Coreutils) — a reservar formalmente no Moodle em 08/09.

`sort.c` foi escrito originalmente por Mike Haertel (dezembro de 1988) e é mantido hoje por Paul Eggert — ambos com bastante material disponível para o critério de "Histórico dos autores" (Eggert também é conhecido pela manutenção do banco de dados de fusos horários, o tz database). É um dos utilitários mais extensos do GNU Coreutils (mais de 5000 linhas), o que dá bastante espaço para os critérios de "Análise de blocos" e "Truques" (implementa merge sort externo, comparação por múltiplas chaves, otimizações de I/O e paralelismo) — vale a pena focar a apresentação em um subconjunto bem escolhido de funções, já que cobrir o arquivo inteiro em 10 minutos de vídeo não é viável.

`cat`, `wc`, `head` e `uniq.c` já haviam sido reservados por outras duplas.

## Progresso

- [x] Dupla formada, tema escolhido: `sort.c`
- [ ] Reserva formal confirmada no Moodle (prazo era 08/09)
- [x] Código-fonte baixado e versionado — [Trabalho1/sort.c](./Trabalho1/sort.c), commit de referência [`bff0e54`](https://github.com/coreutils/coreutils/commit/bff0e54) do GNU Coreutils (branch `master`, 06/09/2026)
- [x] Subconjunto de funções definido para o relatório/apresentação (ver abaixo)
- [x] Histórico dos autores (Mike Haertel, Paul Eggert) levantado
- [x] Convenções de codificação identificadas
- [ ] Aritmética de ponteiros mapeada no subconjunto
- [ ] Blocos de responsabilidade e dependências divididos
- [ ] Diagramas estático e dinâmico
- [ ] Referência acadêmica levantada
- [ ] Exemplo de uso com depurador (stack/heap)
- [ ] Makefile / testes automatizados
- [ ] Relatório escrito
- [ ] Vídeo gravado e publicado (prazo 22/09)

### Subconjunto de código selecionado

~740 linhas de `sort.c`, cobrindo um fluxo único (buffer → extração de chave → comparação → dispatch). Fora do escopo: `main()` (558 linhas) e a maquinaria de threads/merge.

| Função | Linhas | Motivo |
|---|---|---|
| `try_growbuf`/`maybe_growbuf` | 1802-1861 | truque: buffer dobra de tamanho (realloc amortizado) |
| `begfield` | 1862-1908 | aritmética de ponteiros — início do campo |
| `limfield` | 1909-2018 | aritmética de ponteiros — fim do campo, contador regressivo |
| `fillbuf` | 2019-2169 | leitura de entrada/EOF, monta tabela de linhas via ponteiros |
| `keycompare` | 2946-3140 | núcleo: comparação multi-chave |
| `compare` | 3141-3187 | desempate por linha inteira |
| `sort()` | 4315-4444 | dispatcher memória vs. arquivo temporário; contém o único `goto` do arquivo fora de `check()` |

Detalhes e raciocínio completo: [CLAUDE.md](./CLAUDE.md#código-fonte).

### Histórico dos autores

**Mike Haertel** (St. Olaf College) escreveu o GNU grep, GNU diff e GNU sort na FSF em 1988, depois seguiu carreira em arquitetura de CPU (Intel — Pentium 4 —, depois AMD, hoje Intel de novo). **Paul Eggert** (PhD UCLA 1980) é o mantenedor atual de `sort.c`, mantém o tz database da IANA desde 2005 e contribui com Autoconf, Diffutils, RCS, gzip, GCC, glibc e GNU Coreutils; recebeu o FSF Award for the Advancement of Free Software em 2021.

Fontes e detalhes completos: [CLAUDE.md](./CLAUDE.md#histórico-dos-autores).

### Convenções de codificação

Estilo GNU clássico no subconjunto escolhido: 2 espaços de indentação (sem tabs no código), chave de bloco em linha própria, espaço entre nome de função e `(` (`malloc (alloc)`), `*` de ponteiro colado à variável (`char *ptr`), identificadores em `snake_case` e macros em MAIÚSCULO, margem em torno de 80 colunas.

Detalhes e exemplos com número de linha: [CLAUDE.md](./CLAUDE.md#convenções-de-codificação).

## Entrega

- Vídeo de **até 10 minutos**, apresentando os critérios de avaliação abaixo.
- Link do vídeo hospedado em sistema de compartilhamento de mídia (ex.: Zoom, YouTube).
- Participação de **todos os integrantes da dupla é obrigatória**; ausência gera desconto de 30% na nota.
- Cada participante deve se identificar antes de falar e, se possível, manter a webcam ligada.
- A responsabilidade pelo envio e reprodução do vídeo (link válido, sem falhas de áudio/imagem) é dos autores — falhas nesse sentido resultam em trabalho não avaliado.

## Critérios de Avaliação

| Item | Pontuação |
|---|---|
| Histórico dos autores do código, instituição/projeto relacionado | 1 ponto |
| Convenções de codificação (alinhamento, margem, identificadores) | 1 ponto |
| Manipulação de ponteiros e vetores (aritmética de ponteiros) | 1 ponto |
| Análise de blocos de código e funções auxiliares | 2 pontos |
| Truques característicos da linguagem C ("coisas de programador C") | 2 pontos |
| Referência acadêmica relacionada ao programa/autores | 1 ponto |
| Exemplo de uso do programa (execução, consumo de memória, depurador/diagrama) | 1 ponto |
| Diagrama estático e dinâmico (UML ou notação similar) | 1 ponto |
| Construção e testes automatizados (make, bibliotecas etc.) | 1 ponto |

## Documento original

O enunciado completo está disponível em [`T1-PSB.pdf`](./Trabalho1/T1-PSB.pdf).

## Referências

- [1] GNU Core Utils: https://github.com/coreutils/coreutils/tree/master/src
- [2] GNU Savannah: https://git.savannah.gnu.org/cgit/
- [3] GLIBC: https://github.com/lattera/glibc
