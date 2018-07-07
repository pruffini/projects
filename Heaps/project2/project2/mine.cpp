//
//  mine.cpp
//  project2
//
//  Created by Philip Ruffini on 5/10/18.
//  Copyright © 2018 Philip Ruffini. All rights reserved.
//

#include "mine.h"
#include "P2random.h"


Mine::Mine(bool verbose_in, bool statistics_in, bool median_in){
    verbose = verbose_in;
    statistics = statistics_in;
    median = median_in;
}

void Mine::print_mine(){
    cout << "Printing Mine" << "\n";
    unsigned int row = 0;
    unsigned int col = 0;
    
    while(row < mine.size()){
        col = 0;
        while(col < mine.size()){
            cout << setw(mine_size);
            cout << mine[row][col].rubble << " ";
            ++col;
        }
        ++row;
        cout << "\n";
    }
}

//PRint Priority Queue for Testing
void Mine::print_priorityqueue(){
    cout << "Printing Priority Queue" << "\n";
    cout << "size: " << priority_tiles.size() << "\n";
    while (!priority_tiles.empty()) {
        cout << "row: " << priority_tiles.top().row << " ";
        cout << "col: " << priority_tiles.top().col << " ";
        cout << "rubble: " << priority_tiles.top().rubble << " " << "\n";
        priority_tiles.pop();
    }
}

//PRint TNT Queue for Testing
void Mine::print_TNTqueue(){
    cout << "Printing TNT Queue" << "\n";
    cout << "size: " << TNT_tiles.size() << "\n";
    while (!TNT_tiles.empty()) {
        cout << "row: " << TNT_tiles.top().row << " ";
        cout << "col: " << TNT_tiles.top().col << " ";
        cout << "rubble: " << TNT_tiles.top().rubble << " " << "\n";
        TNT_tiles.pop();
    }
}

void Mine::create_mine(){
    
    string junk;
    //string to store integers;
    string read_in_rubble;
    //Used for columns
    int col = 0;
    int row = 0;
    int rubble_num;
    
    //read in type of mine
    cin >> type;
    
    if(type != 'M' && type != 'R'){
        cerr << "The Map type specified at the top of the txt file needs to be 'M' or 'R'" << "\n";
        exit(1);
    }
    
    //If the mine type read in is Map
    if(type == 'M'){
        //reads in Size of mine
        cin >> junk >> mine_size;
        
        //Reads in starting location
        cin >> junk >> start_row >> start_col;
        
        //Checks start Location
        if(!(0 <= start_row && start_row <= (mine_size - 1)) ||
           !(0 <= start_col && start_col <= (mine_size - 1))){
            cerr << "Invalid Starting Location in Txt File" << "\n";
            exit(1);
        }
        
        //Reads in the rest of the mine
        mine.resize(mine_size, vector<map>(mine_size));
        
        //reads in the mine
        
        
        while(getline(cin, read_in_rubble)){
            col = 0;
            while(cin >> rubble_num){
                mine[row][col].rubble = rubble_num;
                mine[row][col].visable = false;
                if(col == (mine_size - 1)){
                    col = 0;
                    ++row;
                }
                else{
                    ++col;
                }
            }
            
        }
    }
    
    //If the mine type read in is Psuedorandom mode
    else{
        stringstream ss;
        
        //reads in Size of mine
        cin >> junk >> mine_size;
        
        //Reads in starting location
        cin >> junk >> start_row >> start_col;
        
        //Reads in Seed
        cin >> junk >> seed;
        
        //Reads in max_rubble
        cin >> junk >> max_rubble;
        
        //Reads in TNT
        cin >> junk >> TNT;
        
        //Checks start Location
        if(!(0 <= start_row && start_row <= (mine_size - 1)) ||
           !(0 <= start_col && start_col <= (mine_size - 1))){
            cerr << "Invalid Starting Location in Txt File" << "\n";
            exit(1);
        }
        
        //Reads in the rest of the mine;
        mine.resize(mine_size, vector<map>(mine_size));
        
        unsigned int ms = unsigned(mine_size);
        
        P2random::PR_init(ss, ms, seed, max_rubble, TNT);
        
        col = 0;
        while(ss >> rubble_num){
            mine[row][col].rubble = rubble_num;
            mine[row][col].visable = false;
            if(col == (mine_size - 1)){
                col = 0;
                ++row;
            }
            else{
                ++col;
            }
        }
        
    }
}

