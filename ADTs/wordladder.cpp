/*
 * File: wordladder.cpp
 * Author: Jamee Krzanich
 * ----------------------
 * This file implements the shortest possible wordladder from one word to another.
 */
#include <iostream>
#include "console.h"
#include <fstream>
#include "filelib.h"
#include "simpio.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
using namespace std;

/** Prints out an introductory message and simulation rules. */
void intro()
{
    cout<<"Welcome to CS 106B/X Word Ladder!"<<endl;
    cout<<"Please give me two English words, and I will convert the"<<endl;
    cout<<"first into the second by modifying one letter at a time."<<endl;
    cout<<endl;
}

/** Function loads each line from the dictionary file and adds them to a set.
 * @return a set of dictionary words
 */
Set<string> loadFile(){
    ifstream infile;
    Set<string> d;
    promptUserForFile(infile, "Dictionary file name: ");
    string line;
    while (getline(infile, line)) {
        d.add(line);
    }
    infile.close();
    cout<<""<<endl;
    return d;
}
/** function finds all neighbors of a given word
 * @param w: the word that the program is finding neighbors of
 * @param d: set of dictionary words
 * @param neighbors: stores all the neighbors of w in a Queue
 */
void findNeighbors(string w,const Set<string>& d, Queue<string>& neighbors)
{

    string word = w;
    int length = w.length();
    for(int i=0; i<length; i++)
    {
        for (char ch = 'a'; ch <= 'z'; ch++)
        {
            word.replace(i, 1, string(1, ch));
            /*cite: (https://www.geeksforgeeks.org/how-to-convert-a-single-character-to-string-in-cpp/) used to
            figure out how to convert a character into a string*/
            if(d.contains(word)){
                neighbors.enqueue(word);
            }
        }
        word=w;
    }

}

/** finds the shortest possible ladder from one word to another
 * @param w1: the word that the user is trying to find a ladder to
 * @param w2: the starting word to build the ladder
 * @param d: set of dictionary words
 * @param result: the stack that is used to store the ladder.
 */
void createLadder(string w1, string w2, const Set<string>& d, Stack<string>& result){

    Queue<Stack<string> > ladders;
    Stack<string> holder;
    Set<string> allNeighbors;
    bool foundLadder = false;
    holder.add(w1);
    ladders.enqueue(holder);
    while(!foundLadder){
          holder = ladders.dequeue();
          Queue<string> neighbors;
          findNeighbors(holder.peek(), d, neighbors);
          while(!neighbors.isEmpty())
          {
              if(allNeighbors.contains(neighbors.peek()))
              {
                  neighbors.dequeue();
              }else{
                    Stack<string> ladder;
                    ladder = holder;
                    allNeighbors.add(neighbors.peek());
                     ladder.add(neighbors.dequeue());
                     ladders.enqueue(ladder);
                    if(ladder.peek().compare(w2) == 0)
                     {
                          foundLadder = true;
                          result=ladder;
                     }
              }
          }
          if(ladders.isEmpty())
          {
              break;
          }
    }

}

/** prints out the shortes possible ladder
 * @param w1: the word that the user is trying to find a ladder to
 * @param w2: the starting word to build the ladder
 * @param d: set of dictionary words
 */
void printResult(string w1, string w2, const Set<string>& d)
{
    Stack<string> ladder;
    createLadder(w1, w2, d, ladder);
    if(!ladder.isEmpty()){
    int size = ladder.size();
        for(int i=0; i<size; i++)
        {
            cout<<ladder.pop()<<" ";
        }
        cout<<endl;
    }else{
        cout<<"No word ladder found from"<< w2<<"back to"<<w1<<endl;
    }
}

int main() {
    intro();
    Set<string> dictionary = loadFile();

    while(true){
        string word1 = toLowerCase(getLine("Word 1 (or Enter to quit):"));

        if(word1 == "")
        {
            break;
        }
        string word2 = toLowerCase(getLine("Word 2 (or Enter to quit):"));
        if(word2 == "")
        {
            break;
        }

        if(word1.length() != word2.length()){
            cout << "The two words must be the same length." << endl;
            cout<<endl;
        }else if(!dictionary.contains(word1) ||!dictionary.contains(word2)){
            cout << "The two words must be found in the dictionary." << endl;
            cout<<endl;
        }else if(word1.compare(word2) == 0){
             cout << "The two words must be different." << endl;
             cout<<endl;
        }else if(word1 != "" && word2 != ""){

            cout << "A ladder from "<<word2<< " back to "<<word1<<":"<< endl;
            printResult(word1,word2,dictionary);

        }
        cout<<endl;
    }

    cout << "Have a nice day." << endl;
    return 0;
}
