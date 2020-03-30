#ifndef __HELPER_INCLUDED__
#define __HELPER_INCLUDED__

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
#include <signal.h>
#include "longest_word_search.h"
#include "queue_ids.h"

pthread_mutex_t lock;
extern int max_prefixes;
extern char **prefixes;
extern char **status;

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

void sigintHandler(int sig_num);                         // signal handler to print status
size_t strlcpy(char *dst, const char *src, size_t size); // get the size of the string
int getNumPassages(char *filename);                      // get the total number of passages from 'passages.txt'
int findSpot(char *passages, char *text);                // find the passage number according to the 'passages.txt'
void *send(void *ptr);                                   // send type 1 message
void *receive(void *ptr);                                // send type 2 message

#endif