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
    // length of source string
    size_t srclen;

    // what goes here: figure out how much room is needed

    size--;

    srclen = strlen(src);

    // what goes here: copy the appropriate amount

    if (srclen > size)
        srclen = size;

    memcpy(dst, src, srclen);
    dst[srclen] = '\0';

    return (srclen);
}

// system v: https://www.softprayog.in/programming/interprocess-communication-using-system-v-message-queues-in-linux
int main(int argc, char **argv)
{
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    prefix_buf sbuf;
    size_t buf_length;

    if (argc <= 1 || strlen(argv[1]) < 2)
    {
        printf("Error: please provide prefix of at least two characters for search\n");
        printf("Usage: %s <prefix>\n", argv[0]);
        exit(-1);
    }

    // ftok: https://pubs.opengroup.org/onlinepubs/009695399/functions/ftok.html
    key = ftok(CRIMSON_ID, QUEUE_NUMBER);
    // msgget: https://pubs.opengroup.org/onlinepubs/009695399/functions/msgget.html
    if ((msqid = msgget(key, msgflg)) < 0)
    {
        int errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("(msgget)");
        fprintf(stderr, "Error msgget: %s\n", strerror(errnum));
    }
    else
        fprintf(stderr, "msgget: msgget succeeded: msgqid = %d\n", msqid);

    // testing :)
    printf("key: %d\nmsqid: %d\n", key, msqid);

    // We'll send message type 1
    sbuf.mtype = 1;
    strlcpy(sbuf.prefix, argv[1], WORD_LENGTH);
    sbuf.id = 0;
    buf_length = strlen(sbuf.prefix) + sizeof(int) + 1; //struct size without long int type

    // Send a message.
    // msgsnd: https://pubs.opengroup.org/onlinepubs/009695399/functions/msgsnd.html
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

    exit(0);
}
