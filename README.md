# Trabalho 2 - range query

O trabalho consiste em adaptar a AVL para que ela suporte range query (busca por intervalos). 

Considerando a base dados do trabalho anterior do IBGE de cidades, [municipios.json](https://github.com/kelvins/municipios-brasileiros/blob/main/json/municipios.json), com esta estrutura adaptada você conseguirá fazer buscas por intervalos. Por exemplo retornar todas as cidades que possuem DDD entre 15 e 20. Retornar todas as cidades que possuem latitude entre -31 e -34 e longitude entre - 50 e -54, ou mesmo fazer combinações entre latitude, longitude e DDD. 

## Tarefa 1: (adaptação da AVL)

**Descrição:**
1. Adapte o código de AVL dado em sala para que os nodes possuam ponteiro para o pai.
2. Cada nó da árvore deve ter uma lista encadeada que armazena os registros com chaves iguais (um nó pode ter dois ou mais registros).
3. Implemente a função de sucessor de um nó que consiga olhar para os ancestrais (veja o código de sucessor no livro do Cormem), esta funcionalidade irá permitir fazer o range query. 

## Tarefa 2 (construção das avls)
**Descrição:** Construa uma AVL para cada um dos seguintes campos como chave da busca: "nome", "latitude","longitude","codigo_uf" e "ddd". Deste modo você terá que instanciar cinco AVLs,  uma para cada campo. O registro armazenado deve conter a chave de busca + o código_ibge.

> Observe que na AVL  adaptada as chaves replicadas são armazenadas um mesmo nó. Por exemplo todas as cidades com DDD 67 serão amazenadas em um mesmo nó.

## Tarefa 3 (combinação de  range queries).
**Descrição:** Considere as seguintes três queries: **(1)** cidades com latitude > 50,  **(2)** 20 <longitude < 30 e **(3)** DDD == 67. Cada query irá retornar um conjunto de códigos ibge. A combinação consiste em fazer a intersecção desses conjuntos. 

## Tarefa 4 (interface)

**Descrição:** Faça uma interface para que seja possível fazer range queries e combinação de range queries com qualquer um dos cinco campos definidos na tarefa 2. A busca deve retornar todos do campos das cidades fornecidas na base de dados. Para a busca das cidades pelo código IBGE você deve utilizar uma tabela hash.  

## Entrega:
1. Link para o código no github
2. Link para o youtube com um vídeo de 12 minutos com a explicação do código e execução da tarefa 4.  

## Trabalho individual

- **Plágio entre alunos:** Será considerado zero tanto para quem passou quanto para quem copiou. Não repasse seu código para o seu colega em hipótese alguma. No entanto fiquem livres para discutir e explicar entre vocês ideias e possibilidades. 

- **Plágio da internet ou chatgpt:** Não serão permitas cópias destas fontes, no entanto vocês são encorajados a utilizar o chatgpt e a internet como material de consulta. Minha sugestão, principalmente para o caso do chatgpt, é que você olhe e entenda as ideias principais do código, feche o código fornecido pela plataforma, e implemente de sua cabeça a funcionalidade desejada.
