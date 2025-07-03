# Organizador de Arquivos TXT

Um programa em C que lê um arquivo de texto (.txt), ordena suas linhas alfabeticamente e salva o resultado em um novo arquivo na pasta `arquivos_ordenados`.

## Funcionalidades

- Ordena linhas de arquivos de texto em ordem alfabética (case-insensitive)
- Suporte para linhas de tamanho customizável
- Cria automaticamente pasta de destino para arquivos ordenados
- Gerenciamento seguro de memória
- Compatível com Windows e Linux
- Tratamento robusto de erros

## Compilação

### Windows (com MinGW ou Visual Studio)
```bash
gcc -o organizar main.c
```

### Linux/macOS
```bash
gcc -o organizar main.c
```

## Como Usar

### Sintaxe
```bash
./organizar <arquivo.txt> [tamanho_maximo_linha]
```

### Parâmetros

- `arquivo.txt` **(obrigatório)**: Caminho para o arquivo de texto que será ordenado
- `tamanho_maximo_linha` **(opcional)**: Tamanho máximo de cada linha em caracteres (padrão: 1000)

### Exemplos de Uso

#### Exemplo básico:
```bash
./organizar lista_nomes.txt
```

#### Com tamanho personalizado de linha:
```bash
./organizar arquivo_grande.txt 2000
```

#### No Windows:
```cmd
organizar.exe dados.txt
```

## Estrutura de Saída

O programa cria automaticamente:
```
pasta_atual/
├── arquivo_original.txt
└── arquivos_ordenados/
    └── arquivo_arquivo_original_ordenado.txt
```

### Padrão de nomenclatura:
- **Entrada**: `meu_arquivo.txt`
- **Saída**: `arquivos_ordenados/arquivo_meu_arquivo_ordenado.txt`

## Exemplo Prático

### Arquivo de entrada (`frutas.txt`):
```
maçã
banana
abacaxi
uva
```

### Comando:
```bash
./organizar frutas.txt
```

### Arquivo de saída (`arquivos_ordenados/arquivo_frutas_ordenado.txt`):
```
abacaxi
banana
maçã
uva

```

## Recursos Técnicos

### Características do programa:
- **Algoritmo de ordenação**: Quicksort (via `qsort()`)
- **Gerenciamento de memória**: Alocação dinâmica com `malloc()` e `realloc()`
- **Tratamento de linhas longas**: Descarta automaticamente caracteres excedentes ao receber tamanho máximo de linhas como argumento
- **Comparação**: Case-insensitive via `strcasecmp()`

### Limites padrão:
- **Tamanho máximo de linha**: 1000 caracteres (configurável)
- **Tamanho máximo de nome de arquivo**: 256 caracteres
- **Número de linhas**: Limitado apenas pela memória disponível

## Tratamento de Erros

O programa valida:
- Número correto de argumentos
- Extensão `.txt` no arquivo de entrada
- Existência do arquivo de entrada
- Valores numéricos válidos para tamanho de linha
- Disponibilidade de memória
- Permissões de escrita na pasta de destino

### Códigos de saída:
- `0` (EXIT_SUCCESS): Execução bem-sucedida
- `1` (EXIT_FAILURE): Erro durante a execução

## Dependências

### Bibliotecas padrão utilizadas:
```c
#include <stdio.h>      // Operações de arquivo
#include <string.h>     // Manipulação de strings
#include <stdlib.h>     // Alocação de memória
#include <stdbool.h>    // Tipo booleano
#include <errno.h>      // Códigos de erro
#include <sys/types.h>  // Tipos de sistema
```

### Compatibilidade de sistema:
- **Windows**: Usa `_mkdir()` e `<direct.h>`
- **Linux/Unix**: Usa `mkdir()` e `<sys/stat.h>`

## Limitações

- Arquivos devem ter extensão `.txt`
- Linhas muito longas são truncadas no tamanho especificado
- Requer permissões de escrita no diretório atual
- Consome memória proporcional ao tamanho do arquivo

## Solução de Problemas

### Erro: "Numero de argumentos invalido"
```bash
# ❌ Incorreto
./organizar

# ✅ Correto
./organizar arquivo.txt
```

### Erro: "O arquivo deve terminar com a extensao .txt"
```bash
# ❌ Incorreto
./organizar arquivo.doc

# ✅ Correto
./organizar arquivo.txt
```

### Erro: "Erro ao abrir arquivo"
- Verifique se o arquivo existe
- Confirme as permissões de leitura
- Verifique o caminho do arquivo

### Erro: "O segundo argumento deve ser composto por numeros positivos"
```bash
# ❌ Incorreto
./organizar arquivo.txt abc
./organizar arquivo.txt -100

# ✅ Correto
./organizar arquivo.txt 1500
```

## Estrutura do Código

### Funções principais:
- `main()`: Função principal que coordena todo o processo
- `argumentos_sao_validos()`: Valida os argumentos de entrada
- `obter_caminho_final_arquivo_ordenado()`: Gera o caminho do arquivo de saída
- `criar_pasta()`: Cria a pasta de destino se não existir
- `compara_string()`: Função de comparação para o `qsort()`
- `liberar_matriz()`: Libera a memória alocada para a matriz de texto do arquivo
- `string_para_numero()`: Converte string para número com validação

---

**Desenvolvido em C** | **Compatível com Windows e Linux** | **Gerenciamento seguro de memória**
