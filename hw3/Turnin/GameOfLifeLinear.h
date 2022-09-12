//
// Example Stub for Class file for CS 4000, HW #3
//
// AJ Stein, GameOfLifeParallel.h, CS4000
// This file implements a Conways Game of Life algo in parallel using threads
// To compile: g++ -Wall -g GOL_display.cc -lncurses 
// To exec: ./a.out < [test file]
// My standard copy and paste
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cctype>
#include <string>
using namespace std;

#ifndef CS4000_GAME_OF_LIFE
#define CS4000_GAME_OF_LIFE

class GameOfLife {
public:
    vector<vector<int> > SimulateLife(vector<vector<int> > &board, int life_cycles);    // Driver
    vector<vector<int> > checklife(vector<vector<int> > &board);                        // Check the type of cell
    int checkdir(vector<vector<int> > &board, int i, int j);                            // Check the 8 directions
};
#endif

vector<vector<int> > GameOfLife::SimulateLife(vector<vector<int> > &board, int life_cycles){
    vector<vector<int> > Cur = board;                                             // Current board
    vector<vector<int> > next;                                                    // Next Board
    for(int i = 0; i < life_cycles; i++){
        next = checklife(Cur);
        Cur = next;                                                               // swap current with next
    }
    return Cur;
} // end func

/**
 * Note of Rules: 
 * Cell with 4+ neighbors are alive, the cell will die                                        4+ = Die
 * Cell with exactly 3 neighbors alive, a cell is born                                        3 = Born
 * Cell with 2 or 3 neighbors alive, the cell will survive (if it was alive)                  2 or 3 = Live
 * Cell with 0 or 1 neighbors alive, the cell will die                                        1 or 2 = Die
 */ 
vector<vector<int> > GameOfLife::checklife(vector<vector<int> > &board){
    vector<vector<int> > solution = board;
    for(size_t i = 0; i < board.size(); i++){                                                     // row
        for(size_t j = 0; j < board.size(); j++){                                                 // col
            int numoflife = checkdir(board, i, j);                                                // num of lives

            // retuns the 2 dimensional gride (1 or 2 -- alive, 0 == dead) after life_Cycles (board size) generations
            // A value of 2 means the cell is immortal 
            if(board[i][j] == 1 && numoflife >= 4)                                                // Die 
                solution[i][j] = 0;
            else if(board[i][j] == 0 && numoflife == 3)                                           // Born
                solution[i][j] = 1;
            else if((board[i][j] == 1 || board[i][j] == 2) && (numoflife == 2 || numoflife == 3)) // Remain the same
                solution[i][j] = board[i][j];
            else if(board[i][j] == 1 && (numoflife == 0 || numoflife == 1))                       // die
                solution[i][j] = 0;
                
        } // end of j
    } // end of i
    return solution;                                                                              // return next board
} // end func

/**
 * Check the 8 given neighbors
 * 0: (i + 1( mod )n, j), 1: (i − 1) mod n, j), 2: (i − 1) mod n, j + 1 mod n), 3: (i − 1) mod n, j − 1 mod n), 
 * 4: (i + 1) mod n, j + 1 mod n), 5: (i + 1) mod n, j − 1 mod n), 6: (i, j + 1 mod n), 7: (i, j − 1 mod n)
 * This function shall return how many pieces are alive on the board
 * This is done by the given 8 directional equations listed above
 */ 
int GameOfLife::checkdir(vector<vector<int> > &board, int i, int j){
    int lifestatus = 0, count = 0, row = (board.size() + i), col = (board.size() + j), bsize = board.size();
    // Note for future: I may not even need this while or switch
    while(count <= 7){
        switch(count){
            case 0:
                if(board[((row + 1) % bsize)][j] == 1 || board[((row + 1) % bsize)][j] == 2)
                    lifestatus++;
                break;
    //***********************************************************************************************
            case 1:
                if(board[((row - 1) % bsize)][j] == 1 || board[((row - 1) % bsize)][j] == 2) 
                    lifestatus++;
                break;
    //***********************************************************************************************
            case 2:
                if(board[((row - 1) % bsize)][((col + 1) % bsize)] == 1 || board[((row - 1) % bsize)][((col + 1) % bsize)] == 2) 
                    lifestatus++;
                break;
    //***********************************************************************************************
            case 3:
                if(board[((row - 1) % bsize)][((col - 1) % bsize)] == 1 || board[((row - 1) % bsize)][((col - 1) % bsize)] == 2) 
                    lifestatus++;
                break;
    //***********************************************************************************************
            case 4:
                if(board[((row + 1) % bsize)][((col + 1) % bsize)] == 1 || board[((row + 1) % bsize)][((col + 1) % bsize)] == 2)
                    lifestatus++;
                break;
    //***********************************************************************************************
            case 5:
                if(board[((row + 1) % bsize)][((col - 1) % bsize)] == 1 || board[((row + 1) % bsize)][((col - 1) % bsize)] == 2) 
                    lifestatus++;
                break;
    //***********************************************************************************************
            case 6:
                if(board[i][((col + 1) % bsize)] == 1 || board[i][((col + 1) % bsize)] == 2)
                    lifestatus++;
                break;
    //***********************************************************************************************
            case 7:
                if(board[i][((col - 1) % bsize)] == 1 || board[i][((col - 1) % bsize)] == 2)
                    lifestatus++;
                break;
    //***********************************************************************************************
            default:
                break;
        }
        count++;                // Increment count
    } // end of while

    return lifestatus;          // Return the amount of lives 
} // end func
