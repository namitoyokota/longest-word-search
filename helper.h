#ifndef __HELPER_INCLUDED__
#define __HELPER_INCLUDED__

#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include "longest_word_search.h"
#include "queue_ids.h"

pthread_mutex_t lock;
extern int max_prefixes;
extern char **prefixes;
extern char **status;

// paramaters to passed start a thread
struct thread_args
{
  int index;
  char *prefix;
  int msqid;
  prefix_buf sbuf;
  size_t buf_length;
  response_buf rbuf;
  int num_passages;
};

bool checkConditions(int argc, char **agrv);             // check for any invalid command line inputs
void allocateStatus(int argc, char **argv);              // dynamically allocate space for status strings
void sigintHandler(int sig_num);                         // signal handler to print status
size_t strlcpy(char *dst, const char *src, size_t size); // get the size of the string
int getNumPassages(char *filename);                      // get the total number of passages from 'passages.txt'
int findSpot(char *passages, char *text);                // find the passage number according to the 'passages.txt'
void *send(void *ptr);                                   // send type 1 message
void *receive(void *ptr);                                // send type 2 message

#endif