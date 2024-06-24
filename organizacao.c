#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 150

int compara_string(const void *a, const void *b);

int main()
{
  int numOfElements = 0;
  int phrase_idx = 0, char_idx = 0;
  char aux;

  char **vet = (char **)malloc(sizeof(char *));
  if (vet == NULL)
  {
    printf("Nao ha memoria suficiente para declarar o vet!");
    return 1;
  }

  FILE *arq = fopen("Evolucoes.txt", "r");
  if (arq == NULL)
  {
    printf("Erro ao abrir arquivo");
    return 1;
  }

  while (fscanf(arq, "%c", &aux) != EOF)
  {
    if (phrase_idx == numOfElements)
    {
      vet = (char **)realloc(vet, (numOfElements + 1) * MAX * sizeof(char));
      vet[phrase_idx] = (char *)malloc(MAX * sizeof(char));

      if (vet == NULL)
      {
        printf("Nao ha memoria suficiente para adicionar mais elementos ao vet!");
        break;
      }
      else
      {
        numOfElements++;
      }
    }

    if (aux == '\n')
    {
      vet[phrase_idx][char_idx] = '\0';
      phrase_idx++;
      char_idx = 0;
      continue;
    }

    vet[phrase_idx][char_idx] = aux;
    char_idx++;
  }

  fclose(arq);

  char copyVet[numOfElements][MAX];

  for (int i = 0; i < numOfElements; i++)
  {
    strcpy(copyVet[i], vet[i]);
  }

  qsort(copyVet[0], numOfElements, sizeof(copyVet[0]), compara_string);

  FILE *output = fopen("Evolucoes-ordenadas.txt", "a");

  for (int i = 0; i < numOfElements; i++)
  {
    fprintf(output, "%s\n", copyVet[i]);
  }

  fclose(output);

  free(vet);

  return 0;
}

int compara_string(const void *a, const void *b)
{
  return stricmp((char *)a, (char *)b); //  Usar stricmp
}
