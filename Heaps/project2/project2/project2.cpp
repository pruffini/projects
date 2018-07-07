//
//  main.cpp
//  project2
//
//  Created by Philip Ruffini on 5/9/18.
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
#include "mine.h"

using namespace std;

int main(int argc, char **argv) {
    
    std::ios_base::sync_with_stdio(false);
    
    opterr = false;
    option long_options[] = {
        {"stats", required_argument, nullptr, 's'},
        {"median", no_argument, nullptr, 'm'},
        {"verbose", no_argument, nullptr, 'v'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, 0}
    };
    
    bool stats_set = false;
    bool median_set = false;
    bool verbose = false;
    int statistics_N = 0;
    
    int option_index = 0;
    int choice = 0;

    while ((choice = getopt_long(argc, argv, "s:mvh", long_options, &option_index)) != -1){
        switch (choice){
                
            case 's':
                statistics_N = atoi(optarg);
                stats_set = true;
                break;
                
            case 'm':
                median_set = true;
                break;
                
            case 'v':
                verbose  = true;
                break;
                
            case 'h':
                cout <<
                "--stats (W|M) or -s (W|M) Prints extra summarization statistics \n"
                "--median or -m Prints median difficulty of clearning a rubble tile \n"
                "--verbose or -v Prints out every Rubble Value as a tile is being cleared \n"
                "--help or -h prints this input specification \n";
                exit(1);
                break;
                
            default:
                cerr << "Unrecognized option" << "\n";;
                exit (1);
        }
    }
    
    Mine mine(verbose, stats_set, median_set);
    mine.create_mine();
    if(stats_set){
        mine.get_stats_N(statistics_N);
    }
    mine.clear_start();
    mine.path();
    
    mine.generic_output();
    if(stats_set){
       mine.stats_output();
    }
    
    return 0;
}
