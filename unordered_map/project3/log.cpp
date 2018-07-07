//
//  log.cpp
//  project3
//
//  Created by Philip Ruffini on 5/28/18.
//  Copyright © 2018 Philip Ruffini. All rights reserved.
//

#include "log.h"

long Log::convert_timestamp_to_int(const string& timestamp){
    
    if(timestamp.size() != 14){
        cerr << "Invalid Timestamp" << "\n";
        return 0;
    }
    
    long time = ((timestamp[0] - '0') * 1000000000LL);
    time += ((timestamp[1] - '0') * 100000000LL);
    time += ((timestamp[3] - '0') * 10000000LL);
    time += ((timestamp[4] - '0') * 1000000LL);
    time += ((timestamp[6] - '0') * 100000LL);
    time += ((timestamp[7] - '0') * 10000LL);
    time += ((timestamp[9] - '0') * 1000LL);
    time += ((timestamp[10] - '0') * 100LL);
    time += ((timestamp[12] - '0') * 10LL);
    time += ((timestamp[13] - '0') * 1LL);
    
    return time;
}
//using .find to locate something in the map, if it isn't in the map it'll add it
//fixing if something isn't in the map

//Returns true if one is higher priority
bool in_order(const log_entry& one, const log_entry& two){
    if(one.time < two.time){
        return true;
    }
    else if(one.time == two.time){
        if(one.category < two.category){
            return true;
        }
        else if(one.category == two.category){
            if(one.entry_num < two.entry_num){
                return true;
            }
            else{
                return false;
            }
        }
        return false;
    }
    return false;
}
bool time_compare(const log_entry& one, const log_entry& two){
    if(one.time < two.time){
        return true;
    }
    else{
        return false;
    }
}

void Log::parse_keywords(const string& category_and_message, const int& num){
    int i = 0;
    int prev = 0;
    string storage;
    while(i < int(category_and_message.size())){
        while(isalnum(category_and_message[i])){
            ++i;
        }
        if(isalnum(category_and_message[i - 1]) && i != 0){
            if(prev == 0){
                storage = category_and_message.substr(prev, i);
            }
            else{
                storage = category_and_message.substr(prev, (i - prev));
            }
            if(database_words[storage].size() == 0){
                database_words[storage].push_back(num);
            }
            else if(database_words[storage].back() != num) {
                database_words[storage].push_back(num);
            }
        }
        prev = i + 1;
        ++i;
    }
}

void Log::load_database(ifstream& file){
    
    int entry_num = 0;
    log_entry storage;
    keywords_search = false;
    cat_search = false;
    append = false;
    
    while(getline(file, storage.timestamp, '|') &&  getline(file, storage.category, '|') && getline(file, storage.message)){
        
        storage.time = convert_timestamp_to_int(storage.timestamp);
        storage.entry_num = entry_num;
        main_database.push_back(storage);
        ++entry_num;
    }
    sort(main_database.begin(), main_database.end(), in_order);
    main_database_size = int(main_database.size());
    cout << main_database_size << " entries read" << "\n";
}

void Log::create_unordered_maps(const char& search_type){
    if(search_type == 'k'){
        int i = 0;
        keywords_search = true;
        database_words.reserve(main_database.size() * 2);
        if(append == false){
            append = true;
            while(i < main_database_size){
                entries[main_database[i].entry_num] = i;
                string category = main_database[i].category + " " + main_database[i].message;
                transform(category.begin(), category.end(), category.begin(), ::tolower);
                parse_keywords(category, i);
                ++i;
            }
        }
        else{
            while(i < main_database_size){
                string category = main_database[i].category + " " + main_database[i].message;
                transform(category.begin(), category.end(), category.begin(), ::tolower);
                parse_keywords(category, i);
                ++i;
            }
        }
    }
    else if(search_type == 'a'){
        int i = 0;
        append = true;
        entries.reserve(main_database_size);
        while(i < main_database_size){
            entries[main_database[i].entry_num] = i;
            ++i;
        }
    }
    else if(search_type == 'c'){
        int i = 0;
        cat_search = true;
        database_categories.reserve(main_database_size);
        if(append == false){
            append = true;
            while(i < main_database_size){
                entries[main_database[i].entry_num] = i;
                string category = main_database[i].category;
                transform(category.begin(), category.end(), category.begin(), ::tolower);
                database_categories[category].push_back(i);
                ++i;
            }
        }
        else{
            while(i < main_database_size){
                string category = main_database[i].category;
                transform(category.begin(), category.end(), category.begin(), ::tolower);
                database_categories[category].push_back(i);
                ++i;
            }
        }
    }
}


