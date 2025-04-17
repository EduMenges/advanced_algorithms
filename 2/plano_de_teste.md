| DFS randomizado            | Caminho mais curto (BFS) | Maior gargalo (fattest path) |
|----------------------------|--------------------------|------------------------------|
| O(E * \|f\|)               | O(V * E²)                | O(log V * E²)                |
| Grafos com alta capacidade | Grafos densos            | Grafos densos                |

Mensurar:
1. Número de iterações
2. Número de operações (número de vértices e arcos tocados)
3. Tempo

Fixar `V` e variar `V` até chegar perto de `V²`. Variar o intervalo de capacidades máximas.