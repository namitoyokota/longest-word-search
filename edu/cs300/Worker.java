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
    while (this.prefixRequestArray.remainingCapacity() > 0) {
      SearchRequest req = new MessageJNI().readPrefixRequestMsg();
      this.prefixRequestArray.add(req.prefix);
      String prefix = (String) req.prefix;
      boolean found = this.textTrieTree.contains(prefix);

      ArrayList<String> possibles = new ArrayList<String>();
      String longest = this.textTrieTree.getAllPossibilities(possibles, this.textTrieTree.getRoot(), "", prefix);

      if (!found) {
        System.out.println("Worker-" + this.id + " " + req.requestID + ":" + prefix + " ==> not found ");
        new MessageJNI().writeLongestWordResponseMsg(id, prefix, prefixRequestArray.size(), passageName, "", 3, 0);
        // resultsOutputArray.put(passageName + ":" + prefix + " not found");
      } else {
        System.out.println("Worker-" + this.id + " " + req.requestID + ":" + prefix + " ==> " + prefix + prefix);
        new MessageJNI().writeLongestWordResponseMsg(id, prefix, prefixRequestArray.size(), passageName, longest, 3, 1);
        // resultsOutputArray.put(passageName + ":" + prefix + " found");
      }
    }
  }
}