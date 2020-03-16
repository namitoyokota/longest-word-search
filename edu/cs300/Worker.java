package edu.cs300;

import CtCILibrary.*;
import java.util.concurrent.*;
import java.io.*;
import java.util.*;

class Worker extends Thread {

  Trie textTrieTree;
  ArrayBlockingQueue prefixRequestArray;
  ArrayBlockingQueue resultsOutputArray;
  int id;
  int index;
  String passageName;
  int origPrefixSize;

  public Worker(String filename, String[] words, int id, ArrayBlockingQueue prefix, ArrayBlockingQueue results) {
    this.textTrieTree = new Trie(words);
    this.prefixRequestArray = prefix;
    this.resultsOutputArray = results;
    this.id = id;
    this.index = id;
    this.passageName = filename;
    this.origPrefixSize = prefix.size();
  }

  public void run() {
    System.out.println("Worker-" + this.id + " (" + this.passageName + ") thread started ...");
    while (this.prefixRequestArray.size() > 0) {
      try {
        String prefix = (String) this.prefixRequestArray.take();
        boolean found = this.textTrieTree.contains(prefix);

        System.out.println();
        System.out.println(textTrieTree.findPossibleStrings(prefix));
        System.out.println();

        if (!found) {
          new MessageJNI().writeLongestWordResponseMsg(id, prefix, prefixRequestArray.size(), passageName,
              prefix + " " + prefix, 5, 0);
          System.out.println(passageName + ":" + prefix + " not found");
          resultsOutputArray.put(passageName + ":" + prefix + " not found");
        } else {
          new MessageJNI().writeLongestWordResponseMsg(id, prefix, prefixRequestArray.size(), passageName,
              prefix + " " + prefix, 5, 1);
          System.out.println(passageName + ":" + prefix + " found");
          resultsOutputArray.put(passageName + ":" + prefix + " found");
        }
      } catch (InterruptedException e) {
        System.out.println(e.getMessage());
      }
    }
  }
}