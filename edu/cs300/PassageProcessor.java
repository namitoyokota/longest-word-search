package edu.cs300;

import java.util.concurrent.*;
import java.io.*;
import java.util.*;

public class PassageProcessor {

  public static void main(String[] args) {
    // main vars
    int treeCount = 0, count = 0;
    ArrayList<String> filenames = new ArrayList<String>();

    // read in passages.txt for passage filenames
    try {
      File file = new File("passages.txt");
      FileReader fr = new FileReader(file);
      BufferedReader br = new BufferedReader(fr);
      String line;
      while ((line = br.readLine()) != null) {
        filenames.add(line);
        count++;
      }
      treeCount = count;
      fr.close();
    } catch (IOException e) {
      e.printStackTrace();
    }

    // initialize workers and output array
    ArrayBlockingQueue[] prefixes = new ArrayBlockingQueue[treeCount];
    ArrayBlockingQueue resultsOutputArray = new ArrayBlockingQueue(treeCount * 10);
    String[][] list = new String[treeCount][];

    // multithreaded starts here!
    Thread[] threads = new Thread[treeCount];
    for (int i = 0; i < treeCount; i++) {
      threads[i] = new Thread();
      threads[i].start();
      list[i] = extractFile(filenames.get(i));
      prefixes[i] = new ArrayBlockingQueue(10);
      prefixes[i].add(new MessageJNI().readPrefixRequestMsg().prefix);
      prefixes[i].add(new MessageJNI().readPrefixRequestMsg().prefix);
      prefixes[i].add(new MessageJNI().readPrefixRequestMsg().prefix);
      new Worker(filenames.get(i), list[i], i, prefixes[i], resultsOutputArray).run();
    }
    for (int j = 0; j < treeCount; j++) {
      try {
        threads[j].join();
      } catch (InterruptedException e) {
        System.out.println("error :(");
      }
    }
  }

  /*
   * Extract a text file and return a trie object with all of the words from the
   * file
   */
  public static String[] extractFile(String filename) {
    ArrayList<String> list = new ArrayList<String>();
    Scanner sc2 = null;
    try {
      sc2 = new Scanner(new File(filename));
    } catch (FileNotFoundException e) {
      e.printStackTrace();
    }
    while (sc2.hasNextLine()) {
      Scanner s2 = new Scanner(sc2.nextLine());
      while (s2.hasNext()) {
        String s = s2.next();
        list.add(s);
        // System.out.println(s);
      }
    }
    String[] array = new String[list.size()];
    array = list.toArray(array);
    return array;
  }

}