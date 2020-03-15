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
    ArrayBlockingQueue[] workers = new ArrayBlockingQueue[treeCount];
    ArrayBlockingQueue resultsOutputArray = new ArrayBlockingQueue(treeCount * 10);

    // experimenting
    MessageJNI test = new MessageJNI();

    // worker 1 testing
    String[] list0 = extractFile(filenames.get(0));
    workers[0] = new ArrayBlockingQueue(10);
    workers[0].add("con");
    workers[0].add("but");
    workers[0].add("yes");
    new Worker(filenames.get(0), list0, 0, workers[0], resultsOutputArray).run();
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