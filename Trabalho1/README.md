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

**Programa/utilitário escolhido:** [`uniq.c`](https://github.com/coreutils/coreutils/blob/master/src/uniq.c) (GNU Coreutils) — a reservar formalmente no Moodle em 08/09.

Motivo da escolha: `uniq.c` já tem 680 linhas (acima do mínimo de 600, sem precisar complementar), implementa um algoritmo simples e bem conhecido (remoção de linhas duplicadas adjacentes), e foi escrito por Richard M. Stallman e David MacKenzie — o que dá bastante material para o critério de "Histórico dos autores". Os utilitários `cat`, `wc` e `head` já foram reservados por outras duplas.

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

O enunciado completo está disponível em [`T1-PSB.pdf`](./T1-PSB.pdf).

## Referências

- [1] GNU Core Utils: https://github.com/coreutils/coreutils/tree/master/src
- [2] GNU Savannah: https://git.savannah.gnu.org/cgit/
- [3] GLIBC: https://github.com/lattera/glibc
