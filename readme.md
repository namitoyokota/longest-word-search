<!-- @format -->

# System V Message Queues Longest Word Finder

## Objective

a multithreaded two process system that communicates via System V message queues designed to find the longest word that begins with the supplied prefix in a series of text passages

## Search Manager Logic

Search manager reads one or more prefixes from the command line, creates and sends prefix request messages (contains prefix string and prefix ID) via System V ipc queues and waits for the passage processor to return a series of responses. The search manager will print the results for each prefix as once all responses for that prefix have been received.

## Passage Processor Logic

The passage processor will read a series of passage file names from passages.txt. A thread will be created for each passage that builds Trie with words in the passage text, receives requests for longest word searches, searches the trie for the longest word and asynchronously returns the longest word.

## Instruction

1. Compile Java files: `make java`
2. Compile C files: `make c`
3. Execute with format: `./searchmanager <secs between sending prefix requests> <prefix1> <prefix2> ...`
4. Clean output files: `make clean`

## System V Commands

- `ipcs -a` : displays the message queue
- `ipcrm -Q [key]` : remove message queue

## File Structure

```
project/
└─── passage.txt
└─── Pride_And_Prejustice.txt
└─── Mansfield_Park.txt
└─── The_Call_Of_The_Wild.txt
└─── Tale_Of_Two_Cities.txt
└─── Peter_Pan.txt
└─── edu_cs300_MessageJNI.h
└─── queue_ids.h
└─── longest_word_search.h
└─── searchmaneger.c
└─── system5msg.c
└─── <additional supporting c files>
└─── <additional supporting c header files>
└─── edu/
│   └─── cs300/
│       └─── SearchRequest.java
│       └─── MessageJNI.java
│       └─── Worker.java
│       └─── PassageProcessor.java
│       └─── <additional supporting java source>
└─── CtCILibrary/
│    └─── Trie.java
│    └─── TrieNode.java
└─── makefile
└─── readme.md
```

## Questions to ask Dr. Anderson

1. While the functionality of the project seems to be well described, it is hard to understand the content of each source file. For example,your GitHub repository consists of 3 .c files (msgrcv_lwr.c, msgsnd_pr.c, system5_msg.c); however, the project prompt asks to have 2 .c files (searchmanager.c and system5msg.c). Is there any way you can briefly describe how each of these files are used?
