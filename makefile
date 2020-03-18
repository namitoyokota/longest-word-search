all:
	export JAVA_HOME=/usr/java/latest
	make package-cs300
	make package-CtCILibrary
	make search-manager
	make jni

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

# run this on the first tab
processor:
	clear
	java -cp . -Djava.library.path=. edu.cs300.PassageProcessor

# run this on second tab
manager:
	clear
	./searchmanager 3 con pre wor

parallel-text-search:
	java -cp . -Djava.library.path=. edu.cs300.ParallelTextSearch

# useful tools
tools:
	make empty-queue
	make send-message
	make receive-message

# empty the system v queue
empty-queue:
	gcc -std=c99 -D_GNU_SOURCE empty_queue.c -o empty

# test: send type 1 message
send-message:
	gcc -std=c99 -D_GNU_SOURCE msgsnd_pr.c -o msgsnd

# test: receive type 2 message
receive-message:
	gcc -std=c99 -D_GNU_SOURCE msgrcv_lwr.c -o msgrcv

# remove all outputted files
clean: 
	rm -f ./msgsnd ./msgrcv ./searchmanager ./empty
	rm -f ./edu/cs300/*.class ./CtCILibrary/*.class
	rm -f *.so *.o