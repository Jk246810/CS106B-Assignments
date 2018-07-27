#include "grammarsolver.h"
#include "map.h"
#include <fstream>
#include "filelib.h"
#include "vector.h"
#include "random.h"
using namespace std;


/**
 * @brief readFile: processes each line of the file and stores it into a map of strings and vectors
 * @param input: the file
 * @param map: the map wrapper that stores the words in the file
 */
void readFile(istream& input, Map<string, Vector<string> >& map){
    string line;
    while(getline(input, line)){
        Vector<string> key = stringSplit(line, "::=");
        if(map.containsKey(key.get(0))){
            throw "the file cannot contain the same non-terminal multiple times";
        }
        Vector<string> value = stringSplit(key.get(1), "|");
        //grabs the nonterminals and terminals and stores them in the mao
        map.put(key[0], value);
    }
}


/**
 * @brief grammarGeneratorHelper: the helper recursive function for grammarGenerate
 * @param map: map that stores the information from the file
 * @param output: the string created from the keys and values
 * @param symbol: the key for the map
 */
void grammarGeneratorHelper(const Map<string, Vector<string> >& map, string& output, string symbol){
    if(!map.containsKey(symbol)){
        output += trim(symbol) + " ";
        //if there is no key for the symbol that means it is a terminal and the function is done
    }else{
        Vector<string> values = map.get(symbol);
        //grabs the values for the given symbol
        int randNum = randomInteger(0, values.size()-1);
        string value = values.get(randNum);
        //chooses a random value
        Vector<string> terminals = stringSplit(value, " ");
        /*if there are multiple non-terminals in a value then you string split it into
        an array*/
        for(int i=0; i<terminals.size(); i++){ 
            grammarGeneratorHelper(map, output, terminals.get(i));
            /*goes through all of the non-terminal elements in the selected value and calls the function recursively
             * to find each terminal of the non-terminals and adds the found string to the output*/
        }
    }
}


/**
 * @brief grammarGenerate: generates a vector of strings based off of the users input, main function
 * @param input: the file
 * @param symbol: the symbol that the user is trying to find
 * @param times: the number of times the user wants to generate the symbol
 * @return: v: vector of outputs
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    if(symbol == ""){
        throw "Cannot enter empty string";
    }
    Map<string, Vector<string> > map;
    readFile(input, map);
    Vector<string> v;
    for(int i=0; i<times; i++){
        string output;
        grammarGeneratorHelper(map, output, symbol);
        v.add(output);
        /* performs a loop to call grammarGeneratorHelper so there is number of outputs == times
        adds the given output from grammar generator helper and adds it to the vector*/
    }
    return v;
}
