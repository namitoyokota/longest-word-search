package edu.cs300;

import java.io.*;
import java.util.*;

public class PassageProcessor {

  public static void main(String[] args) {

    // main vars
    int num_passages = 0;
    ArrayList<String> filenames = new ArrayList<String>();

    // read in passages.txt for passage filenames
    try {
      File file = new File("passages.txt");
      FileReader fr = new FileReader(file);
      BufferedReader br = new BufferedReader(fr);
      String line;
      while ((line = br.readLine()) != null) {
        if (fileExists(line)) {
          filenames.add(line);
          num_passages++;
        }
      }
      fr.close();
    } catch (IOException e) {
      e.printStackTrace();
    }

    // initialize and store all words from all passages
    String[][] words = new String[num_passages][];
    for (int i = 0; i < num_passages; i++) {
      words[i] = extractFile(filenames.get(i));
    }

    // receive request
    SearchRequest req = new MessageJNI().readPrefixRequestMsg();
    while (true) {
      System.out.println("**prefix(" + req.requestID + ") " + req.prefix + " received");
      // if the requestId == 0, it is a signal to terminate the program
      if (req.requestID == 0) {
        System.out.println("Terminating ...");
        System.exit(0);
      }
      // if the requestId != 0, it is the id so start multithreaded search in all
      // passages
      else {
        for (int i = 0; i < num_passages; i++) {
          Thread worker = new Thread(new Worker(req, filenames.get(i), words[i], req.requestID, i + 1, num_passages));
          worker.start();
        }
        // get new request
        req = new MessageJNI().readPrefixRequestMsg();
      }
    }
  }

  public static Boolean fileExists(String filename) {
    try {
      Scanner sc2 = new Scanner(new File(filename));
    } catch (FileNotFoundException e) {
      return false;
    }
    return true;
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
        s = s.replaceAll("[.]", "");
        s = s.replaceAll("[,]", "");
        s = s.replaceAll("[!]", "");
        s = s.replaceAll("[?]", "");
        list.add(s.toLowerCase());
      }
    }
    String[] array = new String[list.size()];
    array = list.toArray(array);
    return array;
  }

}