void Log::run_program(){
    
    string command;
    cout << "% ";
    getline(cin, command);
    while(command[0] != 'q'){
        if(int(command.size()) > 1 && command[1] != ' '){
            cerr << "Invalid Input: That Command does not exist. Enter a New Command." << "\n";
        }
        switch (command[0]){
            case 'm':
                prev_search = command[0];
                collect_information(command);
                break;
            case 't':
                prev_search = command[0];
                collect_information(command);
                break;
            case 'c':
                prev_search = command[0];
                collect_information(command);
                break;
            case 'k':
                prev_search = command[0];
                collect_information(command);
                break;
            case 'a':
                append_log_entry(command);
                break;
            case 'r':
                append_search_results();
                break;
            case 'd':
                delete_entry(command);
                break;
            case 'b':
                move_to_beginning(command);
                break;
            case 'e':
                move_to_end(command);
                break;
            case 'l':
                clear_excerpt_list();
                break;
            case 'g':
                print_recent_search();
                break;
            case 'p':
                print_excerptlist();
                break;
            case 's':
                sort_by_timestamp();
                break;
            case '#':
                break;
            default:
                cerr << "Invalid Input: That Command does not exist. Enter a New Command." << "\n";
                break;
        }
        cout << "% ";
        getline(cin, command);
    }
    
    
}

