#include "billboard.hpp"
#include <iostream>
#include <cstring>


using namespace std; 

int main(int argc, char **argv) {

    // Give an error if no serial ports were given
    if (argc < 2) {
        cout << "ERROR: No serial ports given" << endl;
        return -1;
    }

    // Make a vector of all the given port names
    vector<const char*> port_name_list;
    for (int i = 1; i < argc; i++) {
        port_name_list.push_back(argv[i]);
    }

    try {
        Billboard billboard(port_name_list); // Create an instance of the billboard
        billboard.run(); // Run the billboard
    }

    // Give an error if one of the given ports cannot be found
    catch (exception& e) {
        cerr << e.what() << endl;
        return -1;
    }

    return 0;
}