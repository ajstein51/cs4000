/**
 * CS4000 Homework 6 EC
 * AJ Stein
 * Mapper for hw6, reads through many tweets and looks for a secret msg
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
 * The mapper will take a command line message which is the secret message
 * We will a key and value with the key being username and the value being
 * how many times we have come across the msg 
 * Example: Kappa:3 so we found the msg 3 times on the username: Kappa 
 */ 
int main(int argc, char **argv){
    // the argv is the message we are tryin to figure out
    string input, msg = argv[1];

    // getting the input from the file
    while(!cin.eof()){
        getline(cin, input);

        if(!cin.fail()){
            int getpos;                                                         
            string username;
            // get user by looking for a space then substring it
            for (size_t i = 0; i < input.length(); i++){
                if(input[i] == '\t'){
                    username = input.substr(0, i - 1);                         // get the username
                    getpos = i;                                                // get the position of the username
                }
            } // end of for

            // Total will be a sum of matching letters, lookin for the secret msg matching strings
            int total = 0;
            for (size_t i = getpos; i < input.length(); i++)
                if(msg[total] == input[i] && total < msg.length())
                    total+=1;

            // if the total is the same as the secret msg than its good, double checking        
            if(total == msg.length())
                cout << username << '\t' << "1" << endl;
        } // end of if
    } // end of while
    return 0;
}