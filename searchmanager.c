#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "longest_word_search.h"
#include "queue_ids.h"

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

int main(int argc, char **argv)
{
  int msqid;
  int msgflg = IPC_CREAT | 0666;
  key_t key;
  prefix_buf sbuf;
  size_t buf_length;
  int wait;
  response_buf rbuf;
  int ret;
  int num_messages = 0;
  int num_passages = 5;

  if (argc <= 2)
  {
    printf("Error: provide the delay then the prefixes\n");
    printf("Usage: %s <delay> <prefix1> <prefix2> ...\n", argv[0]);
    exit(-1);
  }

  wait = atoi(argv[1]);

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
    fprintf(stderr, "msgget: msgget succeeded: msgqid = %d\n", msqid);

  // loop through all prefixes
  for (int i = 2; argv[i] != NULL; i++)
  {
    num_messages++;
    sbuf.mtype = 1;
    strlcpy(sbuf.prefix, argv[i], WORD_LENGTH);
    sbuf.id = i - 1;
    buf_length = strlen(sbuf.prefix) + sizeof(int) + 1;

    // send a message
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

        if (rbufs[i].present == 2)
          printf("Passage %d - %s - %s\n", i, rbufs[i].location_description, rbufs[i].longest_word);
        else
          printf("Passage %d - %s - no word found\n", i, rbufs[i].location_description);
      }
    }
  }

  exit(0);
}
