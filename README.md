# Algoritmo Apriori en C++

## Compilación y ejecución

```bash
g++ -std=c++17 -o apriori apriori.cpp
./apriori
```

Requiere un compilador compatible con **C++17** (GCC 7+, Clang 5+, MSVC 2017+).

---

## ¿Cómo ingresar los datos?

El programa ahora solicita transacciones en tiempo de ejecución.

1. Ejecuta el programa.
2. Ingresa cada transacción en el formato:

```text
T100,I1,I2,I5
```

3. Presiona Enter después de cada transacción.
4. Deja una línea vacía para terminar.
5. Ingresa el soporte mínimo cuando se solicite.

Si no ingresas ninguna transacción, el programa utilizará un conjunto de ejemplo.

### Notas

- El primer valor es el identificador de la transacción (por ejemplo, `T100`).
- Los valores siguientes son los items separados por comas.

El soporte mínimo se ingresa durante la ejecución del programa, después de ingresar las transacciones.