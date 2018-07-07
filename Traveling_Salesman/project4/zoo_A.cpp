//
//  zoo.cpp
//  project4
//
//  Created by Philip Ruffini on 6/7/18.
//  Copyright © 2018 Philip Ruffini. All rights reserved.
//

#include "zoo_A.h"


void Zoo_A::fill_zoo_A(){
    //Initalizes Variables
    string line;
    cage_A temp;
    int cages_count = 0;
    total_distance = 0;
    int count_wild = 0;
    
    //Read Num Cages
    cin >> number_of_cages;
    animal_cages_A.reserve(number_of_cages);
    
    //Fill the Zoo!
    while(cages_count < number_of_cages){
        cin >> temp.x;
        cin >> temp.y;

        temp.vistited = false;
        temp.distance = numeric_limits<double>::infinity();
        if((temp.x == 0 && temp.y < 0) || (temp.x < 0 && temp.y == 0) || (temp.x == 0 && temp.y == 0)){
            cage_on_border = true;
            temp.type = 'b';
        }
        else if(temp.x < 0 && temp.y < 0){
            ++count_wild;
            cage_in_wild = true;
            temp.type = 'w';
        }
        else{
            temp.type = 'n';
        }
        animal_cages_A.push_back(temp);
        ++cages_count;
    }
    
    //Check for all wild - that works
    if(cage_in_wild == true && cage_on_border == false && cages_count != count_wild){
        cerr << "Cannot construct MST" << endl;
        exit(1);
    }
}

void Zoo_A::run_A(){
    int index = 0;
    animal_cages_A[index].distance = 0;
    animal_cages_A[index].vistited = true;
    number_visited = 0;
    ++number_visited;
    double distance_between;
    double minimum = numeric_limits<double>::infinity();
    
    //Algorithm to Calculate Path
    while(number_visited < int(number_of_cages)){
        int i = 0;
        while(i < int(animal_cages_A.size())){
            switch(animal_cages_A[index].type){
                case 'n':
                    if(animal_cages_A[i].vistited == false && (animal_cages_A[i].type != 'w')){
                        distance_between = calc_euclidean_distance(animal_cages_A[index], animal_cages_A[i]);
                        if(distance_between < animal_cages_A[i].distance){
                            animal_cages_A[i].preceding_cage_number = int(index);
                            animal_cages_A[i].distance = distance_between;
                        }
                    }
                    break;
                    
                case 'b':
                    if(animal_cages_A[i].vistited == false){
                        distance_between = calc_euclidean_distance(animal_cages_A[index], animal_cages_A[i]);
                        if(distance_between < animal_cages_A[i].distance){
                            animal_cages_A[i].preceding_cage_number = int(index);
                            animal_cages_A[i].distance = distance_between;
                        }
                    }
                    break;
                    
                default:
                    if(animal_cages_A[i].vistited == false && (animal_cages_A[i].type != 'n')){
                            distance_between = calc_euclidean_distance(animal_cages_A[index], animal_cages_A[i]);
                            if(distance_between < animal_cages_A[i].distance){
                                animal_cages_A[i].preceding_cage_number = int(index);
                                animal_cages_A[i].distance = distance_between;
                            }
                    }
                    break;
            }
            ++i;
        }
        minimum = numeric_limits<double>::infinity();
        for(int i = 0; i < int(animal_cages_A.size()); ++i){
            if(animal_cages_A[i].vistited == false){
                if(animal_cages_A[i].distance < minimum){
                    index = i;
                    minimum = animal_cages_A[i].distance;
                }
            }
        }
        animal_cages_A[index].vistited = true;
        ++number_visited;
        results not_in_anyway_temp;
        not_in_anyway_temp.a = int(index);
        not_in_anyway_temp.b = animal_cages_A[index].preceding_cage_number;
        final_path.push_back(not_in_anyway_temp);
        total_distance = total_distance + animal_cages_A[index].distance;
    }
}

void Zoo_A::print_output(){

    cout << std::setprecision(2);
    cout << std::fixed;
    cout << total_distance << '\n';
    int j = 0;
    while(j < int(final_path.size())){
        if(final_path[j].a < final_path[j].b){
            cout << final_path[j].a << " " << final_path[j].b << '\n';
        }
        else{
            cout << final_path[j].b << " " << final_path[j].a << '\n';
        }
        ++j;
    }
    
}

void Zoo_A::print_zoo(){
    int i = 0;
    while(i < int(animal_cages_A.size())){
        cout << "Cage Number: " << i << " Coordinates: ";
        cout << "(" << animal_cages_A[i].x << ", " << animal_cages_A[i].y << ")" << endl;
        ++i;
    }
    cout << endl;
}

double Zoo_A::calc_euclidean_distance(const cage_A& one, const cage_A& two){
    double x = one.x - two.x;
    double y = one.y - two.y;
    return sqrt(((x) * (x)) + ((y) * (y)));
}


