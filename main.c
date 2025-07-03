#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>


#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <unistd.h>
#include <sys/stat.h>
#define MKDIR(path) mkdir(path, 0777)
#endif

#define EXTENSAO_TXT ".txt"
#define TAMANHO_PADRAO_LINHA 1000
#define TAMANHO_MAX_NOME_SEM_EXTENSAO 256
#define TAMANHO_CARACTERE_TERMINADOR_STRING 1

int compara_string(const void *, const void *);
bool argumentos_sao_validos(int, char **);
char *obter_caminho_final_arquivo_ordenado(const char *, const char *);
void criar_pasta(const char *);
void liberar_matriz(char **, size_t);
long string_para_numero(char *);

int main(int argc, char **argv)
{
    if (!argumentos_sao_validos(argc, argv))
    {
        return EXIT_FAILURE;
    }

    char *caminho_arquivo_ordenar = argv[1];
    long tamanho_maximo_linha = (argc == 3 ? string_para_numero(argv[2]) : TAMANHO_PADRAO_LINHA) + TAMANHO_CARACTERE_TERMINADOR_STRING;

    FILE *arquivo_ordenar = fopen(caminho_arquivo_ordenar, "r");
    if (arquivo_ordenar == NULL)
    {
        perror("Erro ao abrir arquivo");
        return EXIT_FAILURE;
    }

    char **matriz_texto = NULL;

    size_t linha_atual = 0, capacidade_linhas = 0;

    char buffer[tamanho_maximo_linha];

    while (fgets(buffer, sizeof(buffer), arquivo_ordenar))
    {
        size_t tamanho_buffer = strlen(buffer);
        bool possui_linha_completa = (tamanho_buffer > 0 && buffer[tamanho_buffer - 1] == '\n');

        if (possui_linha_completa) {
            buffer[tamanho_buffer - 1] = '\0';
        }

        if (linha_atual >= capacidade_linhas)
        {
            char **matriz_texto_realocada = realloc(matriz_texto, (capacidade_linhas + 1) * sizeof(char *));
            if (matriz_texto_realocada == NULL)
            {
                perror("realloc");
                liberar_matriz(matriz_texto, capacidade_linhas);
                return EXIT_FAILURE;
            }
            
            matriz_texto = matriz_texto_realocada;
            capacidade_linhas++;
        }

        matriz_texto[linha_atual] = strdup(buffer);
        if (matriz_texto[linha_atual] == NULL)
        {
            perror("strdup");
            liberar_matriz(matriz_texto, capacidade_linhas);
            return EXIT_FAILURE;
        }

        linha_atual++;

        if (!possui_linha_completa)
        {
            int caractere_descarte;
            while ((caractere_descarte = fgetc(arquivo_ordenar)) != '\n' && caractere_descarte != EOF);
        }
    }

    fclose(arquivo_ordenar);

    qsort(matriz_texto, capacidade_linhas, sizeof(char *), compara_string);

    const char *nome_pasta_arquivos_ordenados = "arquivos_ordenados";
    criar_pasta(nome_pasta_arquivos_ordenados);

    char *caminho_arquivo_ordenado = obter_caminho_final_arquivo_ordenado(caminho_arquivo_ordenar, nome_pasta_arquivos_ordenados);
    if (caminho_arquivo_ordenado == NULL)
    {
        liberar_matriz(matriz_texto, capacidade_linhas);
        return EXIT_FAILURE;
    }

    FILE *arquivo_ordenado = fopen(caminho_arquivo_ordenado, "w+");
    if (arquivo_ordenado == NULL)
    {
        perror("Erro ao criar arquivo ordenado");
        free(caminho_arquivo_ordenado);
        liberar_matriz(matriz_texto, capacidade_linhas);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < capacidade_linhas; i++)
    {
        fprintf(arquivo_ordenado, "%s\n", matriz_texto[i]);
    }

    if (fseek(arquivo_ordenado, 0, SEEK_END) != 0)
    {
        perror("Erro ao posicionar o ponteiro do arquivo ordenado");
    }

    fclose(arquivo_ordenado);
    liberar_matriz(matriz_texto, capacidade_linhas);

    return EXIT_SUCCESS;
}

int compara_string(const void *a, const void *b)
{
    return strcasecmp(*(char **)a, *(char **)b);
}

bool argumentos_sao_validos(int argc, char **argv)
{
    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "Numero de argumentos invalido\n");
        return false;
    }

    char *caminho_arquivo_ordenar = argv[1];

    size_t tamanho_caminho_arquivo_ordenar = strlen(caminho_arquivo_ordenar);
    size_t tamanho_extensao_txt = strlen(EXTENSAO_TXT);

    if (tamanho_caminho_arquivo_ordenar <= tamanho_extensao_txt 
        || strcmp(caminho_arquivo_ordenar + tamanho_caminho_arquivo_ordenar - tamanho_extensao_txt, EXTENSAO_TXT) != 0)
    {
        fprintf(stderr, "O arquivo deve terminar com a extensao .txt\n");
        return false;
    }

    if (argc == 3 && string_para_numero(argv[2]) == -1)
    {
        fprintf(stderr, "O segundo argumento, se passado, deve ser composto por numeros positivos\n");
        return false;
    }

    return true;
}

char *obter_caminho_final_arquivo_ordenado(const char *caminho_arquivo_ordenado, const char *nome_pasta)
{
    const char *nome_arquivo = strrchr(caminho_arquivo_ordenado, '/');
    nome_arquivo = (nome_arquivo == NULL) ? caminho_arquivo_ordenado : nome_arquivo + 1;

    char nome_sem_extensao[TAMANHO_MAX_NOME_SEM_EXTENSAO];
    strncpy(nome_sem_extensao, nome_arquivo, sizeof(nome_sem_extensao) - 1);
    nome_sem_extensao[sizeof(nome_sem_extensao) - 1] = '\0';

    char *extensao = strrchr(nome_sem_extensao, '.');
    if (extensao && strcmp(extensao, EXTENSAO_TXT) == 0)
    {
        *extensao = '\0';
    }

    const char *prefixo_arquivo_ordenado = "arquivo_";
    const char *sufixo_arquivo_ordenado = "_ordenado";

    size_t comprimento_esperado = strlen(nome_pasta)
        + 1  // '/'
        + strlen(prefixo_arquivo_ordenado)
        + strlen(nome_sem_extensao)
        + strlen(sufixo_arquivo_ordenado)
        + strlen(EXTENSAO_TXT)
        + 1; // '\0'

    char *caminho_final = malloc(comprimento_esperado);
    if (caminho_final == NULL)
    {
        perror("Erro ao alocar memoria para o caminho final do arquivo ordenado");
        return NULL;
    }

    snprintf(caminho_final, comprimento_esperado,
            "%s/%s%s%s%s", nome_pasta, prefixo_arquivo_ordenado, nome_sem_extensao, sufixo_arquivo_ordenado, EXTENSAO_TXT);

    return caminho_final;
}

void criar_pasta(const char *nome_pasta)
{
    if (MKDIR(nome_pasta) != 0 && errno != EEXIST)
    {
        perror("Erro ao criar pasta de arquivos ordenados");
    }
}

void liberar_matriz(char **matriz, size_t linhas)
{
    for (size_t i = 0; i < linhas; i++)
    {
        free(matriz[i]);
    }

    free(matriz);
}

long string_para_numero(char *valor)
{
    char *ponteiro_final;
    int base_decimal = 10;
    long numero = strtol(valor, &ponteiro_final, base_decimal);

    if (*ponteiro_final != '\0' || numero <= 0) {
        return -1;
    }

    return numero;
}