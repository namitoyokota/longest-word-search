package edu.cs300;

import CtCILibrary.*;
import java.util.concurrent.*;
import java.io.*;
import java.util.*;

public class ParallelTextSearch {

  public static void main(String[] args) {

    // main vars
    int treeCount = 0, count = 0;
    String[][] samples = { { "test0", "test0", "test0" }, { "test1", "test1", "test1" }, { "test2", "test2", "test2" },
        { "test3", "test3", "test3" }, { "test4", "test4", "test4" }, { "test5", "test5", "test5" } };
    ArrayList<String> filenames = new ArrayList<String>();

    // check command line input
    if (args.length == 0 || args[0].length() <= 2) {
      System.out.println("Provide prefix (min 3 characters) for search i.e. con\n");
      System.exit(0);
    }

    // read in passages.txt for passage filenames
    try {
      File file = new File("passages.txt"); // creates a new file instance
      FileReader fr = new FileReader(file); // reads the file
      BufferedReader br = new BufferedReader(fr); // creates a buffering character input stream
      String line;
      while ((line = br.readLine()) != null) {
        filenames.add(line);
        count++;
      }
      treeCount = count;
      fr.close(); // closes the stream and release the resources
    } catch (IOException e) {
      e.printStackTrace();
    }

    // initialize workers and output array
    /*
     * ArrayBlockingQueue[] workers = new ArrayBlockingQueue[treeCount];
     * ArrayBlockingQueue resultsOutputArray = new ArrayBlockingQueue(treeCount *
     * 10);
     */

    // create new workers and start thread
    /*
     * for (int i = 0; i < treeCount; i++) { workers[i] = new
     * ArrayBlockingQueue(10); new Worker(filenames.get(i), samples[i], i,
     * workers[i], resultsOutputArray).start(); }
     */

    // trie node testing
    ArrayList<String> list0 = extractFile(filenames.get(0));
    ArrayList<String> list1 = extractFile(filenames.get(1));
    ArrayList<String> list2 = extractFile(filenames.get(2));
    ArrayList<String> list3 = extractFile(filenames.get(3));
    ArrayList<String> list4 = extractFile(filenames.get(4));
    ArrayList<String> list5 = extractFile(filenames.get(5));
    Trie file0 = new Trie(list0);
    Trie file1 = new Trie(list1);
    Trie file2 = new Trie(list2);
    Trie file3 = new Trie(list3);
    Trie file4 = new Trie(list4);
    Trie file5 = new Trie(list5);
    System.out.println("file0: " + file0.contains("con"));
    System.out.println("file1: " + file1.contains("con"));
    System.out.println("file2: " + file2.contains("con"));
    System.out.println("file3: " + file3.contains("con"));
    System.out.println("file4: " + file4.contains("con"));
    System.out.println("file5: " + file5.contains("con"));
  }

  /*
   * Extract a text file and return a trie object with all of the words from the
   * file
   */
  public static ArrayList<String> extractFile(String filename) {
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

    return list;
  }

}