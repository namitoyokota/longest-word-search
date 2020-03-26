package CtCILibrary;

import java.util.ArrayList;

/* Implements a trie. We store the input list of words in tries so
 * that we can efficiently find words with a given prefix. 
 */
public class Trie {
    // The root of this trie.
    private TrieNode root;

    /*
     * Takes a list of strings as an argument, and constructs a trie that stores
     * these strings.
     */
    public Trie(ArrayList<String> list) {
        root = new TrieNode();
        for (String word : list) {
            root.addWord(word);
        }
    }

    /*
     * Takes a list of strings as an argument, and constructs a trie that stores
     * these strings.
     */
    public Trie(String[] list) {
        root = new TrieNode();
        for (String word : list) {
            root.addWord(word);
        }
    }

    /*
     * Checks whether this trie contains a string with the prefix passed in as
     * argument.
     */
    public boolean contains(String prefix, boolean exact) {
        TrieNode lastNode = root;
        int i = 0;
        for (i = 0; i < prefix.length(); i++) {
            lastNode = lastNode.getChild(prefix.charAt(i));
            if (lastNode == null) {
                return false;
            }
        }
        return !exact || lastNode.terminates();
    }

    public boolean contains(String prefix) {
        return contains(prefix, false);
    }

    public TrieNode getRoot() {
        return root;
    }

    /*
     * Returns all of the words stored in Trie
     */
    public String getAllPossibilities(ArrayList<String> possibles, TrieNode start, String current, String prefix) {
        for (char letter = 'a'; letter <= 'z'; letter++) {
            TrieNode child = start.getChild(letter);
            if (child == null) {
                continue;
            } else {
                if (child.terminates()) {
                    if (startsWith(current.concat(String.valueOf(letter)), prefix))
                        possibles.add(current.concat(String.valueOf(letter)));
                }
                getAllPossibilities(possibles, child, current.concat(String.valueOf(letter)), prefix);
            }
        }
        return getLongest(possibles);
    }

    /*
     * Checks if the word starts with the given prefix
     */
    public boolean startsWith(String word, String prefix) {
        if (prefix.length() > word.length())
            return false;
        for (int i = 0; i < prefix.length(); i++) {
            if (word.charAt(i) != prefix.charAt(i))
                return false;
        }
        return true;
    }

    /*
     * Returns the longest string word from the ArrayList
     */
    public String getLongest(ArrayList<String> words) {
        if (!words.isEmpty()) {
            String longest = words.get(0);
            for (int i = 0; i < words.size(); i++) {
                if (longest.length() <= words.get(i).length())
                    longest = words.get(i);
            }
            return longest;
        }
        return "";
    }
}