//Used for Verbose Mode
void Mine::verbose_mode(const mine_square& square){
    //Example: "Cleared: 6 at [1,2]"
    cout << "Cleared: " << square.rubble << " at [" << square.row << "," << square.col << "]" << "\n";
}

//Used for Verbose mode with TNT
void Mine::verbose_mode_by_TNT(const mine_square& square){
    //Example: "Cleared by TNT: 7 at [4,3]"
    cout << "Cleared by TNT: " << square.rubble << " at [" << square.row << "," << square.col << "]" << "\n";
}

//Used to calculate median after first time
void Mine::calc_median(const mine_square& square){
    int rubble = mine[square.row][square.col].rubble;
    
    if(median_high.size() == 0 && median_low.size() == 0){
        median_low.push(rubble);
        median_num = rubble;
    }
    else if(median_high.size() == 0){
        median_high.push(rubble);
        if(median_high.top() < median_low.top()){
            int store = median_high.top();
            median_high.pop();
            median_high.push(median_low.top());
            median_low.pop();
            median_low.push(store);
        }
    }
    else if(rubble < median_num){
        median_low.push(rubble);
        if(median_low.size() - median_high.size() > 1){
            int store = median_low.top();
            median_low.pop();
            median_high.push(store);
        }
    }
    else if(rubble > median_num){
        median_high.push(rubble);
        if(median_high.size() - median_low.size() > 1){
            int store = median_high.top();
            median_high.pop();
            median_low.push(store);
        }
    }
    else{
        if(median_high.size() >= median_low.size()){
            median_low.push(rubble);
        }
        else{
            median_high.push(rubble);
        }
    }
    
    if((median_high.size() + median_low.size()) % 2 == 0){
        median_num = (double(median_high.top() + median_low.top()) / 2.00);
    }
    else{
        if(median_high.size() > median_low.size()){
            median_num = double(median_high.top());
        }
        else{
            median_num = double(median_low.top());
        }
    }
    cout << "Median difficulty of clearing rubble is: ";
    cout << fixed << setprecision(2);
    cout << median_num << "\n";
}

//Clears start
//TODO: Account for Starting Location being TNT
void Mine::clear_start(){
    
    mine_square start_square;
    start_square.rubble = mine[start_row][start_col].rubble;
    start_square.row = start_row;
    start_square.col = start_col;
    
    if(mine[start_row][start_col].rubble != -1 && mine[start_row][start_col].rubble != 0){
        ++num_tiles_cleared;
        rubble_cleared += start_square.rubble;
        if(verbose){
            verbose_mode(start_square);
        }
        if(median){
            calc_median(start_square);
        }
        if(statistics){
            collect_stats(start_square);
        }
        if(start_col == 0 || start_row == 0 || start_col == (int(mine_size) - 1) ||
           start_row == (int(mine_size) - 1)){
            escaped = true;
        }
        start_square.rubble = 0;
        mine[start_row][start_col].visable = true;
        new_visable_tiles(start_square);
    }
    else if(start_square.rubble == 0){
        if(start_col == 0 || start_row == 0 || start_col == (int(mine_size) - 1) ||
           start_row == (int(mine_size) - 1)){
            escaped = true;
        }
        mine[start_row][start_col].visable = true;
        new_visable_tiles(start_square);
    }
    else{
        if(verbose){
            //Example: "TNT explosion started at [3,2]!"
            cout << "TNT explosion started at [" << start_row << "," << start_col << "]!" << "\n";
        }
        if(statistics){
            collect_stats_TNT(start_square);
        }
        isTNT(start_square);
    }
}

