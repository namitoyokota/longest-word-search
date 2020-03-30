#include "helper.h"

int max_prefixes = 0;
char **prefixes;
char **status;

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

void *send(void *ptr)
{
  pthread_mutex_lock(&lock);

  struct thread_args *arg = (struct thread_args *)ptr;
  pthread_t thread = arg->thread;
  int index = arg->index;
  char *prefix = arg->prefix;
  int msqid = arg->msqid;
  int msgflg = arg->msgflg;
  key_t key = arg->key;
  prefix_buf sbuf = arg->sbuf;
  size_t buf_length = arg->buf_length;
  int delay = arg->delay;
  response_buf rbuf = arg->rbuf;
  int ret = arg->ret;
  int num_passages = arg->num_passages;

  sleep(delay);

  // prepare type 1 message
  sbuf.mtype = 1;
  strlcpy(sbuf.prefix, prefix, WORD_LENGTH);
  sbuf.id = index;
  buf_length = strlen(sbuf.prefix) + sizeof(int) + 1;

  // send message
  if ((msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT)) < 0)
  {
    int errnum = errno;
    fprintf(stderr, "%d, %ld, %s, %d\n", msqid, sbuf.mtype, sbuf.prefix, (int)buf_length);
    perror("(msgsnd)");
    fprintf(stderr, "Error sending msg: %s\n", strerror(errnum));
    exit(1);
  }
  else
    fprintf(stderr, "\nMessage(%d): \"%s\" Sent (%d bytes)\n", sbuf.id, sbuf.prefix, (int)buf_length);

  pthread_mutex_unlock(&lock);
}

void *receive(void *ptr)
{
  pthread_mutex_lock(&lock);

  struct thread_args *arg = (struct thread_args *)ptr;
  pthread_t thread = arg->thread;
  int index = arg->index;
  char *prefix = arg->prefix;
  int msqid = arg->msqid;
  int msgflg = arg->msgflg;
  key_t key = arg->key;
  prefix_buf sbuf = arg->sbuf;
  size_t buf_length = arg->buf_length;
  int delay = arg->delay;
  response_buf rbuf = arg->rbuf;
  int ret = arg->ret;
  int num_passages = arg->num_passages;
  response_buf rbufs[num_passages];

  for (int current_passage = 0; current_passage < num_passages; current_passage++)
  {
    do
    {
      ret = msgrcv(msqid, &rbuf, sizeof(response_buf), 2, 0);
      int errnum = errno;
      if (ret < 0 && errno != EINTR)
      {
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr, "Error receiving msg: %s\n", strerror(errnum));
      }
      rbufs[findSpot("passages.txt", rbuf.location_description)] = rbuf;
    } while ((ret < 0) && (errno == 4));
    char buf[6];
    snprintf(buf, 12, "%d of %d", current_passage + 1, num_passages); // puts string into buffer
    strcpy(status[index - 1], buf);
  }

  // print report
  printf("\nReport \"%s\"\n", prefix);
  for (int i = 0; i < num_passages; i++)
  {
    if (rbufs[i].present == 1)
      printf("Passage %d - %s - %s\n", i, rbufs[i].location_description, rbufs[i].longest_word);
    else
      printf("Passage %d - %s - no word found\n", i, rbufs[i].location_description);
  }

  strcpy(status[index - 1], "done");

  pthread_mutex_unlock(&lock);
}