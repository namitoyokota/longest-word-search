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
  int delay;
  response_buf rbuf;
  int ret;
  int num_passages = 4;

  if (argc <= 2)
  {
    printf("Error: provide the delay then the prefixes\n");
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
    fprintf(stderr, "msgget: msgget succeeded: msgqid = %d\n", msqid);

  // loop through all prefixes
  for (int i = 2; argv[i] != NULL; i++)
  {
    // send a message: type 1
    sbuf.mtype = 1;
    strlcpy(sbuf.prefix, argv[i], WORD_LENGTH);
    sbuf.id = i - 1;
    buf_length = strlen(sbuf.prefix) + sizeof(int) + 1; //struct size without long int type

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
      fprintf(stderr, "Message(%d): \"%s\" Sent (%d bytes)\n", sbuf.id, sbuf.prefix, (int)buf_length);

    // until all messages are received
    for (int current_passage = 1; current_passage < num_passages; current_passage++)
    {
      // receive a message: type 2
      do
      {
        // msgrcv: https://pubs.opengroup.org/onlinepubs/007908799/xsh/msgrcv.html
        ret = msgrcv(msqid, &rbuf, sizeof(response_buf), 2, 0); //receive type 2 message
        int errnum = errno;
        if (ret < 0 && errno != EINTR)
        {
          fprintf(stderr, "Value of errno: %d\n", errno);
          perror("Error printed by perror");
          fprintf(stderr, "Error receiving msg: %s\n", strerror(errnum));
        }
      } while ((ret < 0) && (errno == 4));
      //fprintf(stderr,"msgrcv error return code --%d:$d--",ret,errno);

      if (rbuf.present == 1)
        fprintf(stderr, "%ld, %d of %d, %s, size=%d\n", rbuf.mtype, rbuf.index, rbuf.count, rbuf.longest_word, ret);
      else
        fprintf(stderr, "%ld, %d of %d, not found, size=%d\n", rbuf.mtype, rbuf.index, rbuf.count, ret);
    }
  }

  exit(0);
}
