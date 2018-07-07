//
// project3
// Philip Ruffini
//

#ifndef log_h
#define log_h

#endif /* log_h */

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
#include <deque>
#include <unordered_map>
#include <cctype>

using namespace std;

//

struct log_entry{
    string timestamp;
    long time;
    string category;
    string message;
    int entry_num;
};

class Log{
public:
    //loads database into unordered map
    void load_database(ifstream& file);
    
    //Used to Convert the timestamp
    long convert_timestamp_to_int(const string& timestamp);
    
    //Used for Testing
    void print_database();
    void print_excerptlist();
    
    //main interface for running program
    void run_program();
    
    //Used to search for matching timestamps
    void matching_search(const string& time);
    
    //Used for interval time searching
    void timestamp_search(const string& commandline);
    
    void category_search(const string& commandline);
    
    void keyword_search(const string& commandline);
    
    void append_log_entry(const string& commandline);
    
    void append_recent_searches();
    
    void append_search_results();
    
    void delete_entry(const string& commandline);
    
    void move_to_beginning(const string& commandline);
    void move_to_end(const string& commandline);
    
    void sort_by_timestamp();
    
    void clear_excerpt_list();
    
    void print_recent_search();
    
    void parse_keywords(const string& category_and_message, const int& num);
    
    void collect_information(const string& commandline);
    
    void create_unordered_maps(const char& search_type);
    
    
private:
    //Datastructures
    vector<log_entry> main_database;
    unordered_map<string, vector<int>> database_categories;
    unordered_map<string, vector<int>> database_words;
    unordered_map<int, int> entries;
    deque<int> excerpt_list;
    
    //Used to Check Past Searches
    char prev_search = ' ';
    
    //For Time Match Search
    vector<log_entry>::iterator it_match;
    long match_time = 0;
    
    //For Range Based Search
    vector<log_entry>::iterator it_low;
    vector<log_entry>::iterator it_high;
    long time_low = 0;
    long time_high = 0;
    
    //Category Search
    string saved_category;
    
    //Keyword Search
    vector<int> saved_keywords;
    bool keyword_search_word_exists = true;
    
    //Used to see if map was constructed
    bool keywords_search = false;
    bool cat_search = false;
    bool append = false;
    
    //Database size
    int main_database_size = 0;
};

