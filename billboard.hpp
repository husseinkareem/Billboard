#ifndef BILLBOARD_HPP
#define BILLBOARD_HPP

#include "account.hpp"
#include "arduino.hpp"

#include <stdbool.h>
#include <vector>


class Billboard {
    private:
    bool is_running; // Is true while the billboard is running. Is false otherwise.
    std::vector<std::unique_ptr<Account>> account_list;
    std::vector<std::unique_ptr<Arduino>> arduino_list; // List of connected arduinos

    void enter_accounts();
    void setup_arduinos();
    void loop_display();

    public:
    Billboard(std::vector<const char*>& port_name_list); // Contructor
    void run(); // Runs the billboard
    void save_accounts(std::string file_name);
    void load_accounts(std::string file_name);
};

#endif // #ifndef BILLBOARD_HPP