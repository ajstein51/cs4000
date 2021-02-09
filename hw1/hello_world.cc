/**
 * AJ Stein
 * CS4100
 * 2/9/2021
 * Part 1, objective is to fix the given program
 *
 */
#include<iostream>
#include "omp.h"

using namespace std;


void Hello() {
  #pragma omp parallel // changed from #pragma omp critical
  {
    cout << "Greetings from Thread " << omp_get_thread_num() << endl;
    cout << "There are " << omp_get_num_threads() << " threads " << endl;
  }
}
int main() {
  #pragma parallel
  Hello();
}
