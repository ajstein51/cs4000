#include <iostream>
#include <future>
//#include "omp.h"
using namespace std;

// Answer i got from base code: 2147016673
int poly(int x) {
  return x * x * x - x * x + 10 * x - 7;
}

int max_over_range(int a, int b) {
  int max_val = 0;
  
//#pragma omp parallel for reduction(max:max_val)
  for (int i = a; i <= b; i++) {
    int c = poly(i);
    if (c > max_val) {
      max_val = c;
    }
  }

  return max_val;

}

int main() {
    future<int> answer1 = async(max_over_range,-100,10000);

    int printout = answer1.get();
    cout << printout << endl;
}