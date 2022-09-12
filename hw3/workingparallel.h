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
    vector<vector<int> > solution = board;
    vector<vector<int> > bstatus;
    vector<vector<int> > sol1;



    int maxth = thread::hardware_concurrency();
    
    GameOfLife g;
    for(int i = 0; i < life_cycles; i++){
        // auto f = async(launch::async,&GameOfLife::checklife,&g,ref(solution));
        // bstatus = f.get();
            //future<vector<vector<int> > > a1 = async(launch::async|launch::deferred, &GameOfLife::checklife, &g, ref(solution),0,20);
            //future<vector<vector<int> > > a2 = async(launch::async|launch::deferred, &GameOfLife::checklife, &g, ref(bstatus), 10, 20);
            // //bstatus = checklife(solution);
            
            //sol1 = a1.get();
            //sol = a2.get();


            thread t1(&GameOfLife::checklife, &g, ref(solution), 0, 10, ref(sol1));
            thread t2(&GameOfLife::checklife, &g, ref(solution), 10, 20, ref(sol1));
            //thread t2(&GameOfLife::checklife, &g, ref(solution), 10, 19, ref(bstatus));

            t1.join();
            t2.join();
            //t2.join();
            bstatus = sol1;
            solution = bstatus;
            

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
    vector<vector<int> > solution = board;
    //GameOfLife g;
    
    for(int i = start; i < end; i++){                                                     // row
        for(int j = start; j < end; j++){                                                 // col
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
    //return solution;
    sol = solution;
} // end func


/**
 * Check the 8 given neighbors
 * 0: (i + 1( mod )n, j), 1: (i − 1) mod n, j), 2: (i − 1) mod n, j + 1 mod n), 3: (i − 1) mod n, j − 1 mod n), 
 * 4: (i + 1) mod n, j + 1 mod n), 5: (i + 1) mod n, j − 1 mod n), 6: (i, j + 1 mod n), 7: (i, j − 1 mod n)
 * This function shall return how many pieces are alive on the board
 * This is done by the given 8 directional equations listed above
 */ 
int GameOfLife::checkdir(vector<vector<int> > &board, int i, int j){
    int row = (board.size() + i), col = (board.size() + j), bsize = board.size(), sol = 0;
    
    if(board[((row + 1) % bsize)][j] == 1 || board[((row + 1) % bsize)][j] == 2)
        sol++;
    //***********************************************************************************************
    if(board[((row - 1) % bsize)][j] == 1 || board[((row - 1) % bsize)][j] == 2) 
        sol++;
    //***********************************************************************************************
    if(board[((row - 1) % bsize)][((col + 1) % bsize)] == 1 || board[((row - 1) % bsize)][((col + 1) % bsize)] == 2) 
        sol++;
    //***********************************************************************************************
    if(board[((row - 1) % bsize)][((col - 1) % bsize)] == 1 || board[((row - 1) % bsize)][((col - 1) % bsize)] == 2) 
        sol++;
    //***********************************************************************************************
    if(board[((row + 1) % bsize)][((col + 1) % bsize)] == 1 || board[((row + 1) % bsize)][((col + 1) % bsize)] == 2)
        sol++;
    //***********************************************************************************************
    if(board[((row + 1) % bsize)][((col - 1) % bsize)] == 1 || board[((row + 1) % bsize)][((col - 1) % bsize)] == 2) 
        sol++;
    //***********************************************************************************************
    if(board[i][((col + 1) % bsize)] == 1 || board[i][((col + 1) % bsize)] == 2)
        sol++;
    //***********************************************************************************************
    if(board[i][((col - 1) % bsize)] == 1 || board[i][((col - 1) % bsize)] == 2)
        sol++;
    //***********************************************************************************************
    return sol;          // Return the amount of lives 
} // end func


/**
 * to try: split the above function into 2's then call each individually
 */