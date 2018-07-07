//
//  zoo_C.cpp
//  project4
//
//  Created by Philip Ruffini on 6/8/18.
//  Copyright © 2018 Philip Ruffini. All rights reserved.
//

#include "zoo_C.h"

void Zoo_C::fill_zoo_C(){
    
    //Initalizes Variables
    string line;
    better temp;
    int cages_count = 0;
    
    //Read Num Cages
    cin >> number_of_cages;
    animal_cages_C.reserve(number_of_cages);
    
    //Fill the Zoo!
    while(cages_count < number_of_cages){
        cin >> temp.x;
        cin >> temp.y;
        temp.index = cages_count;
        opt_path.push_back(cages_count);
        animal_cages_C.push_back(temp);
        ++cages_count;
    }
}

void Zoo_C::run_C(){
    int min_distance_index = 0;
    int current_index = 0;
    //size_t save_index = 0;
    animal_cages_C[current_index].vistited = true;
    TSP_UP_path.reserve(number_of_cages);
    TSP_UP_path.push_back(current_index);
    upper_bound = 0;
    double edge_distance_total = 0;
    
    //first loop to find nearest neighbor
    saved_distance = numeric_limits<double>::infinity();
    distance_ij = numeric_limits<double>::infinity();
    distance_kj = numeric_limits<double>::infinity();
    distance_ik = numeric_limits<double>::infinity();
    for(unsigned int i = 1; i < animal_cages_C.size(); ++i){
        distance_ij = calc_euclidean_distance(animal_cages_C[current_index], animal_cages_C[i]);
        if(distance_ij < saved_distance){
            saved_distance = distance_ij;
            min_distance_index = i;
        }
    }
    animal_cages_C[min_distance_index].vistited = true;
    TSP_UP_path.push_back(min_distance_index);
    saved_distance = numeric_limits<double>::infinity();
    TSP_UP_path.push_back(0);
    if(animal_cages_C[1].vistited == false){
        current_index = 1;
    }
    else{
        current_index = 2;
    }
    while(current_index != number_of_cages){
        //cout << "Current Path: "; print_output();
        if(animal_cages_C[current_index].vistited == false){
            for(unsigned int i = 1; i < TSP_UP_path.size() - 1; ++i){
                distance_ij = distance_matrix[TSP_UP_path[i - 1]][TSP_UP_path[i]];
                distance_ik = distance_matrix[current_index][TSP_UP_path[i - 1]];
                distance_kj = distance_matrix[current_index][TSP_UP_path[i]];
                edge_distance_total = (distance_ik + distance_kj - distance_ij);
                if(edge_distance_total < saved_distance){
                    min_distance_index = i;
                    saved_distance = edge_distance_total;
                }
            }
            auto it = TSP_UP_path.begin() + int(min_distance_index);
            TSP_UP_path.insert(it, current_index);
            saved_distance = numeric_limits<double>::infinity();
        }
        ++current_index;
    }
    
    
    unsigned int t = 0;
    while(t < TSP_UP_path.size() - 1){
        upper_bound += distance_matrix[animal_cages_C[TSP_UP_path[t]].index][animal_cages_C[TSP_UP_path[t + 1]].index];
        ++t;
    }
    upper_bound += calc_euclidean_distance(animal_cages_C[TSP_UP_path[0]], animal_cages_C[TSP_UP_path.back()]);
    output_path = TSP_UP_path;
}

void Zoo_C::create_distance_matrix(){
    
    distance_matrix.resize(animal_cages_C.size(), vector<double> (animal_cages_C.size()));
    for(unsigned int i = 0; i < animal_cages_C.size(); ++i){
        for(unsigned int k = 0; k < animal_cages_C.size(); ++k){
            if(i != k){
                distance_matrix[i][k] = calc_euclidean_distance(animal_cages_C[i], animal_cages_C[k]);
            }
            else{
                distance_matrix[i][k] = numeric_limits<double>::infinity();
            }
        }
    }
}

void Zoo_C::print_distance_matrix(){
    cout << std::setprecision(2);
    cout << std::fixed;
    cout << "Distance Matrix" << endl;
    cout << "    ";
    for(unsigned int i = 0; i < animal_cages_C.size(); ++i){
        cout << animal_cages_C[i].index << "   ";
    }
    cout << endl;
    for(unsigned int i = 0; i < animal_cages_C.size(); ++i){
        cout << animal_cages_C[i].index << " ";
        for(unsigned int k = 0; k < animal_cages_C.size(); ++k){
            cout << distance_matrix[i][k] << " ";
        }
        cout << endl;
    }
}
//Path represents all of the verticies
//Permlength is how many of those verticies are fixed in place
//Do all of the work on one path
//Start with 0 so perm length starts at 1
//0 needs to be fixed and cannot move
//1 swaps with 1
//0 and 1 are fixed, 2 and 3 permuted
//make every permutation that starts with 0 and 1
//then try 0 and 2 make every permutation

