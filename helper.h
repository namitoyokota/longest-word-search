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
size_t strlcpy(char *dst, const char *src, size_t size);

// get the total number of passages from 'passages.txt'
int getNumPassages(char *filename);

// find the passage number according to the 'passages.txt'
int findSpot(char *passages, char *text);
void *send(void *ptr);
void *receive(void *ptr);