void Mine::path(){
    while(escaped != true){
        if(priority_tiles.top().col == 0 || priority_tiles.top().row == 0 || priority_tiles.top().col == (int(mine_size) - 1) ||
           priority_tiles.top().row == (int(mine_size) - 1)){
            escaped = true;
        }
        mine_square next_tile;
        next_tile.rubble = priority_tiles.top().rubble;
        next_tile.row = priority_tiles.top().row;
        next_tile.col = priority_tiles.top().col;
        clear_tile(next_tile);
    }
}

void Mine::generic_output(){
    //Example: Cleared 6 tiles containing 41 rubble and escaped.
    cout << "Cleared " << num_tiles_cleared << " tiles containing " << rubble_cleared;
    cout << " rubble and escaped." << "\n";
}

//Clears Tile
void Mine::clear_tile(mine_square& current_tile){
    priority_tiles.pop();
    current_tile.rubble = mine[current_tile.row][current_tile.col].rubble;
    if(current_tile.rubble != -1 && current_tile.rubble != 0){
        if(verbose){
            verbose_mode(current_tile);
        }
        if(median){
            calc_median(current_tile);
        }
        if(statistics){
            collect_stats(current_tile);
        }
        rubble_cleared += mine[current_tile.row][current_tile.col].rubble;
        ++num_tiles_cleared;
        mine[current_tile.row][current_tile.col].rubble = 0;
        mine[current_tile.row][current_tile.col].visable = true;
        new_visable_tiles(current_tile);
    }
    else if(current_tile.rubble == 0){
        mine[current_tile.row][current_tile.col].visable = true;
        new_visable_tiles(current_tile);
    }
    else{
        if(verbose){
            //Example: "TNT explosion started at [3,2]!"
            cout << "TNT explosion started at [" << current_tile.row << "," << current_tile.col << "]!" << "\n";
        }
        if(statistics){
            collect_stats_TNT(current_tile);
        }
        isTNT(current_tile);
    }
}

//New 4 visable Tiles
void Mine::new_visable_tiles(mine_square& square){
    
    if(check_north(square)){
        if(!mine[square.row - 1][square.col].visable){
            mine[square.row - 1][square.col].visable = true;
            mine_square North;
            North.rubble = mine[square.row - 1][square.col].rubble;
            North.row = square.row - 1;
            North.col = square.col;
            priority_tiles.push(North);
        }
    }
    if(check_south(square)){
        if(!mine[square.row + 1][square.col].visable){
            mine[square.row + 1][square.col].visable = true;
            mine_square South;
            South.rubble = mine[square.row + 1][square.col].rubble;
            South.row = square.row + 1;
            South.col = square.col;
            priority_tiles.push(South);
        }
    }
    if(check_east(square)){
        if(!mine[square.row][square.col + 1].visable){
            mine[square.row][square.col + 1].visable = true;
            mine_square East;
            East.rubble = mine[square.row][square.col + 1].rubble;
            East.row = square.row;
            East.col = square.col + 1;
            priority_tiles.push(East);
        }
    }
    if(check_west(square)){
        if(!mine[square.row][square.col - 1].visable){
            mine[square.row][square.col - 1].visable = true;
            mine_square West;
            West.rubble = mine[square.row][square.col - 1].rubble;
            West.row = square.row;
            West.col = square.col - 1;
            priority_tiles.push(West);
        }
    }
}

