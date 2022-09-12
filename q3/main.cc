#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cctype>
#include <string>
#include "omp.h"
#include "math.h"

using namespace std;

/**
 * Check for perfect square
 * Do other stuff
 */

int main(){
  
  #pragma omp for
  for(int x = 0; x < 100; x++){
    for(int y = 0; y < 100; y++){
      for(int z = 0; z < 100; z++){
        if (sqrt(x) == x * x){
          if (sqrt(y) == y * y){
            if(sqrt(z) == z * z){
              cout << x << " " << y << " " << z << " " << endl;
            }
          }
        }
      }
    }
  }



  return 0;
}