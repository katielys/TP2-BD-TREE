Autores:
 * Katiely dos Santos Oliveira Moreira - 21600808
 * Tiago dos Santos de Souza - 21600650
## Objetivo
Este trabalho consiste na implementação de programas para armazenamento e pesquisa de dados utilizando as estruturas de arquivo e índice estudadas em sala de aula. Os programas devem fornecer suporte para a inserção, assim como diferentes meios de busca, seguindo as técnicas  de organização e indexação de arquivos.

##Como Executar
O projeto foi desenvolvido em C++ no linux, para compilar todos os arquivos use no terminal make, 
isso gerará 4 executaveis: `upload`, `findrec`, `seek1`, `seek2`

 * O executavel `upload` recebe como parametro o caminho do arquivo de dados que será indexado.
Esse executável é responsavel por indexar todos os dados. Portanto, deve ser o primeiro a ser executado.
Exemplo de execução:
``` 
./upload /home/taigo/Documents/2018.2/BD/TP2-BD-TREE/data/arquivo.entrada.tp2.bd1.artigos.csv

```

 * O executavel `findrec` recebe como parametro um id de registro, retornando-o se o encontrar juntamente com a quantidade de blocos lidos para tal.
 
 Exemplo de execução:
``` 
./finrec 123

```

* O executavel `seek1` recebe como parametro um id de registro, retornando-o se o encontrar juntamente com a quantidade de blocos lidos para tal e a quantidade total de blocos do arquivo de indice.
 
 Exemplo de execução:
``` 
./seek1 123

```

* O executavel `seek2` recebe como parametro o titulo de um registro, retornando-o se o encontrar juntamente com a quantidade de blocos lidos para tal e a quantidade total de blocos do arquivo de indice.
 
 Exemplo de execução:
``` 
./seek2 "Poster: 3D"

```
 

##Decisoes de Projeto

Foi escolhido implementar em linguagem C++, ambos os autores possuem mais experiencia ao trabalhar com arquivos em linguagem C++.
 
O tamanho do bloco foi de 4Kbytes pois ambos os componentes possuem tamanho do bloco físico de 4k em seus computadores.

Os buckets do arquivo de dados possuem o tamanho de um bloco. A estrutura de bloco foi implementada de modo que servisse para ser um bucket.
Nela mantemos um offset que serve para indicar o endereço de seu respectivo bloco de overflow. Os blocos de overflow também possuem a mesma estrutura.
Com esse esquema, é possivel manter uma "lista encadeada" lógica de blocos, dessa forma implementando o tratamento de colisões.

Outra decisão importante foi criar dois arquivos, o primeiro servindo para o arquivo principal de hashing e o segundo servindo como área de overflow. Esta decisão foi tomada por motivos de simplicidade.
O bloco possui a estrutura a seguir:
```
struct Block{
    /// header 13 bytes
    unsigned int verificationMask;  //!< 4 bytes
    unsigned int bytesOccupied;     //!< 4 bytes
    unsigned int nextBlock;         //!< 4 bytes offset used as reference to find the overflow (or next) block
    bool overflow;                  //!< 1 byte has overflow sequence or not
    BYTE data[DATA_SIZE];           //!< array of bytes to keep the records
}
```

Para mais detalhes, tanto das estruturas de dados quanto as dependencias de arquivos, consulte a documentação abaixo.
 
O tamanho dos registros a forma de implementar foi preferida fixa e ainda foi feita organização não espalhada de bloco.
 Foi escolhida a função hash através de experimentos já que era necessario geral algumas colisões.
Nas arvores tanto primaria quanto secundaria foi feito um ponteiro para o nó pai que comumente em implementações em memoria principal não existe entretanto a equipe notou a necessidade em memoria secundaria.


 Na arvore secundaria, inicialmente estava sendo indexando o titulo artigo, entretanto desta maneira estava estourando a pilha de memoria, para nao mexer em tal parametro
 do sistema operacional foi tomada a seguinte decisao: usar uma funcao que dado um titulo , retorna-se um numero inteiro.
 O tamanho dos nodos foi feito para caber em um bloco de 4KBytes para aproveitar ao máximo as idas ao disco o que sempre deve ser evitado.
 
 É importante ressaltar que nos arquivos das árvores de índice o offset do nó raiz foi gravando no fim do arquivo por questões de desempenho .
 
 
 