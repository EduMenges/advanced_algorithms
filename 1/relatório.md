# Trabalho 1 (Heaps k-ários e algoritmo de Dijkstra)

## Detalhes de implementação

O `heap` k-ário foi implementado com o `K` fixado em tempo de compilação, através do recurso de `template`s do C++. Isso permitiu otimizar os cálculos dos índices da heap, ao custo de não poder defini-los em tempo de execução, o que para este trabalho não importava.

Ao invés de utilizar a geração de grafos disponibilizada pelo professor, construiu-se uma função que seleciona dois nodos aleatórios e liga-os caso não haja uma aresta prévia e eles não sejam iguais. Isto permite fixar o número de arestas `m` nos casos gerados, acelerar significativamente a criação de novos grafos, e eliminar a dependência da Boost.

## Ambiente de teste

### Máquina

A máquina de teste possui Windows 11, um processador AMD Ryzen 5 7600X e 32 GB de memória RAM. O compilador utilizado foi o MSVC, versão `14.43.34808`, com todas as otimizações padrões habilitadas.

### Parâmetros

Testou-se `K` no intervalo [2, 16], `n` (número de nós) no intervalo de 1000 até 512000 (dobrando a cada iteração), e `m` no intervalo de `2 * n` e dobrando até `((n * (n - 1)) / 2) / 50`, ou seja, metade do número máximo de arestas, com exceção de que o grafo com 512000 nós parou em `1048576000`, em vista de que valores maiores demoraram muito para ser gerado devido à limitação da memória RAM.

Para cada entrada desse conjunto, escolheu-se 10 pares de vértices aleatórios e mensurou-se 10 vezes, a fim de obter uma média.

## Resultados

### Determinação do `K` ótimo

Valores na região [4, 10] mostraram-se melhores. Há um notório viés aos valores superiores conforme o grafo torna-se mais denso, atingindo um limite quando `K=11`. No entanto, `K=13` mostrou-se um valor repetidamente bom para grafos densos e de grande números de nós, o que talvez se dê por ser um ponto de boa localidade espacial com a memória cache do sistema, otimizando a troca de páginas realizada pelo SO.

### Contagem de operações

#### Heap



### Tempo

### Escalonamento

## Análise
