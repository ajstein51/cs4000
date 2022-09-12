/**
 * AJ Stein
 * CS4000 Program 2
 * We are to make a crossword checker in linear and then parallel
 * The solution seems to be mostly working depending on the file (data2 works 100%), the others are iffy
 * 
**/
#include <vector>
#include <string>
#include <algorithm>
#include "omp.h"
using namespace std;
#ifndef DWJ_WORD_CIRCLE_DEF
#define DWJ_WORD_CIRCLE_DEF

// Note to execute: ./a.out < [test file], school pc got 4 core

class CircleTheWordSolver {
 public:
    // returns the answer
    vector<pair<int, int> > word_locations(vector<vector<char> > &puzzle, vector<string> &wordlist);

    bool checkD(vector<vector<char> > &puzzle, string hold, int x, int y);

    // vars
    vector<vector<char> > puzzle;  // The board
    vector<string> wordlist;       // List of words
    
};
#endif

vector<pair<int, int> > CircleTheWordSolver::word_locations(vector<vector<char> > &puzzle, vector<string> &wordlist){
    vector<pair<int, int> > solution;                       // Vector to return 
    solution.resize(wordlist.size());                       
    
    vector<bool> done;                                      // to test at the end for -1 -1
    done.resize(wordlist.size());

    #pragma omp parallel
    for(int i = 0; i < puzzle.size(); i++){                 // row
        for(int j = 0; j < puzzle.size(); j++){             // col
            for(int k = 0; k < wordlist.size(); k++){       // word count
                if (puzzle[i][j] == wordlist[k][0]){        // if we got a match
                    if (checkD(puzzle, wordlist[k], i, j)){
                        solution[k] = make_pair(i, j);     // if true, we got the word and a location of the starting char
                        done[k] = true;
                    }
                } 
            }
        }
    }
    for(int i = 0; i < done.size(); i++){                  // elements that arent true means its not in the puzzle, make it -1 -1
        if(done[i] != true)
            solution[i] = make_pair(-1,-1); 
    }
    return solution;
}

bool CircleTheWordSolver::checkD(vector<vector<char> > &puzzle, string hold, int x, int y){
    
    vector<int> row = {-1, 1, -1, 1, -1, 0, 0,  1};          // rows of the puzzle
    vector<int> col = {1, 1, -1, 0,  0, -1, 1, -1};         // columns of the puzzle
    int xd, yd, j, i;                               // counter to see when the word is done
    
    for(i = 0; i <= 7; i++){                           // const of 7 since theres only 8 ways it can go
        xd = x + row.at(i);
        yd = y + col.at(i);
        for(j = 1; j < hold.length(); j++){
          if(xd - 1 < 0 || xd + 1 > puzzle.size() || yd - 1 < 0 || yd + 1 > puzzle.size() || puzzle[xd][yd] != hold[j])
                goto ENDLOOP;                              // Out of area, go to the next iteration
            
            xd += row.at(i);
            yd += col.at(i);

        } // end j  
        if (j == hold.length())                      // The word is done and its a match, return true
            return true; // we found the word
            
        ENDLOOP: {}
    } // end i
    return false;                                          // we didnt find the word
}
