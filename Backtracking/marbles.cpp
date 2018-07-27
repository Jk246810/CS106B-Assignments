/*
 * File: marbles.cpp
 * Author: Jamee Krzanich
 * ----------------------
 * This file implements logic for finding a solution to the marble solitaire.
 */
#include "grid.h"
#include "vector.h"
#include "marbletypes.h"
#include "compression.h"
#include "marbleutil.h"
#include "set.h"
using namespace std;

bool helper(Grid<Marble>& board, int marbleCount,
            Vector<Move>& moveHistory, Set<BoardEncoding>& badPath);

/**
 * @brief solvePuzzle: solves the marble solitaire
 * @param board: board of marbles to solve
 * @param marbleCount: number of marbles left on the board
 * @param moveHistory: previous moves to keep track of
 * @return: wheter or not the board solitaire game can be solved
 */
bool solvePuzzle(Grid<Marble>& board, int marbleCount, Vector<Move>& moveHistory) {
    Set<BoardEncoding> encoding;
    return helper(board, marbleCount, moveHistory, encoding);
}

/**
 * @brief helper: recursive backtracking function figures which moves will solve the board
 * @param board: board of marbles, solitaire board
 * @param marbleCount: number of marbles currently on the board
 * @param moveHistory: previous moves
 * @param badPath: previous moves that have not worked
 * @return if there is a path that solves the board return true else false
 */
bool helper(Grid<Marble>& board, int marbleCount,
            Vector<Move>& moveHistory, Set<BoardEncoding>& badPath){
    if(marbleCount==1){
        return true;
    }
    BoardEncoding encoding = compressBoard(board);
    if(badPath.contains(encoding)){
        return false;
    }
    Vector<Move> moves = findPossibleMoves(board);
    if(moves.size() == 0){
            badPath.add(encoding);
            return false;
    }
    for(int i=0; i<moves.size(); i++){
        //choose
        makeMove(moves.get(i), board);
        moveHistory.add(moves.get(i));
        if(helper(board, marbleCount-1, moveHistory, badPath)){ //explore
                return true;
        }
        //unchoose
        undoMove(moves.get(i), board);
        moveHistory.remove(moveHistory.size()-1);
        badPath.add(encoding);
    }
    return false;
}
