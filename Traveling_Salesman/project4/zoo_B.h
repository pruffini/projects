//
//  zoo_B.hpp
//  project4
//
//  Created by Philip Ruffini on 6/8/18.
//  Copyright © 2018 Philip Ruffini. All rights reserved.
//

#ifndef zoo_B_hpp
#define zoo_B_hpp

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

using namespace std;


struct cage_B{
    int x;
    int y;
    bool vistited;
};

class Zoo_B{
public:
    
    //Fills the Zoo with Animal Locations
    void fill_zoo_B();
    
    //Program to run mode B
    void run_B();
    
    void print_output();
    
    //Used to calculate distance
    double calc_euclidean_distance(const cage_B& one, const cage_B& two);
    
    //used for testing
    void check_distance();
    
    
    
private:
    vector<cage_B> animal_cages_B;
    vector<int> final_path;
    int number_of_cages;
    double total_distance;
    double saved_distance;
    double distance_ik;
    double distance_kj;
    double distance_ij;

};

#endif /* zoo_B_hpp */
