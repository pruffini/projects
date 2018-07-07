//
//  zoo_B.cpp
//  project4
//
//  Created by Philip Ruffini on 6/8/18.
//  Copyright © 2018 Philip Ruffini. All rights reserved.
//

#include "zoo_B.h"

void Zoo_B::fill_zoo_B(){
    
    //Initalizes Variables
    string line;
    cage_B temp;
    int cages_count = 0;
    
    //Read Num Cages
    cin >> number_of_cages;
    animal_cages_B.reserve(number_of_cages);
    
    //Fill the Zoo!
    while(int(cages_count) < number_of_cages){
        cin >> temp.x;
        cin >> temp.y;
        temp.vistited = false;
        animal_cages_B.push_back(temp);
        ++cages_count;
    }
}

void Zoo_B::run_B(){
    int min_distance_index = 0;
    int current_index = 0;
    //size_t save_index = 0;
    animal_cages_B[int(current_index)].vistited = true;
    final_path.reserve(number_of_cages);
    final_path.push_back(int(current_index));
    total_distance = 0;
    double edge_distance_total = 0;
    
    //first loop to find nearest neighbor
    saved_distance = numeric_limits<double>::infinity();
    distance_ij = numeric_limits<double>::infinity();
    distance_kj = numeric_limits<double>::infinity();
    distance_ik = numeric_limits<double>::infinity();
    for(int i = 1; i < int(animal_cages_B.size()); ++i){
        distance_ij = calc_euclidean_distance(animal_cages_B[int(current_index)], animal_cages_B[i]);
        if(distance_ij < saved_distance){
            saved_distance = distance_ij;
            min_distance_index = i;
        }
    }
    animal_cages_B[min_distance_index].vistited = true;
    final_path.push_back(min_distance_index);
    saved_distance = numeric_limits<double>::infinity();
    final_path.push_back(0);
    if(animal_cages_B[1].vistited == false){
        current_index = 1;
    }
    else{
        current_index = 2;
    }
    while(current_index != number_of_cages){
        //cout << "Current Path: "; print_output();
        if(animal_cages_B[current_index].vistited == false){
            for(int i = 1; i < int(final_path.size() - 1); ++i){
                distance_ij = calc_euclidean_distance(animal_cages_B[final_path[i - 1]], animal_cages_B[final_path[i]]);
                distance_ik = calc_euclidean_distance(animal_cages_B[current_index], animal_cages_B[final_path[i - 1]]);
                distance_kj = calc_euclidean_distance(animal_cages_B[current_index], animal_cages_B[final_path[i]]);
                edge_distance_total = (distance_ik + distance_kj - distance_ij);
                if(edge_distance_total < saved_distance){
                    min_distance_index = i;
                    saved_distance = edge_distance_total;
                }
            }
            auto it = final_path.begin() + int(min_distance_index);
            final_path.insert(it, current_index);
            saved_distance = numeric_limits<double>::infinity();
        }
        ++current_index;
    }
    
    final_path.pop_back();
    int t = 0;
    while(t < int(final_path.size() - 1)){
        total_distance += calc_euclidean_distance(animal_cages_B[final_path[t]], animal_cages_B[final_path[t + 1]]);
        ++t;
    }
    total_distance += calc_euclidean_distance(animal_cages_B[final_path[0]], animal_cages_B[final_path.back()]);
}

void Zoo_B::print_output(){
    cout << std::setprecision(2);
    cout << std::fixed;
    cout << total_distance << '\n';
    int j = 0;
    while(j < int(final_path.size())){
        cout << final_path[j] << " ";
        ++j;
    }
    cout << endl;
}

double Zoo_B::calc_euclidean_distance(const cage_B& one, const cage_B& two){
    double x = one.x - two.x;
    double y = one.y - two.y;
    return sqrt(((x) * (x)) + ((y) * (y)));
}

void Zoo_B::check_distance(){
    double dis_temp = 0;
    double dis = calc_euclidean_distance(animal_cages_B[0], animal_cages_B[1]);
    cout << "Distane Between: " << 0 << " & " << 1 << ": " << dis << endl;
    dis_temp = calc_euclidean_distance(animal_cages_B[1], animal_cages_B[3]);
    cout << "Distane Between: " << 1 << " & " << 3 << ": " << dis_temp << endl;
    dis += dis_temp;
    
    dis_temp = calc_euclidean_distance(animal_cages_B[3], animal_cages_B[4]);
    cout << "Distane Between: " << 3 << " & " << 4 << ": " << dis_temp << endl;
    dis += dis_temp;
    
    dis_temp = calc_euclidean_distance(animal_cages_B[4], animal_cages_B[2]);
    cout << "Distane Between: " << 4 << " & " << 2 << ": " << dis_temp << endl;
    dis += dis_temp;
    
    dis_temp = calc_euclidean_distance(animal_cages_B[2], animal_cages_B[0]);
    cout << "Distane Between: " << 2 << " & " << 0 << ": " << dis_temp << endl;
    dis += dis_temp;
    
    cout << dis << endl;

}











