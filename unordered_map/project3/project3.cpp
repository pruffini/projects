
//  main.cpp
//  project3
//
//  Created by Philip Ruffini on 5/28/18.
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
#include "log.h"

using namespace std;

int main(int argc, char **argv) {
    
    if(argc == 0){
        cout <<
        "Invalid Command Line Input" << "\n";;
        exit(1);
    }
    
    std::ios_base::sync_with_stdio(false);
    if(*argv[1] == '-'){
        cout <<
        "This Program is used to Search through the Developers Log Book for Errors" << "\n";;
        exit(1);
    }
    
    ifstream read_in(argv[1]);
    //ifstream read_in;
    //read_in.open("sample-log.txt");
    
    
    //Checks if there will be an error opening the file
    if (!read_in.good()) {
        cout << "Error opening file" << "\n";
        exit(1);
    }
    
    Log database;
    database.load_database(read_in);
    //database.print_database();
    database.run_program();
    //database.print_excerptlist();
    
    
    return 0;
}