//Add one length before recursive call
//Subtract one length after recursive call  `
void Zoo_C::gen_perms(){
    if(int(opt_path.size()) == permLength){
        current_distance += distance_matrix[opt_path[0]][opt_path.back()];
        if(upper_bound > current_distance){
            output_path = opt_path;
            upper_bound = current_distance;
        }
        current_distance -= distance_matrix[opt_path[0]][opt_path.back()];
        return;
    }
    if(!promising()){
        return;
    }
    for(unsigned int i = permLength; i < opt_path.size(); ++i){
        swap(opt_path[permLength], opt_path[i]);
        current_distance += distance_matrix[opt_path[permLength]][opt_path[permLength-1]];
        ++permLength;
        gen_perms();
        --permLength;
        current_distance -= distance_matrix[opt_path[permLength]][opt_path[permLength-1]];
        swap(opt_path[permLength], opt_path[i]);
    }
}
//Calculate current path
//Calculate MST
//Calculate conection
//Add them together and check
bool Zoo_C::promising(){
    //Calculate distance of current path from current distance
    double temp_distance = current_distance;
    //current_distance = calculate_distance();
    //Check if current path is already too long
    if(temp_distance > upper_bound){
        return false;
    }
    //Return true if there is only a few left - like 6
    if((opt_path.size() - permLength) < 6){
        return true;
    }
    
    ///ERROR IS SOMEWHERE BELOW HERE////
    
    //Create a Deque of the vertices that aren't already locked into the path and fill it
    vector<cage_C> temp_of_unused;
    double distance_between = 0;
    double minimum = numeric_limits<double>::infinity();
    double minimum_two = numeric_limits<double>::infinity();
    double d_b_2 = numeric_limits<double>::infinity();
    temp_of_unused.reserve(opt_path.size() - permLength);
    
    //Create a Vector of the Vertices that aren't stuck in place
    //While doing that check to find the two closes points that will be apart of the MST to the path
    for(unsigned int i = permLength; i < opt_path.size(); ++i){
        cage_C temp;
        temp.index = opt_path[i];
        temp_of_unused.push_back(temp);
        distance_between = distance_matrix[opt_path[permLength - 1]][opt_path[i]];
        d_b_2 = distance_matrix[opt_path.front()][opt_path[i]];
        if(distance_between < minimum){
            minimum = distance_between;
            //edge1 = temp_of_unused[i].index;
        }
        if(d_b_2 < minimum_two){
            minimum_two = d_b_2;
            //edge2 = temp_of_unused[i].index;
        }
    }
    
    //Add two edges to current distance then check
    temp_distance += (minimum + minimum_two);
    if(temp_distance > upper_bound){
        return false;
    }
    
    unsigned int num_visited = 0;
    minimum = numeric_limits<double>::infinity();
    int current_index = 0;
    int save_index = 0;
    
    //Loop to calculate the MST of Remaining points from unused vertices
    temp_of_unused[current_index].vistited = true;
    temp_of_unused[current_index].distance = 0;
    ++num_visited;
    while(num_visited < temp_of_unused.size()){
        minimum = numeric_limits<double>::infinity();
        for(unsigned int i = 0; i < temp_of_unused.size(); ++i){
            if(temp_of_unused[i].vistited == false){
                distance_between = distance_matrix[temp_of_unused[current_index].index][temp_of_unused[i].index];
                //cout << distance_matrix[temp_of_unused[current_index].index][temp_of_unused[i].index] << endl;
                //cout << calc_euclidean_distance(temp_of_unused[current_index], temp_of_unused[i]) << endl << endl;
                if(distance_between < temp_of_unused[i].distance){
                    temp_of_unused[i].distance = distance_between;
                }
                if(temp_of_unused[i].distance < minimum){
                    save_index = i;
                    minimum = temp_of_unused[i].distance;
                }
            }
        }
        //cout << temp_of_unused[current_index].index << " " << temp_of_unused[current_index].preceding << endl;
        current_index = save_index;
        temp_of_unused[current_index].vistited = true;
        ++num_visited;
        
        //Add new MST piece to distance and check
        temp_distance += temp_of_unused[current_index].distance;
        if(temp_distance > upper_bound){
            return false;
        }
    }
    //cout << MST_distance << endl;
    //check_dis();
    //Add the MST and current distance together
    //exit(1);
    /*
    if(temp_distance > upper_bound){
        return false;
    }*/
    
    //Calculate shortest branch from back of path to MST and front of path to MST
    /*minimum = numeric_limits<double>::infinity();
    double minimum_two = numeric_limits<double>::infinity();
    double d_b_2 = numeric_limits<double>::infinity();
    //size_t edge1 = 0;
    //size_t edge2 = 0;
    for(size_t i = 0; i < temp_of_unused.size(); ++i){
        distance_between = distance_matrix[opt_path[permLength - 1].index][temp_of_unused[i].index];
        d_b_2 = distance_matrix[opt_path.front().index][temp_of_unused[i].index];
        if(distance_between < minimum){
            minimum = distance_between;
            //edge1 = temp_of_unused[i].index;
        }
        if(d_b_2 < minimum_two){
            minimum_two = d_b_2;
            //edge2 = temp_of_unused[i].index;
        }
    }*/
    /*cout << "EDGES:" << endl;
     cout << 0 << " " << edge1 << ": " << minimum << endl;
     cout << 0 << " " << edge2 << ": " << minimum_two << endl;*/
    //Add together the Path, MST, and two connection edges
    //temp_distance += (minimum + minimum_two);
    //cout << "Total: " << temp_distance << endl;
    
    /*if(temp_distance == numeric_limits<double>::infinity()){
     cout << "Current Path" << endl;
     for(size_t i = 0; i < opt_path.size(); ++i){
     cout << opt_path[i].index << " ";
     }
     cout << "Not Locked In Verticies" << endl;
     for(size_t i = 0; i < temp_of_unused.size(); ++i){
     cout << temp_of_unused[i].index << " ";
     }
     cout << endl;
     exit(1);
     }*/
    
    if(temp_distance > upper_bound){
        return false;
    }
    return true;
}

