#include "billboard.hpp"
#include "SerialClass.h"
#include "account.hpp"
#include "arduino.hpp"

#include <iostream>
#include <string>
#include <limits>
#include <fstream>

using namespace std;


static void clear_cin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static int get_index_of_lowest(vector<int> &int_vector) {
    int lowest = int_vector[0];
    int index_lowest = 0;

    for (unsigned int i = 1; i < int_vector.size(); i++) {
        int num = int_vector[i];

        if (num < lowest) {
            lowest = num;
            index_lowest = i;
        }
    }

    return index_lowest;
}

void Billboard::enter_accounts() {
    
    bool entering_accounts = true; // Is set to false when the user is done entering accounts.

    while(entering_accounts) {
        cout << endl;

        string company_name;
        int amount;
        string message;

        // Enter company name
        cout << "Enter company name: "; 
        getline(cin, company_name);

        // Enter amount, give an error if the given amount is invalid
        while (true) {
            cout << "Enter amount: ";
            cin >> amount;
            clear_cin();

            if(cin.fail() || amount <= 0) {
                cout << "The amount has to be a positive integer." << endl;
            }
            else {
                break;
            }
        }
        
        // Enter message
        cout << "Enter message: "; 
        getline(cin, message);

        // Put the account in the account list
        account_list.push_back(make_unique<Account>(company_name, amount, message));

        // Stop after 5 accounts have been entered
        if (account_list.size() == 5) {
            entering_accounts = false;
        }

        // Allows the user to enter more accounts if he/she wants to
        bool valid_choice = false;
        while (!valid_choice && entering_accounts) {
            char choice;
            cout << "Do you want to enter more accounts? You can enter " << 5 - account_list.size() << " more accounts. (y/n): ";
            cin >> choice;
            clear_cin();
            
            if (choice == 'y') {
                valid_choice = true;
                
            }
            else if (choice == 'n') {
                entering_accounts = false;
                valid_choice = true;
            }
            else {
                cout << "Invalid choice!" << endl;
            }
        }
    }
}

void Billboard::setup_arduinos() {
    for (unique_ptr<Arduino>& arduino : arduino_list) {
        arduino->set_message_table(account_list);
    }
}

void Billboard::loop_display() {
    
    // For each arduino, randomize the message order
    for (unique_ptr<Arduino>& arduino: arduino_list) {
        arduino->shuffle_message_table();
        Sleep(100);
    }
    
    // Make a list of countdown-timers, one timer for each arduino
    vector<int> counter_list;
    for (unique_ptr<Arduino>& arduino : arduino_list) {
        counter_list.push_back(arduino->get_current_time());
    }
    
    // Make each arduino display their first message
    for (unique_ptr<Arduino>& arduino : arduino_list) {
            arduino->write();
        }

    // Loop each of the arduino displays
    while (true) {
        int index_lowest_counter = get_index_of_lowest(counter_list);
        int lowest_counter = counter_list[index_lowest_counter];
        Sleep(lowest_counter); // Pause the program until one of the arduinos needs to switch message

        // Count down the timers
        for (int& counter: counter_list) {
            counter -= lowest_counter;
        }
        
        // The arduino to be updated
        unique_ptr<Arduino>& arduino_to_update = arduino_list[index_lowest_counter];

        arduino_to_update->increment_current_message_index();

        // If the arduino has completed a full lap on its message table, reshuffle the order of the messages
        if (arduino_to_update->get_current_message_index() == 0) {
            arduino_to_update->shuffle_message_table();
        }

        // Write the next message on the display
        arduino_to_update->write();

        // Reset the arduino's countdown-timer
        counter_list[index_lowest_counter] = arduino_to_update->get_current_time();
    }
}

// Billboard constructor
Billboard::Billboard(std::vector<const char*> &port_name_list) {
    for (const char* port_name : port_name_list) {
        arduino_list.push_back(make_unique<Arduino>(port_name));
    }
}

//Runs the billboard 
void Billboard::run() {

    // Show options
    cout << endl;
    cout << "1: Enter new accounts" << endl;
    cout << "2: Load saved accounts" << endl;
    cout << "3: Exit" << endl;
    cout << endl;

    // Execute option, give an error if an invalid option has been given
    int option;
    bool valid_option = false;
    while (!valid_option) {
    
        cout << "Choose option: ";
        cin >> option;
        clear_cin();

        switch (option) {
            case 1:
            enter_accounts();
            valid_option = true;
            break;
            
            case 2:
            load_accounts("saved_accounts.txt");
            valid_option = true;
            break;
            
            case 3:
            cout << "Ending the program." << endl;
            return;

            default:
            cout << "ERROR: Invalid option." << endl;
        }
    } 

    save_accounts("saved_accounts.txt");
    setup_arduinos();
    loop_display();
}

void Billboard::save_accounts(string file_name) {

    // The file to save to
    ofstream file (file_name);

    // Enter information of each account
    for (std::unique_ptr<Account>& account : account_list) {
        file << endl;
        file << account->company_name << endl;
        file << account->amount << endl;
        file << account->message << endl;
    }

    file.close();
}

void Billboard::load_accounts(string file_name) {

    // The file to load from
    ifstream file (file_name);

    string tmp;
    
    string company_name;
    string amount_str;
    string message;

    while( getline(file, tmp) ) {
        getline(file, company_name);
        getline(file, amount_str);
        getline(file, message);

        account_list.push_back(make_unique<Account>(company_name, stoi(amount_str), message));
    }

    file.close();
}