#ifndef ARDUINO_HPP
#define ARDUINO_HPP

#include "message_time_pair.hpp"
#include "SerialClass.h"
#include "account.hpp"

#include <vector>
#include <memory>

// A class that handles communication with an arduino
class Arduino {
    private:
    std::unique_ptr<Serial> serial_port;
    std::vector<message_time_pair*> message_table;
    unsigned int current_message_index; // An index indicating the current message to be displayed

    public:
    Arduino(const char* port_name); // Constructor
    ~Arduino(); // Destructor
    void set_message_table(std::vector<std::unique_ptr<Account>>& account_list);
    void write(); // Display a message
    void increment_current_message_index();
    int get_current_time(); // Returns the display time of the current displayed message
    void shuffle_message_table(); // Shuffles the display order of the messages
    int get_current_message_index();
};

#endif // #ifndef ARDUINO_HPP