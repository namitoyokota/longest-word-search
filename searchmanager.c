#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include "longest_word_search.h"
#include "queue_ids.h"

pthread_mutex_t lock;

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

  sleep(delay);

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
  {
    fprintf(stderr, "\nMessage(%d): \"%s\" Sent (%d bytes)\n", sbuf.id, sbuf.prefix, (int)buf_length);
    msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT);
    msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT);
    msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT);
    msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT);
    msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT);
    printf("\nReport \"%s\"\n", sbuf.prefix);
    // receive messages
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
      } while ((ret < 0) && (errno == 4));
      rbufs[current_passage] = rbuf;
    }
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

int main(int argc, char **argv)
{
  int msqid;
  int msgflg = IPC_CREAT | 0666;
  key_t key;
  prefix_buf sbuf;
  size_t buf_length;
  int delay;
  response_buf rbuf;
  int ret;
  int num_messages = 0;
  int num_passages = 6;

  if (argc <= 2)
  {
    printf("Error\n");
    printf("Usage: %s <delay> <prefix1> <prefix2> ...\n", argv[0]);
    exit(-1);
  }

  delay = atoi(argv[1]);

  // get key and id
  key = ftok(CRIMSON_ID, QUEUE_NUMBER);
  if ((msqid = msgget(key, msgflg)) < 0)
  {
    int errnum = errno;
    fprintf(stderr, "Value of errno: %d\n", errno);
    perror("(msgget)");
    fprintf(stderr, "Error msgget: %s\n", strerror(errnum));
  }
  else
  {
    // loop through all prefixes
    for (int i = 2; argv[i] != NULL; i++)
    {
      num_messages++;
    }

    // create threads
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
    {
      pthread_join(threads[i], NULL);
    }
  }

  pthread_exit(NULL);
  printf("\nExiting ...\n");

  exit(0);
}