void Log::collect_information(const string& commandline){
    if(commandline[0] == 'm'){
        if(commandline.size() != 16){
            cerr << "Invalid Timestamp" << "\n";
            return;
        }
        if(append == false){
            create_unordered_maps('a');
        }
        match_time = convert_timestamp_to_int(commandline.substr(2,15));
        log_entry temp;
        temp.time = match_time;
        it_match = lower_bound(main_database.begin(), main_database.end(), temp, time_compare);
        if(it_match == main_database.end()){
            cout << "Timestamp search: " << 0 << " entries found" << "\n";
            match_time = -2;
            return;
        }
        auto temp_it = it_match;
        temp = *it_match;
        int count = 0;
        while(temp.time == match_time){
            ++temp_it;
            temp = *temp_it;
            ++count;
        }
        
        cout << "Timestamp search: " << count << " entries found" << "\n";
    }
    else if(commandline[0] == 't'){
        if(commandline.size() != 31){
            cerr << "Invalid Timestamp" << "\n";
            return;
        }
        if(append == false){
            create_unordered_maps('a');
        }
        
        //Find and Check Lowerbound
        time_low = convert_timestamp_to_int(commandline.substr(2,14));
        log_entry temp;
        temp.time = time_low;
        it_low = lower_bound(main_database.begin(), main_database.end(), temp, time_compare);
        if(it_low == main_database.end()){
            time_low = -2;
            cout << "Timestamps search: " << 0 << " entries found" << "\n";
            return;
        }
        
        //Find and Check Upperbound
        time_high = convert_timestamp_to_int(commandline.substr(17,14));
        temp.time = time_high;
        it_high = upper_bound(main_database.begin(), main_database.end(), temp, time_compare);
        
        if(it_high == it_low){
            time_low = -2;
            cout << "Timestamps search: " << 0 << " entries found" << "\n";
            return;
        }
        auto temp_it = it_low;
        int count = 0;
        while(temp_it != it_high){
            ++count;
            ++temp_it;
        }
        
        cout << "Timestamps search: " << count << " entries found" << "\n";
    }
    else if(commandline[0] == 'c'){
        if(cat_search == false){
            create_unordered_maps('c');
        }
        saved_category = commandline.substr(2);
        transform(saved_category.begin(), saved_category.end(), saved_category.begin(), ::tolower);
        if(database_categories.find(saved_category) == database_categories.end()){
            cout << "Category search: " << 0 << " entries found" << "\n";
            return;
        }
        cout << "Category search: " << database_categories[saved_category].size() << " entries found" << "\n";
    }
    else if(commandline[0] == 'k'){
        if(keywords_search == false){
            create_unordered_maps('k');
        }
        string keywords_one_string = commandline.substr(2);
        vector<string> keywords;
        deque<vector<int>> intersection;
        vector<int> results;
        vector<int>::iterator it;
        transform(keywords_one_string.begin(), keywords_one_string.end(), keywords_one_string.begin(), ::tolower);
        int i = 0;
        int prev = 0;
        
        while(i < int(keywords_one_string.size())){
            while(isalnum(keywords_one_string[i])){
                ++i;
            }
            if(isalnum(keywords_one_string[i - 1]) && i != 0){
                if(prev == 0){
                    keywords.push_back(keywords_one_string.substr(prev, i));
                }
                else{
                    keywords.push_back(keywords_one_string.substr(prev, (i - prev)));
                }
            }
            prev = i + 1;
            ++i;
        };
        i = 0;
        int j = 0;
        while(j < int(keywords.size())){
            if(database_words.find(keywords[j]) == database_words.end()){
                keyword_search_word_exists = false;
                cout << "Keyword search: " << 0 << " entries found" << "\n";
                return;
            }
            ++j;
        }
        keyword_search_word_exists = true;
        if(keywords.size() == 1){
            saved_keywords = database_words[keywords.front()];
            cout << "Keyword search: " << database_words[keywords.front()].size() << " entries found" << "\n";
        }
        else if(keywords.size() > 1){
            int k = 0;
            while(k < int(keywords.size())){
                intersection.push_back(database_words[keywords[k]]);
                ++k;
            }
            k = 0;
            while(int(intersection.size()) != 1){
                results.clear();
                set_intersection(intersection[k].begin(), intersection[k].end(),
                                 intersection[k + 1].begin(), intersection[k + 1].end(),
                                 back_inserter(results));
                intersection.push_back(results);
                intersection.pop_front();
                intersection.pop_front();
            }
            saved_keywords = intersection[0];
            cout << "Keyword search: " << intersection[0].size() << " entries found" << "\n";
        }
    }
}


void Log::append_log_entry(const string& commandline){
    //string entry = commandline.substr(2);
    int entry_num = stoi(commandline.substr(2));
    
    if(entry_num < 0 || entry_num > int(main_database_size - 1)){
        cerr << "Invalid Log Entry Number" << "\n";
        return;
    }
    if(append == false){
        create_unordered_maps('a');
    }
    
    excerpt_list.push_back(entries[entry_num]);
    
    cout << "log entry " << entry_num << " appended" << "\n";
}
//Change to can appened search results if 0
//Can appened 0 if 0 is search results. But NRSHO if there is no search
void Log::append_search_results(){
    int count = 0;
    if(prev_search == ' '){
        cerr << "No Recent Searches Have Occurred" << "\n";
        return;
    }
    if(prev_search == 'm'){
        if(match_time != -2){
            log_entry temp;
            temp = *it_match;
            int i = entries[temp.entry_num];
            while(main_database[i].time == match_time){
                excerpt_list.push_back(i);
                ++i;
                ++count;
            }
        }
    }
    else if(prev_search == 't'){
        if(time_low != -2){
            log_entry temp;
            temp = *it_low;
            int i = entries[temp.entry_num];
            temp = *it_high;
            int j = entries[temp.entry_num];
            while(i < j){
                excerpt_list.push_back(i);
                ++i;
                ++count;
            }
        }
    }
    else if(prev_search == 'c'){
        int i = 0;
        
        if(database_categories.find(saved_category) != database_categories.end()){
            while(i < int(database_categories[saved_category].size())){
                excerpt_list.push_back(database_categories[saved_category][i]);
                ++count;
                ++i;
            }
        }
    }
    else if(prev_search == 'k'){
        if(keyword_search_word_exists){
            int i = 0;
            while(i < int(saved_keywords.size())){
                excerpt_list.push_back(saved_keywords[i]);
                ++i;
                ++count;
            }
        }
    }
    
    cout << count << " log entries appended" << "\n";
}

