#include "arduino.hpp"
#include "SerialClass.h"

#include <math.h>
#include <iostream>
#include <memory>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

static char* concatinate_const_char_ptrs(const char* s1, const char* s2) {
    
    char *s = new char[strlen(s1)+strlen(s2)+1];
    strcpy(s,s1);
    strcat(s,s2);

    return s;
}

// Constructor
Arduino::Arduino(const char* port_name) {
    const char* port_name_concatinated = concatinate_const_char_ptrs("\\\\.\\", port_name);
    serial_port = make_unique<Serial>(port_name_concatinated);
    delete port_name_concatinated;
    current_message_index = 0;
}

// Destructor
Arduino::~Arduino() {
    for (message_time_pair *msp : message_table) {
        delete msp;
    }
}

void Arduino::set_message_table(vector<unique_ptr<Account>>& account_list) {
    
    // Get the total paid amount
    int total_amount = 0;
    for (unique_ptr<Account>& account : account_list) {
        total_amount += account->amount;
    }

    // Partion display time between the companies according to their percentage of the total amount 
    for (unique_ptr<Account>& account : account_list) {
        int time = round(60000.0 * (float)account->amount / (float)total_amount);
        message_table.push_back(new message_time_pair(account->message, time));
    }
}

// Display a message
void Arduino::write() {
    string& message = message_table[current_message_index]->message;
    
    char *char_array = new char[message.size() + 1];
    copy(message.begin(), message.end(), char_array);
    char_array[message.size()] = '\n';
    
    serial_port->WriteData(char_array, message.size());
    
    delete [] char_array;
}

void Arduino::increment_current_message_index() {
    if (current_message_index == message_table.size() - 1) {
        current_message_index = 0;
    }
    else {
        current_message_index++;
    }
}

// Returns the display time of the current displayed message
int Arduino::get_current_time() {
    return message_table[current_message_index]->time;
}

// Shuffles the display order of the messages
void Arduino::shuffle_message_table() {
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(message_table.begin(), message_table.end(), default_random_engine(seed));
}

int Arduino::get_current_message_index() {
    return current_message_index;
}