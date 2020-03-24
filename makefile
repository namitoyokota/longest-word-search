all:
	export JAVA_HOME=/usr/java/latest
	make package-cs300
	make package-CtCILibrary
	make search-manager
	make jni


############ COMPILE ALL ############

# compile java files in ./edu/cs300/ package
package-cs300:
	javac edu/cs300/*java

# compile java files in CtCILibrary package
package-CtCILibrary:
	javac CtCILibrary/*.java

# compile searchmanager
search-manager:
	gcc -std=c99 -D_GNU_SOURCE -lpthread searchmanager.c -o searchmanager

# compile jni files to connect java with c
jni:
	gcc -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux system5msg.c -o edu_cs300_MessageJNI.o
	gcc -shared -o libsystem5msg.so edu_cs300_MessageJNI.o -lc


############ EXECUTE TWO PROCESSES ############

# run this on the first tab (passage processor)
processor:
	clear
	java -cp . -Djava.library.path=. edu.cs300.PassageProcessor

# run this on second tab (search manager)
manager:
	clear
	./searchmanager 3 con pre wor


############ SAMPLE PROGRAMS ############

# reads queue contents using native C function and creates and sends a response message via the System V msg queue
message-jni:
	java -cp . -Djava.library.path=. edu.cs300.MessageJNI

# illustrates use of Java threading, Trie and BlockingArrayQueue
parallel-text-search:
	java -cp . -Djava.library.path=. edu.cs300.ParallelTextSearch


############ CLEAN DIRECTORY ############

clean: 
	rm -f ./msgsnd ./msgrcv ./searchmanager ./empty
	rm -f ./edu/cs300/*.class ./CtCILibrary/*.class
	rm -f *.so *.o