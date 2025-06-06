# Sistema de Gerenciamento de Sensores

Este projeto consiste em três programas que trabalham em conjunto para gerenciar dados de sensores.

## Programa 1 - Organização de Dados

O Programa 1 organiza os dados dos sensores em arquivos separados por tipo de sensor.

### Compilação
```bash
gcc Programa1/main.c -o programa1
```

### Uso
```bash
./programa1 <arquivo_de_entrada>
```

### Exemplo
```bash
./programa1 amostras.txt
```

O programa irá:
1. Criar uma pasta chamada "Organizacao"
2. Ler o arquivo de entrada
3. Separar os dados por tipo de sensor (TEMP, PRES, VIBR, ALRM, UMID)
4. Criar um arquivo para cada tipo de sensor na pasta "Organizacao"
5. Ordenar os dados por timestamp em cada arquivo

## Programa 2 - Busca de Dados

O Programa 2 busca o valor mais próximo de um timestamp específico para um sensor.

### Compilação
```bash
gcc Programa2/main.c -o programa2
```

### Uso
```bash
./programa2 arg1 <SENSOR_ID> <dia> <mes> <ano> <hora> <minuto> <segundo>
```

### Exemplo
```bash
./programa2 arg1 TEMP 15 3 2024 14 30 0
```

O programa irá:
1. Buscar o arquivo correspondente ao sensor na pasta "Organizacao"
2. Encontrar o valor mais próximo do timestamp especificado
3. Exibir o resultado encontrado

## Programa 3 - Geração de Dados

O Programa 3 gera dados aleatórios para diferentes tipos de sensores.

### Compilação
```bash
gcc Programa3/main.c -o programa3
```

### Uso
```bash
./programa3 <inicio> <fim> <SENSOR:TIPO> ...
```

### Exemplo
```bash
./programa3 "2024-01-01 00:00:00" "2024-01-02 00:00:00" "TEMP:float" "ALRM:bool" "PRES:int"
```

Tipos de dados suportados:
- `int`: números inteiros (0-999)
- `float`: números decimais com 2 casas
- `double`: números decimais com 5 casas
- `bool`: valores true/false

O programa irá:
1. Gerar dados aleatórios para cada sensor no período especificado
2. Salvar os dados no arquivo "amostras.txt" na pasta "Arquivos_teste"
3. Embaralhar aleatoriamente as linhas do arquivo

## Fluxo de Uso

1. Use o Programa 3 para gerar dados de teste
2. Use o Programa 1 para organizar os dados gerados
3. Use o Programa 2 para consultar valores específicos

## Estrutura de Arquivos

```
.
├── Programa1/
│   ├── main.c
│   ├── sensor_type.h
│   └── sensor_type.c
├── Programa2/
│   └── main.c
├── Programa3/
│   └── main.c
├── Arquivos_teste/
│   └── amostras.txt
└── Organizacao/
    ├── TEMP.txt
    ├── PRES.txt
    ├── VIBR.txt
    ├── ALRM.txt
    └── UMID.txt
``` 