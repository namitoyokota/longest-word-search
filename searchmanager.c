#include "helper.h"
#include "systemv.h"

// main function
int main(int argc, char **argv)
{
  // handle CTRL-C to print status
  signal(SIGINT, sigintHandler);

  // command line format checking
  if (checkConditions(argc, argv))
    return 0;
  if (max_prefixes == 0)
    return 0;

  allocateStatus(argc, argv);

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
  int num_messages = max_prefixes;
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
    pthread_t send_threads[num_messages];
    pthread_t receive_threads[num_messages];
    struct thread_args args[num_messages];

    for (int i = 0; i < num_messages; i++)
    {
      args[i].index = i + 1;
      args[i].prefix = argv[i + 2];
      args[i].msqid = msqid;
      args[i].sbuf = sbuf;
      args[i].buf_length = buf_length;
      args[i].rbuf = rbuf;
      args[i].num_passages = num_passages;
    }

    // create threads for each prefix send and receive
    for (int i = 0; i < num_messages; i++)
    {
      pthread_create(&send_threads[i], NULL, send, (void *)&args[i]);
      pthread_create(&receive_threads[i], NULL, receive, (void *)&args[i]);
      sleep(delay);
    }

    // join threads
    for (int i = 0; i < num_messages; i++)
    {
      pthread_join(send_threads[i], NULL);
      pthread_join(receive_threads[i], NULL);
    }
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
