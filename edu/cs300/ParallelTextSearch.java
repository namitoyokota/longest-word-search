package edu.cs300;

import CtCILibrary.*;
import java.util.concurrent.*;
import java.io.*;

public class ParallelTextSearch {

  public static void main(String[] args) {

    // default
    int treeCount = 2;
    String[][] samples = { { "conspicuous", "parallel", "withering" }, { "coping", "figure", "parachute" } };
    ArrayBlockingQueue[] workers = new ArrayBlockingQueue[treeCount];
    ArrayBlockingQueue resultsOutputArray = new ArrayBlockingQueue(treeCount * 10);

    // my variables
    int num_passages = 0;

    if (args.length == 0 || args[0].length() <= 2) {
      System.out.println("Provide prefix (min 3 characters) for search i.e. con\n");
      System.exit(0);
    }

    // read in passages.txt for passage filenames
    try {
      File file = new File("passages.txt"); // creates a new file instance
      FileReader fr = new FileReader(file); // reads the file
      BufferedReader br = new BufferedReader(fr); // creates a buffering character input stream
      StringBuffer sb = new StringBuffer(); // constructs a string buffer with no characters
      String line;
      while ((line = br.readLine()) != null) {
        new Worker(line, samples[num_passages], num_passages, workers[num_passages], resultsOutputArray).start();
        num_passages++;
        sb.append(line); // appends line to string buffer
        System.out.println(line); // returns a string that textually represents the object
        sb.append("\n"); // line feed
      }
      fr.close(); // closes the stream and release the resources
    } catch (IOException e) {
      e.printStackTrace();
    }

    for (int i = 0; i < num_passages; i++) {
      workers[i] = new ArrayBlockingQueue(10);
    }

    try {
      workers[0].put(args[0]);
      workers[1].put(args[0]);
      workers[2].put(args[0]);
      workers[3].put(args[0]);
      workers[4].put(args[0]);
      workers[5].put(args[0]);
    } catch (InterruptedException e) {
    }
    ;

    int counter = 0;

    while (counter < num_passages) {
      try {
        String results = (String) resultsOutputArray.take();
        System.out.println("results:" + results);
        counter++;
      } catch (InterruptedException e) {
      }
      ;
    }
  }

}