void Mine::detonateTNT(mine_square& square){
    
    if(check_north(square)){
        mine_square North;
        North.rubble = mine[square.row - 1][square.col].rubble;
        North.row = square.row - 1;
        North.col = square.col;
        priority_tiles.push(North);
        if(mine[square.row - 1][square.col].rubble >= 0){
            mine[square.row - 1][square.col].visable = true;
            TNT_tiles.push(North);
        }
        else if(mine[square.row - 1][square.col].rubble == -1){
            mine[square.row - 1][square.col].visable = true;
            mine[square.row - 1][square.col].rubble = -2;
            TNT_tiles.push(North);
        }
    }
    if(check_south(square)){
        mine_square South;
        South.rubble = mine[square.row + 1][square.col].rubble;
        South.row = square.row + 1;
        South.col = square.col;
        priority_tiles.push(South);
        if(mine[square.row + 1][square.col].rubble >= 0){
            mine[square.row + 1][square.col].visable = true;
            TNT_tiles.push(South);
        }
        else if(mine[square.row + 1][square.col].rubble == -1){
            mine[square.row + 1][square.col].visable = true;
            mine[square.row + 1][square.col].rubble = -2;
            TNT_tiles.push(South);
        }
    }
    if(check_east(square)){
        mine_square East;
        East.rubble = mine[square.row][square.col + 1].rubble;
        East.row = square.row;
        East.col = square.col + 1;
        priority_tiles.push(East);
        if(mine[square.row][square.col + 1].rubble >= 0){
            mine[square.row][square.col + 1].visable = true;
            TNT_tiles.push(East);
        }
        else if(mine[square.row][square.col + 1].rubble == -1){
            mine[square.row][square.col + 1].visable = true;
            mine[square.row][square.col + 1].rubble = -2;
            TNT_tiles.push(East);
        }
    }
    if(check_west(square)){
        mine_square West;
        West.rubble = mine[square.row][square.col - 1].rubble;
        West.row = square.row;
        West.col = square.col - 1;
        priority_tiles.push(West);
        if(mine[square.row][square.col - 1].rubble >= 0){
            mine[square.row][square.col - 1].visable = true;
            TNT_tiles.push(West);
        }
        else if(mine[square.row][square.col - 1].rubble == -1){
            mine[square.row][square.col - 1].visable = true;
            mine[square.row][square.col - 1].rubble = -2;
            TNT_tiles.push(West);
        }
    }
}

void Mine::isTNT(mine_square& square){
    
    mine[square.row][square.col].visable = true;
    mine[square.row][square.col].rubble = 0;
    
    detonateTNT(square);
    
    while(!TNT_tiles.empty()){
        mine_square top_of_tnt;
        top_of_tnt.col = TNT_tiles.top().col;
        top_of_tnt.row = TNT_tiles.top().row;
        top_of_tnt.rubble = mine[top_of_tnt.row][top_of_tnt.col].rubble;
        if(TNT_tiles.top().rubble == -1){
            if(statistics){
                collect_stats_TNT(top_of_tnt);
            }
            mine[top_of_tnt.row][top_of_tnt.col].rubble = 0;
            TNT_tiles.pop();
            detonateTNT(top_of_tnt);
            new_visable_tiles(top_of_tnt);
        }
        
        else{
            if(mine[top_of_tnt.row][top_of_tnt.col].rubble > 0){
                if(verbose){
                    verbose_mode_by_TNT(top_of_tnt);
                }
                if(median){
                    calc_median(top_of_tnt);
                }
                if(statistics){
                    collect_stats(top_of_tnt);
                }
                ++num_tiles_cleared;
                rubble_cleared += mine[top_of_tnt.row][top_of_tnt.col].rubble;
            }
            mine[top_of_tnt.row][top_of_tnt.col].rubble = 0;
            top_of_tnt.rubble = 0;
            priority_tiles.push(top_of_tnt);
            TNT_tiles.pop();
        }
    }
}
//Checks West
bool Mine::check_west(const mine_square& square){
    if((int(square.col) - 1) >= 0){
        return true;
    }
    return false;
}

//Checks East
bool Mine::check_east(const mine_square& square){
    if((int(square.col) + 1) <= (int(mine_size) - 1)){
        return true;
    }
    return false;
}

//Checks North
bool Mine::check_north(const mine_square& square){
    if((int(square.row) - 1) >= 0){
        return true;
    }
    return false;
}

//Checks South
bool Mine::check_south(const mine_square& square){
    if((int(square.row) + 1) <= (int(mine_size) - 1)){
        return true;
    }
    return false;
}

