<!-- @format -->

# Longest Word Finder

> a multithreaded two process system that communicates via System V message queues designed to find the longest word that begins with the supplied prefix in a series of text passages

## Logic

### Search Manager Logic (written in C)
reads one or more prefixes from the command line, creates and sends prefix request messages (contains prefix string and prefix ID) via System V ipc queues and waits for the passage processor to return a series of responses. The search manager will print the results for each prefix as once all responses for that prefix have been received.

### Passage Processor Logic (written in Java)
reads a series of passage file names from passages.txt. A thread will be created for each passage that builds Trie with words in the passage text, receives requests for longest word searches, searches the trie for the longest word and asynchronously returns the longest word.

## Make Commands
- `make` : compile all C and Java files necessary
- `make clean` : remove outputted files

## Execution
- `java -cp . -Djava.library.path=. edu.cs300.PassageProcessor` : execute the passage processor program
- `./searchmanager <delay> <prefix1> <prefix2> ...` : execute the search manager program

## Resources
- [Project Files](https://github.com/monicadelaine/Spring_cs300_project)
- [System V Message Queue](https://www.softprayog.in/programming/interprocess-communication-using-system-v-message-queues-in-linux)
- [JNI](https://www.baeldung.com/jni)