void Log::delete_entry(const string& commandline){
    //string entry = commandline.substr(2);
    int excerpt_num = stoi(commandline.substr(2));
    
    if(excerpt_num < 0 || excerpt_num > int(excerpt_list.size() - 1)){
        cerr << "Invalid Log Entry Number" << "\n";
        return;
    }
    
    excerpt_list.erase(excerpt_list.begin() + excerpt_num);
    
    cout << "Deleted excerpt list entry " << excerpt_num << "\n";
    
}

void Log::move_to_beginning(const string& commandline){
    //string entry = commandline.substr(2);
    int excerpt_num = stoi(commandline.substr(2));
    
    if(excerpt_num < 0 || excerpt_num > int(excerpt_list.size() - 1)){
        cerr << "Invalid Log Entry Number" << "\n";
        return;
    }
    
    int save_num = excerpt_list[excerpt_num];
    excerpt_list.erase(excerpt_list.begin() + excerpt_num);
    excerpt_list.push_front(save_num);
    
    cout << "Moved excerpt list entry " << excerpt_num << "\n";
}

void Log::move_to_end(const string& commandline){
    //string entry = commandline.substr(2);
    int excerpt_num = stoi(commandline.substr(2));
    
    if(excerpt_num < 0 || excerpt_num > int(excerpt_list.size() - 1)){
        cerr << "Invalid Log Entry Number" << "\n";
        return;
    }
    
    int save_num = excerpt_list[excerpt_num];
    
    excerpt_list.erase(excerpt_list.begin() + excerpt_num);
    excerpt_list.push_back(save_num);
    
    cout << "Moved excerpt list entry " << excerpt_num << "\n";
}

void Log::sort_by_timestamp(){
    int end_num = int(excerpt_list.size() - 1);
    if(excerpt_list.size() == 0){
        cout << "excerpt list sorted" << "\n";
        cout << "(previously empty)" << "\n";
    }
    else{
        //Print Previous Info
        cout << "excerpt list sorted" << "\n";
        cout << "previous ordering:" << "\n";
        cout << "0|" << main_database[excerpt_list[0]].entry_num << "|" << main_database[(excerpt_list[0])].timestamp << "|";
        cout << main_database[(excerpt_list[0])].category << "|";
        cout << main_database[(excerpt_list[0])].message << "\n";
        cout << "..." << "\n";
        cout << end_num <<"|" << main_database[(excerpt_list[end_num])].entry_num << "|";
        cout << main_database[(excerpt_list[end_num])].timestamp << "|";
        cout << main_database[(excerpt_list[end_num])].category << "|";
        cout << main_database[(excerpt_list[end_num])].message << "\n";
        
        //Sort
        
        
        sort(excerpt_list.begin(), excerpt_list.end());
        
        cout << "new ordering:" << "\n";
        cout << "0|" << main_database[(excerpt_list[0])].entry_num << "|" << main_database[(excerpt_list[0])].timestamp << "|";
        cout << main_database[(excerpt_list[0])].category << "|";
        cout << main_database[(excerpt_list[0])].message << "\n";
        cout << "..." << "\n";
        cout << end_num <<"|" << main_database[(excerpt_list[end_num])].entry_num << "|";
        cout << main_database[(excerpt_list[end_num])].timestamp << "|";
        cout << main_database[(excerpt_list[end_num])].category << "|";
        cout << main_database[(excerpt_list[end_num])].message << "\n";
    }
}

