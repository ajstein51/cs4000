/**
 * AJ Stein
 * CS4100
 * 2/9/2021
 * Part 2, modify this program using OpenMP to count the number of
 * primes between a and b. You are required to the use the OpenMP reduction clause
 * somewhere in your program for this assignment
 * Put somewhere how the solution is computed
 */
#include <iostream>
#include "omp.h"

using namespace std;

//
// Check to see if a number is prime.
//
bool is_prime(long long x) {
  if (x<2) return false;
  long long t=2;
  while (t*t <= x) {
    if (x%t == 0) {
      return false;
    }
    t++;
  }
  return true;
}

// Count the number of primes between a and b, inclusive
/**
 * doing pragma omp parallel for turns the for loop into a parallel for loop. 
 * Which means threads each do a partial portion of the for loop
 * The (+:sum) says that we're reducing the input by summing into the variable 
 * so after the partial loops are done the resulting sum is put into the variable
 * 
 */ 
long long count_primes(long long a, long long b) {  
  long long sum=0;
  #pragma omp parallel for reduction(+:sum) 
  for (long long i = a;i<=b;i++) {
    if (is_prime(i)) sum++;
  }

  return sum;

}

int main() {
  long long a;
  long long b;

  cout << "Enter two large integers " << endl;
  cin >> a;
  cin >> b;

  cout << a << " Start " << endl;
  cout << b << " End " << endl;
  
  long long total = count_primes(a,b);

  cout << "Number of prime numbers between " << a << " and " << b << " = " << total << endl;
}
