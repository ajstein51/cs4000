#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string>
#include <array>
#include "CircleTheWordSolver.h"
#include <omp.h>
using namespace std;

//word_locations we initialize all the variables needed to solve this game
vector<pair<int, int> > CircleTheWordSolver::word_locations(vector<vector<char> > &puzzle, vector<string> &wordlist){
       vector<pair<int, int> > prov;
       string temp[wordlist.size()];
       char temp2[wordlist.size()];
       //in this for loop below we make two array one containing the words given and the other 
       //one containing the first char of every word given
       for(int i = 0; i < wordlist.size(); i++){
            temp[i] = wordlist[i];
            string firstLetter = temp[i];
            temp2[i] = firstLetter[0];     
       }
       //here we call findLetter to solve this game
       prov = findLetter(puzzle, temp, temp2, wordlist.size());
       return prov;
}

//findLetter is the hearth of the algorithm where most of th works happens
vector<pair<int, int> > CircleTheWordSolver::findLetter(vector<vector<char> > &puzzle, string temp[], char temp2[], int wordcount){
       pair<int, int> position;
       vector<pair<int, int> > final;
       //from line 39 until line 43 final is getting initialize by putting every word as "not found" 
       //or -1, -1 position of x and y
       final.reserve(wordcount);
       final.resize(wordcount);
       for(int i = 0; i < wordcount; i++){
              final[i] = make_pair(-1, -1);
       }
       //here is where we scan the matrix and try to find the position x and y of any letter that are
       //the first letter of any word given
       for(int y = 0; y < puzzle.size(); y++){
              for(int x = 0; x < puzzle.size(); x++){
                     int c = 0;
                     //here we compare each letter in the matrix with the first letter of every word given
                     do{
                            if(temp2[c] == puzzle[y][x]){
                                   //if a letter that is temp2 is found it is sent to moveLetter to check
                                   //that it is actually the first letter of the word given
                                   position = moveLetter(puzzle, temp[c], x , y);
                                   //if the location is correct it is save in the vector of location of each word
                                   if(position != make_pair(-1, -1)){
                                          //here we check if it is the first time that a word is found if it is true it will
                                          //enter the first if, if the word it has been found already the program will check
                                          //if the x and y of the word already found are smaller than the new one found
                                          //if they are the value x and y remain the same but if the x and y found in another
                                          //location are smaller than the one previosly found they will be change to the new one
                                          if(final[c] == make_pair(-1, -1)){
                                                 final[c] = position;
                                          }else if(position.first < final[c].first || (position.first == final[c].first && position.second < final[c].second)){
                                                 final[c] = position;
                                          }
                                          
                                   } 
                            }
                            c++;
                     }while(c < wordcount);  
              }
       }
       return final;
}

//moveLetter finds if the location x and y found is a valid position by checking all the other letter
pair<int, int> CircleTheWordSolver::moveLetter(vector<vector<char> > &puzzle, string temp, int x, int y){
       int l = temp.length();
       int posx, posy;
       pair<int, int> initialPosition;
       //this is a bruteforce solution
       //in fact we check all 8 directions if a word is found in any directions the locations of the
       //first letter of a word gets saved and send to findLetter if not the location x and y that
       //is been sent to this function gets mark with -1, -1 that means that is not a valid location
       for(int i = 0; i < 8; i++){
              int counter = 1;
              if(i == 0){
                     for(int j = 1; j < l; j++){
                            if(y+j < puzzle.size() && temp[j] == puzzle[y+j][x]){
                                   if(j == 1){
                                          posx = x;
                                          posy = y;
                                   }
                                   counter++;
                            }else{
                                   break;
                            }
                     }
              }else if(i == 1){
                     for(int j = 1; j < l; j++){
                            if((y+j < puzzle.size() && x-j > -1) && temp[j] == puzzle[y+j][x-j]){
                                   if(j == 1){
                                          posx = x;
                                          posy = y;
                                   }
                                   counter++;
                            }else{
                                   break;
                            }
                     }
              }else if(i == 2){
                   for(int j = 1; j < l; j++){
                            if(x-j > -1 && temp[j] == puzzle[y][x-j]){
                                   if(j == 1){
                                          posx = x;
                                          posy = y;
                                   }
                                   counter++;
                            }else{
                                   break;
                            }
                     }  
              }else if(i == 3){
                     for(int j = 1; j < l; j++){
                            if((y-j > -1 && x-j > -1) && temp[j] == puzzle[y-j][x-j]){
                                   if(j == 1){
                                          posx = x;
                                          posy = y;
                                   }
                                   counter++;
                            }else{
                                   break;
                            }
                     }  
              }else if(i == 4){
                     for(int j = 1; j < l; j++){
                            if(y-j > -1 && temp[j] == puzzle[y-j][x]){
                                   if(j == 1){
                                          posx = x;
                                          posy = y;
                                   }
                                   counter++;
                            }else{
                                   break;
                            }
                     }  
              }else if(i == 5){
                     for(int j = 1; j < l; j++){
                            if((y-j > -1 && x+j < puzzle.size()) && temp[j] == puzzle[y-j][x+j]){
                                   if(j == 1){
                                          posx = x;
                                          posy = y;
                                   }
                                   counter++;
                            }else{
                                   break;
                            }
                     }
              }else if(i == 6){
                    for(int j = 1; j < l; j++){
                            if(x+j < puzzle.size() && temp[j] == puzzle[y][x+j]){
                                   if(j == 1){
                                          posx = x;
                                          posy = y;
                                   }
                                   counter++;
                            }else{
                                   break;
                            }
                     } 
              }else if(i == 7){
                     for(int j = 1; j < l; j++){
                            if((y+j < puzzle.size() && x+j < puzzle.size()) && temp[j] == puzzle[y+j][x+j]){
                                   if(j == 1){
                                          posx = x;
                                          posy = y;
                                   }
                                   counter++;
                            }else{
                                   break;
                            }
                     }
              }
              if(counter == l){
                    initialPosition.first = posx;
                    initialPosition.second = posy;
                    return initialPosition;
              }
       }
       initialPosition.first = -1;
       initialPosition.second = -1;
       return initialPosition;
}