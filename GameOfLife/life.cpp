
#include <iostream>
#include "console.h"
#include <fstream>
#include "filelib.h"
#include "grid.h"
#include "simpio.h"
#include "lifegui.h"
using namespace std;

void introduction(){
    cout << "Welcome to the CS 106B/X Game of Life!!"<< endl;
    cout << "This program simulates the lifecycle of a bacterial colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "* A cell with 1 or fewer neighbors dies." << endl;
    cout << "* Locations with 2 neighbors remain stable." << endl;
    cout << "* Locations with 3 neighbors will create life." << endl;
    cout << "* A cell with 4 or more neighbors dies." << endl;
    cout << "" << endl;
}

void loadGrid(Grid<string>&	g){
    ifstream infile;
    promptUserForFile(infile, "Grid input file name?");

    string line;

    getline(infile, line);
    int row = stringToInteger(line);
    getline(infile, line);
    int col = stringToInteger(line);

    g.resize(row, col);
    LifeGUI::resize(row, col);

    for(int r=0; r<g.numRows(); r++)
    {
        getline(infile, line);
        for(int c=0;c<g.numCols(); c++)
        {
            g.set(r,c, line.substr(0,1));
            line.erase(0,1);
            cout<<g.get(r,c);
            if(g.get(r,c).compare("X")==0)
            {
                LifeGUI::fillCell(r,c);
            }

        }
        cout<<""<<endl;
    }
    LifeGUI::repaint();

    infile.close();
}

void reloadGrid(const Grid<string>&	g)
{
    LifeGUI::clear();
    for(int r=0; r<g.numRows(); r++)
    {
        for(int c=0;c<g.numCols(); c++)
        {

            cout<<g.get(r,c);
            if(g.get(r,c).compare("X")==0){
                LifeGUI::fillCell(r,c);
            }
        }
        cout<<""<<endl;
    }
    LifeGUI::repaint();
}

int numNeighbors(const Grid<string>& g, int r, int c){
    int count = 0;
    int directTopRow = (r+ g.numRows()-1) % g.numRows();
    int directBottomRow = (r+1) % g.numRows();

    int directLeftCol = (c+ g.numCols()-1) % g.numCols();
    int directRightCol = (c+1) % g.numCols();

    if(g[directTopRow][c].compare("X") == 0)
    {
        count++;
    }
    if(g[directBottomRow][c].compare("X") == 0)
    {
        count++;
    }
    if(g[r][directLeftCol].compare("X") == 0)
    {
        count++;
    }
    if(g[r][directRightCol].compare("X") == 0)
    {
        count++;
    }
    if(g[directTopRow][directLeftCol].compare("X") == 0)
    {
        count++;
    }
    if(g[directTopRow][directRightCol].compare("X") == 0)
    {
        count++;
    }
    if(g[directBottomRow][directLeftCol].compare("X") == 0)
    {
        count++;
    }
    if(g[directBottomRow][directRightCol].compare("X") == 0)
    {
        count++;
    }

    return count;
}

void runSimulation(Grid<string>& g){
    Grid<string> result(g.numRows(), g.numCols());
    for(int r=0; r<g.numRows(); r++)
    {
        for(int c=0; c<g.numCols(); c++)
        {
            int neighbors = numNeighbors(g, r, c);
            if(neighbors <= 1)
            {
                result[r][c]= "-";
            }else if(neighbors == 3)
            {
                result[r][c]= "X";
            }else if(neighbors >= 4)
            {
                result[r][c] = "-";
            }else{
                result[r][c] = g[r][c];
            }

        }

    }
    g = result;
    reloadGrid(g);
}

void userMove(Grid<string>& g,string input, bool& endGame, bool& nR)
{
    if(equalsIgnoreCase(input, "t"))
    {
        runSimulation(g);


    }else if(equalsIgnoreCase(input, "a")){

        string cycles = getLine("How many frames?");
        while(!stringIsInteger(cycles)){
            cout<<"Illegal integer format. Try again."<<endl;
            cycles = getLine("How many frames?");
        }
        int numAnimations = stringToInteger(cycles);
        for(int i=0; i<numAnimations; i++)
        {
            clearConsole();
            pause(100);
            runSimulation(g);


        }

    }else if(equalsIgnoreCase(input, "q"))
    {

        string quitPrompt = getLine("Load another file? (y/n)");
        while (!equalsIgnoreCase(quitPrompt, "y") && !equalsIgnoreCase(quitPrompt, "n")) {
            cout<<"Please type a word that starts with 'Y' or 'N'."<<endl;
            quitPrompt = getLine("Load another file? (y/n)");
        }

        if(equalsIgnoreCase(quitPrompt, "n"))
        {
            endGame = true;
        }else if(equalsIgnoreCase(quitPrompt, "y"))
        {
            nR = true;
        }


    }else{
        cout<<"Invalid choice; please try again."<<endl;
    }
}

int main() {
    bool gameOver = false;
    introduction();
    Grid<string> board(0,0);

    LifeGUI::initialize();

    bool newRound = true;

    while(!gameOver){
        while(newRound){
            loadGrid(board);
            newRound = false;
        }

        string userInput= getLine("a)nimate, t)ick, q)uit?");
        userMove(board, userInput, gameOver, newRound);
    }

    cout << "Have a nice Life!" << endl;
    LifeGUI::shutdown();
    return 0;
}