//Sets Stats_N
void Mine::get_stats_N(int& N){
    stats_N = N;
}

void Mine::collect_stats(const mine_square& square){
    stats_square infomration;
    infomration.row = square.row;
    infomration.col = square.col;
    infomration.rubble = square.rubble;
    infomration.TNT = false;
    infomration.order = counter;
    ++counter;
    stats_squares.push_back(infomration);
}

void Mine::collect_stats_TNT(const mine_square& square){
    stats_square infomration;
    infomration.row = square.row;
    infomration.col = square.col;
    infomration.rubble = -1;
    infomration.TNT = true;
    infomration.order = counter;
    ++counter;
    stats_squares.push_back(infomration);
}

//Returns true if current is lower rubble
bool stats_lowest_rubble(const stats_square& current, const stats_square& potential){
    
    if(current.rubble == potential.rubble){
        if(current.col != potential.col){
            return current.col < potential.col;
        }
        else{
            return current.row < potential.row;
        }
    }
    
    return current.rubble < potential.rubble;
}


//Returns true if current is lower rubble
bool stats_highest_rubble(const stats_square& current, const stats_square& potential){
    
    if(current.rubble == potential.rubble){
        if(current.col != potential.col){
            return current.col > potential.col;
        }
        else{
            return current.row > potential.row;
        }
    }
    
    return current.rubble > potential.rubble;
}

//Returns true if current is lower rubble
bool order_occured(const stats_square& current, const stats_square& potential){
    
    return current.order < potential.order;
}

//Returns true if current is lower rubble
bool same(const stats_square& current, const stats_square& potential){
    
    return (!(current.row == potential.row) && (current.col == potential.col));
}

void Mine::stats_output(){
    
    cout << "First tiles cleared: " << "\n";
    int N = int(stats_N);
    int stats_size = int(stats_squares.size());
    int i = 0;
    while(i < N && i < stats_size){
        if(stats_squares[i].TNT == false){
            cout << stats_squares[i].rubble << " at [" << stats_squares[i].row << "," << stats_squares[i].col;
            cout << "]" << "\n";
        }
        else{
            cout << "TNT" << " at [" << stats_squares[i].row << "," << stats_squares[i].col << "]" << "\n";
        }
        ++i;
    }
    
    cout << "Last tiles cleared: " << "\n";
    i = int(stats_squares.size() - 1);
    int count = 0;
    while(count < N && count < stats_size){
        if(stats_squares[i].TNT == false){
            cout << stats_squares[i].rubble << " at [" << stats_squares[i].row << "," << stats_squares[i].col;
            cout << "]" << "\n";
        }
        else{
            cout << "TNT" << " at [" << stats_squares[i].row << "," << stats_squares[i].col << "]" << "\n";
        }
        --i;
        ++count;
    }
    
    cout << "Easiest tiles cleared: " << "\n";
    sort(stats_squares.begin(),stats_squares.end(), stats_lowest_rubble);
    i = 0;
    while(i < N && i < stats_size){
        if(stats_squares[i].TNT == false){
            cout << stats_squares[i].rubble << " at [" << stats_squares[i].row << "," << stats_squares[i].col;
            cout << "]" << "\n";
        }
        else{
            cout << "TNT" << " at [" << stats_squares[i].row << "," << stats_squares[i].col << "]" << "\n";
        }
        ++i;
    }
    
    cout << "Hardest tiles cleared: " << "\n";
    sort(stats_squares.begin(),stats_squares.end(), stats_highest_rubble);
    i = 0;
    while(i < N && i < stats_size){
        if(stats_squares[i].TNT == false){
            cout << stats_squares[i].rubble << " at [" << stats_squares[i].row << "," << stats_squares[i].col;
            cout << "]" << "\n";
        }
        else{
            cout << "TNT" << " at [" << stats_squares[i].row << "," << stats_squares[i].col << "]" << "\n";
        }
        ++i;
    }
}






