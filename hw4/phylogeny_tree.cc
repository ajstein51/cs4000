// A more memory efficient version of phylogeny_tree.cc
// Early version consumed 85% of RAM on a 16GB machine.
//
//
// A Basic C++ implementation of
// a program that computes a Phylogenetic tree from COVID-19 Virus sequences
// Each virus sequence has about 30K base pairs.
// Computing the LCS takes about 7 seconds.
//
// Basic algorithm to build the phylogeny:
// Compute all LCS (longest common subsequence) for all pairs of
// strings.
// Pick the best pair.
// Merge the best into a new string that is the LCS of those two.
// Continue until only one sequence remains.
//
// Written by David W. Juedes
//
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

#include <mpi.h>
/** Reference for later
 * May need to download sudo apt install lam4-dev
 * To compile: mpic++ [filename].cc
 * To run: mpiexec -np [num of processes] ./a.out [filename].dat
 */
using namespace std;


// This code checks whether 
// x[i..n-1] a sequence of y[j..m-1], where n is the length of
// x, m is the length of y.
// This is solely used to check the correctness of the solution.

bool Is_subsequence(int i, int j, string &x, string &y) {
  if (i>=x.length()) return true; // Consumed all of x's characters.
  if (j>=y.length()) return false; // Can't consume y's first.

  if (x[i]==y[j]) {
    return Is_subsequence(i+1,j+1,x,y);
  } else {
    return Is_subsequence(i,j+1,x,y);
  }
}


int max3(int x,int y, int z) {
  return max(max(x,y),z);
}

int max3_loc(int x,int y, int z) {
  if (max3(x,y,z) == x) return 1;
  if (max3(x,y,z) == y) return 2;
  if (max3(x,y,z) == z) return 3;
  return -1;
}

int max_loc(int x,int y) {
  if (max(x,y) == x) return 1;
  else {return 2;}
}

//
// Recursively construct the longest common subsequence from the dynamic
// programming table "from"
//
string rec_string(string &x1,string &y1,vector<vector<int> > &LCS,int m,int n) {
  //  pair<int,pair<int,int> > t;
  if (n==0) return "";
  if (m==0) return "";
  int a = LCS[m-1][n-1];
  int b = LCS[m-1][n];
  int c = LCS[m][n-1];
  // Remember, we are off by 1.
  if (x1[m-1] == y1[n-1]) {
    if (((a+1) >=b) && ((a+1) >=c)) {
      return rec_string(x1,y1,LCS,m-1,n-1)+x1[m-1];
    }
    else {
      if (b>=c) {
	return rec_string(x1,y1,LCS,m-1,n);
      } else {
	return rec_string(x1,y1,LCS,m,n-1);
      }
    }
  } else {
          if (b>=c) {
	    return rec_string(x1,y1,LCS,m-1,n);
	  } else {
	    return rec_string(x1,y1,LCS,m,n-1);
	  }
  }
    
}

string compute_LCS(string &x1, string &y1) {
  vector<vector<int> > LCS;
  // vector<vector<pair<int, pair<int,int> > > > from;

  //from.resize(x1.length()+1);
  LCS.resize(x1.length()+1);
  for (int i=0;i<=x1.length();i++) {
    LCS[i].resize(y1.length()+1);
  }
  LCS[0][0] = 0;
  for (int i=0;i<=x1.length();i++) {
    LCS[i][0]=0;
  }
  for (int i=0;i<=y1.length();i++) {
    LCS[0][i]=0;
  }

  for (int i=1;i<=x1.length();i++) {
    for (int j=1;j<=y1.length();j++) {
      if (x1[i-1]==y1[j-1]) {
    	LCS[i][j] = max3(LCS[i-1][j-1]+1,LCS[i-1][j],LCS[i][j-1]);
    } 
    else {
	    LCS[i][j] = max(LCS[i-1][j],LCS[i][j-1]);
	} 
    }
  }
  //cout << LCS[x1.length()][y1.length()] << endl;
  string z = rec_string(x1,y1,LCS,x1.length(),y1.length());

  assert(z.length() == LCS[x1.length()][y1.length()]);
  assert(Is_subsequence(0,0,z,x1));
  assert(Is_subsequence(0,0,z,y1));

  return z;
}

