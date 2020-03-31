#include "systemv.h"

void *send(void *ptr)
{
  struct thread_args *arg = (struct thread_args *)ptr;
  prefix_buf sbuf = arg->sbuf;
  size_t buf_length = arg->buf_length;

  // prepare type 1 message
  sbuf.mtype = 1;
  sbuf.id = arg->index;
  strlcpy(sbuf.prefix, arg->prefix, WORD_LENGTH);
  buf_length = strlen(sbuf.prefix) + sizeof(int) + 1;

  // send message
  if ((msgsnd(arg->msqid, &sbuf, buf_length, IPC_NOWAIT)) < 0)
  {
    int errnum = errno;
    fprintf(stderr, "%d, %ld, %s, %d\n", arg->msqid, sbuf.mtype, sbuf.prefix, (int)buf_length);
    perror("(msgsnd)");
    fprintf(stderr, "Error sending msg: %s\n", strerror(errnum));
    exit(1);
  }
  else
    fprintf(stderr, "\nMessage(%d): \"%s\" Sent (%d bytes)\n", sbuf.id, sbuf.prefix, (int)buf_length);
}

void *receive(void *ptr)
{
  struct thread_args *arg = (struct thread_args *)ptr;
  response_buf rbuf = arg->rbuf;
  response_buf rbufs[arg->num_passages];
  int ret;

  pthread_mutex_lock(&lock);

  for (int current_passage = 0; current_passage < arg->num_passages; current_passage++)
  {
    do
    {
      ret = msgrcv(arg->msqid, &rbuf, sizeof(response_buf), 2, 0);
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
    snprintf(buf, 12, "%d of %d", current_passage + 1, arg->num_passages); // puts string into buffer
    strcpy(status[arg->index - 1], buf);
  }

  pthread_mutex_unlock(&lock);

  // print report
  printf("\nReport \"%s\"\n", arg->prefix);
  for (int i = 0; i < arg->num_passages; i++)
  {
    if (rbufs[i].present == 1)
      printf("Passage %d - %s - %s\n", i, rbufs[i].location_description, rbufs[i].longest_word);
    else
      printf("Passage %d - %s - no word found\n", i, rbufs[i].location_description);
  }

  strcpy(status[arg->index - 1], "done");
}