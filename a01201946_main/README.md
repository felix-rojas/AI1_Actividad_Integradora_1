# Reporte

```
Félix Javier Rojas Gallardo
A01201946
```
## Corrida
Ejecute run.sh y genera el archivo de output con las pruebas

## Parte 1

### KMP-search

- Búsqueda garantizada $O(n)$, máximo $O(2n)$
- Complejidad espacial $O(n)$, siendo $n$ la longitud de la palabra a buscar
- Implementación separada para construir la tabla

Charran, C. Lecroq, T. (1997) Knuth-Morris-Pratt algorithm. http://www-igm.univ-mlv.fr/~lecroq/string/node8.html#SECTION0080 

### Alternativas

¿Por qué no Naïve?
- Es $O(n^2)$ para el peor caso, lo descartamos a menos que no podamos utilizar un espacio en memoria mayor al patrón.
- El peor caso siempre se va a dar cuando no exista la transmission maliciosa (mayor parte de los casos)

¿Por qué no Karp?
- Alfabeto pequeño (16) implica múltiples colisiones, lo cual alenta todos los procesos en el hashing.

¿Boyer Moore / Horspool?
- Éste puede ser una buena alternativa, preprocesamiento pequeño 
- Buena alternativa si el código malicioso es muy poco frecuente

Charran, C. Lecroq, T. (1997) Boyer-Moore algorithm. http://www-igm.univ-mlv.fr/~lecroq/string/node14.html#SECTION00140

Charran, C. Lecroq, T. (1997) Horspool algorithm. http://www-igm.univ-mlv.fr/%7Elecroq/string/node18.html


## Parte 2

### Manacher

- Búsqueda garantizada $O(n)$
- Complejidad espacial $O(2n + 2)$, siendo n la longitud de la palabra a buscar con los caracteres adicionales
- Implementación separada para construir la tabla

alex65536, adamant-pwn, bruisedsamurai, et al. (Nov 9, 2023) Manacher's Algorithm - Finding all sub-palindromes in  O(N). https://cp-algorithms.com/string/manacher.html  

### Alternativas

¿Por qué no Naïve?
- Es $O(n^2)$ para el peor caso, lo descartamos a menos que haya MUY poco espacio en memoria.

¿Por qué no Hashing?
- Alfabeto pequeño (16) implica múltiples colisiones.
- Podríamos usar open hashing, pero usaríamos mucha más memoria para los espacios de colisión.

¿Eertree?
- Los árboles palindrómicos requieren $O(n \space log \space n)$ para construir y O(n log n)  para encontrar el palíndromo más largo.
- Se pueden hacer $O(n)$ pero tendríamos que conocer los datos previamante.

¿Suffix Automaton?
- Buena alternativa, pero muy compleja para búsqueda de patrones pequeños.

## Parte 3

### Suffix Automaton

Árbol de sufijos comprimido con todas las subcadenas de una cadena. Descubierto por Blumer et al. en 1983.

#### Complejidad
- $O(n)$  en memoria espacial 
-  $O(n \space log \space k)$  para construcción del árbol
- $O(n)$  para encontrar la LCS

jakobkogler et al. (Nov 9, 2023) Suffix Automaton. https://cp-algorithms.com/string/suffix-automaton.html