// bcast, derived from slide notes and the 3 steps
void bcast(vector<string> &v1){
    // tmp array to bcast size
    int tmp[2];
    
    // resize buff to the vec size
    tmp[0] = v1.size();

    // Bcast size to all processes
    MPI_Bcast(tmp, 1, MPI_INT, 0, MPI_COMM_WORLD);;

    // everyone resize vec to the size of the read in string
    v1.resize(tmp[0]);

    // Now bcast the chars from the string
    int stringsize[0];

    // everyone gets the char from the strings
    for(int i = 0; i < tmp[0]; i++){
        stringsize[0] = v1[i].size() + 1;
        MPI_Bcast(stringsize, 1, MPI_INT, 0, MPI_COMM_WORLD);
        v1[i].resize(stringsize[0]);
        MPI_Bcast(&v1[i][0], stringsize[0], MPI_CHAR, 0, MPI_COMM_WORLD);
    }
}

/// Compute the longest common subsequence.
/**
 * To note: the file must starting with MPI_Init(&argc,&argv) and end with MPI_Finalize()
 * 
 */ 
int main() {
    // inits
    int rank, process_num;

    // Required MPI inits
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &process_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   vector<string> genomes;    
    
  if(rank == 0){
    while (!cin.eof()) {  
        string line;
        getline(cin,line);
        if (!cin.fail()) {
            genomes.push_back(line);    
        }
    }
  }
    
    // bcast to processes
    bcast(genomes);

    cout << "Number of covid genomes = " << genomes.size() << endl;

    // Make initial labels on all strings
    vector<pair<string,string> > genome_tree;
    for (int i=0;i<genomes.size();i++) {
      genome_tree.push_back(make_pair(to_string(i),genomes[i]));
    }

    

    // Merge strings
    while (genome_tree.size() >1) {
      // Debugging
      cout << "Tree size = " << genome_tree.size() << endl;

    string best;
    bool start = true;
    int max_i;
    int max_j;

      // root 
      if(rank == 0){

        for (int i=0;i<genome_tree.size();i++) {
          cout << "i = " << i << endl;
          for (int j=i+1;j<genome_tree.size();j++) {
            int buff[2];
            buff[0] = i;
            buff[1] = j;
            MPI_Recv(buff,2,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            string z;
            z=compute_LCS(genome_tree[i].second,genome_tree[j].second);
              if (start) {
                start = false;
                max_i = i;
                max_j = j;
                best = z;
            }
              if (z.length() > best.length()) {
                max_i = i;
                max_j = j;
                best = z;
              }
          }
        }
        
      }
      else{
        int buff[2];
        buff[0] = max_i, buff[1] = max_j;
        cout << "Were are here 1 " << buff[0] << " " << buff[1] << endl;

        MPI_Send(buff,2,MPI_INT,0,0,MPI_COMM_WORLD);
      }
        string new_tree_label = "("+genome_tree[max_i].first + "," + genome_tree[max_j].first +")";
        genome_tree.erase(genome_tree.begin()+max_i);
        genome_tree.erase(genome_tree.begin()+max_j-1); // max_i got deleted!
        genome_tree.push_back(make_pair(new_tree_label,best));

  } // while loop

  // output
  cout << "Phylogeny = " << endl;
  cout << genome_tree[0].first << endl;
  cout << "Root has length " << genome_tree[0].second.length() << endl;
  cout << "Best:\n" << genome_tree[0].second << endl;

  

	// needed for MPI
    MPI_Finalize();
	return 0;
}
  
  
  /** bcast
   * 1) Send size out
   * 1-A) everyone resizes their array
   * 2) send each individual string (vec of chars?)
   * 2-A) send size of string
   * 2-B) resize
   * 2-C) send the string
   */ 

  /**
   * Look at 3/29 for what to do for part b
   * lsof
   * mpic++ [file]
   * time mpiexec -np 4 a.out < covid5.dat
   * lamhalt/lamwipe to kill lam
   */ 