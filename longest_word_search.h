#ifndef LWS_H
#define LWS_H

#define WORD_LENGTH 100 + 1
#define PASSAGE_NAME_LENGTH 30 + 1

// structure of the prefix: like 'con'
typedef struct prefixbuf
{
	// set to 1 and corresponds to type in parm 4-msgrcv(msqid, &rbuf, WORD_LENGTH, 1, 0)
	// in system5msg- Java_edu_cs300_MessageJNI_readPrefixRequestMsg
	long mtype;								// not in byte count
	int id;										// 4 bytes
	char prefix[WORD_LENGTH]; // 4 bytes in the case of a three letter prefix and a null
} prefix_buf;

// structure of the found word: like 'contemporary'
typedef struct foundbuf
{
	// set to 2 and corresponds to type in parm 4-msgrcv(msqid, &rbuf, WORD_LENGTH, 2, 0)
	// ni msgrcv_lwr.c
	long mtype;																			// not in byte count
	int index;																			// index of response - 4 bytes
	int count;																			// total excerpts available - 4 bytes
	int present;																		// 0 if not found; 1 if found - 4 bytes
	char location_description[PASSAGE_NAME_LENGTH]; // 32 bytes
	char longest_word[WORD_LENGTH];									// 191 bytes
} response_buf;

#endif