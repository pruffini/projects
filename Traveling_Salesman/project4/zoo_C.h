//
//  zoo_C.hpp
//  project4
//
//  Created by Philip Ruffini on 6/8/18.
//  Copyright © 2018 Philip Ruffini. All rights reserved.
//

#ifndef zoo_C_hpp
#define zoo_C_hpp

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

struct better{
    double distance = numeric_limits<double>::infinity();
    int x;
    int y;
    int index;
    bool vistited = false;
};

struct cage_C{
    double distance = numeric_limits<double>::infinity();
    int index;
    bool vistited = false;
};

class Zoo_C{
public:
    
    //Fills the Zoo with Animal Locations
    void fill_zoo_C();
    
    //Program to run mode B
    void run_C();
    
    void print_output();
    
    //Used to calculate distance
    double calc_euclidean_distance(const better& one, const better& two);
    
    void check_dis();
    
    void create_distance_matrix();
    
    void optimize_c();
    
    void gen_perms();
    
    bool promising();
    
    double calculate_distance();
    
    void print_distance_matrix();
    
private:
    vector<better> animal_cages_C;
    vector<int> TSP_UP_path;
    vector<int> output_path;
    vector<int> opt_path;
    int number_of_cages;
    int permLength = 1;
    vector<vector<double>> distance_matrix;
    //double new_bound;
    double upper_bound;
    double saved_distance;
    double current_distance;
    double distance_ik;
    double distance_kj;
    double distance_ij;
};

#endif /* zoo_C_hpp */
