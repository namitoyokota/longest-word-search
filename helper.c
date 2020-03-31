#include "helper.h"

int max_prefixes = 0;
char **prefixes;
char **status;

bool checkConditions(int argc, char **argv)
{
  if (argc <= 2)
  {
    fprintf(stderr, "Usage: %s <delay> <prefix1> <prefix2> ...\n", argv[0]);
    return 1;
  }
  for (int i = 0; i < strlen(argv[1]); i++)
  {
    if (!isdigit(argv[1][i]))
    {
      fprintf(stderr, "Error: Enter delay as an integer\n");
      return 1;
    }
  }
  for (int i = 2; i < argc; i++)
  {
    if (strlen(argv[i]) < 3)
    {
      fprintf(stderr, "Error: Prefix '%s' is less than 3 characters long\n", argv[i]);
      for (int j = i; j < argc - 1; j++)
        argv[j] = argv[j + 1];
    }
    else
      max_prefixes++;
  }
  return 0;
}

void allocateStatus(int argc, char **argv)
{
  prefixes = malloc(max_prefixes * sizeof(char *));
  status = malloc(max_prefixes * sizeof(char *));
  for (int i = 2; i < argc; i++)
  {
    prefixes[i - 2] = malloc(sizeof(argv[i]) * sizeof(char));
    status[i - 2] = malloc(6 * sizeof(char));
    strcpy(prefixes[i - 2], argv[i]);
    strcpy(status[i - 2], "pending");
  }
}

void sigintHandler(int sig_num)
{
  signal(SIGINT, sigintHandler);
  printf("\n");
  for (int i = 0; i < max_prefixes; i++)
  {
    printf("\n%s - %s", prefixes[i], status[i]);
  }
  printf("\n\n");
  fflush(stdout);
}

size_t strlcpy(char *dst, const char *src, size_t size)
{
  size_t srclen;
  size--;
  srclen = strlen(src);
  if (srclen > size)
    srclen = size;
  memcpy(dst, src, srclen);
  dst[srclen] = '\0';
  return (srclen);
}

int getNumPassages(char *filename)
{
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(filename, "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  int count = 0;
  while ((read = getline(&line, &len, fp)) != -1)
  {
    line[strcspn(line, "\n")] = 0;
    FILE *f;
    f = fopen(line, "r");
    if (f != NULL)
      count++;
  }
  fclose(fp);
  return count;
}

int findSpot(char *passages, char *text)
{
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(passages, "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  int count = 0;
  while ((read = getline(&line, &len, fp)) != -1)
  {
    if (line[strlen(line) - 1] == '\n')
      line[strlen(line) - 1] = '\0';
    if (strcmp(line, text) == 0)
      return count;
    count++;
  }
  fclose(fp);
  return count;
}
