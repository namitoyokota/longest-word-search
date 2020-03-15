package edu.cs300;

import CtCILibrary.*;
import java.util.concurrent.*;

class Worker extends Thread {

  Trie textTrieTree;
  ArrayBlockingQueue prefixRequestArray;
  ArrayBlockingQueue resultsOutputArray;
  int id;
  String passageName;

  public Worker(String filename, String[] words, int id, ArrayBlockingQueue prefix, ArrayBlockingQueue results) {
    this.textTrieTree = new Trie(words);
    this.prefixRequestArray = prefix;
    this.resultsOutputArray = results;
    this.id = id;
    this.passageName = filename;
  }

  public void run() {
    System.out.println("Worker-" + this.id + " (" + this.passageName + ") thread started ...");
    while (this.prefixRequestArray.size() > 0) {
      try {
        String prefix = (String) this.prefixRequestArray.take();
        boolean found = this.textTrieTree.contains(prefix);

        if (!found) {
          System.out.println(passageName + ":" + prefix + " not found");
          // System.out.println("Worker-" + this.id + " " + req.requestID + ":" + prefix +
          // " ==> not found ");
          resultsOutputArray.put(passageName + ":" + prefix + " not found");
        } else {
          System.out.println(passageName + ":" + prefix + " found");
          // System.out.println("Worker-" + this.id + " " + req.requestID + ":" + prefix +
          // " ==> " + word);
          resultsOutputArray.put(passageName + ":" + prefix + " found");
        }
      } catch (InterruptedException e) {
        System.out.println(e.getMessage());
      }
    }
  }

}