double Zoo_C::calculate_distance(){
    double temp = 0;
    if(permLength != 1){
        for(int i = 1; i < permLength; ++i){
            temp += distance_matrix[opt_path[i-1]][opt_path[i]];
        }
        temp += distance_matrix[opt_path[0]][opt_path.back()];
    }
    else{
        return 0;
    }
    return temp;
}

void Zoo_C::optimize_c(){
    //A partial solution is pruned if its cost >= cost of best known complete solution
    permLength = 1;
    //opt_path = TSP_UP_path;
    gen_perms();
}

void Zoo_C::print_output(){
    cout << std::setprecision(2);
    cout << std::fixed;
    cout << upper_bound << '\n';
    int j = 0;
    while(j < int(output_path.size())){
        cout << output_path[j] << " ";
        ++j;
    }
    cout << endl;
}

double Zoo_C::calc_euclidean_distance(const better& one, const better& two){
    double x = one.x - two.x;
    double y = one.y - two.y;
    return sqrt(((x) * (x)) + ((y) * (y)));
}

void Zoo_C::check_dis(){
    double dis_temp = 0;
    double dis = distance_matrix[0][6];
    cout << "Distane Between: " << 0 << " & " << 6 << ": " << dis << endl;
    
    dis_temp = distance_matrix[6][8];
    cout << "Distane Between: " << 6 << " & " << 8 << ": " << dis_temp << endl;
    dis += dis_temp;
    
    dis_temp = distance_matrix[8][4];
    cout << "Distane Between: " << 8 << " & " << 4 << ": " << dis_temp << endl;
    dis += dis_temp;
    
    dis_temp = distance_matrix[4][2];
    cout << "Distane Between: " << 4 << " & " << 2 << ": " << dis_temp << endl;
    dis += dis_temp;
    
    dis_temp = distance_matrix[2][1];
    cout << "Distane Between: " << 2 << " & " << 1 << ": " << dis_temp << endl;
    dis += dis_temp;
    
    dis_temp = distance_matrix[1][5];
    cout << "Distane Between: " << 1 << " & " << 5 << ": " << dis_temp << endl;
    dis += dis_temp;
    
    dis_temp = distance_matrix[5][7];
    cout << "Distane Between: " << 5 << " & " << 7 << ": " << dis_temp << endl;
    dis += dis_temp;
    
    dis_temp = distance_matrix[7][3];
    cout << "Distane Between: " << 7 << " & " << 3 << ": " << dis_temp << endl;
    dis += dis_temp;
    
    dis_temp = distance_matrix[3][0];
    cout << "Distane Between: " << 3 << " & " << 0 << ": " << dis_temp << endl;
    dis += dis_temp;
    
    
    cout << "Total: " << endl;
    cout << dis << endl;
}




