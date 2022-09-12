//
// A simple CS Coin miner program for CS 4000, Homework #5
//
// The program reads in two big integers l and r, and stores them in the
// boost data structure cpp_int.
//
// This CS Coin miner starts at 0 and iteratively tries all integers until
// it finds an integer z such that l < shar256(z) < r.
// It then outputs z and quits.
//
// Written by David W. Juedes, April 9th, 2021.
//
// Modded by AJ Stein to use OpenMP.

#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <string>
#include <iostream>
#include <iomanip>
#include <openssl/sha.h>
#include <cassert>

#include "omp.h"
using namespace std;
using namespace boost::multiprecision;

// to compiler with openmp do g++ -fopenmp *.cc
// make miner
// ./miner < test/coin_try14 > coin14
// cat test/coin_try14 coin14 | ./verify


//
// Function: to_bits
// Converts a cpp_int x into a vector of unsigned characters (8-bit ints).
// 
//
vector<unsigned char> to_bits(cpp_int x) {
  unsigned char c;

  vector<unsigned char> buffer;
  while (x>0) {
    c = (unsigned char) x%256;
    buffer.push_back(c);
    x=x/256;
  }

  return buffer;
}

//
// Function sha256(x)
// Applies the standard sha256 algorithm from openssl to a cpp_int.
// Returns the corresponding cpp_int.
// Note: SHA256_DIGEST_LENGTH is 32.
// (8*32 = 256).
//
cpp_int sha256(const cpp_int x)
{
  vector<unsigned char> int_buffer;
  int_buffer = to_bits(x);
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256(&int_buffer[0],int_buffer.size(),hash);
  cpp_int total = 0;  
  for (int i=0;i<SHA256_DIGEST_LENGTH;i++) {
    total = total*256+(int) hash[i];
  }
  
  return total;
}
int main() {

  cpp_int t1;
  cpp_int t2;

  cin >> t1;
  cin >> t2;
  assert(t1 < t2);

  bool found = false;                               // if a thread found an answer
  #pragma omp parallel shared(found) num_threads(4) // Parallel this block of code with a shared 'found' so each know when one found one
  {            
    cpp_int t = omp_get_thread_num();               // get the thread            
    while (!found) {
      cpp_int next_sha = sha256(t);                 // changed this to be declared inside the while loop so each thread makes their own
      if (t1 < next_sha && next_sha < t2) {
        cout << t << endl;
        found =true;                               // we could also just exit(0) does the same, then we wouldnt have to deal with shared
      } // end of if
      t+=omp_get_num_threads();                    // increase thread
    } // end of while
  } // end of pragma

  return 0;
} // end of main


