package edu.cs300;

import CtCILibrary.*;
import java.util.concurrent.*;
import java.io.*;
import java.util.*;

public class ParallelTextSearch {

  public static void main(String[] args) {

    System.out.println("\n--------------------------------------\n");
    // main vars
    int num_passages = 6;
    ArrayList<String> filenames = new ArrayList<String>();

    // read in passages.txt for passage filenames
    try {
      File file = new File("passages.txt");
      FileReader fr = new FileReader(file);
      BufferedReader br = new BufferedReader(fr);
      String line;
      while ((line = br.readLine()) != null) {
        filenames.add(line);
      }
      fr.close();
    } catch (IOException e) {
      e.printStackTrace();
    }

    // initialize workers and output array
    String[][] words = new String[num_passages][];

    // multithreaded starts here!
    for (int i = 0; i < num_passages; i++) {
      words[i] = extractFile(filenames.get(i));
      for (int j = 0; j < words[i].length; j++) {
        // System.out.println(words[i][j]);
      }
      // System.out.println();
    }

    Trie words1 = new Trie(words[0]);
    ArrayList<String> possibles = new ArrayList<String>();
    words1.getAllPossibilities(possibles, words1.getRoot(), "");
    // System.out.println(words1.getAllPossibilities(possibles, words1.getRoot(),
    // ""));
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
      }
    }
    String[] array = new String[list.size()];
    array = list.toArray(array);
    return array;
  }

}