#include "port_not_found.hpp"

#include <exception>
#include <stdexcept>

const char * port_not_found::what () const throw () {
    return "ERROR: One or more of the given ports cannot be found.";
}