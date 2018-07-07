//
//  zoo.hpp
//  project4
//
//  Created by Philip Ruffini on 6/7/18.
//  Copyright © 2018 Philip Ruffini. All rights reserved.
//

#ifndef zoo_hpp
#define zoo_hpp

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <array>
#include <vector>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <deque>
#include <stack>
#include <iomanip>
#include <getopt.h>

using namespace std;

struct cage_A{
    double distance;
    int x;
    int y;
    int preceding_cage_number;
    char type; //n = normal, b = boarder, w = wild
    bool vistited;
};

struct results{
    int a;
    int b;
};

class Zoo_A{
public:
    
    
    //Fills the Zoo with Animal Locations
    void fill_zoo_A();
    
    void print_output();
    
    //Program to run mode MST - A
    void run_A();
    
    //Used for Testing - Prints Zoo 
    void print_zoo();
    
    //Used to calculate distance
    double calc_euclidean_distance(const cage_A& one, const cage_A& two);

    
private:
    vector<cage_A> animal_cages_A;
    vector<results> final_path;
    int number_of_cages;
    int number_visited;
    double total_distance;
    bool cage_in_wild = false;
    bool cage_on_border = false;
};


#endif /* zoo_hpp */
