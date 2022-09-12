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
// Modded by AJ Stein to use OpenMPI

#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <string>
#include <iostream>
#include <iomanip>
#include <openssl/sha.h>
#include <cassert>
#include <mpi.h>
using namespace std;
using namespace boost::multiprecision;


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
// To run
// mpic++ in makefile (replace g++)
// mpiexec -np [num of processes] miner < test/coin_try2
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

// rework of the one I made in the last hw
// Derived from slide notes 13 and the 3 step
void bcast(vector<unsigned char> &v1, vector<unsigned char> &v2){
  // tmp array to bcast size
  int tmp[2]; 

  // resize to v1.size
  tmp[0] = v1.size();
  tmp[1] = v2.size();
  // bcast size 
  MPI_Bcast(tmp, 2, MPI_INT, 0, MPI_COMM_WORLD);

  // resize again
  v1.resize(tmp[0]);
  v2.resize(tmp[1]);

  // unlike last time we dont need to bcast an entire string, just a char 
  // so we dont need a loop or anything
  MPI_Bcast(&v1[0], tmp[0], MPI_CHAR, 0, MPI_COMM_WORLD);
  MPI_Bcast(&v2[0], tmp[1], MPI_CHAR, 0, MPI_COMM_WORLD);
}


int main() {
  // inits
  int rank, process_num;
  cpp_int t1, t2;

  // Required MPI inits
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &process_num); 
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);        

  // vec to be filled with t1, t2
  vector<unsigned char> v1;
  vector<unsigned char> v2;

  // we want host to only read
  if(rank == 0){  
    cin >> t1;
    cin >> t2;
    assert(t1 < t2);

    // OG num, see the output
    // cout << "this is t1 " << t1 << endl;
    // cout << "this is t2 " << t2 << endl;

    // vec of bits to bcast later, found online the export bits method
    export_bits(t1, back_inserter(v1), 8);
    export_bits(t2, back_inserter(v2), 8);

  }

  // bcast both vec so everyone got the nums
  bcast(v1, v2);

  // found online
  if(process_num != 0){
    import_bits(t1, v1.begin(), v1.end());
    import_bits(t2, v2.begin(), v2.end());
  }

  // convert back to the cpp_int we need from the bits, race condition, foud import method 
  // cpp_int nt1, nt2;
  // for(int i = 0; i < v1.size(); i++){
  //   nt1 = v1[i] + nt1 * 256;
  // }
  // for(int i = 0; i < v2.size(); i++){
  //   nt2 = v2[i] + nt2 * 256;
  // }

  // check num
  // cout << "this is t1 " << t1 << endl;
  // cout << "this is t2 " << t2 << endl;

  int n = 0, found = 0;
  cpp_int t = process_num;
  while (found == 0) {
    cpp_int next_sha = sha256(t);
    if (t1 < next_sha && next_sha < t2) {
      cout << t << endl;
      found = 1;                              
    } // end of if

    // Look to see if anyone got the answer every 100 iterations
    if(n % 1000 == 0){
      int tmp1 = found;
      MPI_Allreduce(&tmp1, &found, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    }

    // increment our vals counting
    t+=process_num;
    n++;
  } // end of while
  
  // all processes stop here
  MPI_Finalize();

  return 0;
}


