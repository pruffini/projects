//  project4.cpp
//  project4
//
//  Created by Philip Ruffini on 6/3/18.
//  Copyright © 2018 Philip Ruffini. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <algorithm>
#include <deque>
#include <stack>
#include <iomanip>
#include <getopt.h>
#include "zoo_A.h"
#include "zoo_B.h"
#include "zoo_C.h"

using namespace std;

int main(int argc, char **argv) {
    
    std::ios_base::sync_with_stdio(false);
    
    opterr = false;
    option long_options[] = {
        {"mode", required_argument, nullptr, 'm'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, 0}
    };
    
    int option_index = 0;
    int choice = 0;
    
    string mode;
    
    while ((choice = getopt_long(argc, argv, "m:h", long_options, &option_index)) != -1){
        switch (choice){
                
            case 'm':
                mode = optarg;
                if(mode != "MST" && mode != "OPTTSP" && mode != "FASTTSP"){
                    cerr << "No Valid Mode Selected" << endl;
                    exit(1);
                }
                break;
                
            case 'h':
                cout << "This Program is Used To Find the Shortest Path in the Zoo" << endl;
                cout << "Please specify the Mode: MST, OPTTSP, or FASTTSP." << endl;
                exit(1);
                break;
                
            default:
                cerr << "Unrecognized option" << "\n";;
                exit (1);
        }
    }
    
    if(mode == "MST"){
        Zoo_A zoo;
        zoo.fill_zoo_A();
        //zoo.print_zoo();
        zoo.run_A();
        zoo.print_output();
    }
    else if(mode == "FASTTSP"){
        Zoo_B zoo;
        zoo.fill_zoo_B();
        //zoo.check_distance();
        zoo.run_B();
        zoo.print_output();
    }
    else{
        Zoo_C zoo;
        zoo.fill_zoo_C();
        //zoo.check_dis();
        zoo.create_distance_matrix();
        zoo.run_C();
        zoo.optimize_c();
        zoo.print_output();
    }
    
    //Used for Testing
    //zoo.print_zoo();
    
    
}
