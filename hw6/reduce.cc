/**
 * CS4000 Homework 6 EC
 * AJ Stein
 * Reducer for hw6
 */

// Library, I def don't need all of these its just my standard copy and paste
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cctype>
#include <string>
#include <map>
#include <sstream>

using namespace std;

/**
 * Reads in key and values from the mapper
 * Then writes key/value pairs to the std out (cout)
 * We assume this will always be in sorted order
 * Output example: AJ 55 aj being the user and 55 being the total
 */
int main(int argc, char **argv){
    // getting the input from the pipe
    string input, next_name;                  // current username and next username 
    int counter = 0;                        // adding how many times we see the user name
    bool checker = false;
    while (!cin.eof()){
        getline(cin, input);

        if (!cin.fail()){
            string username;
            // get user by looking for a space then substring it
            for (size_t i = 0; i < input.length(); i++){
                if (input[i] == '\t'){
                    username = input.substr(0, i - 1); // get the username
                }
            } // end of for
            // get the user at the start, otherwise outputs scuffed
            if (checker == false){
                next_name = username;
                checker = true;
            }

            // increase in user and check if its the same we already got
            // otherwise reset
            if (next_name == username)
                counter++;
            else{
                cout << next_name << '\t' << counter << endl;
                counter = 1;
            }

            // move onto the next username
            next_name = username;

        } // end of if cin.fail
    } // end of
    cout << next_name << '\t' << counter << endl;

    return 0;
}