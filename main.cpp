#include <iostream>
#include <cmath>
#include <vector>


// INCLUDES OF HANDMADE HEADERS 
#include "headers/densityFunctions.h"
#include "headers/geometryHeader.h"


int main(void){
    
   using namespace std;



    for (int i = 1; i < 100000; i++){
        cout << sample_from_pdf(eng, 1.0, 1.0, 5.0, 0.1) << endl;
    }
    

    return 0;
}


