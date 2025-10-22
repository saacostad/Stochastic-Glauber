#include <iostream>
#include <cmath>
#include <vector>


// INCLUDES OF HANDMADE HEADERS 
#include "headers/densityFunctions.h"
#include "headers/geometryHeader.h"

//Main function
int main(void){
    
   using namespace std;



    for (int i = 1; i < 150; i++){

        vector nucleon = sampleNucleon(eng);

        cout << nucleon[0] << "," << nucleon[1] << "," << nucleon[2] << endl;
    }
    

    return 0;
}


