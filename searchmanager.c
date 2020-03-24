#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include "longest_word_search.h"
#include "queue_ids.h"

pthread_mutex_t lock;

// paramaters to passed start a thread
struct thread_args
{
  pthread_t thread;
  int index;
  char *prefix;
  int msqid;
  int msgflg;
  key_t key;
  prefix_buf sbuf;
  size_t buf_length;
  int delay;
  response_buf rbuf;
  int ret;
  int num_passages;
};

// get the size of the string
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

// get the total number of passages from 'passages.txt'
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

// find the passage number according to the 'passages.txt'
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

// start a new thread and sent type 1 message
void *searchmanager(void *ptr)
{
  pthread_mutex_lock(&lock);
  struct thread_args *arg = (struct thread_args *)ptr;

  // set arguments
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

  // delay the program by the given input
  sleep(delay);

  // prepare type 1 message
  sbuf.mtype = 1;
  strlcpy(sbuf.prefix, prefix, WORD_LENGTH);
  sbuf.id = index;
  buf_length = strlen(sbuf.prefix) + sizeof(int) + 1;

  // send message
  // error
  if ((msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT)) < 0)
  {
    int errnum = errno;
    fprintf(stderr, "%d, %ld, %s, %d\n", msqid, sbuf.mtype, sbuf.prefix, (int)buf_length);
    perror("(msgsnd)");
    fprintf(stderr, "Error sending msg: %s\n", strerror(errnum));
    exit(1);
  }
  // no error
  else
  {
    fprintf(stderr, "\nMessage(%d): \"%s\" Sent (%d bytes)\n", sbuf.id, sbuf.prefix, (int)buf_length);
    response_buf rbufs[num_passages];
    for (int current_passage = 0; current_passage < num_passages; current_passage++)
    {
      do
      {
        ret = msgrcv(msqid, &rbuf, sizeof(response_buf), 2, 0);
        int errnum = errno;
        if (ret < 0 && errno != EINTR)
        {
          // error
          fprintf(stderr, "Value of errno: %d\n", errno);
          perror("Error printed by perror");
          fprintf(stderr, "Error receiving msg: %s\n", strerror(errnum));
        }
        rbufs[findSpot("passages.txt", rbuf.location_description)] = rbuf;
      } while ((ret < 0) && (errno == 4));
    }

    // print report
    printf("\nReport \"%s\"\n", sbuf.prefix);
    for (int i = 0; i < num_passages; i++)
    {
      if (rbufs[i].present == 1)
        printf("Passage %d - %s - %s\n", i, rbufs[i].location_description, rbufs[i].longest_word);
      else
        printf("Passage %d - %s - no word found\n", i, rbufs[i].location_description);
    }
  }
  pthread_mutex_unlock(&lock);
}

// main function
int main(int argc, char **argv)
{

  // command line format checking
  if (argc <= 2)
  {
    printf("Error\n");
    printf("Usage: %s <delay> <prefix1> <prefix2> ...\n", argv[0]);
    return 0;
  }
  for (int i = 0; i < strlen(argv[1]); i++)
  {
    if (!isdigit(argv[1][i]))
    {
      printf("Error\n");
      printf("delay should be a number\n");
      return 0;
    }
  }
  for (int i = 2; i < argc; i++)
  {
    if (strlen(argv[i]) < 3)
    {
      printf("Error\n");
      printf("prefix string has to be at least 3 characters long\n");
      return 0;
    }
  }

  // variable initializations: system v message queue
  int msqid;
  int msgflg = IPC_CREAT | 0666;
  key_t key;
  prefix_buf sbuf;
  size_t buf_length;
  response_buf rbuf;
  int ret;

  // variable initializations: main
  int delay = atoi(argv[1]);
  int num_messages = argc - 2;
  int num_passages = getNumPassages("passages.txt");

  // get key and id
  key = ftok(CRIMSON_ID, QUEUE_NUMBER);
  // when case msgget fails
  if ((msqid = msgget(key, msgflg)) < 0)
  {
    int errnum = errno;
    fprintf(stderr, "Value of errno: %d\n", errno);
    perror("(msgget)");
    fprintf(stderr, "Error msgget: %s\n", strerror(errnum));
  }
  // everything works -> create threads
  else
  {
    pthread_t threads[num_messages];
    struct thread_args args[num_messages];
    for (int i = 0; i < num_messages; i++)
    {
      args[i].thread = threads[i];
      args[i].index = i + 1;
      args[i].prefix = argv[i + 2];
      args[i].msqid = msqid;
      args[i].msgflg = IPC_CREAT | 0666;
      args[i].key = key;
      args[i].sbuf = sbuf;
      args[i].buf_length = buf_length;
      args[i].delay = delay;
      args[i].rbuf = rbuf;
      args[i].ret = ret;
      args[i].num_passages = num_passages;
      pthread_create(&threads[i], NULL, searchmanager, (void *)&args[i]);
    }

    // join threads
    for (int i = 0; i < num_messages; i++)
      pthread_join(threads[i], NULL);
  }

  // prepare to send terminate message
  sbuf.mtype = 1;
  strlcpy(sbuf.prefix, " ", WORD_LENGTH);
  sbuf.id = 0;
  buf_length = strlen(sbuf.prefix) + sizeof(int) + 1; //struct size without long int type

  // send message to quit program
  // msgsnd failed then
  if ((msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT)) < 0)
  {
    int errnum = errno;
    fprintf(stderr, "%d, %ld, %s, %d\n", msqid, sbuf.mtype, sbuf.prefix, (int)buf_length);
    perror("(msgsnd)");
    fprintf(stderr, "Error sending msg: %s\n", strerror(errnum));
    exit(1);
  }
  // message sent successfully
  else
    fprintf(stderr, "\nMessage(%d): \"%s\" Sent (%d bytes)\n", sbuf.id, sbuf.prefix, (int)buf_length);

  printf("\nExiting ...\n");

  exit(0);
}
