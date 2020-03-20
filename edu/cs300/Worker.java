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
  int passageCount;

  public Worker(SearchRequest req, String filename, String[] words, int prefix_id, int passage_id, int passage_count) {
    this.req = req;
    this.textTrieTree = new Trie(words);
    this.id = prefix_id;
    this.index = passage_id;
    this.passageName = filename;
    this.passageCount = passage_count;
  }

  public void run() {
    System.out.println("Worker-" + this.id + " (" + this.passageName + ") thread started ...");

    String prefix = (String) req.prefix;
    boolean found = this.textTrieTree.contains(prefix);

    ArrayList<String> possibles = new ArrayList<String>();
    String longest = this.textTrieTree.getAllPossibilities(possibles, this.textTrieTree.getRoot(), "", req.prefix);

    if (!found) {
      System.out.println("Worker-" + this.index + " " + req.requestID + ":" + prefix + " ==> not found ");
      new MessageJNI().writeLongestWordResponseMsg(this.id, prefix, this.index, passageName, "", passageCount, 0);
      // resultsOutputArray.put(passageName + ":" + prefix + " not found");
    } else {
      System.out.println("Worker-" + this.index + " " + req.requestID + ":" + prefix + " ==> " + longest);
      new MessageJNI().writeLongestWordResponseMsg(this.id, prefix, this.index, passageName, longest, passageCount, 1);
      // resultsOutputArray.put(passageName + ":" + prefix + " found");
    }
  }
}