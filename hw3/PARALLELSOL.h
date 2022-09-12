//
// Example Stub for Class file for CS 4000, HW #3
//
// To compile: g++ GOL_display.cc -lncurses
// ./a.out < [test file]
// My standard copy and paste
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cctype>
#include <string>

#include <thread>
#include <future>
#include <mutex>
using namespace std;

#ifndef CS4000_GAME_OF_LIFE
#define CS4000_GAME_OF_LIFE

class GameOfLife {
public:
    vector<vector<int> > SimulateLife(vector<vector<int> > &board, int life_cycles);    // Driver
    void checklife(vector<vector<int> > &board, int start, int end, vector<vector<int> > &sol);
    int checkdir(vector<vector<int> > &board, int i, int j);                           // Check the 8 directions
};
#endif


vector<vector<int> > GameOfLife::SimulateLife(vector<vector<int> > &board, int life_cycles){
    GameOfLife g;                                               // Needed for threads
    
    vector<vector<int> > solution = board;                      // Maybe not needed
    vector<vector<int> > next = board;                          // Get the next board
    vector<vector<int> > something;                          // Get the next board
    int num1 = 0, num2 = 0, num3 = 0, bsize = board.size();
    num1 = bsize / 4, num2 = bsize / 2, num3 = bsize - num1;   // getting the start and end for threads

    
    for(int i = 0; i < life_cycles; i++){
        
        // always start at 0 and end at board.size()
        //thread t1(&GameOfLife::checklife, &g, ref(solution), 0, 20, ref(sol1));
        thread t1(&GameOfLife::checklife, &g, ref(solution), 0, num1, ref(next));
        thread t2(&GameOfLife::checklife, &g, ref(solution), num1, num2, ref(next));
        thread t3(&GameOfLife::checklife, &g, ref(solution), num2, num3, ref(next));
        thread t4(&GameOfLife::checklife, &g, ref(solution), num3, bsize, ref(next));
        t1.join();
        t2.join();
        t3.join();
        t4.join();
        // next = solution;
        // solution = next;
        something = next;
        solution = something;

    }
    return solution;
} // end func

/**
 * Note of Rules: 
 * Cell with 4+ neighbors are alive, the cell will die                                        4+ = Die
 * Cell with exactly 3 neighbors alive, a cell is born                                        3 = Born
 * Cell with 2 or 3 neighbors alive, the cell will survive (if it was alive)                  2 or 3 = Live
 * Cell with 0 or 1 neighbors alive, the cell will die                                        1 or 2 = Die
 */ 
void GameOfLife::checklife(vector<vector<int> > &board, int start, int end, vector<vector<int> > &sol){
    for(int i = start; i < end; i++){                                                          // row
        for(size_t j = 0; j < board.size(); j++){                                                 // col, changed from start to end
            int numoflife = checkdir(board, i, j);                                                // num of lives

            // retuns the 2 dimensional gride (1 or 2 -- alive, 0 == dead) after life_Cycles (board size) generations
            // A value of 2 means the cell is immortal 
            if(board[i][j] == 1 && numoflife >= 4)                                                // Die 
                sol[i][j] = 0;
            else if(board[i][j] == 0 && numoflife == 3)                                           // Born
                sol[i][j] = 1;
            else if((board[i][j] == 1 || board[i][j] == 2) && (numoflife == 2 || numoflife == 3)) // Remain the same
                sol[i][j] = board[i][j];
            else if(board[i][j] == 1 && (numoflife == 0 || numoflife == 1))                       // die
                sol[i][j] = 0;
        } // end of j
    } // end of i
} // end func

/**
 * Check the 8 given neighbors
 * 0: (i + 1( mod )n, j), 1: (i − 1) mod n, j), 2: (i − 1) mod n, j + 1 mod n), 3: (i − 1) mod n, j − 1 mod n), 
 * 4: (i + 1) mod n, j + 1 mod n), 5: (i + 1) mod n, j − 1 mod n), 6: (i, j + 1 mod n), 7: (i, j − 1 mod n)
 * This function shall return how many pieces are alive on the board
 * This is done by the given 8 directional equations listed above
 */ 
int GameOfLife::checkdir(vector<vector<int> > &board, int i, int j){
    int row = (board.size() + i), col = (board.size() + j), bsize = board.size(), lifecount = 0;
    
    if(board[((row + 1) % bsize)][j] == 1 || board[((row + 1) % bsize)][j] == 2)
        lifecount++;
    //***********************************************************************************************
    if(board[((row - 1) % bsize)][j] == 1 || board[((row - 1) % bsize)][j] == 2) 
        lifecount++;
    //***********************************************************************************************
    if(board[((row - 1) % bsize)][((col + 1) % bsize)] == 1 || board[((row - 1) % bsize)][((col + 1) % bsize)] == 2) 
        lifecount++;
    //***********************************************************************************************
    if(board[((row - 1) % bsize)][((col - 1) % bsize)] == 1 || board[((row - 1) % bsize)][((col - 1) % bsize)] == 2) 
        lifecount++;
    //***********************************************************************************************
    if(board[((row + 1) % bsize)][((col + 1) % bsize)] == 1 || board[((row + 1) % bsize)][((col + 1) % bsize)] == 2)
        lifecount++;
    //***********************************************************************************************
    if(board[((row + 1) % bsize)][((col - 1) % bsize)] == 1 || board[((row + 1) % bsize)][((col - 1) % bsize)] == 2) 
        lifecount++;
    //***********************************************************************************************
    if(board[i][((col + 1) % bsize)] == 1 || board[i][((col + 1) % bsize)] == 2)
        lifecount++;
    //***********************************************************************************************
    if(board[i][((col - 1) % bsize)] == 1 || board[i][((col - 1) % bsize)] == 2)
        lifecount++;
    //***********************************************************************************************
    return lifecount;          // Return the amount of lives 
} // end func

