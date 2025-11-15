# Objetivo
Este repositório é fruto de um trabalho para a disciplina de Algoritmos e Estruturas de Dados 2. Seu objetivo consiste em construir uma *CLI* que faz a indexação de arquivos e busca de termos.

# Como executar?
Compile o código com:
```cpp
make
```
## Modo indexador
```sh
./indice construir ./caminho/para/o/diretorio
```
Isso gerará um arquivo index.dat na saída.

## Modo busca
```sh
./indice busca <palavra 1> <palavra 2> ... <palavra $N$>
```

Isso retornará uma lista dos arquivos que contém essas palavras.

