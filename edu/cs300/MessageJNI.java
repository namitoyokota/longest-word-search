package edu.cs300;

public class MessageJNI {

    static {
        System.loadLibrary("system5msg");
    }

    // dont know why this function is needed haha. maybe to check id and key?
    private static native String readStringMsg();

    // read type 1 messages from the system v queue
    public static native SearchRequest readPrefixRequestMsg();

    // write type 2 messages into the system v queue
    public static native void writeLongestWordResponseMsg(int prefixID, String prefix, int passageIndex,
            String passageName, String longestWord, int passageCount, int present);

}