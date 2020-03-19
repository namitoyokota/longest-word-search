package edu.cs300;

import CtCILibrary.*;
import java.util.concurrent.*;
import java.util.*;

class Worker extends Thread {

  Trie textTrieTree;
  SearchRequest req;
  int id;
  int index;
  String passageName;

  public Worker(SearchRequest req, String filename, String[] words, int prefix_id, int passage_id) {
    this.req = req;
    this.textTrieTree = new Trie(words);
    this.id = prefix_id;
    this.index = passage_id;
    this.passageName = filename;
  }

  public void run() {
    System.out.println("Worker-" + this.id + " (" + this.passageName + ") thread started ...");

    String prefix = (String) req.prefix;
    boolean found = this.textTrieTree.contains(prefix);

    ArrayList<String> possibles = new ArrayList<String>();
    String longest = this.textTrieTree.getAllPossibilities(possibles, this.textTrieTree.getRoot(), "", req.prefix);

    if (!found) {
      System.out.println("Worker-" + this.id + " " + req.requestID + ":" + prefix + " ==> not found ");
      new MessageJNI().writeLongestWordResponseMsg(this.index, prefix, this.id, passageName, "", 3, 0);
      // resultsOutputArray.put(passageName + ":" + prefix + " not found");
    } else {
      System.out.println("Worker-" + this.id + " " + req.requestID + ":" + prefix + " ==> " + longest);
      new MessageJNI().writeLongestWordResponseMsg(this.index, prefix, this.id, passageName, longest, 3, 1);
      // resultsOutputArray.put(passageName + ":" + prefix + " found");
    }
  }
}