#ifndef PORT_NOT_FOUND_HPP
#define PORT_NOT_FOUND_HPP

#include <exception>
#include <stdexcept>

// An exception to be thrown if the user gives a non-existent port as an argument when starting the program
struct port_not_found : public std::exception {
   const char * what () const throw ();
};

#endif // #ifndef PORT_NOT_FOUND_HPP