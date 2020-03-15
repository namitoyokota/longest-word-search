package edu.cs300;

import java.util.concurrent.*;

//statements only relevant on Mac; Use statements on readme instead
//javac -h . MessageJNI.java
//gcc -c -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/darwin system5_msg.c -o edu_cs300_MessageJNI.o
//gcc -dynamiclib -o libsystem5msg.dylib edu_cs300_MessageJNI.o -lc
//java -cp . -Djava.library.path=. edu.cs300.MessageJNI

public class MessageJNI {

    static {
        System.loadLibrary("system5msg");
    }

    public static void main(String[] args) {
        System.out.println(new MessageJNI().readPrefixRequestMsg());
        new MessageJNI().writeLongestWordResponseMsg(1, "con", 1, "Mandfield_Park.txt", "nyokota", 1, 1);
        new MessageJNI().writeLongestWordResponseMsg(2, "pre", 2, "Peter_Pan.txt", "nyokota", 2, 1);
        new MessageJNI().writeLongestWordResponseMsg(3, "wor", 3, "Pride_And_Prejustice", "nyokota", 3, 1);
    }

    private static native String readStringMsg();

    public static native SearchRequest readPrefixRequestMsg();

    public static native void writeLongestWordResponseMsg(int prefixID, String prefix, int passageIndex,
            String passageName, String longestWord, int passageCount, int present);

}