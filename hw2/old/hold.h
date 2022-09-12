#include <vector>
#include <string>
#include <algorithm>
using namespace std;

#ifndef DWJ_WORD_CIRCLE_DEF
#define DWJ_WORD_CIRCLE_DEF

// Note to execute: ./a.out < [test file], school pc got 4 core

class CircleTheWordSolver {
 public:
    // returns the answer
    vector<pair<int, int> > word_locations(vector<vector<char> > &puzzle, vector<string> &wordlist);


    // Functions
   

    // vars
    vector<vector<char> > puzzle;  // The board
    vector<string> wordlist;       // List of words
    
};
#endif


// to return the words 
vector<pair<int, int> > CircleTheWordSolver::word_locations(vector<vector<char> > &puzzle, vector<string> &wordlist) {
    vector<pair<int, int> > solution;
    solution.resize(wordlist.size());

  // Note i is the col, doing + 1 makes it go down 1, -1 goes up 1 in the matrix
    for(int i = 0; i <= puzzle.size() - 1; i++){ // col
        for(int j = 0; j <= puzzle.size() - 1; j++){ // row
            for(int k = 0; k <= wordlist.size() - 1; k++){ // finds words 1 by 1
                if(puzzle[i][j] == wordlist[k][0]){ // found the letter of the word
                // Now start to check what direction its in (8 directions), then it should only ever be in that
                // direction      
                    
                    if(j + 1 <= puzzle.size() - 1){   // GOOD
                        for(int l = 0; l <= wordlist[k].size() + 1; l++){
                            if(puzzle[i][j+1] == wordlist[k][l]){ // checks east 
                                    
                                if(wordlist[k].back() == wordlist[k][l]){
                                    
                                   solution[k] = make_pair(j, i);
                                   goto ENDLOOP;
                                }
                            }
                        }
                    } /*
                    if(j - 1 >= 0){ // GOOD
                    
                        for(int l = 1; l <= wordlist[k].size() - 1; l++){
                            
                            if(puzzle[i][j-1] == wordlist[k][l]){ // checks west 
                                if(wordlist[k].back() == wordlist[k][l]){
                                    solution[k] = make_pair(j, i);
                                    goto ENDLOOP;
                                }
                            }
                        }
                    } 
                    if (i + 1 <= puzzle.size() - 1){ // GOOD
                        for(int l = 1; l <= wordlist[k].size() - 1; l++){
                            if(puzzle[i+1][j] == wordlist[k][l]){ // checks south 
                            
                            
                                if(wordlist[k].back() == wordlist[k][l]){
                                    solution[k] = make_pair(j, i);
                                    goto ENDLOOP;

                                }
                            }
                        }
                    } 
                    if(i - 1 >= 0){ // GOOD
                        for(int l = 1; l <= wordlist[k].size() - 1; l++){
                            if(puzzle[i-1][j] == wordlist[k][l]){ // checks north 
                                
                                if(wordlist[k].back() == wordlist[k][l]){
                                    solution[k] = make_pair(j, i);
                                    goto ENDLOOP;
                                }
                            }
                        }
                    } 
                    
                    if(i - 1 >= 0 && j - 1 >= 0){
                        for(int l = 1; l <= wordlist[k].size() - 1; l++){ // good
                            if(puzzle[i-1][j-1] == wordlist[k][l]){ // up and left
                                
                                if(wordlist[k].back() == wordlist[k][l]){
                                    
                                    solution[k] = make_pair(j, i);
                                   goto ENDLOOP;
                                }
                            }
                        }
                    }
                    if(i - 1 >= 0 && j + 1<= puzzle.size() - 1){
                        for(int l = 1; l <= wordlist[k].size() - 1; l++){ // good
                            if(puzzle[i-1][j+1] == wordlist[k][l]){ // up and right
                                if(wordlist[k].back() == wordlist[k][l]){
                                    cout << "1 " << wordlist[k].back() << endl;
                                    cout << "2 " << wordlist[k][l] << endl;
                                    solution[k] = make_pair(j, i);
                                    goto ENDLOOP;
                                }
                            }
                        }
                    }
                    if(j - 1 >= 0 && i + 1 <= puzzle.size() - 1){
                        for(int l = 1; l <= wordlist[k].size() - 1; l++){ // good
                            if(puzzle[i+1][j-1] == wordlist[k][l]){ // down and left
                                if(wordlist[k].back() == wordlist[k][l]){
                                    solution[k] = make_pair(j, i);
                                   goto ENDLOOP;
                                }
                            }
                        }
                    }
                    if(j + 1<= puzzle.size() - 1 && i + 1 <= puzzle.size() - 1){
                        for(int l = 1; l <= wordlist[k].size() - 1; l++){
                            if(puzzle[i+1][j+1] == wordlist[k][l]){// down and right
                                if(wordlist[k].back() == wordlist[k][l]){
                                    solution[k] = make_pair(j, i);
                                    goto ENDLOOP;
                                }
                            }
                        }
                    } 
                        */
                } // first if
                
                if(solution[k].first == 0 && solution[k].second == 0)
                    solution[k] = make_pair(-1, -1);
                ENDLOOP:
                    {}
            } // third for
            
        } // second for
    } // first for
  return solution;
} 
