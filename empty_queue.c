#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "longest_word_search.h"
#include "queue_ids.h"

int main(int argc, char **argv)
{
  int msqid;
  int msgflg = IPC_CREAT | 0666;
  key_t key;

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

  msgctl(msqid, IPC_RMID, NULL);

  exit(0);
}