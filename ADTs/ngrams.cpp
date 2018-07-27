/*
 * File: ngrams.cpp
 * Author: Jamee Krzanich
 * ----------------------
 * This file implements ngrams algorithm
 */

#include <iostream>
#include "console.h"
#include "simpio.h"
#include <fstream>
#include "filelib.h"
#include "vector.h"
#include "queue.h"
#include "map.h"
#include "random.h"
using namespace std;

/** Prints out an introductory message and simulation rules. */
void intro(){
    cout<< "Welcome to CS 106B/X Random Writer ('N-Grams')!"<<endl<<
           "This program makes random text based on a document."<<endl<<
           "Give me an input file and an 'N' value for groups of"<<endl<<
           "words, and I will create random text for you."<<endl<<
           ""<<endl;
}

/** function that assigns the value of N
 *@param n: the value of N
*/

void nGramSize(int& n){
    int num = getInteger("Value of N? ");
    while(num < 2){
        cout<<"N must be 2 or greater."<<endl;
        num=getInteger("Value of N? ");
    }
    cout<<endl;
    n = num;
}
/** function creates a key of the map and assigns the appropriate values
 *@param nGrams: map of vectors used to store the values and keys used to generate the random output
 *@param pKeys: is the stored keys from the previous iteration to use for the current
 *@param pValues: is the stored values from the previous iteration to use for the current
 * @param variable: this is the word that will be added as the value to the current key being made in the function
*/
void createMap(Map<Vector<string> ,Vector<string> >&  nGrams, Vector<string>& pKeys,
Vector<string>& pValues, string variable){
    Vector<string> keys;
    Vector<string> values;
    for(int i=1; i<pKeys.size(); i++)
    {
        keys.add(pKeys.get(i));
    }
    keys.add(pValues.get(pValues.size()-1));


    if(nGrams.containsKey(keys))
    {
        values = nGrams.get(keys);
        values.add(variable);
        nGrams[keys] = values;
    }else{
        values.add(variable);
        nGrams[keys] = values;

    }

    pKeys = keys;
    pValues = values;
}

/** function reads the input file word by word and uses the word to create the map of values and keys
 *@param nGrams: map of vectors used to store the values and keys used to generate the random output
 *@param n: value of the N to determine the nGrams
*/
void readFile(Map<Vector<string> ,Vector<string> >&  nGrams, int& n){
    ifstream infile;
    string input = promptUserForFile(infile, "Input file name? ");
    nGramSize(n);

    string variable;

    Queue<string> fileWords;
    Vector<string> pKeys;
    Vector<string> pValues;
    int k=0;
    int j=1;
    while(infile >> variable){

        if(j<n){
            pKeys.add(variable);
            j++;
        }else if(j==n)
        {
            pValues.add(variable);
            nGrams.put(pKeys, pValues);
            j++;
        }else{
            createMap(nGrams, pKeys, pValues, variable);
        }
        if(k<n){
            fileWords.enqueue(variable);
            k++;
         }
    }
    for(int i=0; i<n-1; i++)
    {
        createMap(nGrams, pKeys, pValues, fileWords.dequeue());
    }
    infile.close();
}

/** function prompts the user for the number of random words that they want to generate and stores the value
 *@param n: value of the N to determine the nGrams, used to compare the whether the number of random words to generate is greater than n.
 * @return the number of random words to generate
*/
int numWords(int n){
    int input = getInteger("# of random words to generate (0 to quit): ");
    while(input<n){
        if(input!=0 && input<n){
            cout<<"Must be at least "<<n<<" words."<<endl;
        }else{
            break;
        }
        input=getInteger("# of random words to generate (0 to quit): ");
    }
    return input;
}

/** function chooses random keys and prints out the random value of the key to produce and print a random string.
 *@param nGrams: map of vectors used to store the values and keys used to generate the random output
 *@param n: value of the N to determine the nGrams, used to compare the whether the number of random words to generate is greater than n.
 * @param gameOver: tells the main function if the user is or isn't done generating random sentences
*/
void randomWords(Map<Vector<string> ,Vector<string> >&  nGrams, int& n, bool& gameOver){

    int words = numWords(n);
    if(words!=0){
        Vector<Vector<string> > keys = nGrams.keys();
        int randKey = randomInteger(0,keys.size()-1);

        Vector<string> key = keys.get(randKey);
        Vector<string> value = nGrams.get(key);
        string randVal = value.get(randomInteger(0, value.size()-1));
        cout<<"... ";
        for(int i=0; i<n-1; i++){
            cout<<key.get(i)<<" ";
        }
        cout<<randVal<<" ";
        int k=0;
        k+=n;
        while(true){
            Vector<string> nKey;
            for(int j=1; j<n-1; j++){
                nKey.add(key.get(j));
            }
            nKey.add(randVal);
            value = nGrams.get(nKey);
            randVal = value.get(randomInteger(0, value.size()-1));
            cout<<randVal<<" "; k++;
            key=nKey;
            if(k==words){
                cout<<"..."<<endl;
                cout<<endl;
                break;
            }
        }
    }else{
        gameOver = true;
    }
}

int main() {
    int n = 0;
    Map<Vector<string> ,Vector<string> >  nGrams;
    intro();
    readFile(nGrams, n);
    bool gameOver = false;
    while(!gameOver){
        randomWords(nGrams, n, gameOver);
    }
    cout << "Exiting." << endl;
    return 0;
}