void Log::clear_excerpt_list(){
    int end_num = int(excerpt_list.size() - 1);
    if(excerpt_list.size() == 0){
        cout << "excerpt list cleared" << "\n";
        cout << "(previously empty)" << "\n";
    }
    else{
        //Print Previous Info
        cout << "excerpt list cleared" << "\n";
        cout << "previous contents:" << "\n";
        cout << "0|" << main_database[(excerpt_list[0])].entry_num << "|" <<
        main_database[(excerpt_list[0])].timestamp << "|";
        cout << main_database[(excerpt_list[0])].category << "|";
        cout << main_database[(excerpt_list[0])].message << "\n";
        cout << "..." << "\n";
        cout << end_num <<"|" << main_database[(excerpt_list[end_num])].entry_num << "|";
        cout << main_database[(excerpt_list[end_num])].timestamp << "|";
        cout << main_database[(excerpt_list[end_num])].category << "|";
        cout << main_database[(excerpt_list[end_num])].message << "\n";
        excerpt_list.clear();
    }
}

void Log::print_recent_search(){
    int i = 0;
    if(prev_search == ' '){
        cerr << "No Recent Searches Have Occurred" << "\n";
        return;
    }
    if(prev_search == 'm'){
        log_entry temp;
        temp = *it_low;
        int i = entries[temp.entry_num];
        if(main_database[i].time == match_time){
            while(main_database[i].time != match_time){
                cout << main_database[i].entry_num << "|";
                cout << main_database[i].timestamp << "|";
                cout << main_database[i].category << "|";
                cout << main_database[i].message << "\n";
                ++i;
            }
        }
    }
    else if(prev_search == 't'){
        log_entry temp;
        temp = *it_low;
        int i = entries[temp.entry_num];
        temp = *it_high;
        int j = entries[temp.entry_num];
        while(i < j){
            cout << main_database[i].entry_num << "|";
            cout << main_database[i].timestamp << "|";
            cout << main_database[i].category << "|";
            cout << main_database[i].message << "\n";
            ++i;
        }
    }
    else if(prev_search == 'c'){
        if(database_categories.find(saved_category) != database_categories.end()){
            while(i < int(database_categories[saved_category].size())){
                cout << main_database[database_categories[saved_category][i]].entry_num << "|";
                cout << main_database[database_categories[saved_category][i]].timestamp << "|";
                cout << main_database[database_categories[saved_category][i]].category << "|";
                cout << main_database[database_categories[saved_category][i]].message << "\n";
                ++i;
            }
        }
    }
    else if(prev_search == 'k' && keyword_search_word_exists){
        while(i < int(saved_keywords.size())){
            cout << main_database[saved_keywords[i]].entry_num << "|";
            cout << main_database[saved_keywords[i]].timestamp << "|";
            cout << main_database[saved_keywords[i]].category << "|";
            cout << main_database[saved_keywords[i]].message << "\n";
            ++i;
        }
    }
}

void Log::print_excerptlist(){
    
    int count = 0;
    
    while(count < int(excerpt_list.size())){
        cout << count << "|";
        cout << main_database[(excerpt_list[count])].entry_num << "|";
        cout << main_database[(excerpt_list[count])].timestamp << "|";
        cout << main_database[(excerpt_list[count])].category << "|";
        cout << main_database[(excerpt_list[count])].message << "\n";
        ++count;
    }
}


void Log::print_database(){
    
    int count = 0;
    
    while(count < main_database_size){
        
        cout << main_database[count].timestamp << "|";
        cout << main_database[count].category << "|";
        cout << main_database[count].message << "\n";
        ++count;
    }
    
}






