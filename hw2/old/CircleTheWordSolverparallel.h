/**
 * AJ Stein
 * CS4000 Program 2
 * We are to make a crossword checker in linear and then parallel
 * The solution seems to be mostly working depending on the file, the others are iffy
 * 
**/
#include <vector>
#include <string>
#include <algorithm>
#include <omp.h>
using namespace std;

#ifndef DWJ_WORD_CIRCLE_DEF
#define DWJ_WORD_CIRCLE_DEF

// Note to execute: ./a.out < [test file], school pc got 4 core
// to compiler with openmp do g++ -fopenmp *.cc

class CircleTheWordSolver {
 public:
    // returns the answer
    vector<pair<int, int> > word_locations(vector<vector<char> > &puzzle, vector<string> &wordlist);

    // vars
    vector<vector<char> > puzzle;  // The board
    vector<string> wordlist;       // List of words
    
};
#endif

// to return the words 
vector<pair<int, int> > CircleTheWordSolver::word_locations(vector<vector<char> > &puzzle, vector<string> &wordlist) {
    vector<pair<int, int> > solution;
    solution.resize(wordlist.size());
    string hold;        // append matching letters to check for the entire word
    vector<bool> done; // to test at the end for -1 -1
    done.resize(wordlist.size());
    
  // Note i is the col, doing + 1 makes it go down 1, -1 goes up 1 in the matrix
#pragma omp for schedule(runtime)
    for(int i = 0; i < puzzle.size(); i++){ // col
        for(int j = 0; j < puzzle.size(); j++){ // row
            for(int k = 0; k < wordlist.size(); k++){ // finds words 1 by 1
                if((puzzle[i][j] == wordlist[k][0]) ){ // found the letter of the word
                // Now start to check what direction its in (8 directions), then it should only ever be in that
                // direction
                // The first if of each of these blocks is to check the placement of the board to make sure theres no seg faults
                // Then it adds a letter to the string and will eventually check if the two strings are identical 
                // Then marks the location of the start of the string. Then jumps to the end cause it doesnt need to check anymore
                // For the current work k
//**********************************************************************************************************                   
                    if(j + 1 <= puzzle.size()){ // check for edge of the board
                        hold.push_back(wordlist[k][0]);
                        for(int l = 1; l <= wordlist[k].size(); l++){
                            hold.push_back(wordlist[k][l]);
                            if(puzzle[i][j+1] == wordlist[k][l]){ // checks east 
                                hold.push_back(puzzle[i][j+1]);
                                if(hold == wordlist[k]){ 
                                    solution[k] = make_pair(i, j);
                                    hold.clear();
                                    done[k] = true;
                                }
                            }
                            else
                                hold.pop_back();
                        }
                        hold.clear();
                    } 
//**********************************************************************************************************                   
                    if(j - 1 >= 0){ // check for the edge of the board
                        hold.push_back(wordlist[k][0]);
                        for(int l = 1; l <= wordlist[k].size(); l++){
                            hold.push_back(wordlist[k][l]);
                            if(puzzle[i][j-1] == wordlist[k][l]){ // check west
                                hold.push_back(wordlist[k][l]);
                                if(hold == wordlist[k]){
                                    solution[k] = make_pair(i, j);
                                    done[k] = true;
                                    hold.clear();
                                }
                            }
                            else
                                hold.pop_back();
                        }
                        hold.clear();
                    } 
//**********************************************************************************************************                   
                    if (i + 1 < puzzle.size() - 1){  // check the edge of the board
                        hold.push_back(wordlist[k][0]);

                        for(int l = 1; l <= wordlist[k].size(); l++){
                            hold.push_back(wordlist[k][l]);
                            
                            if(puzzle[i+1][j] == wordlist[k][l]){ // checks south

                                hold.push_back(puzzle[i+1][j]);

                                if(hold == wordlist[k]){
                                    solution[k] = make_pair(i, j);
                                    done[k] = true;
                                    hold.clear();
                                }
                            }
                            else 
                                hold.pop_back();
                        }
                        hold.clear();
                    } 
//**********************************************************************************************************                   
                    if(i - 1 >= 0){ // check the edge of the board
                        hold.push_back(wordlist[k][0]);

                        for(int l = 1; l <= wordlist[k].size() - 1; l++){ 
                            hold.push_back(wordlist[k][l]);

                            if(puzzle[i-1][j] == wordlist[k][l]){ // checks north

                                hold.push_back(wordlist[k][l]);
                                if(hold[l] == wordlist[k][l]){
                                    solution[k] = make_pair(i, j);
                                    done[k] = true;
                                    hold.clear();

                                }
                            }
                            else
                                hold.pop_back();
                            
                        }
                        hold.clear(); 
                    } 
//**********************************************************************************************************                   
                    if(i - 1 >= 0 && j - 1 >= 0){           // check the edge of the board
                        hold.push_back(wordlist[k][0]);
                        for(int l = 1; l <= wordlist[k].size() - 1; l++){ 
                            hold.push_back(wordlist[k][l]);
                            if(puzzle[i-1][j-1] == wordlist[k][l]){ // checks north west

                                    hold.push_back(wordlist[k][l]);
                                    if(hold[l] == wordlist[k][l]){
                                        solution[k] = make_pair(i, j);
                                        hold.clear();
                                        done[k] = true;
                                    }
                            }
                            else   
                                hold.pop_back();
                        }
                        hold.clear();
                    } 
//**********************************************************************************************************                   
                    if(i - 1 >= 0 && j + 1 <= puzzle.size()){           // check the edge of the board
                        hold.push_back(wordlist[k][0]);

                        for(int l = 1; l <= wordlist[k].size(); l++){ // up and right
                            hold.push_back(wordlist[k][l]);
                            if(puzzle[i-1][j+1] == wordlist[k][l]){ // checks north east
                                hold.push_back(wordlist[k][l]);

                                if(hold[l] == wordlist[k][l]){
                                    solution[k] = make_pair(i, j);
                                    hold.clear();
                                    done[k] = true;
                                }
                            }
                            else
                                hold.pop_back();
                            
                        }
                        hold.clear();
                    } 
//**********************************************************************************************************                   
                    if(j + 1 < puzzle.size() - 1 && i + 1 < puzzle.size() - 1){ // check the edge of the board
                        hold.push_back(wordlist[k][0]);
                        for(int l = 0; l <= wordlist[k].size() - 1; l++){ // down and left
                            hold.push_back(wordlist[k][l]);
                            if(puzzle[i+1][j+1] == wordlist[k][l]){ // checks south east

                                hold.push_back(wordlist[k][l]);
                                if(hold[l] == wordlist[k][l]){
                                    solution[k] = make_pair(i, j);
                                    hold.clear();
                                    done[k] = true;
                                }
                            }
                            else   
                                hold.pop_back();
                        }
                        hold.clear();
                    }   
//**********************************************************************************************************                   
                    if(j - 1 >= 0 && i + 1 < puzzle.size() - 1){ // down and right  check the edge of the board
                        hold.push_back(wordlist[k][0]);

                        for(int l = 1; l <= wordlist[k].size() - 1; l++){
                            hold.push_back(wordlist[k][l]);
                            if(puzzle[i+1][j-1] == wordlist[k][l]){ // checks south west

                                hold.push_back(wordlist[k][l]);
                                if(hold[l] == wordlist[k][l]){
                                    solution[k] = make_pair(i, j);
                                    hold.clear();
                                    done[k] = true;
                                }
                            }
                            else   
                                hold.pop_back();
                        }
                        hold.clear();
                    }
                } // first if
            } // third for
        } // second for
    } // first for
//**********************************************************************************************************                   
    // check to see if something didnt match then put in a -1 -1
    #pragma omp parallel for
     for(int i = 0; i < done.size(); i++){
        if(done[i] != true)
            solution[i] = make_pair(-1,-1); 
    }         
  return solution;
} 