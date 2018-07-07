//
//  mine.hpp
//  project2
//
//  Created by Philip Ruffini on 5/10/18.
//  Copyright © 2018 Philip Ruffini. All rights reserved.
//

#ifndef mine_hpp
#define mine_hpp

#endif /* mine_hpp */

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <getopt.h>
#include <queue>

using namespace std;


//Struct Declaration for Mine Squares
struct mine_square{
    int rubble;
    int row;
    int col;
};

struct map{
    int rubble;
    bool visable;
};

//Struct Declaration for Mine Squares used in Statistics Mode
struct stats_square{
    int rubble;
    bool TNT;
    int order;
    int row;
    int col;
};

struct prioritysquare{
    
    //Returns true if current is higher priority
    bool operator()(const mine_square& current, const mine_square& potential){
        
        if(current.rubble == -1 && potential.rubble != -1){
            return false;
        }
        else if(current.rubble != -1 && potential.rubble == -1){
            return true;
        }
        else if(current.rubble == potential.rubble){
            if(current.col != potential.col){
                return current.col > potential.col;
            }
            else{
                return current.row > potential.row;
            }
        }
        else{
            return current.rubble > potential.rubble;
        }
    }
};

class Mine{
public:
    
    //Constructor
    Mine(bool verbose_in, bool statistics_in, bool median_in);
    
    //creates mine
    void create_mine();
    
    //Prints Mine (used for error checking)
    void print_mine();
    
    //Clears starting sqaure
    void clear_start();
    
    //executes TNT explosion
    void isTNT(mine_square& square);
    
    void detonateTNT(mine_square& square);
    
    //Clears tile
    void clear_tile(mine_square& square);
    
    //Check other Spaces (directions)
    bool check_west(const mine_square& square);
    bool check_east(const mine_square& square);
    bool check_south(const mine_square& square);
    bool check_north(const mine_square& square);
    
    //Prints Priority Queue used for testing
    void print_priorityqueue();
    
    //Print TNT Queue
    void print_TNTqueue();
    
    //Adds tiles to queue
    void new_visable_tiles(mine_square& square);
    
    //Path to Exit
    void path();
    
    //Priority Queues
    //For Visable blocks
    priority_queue<mine_square, vector<mine_square>, prioritysquare> priority_tiles;
    
    //For TNT
    priority_queue<mine_square, vector<mine_square>, prioritysquare> TNT_tiles;
    
    //Verbose Mode output
    void verbose_mode(const mine_square& square);
    
    //Verbose Mode output
    void verbose_mode_by_TNT(const mine_square& square);
    
    //Generic Output
    void generic_output();
    
    //Calculates the Median after first time
    void calc_median(const mine_square& square);
    
    //sets stats_N
    void get_stats_N(int& N);
    
    //Collects the stats
    void collect_stats(const mine_square& square);
    
    //Collects the stats after TNT
    void collect_stats_TNT(const mine_square& square);
    
    //Output for Statistics Mode
    void stats_output();
    
private:
    //For mine
    char type;
    int mine_size;
    int start_row;
    int start_col;
    vector<vector<map>> mine;
    bool escaped = false;
    int num_tiles_cleared = 0;
    int rubble_cleared = 0;
    
    //For Psuedomode
    unsigned seed;
    unsigned max_rubble;
    unsigned TNT;
    
    //Modes
    bool verbose = false;
    bool median = false;
    bool statistics = false;
    unsigned int stats_N = 0;
    
    //Used to calculate median
    double median_num;
    priority_queue<int, vector<int>> median_low;
    priority_queue<int, vector<int>, greater<int>> median_high;
    
    //Used to calculate stats
    vector<stats_square> stats_squares;
    int counter;
};

