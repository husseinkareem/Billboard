#ifndef MESSAGE_TIME_PAIR_HPP
#define MESSAGE_TIME_PAIR_HPP

#include <string>

// A struct consisting of a message to be displayed and the amount of time (in milliseconds) it is to be displayed
struct message_time_pair{
    std::string message;
    unsigned short time;

    message_time_pair(std::string& message, unsigned short time);
};

#endif // #ifndef MESSAGE_TIME_PAIR