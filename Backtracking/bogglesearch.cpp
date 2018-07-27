/*
 * File: bogglesearch.cpp
 * Author: Jamee Krzanich
 * ----------------------
 * This file implements logic for searching if a human input word is able to be made on the
 * board and the computer logic for finding all possible words that the user
 * has not found on the boggle board.
 */
#include <string>
#include "grid.h"
#include "lexicon.h"
#include "set.h"
#include "strlib.h"
#include "bogglegui.h"
using namespace std;

int score(string word);
bool humanHelper(string& word, int i, int length,
                 int row, int col,
                 Set<string> moves, Grid<char>& board);

/**
 * @brief humanWordSearch:finds the first letter of the word on the board and calls the recursive function
 * to search for the rest of the world
 * @param board: board of characters, boggle board
 * @param dictionary: lexicon of words in dictionary to verify user word
 * @param word: user inputted word
 * @return whether of not the word exists on the board
 */
bool humanWordSearch(Grid<char>& board, Lexicon& dictionary, string word) {
    Set<string> moves;
    toUpperCase(word);
    if(!dictionary.contains(word)){
        return false;
    }else if(word.length()<BoggleGUI::MIN_WORD_LENGTH){
        return false;
    }else{ 
        for (int r = 0; r < board.numRows(); r++) {
           for (int c = 0; c < board.numCols(); c++) {
               BoggleGUI::setHighlighted(r, c, true);
               if(board[r][c] == word[0]){
                   if(humanHelper(word, 1, word.length(), r,c,moves, board)){
                       BoggleGUI::clearHighlighting();
                       BoggleGUI::scorePointsHuman(score(word));
                       return true;
                   }
               }
               BoggleGUI::setHighlighted(r, c, false);
           }
        }

    }
    BoggleGUI::clearHighlighting();
    return false;
}

/**
 * @brief humanHelper: recursive function to search if the user word is on the board
 * @param word: user inputted word
 * @param i: the current index for char of word
 * @param length: length of the word
 * @param row: current row
 * @param col: current col
 * @param moves: moves that the program has made to search for the word
 * @param board: board of character, boggle board
 * @return if the word can be formed based on the rules of the game
 */
bool humanHelper(string& word, int i, int length,
                 int row, int col,
                 Set<string> moves, Grid<char>& board){
    BoggleGUI::setHighlighted(row, col, true);
    string move = integerToString(row) + integerToString(col);
    if(i == length) {
        return true;
    }
    if(moves.contains(move)) {
        return false;
    }
    moves.add(move);
    for (int r = -1; r <= 1; r++) {
        for (int c = -1; c <= 1; c++) {
            int r1 = (row + r);
            int c1 = (col + c);
            if(board.inBounds(r1, c1) && board[r1][c1] == word[i]){
                if(humanHelper(word, i+1, length, r1, c1, moves, board)){
                    return true;
                }
            }
        }
    }
    moves.remove(move);
    BoggleGUI::setHighlighted(row, col, false);
    return false;
}

void computerHelper(string result, Set<string>& s, Set<string> moves,
                    int row, int col, Grid<char>& board,
                    Lexicon& dictionary, Set<string>& humanWords);

/**
 * @brief computerWordSearch: finds all words that can be formed from the board that the user
 * did not find
 * @param board: boggle board, board of characters
 * @param dictionary: all real words
 * @param humanWords: the words that the human found that the computer cannot use
 * @return a set of all words that can be formed on the boggle board that the user did not find
 */
Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {
    Set<string> todo;
     Set<string> moves;
     string result;
     /*loops through each character on the board and calls the recursive helper function
     on the character to find all possible words the can be formed that start with that character */
    for(int r = 0; r < board.numRows(); r++) {
        for(int c = 0; c < board.numCols(); c++) {
            result = board[r][c];
            computerHelper(result, todo,moves, r, c, board, dictionary, humanWords);
        }
    }
    return todo;
}
/**
 * @brief computerHelper: recursive function that checks if the word can be formed on the board
 * @param result: word that is being formed
 * @param s:stores all the valid words that the computer can form with the board
 * @param moves: moves the that the computer has made
 * @param row: current row
 * @param col: current col
 * @param board: boggle board
 * @param dictionary: valid english words
 * @param humanWords: words that the human already guessed
 */
void computerHelper(string result, Set<string>& s, Set<string> moves,
                    int row, int col, Grid<char>& board,
                    Lexicon& dictionary, Set<string>& humanWords){

    string move = integerToString(row) + integerToString(col);
    if(!dictionary.containsPrefix(result)){
        /*if the word so far's prefix is not a valid english prefix (ex: qy)
        then automatically returns*/
        return;
    }
    /* adds word to set of word if the current result fulfills all requirements*/
    if(dictionary.contains(result) &&
       !humanWords.contains(result) &&
       result.length() >= BoggleGUI::MIN_WORD_LENGTH) {
        BoggleGUI::scorePointsComputer(score(result));
        s.add(result);
    }
    /*checks all characters around the current character if it equals
     *  the next character of the word*/
   moves.add(move);
    for (int r = -1; r <= 1; r++) {
       for (int c = -1; c <= 1; c++) {
           int r1 = (row + r);
           int c1 = (col + c);
           if(r1 != row || c1 != col){
               move = integerToString(r1) + integerToString(c1);
               if(board.inBounds(r1,c1) && !moves.contains(move)){
                   //choose
                   moves.add(move);
                   result += board[r1][c1];
                   //explore
                   computerHelper(result, s, moves, r1, c1, board, dictionary, humanWords);
                   //un-choose
                   moves.remove(move);
                   result = result.substr(0, result.size()-1);
               }
           }
       }
    }
    moves.remove(move);
}

/**
 * @brief score: calculates the score for the given valid word
 * @param word: the word that the function is finding the score of
 * @return the score calculated based on the length of the word
 */
int score(string word){
    int length = word.length();
    if(length <= 6){
        return length-3;
    }else if(length == 7){
        return 5;
    }else{
        return 11;
    }
}
